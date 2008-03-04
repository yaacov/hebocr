
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
ho_recognize_nikud_dimentions (const ho_bitmap * m_nikud,
  const ho_bitmap * m_mask, double *height,
  double *width, double *top, double *bottom,
  double *top_left, double *top_mid, double *top_right,
  double *mid_left, double *mid_right,
  double *bottom_left, double *bottom_mid,
  double *bottom_right,
  double *dots_above, double *dots_below, double *dots_inside,
  double *objs_above, double *objs_below, double *objs_inside)
{
  int i, x, y;
  int obj_width, obj_height;
  int line_start, line_end, line_height;
  ho_objmap *o_obj = NULL;
  unsigned char is_dot, is_obj;

  /* init values to zero */
  *height = 0;
  *width = 0;
  *top = 0;
  *bottom = 0;
  *top_left = 0;
  *top_mid = 0;
  *top_right = 0;
  *mid_left = 0;
  *mid_right = 0;
  *bottom_left = 0;
  *bottom_mid = 0;
  *bottom_right = 0;
  *dots_above = 0;
  *dots_below = 0;
  *dots_inside = 0;
  *objs_above = 0;
  *objs_below = 0;
  *objs_inside = 0;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++) ;
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++) ;
  line_end = y;
  line_height = line_end - line_start;

  /* get all the objects of nikud */
  o_obj = ho_objmap_new_from_bitmap (m_nikud);
  if (!o_obj)
    return TRUE;

  /* if no nikud just return */
  if (ho_objmap_get_size (o_obj) == 1)
  {
    ho_objmap_free (o_obj);

    return FALSE;
  }

  /* count points and objects above/in/below font */
  for (i = 0; i < ho_objmap_get_size (o_obj); i++)
  {
    /* is this object inside line ? */
    y = ho_objmap_get_object (o_obj, i).y;
    x = ho_objmap_get_object (o_obj, i).x;
    obj_width = ho_objmap_get_object (o_obj, i).width;
    obj_height = ho_objmap_get_object (o_obj, i).height;

    is_dot = (obj_height < line_height / 4 && obj_height > line_height / 16
      && obj_width < line_height / 4 && obj_width > line_height / 16);

    is_obj = (obj_height > line_height / 4 || obj_width > line_height / 4);

    if (is_dot)
    {
      if (y < line_start)
      {
        *dots_above++;
      }
      else if (y > line_end)
      {
        *dots_below++;
      }
      else
      {
        *dots_inside++;
      }
    }
    else if (is_obj)
    {
      if (y < line_start)
      {
        *objs_above++;
      }
      else if (y > line_end)
      {
        *objs_below++;
      }
      else
      {
        *objs_inside++;
      }
    }
  }

  /* get dimentions of nikud below font */

  return FALSE;
}

int
ho_recognize_create_nikud_array_in (const ho_bitmap * m_nikud,
  const ho_bitmap * m_mask, double *array_in)
{

  return 0;
}

double
ho_recognize_nikud_array (const double *array_in, const int sign_index,
  int font_code)
{
  double return_value;

  switch (sign_index)
  {
  case 1:                      /* alef */

    break;

  default:
    return_value = 0.0;
    break;
  }

  /* we only use the 0..1 range */
  if (return_value < 0.0)
    return_value = 0.0;
  if (return_value > 1.0)
    return_value = 1.0;

  return return_value;
}

int
ho_recognize_create_nikud_array_out (const double *array_in, double *array_out,
  int font_code)
{
  int i;

  /* set array out */
  array_out[0] = 0.1;

  for (i = 1; i < HO_NIKUD_ARRAY_OUT_SIZE; i++)
    array_out[i] = ho_recognize_nikud_array (array_in, i, font_code);
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
  int font_code)
{
  double array_in[HO_NIKUD_ARRAY_IN_SIZE];
  double array_out[HO_NIKUD_ARRAY_OUT_SIZE];
  const char *font;

  ho_recognize_create_nikud_array_in (m_nikud, m_mask, array_in);
  ho_recognize_create_nikud_array_out (array_in, array_out, font_code);
  font = ho_recognize_array_out_to_nikud (array_out);

  return font;
}
