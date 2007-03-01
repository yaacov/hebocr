/***************************************************************************
 *            ho_segment.c
 *
 *  Fri Aug 12 20:13:33 2005
 *  Copyright  2005-2007  Yaacov Zamir
 *  <kzamir@walla.co.il>
 ****************************************************************************/

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
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
#include "ho_bitmap_filter.h"

ho_bitmap *
ho_segment_paragraphs (const ho_bitmap * m, const unsigned char box)
{
  ho_bitmap *m_clean;
  ho_bitmap *m_temp1;
  ho_bitmap *m_out;
  int x, y;

  /* if nikud we need to be more careful */
  if (m->nikud)
    m_clean = ho_bitmap_filter_by_size (m,
					m->font_height / 2,
					m->font_height * 4, m->font_width / 3,
					m->font_width * 5);
  else
    m_clean = ho_bitmap_filter_by_size (m,
					m->font_height / 4,
					m->font_height * 4, m->font_width / 4,
					m->font_width * 5);

  /* link paragraphs */
  m_temp1 = ho_bitmap_vlink (m_clean, m->line_spacing * 1.2);
  if (!m_temp1)
    return NULL;

  m_out = ho_bitmap_hlink (m_temp1, m->font_width * 2);
  if (!m_out)
    return NULL;

  ho_bitmap_free (m_temp1);

  if (box)
    m_temp1 = ho_bitmap_filter_boxes (m_out, 0, 0);
  else
    m_temp1 = ho_bitmap_filter_fill (m_out);

  ho_bitmap_free (m_out);
  ho_bitmap_free (m_clean);

  /* try and link one/two line broken paragraphs */
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
ho_segment_lines (const ho_bitmap * m)
{
  ho_bitmap *m_temp;
  ho_bitmap *m_out;

  /* filter only "regular" sized fonts */
  m_temp = ho_bitmap_filter_by_size (m,
				     8 * m->font_height / 10,
				     12 * m->font_height / 10,
				     m->font_width / 3, m->font_width * 5);
  if (!m_temp)
    return NULL;
  m_out = m_temp;

  /* link lines */
  m_temp = ho_bitmap_hlink (m_out, m->font_width * 6);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;
  m_out = m_temp;

  /* add sideways leeway */
  m_temp = ho_bitmap_filter_obj_extend_lateraly (m_out, m->font_width * 4);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;
  m_out = m_temp;

  /* remove little things up and down the line */
  m_temp = ho_bitmap_herode (m_out, m->font_width * 6);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;
  m_out = m_temp;

  /* add more sideways leeway */
  m_temp = ho_bitmap_filter_obj_extend_lateraly (m_out, m->font_width * 4);
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

  /* try to link broken lines */
  m_temp = ho_bitmap_hlink (m_out, m->font_width * 3);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;
  m_out = m_temp;

  /* remove little things up and down the line */
  m_temp = ho_bitmap_herode (m_out, m->font_width * 3);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;
  m_out = m_temp;

  /* re-set matrix height, we want clean lines of known height */
  m_temp = ho_bitmap_filter_set_height (m_out, m->font_height, 0, 0);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;
  m_out = m_temp;

  return m_out;
}

ho_bitmap *
ho_segment_words (const ho_bitmap * m, const ho_bitmap * m_line_map)
{
  int return_val;

  ho_bitmap *m_temp;
  ho_bitmap *m_out;

  int i;
  int x, y;
  int width;
  int height;
  int line_height;
  unsigned char nikud_ret;

  /* get line_height */
  x = m_line_map->width / 2;
  for (y = 0; y < m_line_map->height && !ho_bitmap_get (m_line_map, x, y);
       y++);
  line_height = y;
  for (; y < m_line_map->height && ho_bitmap_get (m_line_map, x, y); y++);
  line_height = y - line_height;

  /* chop of line thigs */
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
  m_temp = ho_bitmap_hlink (m_out, 2 * m->font_spacing);
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
    ho_bitmap_set_height (m_out, 5 * line_height / 3,
			  line_height / 2, 2 * line_height / 3);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;
  m_out = m_temp;

  return m_out;
}
