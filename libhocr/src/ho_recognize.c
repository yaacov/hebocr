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
#include "ho_segment.h"
#include "ho_font.h"

#include "ho_recognize.h"

int
ho_recognize_array_in_size ()
{
  return HO_ARRAY_IN_SIZE;
}

int
ho_recognize_array_out_size ()
{
  return HO_ARRAY_OUT_SIZE;
}

int
ho_recognize_dimentions (const ho_bitmap * m_text,
			 const ho_bitmap * m_mask, double *height,
			 double *width, double *top, double *bottom,
			 double *top_left, double *top_right,
			 double *bottom_left, double *bottom_right)
{
  int sum, x, y;
  int font_start;
  int font_end;
  int font_height;
  int line_start;
  int line_end;
  int line_height;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_end = y;
  line_height = line_end - line_start;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_text->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > font_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_text->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_end = y + 1;
  font_height = font_end - font_start;

  if (!font_height || !line_height)
    return TRUE;

  *height = (double) font_height / (double) line_height;
  *width = (double) (m_text->width) / (double) line_height;

  *top = (double) (line_start - font_start) / (double) line_height;
  *bottom = (double) (line_end - font_end) / (double) line_height;

  /* check if  font is not just a little off line */
  if (*top < 0.1 && *top > -0.1)
    {
      /* regular size  */
      if (*height < 1.1 && *height > 0.9)
	{
	  *top = *bottom = 0.0;
	}
      /* short and high font */
      else
	{
	  *bottom -= *top;
	  *top = 0.0;
	}
    }
  else if (*bottom < 0.1 && *bottom > -0.1)
    {
      *top -= *bottom;
      *bottom = 0.0;
    }

  /* all values are 0..1 */
  *height = *height / 2.0;
  if (*height > 1.0)
    *height = 1.0;

  *width = *width / 2.0;
  if (*width > 1.0)
    *width = 1.0;

  *top = (*top + 1.0) / 2.0;
  if (*top > 1.0)
    *top = 1.0;
  if (*top < 0.0)
    *top = 0.0;

  *bottom = (*bottom + 1.0) / 2.0;
  if (*bottom > 1.0)
    *bottom = 1.0;
  if (*bottom < 0.0)
    *bottom = 0.0;

  /* get font egdes */
  for (y = font_start, x = 0;
       x < m_mask->width / 2 && y < (font_start + font_height / 2)
       && !ho_bitmap_get (m_text, x, y); x++, y++);
  *top_left = (double) x / (double) (m_mask->width / 2);

  if (*top_left > 1.0)
    *top_left = 1.0;

  for (y = font_end, x = 0;
       x < m_mask->width / 2 && y > (font_start + font_height / 2)
       && !ho_bitmap_get (m_text, x, y); x++, y--);
  *bottom_left = (double) x / (double) (m_mask->width / 2);

  if (*bottom_left > 1.0)
    *bottom_left = 1.0;

  for (y = font_start, x = m_text->width;
       x > m_mask->width / 2 && y < (font_start + font_height / 2)
       && !ho_bitmap_get (m_text, x, y); x--, y++);
  *top_right = (double) (m_mask->width - x) / (double) (m_mask->width / 2);

  if (*top_right > 1.0)
    *top_right = 1.0;

  for (y = font_end, x = m_text->width;
       x > m_mask->width / 2 && y > (font_start + font_height / 2)
       && !ho_bitmap_get (m_text, x, y); x--, y--);
  *bottom_right = (double) (m_mask->width - x) / (double) (m_mask->width / 2);

  if (*bottom_right > 1.0)
    *bottom_right = 1.0;

  return FALSE;
}

int
ho_recognize_bars (const ho_bitmap * m_text,
		   const ho_bitmap * m_mask, double *has_top_bar,
		   double *has_bottom_bar, double *has_left_bar,
		   double *has_right_bar, double *has_diagonal_bar,
		   double *has_diagonal_left_bar)
{
  int sum, x, y;
  int font_start;
  int font_end;
  int font_height;
  int line_start;
  int line_end;
  int line_height;
  int font_parts = 4;

  ho_bitmap *m_bars = NULL;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_end = y;
  line_height = line_end - line_start;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_text->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > font_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_text->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_end = y + 1;
  font_height = font_end - font_start;

  if (!font_height || !line_height)
    return TRUE;

  /* if short font it has only two parts (high and low) */
  if (font_height / line_height < 0.6)
    font_parts = 2;

  /* get horizontal bars */
  m_bars = ho_font_hbars (m_text, m_mask);
  if (!m_bars)
    return TRUE;

  /* look for a bar in top 1/4 of font */
  *has_top_bar = 0.0;
  x = m_bars->width / 2;
  for (y = font_start;
       y < font_start + font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y++);
  if (ho_bitmap_get (m_bars, x, y)
      && y < font_start + font_height / font_parts)
    *has_top_bar = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_bottom_bar = 0.0;
  x = m_bars->width / 2;
  for (y = font_end;
       y > font_end - font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y--);
  if (ho_bitmap_get (m_bars, x, y) && y > font_end - font_height / font_parts)
    *has_bottom_bar = 1.0;

  /* get vertical bars */
  ho_bitmap_free (m_bars);
  m_bars = ho_font_vbars (m_text, m_mask);
  if (!m_bars)
    return TRUE;

  /* look for a bar in left 1/4 of font */
  *has_left_bar = 0.0;
  y = m_bars->height / 2;
  for (x = 0; x < m_bars->width / font_parts && !ho_bitmap_get (m_bars, x, y);
       x++);
  if (ho_bitmap_get (m_bars, x, y) && x < m_bars->width / font_parts)
    *has_left_bar = 1.0;

  /* look for a bar in right 1/4 of font */
  *has_right_bar = 0.0;
  y = m_bars->height / 2;
  for (x = m_bars->width - 1;
       x > (font_parts - 1) * m_bars->width / font_parts
       && !ho_bitmap_get (m_bars, x, y); x--);
  if (ho_bitmap_get (m_bars, x, y)
      && x > (font_parts - 1) * m_bars->width / font_parts)
    *has_right_bar = 1.0;

  /* get diagonals */
  ho_bitmap_free (m_bars);
  m_bars = ho_font_diagonal (m_text, m_mask);
  if (!m_bars)
    return TRUE;

  /* look for a bar in left 1/4 of font */
  *has_diagonal_bar = 0.0;
  y = m_bars->height / 2;
  for (x = 0; x < m_bars->width && !ho_bitmap_get (m_bars, x, y); x++);
  if (ho_bitmap_get (m_bars, x, y) && x < m_bars->width)
    *has_diagonal_bar = 1.0;

  /* get diagonals */
  ho_bitmap_free (m_bars);
  m_bars = ho_font_diagonal_left (m_text, m_mask);
  if (!m_bars)
    return TRUE;

  /* look for a bar in left 1/4 of font */
  *has_diagonal_left_bar = 0.0;
  y = m_bars->height / 2;
  for (x = 0; x < m_bars->width && !ho_bitmap_get (m_bars, x, y); x++);
  if (ho_bitmap_get (m_bars, x, y) && x < m_bars->width)
    *has_diagonal_left_bar = 1.0;

  ho_bitmap_free (m_bars);

  return FALSE;
}

