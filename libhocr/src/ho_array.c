
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

#ifdef USE_FFTW
#include <fftw3.h>
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
ho_array *
ho_array_new (const int width, const int height)
{
  int x, y;
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

  /* init value to 0 */
  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
    {
      (pix->data)[x + y * pix->width] = 0.0;
    }

  return pix;
}

/**
 clone ho_array
 @param m pointer to a ho_array image
 @return newly allocated gray ho_array
 */
ho_array *
ho_array_clone (const ho_array * m)
{
  ho_array *m_out = NULL;

  /* allocate memory */
  m_out = ho_array_new (m->width, m->height);
  if (!m_out)
    return NULL;

  /* copy data */
  memcpy (m_out->data, m->data, m_out->height * m_out->width * sizeof (double));

  return m_out;
}

/**
 copy pixel data to a ho_array
 @param pix pointer to a ho_array image
 @param data the pixel data to copy
 @return false
 */
int
ho_array_set_data (ho_array * pix, double data)
{
  int x, y;

  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
      ho_array_set (pix, x, y, data);

  return FALSE;
}

/**
 copy pixel data to a ho_array
 @param pix pointer to a ho_array image
 @param data the pixel data to copy
 @param x the pixel x
 @param y the pixel y
 @return false
 */
int
ho_array_set_at (ho_array * pix, double data, int x, int y)
{
  ho_array_set (pix, x, y, data);

  return FALSE;
}

/**
 get pixel data from a ho_array
 @param pix pointer to a ho_array image
 @param x the pixel x
 @param y the pixel y
 @return false
 */
double
ho_array_get_at (ho_array * pix, int x, int y)
{
  return ho_array_get (pix, x, y);
}

/**
 new ho_array from gray ho_pixbuf
 @param pix pointer to an ho_array image
 @return newly allocated gray ho_array
 */
ho_array *
ho_array_new_from_pixbuf (const ho_pixbuf * pix)
{
  int x, y;
  double pixel_val;
  ho_array *m_out = NULL;

  /* allocate memory */
  m_out = ho_array_new (pix->width, pix->height);
  if (!m_out)
    return NULL;

  /* copy data */
  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
    {
      pixel_val = (double) ho_pixbuf_get (pix, x, y, 0) / 255.0;
      ho_array_set (m_out, x, y, pixel_val);
    }

  return m_out;
}

/**
 new gray ho_pixbuf from ho_array
 @param pix_in pointer the original array
 @return newly allocated gray ho_pixbuf
 */
ho_pixbuf *
ho_array_to_pixbuf (const ho_array * pix_in)
{
  int x, y;
  unsigned char pixel_val;
  ho_pixbuf *pix = NULL;
  double min, max, range;

  ho_array_minmax (pix_in, &min, &max);
  range = max - min;

  pix = ho_pixbuf_new (1, pix_in->width, pix_in->height, 0);
  if (!pix)
    return NULL;

  for (x = 0; x < pix_in->width; x++)
    for (y = 0; y < pix_in->height; y++)
    {
      pixel_val = (unsigned char) (255.0 *
        (ho_array_get (pix_in, x, y) - min) / range);
      ho_pixbuf_set (pix, x, y, 0, pixel_val);
    }

  return pix;
}

/**
 free an ho_array
 @param pix pointer to an ho_array
 @return FALSE
 */
int
ho_array_free (ho_array * pix)
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
ho_array_minmax (const ho_array * pix, double *min, double *max)
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

/**
 get the min value in a array
 @param pix ho_array
 @return min value
 */
double
ho_array_get_min (const ho_array * ar)
{
  double min, max;

  /* get min max values */
  ho_array_minmax (ar, &min, &max);

  return min;
}

/**
 get the max value in a array
 @param pix ho_array
 @return max value
 */
double
ho_array_get_max (const ho_array * ar)
{
  double min, max;

  /* get min max values */
  ho_array_minmax (ar, &min, &max);

  return max;
}

/**
 add two ho arrays
 @param ar1 left side ho_array
 @param ar2 right side ho_array
 @return FALSE
 */
unsigned char
ho_array_add (ho_array * ar1, const ho_array * ar2)
{
  int x, y;

  for (x = 0; x < ar1->width; x++)
    for (y = 0; y < ar1->height; y++)
    {
      (ar1->data)[x + y * ar1->width] += (ar2->data)[x + y * ar2->width];
    }

  return FALSE;
}

/**
 subtruct two ho arrays
 @param ar1 left side ho_array
 @param ar2 right side ho_array
 @return FALSE
 */
