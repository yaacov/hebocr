/***************************************************************************
 *            ho_segment.h
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

#ifndef HO_SEGMENT_H
#define HO_SEGMENT_H 1

#include <ho_bitmap.h>
#include <ho_objmap.h>

/**
 return a bitmap of the paragraphs, 
 font and line dimentions _must_ be set
 @param m pointer to an ho_bitmap
 @param box squre (boxes) or free formed text blocks 
 @return a newly allocated bitmap
 */
ho_bitmap *ho_segment_paragraphs (const ho_bitmap * m,
				  const unsigned char box);

/**
 return a bitmap of the lines
 @param m pointer to an ho_bitmap
 @return a newly allocated bitmap
 */
ho_bitmap *ho_segment_lines (const ho_bitmap * m);

/**
 return a bitmap of the words
 @param m pointer to an ho_bitmap
 @return a newly allocated bitmap
 */
ho_bitmap *ho_segment_words (const ho_bitmap * m,
			     const ho_bitmap * m_line_map);

/**
 return a bitmap of the interfont spaces
 @param m pointer to an ho_bitmap
 @return a newly allocated bitmap
 */
ho_bitmap *ho_segment_fonts (const ho_bitmap * m,
			     const ho_bitmap * m_line_map,
			     const unsigned char slicing_threshold,
			     const unsigned char slicing_width);

#endif /* HO_SEGMENT_H */
