/***************************************************************************
 *            ho_bitmap.h
 *
 *  Fri Aug 12 20:13:33 2005
 *  Copyright  2005-2007  Yaacov Zamir, Free Software Foundation
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

#include <ho_common.h>
#include <ho_bitmap.h>
#include <ho_objmap.h>
 
/**
 dilation of a a bitmap with 3x3 box
 @param m the bitmap to dilate 
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_filter_dilation (const ho_bitmap * m);

/**
 erosion of a a bitmap with 3x3 box
 @param m the bitmap to erode
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_filter_erosion (const ho_bitmap * m);

/**
 opening of a a bitmap with 3x3 box
 @param m the bitmap to open 
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_filter_opening (const ho_bitmap * m);

/**
 closing of a a bitmap with 3x3 box
 @param m the bitmap to close 
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_filter_closing (const ho_bitmap * m);

/**
 horizontaly link black dots in a bitmap
 @param m the bitmap to horizontaly link
 @patam size maximum distance
 @return newly allocated ho_bitmap
 */
ho_bitmap *hocr_bitmap_filter_hlink (ho_bitmap * m, ho_uint size);

/**
 horizontaly link short objects in a bitmap
 @param m the bitmap to horizontaly link
 @patam size maximum distance
 @patam max_height maximum hight of objects to link
 @return newly allocated ho_bitmap
 */
ho_bitmap *hocr_bitmap_filter_hlink_objecs (ho_bitmap * m, ho_uint size,
					    ho_uint max_height);

/**
 verticaly link black dots in a bitmap
 @param m the bitmap to verticaly link
 @patam size maximum distance
 @return newly allocated ho_bitmap
 */
ho_bitmap *hocr_bitmap_filter_vlink (ho_bitmap * m, ho_uint size);

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
				     ho_uint min_height, ho_uint max_height,
				     ho_uint min_width, ho_uint max_width);

/**
 copy boxed objects from bitmap
 @param m pointer to an ho_bitmap
 @return a newly allocated bitmap
 */
ho_bitmap *ho_bitmap_filter_boxes (const ho_bitmap * m);

/**
 copy filled objects from bitmap
 @param m pointer to an ho_bitmap
 @return a newly allocated bitmap
 */
ho_bitmap *ho_bitmap_filter_fill (const ho_bitmap * m);

/**
 remove halftone dots from bitmap
 @param m pointer to an ho_bitmap
 @param erosion_n the erosion operator factor
 @param dilation_n the dilation operator factor
 @return a newly allocated bitmap
 */
ho_bitmap *ho_bitmap_filter_remove_dots (const ho_bitmap * m,
					 const ho_uchar erosion_n,
					 const ho_uchar dilation_n);

/**
 guess font metrics
 @param m pointer to an ho_bitmap
 @param min_height only objects with this minimal height are used
 @param max_height only objects with this maximal height are used
 @param min_width only objects with this minimal width are used
 @param max_width only objects with this maximal width are used
 @param height pointer to return the guessed font average  height
 @param width pointer to return the guessed font average  width
 @return HO_FALSE
 */
int
ho_bitmap_filter_font_metrix (const ho_bitmap * m, const ho_uint min_height,
			      const ho_uint max_height,
			      const ho_uint min_width,
			      const ho_uint max_width, ho_usint * height,
			      ho_usint * width, ho_uchar * nikud);

/**
 guess line metrics
 @param m pointer to an ho_bitmap
 @param font_height guessed font height
 @param font_width guessed font width
 @param nikud guessed existance of nikud
 @param interline_height pointer to return the guessed interline height
 @return HO_FALSE
 */
int
ho_bitmap_filter_line_metrix (const ho_bitmap * m,
			      const ho_uint font_height,
			      const ho_uint font_width,
			      const ho_uchar nikud,
			      ho_usint * interline_height);

/**
 return a bitmap of the paragraphs
 @param m pointer to an ho_bitmap
 @param font_height guessed font height in document
 @param font_width guessed font width in document
 @return a newly allocated bitmap
 */
ho_bitmap *ho_bitmap_filter_paragraphs (const ho_bitmap * m,
					const ho_uchar font_height,
					const ho_uchar font_width,
					const ho_uchar nikud,
					ho_usint interline_height,
					const ho_uchar box);

#endif /* HO_BITMAP_FILTER_H */