int
ho_recognize_edges (const ho_bitmap * m_text,
		    const ho_bitmap * m_mask,
		    double *has_top_left_edge,
		    double *has_mid_left_edge,
		    double *has_bottom_left_edge,
		    double *has_top_right_edge,
		    double *has_mid_right_edge,
		    double *has_bottom_right_edge,
		    double *has_left_top_edge,
		    double *has_mid_top_edge,
		    double *has_right_top_edge,
		    double *has_left_bottom_edge,
		    double *has_mid_bottom_edge,
		    double *has_right_bottom_edge)
{
  int sum, x, y;
  int font_start;
  int font_end;
  int font_height;
  int line_start;
  int line_end;
  int line_height;
  int font_parts = 3;

  ho_bitmap *m_bars = NULL;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_end = y;
  line_height = line_end - line_start;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_text->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > font_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_text->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_end = y + 1;
  font_height = font_end - font_start;

  if (!font_height || !line_height)
    return TRUE;

  /* get horizontal left egdes */
  m_bars = ho_font_edges_left (m_text, m_mask);
  if (!m_bars)
    return TRUE;

  /* if short font it has only two parts (high and low) */
  if (font_height / line_height < 0.6)
    font_parts = 2;

  /* look for a bar in top 1/4 of font */
  *has_top_left_edge = 0.0;
  x = m_bars->width / 2;
  for (y = font_start;
       y < font_start + font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y++);
  if (ho_bitmap_get (m_bars, x, y)
      && y < font_start + font_height / font_parts)
    *has_top_left_edge = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_mid_left_edge = 0.0;
  x = m_bars->width / 2;
  for (y = font_end - 2 * font_height / font_parts;
       y < font_end - font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y++);
  if (ho_bitmap_get (m_bars, x, y) && y < font_end - font_height / font_parts)
    *has_mid_left_edge = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_bottom_left_edge = 0.0;
  x = m_bars->width / 2;
  for (y = font_end;
       y > font_end - font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y--);
  if (ho_bitmap_get (m_bars, x, y) && y > font_end - font_height / font_parts)
    *has_bottom_left_edge = 1.0;

  /* get horizontal right edges */
  ho_bitmap_free (m_bars);
  m_bars = ho_font_edges_right (m_text, m_mask);
  if (!m_bars)
    return TRUE;

  /* look for a bar in top 1/4 of font */
  *has_top_right_edge = 0.0;
  x = m_bars->width / 2;
  for (y = font_start;
       y < font_start + font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y++);
  if (ho_bitmap_get (m_bars, x, y)
      && y < font_start + font_height / font_parts)
    *has_top_right_edge = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_mid_right_edge = 0.0;
  x = m_bars->width / 2;
  for (y = font_end - 2 * font_height / font_parts;
       y < font_end - font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y++);
  if (ho_bitmap_get (m_bars, x, y) && y < font_end - font_height / font_parts)
    *has_mid_right_edge = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_bottom_right_edge = 0.0;
  x = m_bars->width / 2;
  for (y = font_end;
       y > font_end - font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y--);
  if (ho_bitmap_get (m_bars, x, y) && y > font_end - font_height / font_parts)
    *has_bottom_right_edge = 1.0;

  ho_bitmap_free (m_bars);
  m_bars = ho_font_edges_top (m_text, m_mask);
  if (!m_bars)
    return TRUE;

  /* if thin font it has only two parts (high and low) */
  font_parts = 4;
  if (m_text->width / line_height < 0.5)
    font_parts = 2;

  /* look for a bar in left 1/4 of font */
  *has_left_top_edge = 0.0;
  y = m_bars->height / 2;
  for (x = 0;
       x < m_text->width / font_parts && !ho_bitmap_get (m_bars, x, y); x++);
  if (ho_bitmap_get (m_bars, x, y) && x < m_text->width / font_parts)
    *has_left_top_edge = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_mid_top_edge = 0.0;
  y = m_bars->height / 2;
  for (x = (font_parts - 2) * m_text->width / font_parts;
       x < (font_parts - 1) * m_text->width / font_parts
       && !ho_bitmap_get (m_bars, x, y); x++);
  if (ho_bitmap_get (m_bars, x, y)
      && x < (font_parts - 1) * m_text->width / font_parts)
    *has_mid_top_edge = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_right_top_edge = 0.0;
  y = m_bars->height / 2;
  for (x = m_text->width;
       x > (font_parts - 1) * m_text->width / font_parts
       && !ho_bitmap_get (m_bars, x, y); x--);
  if (ho_bitmap_get (m_bars, x, y)
      && x > (font_parts - 1) * m_text->width / font_parts)
    *has_right_top_edge = 1.0;

  /* get horizontal right edges */
  ho_bitmap_free (m_bars);
  m_bars = ho_font_edges_bottom (m_text, m_mask);
  if (!m_bars)
    return TRUE;

  /* look for a bar in left 1/4 of font */
  *has_left_bottom_edge = 0.0;
  y = m_bars->height / 2;
  for (x = 0;
       x < m_text->width / font_parts && !ho_bitmap_get (m_bars, x, y); x++);
  if (ho_bitmap_get (m_bars, x, y) && x < m_text->width / font_parts)
    *has_left_bottom_edge = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_mid_bottom_edge = 0.0;
  y = m_bars->height / 2;
  for (x = (font_parts - 2) * m_text->width / font_parts;
       x < (font_parts - 1) * m_text->width / font_parts
       && !ho_bitmap_get (m_bars, x, y); x--);
  if (ho_bitmap_get (m_bars, x, y)
      && x < (font_parts - 1) * m_text->width / font_parts)
    *has_mid_bottom_edge = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_right_bottom_edge = 0.0;
  y = m_bars->height / 2;
  for (x = m_text->width;
       x > (font_parts - 1) * m_text->width / font_parts
       && !ho_bitmap_get (m_bars, x, y); x--);
  if (ho_bitmap_get (m_bars, x, y)
      && x > (font_parts - 1) * m_text->width / font_parts)
    *has_right_bottom_edge = 1.0;

  ho_bitmap_free (m_bars);

  return FALSE;
}

