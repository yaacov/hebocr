
/***************************************************************************
 *            ho_segment.c
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

ho_bitmap *
ho_segment_paragraphs_fine (const ho_bitmap * m, const unsigned char box,
  const double font_height_factor_min, const double font_height_factor_max,
  const double font_width_factor_min, const double font_width_factor_max,
  const double horizontal_link_factor, const double vertical_link_factor)
{
  ho_bitmap *m_clean;
  ho_bitmap *m_temp1;
  ho_bitmap *m_out;
  int x, y;

  /* take only "regular" fonts */
  m_clean = ho_bitmap_filter_by_size (m,
    (double) (m->font_height) * font_height_factor_min,
    (double) (m->font_height) * font_height_factor_max,
    (double) (m->font_width) * font_width_factor_min,
    (double) (m->font_width) * font_width_factor_max);
  if (!m_clean)
    return NULL;

  /* link paragraphs */
  m_temp1 = ho_bitmap_vlink (m_clean, m->line_spacing * vertical_link_factor);
  if (!m_temp1)
    return NULL;

  m_out = ho_bitmap_hlink (m_temp1, m->font_width * horizontal_link_factor);
  if (!m_out)
    return NULL;

  ho_bitmap_free (m_temp1);

  if (box)
    m_temp1 = ho_bitmap_filter_boxes (m_out, 0, 0);
  else
    m_temp1 = ho_bitmap_filter_fill (m_out);

  ho_bitmap_free (m_out);
  ho_bitmap_free (m_clean);

  /* try and link one/two line broken paragraphs (free paragraphs need more
   * care) */
  m_out = ho_bitmap_filter_hlink (m_temp1, m->font_width * 6,
    m->font_height * 2 + m->line_spacing * 2);

  ho_bitmap_free (m_temp1);

  /* check all paragraphs for artefacts, too small or large */
  m_temp1 =
    ho_bitmap_filter_by_size (m_out, 2 * m->font_height / 3, m->height,
    m->font_width * 3, m->width);

  ho_bitmap_free (m_out);

  return m_temp1;
}

ho_bitmap *
ho_segment_paragraphs (const ho_bitmap * m, const unsigned char box)
{
  ho_bitmap *m_out;
  double font_height_factor_min = 0.25;
  double font_height_factor_max = 4.0;
  double font_width_factor_min = 0.25;
  double font_width_factor_max = 5.0;
  double horizontal_link_factor = 2.0;
  double vertical_link_factor = 1.2;

  /* if nikud we need to be more careful */
  if (m->nikud)
  {
    font_height_factor_min = 0.5;
    font_height_factor_max = 4.0;
    font_width_factor_min = 1.0 / 3.0;
    font_width_factor_max = 5.0;
  }

  /* if only one column we can be more agresive when linking horizontaly */
  if (box == 1)
  {
    horizontal_link_factor = 6.0;
    vertical_link_factor = 1.2;
  }

  m_out =
    ho_segment_paragraphs_fine (m, box, font_height_factor_min,
    font_height_factor_max, font_width_factor_min, font_width_factor_max,
    horizontal_link_factor, vertical_link_factor);

  return m_out;
}

