/***************************************************************************
 *            ho_font.c
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
#include "ho_pixbuf.h"
#include "ho_objmap.h"
#include "ho_segment.h"

#include "ho_font.h"

ho_bitmap *
ho_font_main_sign (const ho_bitmap * m_text, const ho_bitmap * m_mask)
{
  ho_objmap *o_obj = NULL;
  ho_bitmap *m_sign_mask = NULL;
  ho_bitmap *m_current_object = NULL;
  int x, y, y1, y2, x1, x2;
  int i;
  int line_start, line_end, line_height;
  unsigned char is_inside;
  unsigned char is_dot;
  unsigned char is_long_font;
  int count_dots;

  /* get line_height */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_end = y;
  line_height = line_end - line_start;

  /* get all the objects of the font */
  o_obj = ho_objmap_new_from_bitmap (m_text);
  if (!o_obj)
    return NULL;

  /* if only one object, just return it */
  if (ho_objmap_get_size (o_obj) == 1)
    {
      ho_objmap_free (o_obj);
      m_sign_mask = ho_bitmap_clone (m_text);

      /* clean top and bottom lines, just in case */
      ho_bitmap_delete_hline (m_sign_mask, 0, 0, m_sign_mask->width);
      ho_bitmap_delete_hline (m_sign_mask, 0, m_sign_mask->height - 1,
			      m_sign_mask->width);

      return m_sign_mask;
    }

  /* more than one object */
  m_sign_mask = ho_bitmap_new (m_text->width, m_text->height);
  if (!m_sign_mask)
    return NULL;

  /* copy all the objects inside the line */
  count_dots = 0;
  for (i = 0; i < ho_objmap_get_size (o_obj); i++)
    {
      /* is this object inside line ? */
      y1 = ho_objmap_get_object (o_obj, i).y;
      y2 =
	ho_objmap_get_object (o_obj, i).y + ho_objmap_get_object (o_obj,
								  i).height;

      x1 = ho_objmap_get_object (o_obj, i).x;
      x2 =
	ho_objmap_get_object (o_obj, i).x + ho_objmap_get_object (o_obj,
								  i).width;

      is_inside = ((y1 >= line_start
		    && y1 < line_end - line_height / 10)
		   || (y2 > line_start + line_height / 10 && y2 <= line_end)
		   || (y1 <= line_start && y2 >= line_end));

      is_dot = ((y2 - y1) < line_height / 4 && (y2 - y1) > line_height / 16
		&& (x2 - x1) < line_height / 4
		&& (x2 - x1) > line_height / 16);

      /* can't be more than two dots (one - dagesh, two - shva) */
      if (is_dot && is_inside)
	count_dots++;
      if (count_dots > 2)
	is_dot = FALSE;

      if (is_inside && !is_dot)
	{
	  /* copy the object to output bitmap */
	  m_current_object = ho_objmap_to_bitmap_by_index (o_obj, i);
	  if (m_current_object)
	    {
	      ho_bitmap_or (m_sign_mask, m_current_object);

	      ho_bitmap_free (m_current_object);
	    }
	}
    }

  /* clean top and bottom lines, just in case */
  ho_bitmap_delete_hline (m_sign_mask, 0, 0, m_sign_mask->width);
  ho_bitmap_delete_hline (m_sign_mask, 0, m_sign_mask->height - 1,
			  m_sign_mask->width);

  ho_objmap_free (o_obj);

  return m_sign_mask;
}

ho_bitmap *
ho_font_second_object (const ho_bitmap * m_text, const ho_bitmap * m_mask)
{
  ho_objmap *o_obj = NULL;
  ho_bitmap *m_sign_mask = NULL;
  ho_bitmap *m_current_object = NULL;
  int x, y, y1, y2, x1, x2;
  int i;
  int line_start, line_end, line_height;
  unsigned char is_inside;
  unsigned char is_dot;
  unsigned char is_long_font;
  int count_dots;

  /* get line_height */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_end = y;
  line_height = line_end - line_start;

  /* get all the objects of the font */
  o_obj = ho_objmap_new_from_bitmap (m_text);
  if (!o_obj)
    return NULL;

  /* if not two objects, return NULL */
  if (ho_objmap_get_size (o_obj) < 2)
    {
      ho_objmap_free (o_obj);

      return NULL;
    }

  /* more than one object */
  m_sign_mask = ho_bitmap_new (m_text->width, m_text->height);
  if (!m_sign_mask)
    return NULL;

  /* copy the first object found from lower left of font  */
  for (i = 0, y = line_end - 1, x = 1;
       x < o_obj->width && y > 0 && !i; x++, y--)
    {
      i = ho_objmap_get (o_obj, x, y);
    }

  if (i)
    m_current_object = ho_objmap_to_bitmap_by_index (o_obj, i - 1);
  if (m_current_object)
    {
      ho_bitmap_or (m_sign_mask, m_current_object);

      ho_bitmap_free (m_current_object);
    }

  ho_objmap_free (o_obj);

  return m_sign_mask;
}

ho_bitmap *
ho_font_holes (const ho_bitmap * m_text, const ho_bitmap * m_mask)
{
  ho_bitmap *m_negative = NULL;
  ho_bitmap *m_out = NULL;
  int x, y, line_height;

  /* get line_height */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_height;

  m_negative = ho_bitmap_not (m_text);
  if (!m_negative)
    return NULL;

  /* link all background elemnts */
  ho_bitmap_draw_vline (m_negative, 1, 0, m_negative->height - 1);
  ho_bitmap_draw_vline (m_negative, m_negative->width - 1, 0,
			m_negative->height - 1);
  ho_bitmap_draw_hline (m_negative, 0, 0, m_negative->width - 1);
  ho_bitmap_draw_hline (m_negative, 0, m_negative->height - 2,
			m_negative->width - 1);

  m_out =
    ho_bitmap_filter_by_size (m_negative, line_height / 5,
			      m_negative->height - 2, line_height / 5,
			      m_negative->width - 2);

  ho_bitmap_free (m_negative);

  return m_out;
}