int
ho_recognize_notches (const ho_bitmap * m_text,
		      const ho_bitmap * m_mask,
		      double *has_top_left_notch,
		      double *has_mid_left_notch,
		      double *has_bottom_left_notch,
		      double *has_top_right_notch,
		      double *has_mid_right_notch,
		      double *has_bottom_right_notch,
		      double *has_left_top_notch,
		      double *has_mid_top_notch,
		      double *has_right_top_notch,
		      double *has_left_bottom_notch,
		      double *has_mid_bottom_notch,
		      double *has_right_bottom_notch)
{
  int sum, x, y;
  int font_start;
  int font_end;
  int font_height;
  int line_start;
  int line_end;
  int line_height;
  int font_parts = 3;

  ho_bitmap *m_bars = NULL;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_end = y;
  line_height = line_end - line_start;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_text->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > font_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_text->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_end = y + 1;
  font_height = font_end - font_start;

  if (!font_height || !line_height)
    return TRUE;

  /* get horizontal left egdes */
  m_bars = ho_font_notch_left (m_text, m_mask);
  if (!m_bars)
    return TRUE;

  /* look for a bar in top 1/4 of font */
  *has_top_left_notch = 0.0;
  x = m_bars->width / 2;
  for (y = font_start;
       y < font_start + font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y++);
  if (ho_bitmap_get (m_bars, x, y)
      && y < font_start + font_height / font_parts)
    *has_top_left_notch = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_mid_left_notch = 0.0;
  x = m_bars->width / 2;
  for (y = font_start + font_height / font_parts;
       y < font_end - font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y++);
  if (ho_bitmap_get (m_bars, x, y) && y < font_end - font_height / font_parts)
    *has_mid_left_notch = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_bottom_left_notch = 0.0;
  x = m_bars->width / 2;
  for (y = font_end;
       y > font_end - font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y--);
  if (ho_bitmap_get (m_bars, x, y) && y > font_end - font_height / font_parts)
    *has_bottom_left_notch = 1.0;

  /* get horizontal right edges */
  ho_bitmap_free (m_bars);
  m_bars = ho_font_notch_right (m_text, m_mask);
  if (!m_bars)
    return TRUE;

  /* look for a bar in top 1/4 of font */
  *has_top_right_notch = 0.0;
  x = m_bars->width / 2;
  for (y = font_start;
       y < font_start + font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y++);
  if (ho_bitmap_get (m_bars, x, y)
      && y < font_start + font_height / font_parts)
    *has_top_right_notch = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_mid_right_notch = 0.0;
  x = m_bars->width / 2;
  for (y = font_start + font_height / font_parts;
       y < font_end - font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y++);
  if (ho_bitmap_get (m_bars, x, y) && y < font_end - font_height / font_parts)
    *has_mid_right_notch = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_bottom_right_notch = 0.0;
  x = m_bars->width / 2;
  for (y = font_end;
       y > font_end - font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y--);
  if (ho_bitmap_get (m_bars, x, y) && y > font_end - font_height / font_parts)
    *has_bottom_right_notch = 1.0;

  ho_bitmap_free (m_bars);
  m_bars = ho_font_notch_top (m_text, m_mask);
  if (!m_bars)
    return TRUE;

  /* look for a bar in left 1/4 of font */
  *has_left_top_notch = 0.0;
  y = m_bars->height / 2;
  for (x = 0;
       x < m_text->width / font_parts && !ho_bitmap_get (m_bars, x, y); x++);
  if (ho_bitmap_get (m_bars, x, y) && x < m_text->width / font_parts)
    *has_left_top_notch = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_mid_top_notch = 0.0;
  y = m_bars->height / 2;
  for (x = m_text->width / font_parts;
       x < (font_parts - 1) * m_text->width / font_parts
       && !ho_bitmap_get (m_bars, x, y); x++);
  if (ho_bitmap_get (m_bars, x, y)
      && x < (font_parts - 1) * m_text->width / font_parts)
    *has_mid_top_notch = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_right_top_notch = 0.0;
  y = m_bars->height / 2;
  for (x = m_text->width;
       x > (font_parts - 1) * m_text->width / font_parts
       && !ho_bitmap_get (m_bars, x, y); x--);
  if (ho_bitmap_get (m_bars, x, y)
      && x > (font_parts - 1) * m_text->width / font_parts)
    *has_right_top_notch = 1.0;

  /* get horizontal right edges */
  ho_bitmap_free (m_bars);
  m_bars = ho_font_notch_bottom (m_text, m_mask);
  if (!m_bars)
    return TRUE;

  /* look for a bar in left 1/4 of font */
  *has_left_bottom_notch = 0.0;
  y = m_bars->height / 2;
  for (x = 0;
       x < m_text->width / font_parts && !ho_bitmap_get (m_bars, x, y); x++);
  if (ho_bitmap_get (m_bars, x, y) && x < m_text->width / font_parts)
    *has_left_bottom_notch = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_mid_bottom_notch = 0.0;
  y = m_bars->height / 2;
  for (x = m_text->width / font_parts;
       x < (font_parts - 1) * m_text->width / font_parts
       && !ho_bitmap_get (m_bars, x, y); x++);
  if (ho_bitmap_get (m_bars, x, y)
      && x < (font_parts - 1) * m_text->width / font_parts)
    *has_mid_bottom_notch = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_right_bottom_notch = 0.0;
  y = m_bars->height / 2;
  for (x = m_text->width;
       x > (font_parts - 1) * m_text->width / font_parts
       && !ho_bitmap_get (m_bars, x, y); x--);
  if (ho_bitmap_get (m_bars, x, y)
      && x > (font_parts - 1) * m_text->width / font_parts)
    *has_right_bottom_notch = 1.0;

  ho_bitmap_free (m_bars);

  return FALSE;
}

int
ho_recognize_parts (const ho_bitmap * m_text,
		    const ho_bitmap * m_mask, double *has_one_hole,
		    double *has_two_holes, double *has_hey_part)
{
  int sum, x, y;
  int font_start;
  int font_end;
  int font_height;
  int line_start;
  int line_end;
  int line_height;

  ho_bitmap *m_parts = NULL;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_end = y;
  line_height = line_end - line_start;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_text->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > font_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_text->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_end = y + 1;
  font_height = font_end - font_start;

  if (!font_height || !line_height)
    return TRUE;

  *has_one_hole = 0.0;
  *has_two_holes = 0.0;

  /* holes */
  m_parts = ho_font_holes (m_text, m_mask);
  if (m_parts)
    {
      sum = ho_bitmap_filter_count_objects (m_parts);
      if (sum == 1)
	*has_one_hole = 1.0;
      else if (sum == 2)
	*has_two_holes = 1.0;
    }

  /* hey and kuf part */
  *has_hey_part = 0.0;

  if (m_parts)
    ho_bitmap_free (m_parts);
  m_parts = ho_font_second_object (m_text, m_mask);
  if (m_parts)
    {
      y = font_end - font_height / 5;

      for (x = 0; x < m_parts->width / 2 && !ho_bitmap_get (m_parts, x, y);
	   x++);
      /* part is in left side of font */
      if (x < m_parts->width / 2)
	{
	  for (; x < m_parts->width / 2 && ho_bitmap_get (m_parts, x, y);
	       x++);
	  /* part ends in left side of font */
	  if (x < m_parts->width / 2)
	    *has_hey_part = 1.0;
	}
    }

  if (m_parts)
    ho_bitmap_free (m_parts);

  return FALSE;
}

