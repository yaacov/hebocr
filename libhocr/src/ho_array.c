
/***************************************************************************
 *            ho_array.c
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

#ifdef USE_TIFF
#include <tiffio.h>
#endif

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
#include "ho_array.h"

/**
 new ho_array 
 @param height hight of pixbuf in pixels
 @param width width of pixbuf in pixels
 @return newly allocated ho_array
 */
ho_array *ho_array_new (const int width, const int height)
{
  ho_array *pix = NULL;

  /* 
   * allocate memory for pixbuf 
   */
  pix = (ho_array *) malloc (sizeof (ho_array));
  if (!pix)
  {
    return NULL;
  }

  /* 
   * read header 
   */
  pix->width = width;
  pix->height = height;

  /* 
   * allocate memory for data
   */
  pix->data = malloc (pix->height * pix->width * sizeof (double));
  if (!(pix->data))
  {
    free (pix);
    return NULL;
  }

  return pix;
}

/**
 clone ho_array
 @param m pointer to a ho_array image
 @return newly allocated gray ho_array
 */
ho_array *ho_array_clone (const ho_array * m)
{
  ho_array *m_out;

  /* allocate memory */
  m_out = ho_array_new (m->width, m->height);
  if (!m_out)
    return NULL;

  /* copy data */
  memcpy (m_out->data, m->data, m_out->height * m_out->width * sizeof(double));

  return m_out;
}

/**
 copy pixel data to a ho_array
 @param pix pointer to a ho_array image
 @param data the pixel data to copy
 @return false
 */
int ho_array_set_data (ho_array * pix, double data)
{
  int x, y;

  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
      ho_array_set (pix, x, y, data);

  return FALSE;
}

/**
 new ho_array from gray ho_pixbuf
 @param pix pointer to an ho_array image
 @return newly allocated gray ho_array
 */
ho_array *ho_array_new_from_pixbuf (const ho_pixbuf * pix)
{
  int x, y;
  double pixel_val;
  ho_array *m_out;

  /* allocate memory */
  m_out = ho_array_new (pix->width, pix->height);
  if (!m_out)
    return NULL;

  /* copy data */
  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
    {
      pixel_val = (double)ho_pixbuf_get (pix, x, y, 0) / 255.0;
      ho_array_set (m_out, x, y, pixel_val);
    }
    
  return m_out;
}

/**
 new gray ho_pixbuf from ho_array
 @param pix_in pointer the original array
 @return newly allocated gray ho_pixbuf
 */
ho_pixbuf *ho_array_to_pixbuf (const ho_array * pix_in)
{
  int x, y;
  unsigned char pixel_val;
  ho_pixbuf *pix = NULL;

  pix = ho_pixbuf_new (1, pix_in->width, pix_in->height, 0);
  if (!pix)
    return NULL;

  for (x = 0; x < pix_in->width; x++)
    for (y = 0; y < pix_in->height; y++)
    {
      pixel_val = (unsigned char)(255.0 * ho_array_get (pix_in, x, y));
      ho_pixbuf_set (pix, x, y, 0, pixel_val);
    }
    
  return pix;
}

/**
 free an ho_array
 @param pix pointer to an ho_array
 @return FALSE
 */
int ho_array_free (ho_array * pix)
{
  if (!pix)
    return TRUE;

  if (pix->data)
    free (pix->data);

  free (pix);

  return FALSE;
}

/**
 get the min and max values in a array
 @param pix ho_array
 @param min a pointer to return the min 
 @param max a pointer to return the max 
 @return FALSE
 */
unsigned char
ho_array_minmax (const ho_array * pix, double *min,
  double *max)
{
  int x, y;

  *min = (pix->data)[0];
  *max = (pix->data)[0];

  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
    {
      if (*min > (pix->data)[x + y * pix->width])
        *min = (pix->data)[x + y * pix->width];
      if (*max < (pix->data)[x + y * pix->width])
        *max = (pix->data)[x + y * pix->width];
    }

  return FALSE;
}