ho_bitmap *
ho_font_hbars (const ho_bitmap * m_text, const ho_bitmap * m_mask)
{
  ho_bitmap *m_main_font = NULL;
  ho_bitmap *m_temp = NULL;
  ho_bitmap *m_bars = NULL;
  ho_bitmap *m_out = NULL;
  int i, x, y, line_height, y_start;
  int number_of_parts = 5;
  int threshold = 65;

  /* get line_height */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  y_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - y_start;

  if (!line_height)
    return NULL;

  /* maybe this font is realy broken, dialate  */
  m_temp = ho_bitmap_closing (m_text);
  if (!m_temp)
    return NULL;
  m_main_font = m_temp;

  /* in thin fonts do not chop the font too fine */
  if (line_height / number_of_parts < 3)
    number_of_parts = line_height / 3;

  /* chop main font to vertical parts */
  for (i = 1; i < number_of_parts; i++)
    ho_bitmap_delete_hline (m_main_font, 0,
			    y_start + i * line_height / number_of_parts,
			    m_text->width);

  /* look for wide objects */
  m_bars =
    ho_bitmap_filter_by_size (m_main_font, 1,
			      line_height * 2,
			      threshold * m_text->width / 100,
			      m_text->width + 5);
  ho_bitmap_free (m_main_font);
  if (!m_bars)
    return NULL;

  /* relink the vertical parts and cut off dialated parts */
  m_out = ho_bitmap_vlink (m_bars, 3);
  ho_bitmap_free (m_bars);
  ho_bitmap_and (m_out, m_text);

  /* make bars */

  for (y = y_start; y < y_start + line_height; y++)
    for (x = 0; x < m_text->width; x++)
      {
	if (ho_bitmap_get (m_out, x, y))
	  {
	    ho_bitmap_draw_hline (m_out, 0, y, m_out->width);
	    continue;
	  }
      }

  /* fix the x and y of the output bitmap */
  m_out->x = m_text->x;
  m_out->y = m_text->y;

  return m_out;
}

ho_bitmap *
ho_font_vbars (const ho_bitmap * m_text, const ho_bitmap * m_mask)
{
  ho_bitmap *m_main_font = NULL;
  ho_bitmap *m_temp = NULL;
  ho_bitmap *m_bars = NULL;
  ho_bitmap *m_out = NULL;
  int i, x, y, line_height, y_start;
  int number_of_parts = 4;
  int threshold = 70;

  /* get line_height */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  y_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - y_start;

  if (!line_height)
    return NULL;

  m_temp = ho_bitmap_closing (m_text);
  if (!m_temp)
    return NULL;
  m_main_font = m_temp;

  /* in thin fonts do not chop the font too fine */
  if (m_text->width / number_of_parts < 3)
    number_of_parts = m_text->width / 3;

  /* chop main font to vertical parts */
  for (i = 1; i < number_of_parts; i++)
    ho_bitmap_delete_vline (m_main_font, i * m_text->width / number_of_parts,
			    0, m_text->height);

  /* look for high objects */
  m_bars =
    ho_bitmap_filter_by_size (m_main_font, threshold * line_height / 100,
			      line_height * 2, 1, m_text->width);
  ho_bitmap_free (m_main_font);
  if (!m_bars)
    return NULL;

  /* relink the vertical parts and cut off dialated parts */
  m_out = ho_bitmap_hlink (m_bars, 3);
  ho_bitmap_free (m_bars);
  ho_bitmap_and (m_out, m_text);

  /* make bars */
  for (x = 0; x < m_text->width; x++)
    for (y = y_start; y < y_start + line_height; y++)
      {
	if (ho_bitmap_get (m_out, x, y))
	  {
	    ho_bitmap_draw_vline (m_out, x, y_start, line_height);
	    continue;
	  }
      }

  /* fix the x and y of the output bitmap */
  m_out->x = m_text->x;
  m_out->y = m_text->y;

  return m_out;
}

ho_bitmap *
ho_font_diagonal (const ho_bitmap * m_text, const ho_bitmap * m_mask)
{
  ho_bitmap *m_diagonal_mask = NULL;
  ho_bitmap *m_main_font = NULL;
  ho_bitmap *m_temp = NULL;
  ho_bitmap *m_bars = NULL;
  ho_bitmap *m_out = NULL;
  int i, x, y, line_height, y_start;
  int threshold = 90;

  /* get line_height */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  y_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - y_start;

  if (!line_height || !m_mask->width)
    return NULL;

  /* maybe this font is realy broken, dialate twice  */
  m_temp = ho_bitmap_closing (m_text);
  if (!m_temp)
    return NULL;
  m_main_font = m_temp;

  /* create diagonal mask */
  m_diagonal_mask = ho_bitmap_clone (m_mask);
  if (!m_diagonal_mask)
    {
      ho_bitmap_free (m_temp);
      return NULL;
    }
  for (x = 0; x < m_mask->width; x++)
    {
      ho_bitmap_delete_vline (m_diagonal_mask, x, y_start,
			      x * line_height / m_mask->width -
			      line_height / 6);
      ho_bitmap_delete_vline (m_diagonal_mask, x,
			      y_start + x * line_height / m_mask->width +
			      line_height / 6, line_height);
    }

  ho_bitmap_and (m_main_font, m_diagonal_mask);
  ho_bitmap_free (m_diagonal_mask);

  /* look for high objects */
  m_bars =
    ho_bitmap_filter_by_size (m_main_font, threshold * line_height / 100,
			      line_height * 2, 1, m_text->width);
  ho_bitmap_free (m_main_font);
  if (!m_bars)
    return NULL;

  /* cut off dialated parts */
  ho_bitmap_and (m_bars, m_text);

  /* fix the x and y of the output bitmap */
  m_bars->x = m_text->x;
  m_bars->y = m_text->y;

  return m_bars;
}