unsigned char
ho_array_sub (ho_array * ar1, const ho_array * ar2)
{
  int x, y;

  for (x = 0; x < ar1->width; x++)
    for (y = 0; y < ar1->height; y++)
    {
      (ar1->data)[x + y * ar1->width] -= (ar2->data)[x + y * ar2->width];
    }

  return FALSE;
}

/**
 multiply two ho arrays
 @param ar1 left side ho_array
 @param ar2 right side ho_array
 @return FALSE
 */
unsigned char
ho_array_mul (ho_array * ar1, const ho_array * ar2)
{
  int x, y;

  for (x = 0; x < ar1->width; x++)
    for (y = 0; y < ar1->height; y++)
    {
      (ar1->data)[x + y * ar1->width] *= (ar2->data)[x + y * ar2->width];
    }

  return FALSE;
}

/**
 divide two ho arrays
 @param ar1 left side ho_array
 @param ar2 right side ho_array
 @return FALSE
 */
unsigned char
ho_array_div (ho_array * ar1, const ho_array * ar2)
{
  int x, y;

  for (x = 0; x < ar1->width; x++)
    for (y = 0; y < ar1->height; y++)
    {
      (ar1->data)[x + y * ar1->width] /= (ar2->data)[x + y * ar2->width];
    }

  return FALSE;
}

/**
 inverse ho array
 @param ar ho_array
 @return FALSE
 */
unsigned char
ho_array_inv (ho_array * ar)
{
  int x, y;

  for (x = 0; x < ar->width; x++)
    for (y = 0; y < ar->height; y++)
    {
      (ar->data)[x + y * ar->width] = 1.0 / (ar->data)[x + y * ar->width];
    }

  return FALSE;
}

/**
 do log (ho array + 1)
 @param ar ho_array
 @return FALSE
 */
unsigned char
ho_array_log (ho_array * ar)
{
  int x, y;

  for (x = 0; x < ar->width; x++)
    for (y = 0; y < ar->height; y++)
    {
      (ar->data)[x + y * ar->width] = log (1.0 + (ar->data)[x + y * ar->width]);
    }

  return FALSE;
}

/**
 streach 0..1
 @param ar ho_array
 @return FALSE
 */
unsigned char
ho_array_streach (ho_array * ar)
{
  int x, y;
  double min, max, range;

  ho_array_minmax (ar, &min, &max);
  range = max - min;

  for (x = 0; x < ar->width; x++)
    for (y = 0; y < ar->height; y++)
    {
      (ar->data)[x + y * ar->width] = ((ar->data)[x + y * ar->width] - min) /
        range;
    }

  return FALSE;
}

/**
 histogram equalization 0..1
 @param ar ho_array
 @return FALSE
 */
unsigned char
ho_array_equl (ho_array * ar)
{
  int i, x, y;
  int l, k;
  double min, max, range;
  double acc_hist[256];
  double look_up_table[256];

  /* clean accumulated histogram and look up table */
  for (i = 0; i < 256; i++)
    acc_hist[i] = 0;
  for (i = 0; i < 256; i++)
    look_up_table[i] = 0;

  /* get min max values */
  ho_array_minmax (ar, &min, &max);
  range = max - min;

  /* calc accum hist */
  for (x = 0; x < ar->width; x++)
    for (y = 0; y < ar->height; y++)
    {
      i = (int) (255.0 * ((ar->data)[x + y * ar->width] - min) / range);
      acc_hist[i]++;
    }
  for (i = 1; i < 256; i++)
    acc_hist[i] += acc_hist[i - 1];

  /* normelize accum hist */
  for (i = 0; i < 256; i++)
    acc_hist[i] /= (double) (ar->width * ar->height);

  /* create a look up table */
  l = 0;
  for (k = 0; k < 256; k++)
  {
    while ((255.0 * acc_hist[l]) < k && l < 256)
      l++;
    /* set convertion value for */
    look_up_table[k] = (double) l / 255.0;
  }

  /* convert the array */
  for (x = 0; x < ar->width; x++)
    for (y = 0; y < ar->height; y++)
    {
      i = (int) (255.0 * ((ar->data)[x + y * ar->width] - min) / range);
      (ar->data)[x + y * ar->width] = look_up_table[i];
    }

  return FALSE;
}

/**
 convolution 
 @param ar the ho_array to us for the convolution
 @param kerne a 3x3 kernel ho_array
 @return newly allocated ho array
 */
