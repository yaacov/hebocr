
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

 /* hocr array set/get macros */
#define ho_array_set(m,x,y,val) (((m)->data)[(x)+(y)*(m)->width]=(val))
#define ho_array_get(m,x,y) (((m)->data)[(x)+(y)*(m)->width])

#define ho_array_get_width(m) ((m)->width)
#define ho_array_get_height(m) ((m)->height)
#define ho_array_get_data(m) ((m)->data)

/** @struct ho_array
  @brief libhocr pixbuf map struct (copy gtk pixbuf)
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
 new ho_array from gray ho_pixbuf
 @param pix pointer to an ho_array image
 @return newly allocated gray ho_array
 */
ho_array *ho_array_new_from_pixbuf (const ho_pixbuf * pix);

/**
 new gray ho_pixbuf from ho_array
 @param pix_in pointer the original array
 @return newly allocated gray ho_pixbuf
 */
ho_pixbuf *ho_array_to_pixbuf (const ho_array * pix_in);

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
unsigned char
ho_array_minmax (const ho_array * pix, double *min,
  double *max);

#endif /* HO_ARRAY_H */