ho_bitmap *
ho_font_diagonal_left (const ho_bitmap * m_text, const ho_bitmap * m_mask)
{
  ho_bitmap *m_diagonal_mask = NULL;
  ho_bitmap *m_main_font = NULL;
  ho_bitmap *m_temp = NULL;
  ho_bitmap *m_bars = NULL;
  ho_bitmap *m_out = NULL;
  int i, x, y, line_height, y_start;
  int threshold = 90;

  /* get line_height */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  y_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - y_start;

  if (!line_height || !m_mask->width)
    return NULL;

  /* maybe this font is realy broken, dialate twice  */
  m_temp = ho_bitmap_clone (m_text);
  if (!m_temp)
    return NULL;
  m_main_font = m_temp;

  /* create diagonal mask */
  m_diagonal_mask = ho_bitmap_clone (m_mask);
  if (!m_diagonal_mask)
    {
      ho_bitmap_free (m_temp);
      return NULL;
    }
  for (x = 0; x < m_mask->width; x++)
    {
      ho_bitmap_delete_vline (m_diagonal_mask, m_mask->width - x - 1, y_start,
			      x * line_height / m_mask->width -
			      line_height / 6);
      ho_bitmap_delete_vline (m_diagonal_mask, m_mask->width - x - 1,
			      y_start + x * line_height / m_mask->width +
			      line_height / 6, line_height);
    }

  ho_bitmap_and (m_main_font, m_diagonal_mask);
  ho_bitmap_free (m_diagonal_mask);

  /* look for high objects */
  m_bars =
    ho_bitmap_filter_by_size (m_main_font, threshold * line_height / 100,
			      line_height * 2, 1, m_text->width);
  ho_bitmap_free (m_main_font);
  if (!m_bars)
    return NULL;

  /* cut off dialated parts */
  ho_bitmap_and (m_bars, m_text);

  /* fix the x and y of the output bitmap */
  m_bars->x = m_text->x;
  m_bars->y = m_text->y;

  return m_bars;
}

ho_bitmap *
ho_font_thin_naive (const ho_bitmap * m_text, const ho_bitmap * m_mask)
{
  ho_bitmap *m_out = NULL;
  ho_bitmap *m_temp = NULL;

  int i, x, y, line_height, y_start;
  unsigned char direction;
  unsigned char thin = TRUE;
  unsigned char thinned = TRUE;
  unsigned char edge_pixel = FALSE;
  unsigned char neighbors;
  unsigned char neighbors_up;
  unsigned char neighbors_down;
  unsigned char neighbors_right;
  unsigned char neighbors_left;
  unsigned char neighbors_horizontal;
  unsigned char neighbors_vertical;

  /* get line_height */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  y_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - y_start;

  if (!line_height || !m_mask->width)
    return NULL;

  /* copy the original font */
  m_out = ho_bitmap_clone (m_text);
  if (!m_out)
    return NULL;

  m_temp = ho_bitmap_new (m_text->width, m_text->height);
  if (!m_temp)
    return NULL;

  /* clean the edges */
  ho_bitmap_delete_vline (m_out, 0, 0, m_out->height - 1);
  ho_bitmap_delete_vline (m_out, m_out->width - 1, 0, m_out->height - 1);
  ho_bitmap_delete_hline (m_out, 0, 0, m_out->width - 1);
  ho_bitmap_delete_hline (m_out, 0, m_out->height - 1, m_out->width - 1);

  /* do until no pixel is thinned */
  while (thinned)
    {
      thinned = FALSE;
      /* east,west,north,south scans */
      for (direction = 0; direction < 4; direction++)
	{
	  /* get a temporary image to work on */
	  ho_bitmap_copy (m_temp, m_out);

	  /* look at evry pixel */
	  for (x = 1; x < m_text->width - 1; x++)
	    for (y = 1; y < m_text->height - 1; y++)
	      {
		/* is this an edge pixel for this direction */
		switch (direction)
		  {
		  case 0:
		    edge_pixel = ho_bitmap_get (m_out, x, y)
		      && !ho_bitmap_get (m_out, x, y + 1);
		    break;
		  case 1:
		    edge_pixel = ho_bitmap_get (m_out, x, y)
		      && !ho_bitmap_get (m_out, x + 1, y);
		    break;
		  case 2:
		    edge_pixel = ho_bitmap_get (m_out, x, y)
		      && !ho_bitmap_get (m_out, x, y - 1);
		    break;
		  case 3:
		    edge_pixel = ho_bitmap_get (m_out, x, y)
		      && !ho_bitmap_get (m_out, x - 1, y);
		    break;
		  }

		/* if this is an edge */
		if (edge_pixel)
		  {
		    /* look at the border pixels */
		    neighbors = ho_bitmap_get (m_out, x + 1, y + 1) +
		      ho_bitmap_get (m_out, x + 1, y) +
		      ho_bitmap_get (m_out, x + 1, y - 1) +
		      ho_bitmap_get (m_out, x - 1, y + 1) +
		      ho_bitmap_get (m_out, x - 1, y) +
		      ho_bitmap_get (m_out, x - 1, y - 1) +
		      ho_bitmap_get (m_out, x, y - 1) +
		      ho_bitmap_get (m_out, x, y + 1);

		    neighbors_up =
		      ho_bitmap_get (m_out, x + 1,
				     y - 1) + ho_bitmap_get (m_out, x - 1,
							     y - 1) +
		      ho_bitmap_get (m_out, x, y - 1);

		    neighbors_down =
		      ho_bitmap_get (m_out, x + 1,
				     y + 1) + ho_bitmap_get (m_out, x - 1,
							     y + 1) +
		      ho_bitmap_get (m_out, x, y + 1);

		    neighbors_right =
		      ho_bitmap_get (m_out, x - 1,
				     y - 1) + ho_bitmap_get (m_out, x - 1,
							     y) +
		      ho_bitmap_get (m_out, x - 1, y + 1);

		    neighbors_left =
		      ho_bitmap_get (m_out, x + 1,
				     y - 1) + ho_bitmap_get (m_out, x + 1,
							     y) +
		      ho_bitmap_get (m_out, x + 1, y + 1);

		    neighbors_horizontal = ho_bitmap_get (m_out, x - 1,
							  y) +
		      ho_bitmap_get (m_out, x + 1, y);

		    neighbors_vertical = ho_bitmap_get (m_out, x,
							y - 1) +
		      ho_bitmap_get (m_out, x, y + 1);

		    /* only one neigbor it's an edge */
		    thin = (neighbors > 1);

		    /* look for side to side connections */
		    thin = thin && !((neighbors_up && neighbors_down
				      && !neighbors_horizontal)
				     || (neighbors_right && neighbors_left
					 && !neighbors_vertical));

		    /* look for corner connection */
		    thin = thin && !(ho_bitmap_get (m_out, x - 1, y - 1)
				     && !ho_bitmap_get (m_out, x, y - 1)
				     && !ho_bitmap_get (m_out, x - 1, y));
		    thin = thin && !(ho_bitmap_get (m_out, x + 1,
						    y + 1)
				     && !ho_bitmap_get (m_out, x, y + 1)
				     && !ho_bitmap_get (m_out, x + 1, y));
		    thin = thin && !(ho_bitmap_get (m_out, x - 1,
						    y + 1)
				     && !ho_bitmap_get (m_out, x, y + 1)
				     && !ho_bitmap_get (m_out, x - 1, y));
		    thin = thin && !(ho_bitmap_get (m_out, x + 1,
						    y - 1)
				     && !ho_bitmap_get (m_out, x, y - 1)
				     && !ho_bitmap_get (m_out, x + 1, y));

		    if (thin)
		      {
			ho_bitmap_unset (m_temp, x, y);
			thinned = TRUE;
		      }
		  }
	      }

	  /* copy the temporary image back to image out */
	  ho_bitmap_copy (m_out, m_temp);
	}
    }

  ho_bitmap_free (m_temp);

  return m_out;
}

