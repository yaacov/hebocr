
/***************************************************************************
 *            ho_array.h
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

/** @file ho_array.h
    @brief libhocr C language header.
    
    libhocr - LIBrary for Hebrew Optical Character Recognition 
*/

#ifndef HO_ARRAY_H
#define HO_ARRAY_H 1

#include <ho_pixbuf.h>
#include <ho_bitmap.h>

 /* hocr array set/get macros */
#define ho_array_set(m,x,y,val) (((m)->data)[(x)+(y)*(m)->width]=(val))
#define ho_array_get(m,x,y) (((m)->data)[(x)+(y)*(m)->width])

#define ho_array_get_width(m) ((m)->width)
#define ho_array_get_height(m) ((m)->height)
#define ho_array_get_data(m) ((m)->data)

/** @struct ho_array
  @brief libhocr array struct
*/
typedef struct
{
  int height;
  int width;
  double *data;
} ho_array;

/**
 new ho_array 
 @param height hight of pixbuf in pixels
 @param width width of pixbuf in pixels
 @return newly allocated ho_array
 */
ho_array *ho_array_new (const int width, const int height);

/**
 clone ho_array
 @param m pointer to a ho_array image
 @return newly allocated gray ho_array
 */
ho_array *ho_array_clone (const ho_array * m);

/**
 copy pixel data to a ho_array
 @param pix pointer to a ho_array image
 @param data the pixel data to copy
 @return false
 */
int ho_array_set_data (ho_array * pix, double data);

/**
 copy pixel data to a ho_array
 @param pix pointer to a ho_array image
 @param data the pixel data to copy
 @param x the pixel x
 @param y the pixel y
 @return false
 */
int ho_array_set_at (ho_array * pix, int x, int y, double data);

/**
 get pixel data from a ho_array
 @param pix pointer to a ho_array image
 @param x the pixel x
 @param y the pixel y
 @return false
 */
double ho_array_get_at (ho_array * pix, int x, int y);

/**
 new ho_array from ho_pixbuf
 @param pix pointer to an ho_pixbuf image
 @return newly allocated ho_array
 */
ho_array *ho_array_new_from_pixbuf (const ho_pixbuf * pix);

/**
 new ho_array from ho_bitmap
 @param pix pointer to an ho_bitmap image
 @return newly allocated ho_array
 */
ho_array *ho_array_new_from_bitmap (const ho_bitmap * pix);

/**
 new gray ho_pixbuf from ho_array
 @param pix_in pointer the original array
 @return newly allocated gray ho_pixbuf
 */
ho_pixbuf *ho_array_to_pixbuf (const ho_array * pix_in);

/**
 new rgb ho_pixbuf from ho_array
 @param pix_in pointer the original array
 @return newly allocated gray ho_pixbuf
 */
ho_pixbuf *ho_array_to_rgb_pixbuf (const ho_array * pix_in);

/**
 free an ho_array
 @param pix pointer to an ho_array
 @return FALSE
 */
int ho_array_free (ho_array * pix);

/**
 get the min and max values in a array
 @param pix ho_array
 @param min a pointer to return the min 
 @param max a pointer to return the max 
 @return FALSE
 */
unsigned char ho_array_minmax (const ho_array * pix, double *min, double *max);

/**
 get the mean value in a array
 @param pix ho_array
 @return min value
 */
double ho_array_get_mean (const ho_array * pix);

/**
 get the min value in a array
 @param pix ho_array
 @return min value
 */
double ho_array_get_min (const ho_array * pix);

/**
 get the max value in a array
 @param pix ho_array
 @return max value
 */
double ho_array_get_max (const ho_array * pix);

/**
 add two ho arrays
 @param ar1 left side ho_array
 @param ar2 right side ho_array
 @return FALSE
 */
unsigned char ho_array_add (ho_array * ar1, const ho_array * ar2);

/**
 add const to ho arrays
 @param ar left side ho_array
 @param num a number to add to array
 @return FALSE
 */
unsigned char ho_array_add_const (ho_array * ar, const double num);

/**
 subtruct two ho arrays
 @param ar1 left side ho_array
 @param ar2 right side ho_array
 @return FALSE
 */
