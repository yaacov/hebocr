/***************************************************************************
 *            ho_recognize.c
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
#include "ho_segment.h"
#include "ho_font.h"
#include "ho_recognize_sign.h"

#include "ho_recognize.h"

int
ho_recognize_hbar_up (const ho_bitmap * m_text, const ho_bitmap * m_mask,
		      double *height, double *start, double *end,
		      double *dist)
{
  ho_bitmap *m_hbars = NULL;
  int x, y, y_start, y_end, line_start, line_height;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the hbar bitmap */
  m_hbars = ho_font_hbars (m_text, m_mask);
  if (!m_hbars)
    return TRUE;

  /* get start and end of horizontal top bar */
  y = line_start - line_height / 2;
  if (y < 0)
    y = 0;
  for (;
       y < line_start + line_height / 2 && !ho_bitmap_get (m_hbars, x, y);
       y++);
  y_start = y;
  for (; y < line_start + line_height / 2 && ho_bitmap_get (m_hbars, x, y);
       y++);
  y_end = y;

  ho_bitmap_free (m_hbars);

  /* check for no top bar */
  if (y_start == y_end)
    {
      *height = 0.0;
      *start = 0.0;
      *end = 0.0;

      return FALSE;
    }

  /* if start is far from line top check artefacts */
  if (y_start > line_start + line_height / 12)
    {
      int new_sum = 0;
      int sum = new_sum;

      for (;
	   y_start > line_start && (new_sum >= sum
				    || new_sum > m_text->width / 2);
	   y_start--)
	for (sum = new_sum, new_sum = 0, x = 0; x < m_text->width; x++)
	  new_sum += ho_bitmap_get (m_text, x, y_start);
    }

  /* set height */
  *height = (double) (y_end - y_start) / (double) line_height;

  /* set distance from top */
  *dist = (double) (line_start - y_start) / (double) line_height;

  /* get x start and end of bar */
  {
    int sum = 0;
    for (x = 0; x < m_text->width && sum == 0; x++)
      for (sum = 0, y = y_start; y < y_end; y++)
	sum += ho_bitmap_get (m_text, x, y);

    *start = (double) x / (double) (m_text->width);

    for (; x < m_text->width && sum > 0; x++)
      for (sum = 0, y = y_start; y < y_end; y++)
	sum += ho_bitmap_get (m_text, x, y);

    *end = (double) x / (double) (m_text->width);
  }

  return FALSE;
}

int
ho_recognize_hbar_down (const ho_bitmap * m_text,
			const ho_bitmap * m_mask, double *height,
			double *start, double *end, double *dist)
{
  ho_bitmap *m_hbars = NULL;
  int x, y, y_start, y_end, line_start, line_height;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the hbar bitmap */
  m_hbars = ho_font_hbars (m_text, m_mask);
  if (!m_hbars)
    return TRUE;

  /* get start and end of horizontal top bar */
  y = line_start + 3 * line_height / 2;
  if (y > m_mask->height)
    y = m_mask->height;
  for (;
       y > line_start + line_height / 2 && !ho_bitmap_get (m_hbars, x, y);
       y--);
  y_end = y;
  for (; y > line_start + line_height / 2 && ho_bitmap_get (m_hbars, x, y);
       y--);
  y_start = y;

  ho_bitmap_free (m_hbars);

  /* check for no top bar */
  if (y_start == y_end)
    {
      *height = 0.0;
      *start = 0.0;
      *end = 0.0;

      return FALSE;
    }

  /* if start is far from line top check artefacts */
  if (y_end < line_start + 11 * line_height / 12)
    {
      int new_sum = 0;
      int sum = new_sum;

      for (;
	   y_end < line_start + line_height && (new_sum >= sum
						|| new_sum >
						m_text->width / 2); y_end++)
	for (sum = new_sum, new_sum = 0, x = 0; x < m_text->width; x++)
	  new_sum += ho_bitmap_get (m_text, x, y_start);
    }

  /* set height */
  *height = (double) (y_end - y_start) / (double) line_height;

  /* set distance from bottom */
  *dist = (double) (line_start + line_height - y_end) / (double) line_height;

  /* get x start and end of bar */
  {
    int sum = 0;
    for (x = 0; x < m_text->width && sum == 0; x++)
      for (sum = 0, y = y_start; y < y_end; y++)
	sum += ho_bitmap_get (m_text, x, y);

    *start = (double) x / (double) (m_text->width);

    for (; x < m_text->width && sum > 0; x++)
      for (sum = 0, y = y_start; y < y_end; y++)
	sum += ho_bitmap_get (m_text, x, y);

    *end = (double) x / (double) (m_text->width);
  }

  return FALSE;
}

