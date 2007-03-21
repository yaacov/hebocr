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

#include "ho_recognize.h"

int
ho_recognize_hbar_up (const ho_bitmap * m_text, const ho_bitmap * m_mask,
		      double *height, double *start, double *end)
{
  ho_bitmap *m_hbars = NULL;
  int x, y, y_start, y_end, line_start, line_height;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the hbar bitmap */
  m_hbars = ho_font_hbars (m_text, m_mask);
  if (!m_hbars)
    return TRUE;

  /* get start and end of horizontal top bar */
  for (y = line_start;
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
  *height = 1.0 * (double) (y_end - y_start) / (double) line_height;

  /* get x start and end of bar */
  {
    int sum = 0;
    for (x = 0; x < m_text->width && sum == 0; x++)
      for (sum = 0, y = y_start; y < y_end; y++)
	sum += ho_bitmap_get (m_text, x, y);

    *start = 1.0 * (double) x / (double) (m_text->width);

    for (; x < m_text->width && sum > 0; x++)
      for (sum = 0, y = y_start; y < y_end; y++)
	sum += ho_bitmap_get (m_text, x, y);

    *end = 1.0 * (double) x / (double) (m_text->width);
  }

  return FALSE;
}

int
ho_recognize_hbar_down (const ho_bitmap * m_text,
			const ho_bitmap * m_mask, double *height,
			double *start, double *end)
{
  ho_bitmap *m_hbars = NULL;
  int x, y, y_start, y_end, line_start, line_height;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the hbar bitmap */
  m_hbars = ho_font_hbars (m_text, m_mask);
  if (!m_hbars)
    return TRUE;

  /* get start and end of horizontal top bar */
  for (y = line_start + line_height;
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
  *height = 1.0 * (double) (y_end - y_start) / (double) line_height;

  /* get x start and end of bar */
  {
    int sum = 0;
    for (x = 0; x < m_text->width && sum == 0; x++)
      for (sum = 0, y = y_start; y < y_end; y++)
	sum += ho_bitmap_get (m_text, x, y);

    *start = 1.0 * (double) x / (double) (m_text->width);

    for (; x < m_text->width && sum > 0; x++)
      for (sum = 0, y = y_start; y < y_end; y++)
	sum += ho_bitmap_get (m_text, x, y);

    *end = 1.0 * (double) x / (double) (m_text->width);
  }

  return FALSE;
}

int
ho_recognize_vbar_right (const ho_bitmap * m_text,
			 const ho_bitmap * m_mask, double *width,
			 double *start, double *end)
{
  ho_bitmap *m_vbars = NULL;
  int x, y, x_start, x_end, line_start, line_height;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y;
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
  *width = 1.0 * (double) (x_end - x_start) / (double) m_text->width;

  /* get y start and end of bar */
  {
    int sum = 0;
    for (y = line_start; y < line_start + line_height && sum == 0; y++)
      for (sum = 0, x = x_start; x < x_end; x++)
	sum += ho_bitmap_get (m_text, x, y);

    *start = 1.0 * (double) (y - line_start) / (double) line_height;

    for (; y < line_start + line_height && sum > 0; y++)
      for (sum = 0, x = x_start; x < x_end; x++)
	sum += ho_bitmap_get (m_text, x, y);

    *end = 1.0 * (double) (y - line_start) / (double) line_height;
  }

  return FALSE;
}

int
ho_recognize_vbar_left (const ho_bitmap * m_text,
			const ho_bitmap * m_mask, double *width,
			double *start, double *end)
{
  ho_bitmap *m_vbars = NULL;
  int x, y, x_start, x_end, line_start, line_height;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y;
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
  *width = 1.0 * (double) (x_end - x_start) / (double) m_text->width;

  /* get y start and end of bar */
  {
    int sum = 0;
    for (y = line_start; y < line_start + line_height && sum == 0; y++)
      for (sum = 0, x = x_start; x < x_end; x++)
	sum += ho_bitmap_get (m_text, x, y);

    *start = 1.0 * (double) (y - line_start) / (double) line_height;

    for (; y < line_start + line_height && sum > 0; y++)
      for (sum = 0, x = x_start; x < x_end; x++)
	sum += ho_bitmap_get (m_text, x, y);

    *end = 1.0 * (double) (y - line_start) / (double) line_height;
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
  line_start = y;
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
  line_start = y;
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

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

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

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

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
  line_start = y;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_start = y;
  sum = 0;
  for (y = m_mask->height; y > 0 && sum == 0; y--)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_height = y - font_start;

  *height = (double) font_height / (double) line_height;
  *width = (double) (m_mask->width) / (double) line_height;
  *start = (double) (font_start - line_start) / (double) line_height;
  *end =
    (double) (font_start + font_height - line_start -
	      line_height) / (double) line_height;

  return FALSE;
}

int
ho_recognize_array (const ho_bitmap * m_text, const ho_bitmap * m_mask,
		    double *array)
{
  ho_bitmap *m_holes = NULL;

  double height, width, start, end;
  int num, top, middle, bottom, left, right;
  int nobjects, nholes;

  /* fill the bars part of array */
  ho_recognize_hbar_up (m_text, m_mask, &height, &start, &end);
  (array)[0] = 2.0 * height - 1.0;
  (array)[1] = 2.0 * start - 1.0;
  (array)[2] = 2.0 * end - 1.0;

  ho_recognize_hbar_down (m_text, m_mask, &height, &start, &end);
  (array)[3] = 2.0 * height - 1.0;
  (array)[4] = 2.0 * start - 1.0;
  (array)[5] = 2.0 * end - 1.0;

  ho_recognize_vbar_left (m_text, m_mask, &width, &start, &end);
  (array)[6] = 2.0 * width - 1.0;
  (array)[7] = 2.0 * start - 1.0;
  (array)[8] = 2.0 * end - 1.0;

  ho_recognize_vbar_right (m_text, m_mask, &width, &start, &end);
  (array)[9] = 2.0 * width - 1.0;
  (array)[10] = 2.0 * start - 1.0;
  (array)[11] = 2.0 * end - 1.0;

  /* fill the diagonals part of array */
  ho_recognize_dbar (m_text, m_mask, &width, &start, &end);
  (array)[12] = 2.0 * width - 1.0;
  (array)[13] = 2.0 * start - 1.0;
  (array)[14] = 2.0 * end - 1.0;

  ho_recognize_dbar_left (m_text, m_mask, &width, &start, &end);
  (array)[15] = 2.0 * width - 1.0;
  (array)[16] = 2.0 * start - 1.0;
  (array)[17] = 2.0 * end - 1.0;

  /* fill the egdes part of array */
  ho_recognize_edges_top (m_text, m_mask, &num, &left, &middle, &right);
  (array)[18] = (num < 3) ? -1.0 : 1.0;
  (array)[19] = (left < 1) ? -1.0 : 1.0;
  (array)[20] = (middle < 1) ? -1.0 : 1.0;
  (array)[21] = (right < 1) ? -1.0 : 1.0;

  ho_recognize_edges_bottom (m_text, m_mask, &num, &left, &middle, &right);
  (array)[22] = (num < 3) ? -1.0 : 1.0;
  (array)[23] = (left < 1) ? -1.0 : 1.0;
  (array)[24] = (middle < 1) ? -1.0 : 1.0;
  (array)[25] = (right < 1) ? -1.0 : 1.0;

  ho_recognize_edges_left (m_text, m_mask, &num, &top, &middle, &bottom);
  (array)[26] = (num < 3) ? -1.0 : 1.0;
  (array)[27] = (top < 1) ? -1.0 : 1.0;
  (array)[28] = (middle < 1) ? -1.0 : 1.0;
  (array)[29] = (bottom < 1) ? -1.0 : 1.0;

  ho_recognize_edges_right (m_text, m_mask, &num, &top, &middle, &bottom);
  (array)[30] = (num < 3) ? -1.0 : 1.0;
  (array)[31] = (top < 1) ? -1.0 : 1.0;
  (array)[32] = (middle < 1) ? -1.0 : 1.0;
  (array)[33] = (bottom < 1) ? -1.0 : 1.0;

  /* fill the dimentions part */
  ho_recognize_dimentions (m_text, m_mask, &height, &width, &start, &end);
  (array)[34] = height - 1.0;
  if ((array)[34] < -1.0)
    (array)[34] = -1.0;
  else if ((array)[34] > 1.0)
    (array)[34] = 1.0;

  (array)[35] = width - 1.0;
  if ((array)[35] < -1.0)
    (array)[35] = -1.0;
  else if ((array)[35] > 1.0)
    (array)[35] = 1.0;

  (array)[36] = start;
  if ((array)[36] < -1.0)
    (array)[36] = -1.0;
  else if ((array)[36] > 1.0)
    (array)[36] = 1.0;

  (array)[37] = end;
  if ((array)[37] < -1.0)
    (array)[37] = -1.0;
  else if ((array)[37] > 1.0)
    (array)[37] = 1.0;

  /* fill the objects and holes part */
  nobjects = ho_bitmap_filter_count_objects (m_text);
  (array)[38] = (nobjects > 1) ? 1.0 : -1.0;

  m_holes = ho_font_holes (m_text, m_mask);
  if (m_holes)
    {
      nholes = ho_bitmap_filter_count_objects (m_holes);
      (array)[39] = (nholes > 0) ? 1.0 : -1.0;
      (array)[40] = (nholes > 1) ? 1.0 : -1.0;

      ho_bitmap_free (m_holes);
    }
  else
    {
      (array)[39] = -1.0;
      (array)[40] = -1.0;
    }

  return FALSE;
}

#include "ho_recognize.h"
