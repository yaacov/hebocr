/***************************************************************************
 *            ho_bitmap.h
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

#ifndef HO_DIMENTIONS_H
#define HO_DIMENTIONS_H 1

#include <ho_common.h>
#include <ho_bitmap.h>
#include <ho_objmap.h>

/**
 guess font dimentions
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
ho_dimentions_font (const ho_bitmap * m, const ho_uint min_height,
			      const ho_uint max_height,
			      const ho_uint min_width,
			      const ho_uint max_width, ho_usint * height,
			      ho_usint * width, ho_uchar * nikud);

/**
 guess line dimentions
 @param m pointer to an ho_bitmap
 @param font_height guessed font height
 @param font_width guessed font width
 @param nikud guessed existance of nikud
 @param interline_height pointer to return the guessed interline height
 @return HO_FALSE
 */
int
ho_dimentions_line (const ho_bitmap * m,
			      const ho_uint font_height,
			      const ho_uint font_width,
			      const ho_uchar nikud,
			      ho_usint * interline_height);

#endif /* HO_DIMENTIONS_H */
