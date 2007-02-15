/***************************************************************************
 *            ho_bitmap.c
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

#include "ho_common.h"
#include "ho_bitmap.h"

ho_bitmap *
ho_bitmap_new (const ho_uint width, const ho_uint height)
{
  ho_bitmap *m_new = HO_NULL;

  /*
   * allocate memory for pixbuf 
   */
  m_new = (ho_bitmap *) malloc (sizeof (ho_bitmap));
  if (!m_new)
    {
      return HO_NULL;
    }

  /*
   * read header 
   */
  m_new->width = width;
  m_new->height = height;
  m_new->rowstride = (width - 1) / 8 + 1;

  /*
   * allocate memory for data (and set to zero)
   */
  m_new->data = calloc (m_new->height * m_new->rowstride, sizeof (ho_uchar));
  if (!(m_new->data))
    {
      free (m_new);
      return HO_NULL;
    }

  return m_new;
}

int
ho_bitmap_free (ho_bitmap * m)
{
  if (!m)
    return HO_TRUE;

  if (m->data)
    free (m->data);

  free (m);

  return HO_FALSE;
}

ho_bitmap *
ho_bitmap_clone (const ho_bitmap * m)
{
  ho_bitmap *m_out;

  /* allocate memory */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return HO_NULL;

  /* copy data */
  memcpy (m_out->data, m->data, m_out->height * m_out->rowstride);

  return m_out;
}

ho_bitmap *
ho_bitmap_clone_window (const ho_bitmap * m, const ho_uint x, const ho_uint y,
			const ho_uint width, const ho_uint height)
{
  ho_bitmap *m_out;
  ho_uint x1, y1;

  /* allocate memory */
  m_out = ho_bitmap_new (width, height);
  if (!m_out)
    return HO_NULL;

  /* copy data */
  for (x1 = 0; x1 < width; x1++)
    for (y1 = 0; y1 < height; y1++)
      {
	if (ho_bitmap_get (m, x + x1, y + y1))
	  ho_bitmap_set (m_out, x1, y1);
      }
  return m_out;
}

int
ho_bitmap_and (ho_bitmap * m_left, const ho_bitmap * m_right)
{
  ho_uint i;

  /* check sizes */
  if (m_left->width != m_right->width || m_left->height != m_right->height)
    return HO_TRUE;

  /* copy data */
  for (i = 0; i < m_right->height * m_right->rowstride; i++)
    m_left->data[i] &= m_right->data[i];

  return HO_FALSE;
}

int
ho_bitmap_or (ho_bitmap * m_left, const ho_bitmap * m_right)
{
  ho_uint i;

  /* check sizes */
  if (m_left->width != m_right->width || m_left->height != m_right->height)
    return HO_TRUE;

  /* copy data */
  for (i = 0; i < m_right->height * m_right->rowstride; i++)
    m_left->data[i] |= m_right->data[i];

  return HO_FALSE;
}

int
ho_bitmap_xor (ho_bitmap * m_left, const ho_bitmap * m_right)
{
  ho_uint i;

  /* check sizes */
  if (m_left->width != m_right->width || m_left->height != m_right->height)
    return HO_TRUE;

  /* copy data */
  for (i = 0; i < m_right->height * m_right->rowstride; i++)
    m_left->data[i] ^= m_right->data[i];

  return HO_FALSE;
}

int
ho_bitmap_andnot (ho_bitmap * m_left, const ho_bitmap * m_right)
{
  ho_uint i;

  /* check sizes */
  if (m_left->width != m_right->width || m_left->height != m_right->height)
    return HO_TRUE;

  /* copy data */
  for (i = 0; i < m_right->height * m_right->rowstride; i++)
    m_left->data[i] &= ~m_right->data[i];

  return HO_FALSE;
}

ho_bitmap *
ho_bitmap_dilation_n (const ho_bitmap * m, const ho_uchar n)
{
  ho_bitmap *m_out;
  ho_uint x, y;
  ho_uchar sum;

  /* allocate memory */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return HO_NULL;

  /*  do dilation */
  for (x = 1; x < m->width - 1; x++)
    for (y = 1; y < m->height - 1; y++)
      {
	/* if white pixel */
	if (!ho_bitmap_get (m, x, y))
	  {
	    sum = ho_bitmap_get (m, x - 1, y - 1) +
	      ho_bitmap_get (m, x - 1, y) +
	      ho_bitmap_get (m, x - 1, y + 1) +
	      ho_bitmap_get (m, x, y - 1) +
	      ho_bitmap_get (m, x, y + 1) +
	      ho_bitmap_get (m, x + 1, y - 1) +
	      ho_bitmap_get (m, x + 1, y) + ho_bitmap_get (m, x + 1, y + 1);
	    if (sum >= n)
	      ho_bitmap_set (m_out, x, y);
	  }
	else
	  ho_bitmap_set (m_out, x, y);
      }

  return m_out;
}

ho_bitmap *
ho_bitmap_erosion_n (const ho_bitmap * m, const ho_uchar n)
{
  ho_bitmap *m_out;
  ho_uint x, y;
  ho_uchar sum;

  /* allocate memory */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return HO_NULL;

  /*  do erosion */
  for (x = 1; x < m->width - 1; x++)
    for (y = 1; y < m->height - 1; y++)
      {
	/* if black pixel */
	if (ho_bitmap_get (m, x, y))
	  {
	    sum = ho_bitmap_get (m, x - 1, y - 1) +
	      ho_bitmap_get (m, x - 1, y) +
	      ho_bitmap_get (m, x - 1, y + 1) +
	      ho_bitmap_get (m, x, y - 1) +
	      ho_bitmap_get (m, x, y + 1) +
	      ho_bitmap_get (m, x + 1, y - 1) +
	      ho_bitmap_get (m, x + 1, y) + ho_bitmap_get (m, x + 1, y + 1);
	    if ((8 - sum) < n)
	      ho_bitmap_set (m_out, x, y);
	  }
      }

  return m_out;
}

int
ho_bitmap_draw_box (ho_bitmap * m, const ho_uint x, const ho_uint y,
		    const ho_uint width, const ho_uint height)
{
  ho_uint x1, y1;

  /*  draw */
  for (x1 = x; x1 < (x + width); x1++)
    for (y1 = y; y1 < (y + height); y1++)
      ho_bitmap_set (m, x1, y1);

  return HO_FALSE;
}