unsigned char ho_array_sub (ho_array * ar1, const ho_array * ar2);

/**
 multiply two ho arrays
 @param ar1 left side ho_array
 @param ar2 right side ho_array
 @return FALSE
 */
unsigned char ho_array_mul (ho_array * ar1, const ho_array * ar2);

/**
 multiply two ho real and imagenary ho_arrays pairs
 @param ar1_r left side ho_array real part
 @param ar1_i left side ho_array imagenaryl part
 @param ar2_r right side ho_array real part
 @param ar2_i right side ho_array imagenary part
 @return FALSE
 */
unsigned char
ho_array_complex_mul (ho_array * ar1_r, ho_array * ar1_i,
  const ho_array * ar2_r, const ho_array * ar2_i);

/**
 multiply const to ho arrays
 @param ar left side ho_array
 @param num a number to multiply to array
 @return FALSE
 */
unsigned char ho_array_mul_const (ho_array * ar, const double num);

/**
 divide two ho arrays
 @param ar1 left side ho_array
 @param ar2 right side ho_array
 @return FALSE
 */
unsigned char ho_array_div (ho_array * ar1, const ho_array * ar2);

/**
 inverse ho array
 @param ar ho_array
 @return FALSE
 */
unsigned char ho_array_inv (ho_array * ar);

/**
 polerize ho array
 @param ar ho_array
 @return FALSE
 */
unsigned char ho_array_polerize (ho_array * ar, const double treshold);

/**
 do log (ho array + 1)
 @param ar ho_array
 @return FALSE
 */
unsigned char ho_array_log (ho_array * ar);

/**
 streach 0..1
 @param ar ho_array
 @return FALSE
 */
unsigned char ho_array_streach (ho_array * ar);

/**
 histogram equalization 0..1
 @param ar ho_array
 @return FALSE
 */
unsigned char ho_array_equl (ho_array * ar);

/**
 convolution 
 @param ar the ho_array to us for the convolution
 @param kernel a 3x3 kernel ho_array
 @return newly allocated ho array
 */
ho_array *ho_array_convolution_filter (const ho_array * ar,
  const ho_array * kernel);

/**
 median
 @param ar the ho_array to us for median filter
 @return newly allocated ho array
 */
ho_array *ho_array_median_filter (const ho_array * ar);

/**
 max filter
 @param ar the ho_array to us for max filter
 @return newly allocated ho array
 */
ho_array *ho_array_max_filter (const ho_array * ar);

/**
 min filter
 @param ar the ho_array to us for max filter
 @return newly allocated ho array
 */
ho_array *ho_array_min_filter (const ho_array * ar);

/**
 absulute value 
 @param ar_r real part ho_array
 @param ar_i imagenary part ho_array
 @return newly allocated ho array
 */
ho_array *ho_array_complex_abs (const ho_array * ar_r, const ho_array * ar_i);

/**
 atan2
 @param ar_r real part ho_array
 @param ar_i imagenary part ho_array
 @return newly allocated ho array
 */
ho_array *ho_array_complex_atan2 (const ho_array * ar_r, const ho_array * ar_i);

/**
 gradient 
 @param ar the ho_array to us for gradient detection
 @param ar_r return the r value of the gradient
 @param ar_theta return the theta value of the gradient
 @return FALSE
 */
unsigned char ho_array_gradient (const ho_array * ar, ho_array * ar_r,
  ho_array * ar_theta);

/**
 hough trasform for circles
 @param ar the ho_array to us for gradient detection
 @param min_radius the circles min radius
 @param max_radius the circles max radius
 @param threshold the min value to use in the gradient matrix percent
 @return the transformed matrix
 */
ho_array *ho_array_hough_circles (const ho_array * ar, const int min_radius,
  const int max_radius, const unsigned char threshold);

/**
 hough trasform for lines
 @param ar the ho_array to use for gradient detection
 @param threshold the min value to use in the gradient matrix percent
 @return the transformed matrix
 */
ho_array *ho_array_hough_lines (const ho_array * ar,
  const unsigned char threshold);

