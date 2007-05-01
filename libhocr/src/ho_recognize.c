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
			 double *width, double *height_by_width, double *top,
			 double *bottom)
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
  *height_by_width = (double) font_height / (double) (m_text->width);

  *top = (double) (line_start - font_start) / (double) line_height;
  *bottom = (double) (line_end - font_end) / (double) line_height;

  return FALSE;
}

int
ho_recognize_bars (const ho_bitmap * m_text,
		   const ho_bitmap * m_mask, double *has_top_bar,
		   double *has_bottom_bar, double *has_left_bar,
		   double *has_right_bar, double *has_diagonal_bar)
{
  int sum, x, y;
  int font_start;
  int font_end;
  int font_height;
  int line_start;
  int line_end;
  int line_height;

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

  /* get horizontal bars */
  m_bars = ho_font_hbars (m_text, m_mask);
  if (!m_bars)
    return TRUE;

  /* look for a bar in top 1/4 of font */
  *has_top_bar = 0.0;
  x = m_bars->width / 2;
  for (y = font_start;
       y < font_start + font_height / 4 && !ho_bitmap_get (m_bars, x, y);
       y++);
  if (ho_bitmap_get (m_bars, x, y) && y < font_start + font_height / 4)
    *has_top_bar = 1.0;

  /* look for a bar in bottom 1/4 of font */
  *has_bottom_bar = 0.0;
  x = m_bars->width / 2;
  for (y = font_end;
       y > font_end - font_height / 4 && !ho_bitmap_get (m_bars, x, y); y--);
  if (ho_bitmap_get (m_bars, x, y) && y > font_end - font_height / 4)
    *has_bottom_bar = 1.0;

  /* get vertical bars */
  ho_bitmap_free (m_bars);
  m_bars = ho_font_vbars (m_text, m_mask);
  if (!m_bars)
    return TRUE;

  /* look for a bar in left 1/4 of font */
  *has_left_bar = 0.0;
  y = m_bars->height / 2;
  for (x = 0; x < m_bars->width / 4 && !ho_bitmap_get (m_bars, x, y); x++);
  if (ho_bitmap_get (m_bars, x, y) && x < m_bars->width / 4)
    *has_left_bar = 1.0;

  /* look for a bar in right 1/4 of font */
  *has_right_bar = 0.0;
  y = m_bars->height / 2;
  for (x = m_bars->width - 1;
       x > 3 * m_bars->width / 4 && !ho_bitmap_get (m_bars, x, y); x--);
  if (ho_bitmap_get (m_bars, x, y) && x > 3 * m_bars->width / 4)
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
  double height_by_width;
  double top;
  double bottom;

  double has_top_bar;
  double has_bottom_bar;
  double has_left_bar;
  double has_right_bar;
  double has_diagonal_bar;

  for (i = 0; i < HO_ARRAY_IN_SIZE; i++)
    array_in[i] = 0.0;

  ho_recognize_dimentions (m_text,
			   m_mask, &height,
			   &width, &height_by_width, &top, &bottom);

  array_in[0] = height;
  array_in[1] = width;
  array_in[2] = height_by_width;
  array_in[3] = top;
  array_in[4] = bottom;

  ho_recognize_bars (m_text,
		     m_mask, &has_top_bar,
		     &has_bottom_bar, &has_left_bar,
		     &has_right_bar, &has_diagonal_bar);

  array_in[5] = has_top_bar;
  array_in[6] = has_bottom_bar;
  array_in[7] = has_left_bar;
  array_in[8] = has_right_bar;
  array_in[9] = has_diagonal_bar;

  return 0;
}

double
ho_recognize_font_dot (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the bottom */
  if (array_in[0] < 0.4 && array_in[2] > 0.6 && array_in[2] < 1.2
      && array_in[3] < -0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_comma (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the bottom */
  if (array_in[0] < 0.5 && array_in[2] > 1.1 && array_in[3] < -0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_minus (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the bottom */
  if (array_in[2] < 0.25)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_tag (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the bottom */
  if (array_in[0] < 0.6 && array_in[2] > 1.0 && array_in[4] > 0.4)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_alef (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the bottom */
  if (array_in[0] < 1.15 && array_in[0] > 0.85 && array_in[2] < 1.3
      && array_in[2] > 0.7 && array_in[6] < 0.5 && array_in[9] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_lamed (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the bottom */
  if (array_in[0] > 1.1 && array_in[2] > 1.1 && array_in[3] > 0.1)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_vav (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the bottom */
  if (array_in[0] < 1.05 && array_in[0] > 0.85 && array_in[2] > 1.95
      && array_in[3] < 0.05 && array_in[3] > -0.05)
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
    case 6:			/* vav */
      return_value = ho_recognize_font_vav (array_in);
      break;
    case 13:			/* lamed */
      return_value = ho_recognize_font_lamed (array_in);
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
