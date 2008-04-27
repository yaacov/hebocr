
/***************************************************************************
 *            ho_bitmap.c
 *
 *  Fri Aug 12 20:13:33 2005
 *  Copyright  2005-2007  Yaacov Zamir
 *  <kzamir@walla.co.il>
 ****************************************************************************/

/*  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* for save tiff function */
#include <tiffio.h>
 
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
  m_new->line_spacing = 0;
  m_new->font_spacing = 0;
  m_new->avg_line_fill = 0;
  m_new->com_line_fill = 0;
  m_new->nikud = FALSE;

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

  /* 
   * allocate memory 
   */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return NULL;
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->font_spacing = m->font_spacing;
  m_out->line_spacing = m->line_spacing;
  m_out->avg_line_fill = m->avg_line_fill;
  m_out->com_line_fill = m->com_line_fill;
  m_out->nikud = m->nikud;

  /* 
   * copy data 
   */
  memcpy (m_out->data, m->data, m_out->height * m_out->rowstride);

  return m_out;
}

ho_bitmap *
ho_bitmap_clone_window (const ho_bitmap * m, const int x, const int y,
  const int width, const int height)
{
  ho_bitmap *m_out;
  int x1, y1;

  /* 
   * allocate memory 
   */
  m_out = ho_bitmap_new (width, height);
  if (!m_out)
    return NULL;

  /* 
   * set origin of sub window 
   */
  m_out->x = m->x + x;
  m_out->y = m->y + y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->font_spacing = m->font_spacing;
  m_out->line_spacing = m->line_spacing;
  m_out->avg_line_fill = m->avg_line_fill;
  m_out->com_line_fill = m->com_line_fill;
  m_out->nikud = m->nikud;

  /* 
   * copy data 
   */
  for (x1 = 0; x1 < width && x1 < m->width; x1++)
    for (y1 = 0; y1 < height && y1 < m->height; y1++)
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
  case 0:                      /* and */
    for (i = 0; i < m_right->height * m_right->rowstride; i++)
      m_left->data[i] &= m_temp->data[i];
    break;
  case 1:                      /* or */
    for (i = 0; i < m_right->height * m_right->rowstride; i++)
      m_left->data[i] |= m_temp->data[i];
    break;
  case 2:                      /* xor */
    for (i = 0; i < m_right->height * m_right->rowstride; i++)
      m_left->data[i] ^= m_temp->data[i];
    break;
  case 3:                      /* andnot */
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

  /* 
   * check sizes 
   */
  if (m_left->width != m_right->width || m_left->height != m_right->height)
    return TRUE;

  /* 
   * copy data 
   */
  for (i = 0; i < m_right->height * m_right->rowstride; i++)
    m_left->data[i] &= m_right->data[i];

  return FALSE;
}

int
ho_bitmap_or (ho_bitmap * m_left, const ho_bitmap * m_right)
{
  int i;

  /* 
   * check sizes 
   */
  if (m_left->width != m_right->width || m_left->height != m_right->height)
    return TRUE;

  /* 
   * copy data 
   */
  for (i = 0; i < m_right->height * m_right->rowstride; i++)
    m_left->data[i] |= m_right->data[i];

  return FALSE;
}

int
ho_bitmap_xor (ho_bitmap * m_left, const ho_bitmap * m_right)
{
  int i;

  /* 
   * check sizes 
   */
  if (m_left->width != m_right->width || m_left->height != m_right->height)
    return TRUE;

  /* 
   * copy data 
   */
  for (i = 0; i < m_right->height * m_right->rowstride; i++)
    m_left->data[i] ^= m_right->data[i];

  return FALSE;
}

int
ho_bitmap_andnot (ho_bitmap * m_left, const ho_bitmap * m_right)
{
  int i;

  /* 
   * check sizes 
   */
  if (m_left->width != m_right->width || m_left->height != m_right->height)
    return TRUE;

  /* 
   * copy data 
   */
  for (i = 0; i < m_right->height * m_right->rowstride; i++)
    m_left->data[i] &= ~m_right->data[i];

  return FALSE;
}

