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
       y++);
  line_height = y;
  for (; y < m_line_map->height && ho_bitmap_get (m_line_map, x, y); y++);
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
  m_temp = ho_bitmap_hlink (m_out, 9 * font_spacing / 5);
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

  /* set position in m_text */
  m_out->x = m->x;
  m_out->y = m->y;

  return m_out;
}

ho_bitmap *
ho_segment_fonts (const ho_bitmap * m, const ho_bitmap * m_line_map)
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
  unsigned char nikud_ret;
  int *line_fill;

  /* sanity check */
  if (!m->width || !m->height)
    return NULL;


  /* create a fill arrays */
  line_fill = (int *) calloc (m->width, sizeof (int));
  if (!line_fill)
    return NULL;

  /* chop of none line thigs */
  m_temp = ho_bitmap_clone (m);
  if (!m_temp)
    return NULL;
  ho_bitmap_and (m_temp, m_line_map);

  for (x = 0; x < m->width; x++)
    for (y = 0; y < m->height; y++)
      line_fill[x] += ho_bitmap_get (m_temp, x, y);

  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return NULL;

  /* check for clean spaces */
  for (x = 0; x < m->width; x++)
    {
      if (line_fill[x] <= m->com_line_fill / 15)
	ho_bitmap_draw_vline (m_out, x, 10, m_out->height - 20);
    }

  /* look for small overlaping */
  {
    int font_start;
    int font_end;

    x = 0;
    while (x < m->width)
      {
	/* get start&end of font */
	for (; x < m->width && ho_bitmap_get (m_out, x, 11); x++);
	font_start = x;
	for (; x < m->width && !ho_bitmap_get (m_out, x, 11); x++);
	font_end = x;

	{
	  /* get the font bitmap */
	  m_font =
	    ho_bitmap_clone_window (m_temp, font_start, 0,
				    font_end - font_start, m->height);
	  /* look only for large objects that can be complete fonts */
	  m_font_temp =
	    ho_bitmap_filter_by_size (m_font, m->font_height / 4,
				      2 * m->font_height, m->font_width / 5,
				      3 * m->font_width);
	  o_obj = ho_objmap_new_from_bitmap (m_font_temp);
	  ho_bitmap_free (m_font_temp);

	  ho_objmap_sort_by_reading_index (o_obj, 255);

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
		  if (thin_object_width > ho_objmap_get_object (o_obj,
								i2).width)
		    thin_object_width =
		      ho_objmap_get_object (o_obj, i2).width;

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
								  i1).x +
					    font_start, 10,
					    m_out->height - 20);
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
	for (; x < m->width && ho_bitmap_get (m_out, x, 11); x++);
	font_start = x;
	for (; x < m->width && !ho_bitmap_get (m_out, x, 11); x++);
	font_end = x;

	/* if this is a wide font, try to break it using force */
	if ((font_end - font_start) > 3 * (m->font_width) / 2)
	  {
	    /* get the font bitmap */
	    m_font =
	      ho_bitmap_clone_window (m_temp, font_start, 0,
				      font_end - font_start, m->height);
	    o_obj = ho_objmap_new_from_bitmap (m_font);
	    ho_bitmap_free (m_font);

	    /* check that it is not a long _ or - 's */
	    if (!(ho_objmap_get_size (o_obj) == 1
		  && ho_objmap_get_object (o_obj,
					   0).height < m->font_height / 3))
	      for (i = font_start; i < font_end; i++)
		{
		  if (line_fill[i] <= m->com_line_fill / 4)
		    ho_bitmap_draw_vline (m_out, i, 10, m_out->height - 20);
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

    x = 1;
    while (x < m->width)
      {
	/* get start&end of interfont line */
	for (; x < m->width && !ho_bitmap_get (m_out, x, 11); x++);
	min_x = x - 1;
	for (; x < m->width && ho_bitmap_get (m_out, x, 11); x++)
	  {
	    if (line_fill[min_x] > line_fill[x])
	      min_x = x;
	    /* if it is open space draw a line too */
	    if (line_fill[x] <= m->com_line_fill / 15)
	      ho_bitmap_draw_vline (m_temp, x, 10, m_out->height - 20);
	  }
	/* draw one line per interfont space that is not just open space */
	ho_bitmap_draw_vline (m_temp, min_x, 10, m_out->height - 20);
      }
  }

  /* set position in m_text */
  m_temp->x = m->x;
  m_temp->y = m->y;

  ho_bitmap_free (m_out);
  free (line_fill);

  return m_temp;
}