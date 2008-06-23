
/***************************************************************************
 *            ho_bitmap_hist.c
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
#include "ho_bitmap_hist.h"

ho_bitmap_hist *
ho_bitmap_hist_new (const int width, const int height)
{
  ho_bitmap_hist *m_new = NULL;

  /* 
   * allocate memory for pixbuf 
   */
  m_new = (ho_bitmap_hist *) malloc (sizeof (ho_bitmap_hist));
  if (!m_new)
  {
    return NULL;
  }

  /* 
   * read header 
   */
  m_new->fill = 0;
  m_new->max_x = 0;
  m_new->max_y = 0;
  m_new->max_x_index = 0;
  m_new->max_y_index = 0;
  m_new->width = width;
  m_new->height = height;

  /* 
   * allocate memory for data (and set to zero)
   */
  m_new->data_x =
    calloc (m_new->width, sizeof (unsigned int));
  if (!(m_new->data_x))
  {
    free (m_new);
    return NULL;
  }
  
  m_new->data_y =
    calloc (m_new->height, sizeof (unsigned int));
  if (!(m_new->data_y))
  {
    free (m_new->data_x);
    free (m_new);
    return NULL;
  }

  return m_new;
}

int
ho_bitmap_hist_free (ho_bitmap_hist * m)
{
  if (!m)
    return TRUE;

  if (m->data_x)
    free (m->data_x);

  if (m->data_y)
    free (m->data_y);
  
  free (m);

  return FALSE;
}

int
ho_bitmap_hist_update(ho_bitmap_hist * hist, const ho_bitmap * m)
{
  int x, y;
  
  /* sanity check */
  if (!hist || !m)
    return TRUE;
  
  /* check width and hight */
  if (hist->width != m->width || hist->height != m->height)
    return TRUE;
  
  hist->fill = 0;
  hist->max_x = 0;
  hist->max_y = 0;
  hist->max_x_index = 0;
  hist->max_y_index = 0;
  
  /* clean hist */
  for (x = 0; x < hist->width; x++)
    (hist->data_x)[x] = 0;
  for (y = 0; y < hist->height; y++)
    (hist->data_y)[y] = 0;
    
  /* fill hist */
  for (x = 0; x < hist->width; x++)
    for (y = 0; y < hist->height; y++)
  {
    /* is this pixel black ? */
    if (ho_bitmap_get(m,x,y))
    {
      (hist->fill)++;
      (hist->data_x)[x]++;
      (hist->data_y)[y]++;
      
      if ((hist->data_x)[x] > hist->max_x)
      {
         hist->max_x = (hist->data_x)[x];
         hist->max_x_index = x;
      }
      
      if ((hist->data_y)[y] > hist->max_y)
      {
         hist->max_y = (hist->data_y)[y];
         hist->max_y_index = y;
      }
    }
  }
  
  return FALSE;
}