int
ho_recognize_vbar_right (const ho_bitmap * m_text,
			 const ho_bitmap * m_mask, double *width,
			 double *start, double *end, double *dist)
{
  ho_bitmap *m_vbars = NULL;
  int x, y, x_start, x_end, line_start, line_height;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the vbar bitmap */
  m_vbars = ho_font_vbars (m_text, m_mask);
  if (!m_vbars)
    return TRUE;

  /* get start and end of vertical left bar */
  y = line_start + line_height / 2;
  for (x = m_text->width;
       x > m_text->width / 2 && !ho_bitmap_get (m_vbars, x, y); x--);
  x_end = x;
  for (; x > m_text->width / 2 && ho_bitmap_get (m_vbars, x, y); x--);
  x_start = x;

  ho_bitmap_free (m_vbars);

  /* check for no top bar */
  if (x_start == x_end)
    {
      *width = 0.0;
      *start = 0.0;
      *end = 0.0;

      return FALSE;
    }

  /* if start is far from line top check artefacts */
  if (x_end < 11 * m_text->width / 12)
    {
      int new_sum = 0;
      int sum = new_sum;

      for (;
	   x_end < m_text->width && (new_sum >= sum
				     || new_sum > line_height / 2); x_end++)
	for (sum = new_sum, new_sum = 0, y = line_start;
	     y < line_start + line_height; y++)
	  new_sum += ho_bitmap_get (m_text, x_end, y);
    }

  /* set width */
  *width = (double) (x_end - x_start) / (double) m_text->width;

  /* set distance from top */
  *dist = (double) (m_text->width - x_end) / (double) m_text->width;

  /* get y start and end of bar */
  {
    int sum = 0;
    for (y = line_start; y < line_start + line_height && sum == 0; y++)
      for (sum = 0, x = x_start; x < x_end; x++)
	sum += ho_bitmap_get (m_text, x, y);

    *start = (double) (y - line_start) / (double) line_height;

    for (; y < line_start + line_height && sum > 0; y++)
      for (sum = 0, x = x_start; x < x_end; x++)
	sum += ho_bitmap_get (m_text, x, y);

    *end = (double) (y - line_start) / (double) line_height;
  }

  return FALSE;
}

int
ho_recognize_vbar_left (const ho_bitmap * m_text,
			const ho_bitmap * m_mask, double *width,
			double *start, double *end, double *dist)
{
  ho_bitmap *m_vbars = NULL;
  int x, y, x_start, x_end, line_start, line_height;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the vbar bitmap */
  m_vbars = ho_font_vbars (m_text, m_mask);
  if (!m_vbars)
    return TRUE;

  /* get start and end of vertical left bar */
  y = line_start + line_height / 2;
  for (x = 0; x < m_text->width / 2 && !ho_bitmap_get (m_vbars, x, y); x++);
  x_start = x;
  for (; x < m_text->width / 2 && ho_bitmap_get (m_vbars, x, y); x++);
  x_end = x;

  ho_bitmap_free (m_vbars);

  /* check for no top bar */
  if (x_start == x_end)
    {
      *width = 0.0;
      *start = 0.0;
      *end = 0.0;

      return FALSE;
    }

  /* if start is far from line top check artefacts */
  if (x_start > m_text->width / 12)
    {
      int new_sum = 0;
      int sum = new_sum;

      for (;
	   x_start > 0 && (new_sum >= sum
			   || new_sum > line_height / 2); x_start--)
	for (sum = new_sum, new_sum = 0, y = line_start;
	     y < line_start + line_height; y++)
	  new_sum += ho_bitmap_get (m_text, x_start, y);
    }

  /* set width */
  *width = (double) (x_end - x_start) / (double) m_text->width;

  /* set distance from top */
  *dist = (double) (x_start) / (double) m_text->width;

  /* get y start and end of bar */
  {
    int sum = 0;
    for (y = line_start; y < line_start + line_height && sum == 0; y++)
      for (sum = 0, x = x_start; x < x_end; x++)
	sum += ho_bitmap_get (m_text, x, y);

    *start = (double) (y - line_start) / (double) line_height;

    for (; y < line_start + line_height && sum > 0; y++)
      for (sum = 0, x = x_start; x < x_end; x++)
	sum += ho_bitmap_get (m_text, x, y);

    *end = (double) (y - line_start) / (double) line_height;
  }

  return FALSE;
}

