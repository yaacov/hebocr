/***************************************************************************
 *            ho_bitmap_draw.c
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

#include "ho_bitmap_draw.h"

int
ho_bitmap_draw_box (ho_bitmap * m, const int x, const int y,
		    const int width, const int height)
{
  int x1, y1;

  /*  draw */
  for (x1 = x; x1 < (x + width) && x1 < m->width; x1++)
    for (y1 = y; y1 < (y + height) && y1 < m->height; y1++)
      ho_bitmap_set (m, x1, y1);

  return FALSE;
}

int
ho_bitmap_draw_vline (ho_bitmap * m, const int x, const int y,
		      const int height)
{
  int ix = x, iy = y;
  int y1;

  /* sanity check */
  if (ix > m->width)
    ix = m->width - 1;
  if (iy > m->height)
    iy = m->height - 1;

  /*  draw */
  for (y1 = iy; y1 < (iy + height) && y1 < m->height; y1++)
    {
      ho_bitmap_set (m, ix, y1);
    }

  return FALSE;
}

int
ho_bitmap_delete_vline (ho_bitmap * m, const int x, const int y,
			const int height)
{
  int ix = x, iy = y;
  int y1;

  /* sanity check */
  if (ix > m->width)
    ix = m->width - 1;
  if (iy > m->height)
    iy = m->height - 1;

  /*  draw */
  for (y1 = iy; y1 < (iy + height) && y1 < m->height; y1++)
    ho_bitmap_unset (m, ix, y1);

  return FALSE;
}

int
ho_bitmap_draw_hline (ho_bitmap * m, const int x, const int y,
		      const int width)
{
  int ix = x, iy = y;
  int x1;

  /* sanity check */
  if (ix > m->width)
    ix = m->width - 1;
  if (iy > m->height)
    iy = m->height - 1;

  /*  draw */
  for (x1 = x; x1 < (x + width) && x1 < m->width; x1++)
    ho_bitmap_set (m, x1, y);

  return FALSE;
}

int
ho_bitmap_delete_hline (ho_bitmap * m, const int x, const int y,
			const int width)
{
  int ix = x, iy = y;
  int x1;

  /* sanity check */
  if (ix > m->width)
    ix = m->width - 1;
  if (iy > m->height)
    iy = m->height - 1;

  /*  draw */
  for (x1 = x; x1 < (x + width) && x1 < m->width; x1++)
    ho_bitmap_unset (m, x1, y);

  return FALSE;
}

int
ho_bitmap_draw_box_empty (ho_bitmap * m, const int x, const int y,
			  const int width, const int height)
{
  ho_bitmap_draw_hline (m, x, y, width);
  ho_bitmap_draw_hline (m, x, y + m->height - 1, width);
  ho_bitmap_draw_vline (m, x, y, height);
  ho_bitmap_draw_vline (m, x + m->width - 1, y, height);

  return FALSE;
}