int
ho_recognize_ends (const ho_bitmap * m_text,
		   const ho_bitmap * m_mask,
		   double *has_top_left_end,
		   double *has_top_mid_end,
		   double *has_top_right_end,
		   double *has_mid_left_end,
		   double *has_mid_mid_end,
		   double *has_mid_right_end,
		   double *has_bottom_left_end,
		   double *has_bottom_mid_end,
		   double *has_bottom_right_end,
		   double *has_top_left_cross,
		   double *has_top_mid_cross,
		   double *has_top_right_cross,
		   double *has_mid_left_cross,
		   double *has_mid_mid_cross,
		   double *has_mid_right_cross,
		   double *has_bottom_left_cross,
		   double *has_bottom_mid_cross,
		   double *has_bottom_right_cross)
{
  int sum, x, y;
  int font_start;
  int font_end;
  int font_height;
  int line_start;
  int line_end;
  int line_height;

  ho_bitmap *m_parts = NULL;

  /* init values */
  *has_top_left_end = 0.0;
  *has_top_mid_end = 0.0;
  *has_top_right_end = 0.0;

  *has_mid_left_end = 0.0;
  *has_mid_mid_end = 0.0;
  *has_mid_right_end = 0.0;

  *has_bottom_left_end = 0.0;
  *has_bottom_mid_end = 0.0;
  *has_bottom_right_end = 0.0;

  *has_top_left_cross = 0.0;
  *has_top_mid_cross = 0.0;
  *has_top_right_cross = 0.0;

  *has_mid_left_cross = 0.0;
  *has_mid_mid_cross = 0.0;
  *has_mid_right_cross = 0.0;

  *has_bottom_left_cross = 0.0;
  *has_bottom_mid_cross = 0.0;
  *has_bottom_right_cross = 0.0;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_end = y;
  line_height = line_end - line_start;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_text->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > font_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_text->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_end = y + 1;
  font_height = font_end - font_start;

  if (!font_height || !line_height)
    return TRUE;

  /* ends */
  m_parts = ho_font_ends (m_text, m_mask);
  if (!m_parts)
    return TRUE;

  /* top left */
  for (x = 0; x < m_parts->width / 3 && !ho_bitmap_get (m_parts, x, y); x++)
    for (y = font_start;
	 y < font_start + font_height / 3 && !ho_bitmap_get (m_parts, x, y);
	 y++);
  if (ho_bitmap_get (m_parts, x, y))
    *has_top_left_end = 1.0;

  /* top mid */
  for (x = m_parts->width / 3;
       x < 2 * m_parts->width / 3 && !ho_bitmap_get (m_parts, x, y); x++)
    for (y = font_start;
	 y < font_start + font_height / 3 && !ho_bitmap_get (m_parts, x, y);
	 y++);
  if (ho_bitmap_get (m_parts, x, y))
    *has_top_mid_end = 1.0;

  /* top right */
  for (x = 2 * m_parts->width / 3;
       x < m_parts->width && !ho_bitmap_get (m_parts, x, y); x++)
    for (y = font_start;
	 y < font_start + font_height / 3 && !ho_bitmap_get (m_parts, x, y);
	 y++);
  if (ho_bitmap_get (m_parts, x, y))
    *has_top_right_end = 1.0;

  /* mid left */
  for (x = 0; x < m_parts->width / 3 && !ho_bitmap_get (m_parts, x, y); x++)
    for (y = font_start + font_height / 3;
	 y < font_start + 2 * font_height / 3
	 && !ho_bitmap_get (m_parts, x, y); y++);
  if (ho_bitmap_get (m_parts, x, y))
    *has_bottom_left_end = 1.0;

  /* mid mid */
  for (x = m_parts->width / 3;
       x < 2 * m_parts->width / 3 && !ho_bitmap_get (m_parts, x, y); x++)
    for (y = font_start + font_height / 3;
	 y < font_start + 2 * font_height / 3
	 && !ho_bitmap_get (m_parts, x, y); y++);
  if (ho_bitmap_get (m_parts, x, y))
    *has_bottom_mid_end = 1.0;

  /* mid right */
  for (x = 2 * m_parts->width / 3;
       x < m_parts->width && !ho_bitmap_get (m_parts, x, y); x++)
    for (y = font_start + font_height / 3;
	 y < font_start + 2 * font_height / 3
	 && !ho_bitmap_get (m_parts, x, y); y++);
  if (ho_bitmap_get (m_parts, x, y))
    *has_bottom_right_end = 1.0;

  /* bottom left */
  for (x = 0; x < m_parts->width / 3 && !ho_bitmap_get (m_parts, x, y); x++)
    for (y = font_start + 2 * font_height / 3;
	 y < font_start + font_height && !ho_bitmap_get (m_parts, x, y); y++);
  if (ho_bitmap_get (m_parts, x, y))
    *has_bottom_left_end = 1.0;

  /* bottom mid */
  for (x = m_parts->width / 3;
       x < 2 * m_parts->width / 3 && !ho_bitmap_get (m_parts, x, y); x++)
    for (y = font_start + 2 * font_height / 3;
	 y < font_start + font_height && !ho_bitmap_get (m_parts, x, y); y++);
  if (ho_bitmap_get (m_parts, x, y))
    *has_bottom_mid_end = 1.0;

  /* bottom right */
  for (x = 2 * m_parts->width / 3;
       x < m_parts->width && !ho_bitmap_get (m_parts, x, y); x++)
    for (y = font_start + 2 * font_height / 3;
	 y < font_start + font_height && !ho_bitmap_get (m_parts, x, y); y++);
  if (ho_bitmap_get (m_parts, x, y))
    *has_bottom_right_end = 1.0;

  /* crosses */
  ho_bitmap_free (m_parts);
  m_parts = ho_font_cross (m_text, m_mask);
  if (!m_parts)
    return TRUE;

  /* top left */
  for (x = 0; x < m_parts->width / 3 && !ho_bitmap_get (m_parts, x, y); x++)
    for (y = font_start;
	 y < font_start + font_height / 3 && !ho_bitmap_get (m_parts, x, y);
	 y++);
  if (ho_bitmap_get (m_parts, x, y))
    *has_top_left_cross = 1.0;

  /* top mid */
  for (x = m_parts->width / 3;
       x < 2 * m_parts->width / 3 && !ho_bitmap_get (m_parts, x, y); x++)
    for (y = font_start;
	 y < font_start + font_height / 3 && !ho_bitmap_get (m_parts, x, y);
	 y++);
  if (ho_bitmap_get (m_parts, x, y))
    *has_top_mid_cross = 1.0;

  /* top right */
  for (x = 2 * m_parts->width / 3;
       x < m_parts->width && !ho_bitmap_get (m_parts, x, y); x++)
    for (y = font_start;
	 y < font_start + font_height / 3 && !ho_bitmap_get (m_parts, x, y);
	 y++);
  if (ho_bitmap_get (m_parts, x, y))
    *has_top_right_cross = 1.0;

  /* mid left */
  for (x = 0; x < m_parts->width / 3 && !ho_bitmap_get (m_parts, x, y); x++)
    for (y = font_start + font_height / 3;
	 y < font_start + 2 * font_height / 3
	 && !ho_bitmap_get (m_parts, x, y); y++);
  if (ho_bitmap_get (m_parts, x, y))
    *has_bottom_left_cross = 1.0;

  /* mid mid */
  for (x = m_parts->width / 3;
       x < 2 * m_parts->width / 3 && !ho_bitmap_get (m_parts, x, y); x++)
    for (y = font_start + font_height / 3;
	 y < font_start + 2 * font_height / 3
	 && !ho_bitmap_get (m_parts, x, y); y++);
  if (ho_bitmap_get (m_parts, x, y))
    *has_bottom_mid_cross = 1.0;

  /* mid right */
  for (x = 2 * m_parts->width / 3;
       x < m_parts->width && !ho_bitmap_get (m_parts, x, y); x++)
    for (y = font_start + font_height / 3;
	 y < font_start + 2 * font_height / 3
	 && !ho_bitmap_get (m_parts, x, y); y++);
  if (ho_bitmap_get (m_parts, x, y))
    *has_bottom_right_cross = 1.0;

  /* bottom left */
  for (x = 0; x < m_parts->width / 3 && !ho_bitmap_get (m_parts, x, y); x++)
    for (y = font_start + 2 * font_height / 3;
	 y < font_start + font_height && !ho_bitmap_get (m_parts, x, y); y++);
  if (ho_bitmap_get (m_parts, x, y))
    *has_bottom_left_cross = 1.0;

  /* bottom mid */
  for (x = m_parts->width / 3;
       x < 2 * m_parts->width / 3 && !ho_bitmap_get (m_parts, x, y); x++)
    for (y = font_start + 2 * font_height / 3;
	 y < font_start + font_height && !ho_bitmap_get (m_parts, x, y); y++);
  if (ho_bitmap_get (m_parts, x, y))
    *has_bottom_mid_cross = 1.0;

  /* bottom right */
  for (x = 2 * m_parts->width / 3;
       x < m_parts->width && !ho_bitmap_get (m_parts, x, y); x++)
    for (y = font_start + 2 * font_height / 3;
	 y < font_start + font_height && !ho_bitmap_get (m_parts, x, y); y++);
  if (ho_bitmap_get (m_parts, x, y))
    *has_bottom_right_cross = 1.0;

  ho_bitmap_free (m_parts);

  return FALSE;
}