ho_bitmap *
ho_font_thin (const ho_bitmap * m_text, const ho_bitmap * m_mask)
{
  ho_bitmap *m_out = NULL;
  ho_bitmap *m_temp = NULL;
  int i, x, y, line_height, y_start;

  /* get line_height */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  y_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - y_start + 1;

  if (!line_height || !m_mask->width)
    return NULL;

  /* do naive thinning */
  m_out = ho_font_thin_naive (m_text, m_mask);

  /* fix holes */
  m_temp = ho_bitmap_hlink (m_out, line_height / 5);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;

  m_out = ho_bitmap_vlink (m_temp, line_height / 5);
  ho_bitmap_free (m_temp);
  if (!m_out)
    return NULL;

  /* smoth font edges */
  m_temp = ho_bitmap_closing (m_out);
  ho_bitmap_free (m_out);
  if (!m_temp)
    return NULL;

  /* rethin the font */
  m_out = ho_font_thin_naive (m_temp, m_mask);

  return m_temp;
}

ho_bitmap *
ho_font_edges_top (const ho_bitmap * m_text, const ho_bitmap * m_mask)
{
  ho_bitmap *m_out = NULL;
  ho_bitmap *m_temp = NULL;
  ho_bitmap *m_clean = NULL;
  int *a_height;
  int dx, dy;
  int i, x, y, line_height, y_start;
  int y1, y2, x1, x2;
  int threshold = 2;
  int sum;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  y_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > y_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_height = y - y_start + 1;

  if (!line_height || m_mask->width < 2)
    return NULL;

  /* get the fill of the font */
  a_height = (int *) calloc (m_text->width, sizeof (int));
  if (!a_height)
    return NULL;

  /* set y start and y end */
  dy = threshold * line_height / 100 + 1;
  y1 = y_start - line_height / 6 - 3;
  y2 = y_start + line_height / 2 + 3;
  if (y1 < 0)
    y1 = 0;
  dx = m_text->width / 30 + 3;
  x1 = dx + 1;
  x2 = m_text->width - x1;

  for (x = 0; x < m_text->width; x++)
    {
      for (y = y1; y < y2 && !ho_bitmap_get (m_text, x, y); y++);
      a_height[x] = (y - y1);
    }

  /* allocate bitmap output */
  m_out = ho_bitmap_new (m_text->width, m_text->height);
  if (!m_out)
    return NULL;

  /* set lines where it looks like a notch */
  for (x = 0; x < m_out->width; x++)
    {
      if (((x < dx || a_height[x - dx] > a_height[x] + dy)
	   && (x > m_out->width - dx || a_height[x + dx] > a_height[x] + dy))
	  || ((x < 2 * dx || a_height[x - dx * 2] > a_height[x] + dy)
	      && (x > m_out->width - 2 * dx
		  || a_height[x + dx * 2] > a_height[x] + dy)))
	{
	  ho_bitmap_draw_vline (m_out, x, 0, y_start);
	}
    }

  m_temp = ho_bitmap_hlink (m_out, dx * 2);
  ho_bitmap_free (m_out);

  /* thin notch lines to one line per font */
  m_out = ho_bitmap_new (m_temp->width, m_temp->height);
  if (!m_out)
    return NULL;

  {
    int min_x;
    int min_x_start;

    x = 1;
    for (; x < m_temp->width && !ho_bitmap_get (m_temp, x, 2); x++);
    while (x < m_temp->width)
      {
	/* get start&end of notch line */
	min_x = x - 1;
	min_x_start = min_x;
	for (; x < m_temp->width && ho_bitmap_get (m_temp, x, 2); x++)
	  {
	    if (a_height[min_x] >= a_height[x])
	      min_x = x;
	    if (a_height[min_x_start] > a_height[x])
	      min_x_start = x;
	  }
	/* draw line on minimal interfont space */
	for (i = min_x_start; i <= min_x; i++)
	  ho_bitmap_draw_vline (m_out, i, 0, m_out->height);

	for (; x < m_temp->width && !ho_bitmap_get (m_temp, x, 2); x++);
      }
  }

  free (a_height);
  ho_bitmap_free (m_temp);

  /* fix the x and y of the output bitmap */
  m_out->x = m_text->x;
  m_out->y = m_text->y;

  return m_out;
}