ho_bitmap *
ho_segment_lines_fine (const ho_bitmap * m,
  const double font_height_factor_min, const double font_height_factor_max,
  const double font_width_factor_min, const double font_width_factor_max,
  const double link_arg, const double link_arg_2,
  const double extend_arg, const double extend_arg_2, const double erode_arg)
{
  int i;
  ho_bitmap *m_clean;
  ho_bitmap *m_temp;
  ho_bitmap *m_out;

  /* filter only "regular" sized fonts */
  m_clean = ho_bitmap_filter_by_size (m,
    (double) (m->font_height) * font_height_factor_min,
    (double) (m->font_height) * font_height_factor_max,
    (double) (m->font_width) * font_width_factor_min,
    (double) (m->font_width) * font_width_factor_max);
  if (!m_clean)
    return NULL;

  /* link lines */
  m_temp = ho_bitmap_hlink (m_clean, m->font_width * link_arg);
  if (!m_temp)
    return NULL;
  m_out = m_temp;

  /* add sideways leeway */
  for (i = 0; i < 1; i++)
  {
    /* add sideways leeway */
    m_temp =
      ho_bitmap_filter_obj_extend_lateraly (m_out, m->font_width * extend_arg);
    ho_bitmap_free (m_out);
    if (!m_temp)
      return NULL;
    m_out = m_temp;

    m_temp = ho_bitmap_hlink (m_out, m->font_width * link_arg);
    ho_bitmap_free (m_out);
    if (!m_temp)
      return NULL;
    m_out = m_temp;
  }

  /* set out matrix height, we want clean lines of known height */
  m_temp = ho_bitmap_filter_set_height (m_out, m->font_height, 0, 0);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;
  m_out = m_temp;

  /* remove little things up and down the line */
  m_temp = ho_bitmap_herode (m_out, m->font_width * erode_arg);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;
  m_out = m_temp;

  /* set out matrix height, we want clean lines of known height */
  m_temp = ho_bitmap_filter_set_height (m_out, m->font_height, 0, 0);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;
  m_out = m_temp;

  for (i = 0; i < 6; i++)
  {
    m_temp = ho_bitmap_hlink (m_out, m->font_width * link_arg);
    ho_bitmap_free (m_out);
    if (!m_temp)
      return NULL;
    m_out = m_temp;

    /* add sideways leeway */
    m_temp =
      ho_bitmap_filter_obj_extend_lateraly (m_out,
      m->font_width * extend_arg_2);
    ho_bitmap_free (m_out);
    if (!m_temp)
      return NULL;
    m_out = m_temp;
  }

  /* set out matrix height, we want clean lines of known height */
  m_temp = ho_bitmap_filter_set_height (m_out, m->font_height, 0, 0);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;
  m_out = m_temp;

  /* check for fonts that are outside the line */
  ho_bitmap_or (m_out, m_clean);

  /* link fonts that are autside line */
  m_temp = ho_bitmap_hlink (m_out, m->font_width * link_arg);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;
  m_out = m_temp;

  /* add sideways leeway */
  m_temp =
    ho_bitmap_filter_obj_extend_lateraly (m_out, m->font_width * extend_arg_2);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;
  m_out = m_temp;

  /* remove little things up and down the line */
  m_temp = ho_bitmap_herode (m_out, m->font_width * erode_arg);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;
  m_out = m_temp;

  /* set out matrix height, we want clean lines of known height */
  m_temp = ho_bitmap_filter_set_height (m_out, m->font_height, 0, 0);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;
  m_out = m_temp;

  ho_bitmap_free (m_clean);

  return m_out;
}

ho_bitmap *
ho_segment_lines (const ho_bitmap * m)
{
  ho_bitmap *m_out;

  double font_height_factor_min = 8.0 / 10.0;
  double font_height_factor_max = 12.0 / 10.0;
  double font_width_factor_min = 1.0 / 3.0;
  double font_width_factor_max = 5.0;

  double link_arg = 3.0;
  double link_arg_2 = 6.0;
  double extend_arg = 1.2;
  double extend_arg_2 = 3.0;
  double erode_arg = 5.0;

  /* check for one word lines */
  if (m->width < (m->font_width * erode_arg))
    erode_arg = 1;

  /* if not nikud we can include smaller fonts as "regular" */
  if (!(m->nikud))
  {
    font_height_factor_min = 7.0 / 10.0;
    font_height_factor_max = 12.0 / 10.0;
    font_width_factor_min = 1.0 / 5.0;
    font_width_factor_max = 5.0;
  }

  m_out = ho_segment_lines_fine (m,
    font_height_factor_min, font_height_factor_max,
    font_width_factor_min, font_width_factor_max,
    link_arg, link_arg_2, extend_arg, extend_arg_2, erode_arg);

  return m_out;
}

ho_bitmap *
ho_segment_words_fine (const ho_bitmap * m, const ho_bitmap * m_line_map,
  const double horizontal_link_factor, const double top_frame_factor,
  const double bottom_frame_factor)
{
  ho_bitmap *m_temp;
  ho_bitmap *m_out;

  int i;
  int x, y;
  int width;
  int height;
  int line_height;
  int font_spacing;
  unsigned char nikud_ret;

  /* check for extreem font spacing */
  if (m->font_spacing == 0)
    font_spacing = m->font_width / 4;
  else if (m->font_spacing == 255)
    font_spacing = m->font_width;
  else
    font_spacing = m->font_spacing;

  /* get line_height */
  x = m_line_map->width / 2;
  for (y = 0; y < m_line_map->height && !ho_bitmap_get (m_line_map, x, y);
    y++) ;
  line_height = y;
  for (; y < m_line_map->height && ho_bitmap_get (m_line_map, x, y); y++) ;
  line_height = y - line_height;

  /* chop of none line thigs */
  m_temp = ho_bitmap_clone (m);
  if (!m_temp)
    return NULL;
  ho_bitmap_and (m_temp, m_line_map);
  m_out = m_temp;

  /* get font boxes */
  m_temp =
    ho_bitmap_set_height (m_out, line_height * 2, line_height, line_height);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;
  m_out = m_temp;
  ho_bitmap_and (m_out, m_line_map);

  /* try to link words */
  m_temp =
    ho_bitmap_hlink (m_out, (double) font_spacing * horizontal_link_factor);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;
  m_out = m_temp;

  /* get font boxes */
  m_temp =
    ho_bitmap_set_height (m_out, line_height * 2, line_height, line_height);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;
  m_out = m_temp;
  ho_bitmap_and (m_out, m_line_map);

  m_temp =
    ho_bitmap_set_height (m_out, line_height,
    (double) line_height * top_frame_factor,
    (double) line_height * bottom_frame_factor);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;
  m_out = m_temp;

  /* set position in m_text */
  m_out->x = m->x;
  m_out->y = m->y;

  return m_out;
}