ho_array *
ho_array_conv (const ho_array * ar, const ho_array * kernel)
{
  int x, y;
  double sum;
  double neigbours[9];
  ho_array *ar_out = NULL;

  /* allocate memory */
  ar_out = ho_array_new (ar->width, ar->height);
  if (!ar_out)
    return NULL;

  /* copy data */
  for (x = 0; x < ar->width; x++)
  {
    (ar_out->data)[x] = (ar->data)[x];
    (ar_out->data)[x + (ar_out->height - 1) * ar_out->width] =
      (ar->data)[x + (ar->height - 1) * ar->width];
  }
  for (y = 0; y < ar->height; y++)
  {
    (ar_out->data)[y * ar_out->width] = (ar->data)[y * ar->width];
    (ar_out->data)[ar_out->width - 1 + y * ar_out->width] =
      (ar->data)[ar->width - 1 + y * ar->width];
  }

  /* copy data */
  for (x = 1; x < ar->width - 1; x++)
    for (y = 1; y < ar->height - 1; y++)
    {
      neigbours[0] = (ar->data)[(x - 1) + (y - 1) * ar->width];
      neigbours[1] = (ar->data)[(x) + (y - 1) * ar->width];
      neigbours[2] = (ar->data)[(x + 1) + (y - 1) * ar->width];

      neigbours[3] = (ar->data)[(x - 1) + (y) * ar->width];
      neigbours[4] = (ar->data)[(x) + (y) * ar->width];
      neigbours[5] = (ar->data)[(x + 1) + (y) * ar->width];

      neigbours[6] = (ar->data)[(x - 1) + (y + 1) * ar->width];
      neigbours[7] = (ar->data)[(x) + (y + 1) * ar->width];
      neigbours[8] = (ar->data)[(x + 1) + (y + 1) * ar->width];

      sum = neigbours[0] * (kernel->data)[0] +
        neigbours[1] * (kernel->data)[1] +
        neigbours[2] * (kernel->data)[2] +
        neigbours[3] * (kernel->data)[3] +
        neigbours[4] * (kernel->data)[4] +
        neigbours[5] * (kernel->data)[5] +
        neigbours[6] * (kernel->data)[6] +
        neigbours[7] * (kernel->data)[7] + neigbours[8] * (kernel->data)[8];

      (ar_out->data)[x + y * ar_out->width] = sum;
    }

  return ar_out;
}

/**
 median
 @param ar the ho_array to us for median filter
 @param kerne a 3x3 kernel ho_array
 @return newly allocated ho array
 */
ho_array *
ho_array_median (const ho_array * ar)
{
  int i, j, x, y;
  int i_min;
  double median, max;
  double neigbours[9];
  ho_array *ar_out = NULL;

  /* allocate memory */
  ar_out = ho_array_new (ar->width, ar->height);
  if (!ar_out)
    return NULL;

  /* copy data */
  for (x = 0; x < ar->width; x++)
  {
    (ar_out->data)[x] = (ar->data)[x];
    (ar_out->data)[x + (ar_out->height - 1) * ar_out->width] =
      (ar->data)[x + (ar->height - 1) * ar->width];
  }
  for (y = 0; y < ar->height; y++)
  {
    (ar_out->data)[y * ar_out->width] = (ar->data)[y * ar->width];
    (ar_out->data)[ar_out->width - 1 + y * ar_out->width] =
      (ar->data)[ar->width - 1 + y * ar->width];
  }

  /* copy data */
  for (x = 1; x < ar->width - 1; x++)
    for (y = 1; y < ar->height - 1; y++)
    {
      neigbours[0] = (ar->data)[(x - 1) + (y - 1) * ar->width];
      neigbours[1] = (ar->data)[(x) + (y - 1) * ar->width];
      neigbours[2] = (ar->data)[(x + 1) + (y - 1) * ar->width];

      neigbours[3] = (ar->data)[(x - 1) + (y) * ar->width];
      neigbours[4] = (ar->data)[(x) + (y) * ar->width];
      neigbours[5] = (ar->data)[(x + 1) + (y) * ar->width];

      neigbours[6] = (ar->data)[(x - 1) + (y + 1) * ar->width];
      neigbours[7] = (ar->data)[(x) + (y + 1) * ar->width];
      neigbours[8] = (ar->data)[(x + 1) + (y + 1) * ar->width];

      /* get max value */
      max = neigbours[0];
      for (i = 0; i < 9; i++)
        if (max < neigbours[i])
          max = neigbours[i];

      /* get median */
      for (j = 0; j < 4; j++)
      {
        i_min = 0;
        for (i = 0; i < 9; i++)
          if (neigbours[i_min] > neigbours[i])
            i_min = i;
        neigbours[i_min] = max;
      }

      median = max;
      for (i = 0; i < 9; i++)
        if (median > neigbours[i])
          median = neigbours[i];

      (ar_out->data)[x + y * ar_out->width] = median;
    }

  return ar_out;
}

