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
#include "ho_bitmap_draw.h"

ho_bitmap *
ho_bitmap_new (const int width, const int height)
{
  ho_bitmap *m_new = NULL;

  /*
   * allocate memory for pixbuf 
   */
  m_new = (ho_bitmap *) malloc (sizeof (ho_bitmap));
  if (!m_new)
    {
      return NULL;
    }

  /*
   * read header 
   */
  m_new->x = 0;
  m_new->y = 0;
  m_new->width = width;
  m_new->height = height;
  m_new->rowstride = (width - 1) / 8 + 1;

  m_new->type = 0;
  m_new->font_height = 0;
  m_new->font_width = 0;
  m_new->nikud = FALSE;

  int font_spacing;
  int line_spacing;

  /*
   * allocate memory for data (and set to zero)
   */
  m_new->data =
    calloc (m_new->height * m_new->rowstride, sizeof (unsigned char));
  if (!(m_new->data))
    {
      free (m_new);
      return NULL;
    }

  return m_new;
}

int
ho_bitmap_free (ho_bitmap * m)
{
  if (!m)
    return TRUE;

  if (m->data)
    free (m->data);

  free (m);

  return FALSE;
}

ho_bitmap *
ho_bitmap_clone (const ho_bitmap * m)
{
  ho_bitmap *m_out;

  /* allocate memory */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return NULL;
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->nikud = m->nikud;

  /* copy data */
  memcpy (m_out->data, m->data, m_out->height * m_out->rowstride);

  return m_out;
}

ho_bitmap *
ho_bitmap_clone_window (const ho_bitmap * m, const int x, const int y,
			const int width, const int height)
{
  ho_bitmap *m_out;
  int x1, y1;

  /* allocate memory */
  m_out = ho_bitmap_new (width, height);
  if (!m_out)
    return NULL;

  /* set origin of sub window */
  m_out->x = m->x + x;
  m_out->y = m->y + y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->nikud = m->nikud;

  /* copy data */
  for (x1 = 0; x1 < width; x1++)
    for (y1 = 0; y1 < height; y1++)
      {
	if ((x + x1) > 0 && (y + y1) > 0 && ho_bitmap_get (m, x + x1, y + y1))
	  ho_bitmap_set (m_out, x1, y1);
      }
  return m_out;
}

int
ho_bitmap_binop_window (ho_bitmap * m_left, const ho_bitmap * m_right,
			unsigned char op)
{
  ho_bitmap *m_temp = NULL;
  int i;

  m_temp =
    ho_bitmap_clone_window (m_right, m_right->x, m_right->y, m_right->width,
			    m_right->height);
  if (!m_temp)
    return TRUE;

  switch (op)
    {
    case 0:			/* and */
      for (i = 0; i < m_right->height * m_right->rowstride; i++)
	m_left->data[i] &= m_temp->data[i];
      break;
    case 1:			/* or */
      for (i = 0; i < m_right->height * m_right->rowstride; i++)
	m_left->data[i] |= m_temp->data[i];
      break;
    case 2:			/* xor */
      for (i = 0; i < m_right->height * m_right->rowstride; i++)
	m_left->data[i] ^= m_temp->data[i];
      break;
    case 3:			/* andnot */
      for (i = 0; i < m_right->height * m_right->rowstride; i++)
	m_left->data[i] &= ~m_temp->data[i];
      break;
    }
  ho_bitmap_free (m_temp);
  return FALSE;
}

int
ho_bitmap_and (ho_bitmap * m_left, const ho_bitmap * m_right)
{
  int i;

  /* check sizes */
  if (m_left->width != m_right->width || m_left->height != m_right->height)
    return TRUE;

  /* copy data */
  for (i = 0; i < m_right->height * m_right->rowstride; i++)
    m_left->data[i] &= m_right->data[i];

  return FALSE;
}

int
ho_bitmap_or (ho_bitmap * m_left, const ho_bitmap * m_right)
{
  int i;

  /* check sizes */
  if (m_left->width != m_right->width || m_left->height != m_right->height)
    return TRUE;

  /* copy data */
  for (i = 0; i < m_right->height * m_right->rowstride; i++)
    m_left->data[i] |= m_right->data[i];

  return FALSE;
}

int
ho_bitmap_xor (ho_bitmap * m_left, const ho_bitmap * m_right)
{
  int i;

  /* check sizes */
  if (m_left->width != m_right->width || m_left->height != m_right->height)
    return TRUE;

  /* copy data */
  for (i = 0; i < m_right->height * m_right->rowstride; i++)
    m_left->data[i] ^= m_right->data[i];

  return FALSE;
}

int
ho_bitmap_andnot (ho_bitmap * m_left, const ho_bitmap * m_right)
{
  int i;

  /* check sizes */
  if (m_left->width != m_right->width || m_left->height != m_right->height)
    return TRUE;

  /* copy data */
  for (i = 0; i < m_right->height * m_right->rowstride; i++)
    m_left->data[i] &= ~m_right->data[i];

  return FALSE;
}