ho_bitmap *
ho_segment_words (const ho_bitmap * m, const ho_bitmap * m_line_map,
  const char font_spacing_code)
{
  ho_bitmap *m_out = NULL;

  double horizontal_link_factor = 9.0 / 5.0;
  double top_frame_factor = 1.0 / 2.0;
  double bottom_frame_factor = 3.0 / 2.0;

  /* if no nikud we do not need lots of bottom frame */
  if (!(m->nikud))
  {
    bottom_frame_factor = 1.0;
  }

  /* check font spacing */
  switch (font_spacing_code)
  {
  case -1:
    horizontal_link_factor = 6.0 / 5.0;
    break;
  case -2:
    horizontal_link_factor = 4.0 / 5.0;
    break;
  case -3:
    horizontal_link_factor = 2.0 / 5.0;
    break;
  case 1:
    horizontal_link_factor = 11.0 / 5.0;
    break;
  case 2:
    horizontal_link_factor = 16.0 / 5.0;
    break;
  case 3:
    horizontal_link_factor = 22.0 / 5.0;
    break;
  default:
    horizontal_link_factor = 9.0 / 5.0;
  }

  m_out = ho_segment_words_fine (m, m_line_map,
    horizontal_link_factor, top_frame_factor, bottom_frame_factor);

  return m_out;
}