ho_bitmap *
ho_font_edges_bottom (const ho_bitmap * m_text, const ho_bitmap * m_mask)
{
  ho_bitmap *m_out = NULL;
  ho_bitmap *m_temp = NULL;
  ho_bitmap *m_clean = NULL;
  int *a_height;
  int dx, dy;
  int i, x, y, line_height, y_start;
  int y1, y2, x1, x2;
  int threshold = 2;
  int sum;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  y_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > y_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_height = y - y_start + 1;

  if (!line_height || m_mask->width < 2)
    return NULL;

  /* get the fill of the font */
  a_height = (int *) calloc (m_text->width, sizeof (int));
  if (!a_height)
    return NULL;

  /* set y start and y end */
  dy = threshold * line_height / 100 + 1;
  y1 = y_start + line_height - line_height / 2 - 3;
  y2 = y_start + line_height + line_height / 6 + 3;
  if (y2 > m_text->height - 1)
    y2 = m_text->height - 1;

  dx = m_text->width / 30 + 3;
  x1 = dx + 1;
  x2 = m_text->width - x1;

  for (x = 0; x < m_text->width; x++)
    {
      for (y = y2; y > y1 && !ho_bitmap_get (m_text, x, y); y--);
      a_height[x] = (y2 - y);
    }

  /* allocate bitmap output */
  m_out = ho_bitmap_new (m_text->width, m_text->height);
  if (!m_out)
    return NULL;

  /* set lines where it looks like a notch */
  for (x = 0; x < m_out->width; x++)
    {
      if (((x < dx || a_height[x - dx] > a_height[x] + dy)
	   && (x > m_out->width - dx
	       || a_height[x + dx] > a_height[x] + dy))
	  || ((x < 2 * dx || a_height[x - dx * 2] > a_height[x] + dy)
	      && (x > m_out->width - 2 * dx
		  || a_height[x + dx * 2] > a_height[x] + dy)))
	{
	  ho_bitmap_draw_vline (m_out, x, 0, m_out->height);
	}
    }

  m_temp = ho_bitmap_hlink (m_out, dx * 2);
  ho_bitmap_free (m_out);

  /* thin notch lines to one line per font */
  m_out = ho_bitmap_new (m_temp->width, m_temp->height);
  if (!m_out)
    return NULL;

  {
    int min_x;
    int min_x_start;

    x = 1;
    for (;
	 x < m_temp->width && !ho_bitmap_get (m_temp, x, m_temp->height - 2);
	 x++);
    while (x < m_temp->width)
      {
	/* get start&end of notch line */
	min_x = x - 1;
	min_x_start = min_x;
	for (;
	     x < m_temp->width
	     && ho_bitmap_get (m_temp, x, m_temp->height - 2); x++)
	  {
	    if (a_height[min_x] >= a_height[x])
	      min_x = x;
	    if (a_height[min_x_start] > a_height[x])
	      min_x_start = x;
	  }
	/* draw line on minimal interfont space */
	for (i = min_x_start; i <= min_x; i++)
	  ho_bitmap_draw_vline (m_out, i, 0, m_out->height);

	for (;
	     x < m_temp->width
	     && !ho_bitmap_get (m_temp, x, m_temp->height - 2); x++);
      }
  }

  free (a_height);
  ho_bitmap_free (m_temp);

  /* fix the x and y of the output bitmap */
  m_out->x = m_text->x;
  m_out->y = m_text->y;

  return m_out;
}

ho_bitmap *
ho_font_edges_left (const ho_bitmap * m_text, const ho_bitmap * m_mask)
{
  ho_bitmap *m_out = NULL;
  ho_bitmap *m_temp = NULL;
  ho_bitmap *m_clean = NULL;
  int *a_height;
  int dx, dy;
  int i, x, y, line_height, y_start;
  int y1, y2, x1, x2;
  int threshold = 2;
  int sum;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  y_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > y_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_height = y - y_start + 1;

  if (!line_height || m_mask->width < 2)
    return NULL;

  /* set y start and y end */
  dx = threshold * line_height / 100 + 1;
  x1 = 0;
  x2 = 3 * m_text->width / 4 + 1;

  dy = line_height / 10 + 2;
  y1 = y_start;
  y2 = y_start + line_height + 1;
  if (y2 > m_text->height)
    y2 = m_text->height;

  /* get the fill of the font */
  a_height = (int *) calloc (line_height + 1, sizeof (int));
  if (!a_height)
    return NULL;

  for (y = y1; y < y2; y++)
    {
      for (x = x1; x < x2 && !ho_bitmap_get (m_text, x, y); x++);
      a_height[y - y1] = (x - x1);
    }

  /* allocate bitmap output */
  m_out = ho_bitmap_new (m_text->width, m_text->height);
  if (!m_out)
    return NULL;

  /* set lines where it looks like a notch */
  for (y = y1; y < y2 && y < m_out->height; y++)
    {
      if (((y < y1 + dy || a_height[y - dy - y1] > a_height[y - y1] + dx)
	   && (y > y2 - dy
	       || a_height[y + dy - y1] > a_height[y - y1] + dx))
	  ||
	  ((y < y1 + 2 * dy
	    || a_height[y - dy * 2 - y1] > a_height[y - y1] + dx)
	   && (y > y2 - 2 * dy
	       || a_height[y + dy * 2 - y1] > a_height[y - y1] + dx)))
	{
	  ho_bitmap_draw_hline (m_out, 0, y, m_out->width);
	}
    }

  m_temp = ho_bitmap_vlink (m_out, dy * 2);
  ho_bitmap_free (m_out);

  /* thin notch lines to one line per font */
  m_out = ho_bitmap_new (m_temp->width, m_temp->height);
  if (!m_out)
    return NULL;

  {
    int min_y;
    int min_y_start;

    y = y1 + 1;
    for (; y < y2 && !ho_bitmap_get (m_temp, 1, y); y++);
    while (y < y2)
      {
	/* get start&end of notch line */
	min_y = y - 1;
	min_y_start = min_y;
	for (; y < y2 && ho_bitmap_get (m_temp, 1, y); y++)
	  {
	    if (a_height[min_y - y1] >= a_height[y - y1])
	      min_y = y;
	    if (a_height[min_y_start - y1] > a_height[y - y1])
	      min_y_start = y;
	  }
	/* draw line on minimal interfont space */
	for (i = min_y_start; i <= min_y; i++)
	  ho_bitmap_draw_hline (m_out, 0, i, m_out->width);

	for (; y < y2 && !ho_bitmap_get (m_temp, 1, y); y++);
      }
  }

  free (a_height);
  ho_bitmap_free (m_temp);

  /* fix the x and y of the output bitmap */
  m_out->x = m_text->x;
  m_out->y = m_text->y;

  return m_out;
}

