
/***************************************************************************
 *            ho_array_stat.c
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

#include "ho_array_stat.h"

#define square(x) ((x)*(x))

/**
 new ho_array_stat
 @param height hight of pixbuf in pixels
 @param width width of pixbuf in pixels
 @param threshold the threshold for making a mask
 @return newly allocated ho_array
 */
ho_array_stat *
ho_array_stat_new (const int width, const int height, double threshold)
{
  int x, y;

  ho_array_stat *pix = NULL;

  /* 
   * allocate memory for pixbuf 
   */
  pix = (ho_array_stat *) malloc (sizeof (ho_array));
  if (!pix)
  {
    return NULL;
  }

  /* 
   * read header 
   */
  pix->width = width;
  pix->height = height;
  pix->size = 0;
  pix->threshold = threshold;

  /* 
   * allocate memory for data
   */
  pix->data_sums = malloc (pix->height * pix->width * sizeof (double));
  if (!(pix->data_sums))
  {
    free (pix);
    return NULL;
  }

  pix->data_squre_sums = malloc (pix->height * pix->width * sizeof (double));
  if (!(pix->data_squre_sums))
  {
    free (pix->data_sums);
    free (pix);
    return NULL;
  }

  /* init value to 0 */
  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
    {
      (pix->data_sums)[x + y * pix->width] = 0.0;
      (pix->data_squre_sums)[x + y * pix->width] = 0.0;
    }

  return pix;
}

/**
 free an ho_array_stat
 @param pix pointer to an ho_array
 @return FALSE
 */
int
ho_array_stat_free (ho_array_stat * pix)
{
  if (!pix)
    return TRUE;

  if (pix->data_sums)
    free (pix->data_sums);

  if (pix->data_squre_sums)
    free (pix->data_squre_sums);

  free (pix);

  return FALSE;
}

/**
 init an ho_array_stat from an ho_array
 @param pix pointer to an ho_array image
 @return FALSE
 */
unsigned char
ho_array_stat_init (ho_array_stat * ar, const ho_array * pix)
{
  double value;

  double value2;

  int x, y;

  /* copy data */
  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
    {
      value = ho_array_get (pix, x, y);
      value2 = square (value);

      ho_array_stat_set_sum (ar, x, y, value);
      ho_array_stat_set_sqr_sum (ar, x, y, value2);
    }

  /* incrase size counter */
  ar->size = 1;

  return FALSE;
}

/**
 add an ho_array to an ho_array_stat
 @param pix pointer to an ho_array image
 @return FALSE
 */
unsigned char
ho_array_stat_add (ho_array_stat * ar, const ho_array * pix)
{
  double value;

  double value2;

  int x, y;

  /* copy data */
  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
    {
      value = ho_array_get (pix, x, y);
      value2 = square (value);

      ho_array_stat_add_sum (ar, x, y, value);
      ho_array_stat_add_sqr_sum (ar, x, y, value2);
    }

  /* incrase size counter */
  ar->size++;

  return FALSE;
}

/**
 new ho_array from ho_array_stat
 @param hist pointer to an ho_array_stat
 @return newly allocated ho_array
 */
ho_array *
ho_array_stat_to_array (const ho_array_stat * ar_st)
{
  int x, y;

  double value;

  ho_array *m_out = NULL;

  /* allocate memory */
  m_out = ho_array_new (ar_st->width, ar_st->height);
  if (!m_out)
    return NULL;

  /* copy data */
  for (x = 0; x < ar_st->width; x++)
    for (y = 0; y < ar_st->height; y++)
    {
      value = ho_array_stat_get_sum (ar_st, x, y) / (double) (ar_st->size);
      ho_array_set (m_out, x, y, value);
    }

  return m_out;
}

/**
 new ho_array mask from ho_array_stat and an ho_array
 @param ar_st pointer to an ho_array_stst
 @param ar pointer to an ho_array
 @return newly allocated mask ho_array
 */
ho_array *
ho_array_stat_to_array_mask (const ho_array_stat * ar_st, const ho_array * ar)
{
  int x, y;

  double val;

  double avg, avg2, sd2;

  double threshold;

  ho_array *m_out = NULL;

  /* do we know somthing about the image */
  if (ar_st->size < 1)
    return NULL;

  /* allocate memory */
  m_out = ho_array_new (ar_st->width, ar_st->height);
  if (!m_out)
    return NULL;

  /* copy data */
  for (x = 0; x < ar_st->width; x++)
    for (y = 0; y < ar_st->height; y++)
    {
      avg = ho_array_stat_get_sum (ar_st, x, y) / (double) (ar_st->size);
      avg2 = ho_array_stat_get_sqr_sum (ar_st, x, y) / (double) (ar_st->size);
      sd2 = avg2 - square (avg);
      threshold = square (ar_st->threshold) * sd2;

      val = ho_array_get (ar, x, y);

      ho_array_set (m_out, x, y, square (val - avg) - threshold);
    }

  return m_out;
}