ho_bitmap *
ho_segment_fonts (const ho_bitmap * m, const ho_bitmap * m_line_map,
  const unsigned char slicing_threshold, const unsigned char slicing_width)
{
  int return_val;

  ho_bitmap *m_temp;
  ho_bitmap *m_font;
  ho_bitmap *m_font_temp;
  ho_bitmap *m_out;
  ho_objmap *o_obj;

  int i;
  int x, y;
  int width;
  int height;
  int line_height;
  int s_threshold;
  int s_width;
  unsigned char nikud_ret;
  int *line_fill;

  /* sanity check */
  if (!m->width || !m->height)
    return NULL;

  /* set default slicing_threshold */
  if (slicing_threshold < 15)
    s_threshold = 70;
  else
    s_threshold = slicing_threshold;

  if (slicing_width < 80)
    s_width = 160;
  else
    s_width = slicing_width;

  /* get line_height */
  x = m_line_map->width / 2;
  for (y = 0; y < m_line_map->height && !ho_bitmap_get (m_line_map, x, y);
    y++) ;
  line_height = y;
  for (; y < m_line_map->height && ho_bitmap_get (m_line_map, x, y); y++) ;
  line_height = y - line_height;

  /* create a fill arrays */
  line_fill = (int *) calloc (m->width, sizeof (int));
  if (!line_fill)
    return NULL;

  /* chop of none line thigs */
  m_temp = ho_bitmap_clone (m);
  if (!m_temp)
    return NULL;
  /* FIXME: here and in ho_dimentions_line_fill() use a m_line_map + some
   * leeyway, letters that are out of the line like 'lamed' are choped */
  ho_bitmap_and (m_temp, m_line_map);

  for (x = 0; x < m->width; x++)
    for (y = 0; y < m->height; y++)
      line_fill[x] += ho_bitmap_get (m_temp, x, y);

  /* make line fill % of 100 */
  for (x = 0; x < m->width; x++)
    line_fill[x] = 100 * line_fill[x] / line_height;

  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return NULL;

  /* check for clean spaces */
  for (x = 0; x < m->width; x++)
  {
    if (line_fill[x] <= m->com_line_fill / 15)
      ho_bitmap_draw_vline (m_out, x, 0, m_out->height);
  }

  /* look for small overlaping */
  {
    int font_start;
    int font_end;

    x = 0;
    while (x < m->width)
    {
      /* get start&end of font */
      for (; x < m->width && ho_bitmap_get (m_out, x, 1); x++) ;
      font_start = x;
      for (; x < m->width && !ho_bitmap_get (m_out, x, 1); x++) ;
      font_end = x;

      {
        /* get the font bitmap */
        m_font =
          ho_bitmap_clone_window (m_temp, font_start, 0,
          font_end - font_start, m->height);
        /* look only for large objects that can be complete fonts */
        m_font_temp =
          ho_bitmap_filter_by_size (m_font, m->font_height / 4,
          2 * m->font_height, m->font_width / 5, 3 * m->font_width);
        o_obj = ho_objmap_new_from_bitmap (m_font_temp);
        ho_bitmap_free (m_font_temp);

        ho_objmap_sort_by_reading_index (o_obj, 255, FALSE);

        /* check for two or more objects */
        if (ho_objmap_get_size (o_obj) > 1)
        {
          /* search for little overlaping object */
          for (i = 0; i < ho_objmap_get_size (o_obj) - 1; i++)
          {
            int i1, i2;
            int this_object_end;
            int next_object_start;
            int thin_object_width;
            int overlaping;

            i1 = i;
            i2 = i + 1;

            thin_object_width = ho_objmap_get_object (o_obj, i1).width;
            if (thin_object_width > ho_objmap_get_object (o_obj, i2).width)
              thin_object_width = ho_objmap_get_object (o_obj, i2).width;

            this_object_end = ho_objmap_get_object (o_obj, i1).x;
            next_object_start = ho_objmap_get_object (o_obj, i2).x +
              ho_objmap_get_object (o_obj, i2).width;

            overlaping = next_object_start - this_object_end;

            /* check overlaping */
            if (overlaping >= 0 && overlaping < thin_object_width / 2)
            {
              /* draw lines at the objects egjes */
              ho_bitmap_draw_vline (m_out,
                ho_objmap_get_object (o_obj,
                  i1).x + font_start, 0, m_out->height);
            }
          }
        }
        ho_objmap_free (o_obj);
        ho_bitmap_free (m_font);
      }
    }
  }

  /* look for linked fonts, and try to break them using force */
  {
    int font_start;
    int font_end;

    x = 0;
    while (x < m->width)
    {
      /* get start&end of font */
      for (; x < m->width && ho_bitmap_get (m_out, x, 2); x++) ;
      font_start = x;
      for (; x < m->width && !ho_bitmap_get (m_out, x, 2); x++) ;
      font_end = x;

      /* if this is a wide font, try to break it using force */
      if ((font_end - font_start) > s_width * (m->font_width) / 100)
      {
        /* get the font bitmap */
        m_font =
          ho_bitmap_clone_window (m_temp, font_start, 0,
          font_end - font_start, m->height);
        o_obj = ho_objmap_new_from_bitmap (m_font);
        ho_bitmap_free (m_font);

        /* check that it is not a long _ or - 's */
        if (!(ho_objmap_get_size (o_obj) == 1
            && ho_objmap_get_object (o_obj, 0).height < m->font_height / 3))
          for (i = font_start; i < font_end; i++)
          {
            if (line_fill[i] <= s_threshold * m->com_line_fill / 100)
              ho_bitmap_draw_vline (m_out, i, 0, m_out->height);
          }

        ho_objmap_free (o_obj);
      }
    }
  }

  ho_bitmap_free (m_temp);

  /* thin interfont lines to one line per font */
  m_temp = ho_bitmap_new (m->width, m->height);
  if (!m_temp)
    return NULL;

  {
    int min_x;
    int min_x_start;
    int width = m->width;

    x = 0;
    while (x < width)
    {
      /* get start&end of interfont line */
      for (; x < width && !ho_bitmap_get (m_out, x, 2); x++) ;
      min_x = x - 1;
      min_x_start = min_x;
      for (; x < width && ho_bitmap_get (m_out, x, 2); x++)
      {
        if (line_fill[min_x] >= line_fill[x])
          min_x = x;
        if (line_fill[min_x_start] > line_fill[x])
          min_x_start = x;
      }
      /* draw line on minimal interfont space */
      for (i = min_x_start; i <= min_x && i < width - 1; i++)
        ho_bitmap_draw_vline (m_temp, i, 0, m_out->height);
    }
  }

  /* set position in m_text */
  m_temp->x = m->x;
  m_temp->y = m->y;

  ho_bitmap_free (m_out);
  free (line_fill);

  return m_temp;
}
