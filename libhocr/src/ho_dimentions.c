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

  ho_bitmap *m_clean = NULL;
  ho_bitmap *m_temp1 = NULL;
  ho_bitmap *m_temp2 = NULL;

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
  for (x = font_width * 3; x < m->width; x += font_width * 3 + 2)
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
ho_dimentions_font_spacing (ho_bitmap * m, const ho_bitmap * m_line_map)
{
  int font_spacing = 0;
  ho_objmap *o_obj = NULL;

  ho_bitmap *m_temp;
  ho_bitmap *m_out;

  int n_fonts_free;
  int n_fonts_block;

  int x, y, i;

  /* chop of big/smal and out of line thigs */
  m_temp =
    ho_bitmap_filter_by_size (m, m->font_height / 6, m->font_height * 3,
			      m->font_width / 8, 3 * m->font_width);
  if (!m_temp)
    return TRUE;
  ho_bitmap_and (m_temp, m_line_map);
  m_out = m_temp;

  /* count ~number of fonts */
  n_fonts_free = ho_bitmap_filter_count_objects (m_out);

  /* get inter font spacing */
  m_temp =
    ho_bitmap_set_height (m_out, m->font_height * 2,
			  m->font_height, m->font_height);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return TRUE;
  m_out = m_temp;

  m_temp = ho_bitmap_clone (m_line_map);
  if (!m_temp)
    return TRUE;
  ho_bitmap_and (m_temp, m_out);
  ho_bitmap_free (m_out);
  m_out = m_temp;

  m_temp = ho_bitmap_hlink (m_out, m->font_width / 4);
  if (!m_temp)
    return TRUE;
  ho_bitmap_free (m_out);
  m_out = m_temp;

  /* look for zero width font spacing */
  n_fonts_block = ho_bitmap_filter_count_objects (m_out);
  if (n_fonts_block < 4 * n_fonts_free / 5)
    {
      /* zero font spacing */
      m->font_spacing = 0;
      ho_bitmap_free (m_out);
      return FALSE;
    }

  m_temp = ho_bitmap_clone (m_line_map);
  ho_bitmap_andnot (m_temp, m_out);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return TRUE;
  m_out = m_temp;

  m_temp =
    ho_bitmap_filter_by_size (m_out, 5, m->height, 1, m->font_width / 2);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return TRUE;
  m_out = m_temp;

  /* cut the betwine lines objects to word size objects */
  for (y = m->font_height; y < m->height; y += m->font_height / 5 + 2)
    for (x = 0; x < m->width; x++)
      ho_bitmap_unset (m_out, x, y);

  /* create an object map from b/w image */
  o_obj = ho_objmap_new_from_bitmap (m_out);
  ho_bitmap_free (m_out);
  if (!o_obj)
    return TRUE;

  if (ho_objmap_get_size (o_obj) < 4)
    {
      /* big font spacing */
      m->font_spacing = 255;
      ho_objmap_free (o_obj);
      return FALSE;
    }

  /* normal font spacing */
  for (i = 0; i < ho_objmap_get_size (o_obj); i++)
    if (font_spacing < ho_objmap_get_object (o_obj, i).width)
      font_spacing = ho_objmap_get_object (o_obj, i).width;

  if (font_spacing < m->font_width / 5)
    font_spacing = m->font_width / 5;

  m->font_spacing = font_spacing;

  ho_objmap_free (o_obj);

  return FALSE;
}

int
ho_dimentions_line_fill (ho_bitmap * m, const ho_bitmap * m_line_map)
{
  ho_bitmap *m_temp;
  int avg_line_fill;
  int com_line_fill;
  int *line_fill;
  int *line_fill_hist;
  int counter;
  int x, y;
  int line_height;
  int cell_size_for_common_fill_hist = 3;

  /* get line_height */
  x = m_line_map->width / 2;
  for (y = 0; y < m_line_map->height && !ho_bitmap_get (m_line_map, x, y);
       y++);
  line_height = y;
  for (; y < m_line_map->height && ho_bitmap_get (m_line_map, x, y); y++);
  line_height = y - line_height;

  /* create a fill arrays fill with {0, 0 ... } */
  line_fill = (int *) calloc (m->width, sizeof (int));
  if (!line_fill)
    return TRUE;
  line_fill_hist =
    (int *) malloc (m->height / cell_size_for_common_fill_hist *
		    sizeof (int));
  if (!line_fill_hist)
    return TRUE;

  /* fill hist with max value */
  for (y = 0; y < m->height / cell_size_for_common_fill_hist; y++)
    {
      line_fill_hist[y] = line_height;
    }

  /* chop of none line thigs */
  m_temp = ho_bitmap_clone (m);
  if (!m_temp)
    return TRUE;
  ho_bitmap_and (m_temp, m_line_map);

  /* get line fill data */
  avg_line_fill = 0;
  counter = 0;
  for (x = 0; x < m->width; x++)
    {
      for (y = 0; y < m->height; y++)
	{
	  line_fill[x] += ho_bitmap_get (m_temp, x, y);
	}
      avg_line_fill += line_fill[x];
      if (line_fill[x])
	counter++;
    }

  /* we do not need the temp text bitmap */
  ho_bitmap_free (m_temp);

  if (!counter)
    {
      return TRUE;
    }

  /* fill common fill histogram */
  for (y = cell_size_for_common_fill_hist + 1; y < m->height; y++)
    {
      for (x = 0; x < m->width; x++)
	if (line_fill[x] / cell_size_for_common_fill_hist ==
	    y / cell_size_for_common_fill_hist)
	  line_fill_hist[y / cell_size_for_common_fill_hist]++;
    }

  /* get the most common fill */
  com_line_fill = 1;
  for (y = 1; y < line_height / cell_size_for_common_fill_hist; y++)
    {
      if (line_fill_hist[com_line_fill] < line_fill_hist[y])
	com_line_fill = y;
    }
  com_line_fill *= cell_size_for_common_fill_hist;

  /* get avg fill */
  avg_line_fill = avg_line_fill / counter;

  /* free fill arrays */
  free (line_fill);
  free (line_fill_hist);

  /* set to precent of line height */
  m->avg_line_fill = 100 * avg_line_fill / line_height;
  m->com_line_fill = 100 * com_line_fill / line_height;

  return FALSE;
}

int
ho_dimentions_get_columns (const ho_bitmap * m)
{
  int i, return_val;
  ho_bitmap *m_cols = NULL;
  ho_bitmap *m_temp = NULL;
  ho_objmap *m_obj = NULL;

  /* link columns */
  m_temp = ho_bitmap_set_height (m, m->height, m->height, m->height);
  m_cols = ho_bitmap_hlink (m_temp, 40);
  ho_bitmap_free (m_temp);

  /* create an object map from b/w image */
  m_obj = ho_objmap_new_from_bitmap (m_cols);
  if (!m_obj)
    return TRUE;

  ho_bitmap_free (m_cols);

  /* get number ot columns */
  return_val = 0;
  for (i = 0; i < ho_objmap_get_size (m_obj); i++)
    if (ho_objmap_get_object (m_obj, i).width > m->width / 4)
      return_val++;

  ho_objmap_free (m_obj);

  return return_val;
}