int
ho_recognize_holes_edges (const ho_bitmap * m_text,
			  const ho_bitmap * m_mask,
			  double *has_top_left_edge,
			  double *has_mid_left_edge,
			  double *has_bottom_left_edge,
			  double *has_top_right_edge,
			  double *has_mid_right_edge,
			  double *has_bottom_right_edge,
			  double *has_left_top_edge,
			  double *has_mid_top_edge,
			  double *has_right_top_edge,
			  double *has_left_bottom_edge,
			  double *has_mid_bottom_edge,
			  double *has_right_bottom_edge)
{
  int sum, x, y;
  int font_start;
  int font_end;
  int font_height;
  int font_start_x;
  int font_end_x;
  int font_width;
  int line_start;
  int line_end;
  int line_height;
  int font_parts = 3;

  ho_bitmap *m_bars = NULL;
  ho_bitmap *m_holes = NULL;

  m_holes = ho_font_holes (m_text, m_mask);
  if (!m_holes)
    return TRUE;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_end = y;
  line_height = line_end - line_start;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_text->width; x++)
      sum += ho_bitmap_get (m_holes, x, y);
  font_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > font_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_text->width; x++)
      sum += ho_bitmap_get (m_holes, x, y);
  font_end = y + 1;
  font_height = font_end - font_start;

  if (!font_height || !line_height)
    return TRUE;

  sum = 0;
  for (x = 0; x < m_mask->width && sum == 0; x++)
    for (sum = 0, y = font_start; y < font_end; y++)
      sum += ho_bitmap_get (m_holes, x, y);
  font_start_x = x - 1;
  sum = 0;
  for (x = m_mask->width - 1; x > font_start_x && sum == 0; x--)
    for (sum = 0, y = font_start; y < font_end; y++)
      sum += ho_bitmap_get (m_holes, x, y);
  font_end_x = x + 1;
  font_width = font_end_x - font_start_x;

  if (!font_width)
    return TRUE;

  /* get horizontal left egdes */
  m_bars = ho_font_edges_left (m_holes, m_mask);
  if (!m_bars)
    return TRUE;

  /* if short font it has only two parts (high and low) */
  if (font_height / line_height < 0.6)
    font_parts = 2;

  /* look for a bar in top 1/4 of font */
  *has_top_left_edge = 0.0;
  x = m_bars->width / 2;
  for (y = font_start;
       y < font_start + font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y++);
  if (ho_bitmap_get (m_bars, x, y)
      && y < font_start + font_height / font_parts)
    *has_top_left_edge = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_mid_left_edge = 0.0;
  x = m_bars->width / 2;
  for (y = font_end - 2 * font_height / font_parts;
       y < font_end - font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y++);
  if (ho_bitmap_get (m_bars, x, y) && y < font_end - font_height / font_parts)
    *has_mid_left_edge = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_bottom_left_edge = 0.0;
  x = m_bars->width / 2;
  for (y = font_end;
       y > font_end - font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y--);
  if (ho_bitmap_get (m_bars, x, y) && y > font_end - font_height / font_parts)
    *has_bottom_left_edge = 1.0;

  /* get horizontal right edges */
  ho_bitmap_free (m_bars);
  m_bars = ho_font_edges_right (m_holes, m_mask);
  if (!m_bars)
    return TRUE;

  /* look for a bar in top 1/4 of font */
  *has_top_right_edge = 0.0;
  x = m_bars->width / 2;
  for (y = font_start;
       y < font_start + font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y++);
  if (ho_bitmap_get (m_bars, x, y)
      && y < font_start + font_height / font_parts)
    *has_top_right_edge = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_mid_right_edge = 0.0;
  x = m_bars->width / 2;
  for (y = font_end - 2 * font_height / font_parts;
       y < font_end - font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y++);
  if (ho_bitmap_get (m_bars, x, y) && y < font_end - font_height / font_parts)
    *has_mid_right_edge = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_bottom_right_edge = 0.0;
  x = m_bars->width / 2;
  for (y = font_end;
       y > font_end - font_height / font_parts
       && !ho_bitmap_get (m_bars, x, y); y--);
  if (ho_bitmap_get (m_bars, x, y) && y > font_end - font_height / font_parts)
    *has_bottom_right_edge = 1.0;

  ho_bitmap_free (m_bars);
  m_bars = ho_font_edges_top (m_holes, m_mask);
  if (!m_bars)
    return TRUE;

  /* if thin font it has only two parts (high and low) */
  font_parts = 4;
  if (font_width / line_height < 0.5)
    font_parts = 2;

  /* look for a bar in left 1/4 of font */
  *has_left_top_edge = 0.0;
  y = m_bars->height / 2;
  for (x = 0;
       x < m_text->width / font_parts && !ho_bitmap_get (m_bars, x, y); x++);
  if (ho_bitmap_get (m_bars, x, y) && x < m_text->width / font_parts)
    *has_left_top_edge = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_mid_top_edge = 0.0;
  y = m_bars->height / 2;
  for (x = (font_parts - 2) * m_text->width / font_parts;
       x < (font_parts - 1) * m_text->width / font_parts
       && !ho_bitmap_get (m_bars, x, y); x++);
  if (ho_bitmap_get (m_bars, x, y)
      && x < (font_parts - 1) * m_text->width / font_parts)
    *has_mid_top_edge = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_right_top_edge = 0.0;
  y = m_bars->height / 2;
  for (x = m_text->width;
       x > (font_parts - 1) * m_text->width / font_parts
       && !ho_bitmap_get (m_bars, x, y); x--);
  if (ho_bitmap_get (m_bars, x, y)
      && x > (font_parts - 1) * m_text->width / font_parts)
    *has_right_top_edge = 1.0;

  /* get horizontal right edges */
  ho_bitmap_free (m_bars);
  m_bars = ho_font_edges_bottom (m_holes, m_mask);
  if (!m_bars)
    return TRUE;

  /* look for a bar in left 1/4 of font */
  *has_left_bottom_edge = 0.0;
  y = m_bars->height / 2;
  for (x = 0;
       x < m_text->width / font_parts && !ho_bitmap_get (m_bars, x, y); x++);
  if (ho_bitmap_get (m_bars, x, y) && x < m_text->width / font_parts)
    *has_left_bottom_edge = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_mid_bottom_edge = 0.0;
  y = m_bars->height / 2;
  for (x = (font_parts - 2) * m_text->width / font_parts;
       x < (font_parts - 1) * m_text->width / font_parts
       && !ho_bitmap_get (m_bars, x, y); x--);
  if (ho_bitmap_get (m_bars, x, y)
      && x < (font_parts - 1) * m_text->width / font_parts)
    *has_mid_bottom_edge = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_right_bottom_edge = 0.0;
  y = m_bars->height / 2;
  for (x = m_text->width;
       x > (font_parts - 1) * m_text->width / font_parts
       && !ho_bitmap_get (m_bars, x, y); x--);
  if (ho_bitmap_get (m_bars, x, y)
      && x > (font_parts - 1) * m_text->width / font_parts)
    *has_right_bottom_edge = 1.0;

  ho_bitmap_free (m_bars);

  return FALSE;
}