ho_bitmap *
ho_bitmap_dilation_n (const ho_bitmap * m, const unsigned char n)
{
  ho_bitmap *m_out;
  int x, y;
  unsigned char sum;

  /* allocate memory */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return NULL;
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->nikud = m->nikud;

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
ho_bitmap_erosion_n (const ho_bitmap * m, const unsigned char n)
{
  ho_bitmap *m_out;
  int x, y;
  unsigned char sum;

  /* allocate memory */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return NULL;
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->nikud = m->nikud;
  
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

ho_bitmap *
ho_bitmap_set_height (const ho_bitmap * m, const int height, const int top,
		      const int bottom)
{
  ho_bitmap *m_out;
  int x, y, locale_top, locale_bottom, locale_height;
  unsigned char sum;

  /* allocate memory */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return NULL;
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->nikud = m->nikud;
  
  /*  do max_height */
  for (x = 0; x < m->width; x++)
    for (y = 0; y < m->height; y++)
      {
	/* if black pixel */
	if (ho_bitmap_get (m, x, y))
	  {
	    locale_height = height;
	    locale_top = top;
	    locale_bottom = bottom;

	    if (y - top < 0)
	      locale_top = y;
	    if (y + height + bottom >= m->height)
	      {
		locale_bottom = 0;
		locale_height = m->height - y - 1;
	      }

	    ho_bitmap_draw_vline (m_out, x, y - locale_top,
				  locale_height + locale_bottom);
	    y = m->height;
	  }
      }

  return m_out;
}

ho_bitmap *
ho_bitmap_dilation (const ho_bitmap * m)
{
  return ho_bitmap_dilation_n (m, 1);
}

ho_bitmap *
ho_bitmap_erosion (const ho_bitmap * m)
{
  return ho_bitmap_erosion_n (m, 1);
}

ho_bitmap *
ho_bitmap_opening (const ho_bitmap * m)
{
  ho_bitmap *m_temp;
  ho_bitmap *m_out;

  m_temp = ho_bitmap_erosion (m);
  m_out = ho_bitmap_dilation (m_temp);
  ho_bitmap_free (m_temp);

  return m_out;
}

ho_bitmap *
ho_bitmap_closing (const ho_bitmap * m)
{
  ho_bitmap *m_temp;
  ho_bitmap *m_out;

  m_temp = ho_bitmap_dilation (m);
  m_out = ho_bitmap_erosion (m_temp);
  ho_bitmap_free (m_temp);

  return m_out;
}

ho_bitmap *
ho_bitmap_hlink (const ho_bitmap * m, const int size)
{
  ho_bitmap *m_out;
  int x, y;
  int k;
  int l, last;

  /* allocate memory */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return NULL;
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->nikud = m->nikud;
  
  for (y = 0; y < m->height; y++)
    {
      last = -size;
      for (x = 0; x < m->width; x++)
	{
	  if (ho_bitmap_get (m, x, y))	/* black pixel */
	    {
	      ho_bitmap_set (m_out, x, y);
	      l = (int) x - last;
	      if ((l > 1) && (l < size))
		{
		  if (last < 0)
		    last = 0;
		  for (k = last; k < x; k++)
		    ho_bitmap_set (m_out, k, y);
		}
	      last = x;
	    }
	}
    }

  return m_out;
}

ho_bitmap *
ho_bitmap_herode (const ho_bitmap * m, const int size)
{
  ho_bitmap *m_out;
  int x, y;
  int k;
  int l, last;

  /* allocate memory */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return NULL;
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->nikud = m->nikud;
  
  /* set all bitmap black */
  memset ((void *) (m_out->data), 0xff, m_out->height * m_out->rowstride);

  for (y = 0; y < m->height; y++)
    {
      last = -size;
      for (x = 0; x < m->width; x++)
	{
	  if (!ho_bitmap_get (m, x, y))	/* white pixel */
	    {
	      ho_bitmap_unset (m_out, x, y);
	      l = (int) x - last;
	      if ((l > 1) && (l < size))
		{
		  if (last < 0)
		    last = 0;
		  for (k = last; k < x; k++)
		    ho_bitmap_unset (m_out, k, y);
		}
	      last = x;
	    }
	}
    }

  return m_out;
}

ho_bitmap *
ho_bitmap_vlink (const ho_bitmap * m, const int size)
{
  ho_bitmap *m_out;
  int x, y;
  int k;
  int l, last;

  /* allocate memory */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return NULL;
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->nikud = m->nikud;
  
  for (x = 0; x < m->width; x++)
    {
      last = -size;
      for (y = 0; y < m->height; y++)
	{
	  if (ho_bitmap_get (m, x, y))	/* black pixel */
	    {
	      ho_bitmap_set (m_out, x, y);
	      l = (int) y - last;
	      if ((l > 1) && (l < size))
		{
		  for (k = last; k < y; k++)
		    ho_bitmap_set (m_out, x, k);
		}
	      last = y;
	    }
	}
    }

  return m_out;
}

ho_bitmap *
ho_bitmap_edge (const ho_bitmap * m, const int n)
{
  ho_bitmap *m_out;
  ho_bitmap *m_temp1;
  ho_bitmap *m_temp2;
  int i;

  m_temp1 = ho_bitmap_clone (m);
  if (!m_temp1)
    return NULL;

  for (i = 0; i < n; i++)
    {
      m_temp2 = ho_bitmap_erosion (m_temp1);
      ho_bitmap_free (m_temp1);
      if (!m_temp2)
	return NULL;
      m_temp1 = m_temp2;
    }

  m_out = ho_bitmap_clone (m);
  if (!m_out)
    return NULL;
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->nikud = m->nikud;
  
  ho_bitmap_andnot (m_out, m_temp1);
  ho_bitmap_free (m_temp1);

  return m_out;
}
