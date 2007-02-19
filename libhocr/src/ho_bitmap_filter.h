/***************************************************************************
 *            ho_bitmap_filter.h
 *
 *  Fri Aug 12 20:13:33 2005
 *  Copyright  2005-2007  Yaacov Zamir
 *  <kzamir@walla.co.il>
 ****************************************************************************/

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
 */

#ifndef HO_BITMAP_FILTER_H
#define HO_BITMAP_FILTER_H 1

#include <ho_bitmap.h>
#include <ho_objmap.h>

/**
 horizontaly link short objects in a bitmap
 @param m the bitmap to horizontaly link
 @patam size maximum distance
 @patam max_height maximum hight of objects to link
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_filter_hlink (ho_bitmap * m, int size, int max_height);

/**
 copy objects from bitmap to bitmap by size
 @param m pointer to an ho_bitmap
 @param min_height only objects with this minimal height are copied
 @param max_height only objects with this maximal height are copied
 @param min_width only objects with this minimal width are copied
 @param max_width only objects with this maximal width are copied
 @return a newly allocated bitmap
 */
ho_bitmap *ho_bitmap_filter_by_size (const ho_bitmap * m,
				     int min_height, int max_height,
				     int min_width, int max_width);

/**
 copy boxed objects from bitmap
 @param m pointer to an ho_bitmap
 @return a newly allocated bitmap
 */
ho_bitmap *ho_bitmap_filter_boxes (const ho_bitmap * m, const int leeway_down,
				   const int leeway_up);

/**
 copy filled objects from bitmap
 @param m pointer to an ho_bitmap
 @param leeway add this leeway downwords to the box
 @return a newly allocated bitmap
 */
ho_bitmap *ho_bitmap_filter_fill (const ho_bitmap * m);

/**
 take height top pixels from objects in bitmap
 @param m pointer to an ho_bitmap
 @param hight of new objects
 @return a newly allocated bitmap
 */
ho_bitmap *ho_bitmap_filter_obj_max_height (const ho_bitmap * m,
					    const int height);

/**
 take extend objects lateraly
 @param m pointer to an ho_bitmap
 @param ext_width width of lateral extention
 @return a newly allocated bitmap
 */
ho_bitmap *ho_bitmap_filter_obj_extend_lateraly (const ho_bitmap * m,
						 const int ext_width);

/**
 remove halftone dots from bitmap
 @param m pointer to an ho_bitmap
 @param erosion_n the erosion operator factor
 @param dilation_n the dilation operator factor
 @return a newly allocated bitmap
 */
ho_bitmap *ho_bitmap_filter_remove_dots (const ho_bitmap * m,
					 const unsigned char erosion_n,
					 const unsigned char dilation_n);

#endif /* HO_BITMAP_FILTER_H */