int
ho_recognize_dbar (const ho_bitmap * m_text, const ho_bitmap * m_mask,
		   double *width, double *start, double *end)
{
  ho_bitmap *m_dbar = NULL;
  int x, y, x_start, x_end, line_start, line_height;
  int sum;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the vbar bitmap */
  m_dbar = ho_font_diagonal (m_text, m_mask);
  if (!m_dbar)
    return TRUE;

  /* get start and end of vertical left bar */
  sum = 0;
  for (x = 0; x < m_text->width && sum == 0; x++)
    for (sum = 0, y = 0; y < m_text->height; y++)
      sum += ho_bitmap_get (m_dbar, x, y);

  x_start = x;

  for (; x < m_text->width && sum > 0; x++)
    for (sum = 0, y = 0; y < m_text->height; y++)
      sum += ho_bitmap_get (m_dbar, x, y);

  x_end = x;

  ho_bitmap_free (m_dbar);

  /* check for no top bar */
  if (x_start == x_end)
    {
      *width = 0.0;
      *start = 0.0;
      *end = 0.0;

      return FALSE;
    }

  *width = (double) (x_end - x_start) / (double) (m_text->width);;
  *start = (double) x_start / (double) (m_text->width);
  *end = (double) x_end / (double) (m_text->width);

  return FALSE;
}

int
ho_recognize_dbar_left (const ho_bitmap * m_text, const ho_bitmap * m_mask,
			double *width, double *start, double *end)
{
  ho_bitmap *m_dbar = NULL;
  int x, y, x_start, x_end, line_start, line_height;
  int sum;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the vbar bitmap */
  m_dbar = ho_font_diagonal_left (m_text, m_mask);
  if (!m_dbar)
    return TRUE;

  /* get start and end of vertical left bar */
  sum = 0;
  for (x = 0; x < m_text->width && sum == 0; x++)
    for (sum = 0, y = 0; y < m_text->height; y++)
      sum += ho_bitmap_get (m_dbar, x, y);

  x_start = x;

  for (; x < m_text->width && sum > 0; x++)
    for (sum = 0, y = 0; y < m_text->height; y++)
      sum += ho_bitmap_get (m_dbar, x, y);

  x_end = x;

  ho_bitmap_free (m_dbar);

  /* check for no top bar */
  if (x_start == x_end)
    {
      *width = 0.0;
      *start = 0.0;
      *end = 0.0;

      return FALSE;
    }

  *width = (double) (x_end - x_start) / (double) (m_text->width);;
  *start = (double) x_start / (double) (m_text->width);
  *end = (double) x_end / (double) (m_text->width);

  return FALSE;
}

int
ho_recognize_edges_top (const ho_bitmap * m_text, const ho_bitmap * m_mask,
			int *num, int *left, int *middle, int *right)
{
  ho_bitmap *m_edges = NULL;
  int x, y, x_start, x_end, line_start, line_height;
  int sum;

  /* get the top edges */
  m_edges = ho_font_edges_top (m_text, m_mask);
  if (!m_edges)
    return TRUE;

  /* get lines */
  y = 1;
  x = 1;
  (*num) = (*left) = (*middle) = (*right) = 0;

  while (x < m_mask->width)
    {
      for (; x < m_mask->width && !ho_bitmap_get (m_edges, x, y); x++);
      if (x < m_mask->width)
	{
	  (*num)++;
	  if (x < m_mask->width / 3)
	    (*left)++;
	  else if (x < 2 * m_mask->width / 3)
	    (*middle)++;
	  else
	    (*right)++;
	}
      for (; x < m_mask->width && ho_bitmap_get (m_edges, x, y); x++);
    }

  return FALSE;
}