ho_bitmap *
ho_font_edges_right (const ho_bitmap * m_text, const ho_bitmap * m_mask)
{
  ho_bitmap *m_out = NULL;
  ho_bitmap *m_temp = NULL;
  ho_bitmap *m_clean = NULL;
  int *a_height;
  int dx, dy;
  int i, x, y, line_height, y_start;
  int y1, y2, x1, x2;
  int threshold = 0;
  int sum;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  y_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > y_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_height = y - y_start + 1;

  if (!line_height || m_mask->width < 2)
    return NULL;

  /* set y start and y end */
  dx = threshold * m_text->width / 100 + 1;
  x1 = m_text->width - 3 * m_text->width / 4 - 1;
  x2 = m_text->width;

  dy = line_height / 10 + 2;
  y1 = y_start;
  y2 = y_start + line_height + 1;
  if (y2 > m_text->height)
    y2 = m_text->height;

  /* get the fill of the font */
  a_height = (int *) calloc (line_height + 1, sizeof (int));
  if (!a_height)
    return NULL;

  for (y = y1; y < y2; y++)
    {
      for (x = x2 - 1; x >= x1 && !ho_bitmap_get (m_text, x, y); x--);
      a_height[y - y1] = (x2 - x);
    }

  /* allocate bitmap output */
  m_out = ho_bitmap_new (m_text->width, m_text->height);
  if (!m_out)
    return NULL;

  /* set lines where it looks like a notch */
  for (y = y1; y < y2; y++)
    {
      if (((y < y1 + dy || a_height[y - dy - y1] > a_height[y - y1] + dx)
	   && (y > y2 - dy || a_height[y + dy - y1] > a_height[y - y1] + dx))
	  ||
	  ((y < y1 + 2 * dy
	    || a_height[y - dy * 2 - y1] > a_height[y - y1] + dx)
	   && (y > y2 - 2 * dy
	       || a_height[y + dy * 2 - y1] > a_height[y - y1] + dx)))
	{
	  ho_bitmap_draw_hline (m_out, 0, y, m_text->width);
	}
    }

  m_temp = ho_bitmap_vlink (m_out, dy * 2);
  ho_bitmap_free (m_out);

  /* thin notch lines to one line per font */
  m_out = ho_bitmap_new (m_temp->width, m_temp->height);
  if (!m_out)
    return NULL;

  {
    int min_y;
    int min_y_start;

    y = y1 + 1;
    for (; y < y2 && !ho_bitmap_get (m_temp, 2, y); y++);
    while (y < y2)
      {
	/* get start&end of notch line */
	min_y = y - 2;
	min_y_start = min_y;
	for (; y < y2 && ho_bitmap_get (m_temp, 2, y); y++)
	  {
	    if (a_height[min_y - y1] >= a_height[y - y1])
	      min_y = y;
	    if (a_height[min_y_start - y1] > a_height[y - y1])
	      min_y_start = y;
	  }
	/* draw line on minimal interfont space */
	for (i = min_y_start; i <= min_y; i++)
	  ho_bitmap_draw_hline (m_out, 0, i, m_out->width);

	for (; y < y2 && !ho_bitmap_get (m_temp, 2, y); y++);
      }
  }

  free (a_height);
  ho_bitmap_free (m_temp);

  /* fix the x and y of the output bitmap */
  m_out->x = m_text->x;
  m_out->y = m_text->y;

  return m_out;
}

ho_bitmap *
ho_font_notch_top (const ho_bitmap * m_text, const ho_bitmap * m_mask)
{
  ho_bitmap *m_out = NULL;
  ho_bitmap *m_temp = NULL;
  ho_bitmap *m_clean = NULL;
  int *a_height;
  int dx, dy;
  int i, x, y, line_height, y_start;
  int y1, y2, x1, x2;
  int threshold = 2;
  int sum;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  y_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > y_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_height = y - y_start + 1;

  if (!line_height || m_mask->width < 2)
    return NULL;

  /* get the fill of the font */
  a_height = (int *) calloc (m_text->width, sizeof (int));
  if (!a_height)
    return NULL;

  /* set y start and y end */
  dy = threshold * line_height / 100 + 2;
  y1 = y_start - line_height / 6 - 3;
  y2 = y_start + line_height / 2 + 3;
  if (y1 < 0)
    y1 = 0;
  dx = m_text->width / 30 + 3;
  x1 = dx + 1;
  x2 = m_text->width - x1;

  for (x = 0; x < m_text->width; x++)
    {
      for (y = y1; y < y2 && !ho_bitmap_get (m_text, x, y); y++);
      a_height[x] = (y - y1);
    }

  /* allocate bitmap output */
  m_out = ho_bitmap_new (m_text->width, m_text->height);
  if (!m_out)
    return NULL;

  /* set lines where it looks like a notch */
  for (x = 0; x < m_out->width; x++)
    {
      if (((x > dx && a_height[x - dx] < a_height[x] - dy)
	   && (x < m_out->width - dx && a_height[x + dx] < a_height[x] - dy))
	  || ((x > 2 * dx && a_height[x - dx * 2] < a_height[x] - dy)
	      && (x < m_out->width - 2 * dx
		  && a_height[x + dx * 2] < a_height[x] - dy)))
	{
	  ho_bitmap_draw_vline (m_out, x, 0, y_start);
	}
    }

  m_temp = ho_bitmap_hlink (m_out, dx * 2);
  ho_bitmap_free (m_out);

  /* thin notch lines to one line per font */
  m_out = ho_bitmap_new (m_temp->width, m_temp->height);
  if (!m_out)
    return NULL;

  {
    int min_x;
    int min_x_start;

    x = 1;
    for (; x < m_temp->width && !ho_bitmap_get (m_temp, x, 2); x++);
    while (x < m_temp->width)
      {
	/* get start&end of notch line */
	min_x = x - 1;
	min_x_start = min_x;
	for (; x < m_temp->width && ho_bitmap_get (m_temp, x, 2); x++)
	  {
	    if (a_height[min_x] <= a_height[x])
	      min_x = x;
	    if (a_height[min_x_start] < a_height[x])
	      min_x_start = x;
	  }
	/* draw line on minimal interfont space */
	for (i = min_x_start; i <= min_x; i++)
	  ho_bitmap_draw_vline (m_out, i, 0, m_out->height);

	for (; x < m_temp->width && !ho_bitmap_get (m_temp, x, 2); x++);
      }
  }

  free (a_height);
  ho_bitmap_free (m_temp);

  /* fix the x and y of the output bitmap */
  m_out->x = m_text->x;
  m_out->y = m_text->y;

  return m_out;
}

