
/***************************************************************************
 *            ho_array_hist.h
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

/** @file ho_array_hist.h
    @brief libhocr C language header.
    
    libhocr - LIBrary for Hebrew Optical Character Recognition 
*/

#ifndef HO_ARRAY_HIST_H
#define HO_ARRAY_HIST_H 1

#include <ho_pixbuf.h>
#include <ho_bitmap.h>
#include <ho_array.h>

/** @struct ho_array_hist
  @brief libhocr array histogram struct
*/
typedef struct
{
  int size;
  double *data;
} ho_array_hist;

/**
 new ho_array_hist 
 @param size the number of buckets in the histogram
 @return newly allocated ho_array_hist
 */
ho_array_hist *ho_array_hist_new (const int size);

/**
 free an ho_array_hist
 @param hist pointer to an ho_array_hist
 @return FALSE
 */
int ho_array_hist_free (ho_array_hist * hist);

/**
 calculate ho_array_hist from ho_array
 @param hist the histogram to init
 @param ar the ho_array to use
 @return FALSE
 */
unsigned char ho_array_hist_init (ho_array_hist * hist, const ho_array * ar);

/**
 new ho_array_hist from ho_array data
 @param size the number of buckets in the histogram
 @param ar the ho_array to use
 @return newly allocated ho_array_hist
 */
ho_array_hist *ho_array_hist_new_from_array (const int size,
  const ho_array * ar);

/**
 get data from a ho_array_hist
 @param hist pointer to a ho_array_hist
 @param i the bucket index
 @return the value of the histogram
 */
double ho_array_hist_get_at (const ho_array_hist * hist, int i);


/**
 new ho_array from ho_array_hist
 @param hist pointer to an ho_array_hist
 @return newly allocated ho_array
 */
ho_array *ho_array_hist_to_array (const ho_array_hist * hist);

#endif /* HO_ARRAY_HIST_H */
