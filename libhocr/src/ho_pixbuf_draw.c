/***************************************************************************
 *            ho_pixbuf_draw.c
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
#include "ho_pixbuf_draw.h"

int
ho_pixbuf_draw_bitmap (ho_pixbuf * m, const ho_bitmap * bit_in,
		       const unsigned char red, const unsigned char green,
		       const unsigned char blue, const unsigned char alpha)
{
  int x, y;
  unsigned char new_red;
  unsigned char new_green;
  unsigned char new_blue;

  /* check for bitmap origin, if this is a sub window draw at its origin */
  if (bit_in->x != 0 || bit_in->y != 0)
    ho_pixbuf_draw_bitmap_at (m, bit_in, bit_in->x, bit_in->y,
			      red, green, blue, alpha);

  /* sanity check */
  if (m->width != bit_in->width || m->height != bit_in->height)
    return TRUE;

  /* is pixbuf color ? */
  if (m->n_channels < 3)
    {				/* gray scale */
      for (x = 0; x < bit_in->width; x++)
	for (y = 0; y < bit_in->height; y++)
	  if (ho_bitmap_get (bit_in, x, y))
	    {
	      new_red =
		alpha * red / 255 + (255 - alpha) * ho_pixbuf_get (m, x, y,
								   0) / 255;
	      ho_pixbuf_set (m, x, y, 0, new_red);
	    }
    }
  else
    {				/* color */
      for (x = 0; x < bit_in->width; x++)
	for (y = 0; y < bit_in->height; y++)
	  if (ho_bitmap_get (bit_in, x, y))
	    {
	      new_red =
		alpha * red / 255 + (255 - alpha) * ho_pixbuf_get (m, x, y,
								   0) / 255;
	      new_green =
		alpha * green / 255 + (255 - alpha) * ho_pixbuf_get (m, x, y,
								     1) / 255;
	      new_blue =
		alpha * blue / 255 + (255 - alpha) * ho_pixbuf_get (m, x, y,
								    2) / 255;

	      ho_pixbuf_set (m, x, y, 0, new_red);
	      ho_pixbuf_set (m, x, y, 1, new_green);
	      ho_pixbuf_set (m, x, y, 2, new_blue);
	    }
    }

  return FALSE;
}

int
ho_pixbuf_draw_bitmap_at (ho_pixbuf * m, const ho_bitmap * bit_in,
			  const int x1, const int y1,
			  const unsigned char red, const unsigned char green,
			  const unsigned char blue, const unsigned char alpha)
{
  int x, y;
  unsigned char new_red;
  unsigned char new_green;
  unsigned char new_blue;

  /* is pixbuf color ? */
  if (m->n_channels < 3)
    {				/* gray scale */
      for (x = 0; x < bit_in->width; x++)
	for (y = 0; y < bit_in->height; y++)
	  if ((x + x1) >= 0 && (x + x1) < m->width && (y + y1) >= 0
	      && (y + y1) < m->height)
	    if (ho_bitmap_get (bit_in, x, y))
	      {
		new_red =
		  alpha * red / 255 + (255 - alpha) * ho_pixbuf_get (m,
								     (x + x1),
								     (y + y1),
								     0) / 255;
		ho_pixbuf_set (m, (x + x1), (y + y1), 0, new_red);
	      }
    }
  else
    {				/* color */
      for (x = 0; x < bit_in->width; x++)
	for (y = 0; y < bit_in->height; y++)
	  if ((x + x1) >= 0 && (x + x1) < m->width && (y + y1) >= 0
	      && (y + y1) < m->height)
	    if (ho_bitmap_get (bit_in, x, y))
	      {
		new_red =
		  alpha * red / 255 + (255 - alpha) * ho_pixbuf_get (m,
								     (x + x1),
								     (y + y1),
								     0) / 255;
		new_green =
		  alpha * green / 255 + (255 - alpha) * ho_pixbuf_get (m,
								       (x +
									x1),
								       (y +
									y1),
								       1) /
		  255;
		new_blue =
		  alpha * blue / 255 + (255 - alpha) * ho_pixbuf_get (m,
								      (x +
								       x1),
								      (y +
								       y1),
								      2) /
		  255;

		ho_pixbuf_set (m, (x + x1), (y + y1), 0, new_red);
		ho_pixbuf_set (m, (x + x1), (y + y1), 1, new_green);
		ho_pixbuf_set (m, (x + x1), (y + y1), 2, new_blue);
	      }
    }

  return FALSE;
}