ho_bitmap *
ho_font_notch_bottom (const ho_bitmap * m_text, const ho_bitmap * m_mask)
{
  ho_bitmap *m_out = NULL;
  ho_bitmap *m_temp = NULL;
  ho_bitmap *m_clean = NULL;
  int *a_height;
  int dx, dy;
  int i, x, y, line_height, y_start;
  int y1, y2, x1, x2;
  int threshold = 2;
  int sum;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  y_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > y_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_height = y - y_start + 1;

  if (!line_height || m_mask->width < 2)
    return NULL;

  /* get the fill of the font */
  a_height = (int *) calloc (m_text->width, sizeof (int));
  if (!a_height)
    return NULL;

  /* set y start and y end */
  dy = threshold * line_height / 100 + 2;
  y1 = y_start + line_height - line_height / 2 - 3;
  y2 = y_start + line_height + line_height / 6 + 3;
  if (y2 > m_text->height - 1)
    y2 = m_text->height - 1;

  dx = m_text->width / 30 + 3;
  x1 = dx + 1;
  x2 = m_text->width - x1;

  for (x = 0; x < m_text->width; x++)
    {
      for (y = y2; y > y1 && !ho_bitmap_get (m_text, x, y); y--);
      a_height[x] = (y2 - y);
    }

  /* allocate bitmap output */
  m_out = ho_bitmap_new (m_text->width, m_text->height);
  if (!m_out)
    return NULL;

  /* set lines where it looks like a notch */
  for (x = 0; x < m_out->width; x++)
    {
      if (((x > dx && a_height[x - dx] < a_height[x] - dy)
	   && (x < m_out->width - dx && a_height[x + dx] < a_height[x] - dy))
	  || ((x > 2 * dx && a_height[x - dx * 2] < a_height[x] - dy)
	      && (x < m_out->width - 2 * dx
		  && a_height[x + dx * 2] < a_height[x] - dy)))
	{
	  ho_bitmap_draw_vline (m_out, x, 0, m_out->height);
	}
    }

  m_temp = ho_bitmap_hlink (m_out, dx * 2);
  ho_bitmap_free (m_out);

  /* thin notch lines to one line per font */
  m_out = ho_bitmap_new (m_temp->width, m_temp->height);
  if (!m_out)
    return NULL;

  {
    int min_x;
    int min_x_start;

    x = 1;
    for (;
	 x < m_temp->width && !ho_bitmap_get (m_temp, x, m_temp->height - 2);
	 x++);
    while (x < m_temp->width)
      {
	/* get start&end of notch line */
	min_x = x - 1;
	min_x_start = min_x;
	for (;
	     x < m_temp->width
	     && ho_bitmap_get (m_temp, x, m_temp->height - 2); x++)
	  {
	    if (a_height[min_x] <= a_height[x])
	      min_x = x;
	    if (a_height[min_x_start] < a_height[x])
	      min_x_start = x;
	  }
	/* draw line on minimal interfont space */
	for (i = min_x_start; i <= min_x; i++)
	  ho_bitmap_draw_vline (m_out, i, 0, m_out->height);

	for (;
	     x < m_temp->width
	     && !ho_bitmap_get (m_temp, x, m_temp->height - 2); x++);
      }
  }

  free (a_height);
  ho_bitmap_free (m_temp);

  /* fix the x and y of the output bitmap */
  m_out->x = m_text->x;
  m_out->y = m_text->y;

  return m_out;
}

ho_bitmap *
ho_font_notch_left (const ho_bitmap * m_text, const ho_bitmap * m_mask)
{
  ho_bitmap *m_out = NULL;
  ho_bitmap *m_temp = NULL;
  ho_bitmap *m_clean = NULL;
  int *a_height;
  int dx, dy;
  int i, x, y, line_height, y_start;
  int y1, y2, x1, x2;
  int threshold = 2;
  int sum;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  y_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > y_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_height = y - y_start + 1;

  if (!line_height || m_mask->width < 2)
    return NULL;

  /* set y start and y end */
  dx = threshold * line_height / 100 + 2;
  x1 = 0;
  x2 = 3 * m_text->width / 4 + 1;

  dy = line_height / 10 + 2;
  y1 = y_start;
  y2 = y_start + line_height + 1;
  if (y2 > m_text->height)
    y2 = m_text->height;

  /* get the fill of the font */
  a_height = (int *) calloc (line_height + 1, sizeof (int));
  if (!a_height)
    return NULL;

  for (y = y1; y < y2; y++)
    {
      for (x = x1; x < x2 && !ho_bitmap_get (m_text, x, y); x++);
      a_height[y - y1] = (x - x1);
    }

  /* allocate bitmap output */
  m_out = ho_bitmap_new (m_text->width, m_text->height);
  if (!m_out)
    return NULL;

  /* set lines where it looks like a notch */
  for (y = y1; y < y2 && y < m_out->height; y++)
    {
      if (((y > y1 + dy && a_height[y - dy - y1] < a_height[y - y1] - dx)
	   && (y < y2 - dy
	       && a_height[y + dy - y1] < a_height[y - y1] - dx))
	  ||
	  ((y > y1 + 2 * dy
	    && a_height[y - dy * 2 - y1] < a_height[y - y1] - dx)
	   && (y < y2 - 2 * dy
	       && a_height[y + dy * 2 - y1] < a_height[y - y1] - dx)))
	{
	  ho_bitmap_draw_hline (m_out, 0, y, m_out->width);
	}
    }

  m_temp = ho_bitmap_vlink (m_out, dy * 2);
  ho_bitmap_free (m_out);

  /* thin notch lines to one line per font */
  m_out = ho_bitmap_new (m_temp->width, m_temp->height);
  if (!m_out)
    return NULL;

  {
    int min_y;
    int min_y_start;

    y = y1 + 1;
    for (; y < y2 && !ho_bitmap_get (m_temp, 1, y); y++);
    while (y < y2)
      {
	/* get start&end of notch line */
	min_y = y - 1;
	min_y_start = min_y;
	for (; y < y2 && ho_bitmap_get (m_temp, 1, y); y++)
	  {
	    if (a_height[min_y - y1] <= a_height[y - y1])
	      min_y = y;
	    if (a_height[min_y_start - y1] < a_height[y - y1])
	      min_y_start = y;
	  }
	/* draw line on minimal interfont space */
	for (i = min_y_start; i <= min_y; i++)
	  ho_bitmap_draw_hline (m_out, 0, i, m_out->width);

	for (; y < y2 && !ho_bitmap_get (m_temp, 1, y); y++);
      }
  }

  free (a_height);
  ho_bitmap_free (m_temp);

  /* fix the x and y of the output bitmap */
  m_out->x = m_text->x;
  m_out->y = m_text->y;

  return m_out;
}

