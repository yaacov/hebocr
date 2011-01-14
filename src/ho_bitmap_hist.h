
/***************************************************************************
 *            ho_bitmap_hist.h
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

/** @file ho_bitmap_hist.h
    @brief libhocr C language header.
    
    libhocr - LIBrary for Hebrew Optical Character Recognition 
*/

#ifndef HO_BITMAP_HIST_H
#define HO_BITMAP_HIST_H 1

/* hocr bitmap_hist set/get macros */

#define ho_bitmap_hist_get_width(m) ((m)->width)
#define ho_bitmap_hist_get_height(m) ((m)->height)

#define ho_bitmap_hist_get_fill(m) ((m)->fill)

#define ho_bitmap_hist_get_max_x(m) ((m)->max_x)
#define ho_bitmap_hist_get_max_y(m) ((m)->max_y)

#define ho_bitmap_hist_get_max_x_index(m) ((m)->max_x_index)
#define ho_bitmap_hist_get_max_y_index(m) ((m)->max_y_index)

/** @struct ho_bitmap_hist
  @brief libhocr bitmap histogram struct
*/
typedef struct
{
  
  int height;
  int width;
  
  int fill;
  
  int max_x;
  int max_y;
  
  int max_x_index;
  int max_y_index;
  
  unsigned int *data_x;
  unsigned int *data_y;
} ho_bitmap_hist;

/**
 new ho_bitamp_hist
 
 @param height hight of bitmap in pixels
 @param width width of bitmap in pixels
 @return newly allocated ho_bitmap_hist
 */
ho_bitmap_hist *ho_bitmap_hist_new (const int width, const int height);

/**
 free an ho_bitmap_hist
 
 @param m pointer to an ho_bitmap_hist
 @return FALSE
 */
int ho_bitmap_hist_free (ho_bitmap_hist * m);

/**
 update an ho_bitmap_hist from ho_bitmap data
 
 @param hist pointer to an ho_bitmap_hist
 @param m pointer to an ho_bitmap
 @return FALSE
 */
int ho_bitmap_hist_update(ho_bitmap_hist * hist, const ho_bitmap * m);

#endif /* HO_BITMAP_HIST_H */