int
ho_recognize_edges_bottom (const ho_bitmap * m_text, const ho_bitmap * m_mask,
			   int *num, int *left, int *middle, int *right)
{
  ho_bitmap *m_edges = NULL;
  int x, y, x_start, x_end, line_start, line_height;
  int sum;

  /* get the top edges */
  m_edges = ho_font_edges_bottom (m_text, m_mask);
  if (!m_edges)
    return TRUE;

  /* get lines */
  y = m_edges->height - 2;
  x = 1;
  (*num) = (*left) = (*middle) = (*right) = 0;

  while (x < m_mask->width)
    {
      for (; x < m_mask->width && !ho_bitmap_get (m_edges, x, y); x++);
      if (x < m_mask->width)
	{
	  (*num)++;
	  if (x < m_mask->width / 3)
	    (*left)++;
	  else if (x < 2 * m_mask->width / 3)
	    (*middle)++;
	  else
	    (*right)++;
	}

      for (; x < m_mask->width && ho_bitmap_get (m_edges, x, y); x++);
    }

  return FALSE;
}

int
ho_recognize_edges_left (const ho_bitmap * m_text, const ho_bitmap * m_mask,
			 int *num, int *top, int *middle, int *bottom)
{
  ho_bitmap *m_edges = NULL;
  int x, y, x_start, x_end, line_start, line_height;
  int sum;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > line_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_height = y - line_start + 1;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the top edges */
  m_edges = ho_font_edges_left (m_text, m_mask);
  if (!m_edges)
    return TRUE;

  /* get lines */
  y = line_start;
  x = 1;
  (*num) = (*top) = (*middle) = (*bottom) = 0;

  while (y < line_start + line_height)
    {
      for (; y < line_start + line_height && !ho_bitmap_get (m_edges, x, y);
	   y++);
      if (y < line_start + line_height)
	{
	  (*num)++;
	  if (y < line_start + line_height / 3)
	    (*top)++;
	  else if (y < line_start + 2 * line_height / 3)
	    (*middle)++;
	  else
	    (*bottom)++;
	}

      for (; y < line_start + line_height && ho_bitmap_get (m_edges, x, y);
	   y++);
    }

  return FALSE;
}

int
ho_recognize_edges_right (const ho_bitmap * m_text, const ho_bitmap * m_mask,
			  int *num, int *top, int *middle, int *bottom)
{
  ho_bitmap *m_edges = NULL;
  int x, y, x_start, x_end, line_start, line_height;
  int sum;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > line_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_height = y - line_start + 1;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the top edges */
  m_edges = ho_font_edges_right (m_text, m_mask);
  if (!m_edges)
    return TRUE;

  /* get lines */
  y = line_start;
  x = m_mask->width - 2;
  (*num) = (*top) = (*middle) = (*bottom) = 0;

  while (y < line_start + line_height)
    {
      for (; y < line_start + line_height && !ho_bitmap_get (m_edges, x, y);
	   y++);
      if (y < line_start + line_height)
	{
	  (*num)++;
	  if (y < line_start + line_height / 3)
	    (*top)++;
	  else if (y < line_start + 2 * line_height / 3)
	    (*middle)++;
	  else
	    (*bottom)++;
	}

      for (; y < line_start + line_height && ho_bitmap_get (m_edges, x, y);
	   y++);
    }

  return FALSE;
}

int
ho_recognize_dimentions (const ho_bitmap * m_text, const ho_bitmap * m_mask,
			 double *height, double *width, double *start,
			 double *end)
{
  int x, y, x_start, x_end, line_start, line_height;
  int font_start, font_height;
  int sum;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > font_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_height = y - font_start + 1;

  *height = (double) font_height / (double) line_height;
  *width = (double) (m_mask->width) / (double) line_height;
  *start = (double) (line_start - font_start) / (double) line_height;
  *end =
    (double) ((line_start + line_height) -
	      (font_start + font_height)) / (double) line_height;

  return FALSE;
}

