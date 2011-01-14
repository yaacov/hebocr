
/***************************************************************************
 *            ho_array_stat.h
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

/** @file ho_array_stat.h
    @brief libhocr C language header.
    
    libhocr - LIBrary for Hebrew Optical Character Recognition 
*/

#ifndef HO_ARRAY_STAT_H
#define HO_ARRAY_STAT_H 1

#include <ho_pixbuf.h>
#include <ho_bitmap.h>
#include <ho_array.h>

#define ho_array_stat_set_sum(m,x,y,val) (((m)->data_sums)[(x)+(y)*(m)->width]=(val))
#define ho_array_stat_set_sqr_sum(m,x,y,val) (((m)->data_squre_sums)[(x)+(y)*(m)->width]=(val))

#define ho_array_stat_add_sum(m,x,y,val) (((m)->data_sums)[(x)+(y)*(m)->width]+=(val))
#define ho_array_stat_add_sqr_sum(m,x,y,val) (((m)->data_squre_sums)[(x)+(y)*(m)->width]+=(val))

#define ho_array_stat_get_sum(m,x,y) (((m)->data_sums)[(x)+(y)*(m)->width])
#define ho_array_stat_get_sqr_sum(m,x,y) (((m)->data_squre_sums)[(x)+(y)*(m)->width])

#define ho_array_stat_get_width(m) ((m)->width)
#define ho_array_stat_get_height(m) ((m)->height)
#define ho_array_stat_get_size(m) ((m)->size)
#define ho_array_stat_get_threshold(m) ((m)->threshold)
#define ho_array_stat_set_threshold(m,t) ((m)->threshold=(t))
#define ho_array_stat_get_data(m) ((m)->data)

/** @struct ho_array_stat
  @brief libhocr array statistics struct
*/
typedef struct
{
  /* FIXME: do we need a histogram history of a pixel ? */
  int height;
  int width;
  int size;
  double threshold;
  double *data_sums;
  double *data_squre_sums;
} ho_array_stat;

/**
 new ho_array_stat
 @param height hight of pixbuf in pixels
 @param width width of pixbuf in pixels
 @param threshold the threshold for making a mask
 @return newly allocated ho_array
 */
ho_array_stat *ho_array_stat_new (const int width, const int height,
  double threshold);

/**
 free an ho_array_stat
 @param pix pointer to an ho_array
 @return FALSE
 */
int ho_array_stat_free (ho_array_stat * pix);

/**
 init an ho_array_stat from an ho_array
 @param pix pointer to an ho_array image
 @return newly allocated gray ho_array
 */
unsigned char ho_array_stat_init (ho_array_stat * ar, const ho_array * pix);

/**
 add an ho_array to an ho_array_stat
 @param pix pointer to an ho_array image
 @return newly allocated gray ho_array
 */
unsigned char ho_array_stat_add (ho_array_stat * ar, const ho_array * pix);

/**
 new ho_array from ho_array_stat
 @param hist pointer to an ho_array_stat
 @return newly allocated ho_array
 */
ho_array *ho_array_stat_to_array (const ho_array_stat * ar_st);

/**
 new ho_array mask from ho_array_stat and an ho_array
 @param ar_st pointer to an ho_array_stst
 @param ar pointer to an ho_array
 @return newly allocated mask ho_array
 */
ho_array *ho_array_stat_to_array_mask (const ho_array_stat * ar_st,
  const ho_array * ar);

#endif /* HO_ARRAY_STAT_H */
