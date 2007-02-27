/***************************************************************************
 *            ho_dimentions.c
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
#include "ho_dimentions.h"

int
ho_dimentions_font_width_height_nikud (ho_bitmap * m,
				       const int min_height,
				       const int max_height,
				       const int min_width,
				       const int max_width)
{
  int return_val;
  ho_objmap *m_obj = NULL;
  int height;
  int width;
  unsigned char nikud;

  /* create an object map from b/w image */
  m_obj = ho_objmap_new_from_bitmap (m);
  if (!m_obj)
    return TRUE;

  /* get fonts size */
  return_val =
    ho_objmap_font_metrix (m_obj, min_height, max_height, min_width,
			   max_width, &height, &width, &nikud);

  ho_objmap_free (m_obj);

  m->font_width = width;
  m->font_height = height;
  m->nikud = nikud;
  
  return return_val;
}

int
ho_dimentions_line_spacing (ho_bitmap * m)
{
  int return_val;
  ho_objmap *m_obj = NULL;

  ho_bitmap *m_clean;
  ho_bitmap *m_temp1;
  ho_bitmap *m_temp2;
  ho_bitmap *m_out;
  int x, y;
  int width;
  int height;
  unsigned char nikud_ret;

  int font_height = m->font_height;
  int font_width = m->font_width;
  unsigned char nikud = m->nikud;
  
  /* if nikud we need to be more careful */
  if (nikud)
    m_clean = ho_bitmap_filter_by_size (m,
					font_height / 2, font_height * 4,
					font_width / 3, font_width * 5);
  else
    m_clean = ho_bitmap_filter_by_size (m,
					font_height / 4, font_height * 4,
					font_width / 4, font_width * 5);

  /* get common height betwine lines */
  m_temp1 = ho_bitmap_hlink (m_clean, font_width * 4);
  if (!m_temp1)
    return TRUE;
  ho_bitmap_free (m_clean);

  m_temp2 = ho_bitmap_vlink (m_temp1, font_height * 4);
  if (!m_temp2)
    return TRUE;

  ho_bitmap_andnot (m_temp2, m_temp1);
  ho_bitmap_free (m_temp1);

  /* cut the betwine lines objects to word size objects */
  for (x = font_width * 3; x < m->width; x += font_width * 3)
    for (y = 0; y < m->height; y++)
      ho_bitmap_unset (m_temp2, x, y);

  /* create an object map from b/w image */
  m_obj = ho_objmap_new_from_bitmap (m_temp2);
  ho_bitmap_free (m_temp2);

  /* get interline size */
  return_val =
    ho_objmap_font_metrix (m_obj, font_height / 2, font_height * 4,
			   font_width * 2, font_width * 4, &height,
			   &width, &nikud_ret);

  ho_objmap_free (m_obj);

  /* sanity chack */
  if (height < font_height || height > font_height * 4)
    height = font_height * 1.5;

  m->line_spacing = height;
  
  return return_val;
}

int
ho_dimentions_font_spacing (ho_bitmap * m)
{
  int return_val;

  return return_val;
}