int
ho_recognize_create_array_in (const ho_bitmap * m_text,
			      const ho_bitmap * m_mask, double *array_in)
{
  int i;

  double height;
  double width;
  double top;
  double bottom;
  double top_left;
  double top_right;
  double bottom_left;
  double bottom_right;

  double has_top_bar;
  double has_bottom_bar;
  double has_left_bar;
  double has_right_bar;
  double has_diagonal_bar;
  double has_diagonal_left_bar;

  double has_top_left_edge;
  double has_mid_left_edge;
  double has_bottom_left_edge;
  double has_top_right_edge;
  double has_mid_right_edge;
  double has_bottom_right_edge;
  double has_left_top_edge;
  double has_mid_top_edge;
  double has_right_top_edge;
  double has_left_bottom_edge;
  double has_mid_bottom_edge;
  double has_right_bottom_edge;

  double has_top_left_notch;
  double has_mid_left_notch;
  double has_bottom_left_notch;
  double has_top_right_notch;
  double has_mid_right_notch;
  double has_bottom_right_notch;
  double has_left_top_notch;
  double has_mid_top_notch;
  double has_right_top_notch;
  double has_left_bottom_notch;
  double has_mid_bottom_notch;
  double has_right_bottom_notch;

  double has_one_hole;
  double has_two_holes;
  double has_hey_part;

  double has_top_left_end;
  double has_top_mid_end;
  double has_top_right_end;
  double has_mid_left_end;
  double has_mid_mid_end;
  double has_mid_right_end;
  double has_bottom_left_end;
  double has_bottom_mid_end;
  double has_bottom_right_end;
  double has_top_left_cross;
  double has_top_mid_cross;
  double has_top_right_cross;
  double has_mid_left_cross;
  double has_mid_mid_cross;
  double has_mid_right_cross;
  double has_bottom_left_cross;
  double has_bottom_mid_cross;
  double has_bottom_right_cross;

  for (i = 0; i < HO_ARRAY_IN_SIZE; i++)
    array_in[i] = 0.0;

  ho_recognize_dimentions (m_text, m_mask,
			   &height, &width, &top, &bottom,
			   &top_left, &top_right,
			   &bottom_left, &bottom_right);

  array_in[0] = height;
  array_in[1] = width;
  array_in[2] = width / height;
  if (array_in[2] > 1.0)
    array_in[2] = 1.0;
  array_in[3] = top;
  array_in[4] = bottom;
  array_in[5] = top_left;
  array_in[6] = top_right;
  array_in[7] = bottom_left;
  array_in[8] = bottom_right;

  ho_recognize_bars (m_text, m_mask,
		     &has_top_bar,
		     &has_bottom_bar,
		     &has_left_bar,
		     &has_right_bar,
		     &has_diagonal_bar, &has_diagonal_left_bar);

  array_in[9] = has_top_bar;
  array_in[10] = has_bottom_bar;
  array_in[11] = has_left_bar;
  array_in[12] = has_right_bar;
  array_in[13] = has_diagonal_bar;
  array_in[14] = has_diagonal_left_bar;

  ho_recognize_edges (m_text, m_mask,
		      &has_top_left_edge,
		      &has_mid_left_edge,
		      &has_bottom_left_edge,
		      &has_top_right_edge,
		      &has_mid_right_edge,
		      &has_bottom_right_edge,
		      &has_left_top_edge,
		      &has_mid_top_edge,
		      &has_right_top_edge,
		      &has_left_bottom_edge,
		      &has_mid_bottom_edge, &has_right_bottom_edge);

  array_in[15] = has_top_left_edge;
  array_in[16] = has_mid_left_edge;
  array_in[17] = has_bottom_left_edge;

  array_in[18] = has_top_right_edge;
  array_in[19] = has_mid_right_edge;
  array_in[20] = has_bottom_right_edge;

  array_in[21] = has_left_top_edge;
  array_in[22] = has_mid_top_edge;
  array_in[23] = has_right_top_edge;

  array_in[24] = has_left_bottom_edge;
  array_in[25] = has_mid_bottom_edge;
  array_in[26] = has_right_bottom_edge;

  ho_recognize_notches (m_text,
			m_mask,
			&has_top_left_notch,
			&has_mid_left_notch,
			&has_bottom_left_notch,
			&has_top_right_notch,
			&has_mid_right_notch,
			&has_bottom_right_notch,
			&has_left_top_notch,
			&has_mid_top_notch,
			&has_right_top_notch,
			&has_left_bottom_notch,
			&has_mid_bottom_notch, &has_right_bottom_notch);

  array_in[27] = has_top_left_notch;
  array_in[28] = has_mid_left_notch;
  array_in[29] = has_bottom_left_notch;

  array_in[30] = has_top_right_notch;
  array_in[31] = has_mid_right_notch;
  array_in[32] = has_bottom_right_notch;

  array_in[33] = has_left_top_notch;
  array_in[34] = has_mid_top_notch;
  array_in[35] = has_right_top_notch;

  array_in[36] = has_left_bottom_notch;
  array_in[37] = has_mid_bottom_notch;
  array_in[38] = has_right_bottom_notch;

  ho_recognize_ends (m_text,
		     m_mask,
		     &has_top_left_end,
		     &has_top_mid_end,
		     &has_top_right_end,
		     &has_mid_left_end,
		     &has_mid_mid_end,
		     &has_mid_right_end,
		     &has_bottom_left_end,
		     &has_bottom_mid_end,
		     &has_bottom_right_end,
		     &has_top_left_cross,
		     &has_top_mid_cross,
		     &has_top_right_cross,
		     &has_mid_left_cross,
		     &has_mid_mid_cross,
		     &has_mid_right_cross,
		     &has_bottom_left_cross,
		     &has_bottom_mid_cross, &has_bottom_right_cross);

  array_in[39] = has_top_left_end;
  array_in[40] = has_top_mid_end;
  array_in[41] = has_top_right_end;

  array_in[42] = has_mid_left_end;
  array_in[43] = has_mid_mid_end;
  array_in[44] = has_mid_right_end;

  array_in[45] = has_bottom_left_end;
  array_in[46] = has_bottom_mid_end;
  array_in[47] = has_bottom_right_end;

  array_in[48] = has_top_left_cross;
  array_in[49] = has_top_mid_cross;
  array_in[50] = has_top_right_cross;

  array_in[51] = has_mid_left_cross;
  array_in[52] = has_mid_mid_cross;
  array_in[53] = has_mid_right_cross;

  array_in[54] = has_bottom_left_cross;
  array_in[55] = has_bottom_mid_cross;
  array_in[56] = has_bottom_right_cross;

  ho_recognize_parts (m_text,
		      m_mask, &has_one_hole, &has_two_holes, &has_hey_part);

  array_in[57] = has_one_hole;
  array_in[58] = has_two_holes;
  array_in[59] = has_hey_part;

  ho_recognize_holes_edges (m_text, m_mask,
			    &has_top_left_edge,
			    &has_mid_left_edge,
			    &has_bottom_left_edge,
			    &has_top_right_edge,
			    &has_mid_right_edge,
			    &has_bottom_right_edge,
			    &has_left_top_edge,
			    &has_mid_top_edge,
			    &has_right_top_edge,
			    &has_left_bottom_edge,
			    &has_mid_bottom_edge, &has_right_bottom_edge);

  array_in[60] = has_top_left_edge;
  array_in[61] = has_mid_left_edge;
  array_in[62] = has_bottom_left_edge;

  array_in[63] = has_top_right_edge;
  array_in[64] = has_mid_right_edge;
  array_in[65] = has_bottom_right_edge;

  array_in[66] = has_left_top_edge;
  array_in[67] = has_mid_top_edge;
  array_in[68] = has_right_top_edge;

  array_in[69] = has_left_bottom_edge;
  array_in[70] = has_mid_bottom_edge;
  array_in[71] = has_right_bottom_edge;

  return 0;
}