/**
 backwords hough trasform for lines
 @param ar the ho_array to use for backword hough
 @param width the array height
 @param height the array height
 @return the backword transformed matrix
 */
ho_array *ho_array_hough_lines_backwords (const ho_array * ar, const int width,
  const int height);

/**
 backwords hough trasform for lines by angle
 @param ar the ho_array to use for backword hough
 @param width the array height
 @param height the array height
 @param angle1 start line angle
 @param angle2 end line angle
 @return FALSE
 */
ho_array *ho_array_hough_lines_backwords_by_angle (const ho_array * ar,
  const int width, const int height, const int angle1, const int angle2);

/**
 backwords hough trasform for lines by vertical angle
 @param ar the ho_array to use for backword hough
 @param width the array height
 @param height the array height
 @param angle1 start line angle
 @param angle2 end line angle
 @return FALSE
 */
ho_array *ho_array_hough_lines_backwords_by_angle_v (const ho_array * ar,
  const int width, const int height, const int angle1, const int angle2);

/**
 fft_forword 
 @param ar the ho_array to us for fft
 @param ar_re the output real values
 @param ar_im the output imaginary values
 @return FALSE
 */
unsigned char ho_array_fft_forword (const ho_array * ar, ho_array * ar_r,
  ho_array * ar_im);

/**
 fft_backword
 @param ar_re input array of the real values
 @param ar_im input array of the imaginary values
 @param ar the output ho_array
 @return FALSE
 */
unsigned char ho_array_fft_backword (const ho_array * ar_r,
  const ho_array * ar_im, ho_array * ar);

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
  ho_array * shift_ar_r, ho_array * shift_ar_im);

/**
 fft_filter - applay a filter in w space
 @param ar input array
 @param ar_filter input array of the filter
 @return FALSE
 */
unsigned char ho_array_fft_filter (ho_array * ar, const ho_array * ar_filter);

/**
 fft_filter - applay a box filter in w space
 @param ar input array
 @param box_height height of box
 @param box_width width of box
 @return FALSE
 */
unsigned char
ho_array_fft_filter_box (ho_array * ar, const int box_width,
  const int box_height);

/**
 fft_filter - applay a circle filter in w space
 @param ar input array
 @param radius radius of circle
 @return FALSE
 */
unsigned char ho_array_fft_filter_circle (ho_array * ar, const int radius);

/**
 fft_filter - applay a gaussien filter in w space
 @param ar input array
 @param sigma of gaussien
 @return FALSE
 */
unsigned char ho_array_fft_filter_gaussien (ho_array * ar, const double sigma);

/**
 writes ho_array to pnm file
 @param ar ho_array to save as gray image
 @param filename save as file name 
 @return FALSE
 */
int ho_array_pnm_save (const ho_array * ar, const char *filename);

/**
 load ho_array from pnm file
 @param filename save as file name 
 @return new ho_array
 */
const ho_array *ho_array_pnm_load (const char *filename);

/**
 writes ho_array to tiff file
 @param ar ho_array to save as gray image
 @param filename save as file name 
 @return FALSE
 */
int ho_array_tiff_save (const ho_array * ar, const char *filename);

/**
 load ho_array from tiff file
 @param filename save as file name 
 @return new ho_array
 */
const ho_array *ho_array_tiff_load (const char *filename);

/**
 new ho_array init to gaussian
 @param height hight of pixbuf in pixels
 @param width width of pixbuf in pixels
 @param sigma the sigma to use in the gaussien
 @return newly allocated ho_array
 */
ho_array *ho_array_new_gaussien (const int width, const int height,
  const double sigma);

/**
 new ho_array init to box
 @param height hight of pixbuf in pixels
 @param width width of pixbuf in pixels
 @param box_height height of box
 @param box_width width of box
 @return newly allocated ho_array
 */
ho_array *ho_array_new_box (const int width, const int height,
  const int box_width, const int box_height);

/**
 new ho_array init to circle
 @param height hight of pixbuf in pixels
 @param width width of pixbuf in pixels
 @param radius radius of circle
 @return newly allocated ho_array
 */
ho_array *ho_array_new_circle (const int width, const int height,
  const int radius);

#endif /* HO_ARRAY_H */