ho_bitmap *
ho_font_notch_right (const ho_bitmap * m_text, const ho_bitmap * m_mask)
{
  ho_bitmap *m_out = NULL;
  ho_bitmap *m_temp = NULL;
  ho_bitmap *m_clean = NULL;
  int *a_height;
  int dx, dy;
  int i, x, y, line_height, y_start;
  int y1, y2, x1, x2;
  int threshold = 0;
  int sum;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  y_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > y_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_height = y - y_start + 1;

  if (!line_height || m_mask->width < 2)
    return NULL;

  /* set y start and y end */
  dx = threshold * m_text->width / 100 + 2;
  x1 = m_text->width - 3 * m_text->width / 4 - 1;
  x2 = m_text->width;

  dy = line_height / 10 + 2;
  y1 = y_start;
  y2 = y_start + line_height + 1;
  if (y2 > m_text->height)
    y2 = m_text->height;

  /* get the fill of the font */
  a_height = (int *) calloc (line_height + 1, sizeof (int));
  if (!a_height)
    return NULL;

  for (y = y1; y < y2; y++)
    {
      for (x = x2 - 1; x >= x1 && !ho_bitmap_get (m_text, x, y); x--);
      a_height[y - y1] = (x2 - x);
    }

  /* allocate bitmap output */
  m_out = ho_bitmap_new (m_text->width, m_text->height);
  if (!m_out)
    return NULL;

  /* set lines where it looks like a notch */
  for (y = y1; y < y2; y++)
    {
      if (((y > y1 + dy && a_height[y - dy - y1] < a_height[y - y1] - dx)
	   && (y < y2 - dy && a_height[y + dy - y1] < a_height[y - y1] - dx))
	  ||
	  ((y > y1 + 2 * dy
	    && a_height[y - dy * 2 - y1] < a_height[y - y1] - dx)
	   && (y < y2 - 2 * dy
	       || a_height[y + dy * 2 - y1] < a_height[y - y1] - dx)))
	{
	  ho_bitmap_draw_hline (m_out, 0, y, m_text->width);
	}
    }

  m_temp = ho_bitmap_vlink (m_out, dy * 2);
  ho_bitmap_free (m_out);

  /* thin notch lines to one line per font */
  m_out = ho_bitmap_new (m_temp->width, m_temp->height);
  if (!m_out)
    return NULL;

  {
    int min_y;
    int min_y_start;

    y = y1 + 1;
    for (; y < y2 && !ho_bitmap_get (m_temp, 2, y); y++);
    while (y < y2)
      {
	/* get start&end of notch line */
	min_y = y - 2;
	min_y_start = min_y;
	for (; y < y2 && ho_bitmap_get (m_temp, 2, y); y++)
	  {
	    if (a_height[min_y - y1] <= a_height[y - y1])
	      min_y = y;
	    if (a_height[min_y_start - y1] < a_height[y - y1])
	      min_y_start = y;
	  }
	/* draw line on minimal interfont space */
	for (i = min_y_start; i <= min_y; i++)
	  ho_bitmap_draw_hline (m_out, 0, i, m_out->width);

	for (; y < y2 && !ho_bitmap_get (m_temp, 2, y); y++);
      }
  }

  free (a_height);
  ho_bitmap_free (m_temp);

  /* fix the x and y of the output bitmap */
  m_out->x = m_text->x;
  m_out->y = m_text->y;

  return m_out;
}

int
ho_font_pnm_save (const ho_bitmap * m_text, const ho_bitmap * m_nikud,
		  const ho_bitmap * m_mask, const char *filename)
{
  FILE *file = NULL;
  ho_pixbuf *pix = NULL;
  int x, y;
  unsigned char red, green, blue;

  /* create pixbuf */
  pix = ho_pixbuf_new (3, m_text->width, m_text->height, 0);
  if (!pix)
    return TRUE;

  /* fill the pixbuf */
  for (x = 0; x < m_text->width; x++)
    for (y = 0; y < m_text->height; y++)
      {
	red = ho_bitmap_get (m_text, x, y) * 255;
	green = ho_bitmap_get (m_nikud, x, y) * 255;
	blue = ho_bitmap_get (m_mask, x, y) * 255;

	ho_pixbuf_set (pix, x, y, 0, red);
	ho_pixbuf_set (pix, x, y, 1, green);
	ho_pixbuf_set (pix, x, y, 2, blue);
      }

  /* open file for writing */
  file = fopen (filename, "wb");

  if (!file)
    {
      ho_pixbuf_free (pix);
      return TRUE;
    }

  /* print header */
  fprintf (file, "P6 %d %d 255\n", pix->width, pix->height);

  /* this might be a huge write... */
  fwrite (pix->data, 1, pix->height * pix->rowstride, file);
  fclose (file);

  ho_pixbuf_free (pix);

  return FALSE;
}

int
ho_font_pnm_load (ho_bitmap ** m_text, ho_bitmap ** m_nikud,
		  ho_bitmap ** m_mask, const char *filename)
{
  ho_pixbuf *pix = NULL;
  int x, y;
  unsigned char red, green, blue;

  /* load pix from file */
  pix = ho_pixbuf_pnm_load (filename);
  if (!pix)
    return TRUE;

  if (pix->n_channels != 3)
    {
      ho_pixbuf_free (pix);
      return TRUE;
    }

  /* allocate the font bitmaps */
  (*m_text) = ho_bitmap_new (pix->width, pix->height);
  (*m_nikud) = ho_bitmap_new (pix->width, pix->height);
  (*m_mask) = ho_bitmap_new (pix->width, pix->height);

  /* set contents */
  for (x = 0; x < (*m_text)->width; x++)
    for (y = 0; y < (*m_text)->height; y++)
      {
	red = ho_pixbuf_get (pix, x, y, 0);
	green = ho_pixbuf_get (pix, x, y, 1);
	blue = ho_pixbuf_get (pix, x, y, 2);

	if (red)
	  ho_bitmap_set ((*m_text), x, y);
	if (green)
	  ho_bitmap_set ((*m_nikud), x, y);
	if (blue)
	  ho_bitmap_set ((*m_mask), x, y);

      }

  ho_pixbuf_free (pix);
  return FALSE;
}