int
ho_recognize_create_array_in (const ho_bitmap * m_text,
			      const ho_bitmap * m_mask, double *array_in)
{
  ho_bitmap *m_holes = NULL;

  double height, width, start, end, dist;
  int num, top, middle, bottom, left, right;
  int nobjects, nholes;

  /* fill the bars part of array */
  ho_recognize_hbar_up (m_text, m_mask, &height, &start, &end, &dist);
  array_in[0] = 2.0 * height - 1.0;
  array_in[1] = 2.0 * start - 1.0;
  array_in[2] = 2.0 * end - 1.0;
  array_in[3] = 2.0 * dist - 1.0;

  ho_recognize_hbar_down (m_text, m_mask, &height, &start, &end, &dist);
  array_in[4] = 2.0 * height - 1.0;
  array_in[5] = 2.0 * start - 1.0;
  array_in[6] = 2.0 * end - 1.0;
  array_in[7] = 2.0 * dist - 1.0;

  ho_recognize_vbar_left (m_text, m_mask, &width, &start, &end, &dist);
  array_in[8] = 2.0 * width - 1.0;
  array_in[9] = 2.0 * start - 1.0;
  array_in[10] = 2.0 * end - 1.0;
  array_in[11] = 2.0 * dist - 1.0;

  ho_recognize_vbar_right (m_text, m_mask, &width, &start, &end, &dist);
  array_in[12] = 2.0 * width - 1.0;
  array_in[13] = 2.0 * start - 1.0;
  array_in[14] = 2.0 * end - 1.0;
  array_in[15] = 2.0 * dist - 1.0;

  /* fill the diagonals part of array */
  ho_recognize_dbar (m_text, m_mask, &width, &start, &end);
  array_in[16] = 2.0 * width - 1.0;
  array_in[17] = 2.0 * start - 1.0;
  array_in[18] = 2.0 * end - 1.0;

  ho_recognize_dbar_left (m_text, m_mask, &width, &start, &end);
  array_in[19] = 2.0 * width - 1.0;
  array_in[20] = 2.0 * start - 1.0;
  array_in[21] = 2.0 * end - 1.0;

  /* fill the egdes part of array */
  ho_recognize_edges_top (m_text, m_mask, &num, &left, &middle, &right);
  array_in[22] = (num < 3) ? -1.0 : 1.0;
  array_in[23] = (left < 1) ? -1.0 : 1.0;
  array_in[24] = (middle < 1) ? -1.0 : 1.0;
  array_in[25] = (right < 1) ? -1.0 : 1.0;

  ho_recognize_edges_bottom (m_text, m_mask, &num, &left, &middle, &right);
  array_in[26] = (num < 3) ? -1.0 : 1.0;
  array_in[27] = (left < 1) ? -1.0 : 1.0;
  array_in[28] = (middle < 1) ? -1.0 : 1.0;
  array_in[29] = (right < 1) ? -1.0 : 1.0;

  ho_recognize_edges_left (m_text, m_mask, &num, &top, &middle, &bottom);
  array_in[30] = (num < 3) ? -1.0 : 1.0;
  array_in[31] = (top < 1) ? -1.0 : 1.0;
  array_in[32] = (middle < 1) ? -1.0 : 1.0;
  array_in[33] = (bottom < 1) ? -1.0 : 1.0;

  ho_recognize_edges_right (m_text, m_mask, &num, &top, &middle, &bottom);
  array_in[34] = (num < 3) ? -1.0 : 1.0;
  array_in[35] = (top < 1) ? -1.0 : 1.0;
  array_in[36] = (middle < 1) ? -1.0 : 1.0;
  array_in[37] = (bottom < 1) ? -1.0 : 1.0;

  /* fill the dimentions part */
  ho_recognize_dimentions (m_text, m_mask, &height, &width, &start, &end);
  array_in[38] = height - 1.0;
  if (array_in[38] < -1.0)
    array_in[38] = -1.0;
  else if (array_in[38] > 1.0)
    array_in[38] = 1.0;

  array_in[39] = width - 1.0;
  if (array_in[39] < -1.0)
    array_in[39] = -1.0;
  else if (array_in[39] > 1.0)
    array_in[39] = 1.0;

  array_in[40] = start;
  if (array_in[40] < -1.0)
    array_in[40] = -1.0;
  else if (array_in[40] > 1.0)
    array_in[40] = 1.0;

  array_in[41] = end;
  if (array_in[41] < -1.0)
    array_in[41] = -1.0;
  else if (array_in[41] > 1.0)
    array_in[41] = 1.0;

  /* fill the objects and holes part */
  nobjects = ho_bitmap_filter_count_objects (m_text);
  array_in[42] = (nobjects > 1) ? 1.0 : -1.0;

  m_holes = ho_font_holes (m_text, m_mask);
  if (m_holes)
    {
      nholes = ho_bitmap_filter_count_objects (m_holes);
      array_in[43] = (nholes > 0) ? 1.0 : -1.0;
      array_in[44] = (nholes > 1) ? 1.0 : -1.0;

      ho_bitmap_free (m_holes);
    }
  else
    {
      array_in[43] = -1.0;
      array_in[44] = -1.0;
    }

  return FALSE;
}

