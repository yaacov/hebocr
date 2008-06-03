
/***************************************************************************
 *            ho_array_hist.c
 *
 *  Fri Aug 12 20:13:33 2005
 *  Copyright  2005-2008  Yaacov Zamir
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

#ifndef TRUE
#define TRUE -1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL ((void*)0)
#endif

#include "ho_array_hist.h"

#define square(x) ((x)*(x))

/**
 new ho_array_hist 
 @param size the number of buckets in the histogram
 @return newly allocated ho_array
 */
ho_array_hist *
ho_array_hist_new (const int size)
{
  int x;

  ho_array_hist *hist = NULL;

  /* 
   * allocate memory for histogram 
   */
  hist = (ho_array_hist *) malloc (sizeof (ho_array));
  if (!hist)
  {
    return NULL;
  }

  /* 
   * set header 
   */
  hist->size = size;

  /* 
   * allocate memory for data
   */
  hist->data = malloc (hist->size * sizeof (double));
  if (!(hist->data))
  {
    free (hist);
    return NULL;
  }

  /* init value to 0 */
  for (x = 0; x < hist->size; x++)
  {
    (hist->data)[x] = 0.0;
  }

  return hist;
}

/**
 free an ho_array_hist
 @param hist pointer to an ho_array_hist
 @return FALSE
 */
int
ho_array_hist_free (ho_array_hist * hist)
{
  if (!hist)
    return TRUE;

  if (hist->data)
    free (hist->data);

  free (hist);

  return FALSE;
}

/**
 calculate ho_array_hist from ho_array
 @param hist the histogram to init
 @param ar the ho_array to use
 @return FALSE
 */
unsigned char
ho_array_hist_init (ho_array_hist * hist, const ho_array * ar)
{
  int i, x, y;

  double min, max, range;

  /* get min max values */
  ho_array_minmax (ar, &min, &max);
  range = max - min;

  /* calc accum hist */
  for (x = 0; x < ar->width; x++)
    for (y = 0; y < ar->height; y++)
    {
      i =
        (int) ((hist->size - 1) * ((ar->data)[x + y * ar->width] -
          min) / range);
      ((hist->data)[i])++;
    }

  /* normelize hist */
  for (i = 0; i < hist->size; i++)
    ((hist->data)[i]) /= (double) (ar->width * ar->height);

  return FALSE;
}

/**
 new ho_array_hist 
 @param size the number of buckets in the histogram
 @param ar the ho_array to use
 @return newly allocated ho_array
 */
ho_array_hist *
ho_array_hist_new_from_array (const int size, const ho_array * ar)
{
  ho_array_hist *hist = NULL;

  hist = ho_array_hist_new (size);

  if (!hist)
    return NULL;

  ho_array_hist_init (hist, ar);

  return hist;
}

/**
 get data from a ho_array_hist
 @param hist pointer to a ho_array_hist
 @param i the bucket index
 @return the value of the histogram
 */
double
ho_array_hist_get_at (const ho_array_hist * hist, int i)
{
  return hist->data[i];
}

/**
 new ho_array from ho_array_hist
 @param hist pointer to an ho_array_hist
 @return newly allocated ho_array
 */
ho_array *
ho_array_hist_to_array (const ho_array_hist * hist)
{
  int x, y;

  ho_array *m_out = NULL;

  /* allocate memory */
  m_out = ho_array_new (hist->size, hist->size);
  if (!m_out)
    return NULL;

  /* copy data */
  for (x = 0; x < hist->size; x++)
  {
    y = (int) ((double) hist->size * hist->data[x]);
    ho_array_set (m_out, x, y, 1.0);
  }

  return m_out;
}