int
ho_pixbuf_draw_line (ho_pixbuf * m, const int x1, const int y1,
		     const int x2, const int y2, const unsigned char red,
		     const unsigned char green, const unsigned char blue)
{
  double x, step_x;
  double y, step_y;
  int x_start = x1;
  int x_end = x2;
  int y_start = y1;
  int y_end = y2;

  step_x = ((double) x2 - (double) x1) * ((double) x2 - (double) x1);
  step_y = ((double) y2 - (double) y1) * ((double) y2 - (double) y1);

  if (step_y > step_x)
    {
      /* vertical line */
      if (y1 > y2)
	{
	  x_start = x2;
	  x_end = x1;
	  y_start = y2;
	  y_end = y1;
	}

      x = (double) x_start;
      step_x =
	((double) x_end - (double) x_start) / ((double) y_end -
					       (double) y_start);
      for (y = y_start; y <= y_end; y++)
	{
	  (m->data)[(int) x * m->n_channels + (int) y * m->rowstride] = red;
	  if (m->n_channels >= 3)
	    {
	      (m->data)[(int) x * m->n_channels + (int) y * m->rowstride +
			1] = green;
	      (m->data)[(int) x * m->n_channels + (int) y * m->rowstride +
			2] = blue;
	    }
	  x += step_x;
	}
    }
  else
    {
      /* horizontal line */
      if (x1 > x2)
	{
	  x_start = x2;
	  x_end = x1;
	  y_start = y2;
	  y_end = y1;
	}

      y = (double) y_start;
      step_y =
	((double) y_end - (double) y_start) / ((double) x_end -
					       (double) x_start);
      for (x = x_start; x <= x_end; x++)
	{
	  (m->data)[(int) x * m->n_channels + (int) y * m->rowstride] = red;
	  if (m->n_channels >= 3)
	    {
	      (m->data)[(int) x * m->n_channels + (int) y * m->rowstride +
			1] = green;
	      (m->data)[(int) x * m->n_channels + (int) y * m->rowstride +
			2] = blue;
	    }
	  y += step_y;
	}
    }

  return FALSE;
}

int
ho_pixbuf_draw_horizontal_scale (ho_pixbuf * m, const int x1,
				 const int y1, const int length,
				 const int step, const unsigned char red,
				 const unsigned char green,
				 const unsigned char blue)
{
  int x;

  for (x = 0; x <= length; x++)
    {
      ho_pixbuf_set (m, x1 + x, y1, 0, red);
      if (m->n_channels >= 3)
	{
	  ho_pixbuf_set (m, x1 + x, y1, 1, green);
	  ho_pixbuf_set (m, x1 + x, y1, 2, blue);
	}
      /* draw a vertical line evry step */
      if (!(x % step))
	{
	  ho_pixbuf_set (m, x1 + x, y1 - 4, 0, red);
	  ho_pixbuf_set (m, x1 + x, y1 - 3, 0, red);
	  ho_pixbuf_set (m, x1 + x, y1 - 2, 0, red);
	  ho_pixbuf_set (m, x1 + x, y1 + 2, 0, red);
	  ho_pixbuf_set (m, x1 + x, y1 + 3, 0, red);
	  ho_pixbuf_set (m, x1 + x, y1 + 4, 0, red);

	  if (m->n_channels >= 3)
	    {
	      ho_pixbuf_set (m, x1 + x, y1 - 4, 1, green);
	      ho_pixbuf_set (m, x1 + x, y1 - 3, 1, green);
	      ho_pixbuf_set (m, x1 + x, y1 - 2, 1, green);
	      ho_pixbuf_set (m, x1 + x, y1 + 2, 1, green);
	      ho_pixbuf_set (m, x1 + x, y1 + 3, 1, green);
	      ho_pixbuf_set (m, x1 + x, y1 + 4, 1, green);

	      ho_pixbuf_set (m, x1 + x, y1 - 4, 2, blue);
	      ho_pixbuf_set (m, x1 + x, y1 - 3, 2, blue);
	      ho_pixbuf_set (m, x1 + x, y1 - 2, 2, blue);
	      ho_pixbuf_set (m, x1 + x, y1 + 2, 2, blue);
	      ho_pixbuf_set (m, x1 + x, y1 + 3, 2, blue);
	      ho_pixbuf_set (m, x1 + x, y1 + 4, 2, blue);
	    }
	}
    }

  return FALSE;
}

