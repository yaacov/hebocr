
/***************************************************************************
 *            ho_recognize_nikud.c
 *
 *  Fri Aug 12 20:13:33 2005
 *  Copyright  2005-2007  Yaacov Zamir
 *  <kzamir@walla.co.il>
 ****************************************************************************/

/*  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#ifndef TRUE
#define TRUE -1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL ((void*)0)
#endif

#include "ho_bitmap.h"
#include "ho_objmap.h"
#include "ho_segment.h"
#include "ho_font.h"

#include "ho_recognize_nikud.h"

int
ho_recognize_nikud_array_in_size ()
{
  return HO_NIKUD_ARRAY_IN_SIZE;
}

int
ho_recognize_nikud_array_out_size ()
{
  return HO_NIKUD_ARRAY_OUT_SIZE;
}

int
ho_recognize_nikud_dimentions (const ho_bitmap * m_text,
  const ho_bitmap * m_mask, double *height,
  double *width, double *top, double *bottom,
  double *top_left, double *top_mid, double *top_right,
  double *mid_left, double *mid_right,
  double *bottom_left, double *bottom_mid,
  double *bottom_right,
  double *dots_above, double *dots_below, double *dots_inside,
  double *objs_above, double *objs_below, double *objs_inside,
  double *main_obj_height, double *main_obj_width,
  double *top_dot_x, double *inside_dot_x, double *font_width_by_height)
{
  int i, x, y;
  int obj_width, obj_height;
  int obj_x, obj_y;
  int obj_w, obj_h;
  int line_start, line_end, line_height;
  int sum, font_start_x, font_end_x, font_height, font_width;
  int font_start_y, font_end_y;
  ho_objmap *o_obj = NULL;
  unsigned char is_dot, is_obj;

  /* init values to zero */
  *height = 0.0;
  *width = 0.0;
  *top = 0.0;
  *bottom = 0.0;
  *top_left = 0.0;
  *top_mid = 0.0;
  *top_right = 0.0;
  *mid_left = 0.0;
  *mid_right = 0.0;
  *bottom_left = 0.0;
  *bottom_mid = 0.0;
  *bottom_right = 0.0;
  *dots_above = 0.0;
  *dots_below = 0.0;
  *dots_inside = 0.0;
  *objs_above = 0.0;
  *objs_below = 0.0;
  *objs_inside = 0.0;
  *top_dot_x = 0.0;
  *inside_dot_x = 0.0;
  *font_width_by_height = 0.0;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++) ;
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++) ;
  line_end = y;
  line_height = line_end - line_start;

  if (line_height < 4 || m_text->width < 4)
    return TRUE;

  *font_width_by_height = (double) m_mask->width / (double) line_height;

  /* get all the objects of nikud */
  o_obj = ho_objmap_new_from_bitmap (m_text);
  if (!o_obj)
    return TRUE;

  /* if no nikud just return */
  if (ho_objmap_get_size (o_obj) == 0)
  {
    ho_objmap_free (o_obj);

    return FALSE;
  }

  /* count points and objects above/in/below font */
  obj_x = 0;
  obj_y = 0;
  obj_w = 0;
  obj_h = 0;

  for (i = 0; i < ho_objmap_get_size (o_obj); i++)
  {
    /* is this object inside line ? */
    y = ho_objmap_get_object (o_obj, i).y;
    x = ho_objmap_get_object (o_obj, i).x;
    obj_width = ho_objmap_get_object (o_obj, i).width;
    obj_height = ho_objmap_get_object (o_obj, i).height;

    is_dot = (obj_height < line_height / 3 && obj_height > line_height / 16
      && obj_width < line_height / 3 && obj_width > line_height / 16);

    is_obj = (obj_height > line_height / 3 || obj_width > line_height / 3);

    if (is_dot)
    {
      /* count */
      if ((y + obj_height / 2) < line_start)
      {
        (*dots_above) += 0.1;
        *top_dot_x = (double) x / (double) (m_text->width);
      }
      else if ((y + obj_height / 2) > line_end)
      {
        (*dots_below) += 0.1;
      }
      else
      {
        (*dots_inside) += 0.1;
        *inside_dot_x = (double) x / (double) (m_text->width);
      }
    }
    else if (is_obj)
    {
      /* count */
      if ((y + obj_height / 2) < line_start)
      {
        (*objs_above) += 0.1;
      }
      else if ((y + obj_height / 2) > line_end)
      {
        (*objs_below) += 0.1;

        /* get metrics of bigest obj under font */
        if (obj_width > *width)
        {
          obj_x = x;
          obj_y = y;
          obj_w = obj_width;
          obj_h = obj_height;
        }
      }
      else
      {
        (*objs_inside) += 0.1;
      }
    }
  }

  /* free obj map */
  ho_objmap_free (o_obj);

  /* if nothing below font we are finished */
  if (!(*objs_below) && !(*dots_below))
    return FALSE;

  /* get nikud metrics */
  *main_obj_height = 2.0 * (double) obj_h / (double) line_height;
  *main_obj_width = 2.0 * (double) obj_w / (double) line_height;

  if (obj_y && obj_h)
  {
    *top = 2.0 * (double) (obj_y - line_end) / (double) line_height;
    *bottom =
      2.0 * (double) ((obj_y + obj_h) - line_end) / (double) line_height;
  }

  /* get nikud start and end */
  sum = 0;
  for (y = line_end + 1; y < m_text->height && sum == 0; y++)
    for (sum = 0, x = 1; x < (m_text->width - 1); x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_start_y = y - 1;
  sum = 0;
  for (y = m_text->height - 1; y > font_start_y && sum == 0; y--)
    for (sum = 0, x = 1; x < (m_text->width - 1); x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_end_y = y + 1;
  font_height = font_end_y - font_start_y;

  if (!font_height)
    return TRUE;

  *height = 2.0 * (double) font_height / (double) line_height;

  sum = 0;
  for (x = 2; x < m_text->width && sum == 0; x++)
    for (sum = 0, y = line_end; y < (m_text->height - 1); y++)
      sum += ho_bitmap_get (m_text, x, y);
  font_start_x = x - 1;
  sum = 0;

  for (sum = 0, x = m_text->width - 2; x > (font_start_x + 1) && sum == 0; x--)
    for (sum = 0, y = line_end; y < (m_text->height - 1); y++)
      sum += ho_bitmap_get (m_text, x, y);
  font_end_x = x + 1;
  font_width = font_end_x - font_start_x;

  if (!font_width)
    return TRUE;

  *width = 2.0 * (double) font_width / (double) line_height;

  /* get nikud egdes */
  for (y = font_start_y, x = font_start_x;
    x < font_end_x && y < (font_end_y)
    && !ho_bitmap_get (m_text, x, y); x++, y++) ;
  *top_left = (double) (x - font_start_x) / (double) (line_height / 4);

  if (*top_left > 1.0)
    *top_left = 1.0;

  for (y = font_end_y, x = font_start_x;
    x < font_end_x && y > (font_start_y)
    && !ho_bitmap_get (m_text, x, y); x++, y--) ;
  *bottom_left = (double) (x - font_start_x) / (double) (line_height / 4);

  if (*bottom_left > 1.0)
    *bottom_left = 1.0;

  for (y = font_start_y, x = font_end_x - 1;
    x > font_start_x && y < (font_end_y)
    && !ho_bitmap_get (m_text, x, y); x--, y++) ;
  *top_right = (double) (font_end_x - x) / (double) (line_height / 4);

  if (*top_right > 1.0)
    *top_right = 1.0;

  for (y = font_end_y, x = font_end_x - 1;
    x > font_start_x && y > font_start_y
    && !ho_bitmap_get (m_text, x, y); x--, y--) ;
  *bottom_right = (double) (font_end_x - x) / (double) (line_height / 4);

  if (*bottom_right > 1.0)
    *bottom_right = 1.0;

  for (y = font_start_y, x = font_start_x + font_width / 2;
    y < (font_start_y + font_height) && !ho_bitmap_get (m_text, x, y); y++) ;
  *top_mid = (double) (y - font_start_y) / (double) (line_height / 4);

  if (*top_mid > 1.0)
    *top_mid = 1.0;

  for (y = font_end_y, x = font_start_x + font_width / 2;
    y > (font_start_y + font_height) && !ho_bitmap_get (m_text, x, y); y--) ;
  *bottom_mid = (double) (font_end_y - y) / (double) (line_height / 4);

  if (*bottom_mid > 1.0)
    *bottom_mid = 1.0;

  for (y = font_start_y + font_height / 2, x = font_start_x;
    x < (font_end_x) && !ho_bitmap_get (m_text, x, y); x++) ;
  *mid_left = (double) (x - font_start_x) / (double) (line_height / 4);

  if (*mid_left > 1.0)
    *mid_left = 1.0;

  for (y = font_start_y + font_height / 2, x = font_end_x - 1;
    x > (font_start_x) && !ho_bitmap_get (m_text, x, y); x--) ;
  *mid_right = (double) (font_end_x - x) / (double) (line_height / 4);

  if (*mid_right > 1.0)
    *mid_right = 1.0;

  return FALSE;
}

int
ho_recognize_nikud_array_in (const ho_bitmap * m_nikud,
  const ho_bitmap * m_mask, double *array_in)
{

  double height;
  double width;
  double top;
  double bottom;
  double top_left;
  double top_mid;
  double top_right;
  double mid_left;
  double mid_right;
  double bottom_left;
  double bottom_mid;
  double bottom_right;
  double dots_above;
  double dots_below;
  double dots_inside;
  double objs_above;
  double objs_below;
  double objs_inside;
  double main_obj_height;
  double main_obj_width;
  double top_dot_x;
  double inside_dot_x;
  double font_width_by_height;

  ho_recognize_nikud_dimentions (m_nikud,
    m_mask, &height,
    &width, &top, &bottom,
    &top_left, &top_mid, &top_right,
    &mid_left, &mid_right,
    &bottom_left, &bottom_mid,
    &bottom_right,
    &dots_above, &dots_below, &dots_inside,
    &objs_above, &objs_below, &objs_inside, &main_obj_height, &main_obj_width,
    &top_dot_x, &inside_dot_x, &font_width_by_height);

  array_in[0] = height;
  array_in[1] = width;
  if (height)
    array_in[2] = width / height;
  else
    array_in[2] = 0.0;
  if (array_in[2] > 1.0)
    array_in[2] = 1.0;
  array_in[3] = top;
  array_in[4] = bottom;
  array_in[5] = top_left;
  array_in[6] = top_mid;
  array_in[7] = top_right;
  array_in[8] = mid_left;
  array_in[9] = mid_right;
  array_in[10] = bottom_left;
  array_in[11] = bottom_mid;
  array_in[12] = bottom_right;

  array_in[13] = dots_above;
  array_in[14] = dots_inside;
  array_in[15] = dots_below;
  array_in[16] = objs_above;
  array_in[17] = objs_inside;
  array_in[18] = objs_below;

  array_in[19] = main_obj_width;
  array_in[20] = main_obj_height;

  array_in[21] = top_dot_x;
  array_in[22] = inside_dot_x;
  array_in[23] = font_width_by_height;

  return 0;
}

int
ho_recognize_nikud_array_out (const double *array_in, double *array_out,
  int font_code)
{
  int i;

  /* set array out */
  array_out[0] = 0.1;

  for (i = 1; i < HO_NIKUD_ARRAY_OUT_SIZE; i++)
    array_out[i] = 0.0;

  /* one dot above line */
  if (array_in[13] == 0.1)
  {
    if (array_in[23] < 0.53)
    {
      /* holam */
      array_out[5] = 1.0;
    }
    else
    {
      if (array_in[21] > 0.5)
      {
        /* shin */
        array_out[14] = -1.0;
      }
      else
      {
        /* sin */
        array_out[15] = -1.0;
      }
    }
  }

  /* dot inside font */
  if (array_in[14] == 0.1)
  {
    if (array_in[23] < 0.53)
    {
      /* shorok */
      array_out[6] = 1.0;
    }
    else
    {
      /* dagesh */
      array_out[13] = -1.0;
    }
  }

  /* two dots inside */
  if (array_in[14] > 0.1 && array_in[14] < 0.3)
  {
    /* shva */
    array_out[9] = 1.0;
  }

  /* one obj below line */
  if (array_in[18] == 0.1 && array_in[15] == 0.0)
  {

    if (array_in[1] > (1.8 * array_in[0]))
    {
      if (array_in[6] > 0.25)
      {
        /* tzere */
        array_out[3] = 1.0;
      }
      else
      {
        /* patach */
        array_out[2] = 1.0;
      }
    }
    else if (array_in[0] > (1.8 * array_in[1]))
    {
      /* shva */
      array_out[9] = 1.0;
    }
    else
    {
      if (array_in[12] > 0.1)
      {
        if (array_in[6] < 0.2)
        {
          /* kamatz */
          array_out[1] = 1.0;
        }
        else
        {
          /* segul */
          array_out[4] = 1.0;
        }
      }
      else
      {
        /* kobotz */
        array_out[7] = 1.0; 
      }
    }
  }

  /* one dot below line */
  if (array_in[15] > 0.0 && array_in[15] < 0.2 && array_in[18] == 0.0)
  {
    /* hirik */
    array_out[8] = 1.0;
  }

  /* two dots */
  if (array_in[15] > 0.1 && array_in[15] < 0.3)
  {
    if (array_in[18] != 0.1)
    {
      if (array_in[1] > (1.8 * array_in[0]) && array_in[1] > 0.65)
      {
        /* tzere */
        array_out[3] = 1.0;
      }
      else
      {
        /* shva */
        array_out[9] = 1.0;
      }
    }
    else                        /* hataf */
    {
      if (array_in[19] > (2.0 * array_in[20]))
      {
        /* hataf patach */
        array_out[11] = 1.0;
      }
      else
      {
        /* hataf kamatz */
        array_out[12] = 1.0;
      }
    }
  }

  /* three dots */
  if (array_in[15] > 0.2 && array_in[15] < 0.4)
  {
    if (array_in[11] > 0.1)
    {
      /* kobotz */
      array_out[7] = 1.0;
    }
    else
    {
      /* segul */
      array_out[4] = 1.0;
    }
  }

  /* five dots */
  if (array_in[15] > 0.4 && array_in[15] < 0.6)
  {
    /* hataf segul */
    array_out[10] = 1.0;
  }

  return FALSE;
}

const char *
ho_recognize_array_out_to_nikud (const double *array_out)
{
  int i = 0;
  int max_i = 0;

  /* find the font with hiegher score */
  for (i = 1; i < HO_NIKUD_ARRAY_OUT_SIZE; i++)
    if (array_out[i] > array_out[max_i])
      max_i = i;
  return ho_nikud_array[max_i];
}

const char *
ho_recognize_nikud (const ho_bitmap * m_nikud, const ho_bitmap * m_mask,
  int font_code, const char **dagesh, const char **shin)
{
  double array_in[HO_NIKUD_ARRAY_IN_SIZE];
  double array_out[HO_NIKUD_ARRAY_OUT_SIZE];
  const char *font;

  *dagesh = ho_nikud_array[0];
  *shin = ho_nikud_array[0];

  ho_recognize_nikud_array_in (m_nikud, m_mask, array_in);
  ho_recognize_nikud_array_out (array_in, array_out, font_code);
  font = ho_recognize_array_out_to_nikud (array_out);

  /* dagesh */
  if (array_out[13] == -1.0)
    *dagesh = ho_nikud_array[13];

  /* shin */
  if (array_out[14] == -1.0)
    *shin = ho_nikud_array[14];
  if (array_out[15] == -1.0)
    *shin = ho_nikud_array[15];

  return font;
}