/**
 absulute value 
 @param ar1 left side ho_array
 @param ar2 right side ho_array
 @return newly allocated ho array
 */
ho_array *
ho_array_abs (const ho_array * ar1, const ho_array * ar2)
{
  int x, y;
  ho_array *ar_out = NULL;

  /* allocate memory */
  ar_out = ho_array_new (ar1->width, ar1->height);
  if (!ar_out)
    return NULL;

  /* copy data */
  for (x = 0; x < ar1->width; x++)
    for (y = 0; y < ar1->height; y++)
    {
      (ar_out->data)[x + y * ar_out->width] =
        sqrt ((ar1->data)[x + y * ar1->width] * (ar1->data)[x +
          y * ar1->width] + (ar2->data)[x + y * ar2->width] * (ar2->data)[x +
          y * ar2->width]);
    }

  return ar_out;
}

/**
 atan2
 @param ar1 left side ho_array
 @param ar2 right side ho_array
 @return newly allocated ho array
 */
ho_array *
ho_array_atan2 (const ho_array * ar1, const ho_array * ar2)
{
  int x, y;
  ho_array *ar_out = NULL;

  /* allocate memory */
  ar_out = ho_array_new (ar1->width, ar1->height);
  if (!ar_out)
    return NULL;

  /* copy data */
  for (x = 0; x < ar1->width; x++)
    for (y = 0; y < ar1->height; y++)
    {
      (ar_out->data)[x + y * ar_out->width] =
        atan2 ((ar1->data)[x + y * ar1->width],
        (ar2->data)[x + y * ar2->width]);
    }

  return ar_out;
}

/**
 gradient 
 @param ar the ho_array to us for gradient detection
 @param ar_r the r value of the gradient
 @param ar_theta the theta value of the gradient
 @return FALSE
 */
unsigned char
ho_array_gradient (const ho_array * ar, ho_array * ar_r, ho_array * ar_theta)
{
  int x, y;
  double sum_x, sum_y;
  double neigbours[9];
  double sobol_x[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
  double sobol_y[] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };

  /* copy data */
  for (x = 1; x < ar->width - 1; x++)
    for (y = 1; y < ar->height - 1; y++)
    {
      neigbours[0] = (ar->data)[(x - 1) + (y - 1) * ar->width];
      neigbours[1] = (ar->data)[(x) + (y - 1) * ar->width];
      neigbours[2] = (ar->data)[(x + 1) + (y - 1) * ar->width];

      neigbours[3] = (ar->data)[(x - 1) + (y) * ar->width];
      neigbours[4] = (ar->data)[(x) + (y) * ar->width];
      neigbours[5] = (ar->data)[(x + 1) + (y) * ar->width];

      neigbours[6] = (ar->data)[(x - 1) + (y + 1) * ar->width];
      neigbours[7] = (ar->data)[(x) + (y + 1) * ar->width];
      neigbours[8] = (ar->data)[(x + 1) + (y + 1) * ar->width];

      sum_x = neigbours[0] * sobol_x[0] +
        neigbours[1] * sobol_x[1] +
        neigbours[2] * sobol_x[2] +
        neigbours[3] * sobol_x[3] +
        neigbours[4] * sobol_x[4] +
        neigbours[5] * sobol_x[5] +
        neigbours[6] * sobol_x[6] +
        neigbours[7] * sobol_x[7] + neigbours[8] * sobol_x[8];

      sum_x = neigbours[0] * sobol_y[0] +
        neigbours[1] * sobol_y[1] +
        neigbours[2] * sobol_y[2] +
        neigbours[3] * sobol_y[3] +
        neigbours[4] * sobol_y[4] +
        neigbours[5] * sobol_y[5] +
        neigbours[6] * sobol_y[6] +
        neigbours[7] * sobol_y[7] + neigbours[8] * sobol_y[8];

      /* calculate r and theta vlaues */
      (ar_r->data)[x + y * ar_r->width] =
        sqrt ((sum_x * sum_x) + (sum_y * sum_y));
      (ar_theta->data)[x + y * ar_theta->width] = -atan2 (sum_x, sum_y);
    }

  return FALSE;
}