int
ho_bitmap_copy (ho_bitmap * m_left, const ho_bitmap * m_right)
{
  int i;

  /* 
   * check sizes 
   */
  if (m_left->width != m_right->width || m_left->height != m_right->height)
    return TRUE;

  /* 
   * copy data 
   */
  for (i = 0; i < m_right->height * m_right->rowstride; i++)
    m_left->data[i] = m_right->data[i];

  return FALSE;
}

ho_bitmap *
ho_bitmap_not (const ho_bitmap * m)
{
  int i;

  ho_bitmap *m_out;

  /* 
   * allocate m_out 
   */
  m_out = ho_bitmap_new (m->width, m->height);

  /* 
   * copy header data 
   */
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->font_spacing = m->font_spacing;
  m_out->line_spacing = m->line_spacing;
  m_out->avg_line_fill = m->avg_line_fill;
  m_out->com_line_fill = m->com_line_fill;
  m_out->nikud = m->nikud;

  /* 
   * check valid memory 
   */
  if (!m_out)
    return NULL;

  /* 
   * copy data 
   */
  for (i = 0; i < m->height * m->rowstride; i++)
    m_out->data[i] = ~m->data[i];

  return m_out;
}

ho_bitmap *
ho_bitmap_dilation_n (const ho_bitmap * m, const unsigned char n)
{
  ho_bitmap *m_out;
  int x, y;
  unsigned char sum;

  /* 
   * allocate memory 
   */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return NULL;
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->font_spacing = m->font_spacing;
  m_out->line_spacing = m->line_spacing;
  m_out->avg_line_fill = m->avg_line_fill;
  m_out->com_line_fill = m->com_line_fill;
  m_out->nikud = m->nikud;

  /* 
   * do dilation 
   */
  for (x = 1; x < m->width - 1; x++)
    for (y = 1; y < m->height - 1; y++)
    {
      /* 
       * if white pixel 
       */
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

  /* 
   * allocate memory 
   */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return NULL;
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->font_spacing = m->font_spacing;
  m_out->line_spacing = m->line_spacing;
  m_out->avg_line_fill = m->avg_line_fill;
  m_out->com_line_fill = m->com_line_fill;
  m_out->nikud = m->nikud;

  /* 
   * do erosion 
   */
  for (x = 1; x < m->width - 1; x++)
    for (y = 1; y < m->height - 1; y++)
    {
      /* 
       * if black pixel 
       */
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

  /* 
   * allocate memory 
   */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return NULL;

  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->font_spacing = m->font_spacing;
  m_out->line_spacing = m->line_spacing;
  m_out->avg_line_fill = m->avg_line_fill;
  m_out->com_line_fill = m->com_line_fill;
  m_out->nikud = m->nikud;

  /* 
   * do max_height 
   */
  for (x = 0; x < m->width; x++)
    for (y = 0; y < m->height; y++)
    {
      /* 
       * if black pixel 
       */
      if (ho_bitmap_get (m, x, y))
      {
        locale_height = height;
        locale_top = top;
        locale_bottom = bottom;

        if (y - top < 0)
          locale_top = y;
        if (y + height + bottom > m->height)
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
ho_bitmap_set_height_from_bottom (const ho_bitmap * m, const int height,
  const int top, const int bottom)
{
  ho_bitmap *m_out;
  int x, y, locale_top, locale_bottom, locale_height;
  unsigned char sum;

  /* 
   * allocate memory 
   */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return NULL;

  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->font_spacing = m->font_spacing;
  m_out->line_spacing = m->line_spacing;
  m_out->avg_line_fill = m->avg_line_fill;
  m_out->com_line_fill = m->com_line_fill;
  m_out->nikud = m->nikud;

  /* 
   * do max_height 
   */
  for (x = 0; x < m->width; x++)
    for (y = m->height - 1; y >= 0; y--)
    {
      /* 
       * if black pixel 
       */
      if (ho_bitmap_get (m, x, y))
      {
        y -= height;
        locale_height = height;
        locale_top = top;
        locale_bottom = bottom;

        if (y - top < 0)
          locale_top = y;
        if (y + height + bottom > m->height)
        {
          locale_bottom = 0;
          locale_height = m->height - y - 1;
        }

        ho_bitmap_draw_vline (m_out, x, y - locale_top,
          locale_height + locale_bottom);
        y = -1;
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

  /* 
   * allocate memory 
   */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return NULL;
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->font_spacing = m->font_spacing;
  m_out->line_spacing = m->line_spacing;
  m_out->avg_line_fill = m->avg_line_fill;
  m_out->com_line_fill = m->com_line_fill;
  m_out->nikud = m->nikud;

  for (y = 0; y < m->height; y++)
  {
    last = -size;
    for (x = 0; x < m->width; x++)
    {
      if (ho_bitmap_get (m, x, y))  /* black pixel */
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

  /* 
   * allocate memory 
   */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return NULL;
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->font_spacing = m->font_spacing;
  m_out->line_spacing = m->line_spacing;
  m_out->avg_line_fill = m->avg_line_fill;
  m_out->com_line_fill = m->com_line_fill;
  m_out->nikud = m->nikud;

  /* 
   * set all bitmap black 
   */
  memset ((void *) (m_out->data), 0xff, m_out->height * m_out->rowstride);

  for (y = 0; y < m->height; y++)
  {
    last = -size;
    for (x = 0; x < m->width; x++)
    {
      if (!ho_bitmap_get (m, x, y)) /* white pixel */
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

  /* 
   * allocate memory 
   */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return NULL;
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->font_spacing = m->font_spacing;
  m_out->line_spacing = m->line_spacing;
  m_out->avg_line_fill = m->avg_line_fill;
  m_out->com_line_fill = m->com_line_fill;
  m_out->nikud = m->nikud;

  for (x = 0; x < m->width; x++)
  {
    last = -size;
    for (y = 0; y < m->height; y++)
    {
      if (ho_bitmap_get (m, x, y))  /* black pixel */
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
  m_out->font_spacing = m->font_spacing;
  m_out->line_spacing = m->line_spacing;
  m_out->avg_line_fill = m->avg_line_fill;
  m_out->com_line_fill = m->com_line_fill;
  m_out->nikud = m->nikud;

  ho_bitmap_andnot (m_out, m_temp1);
  ho_bitmap_free (m_temp1);

  return m_out;
}

double
ho_bitmap_get_fill (const ho_bitmap * m, const int x, const int y,
  const int width, const int height)
{
  int current_x, current_y;
  int fill;

  if (!width || !height || x < 0 || y < 0 || (x + width) > m->width
    || (y + height) > m->height)
    return -1.0;

  fill = 0;
  for (current_y = y; current_y < y + height; current_y++)
    for (current_x = x; current_x < x + width; current_x++)
      fill += ho_bitmap_get (m, current_x, current_y);

  return (double) fill / (double) (width * height);
}

int
ho_bitmap_draw_box (ho_bitmap * m, const int x, const int y,
  const int width, const int height)
{
  int x1, y1;

  /* draw */
  for (x1 = x; x1 < (x + width) && x1 < m->width; x1++)
    for (y1 = y; y1 < (y + height) && y1 < m->height; y1++)
      ho_bitmap_set (m, x1, y1);

  return FALSE;
}

int
ho_bitmap_draw_vline (ho_bitmap * m, const int x, const int y, const int height)
{
  int ix = x, iy = y;
  int y1;

  /* sanity check */
  if (ix > m->width)
    ix = m->width - 1;
  if (iy > m->height)
    iy = m->height - 1;

  /* draw */
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

  /* draw */
  for (y1 = iy; y1 < (iy + height) && y1 < m->height - 1; y1++)
    ho_bitmap_unset (m, ix, y1);

  return FALSE;
}

int
ho_bitmap_draw_hline (ho_bitmap * m, const int x, const int y, const int width)
{
  int ix = x, iy = y;
  int x1;

  /* sanity check */
  if (ix > m->width)
    ix = m->width - 1;
  if (iy > m->height)
    iy = m->height - 1;

  /* draw */
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

  /* draw */
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

ho_bitmap *
ho_bitmap_filter_by_size (const ho_bitmap * m,
  int min_height, int max_height, int min_width, int max_width)
{
  ho_objmap *m_obj;
  ho_bitmap *m_out;

  /* create a new objmap */
  m_obj = ho_objmap_new_from_bitmap (m);

  if (!m_obj)
    return NULL;

  m_out = ho_objmap_to_bitmap_by_size (m_obj,
    min_height, max_height, min_width, max_width);

  /* free objmap */
  ho_objmap_free (m_obj);

  return m_out;
}

ho_bitmap *
ho_bitmap_filter_boxes (const ho_bitmap * m, const int leeway_down,
  const int leeway_up)
{
  ho_objmap *m_obj;
  ho_bitmap *m_out;
  int index;
  int x, y, width, height;

  /* allocate memory */
  m_obj = ho_objmap_new_from_bitmap (m);
  if (!m_obj)
    return NULL;
  m_out = ho_bitmap_new (m->width, m->height);
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->font_spacing = m->font_spacing;
  m_out->line_spacing = m->line_spacing;
  m_out->avg_line_fill = m->avg_line_fill;
  m_out->com_line_fill = m->com_line_fill;
  m_out->nikud = m->nikud;

  if (!m_out)
  {
    ho_objmap_free (m_obj);
    return NULL;
  }

  /* loop over all the objects and box them */
  for (index = 0; index < m_obj->obj_list->size; index++)
  {
    x = (((m_obj->obj_list)->objects)[index]).x;
    y = (((m_obj->obj_list)->objects)[index]).y;
    width = (((m_obj->obj_list)->objects)[index]).width;
    height = (((m_obj->obj_list)->objects)[index]).height;

    y -= leeway_up;
    height += leeway_up + leeway_down;
    if (y < 0)
      y = 0;
    if (y + height >= m->height)
      height = m->height - y - 1;

    ho_bitmap_draw_box (m_out, x, y, width, height);
  }

  return m_out;
}

ho_bitmap *
ho_bitmap_filter_fill (const ho_bitmap * m)
{
  ho_objmap *m_obj;
  ho_bitmap *m_out;
  ho_bitmap *m_temp1;
  ho_bitmap *m_temp2;
  int index;
  int width, height;

  /* allocate memory */
  m_obj = ho_objmap_new_from_bitmap (m);
  if (!m_obj)
    return NULL;

  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
  {
    ho_objmap_free (m_obj);
    return NULL;
  }
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->font_spacing = m->font_spacing;
  m_out->line_spacing = m->line_spacing;
  m_out->avg_line_fill = m->avg_line_fill;
  m_out->com_line_fill = m->com_line_fill;
  m_out->nikud = m->nikud;

  /* loop over all the objects and box them */
  for (index = 0; index < m_obj->obj_list->size; index++)
  {
    /* get a dimention factor */
    width = (((m_obj->obj_list)->objects)[index]).width;
    height = (((m_obj->obj_list)->objects)[index]).height;

    /* copy only the current object to a new bitmap */
    m_temp1 = ho_objmap_to_bitmap_by_index (m_obj, index);

    /* fill the current object */
    m_temp2 = ho_bitmap_hlink (m_temp1, width / 4);
    ho_bitmap_free (m_temp1);
    m_temp1 = ho_bitmap_vlink (m_temp2, height / 4);
    ho_bitmap_free (m_temp2);

    /* add to matrix out */
    ho_bitmap_or (m_out, m_temp1);
    ho_bitmap_free (m_temp1);
  }

  return m_out;
}

ho_bitmap *
ho_bitmap_filter_set_height (const ho_bitmap * m, const int height,
  const int top, const int bottom)
{
  ho_objmap *m_obj;
  ho_bitmap *m_out;
  ho_bitmap *m_temp1;
  ho_bitmap *m_temp2;
  int index;

  /* allocate memory */
  m_obj = ho_objmap_new_from_bitmap (m);
  if (!m_obj)
    return NULL;

  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
  {
    ho_objmap_free (m_obj);
    return NULL;
  }
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->font_spacing = m->font_spacing;
  m_out->line_spacing = m->line_spacing;
  m_out->avg_line_fill = m->avg_line_fill;
  m_out->com_line_fill = m->com_line_fill;
  m_out->nikud = m->nikud;

  /* loop over all the objects and box them */
  for (index = 0; index < m_obj->obj_list->size; index++)
  {
    /* copy only the current object to a new bitmap */
    m_temp1 = ho_objmap_to_bitmap_by_index (m_obj, index);
    if (!m_temp1)
      continue;

    /* take height pixels from this object */
    m_temp2 = ho_bitmap_set_height (m_temp1, height, top, bottom);
    ho_bitmap_free (m_temp1);
    if (!m_temp2)
      continue;

    /* add to matrix out */
    ho_bitmap_or (m_out, m_temp2);
    ho_bitmap_free (m_temp2);
  }

  return m_out;
}

ho_bitmap *
ho_bitmap_filter_set_height_from_bottom (const ho_bitmap * m,
  const int height, const int top, const int bottom)
{
  ho_objmap *m_obj;
  ho_bitmap *m_out;
  ho_bitmap *m_temp1;
  ho_bitmap *m_temp2;
  int index;

  /* allocate memory */
  m_obj = ho_objmap_new_from_bitmap (m);
  if (!m_obj)
    return NULL;

  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
  {
    ho_objmap_free (m_obj);
    return NULL;
  }
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->font_spacing = m->font_spacing;
  m_out->line_spacing = m->line_spacing;
  m_out->avg_line_fill = m->avg_line_fill;
  m_out->com_line_fill = m->com_line_fill;
  m_out->nikud = m->nikud;

  /* loop over all the objects and box them */
  for (index = 0; index < m_obj->obj_list->size; index++)
  {
    /* copy only the current object to a new bitmap */
    m_temp1 = ho_objmap_to_bitmap_by_index (m_obj, index);
    if (!m_temp1)
      continue;

    /* take height pixels from this object */
    m_temp2 = ho_bitmap_set_height_from_bottom (m_temp1, height, top, bottom);
    ho_bitmap_free (m_temp1);
    if (!m_temp2)
      continue;

    /* add to matrix out */
    ho_bitmap_or (m_out, m_temp2);
    ho_bitmap_free (m_temp2);
  }

  return m_out;
}

ho_bitmap *
ho_bitmap_filter_hlink (ho_bitmap * m, int size, int max_height)
{
  ho_bitmap *m_out;
  ho_bitmap *m_temp;

  /* this function use objects by the "_by_size" function this is why it is a
   * filter and not regular bitmap function */

  /* get only the thin objects */
  m_temp = ho_bitmap_filter_by_size (m, 5, max_height, 5, m->width / 2);

  /* hlink the thin objects */
  m_out = ho_bitmap_hlink (m_temp, size);

  /* add the rest of the large objects */
  ho_bitmap_or (m_out, m);

  ho_bitmap_free (m_temp);

  return m_out;
}

ho_bitmap *
ho_bitmap_filter_remove_dots (const ho_bitmap * m,
  const unsigned char erosion_n, const unsigned char dilation_n)
{
  int x, y;
  unsigned char sum;
  ho_bitmap *m_temp;
  ho_bitmap *m_out;
  ho_objmap *m_obj;
  int index;
  int width, height;

  /* allocate memory */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return NULL;
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->font_spacing = m->font_spacing;
  m_out->line_spacing = m->line_spacing;
  m_out->avg_line_fill = m->avg_line_fill;
  m_out->com_line_fill = m->com_line_fill;
  m_out->nikud = m->nikud;

  /* connect all the small dots */
  m_temp = ho_bitmap_dilation (m);

  /* check the size of objects */
  m_obj = ho_objmap_new_from_bitmap (m_temp);
  ho_bitmap_free (m_temp);

  for (x = 1; x < (m->width - 1); x++)
    for (y = 1; y < (m->height - 1); y++)
    {

      /* check the size of this pixel's object */
      index = ho_objmap_get (m_obj, x, y);
      if (index)
      {
        width = (((m_obj->obj_list)->objects)[index - 1]).width;
        height = (((m_obj->obj_list)->objects)[index - 1]).height;
      }
      else
      {
        width = 0;
        height = 0;
      }

      /* in a big object do erosion */
      if (width > m->width / 4 || height > m->height / 4)
      {
        if (ho_bitmap_get (m, x, y))  /* black pixel */
        {
          sum = ho_bitmap_get (m, x - 1, y - 1) +
            ho_bitmap_get (m, x - 1, y) +
            ho_bitmap_get (m, x - 1, y + 1) +
            ho_bitmap_get (m, x, y - 1) +
            ho_bitmap_get (m, x, y + 1) +
            ho_bitmap_get (m, x + 1, y - 1) +
            ho_bitmap_get (m, x + 1, y) + ho_bitmap_get (m, x + 1, y + 1);
          /* n number of white pixels or more */
          if ((8 - sum) < erosion_n)
            ho_bitmap_set (m_out, x, y);
        }
      }
      else                      /* if in a small object do dilation */
      {
        if (!ho_bitmap_get (m, x, y)) /* white pixel */
        {
          sum = ho_bitmap_get (m, x - 1, y - 1) +
            ho_bitmap_get (m, x - 1, y) +
            ho_bitmap_get (m, x - 1, y + 1) +
            ho_bitmap_get (m, x, y - 1) +
            ho_bitmap_get (m, x, y + 1) +
            ho_bitmap_get (m, x + 1, y - 1) +
            ho_bitmap_get (m, x + 1, y) + ho_bitmap_get (m, x + 1, y + 1);
          /* n number of black neighbors or more */
          if (sum >= dilation_n)
            ho_bitmap_set (m_out, x, y);
        }
        else
          ho_bitmap_set (m_out, x, y);
      }
    }

  ho_objmap_free (m_obj);

  return m_out;
}

ho_bitmap *
ho_bitmap_filter_obj_extend_lateraly (const ho_bitmap * m, const int ext_width)
{
  ho_objmap *m_obj;

  ho_bitmap *m_temp;
  ho_bitmap *m_out;

  int x, y;
  int index;
  int width, height;

  m_temp = ho_bitmap_clone (m);
  if (!m_temp)
    return NULL;

  /* loop over all objects and extend them lateraly */
  /* allocate memory */
  m_obj = ho_objmap_new_from_bitmap (m_temp);
  if (!m_obj)
  {
    ho_bitmap_free (m_temp);
    return NULL;
  }

  /* draw stopers */
  for (index = 0; index < m_obj->obj_list->size; index++)
  {
    x = (((m_obj->obj_list)->objects)[index]).x;
    y = (((m_obj->obj_list)->objects)[index]).y;
    width = (((m_obj->obj_list)->objects)[index]).width;
    height = (((m_obj->obj_list)->objects)[index]).height;

    if (x - ext_width < 0)
      x = ext_width;
    if (x + width + ext_width >= m->width)
      width = m->width - x - ext_width - 1;

    ho_bitmap_draw_vline (m_temp, x - ext_width, y, height);
    ho_bitmap_draw_vline (m_temp, x + width + ext_width, y, height);
  }

  /* extend */
  m_out = ho_bitmap_hlink (m_temp, 7 * ext_width / 4);

  ho_bitmap_free (m_temp);
  if (!m_out)
    return NULL;

  /* delete stopers */
  for (index = 0; index < m_obj->obj_list->size; index++)
  {
    x = (((m_obj->obj_list)->objects)[index]).x;
    y = (((m_obj->obj_list)->objects)[index]).y;
    width = (((m_obj->obj_list)->objects)[index]).width;
    height = (((m_obj->obj_list)->objects)[index]).height;

    if (x - ext_width < 0)
      x = ext_width;
    if (x + width + ext_width >= m->width)
      width = m->width - x - ext_width - 1;
    ho_bitmap_delete_vline (m_out, x - ext_width, y, height);
    ho_bitmap_delete_vline (m_out, x + width + ext_width, y, height);
  }

  /* set origin */
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->font_spacing = m->font_spacing;
  m_out->line_spacing = m->line_spacing;
  m_out->avg_line_fill = m->avg_line_fill;
  m_out->com_line_fill = m->com_line_fill;
  m_out->nikud = m->nikud;

  ho_objmap_free (m_obj);

  return m_out;
}

int
ho_bitmap_filter_count_objects (const ho_bitmap * m)
{
  ho_objmap *o_obj;
  int count;

  o_obj = ho_objmap_new_from_bitmap (m);
  if (!o_obj)
    return -1;

  count = ho_objmap_get_size (o_obj);
  ho_objmap_free (o_obj);

  return count;
}

int
ho_bitmap_pnm_save (const ho_bitmap * m, const char *filename)
{
  FILE *file = NULL;

  file = fopen (filename, "wb");

  if (!file)
    return TRUE;

  /* print header */
  fprintf (file, "P4 %d %d\n", m->width, m->height);

  /* this might be a huge write... */
  fwrite (m->data, 1, m->height * m->rowstride, file);
  fclose (file);

  return FALSE;
}

int
ho_bitmap_tiff_save (const ho_bitmap * m, const char *filename)
{
  TIFF *image;

  /* Open the TIFF file */
  if((image = TIFFOpen(filename, "w")) == NULL)
    return TRUE;

  /* set some values */
  TIFFSetField(image, TIFFTAG_IMAGEWIDTH, m->rowstride * 8);
  TIFFSetField(image, TIFFTAG_IMAGELENGTH, m->height);
  TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, 1);
  TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, 1);
  /* FIXME: strip limit is 65,536 pixels in a single strip ? */
  TIFFSetField(image, TIFFTAG_ROWSPERSTRIP, m->height);

  TIFFSetField(image, TIFFTAG_COMPRESSION, COMPRESSION_CCITTFAX4);
  TIFFSetField(image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISWHITE);
  TIFFSetField(image, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
  TIFFSetField(image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

  TIFFSetField(image, TIFFTAG_XRESOLUTION, 300.0);
  TIFFSetField(image, TIFFTAG_YRESOLUTION, 300.0);
  TIFFSetField(image, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
  
  /* Write the information to the file */
  TIFFWriteEncodedStrip(image, 0, m->data, m->rowstride * m->height);

  /* Close the file */
  TIFFClose(image);
  
  return FALSE;
}

ho_bitmap *
ho_bitmap_rotate (const ho_bitmap * m, const double angle)
{
  ho_bitmap *m_out;
  int x, y;
  int xtag, ytag;
  double xtag_part, ytag_part;
  double angle_rads;
  double new_point;
  unsigned char neighbors[2][2];
  double affine_matrix[2][2];

  /* 
   * allocate memory 
   */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return NULL;
  m_out->x = m->x;
  m_out->y = m->y;

  m_out->type = m->type;
  m_out->font_height = m->font_height;
  m_out->font_width = m->font_width;
  m_out->font_spacing = m->font_spacing;
  m_out->line_spacing = m->line_spacing;
  m_out->avg_line_fill = m->avg_line_fill;
  m_out->com_line_fill = m->com_line_fill;
  m_out->nikud = m->nikud;

  /* get angle in radians */
  angle_rads = angle *(2.0 * M_PI / 360);

  /* fill rotation matrix */
  affine_matrix[0][0] = cos (angle_rads);
  affine_matrix[0][1] = sin (angle_rads);
  affine_matrix[1][0] = -affine_matrix[0][1];
  affine_matrix[1][1] = affine_matrix[0][0];

  /* 
   * copy data 
   */
  for (x = 0; x < m->width; x++)
    for (y = 0; y < m->height; y++)
    {

      xtag_part = x * affine_matrix[0][0] + y * affine_matrix[0][1];
      ytag_part = x * affine_matrix[1][0] + y * affine_matrix[1][1];
      xtag = (int) xtag_part;
      ytag = (int) ytag_part;
      xtag_part = xtag_part - (double) xtag;
      ytag_part = ytag_part - (double) ytag;

      /* get neighbors */
      if (xtag < 1 || ytag < 1 || xtag >= m->width || ytag >= m->height)
      {
        new_point = 0;
      }
      else
      {
        neighbors[0][0] = ho_bitmap_get (m, xtag - 1, ytag - 1);
        neighbors[0][1] = ho_bitmap_get (m, xtag - 1, ytag - 0);
        neighbors[1][0] = ho_bitmap_get (m, xtag - 0, ytag - 1);
        neighbors[1][1] = ho_bitmap_get (m, xtag - 0, ytag - 0);

        new_point =
          (double) neighbors[0][0] * (1.0 - xtag_part) * (1.0 - ytag_part) +
          (double) neighbors[0][1] * (1.0 - xtag_part) * ytag_part +
          (double) neighbors[1][0] * xtag_part * (1.0 - ytag_part) +
          (double) neighbors[1][1] * xtag_part * ytag_part;
      }

      /* get new point */
      if (new_point > 0.5)
        ho_bitmap_set (m_out, x, y);
    }

  return m_out;
}