int
ho_recognize_create_array_out (const double *array_in, double *array_out)
{
  int i;

  /* clean array out */
  for (i = 0; i < HO_ARRAY_OUT_SIZE; i++)
    array_out[i] = -1.0;

  /* fill array out */

  /* dot */
  array_out[28] = ho_recognize_sign_dot (array_in);

  /* comma */
  array_out[29] = ho_recognize_sign_comma (array_in);

  /* minus */
  array_out[37] = ho_recognize_sign_minus (array_in);

  /* hebrew fonts */
  array_out[1] = ho_recognize_sign_alef (array_in);
  array_out[2] = ho_recognize_sign_bet (array_in);
  array_out[3] = ho_recognize_sign_gimal (array_in);
  array_out[4] = ho_recognize_sign_dalet (array_in);
  array_out[5] = ho_recognize_sign_hey (array_in);
  array_out[6] = ho_recognize_sign_vav (array_in);
  array_out[7] = ho_recognize_sign_zayin (array_in);
  array_out[8] = ho_recognize_sign_het (array_in);
  array_out[9] = ho_recognize_sign_tet (array_in);
  array_out[10] = ho_recognize_sign_yod (array_in);
  array_out[11] = ho_recognize_sign_caf (array_in);
  array_out[12] = ho_recognize_sign_caf_sofit (array_in);
  array_out[13] = ho_recognize_sign_lamed (array_in);
  array_out[14] = ho_recognize_sign_mem (array_in);
  array_out[15] = ho_recognize_sign_mem_sofit (array_in);
  array_out[16] = ho_recognize_sign_nun (array_in);
  array_out[17] = ho_recognize_sign_nun_sofit (array_in);
  array_out[18] = ho_recognize_sign_samech (array_in);
  array_out[19] = ho_recognize_sign_ayin (array_in);
  array_out[20] = ho_recognize_sign_pey (array_in);
  array_out[21] = ho_recognize_sign_pey_sofit (array_in);
  array_out[22] = ho_recognize_sign_tzadi (array_in);
  array_out[23] = ho_recognize_sign_tzadi_sofit (array_in);
  array_out[24] = ho_recognize_sign_kof (array_in);
  array_out[25] = ho_recognize_sign_resh (array_in);
  array_out[26] = ho_recognize_sign_shin (array_in);
  array_out[27] = ho_recognize_sign_tav (array_in);

  return FALSE;
}

char *
ho_recognize_array_out_to_font (const double *array_out)
{
  static char *sign_array[HO_ARRAY_OUT_SIZE] =
    { "*", "א", "ב", "ג", "ד", "ה", "ו", "ז", "ח", "ט", "י", "כ",
    "ך", "ל", "מ", "ם", "נ", "ן", "ס", "ע", "פ", "ף", "צ", "ץ",
    "ק", "ר", "ש", "ת", ".", ",",
    "'", "?", "!", ":", ";", ")", "(", "-"
  };

  int i = 0;
  int max_i = 0;

  for (i = 0; i < HO_ARRAY_OUT_SIZE; i++)
    if (array_out[i] > array_out[max_i])
      max_i = i;

  return sign_array[max_i];
}

char *
ho_recognize_font (const ho_bitmap * m_text, const ho_bitmap * m_mask)
{
  double array_in[HO_ARRAY_IN_SIZE];
  double array_out[HO_ARRAY_OUT_SIZE];
  char *font;
  int i;

  ho_recognize_create_array_in (m_text, m_mask, array_in);
  ho_recognize_create_array_out (array_in, array_out);
  font = ho_recognize_array_out_to_font (array_out);

  return font;
}

#include "ho_recognize.h"