#ifdef USE_FFTW

/**
 fft_forword 
 @param ar the ho_array to us for fft
 @param ar_re the output real values
 @param ar_im the output imaginary values
 @return FALSE
 */
unsigned char
ho_array_fft_forword (const ho_array * ar, ho_array * ar_r, ho_array * ar_im)
{
  unsigned int i;
  int width, height;
  fftw_plan plan;
  fftw_complex *fft_ar_xy;
  fftw_complex *fft_ar_w;

  width = ar->width;
  height = ar->height;

  fft_ar_xy = fftw_malloc (sizeof (fftw_complex) * width * height);
  fft_ar_w = fftw_malloc (sizeof (fftw_complex) * width * height);

  /* copy data to fftw array */
  for (i = 0; i < width * height; i++)
  {
    fft_ar_xy[i][0] = (ar->data)[i];
    fft_ar_xy[i][1] = 0.0f;
  }
  /* calculate the implementation plan */
  plan = fftw_plan_dft_2d
    (height, width, fft_ar_xy, fft_ar_w, FFTW_FORWARD, FFTW_ESTIMATE);

  /* perform on the Fourier transform */
  fftw_execute (plan);

  /* copy results to output arrays */
  for (i = 0; i < width * height; i++)
  {
    (ar_r->data)[i] = fft_ar_w[i][0];
    (ar_im->data)[i] = fft_ar_w[i][1];
  }

  /* free fft objects */
  fftw_destroy_plan (plan);
  fftw_free (fft_ar_xy);
  fftw_free (fft_ar_w);

  return FALSE;
}

/**
 fft_backword
 @param ar_re input array of the real values
 @param ar_im input array of the imaginary values
 @param ar the output ho_array
 @return FALSE
 */
unsigned char
ho_array_fft_backword (const ho_array * ar_r, const ho_array * ar_im,
  ho_array * ar)
{
  unsigned int i;
  int width, height;
  fftw_plan plan;
  fftw_complex *fft_ar_xy;
  fftw_complex *fft_ar_w;

  width = ar->width;
  height = ar->height;

  fft_ar_xy = fftw_malloc (sizeof (fftw_complex) * width * height);
  fft_ar_w = fftw_malloc (sizeof (fftw_complex) * width * height);

  /* copy data to fftw array */
  for (i = 0; i < width * height; i++)
  {
    fft_ar_w[i][0] = (ar_r->data)[i];
    fft_ar_w[i][1] = (ar_im->data)[i];
  }
  /* calculate the implementation plan */
  plan = fftw_plan_dft_2d
    (height, width, fft_ar_w, fft_ar_xy, FFTW_BACKWARD, FFTW_ESTIMATE);

  /* perform on the Fourier transform */
  fftw_execute (plan);

  /* copy results to output arrays */
  for (i = 0; i < width * height; i++)
  {
    (ar->data)[i] = fft_ar_xy[i][0];
  }

  /* free fft objects */
  fftw_destroy_plan (plan);
  fftw_free (fft_ar_xy);
  fftw_free (fft_ar_w);

  return FALSE;
}

/**
 fft_shift
 @param ar_re input array of the real values
 @param ar_im input array of the imaginary values
 @param shift_ar_re output array of the real values
 @param shift_ar_im output array of the imaginary values
 @return FALSE
 */
unsigned char
ho_array_fft_shift (const ho_array * ar_r, const ho_array * ar_im,
  ho_array * shift_ar_r, ho_array * shift_ar_im)
{
  int x, y, xtag, ytag;

  /* copy data */
  for (x = 0; x < ar_r->width; x++)
    for (y = 0; y < ar_r->height; y++)
    {
      xtag = (x + ar_r->width / 2) % ar_r->width;
      ytag = (y + ar_r->height / 2) % ar_r->height;

      (shift_ar_r->data)[x + y * ar_r->width] =
        (ar_r->data)[xtag + ytag * ar_r->width];
      (shift_ar_im->data)[x + y * ar_r->width] =
        (ar_im->data)[xtag + ytag * ar_r->width];
    }

  return FALSE;
}

int
ho_array_pnm_save (const ho_array * ar, const char *filename)
{
  ho_pixbuf *pix;

  pix = ho_array_to_pixbuf (ar);
  if (!pix)
    return TRUE;

  ho_pixbuf_pnm_save (pix, filename);

  ho_pixbuf_free (pix);

  return FALSE;
}

#endif /* USE_FFTW */