double
ho_recognize_font_dot (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the bottom */
  if (array_in[0] < 0.20 && array_in[0] > 0.05 &&
      array_in[1] < 0.20 && array_in[1] > 0.05 &&
      array_in[3] < 0.22 && array_in[3] > 0.10
      && array_in[4] < 0.55 && array_in[4] > 0.35 && array_in[35] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_comma (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the bottom */
  if (array_in[0] < 0.30 && array_in[0] > 0.15 &&
      array_in[1] < 0.20 && array_in[1] > 0.10 &&
      array_in[3] < 0.22 && array_in[3] > 0.10
      && array_in[4] < 0.55 && array_in[4] > 0.35 && array_in[35] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_minus (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the bottom */
  if (array_in[2] < 0.07)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_tag (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the top */
  if (array_in[0] < 0.31 && array_in[0] > 0.18 &&
      array_in[1] < 0.26 && array_in[1] > 0.12 &&
      array_in[3] < 0.58 && array_in[3] > 0.42
      && array_in[4] < 0.85 && array_in[4] > 0.45 && array_in[35] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_alef (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the bottom */
  if (array_in[0] < 1.55 && array_in[0] > 0.42 && array_in[2] < 0.3
      && array_in[2] > 0.2 && array_in[6] < 0.5 && array_in[9] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_bet (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the top */
  if (array_in[0] < 0.55 && array_in[0] > 0.40 &&
      array_in[1] < 0.50 && array_in[1] > 0.30 &&
      array_in[3] < 0.58 && array_in[3] > 0.42 &&
      array_in[4] < 0.58 && array_in[4] > 0.42 &&
      array_in[6] > 0.5 && array_in[13] > 0.5 && array_in[12] < 0.5
      && array_in[37] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_dalet (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the bottom */
  if (array_in[0] < 0.55 && array_in[0] > 0.40 &&
      array_in[1] < 0.55 && array_in[1] > 0.33 &&
      array_in[3] < 0.55 && array_in[3] > 0.45 &&
      array_in[4] < 0.55 && array_in[4] > 0.45 &&
      array_in[12] > 0.5 && array_in[5] > 0.5 && array_in[32] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_hey (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the top */
  if (array_in[0] < 0.55 && array_in[0] > 0.40 &&
      array_in[1] < 0.50 && array_in[1] > 0.30 &&
      array_in[3] < 0.58 && array_in[3] > 0.42 &&
      array_in[4] < 0.58 && array_in[4] > 0.42 &&
      array_in[5] > 0.5 && array_in[32] > 0.5 && array_in[36] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_vav (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the bottom */
  if (array_in[0] < 0.55 && array_in[0] > 0.40 &&
      array_in[1] < 0.31 && array_in[1] > 0.12 &&
      array_in[3] < 0.55 && array_in[3] > 0.45 &&
      array_in[4] < 0.55 && array_in[4] > 0.45 &&
      array_in[12] < 0.5 &&
      array_in[5] > 0.5 && array_in[6] < 0.5 &&
      array_in[7] < 0.5 && array_in[8] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_zayin (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the bottom */
  if (array_in[0] < 0.55 && array_in[0] > 0.40 &&
      array_in[1] < 0.33 && array_in[1] > 0.12 &&
      array_in[3] < 0.55 && array_in[3] > 0.45 &&
      array_in[4] < 0.55 && array_in[4] > 0.45 &&
      array_in[12] > 0.5 && array_in[36] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_yud (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the top */
  if (array_in[0] < 0.31 && array_in[0] > 0.18 &&
      array_in[1] < 0.26 && array_in[1] > 0.12 &&
      array_in[3] < 0.58 && array_in[3] > 0.42
      && array_in[4] < 0.85 && array_in[4] > 0.45 && array_in[35] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_caf (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the top */
  if (array_in[0] < 0.55 && array_in[0] > 0.40 &&
      array_in[1] < 0.50 && array_in[1] > 0.30 &&
      array_in[3] < 0.58 && array_in[3] > 0.42 &&
      array_in[4] < 0.58 && array_in[4] > 0.42 &&
      array_in[6] > 0.5 && array_in[13] < 0.5 && array_in[30] < 0.5 &&
      array_in[15] < 0.5 && array_in[25] < 0.5 && array_in[38] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_lamed (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the bottom */
  if (array_in[0] < 0.75 && array_in[0] > 0.60 &&
      array_in[1] < 0.50 && array_in[1] > 0.25 &&
      array_in[3] < 0.85 && array_in[3] > 0.55 &&
      array_in[4] < 0.58 && array_in[4] > 0.42)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_mem (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the top */
  if (array_in[0] < 0.55 && array_in[0] > 0.40 &&
      array_in[1] < 0.50 && array_in[1] > 0.30 &&
      array_in[3] < 0.58 && array_in[3] > 0.42 &&
      array_in[4] < 0.58 && array_in[4] > 0.42 &&
      array_in[27] > 0.5 && (array_in[24] > 0.5 || array_in[25] > 0.5)
      && array_in[11] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_mem_sofit (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the top */
  if (array_in[0] < 0.55 && array_in[0] > 0.40 &&
      array_in[1] < 0.50 && array_in[1] > 0.30 &&
      array_in[3] < 0.58 && array_in[3] > 0.42 &&
      array_in[4] < 0.58 && array_in[4] > 0.42 &&
      array_in[6] > 0.5 && array_in[30] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_ayin (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the top */
  if (array_in[0] < 0.55 && array_in[0] > 0.40 &&
      array_in[1] < 0.50 && array_in[1] > 0.30 &&
      array_in[3] < 0.58 && array_in[3] > 0.42 &&
      array_in[4] < 0.58 && array_in[4] > 0.42 &&
      array_in[5] < 0.5 &&
      array_in[13] < 0.5 && array_in[30] < 0.5 &&
      array_in[15] > 0.5 && array_in[25] > 0.5 && array_in[38] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_tzadi (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the top */
  if (array_in[0] < 0.55 && array_in[0] > 0.40 &&
      array_in[1] < 0.50 && array_in[1] > 0.30 &&
      array_in[3] < 0.58 && array_in[3] > 0.42 &&
      array_in[4] < 0.58 && array_in[4] > 0.42 &&
      array_in[6] > 0.5 && array_in[13] > 0.5 && array_in[12] > 0.5
      && array_in[37] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_resh (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the bottom */
  if (array_in[0] < 0.55 && array_in[0] > 0.40 &&
      array_in[1] < 0.60 && array_in[1] > 0.31 &&
      array_in[3] < 0.55 && array_in[3] > 0.45 &&
      array_in[4] < 0.55 && array_in[4] > 0.45 &&
      array_in[12] < 0.5 &&
      array_in[5] > 0.5 && array_in[6] < 0.5 &&
      array_in[7] < 0.5 && array_in[8] > 0.5 && array_in[38] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_shin (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the top */
  if (array_in[0] < 0.60 && array_in[0] > 0.40 &&
      array_in[1] < 0.60 && array_in[1] > 0.40 &&
      array_in[3] < 0.60 && array_in[3] > 0.40 &&
      array_in[4] < 0.60 && array_in[4] > 0.40 &&
      array_in[10] > 0.5 && array_in[35] < 0.5 && array_in[38] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_tav (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the top */
  if (array_in[0] < 0.55 && array_in[0] > 0.40 &&
      array_in[1] < 0.55 && array_in[1] > 0.40 &&
      array_in[3] < 0.60 && array_in[3] > 0.40 &&
      array_in[4] < 0.60 && array_in[4] > 0.40 &&
      array_in[11] > 0.5 && array_in[5] > 0.5 &&
      array_in[8] > 0.5 && array_in[28] > 0.5 &&
      array_in[35] > 0.5 && array_in[36] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_array (const double *array_in, const int sign_index)
{
  int i;
  double return_value = 0.0;

  switch (sign_index)
    {
    case 1:			/* alef */
      return_value = ho_recognize_font_alef (array_in);
      break;
    case 2:			/* bet */
      return_value = ho_recognize_font_bet (array_in);
      break;
    case 4:			/* dalet */
      return_value = ho_recognize_font_dalet (array_in);
      break;
    case 5:			/* hey */
      return_value = ho_recognize_font_hey (array_in);
      break;
    case 6:			/* vav */
      return_value = ho_recognize_font_vav (array_in);
      break;
    case 7:			/* zayin */
      return_value = ho_recognize_font_zayin (array_in);
      break;
    case 10:			/* yud */
      return_value = ho_recognize_font_yud (array_in);
      break;
    case 11:			/* caf */
      return_value = ho_recognize_font_caf (array_in);
      break;
    case 13:			/* lamed */
      return_value = ho_recognize_font_lamed (array_in);
      break;
    case 14:			/* mem */
      return_value = ho_recognize_font_mem (array_in);
      break;
    case 15:			/* mem sofit */
      return_value = ho_recognize_font_mem_sofit (array_in);
      break;
    case 19:			/* ayin */
      return_value = ho_recognize_font_ayin (array_in);
      break;
    case 22:			/* tzadi */
      return_value = ho_recognize_font_tzadi (array_in);
      break;
    case 25:			/* resh */
      return_value = ho_recognize_font_resh (array_in);
      break;
    case 26:			/* shin */
      return_value = ho_recognize_font_shin (array_in);
      break;
    case 27:			/* tav */
      return_value = ho_recognize_font_tav (array_in);
      break;
    case 28:			/* dot */
      return_value = ho_recognize_font_dot (array_in);
      break;
    case 29:			/* comma */
      return_value = ho_recognize_font_comma (array_in);
      break;
    case 30:			/* tag */
      return_value = ho_recognize_font_tag (array_in);
      break;
    case 37:			/* minus */
      return_value = ho_recognize_font_minus (array_in);
      break;
    }

  return return_value;
}

int
ho_recognize_create_array_out (const double *array_in, double *array_out)
{
  int i;

  /* set array out */
  array_out[0] = 0.5;

  for (i = 1; i < HO_ARRAY_OUT_SIZE; i++)
    array_out[i] = ho_recognize_array (array_in, i);

  return FALSE;
}

char *
ho_recognize_array_out_to_font (const double *array_out)
{
  int i = 0;
  int max_i = 0;

  for (i = 1; i < HO_ARRAY_OUT_SIZE; i++)
    if (array_out[i] > array_out[max_i])
      max_i = i;

  return ho_sign_array[max_i];
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