int
ho_pixbuf_draw_vertical_scale (ho_pixbuf * m, const int x1,
			       const int y1, const int length,
			       const int step, const unsigned char red,
			       const unsigned char green,
			       const unsigned char blue)
{
  int y;

  for (y = 0; y <= length; y++)
    {
      ho_pixbuf_set (m, x1, y1 + y, 0, red);
      if (m->n_channels >= 3)
	{
	  ho_pixbuf_set (m, x1, y1 + y, 1, green);
	  ho_pixbuf_set (m, x1, y1 + y, 2, blue);
	}
      /* draw a vertical line evry step */
      if (!(y % step))
	{
	  ho_pixbuf_set (m, x1 - 4, y1 + y, 0, red);
	  ho_pixbuf_set (m, x1 - 3, y1 + y, 0, red);
	  ho_pixbuf_set (m, x1 - 2, y1 + y, 0, red);
	  ho_pixbuf_set (m, x1 + 2, y1 + y, 0, red);
	  ho_pixbuf_set (m, x1 + 3, y1 + y, 0, red);
	  ho_pixbuf_set (m, x1 + 4, y1 + y, 0, red);

	  if (m->n_channels >= 3)
	    {
	      ho_pixbuf_set (m, x1 - 4, y1 + y, 1, green);
	      ho_pixbuf_set (m, x1 - 3, y1 + y, 1, green);
	      ho_pixbuf_set (m, x1 - 2, y1 + y, 1, green);
	      ho_pixbuf_set (m, x1 + 2, y1 + y, 1, green);
	      ho_pixbuf_set (m, x1 + 3, y1 + y, 1, green);
	      ho_pixbuf_set (m, x1 + 4, y1 + y, 1, green);

	      ho_pixbuf_set (m, x1 - 4, y1 + y, 2, blue);
	      ho_pixbuf_set (m, x1 - 3, y1 + y, 2, blue);
	      ho_pixbuf_set (m, x1 - 2, y1 + y, 2, blue);
	      ho_pixbuf_set (m, x1 + 2, y1 + y, 2, blue);
	      ho_pixbuf_set (m, x1 + 3, y1 + y, 2, blue);
	      ho_pixbuf_set (m, x1 + 4, y1 + y, 2, blue);
	    }
	}
    }

  return FALSE;
}

int
ho_pixbuf_draw_grid (ho_pixbuf * m, const int size, const int step,
		     const unsigned char red, const unsigned char green,
		     const unsigned char blue)
{
  int i;

  /* check fot matrix size */
  if (m->width < 120 || m->height < 120)
    return TRUE;

  /* adding  grid */
  for (i = 60; i < (m->width - 60); i += size)
    {
      ho_pixbuf_draw_vertical_scale (m, i, 30, m->height - 60, step, red,
				     green, blue);
    }

  for (i = 60; i < (m->height - 60); i += size)
    {
      ho_pixbuf_draw_horizontal_scale (m, 30, i, m->width - 60, step, red,
				       green, blue);
    }

  return FALSE;
}
