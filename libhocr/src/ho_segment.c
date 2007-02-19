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

#include "ho_bitmap.h"
#include "ho_objmap.h"
#include "ho_bitmap_filter.h"

ho_bitmap *
ho_segment_paragraphs (const ho_bitmap * m,
		       const unsigned char font_height,
		       const unsigned char font_width,
		       const unsigned char nikud,
		       const int interline_height,
		       const unsigned char box)
{
  ho_bitmap *m_clean;
  ho_bitmap *m_temp1;
  ho_bitmap *m_out;
  int x, y;

  /* if nikud we need to be more careful */
  if (nikud)
    m_clean = ho_bitmap_filter_by_size (m,
					font_height / 2, font_height * 4,
					font_width / 3, font_width * 5);
  else
    m_clean = ho_bitmap_filter_by_size (m,
					font_height / 4, font_height * 4,
					font_width / 4, font_width * 5);

  /* link paragraphs */
  m_temp1 = ho_bitmap_vlink (m_clean, interline_height * 1.2);
  if (!m_temp1)
    return NULL;

  m_out = ho_bitmap_hlink (m_temp1, font_width * 2);
  if (!m_out)
    return NULL;

  ho_bitmap_free (m_temp1);

  if (box)
    m_temp1 = ho_bitmap_filter_boxes (m_out, font_height / 2, 0);
  else
    m_temp1 = ho_bitmap_filter_fill (m_out);

  ho_bitmap_free (m_out);
  ho_bitmap_free (m_clean);

  /* try and link one/two line broken paragraphs */
  m_out = ho_bitmap_filter_hlink (m_temp1, font_width * 6,
				  font_height * 2 + interline_height * 2);

  ho_bitmap_free (m_temp1);

  /* check all paragraphs for artefacts, too small or large */
  m_temp1 =
    ho_bitmap_filter_by_size (m_out, 2 * font_height / 3, m->height,
			      font_width * 3, m->width);

  ho_bitmap_free (m_out);

  return m_temp1;
}

ho_bitmap *
ho_segment_lines (const ho_bitmap * m,
		  const unsigned char font_height,
		  const unsigned char font_width,
		  const unsigned char nikud, const int interline_height)
{
  ho_objmap *m_obj;

  ho_bitmap *m_clean;
  ho_bitmap *m_temp;
  ho_bitmap *m_out;

  int x, y;
  int index;
  int width, height;

  m_clean = ho_bitmap_filter_by_size (m,
				      4 * font_height / 5, font_height * 1.2,
				      font_width / 3, font_width * 5);

  /* link lines */
  m_temp = ho_bitmap_hlink (m_clean, font_width * 6);
  ho_bitmap_free (m_clean);
  if (!m_temp)
    return NULL;

  /* add sideways leeway */
  m_out = ho_bitmap_filter_obj_extend_lateraly (m_temp, font_width * 4);

  /* remove little things up and down the line */
  m_temp = ho_bitmap_herode (m_out, font_width * 6);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;

  /* re link lines, helps connect broken lines */
  m_out = ho_bitmap_hlink (m_temp, font_width * 6);
  ho_bitmap_free (m_temp);
  if (!m_out)
    return NULL;

  /* add more sideways leeway */
  m_temp = ho_bitmap_filter_obj_extend_lateraly (m_out, font_width * 4);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;

  /* set out matrix height, we want clean lines of known height */
  m_out = ho_bitmap_filter_obj_max_height (m_temp, font_height);
  ho_bitmap_free (m_temp);
  if (!m_out)
    return NULL;

  /* final touch remove little things up and down the line */
  m_temp = ho_bitmap_herode (m_out, font_width * 6);
  ho_bitmap_free (m_out);

  return m_temp;
}
