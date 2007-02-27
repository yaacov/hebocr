/***************************************************************************
 *            ho_bitmap_draw.h
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

#ifndef HO_BITMAP_DRAW_H
#define HO_BITMAP_DRAW_H 1

#include <ho_bitmap.h>

/**
 draw a black box on bitmap
 @param m the bitmap to draw on
 @param x x-start of box
 @param y y-start of box
 @param width width of box
 @param height height of box
 @return FALSE
 */
int
ho_bitmap_draw_box (ho_bitmap * m, const int x, const int y,
		    const int width, const int height);

/**
 draw a black empty box on bitmap
 @param m the bitmap to draw on
 @param x x-start of box
 @param y y-start of box
 @param width width of box
 @param height height of box
 @return FALSE
 */
int
ho_bitmap_draw_box_empty (ho_bitmap * m, const int x, const int y,
			  const int width, const int height);

/**
 draw vertical line on bitmap
 @param m the bitmap to draw on
 @param x x-start of line
 @param y y-start of line
 @param height height of line
 @return FALSE
 */
int
ho_bitmap_draw_vline (ho_bitmap * m, const int x, const int y,
		      const int height);

/**
 delete vertical line on bitmap
 @param m the bitmap to draw on
 @param x x-start of line
 @param y y-start of line
 @param height height of line
 @return FALSE
 */
int
ho_bitmap_delete_vline (ho_bitmap * m, const int x, const int y,
			const int height);

/**
 draw horizontal line on bitmap
 @param m the bitmap to draw on
 @param x x-start of line
 @param y y-start of line
 @param width height of line
 @return FALSE
 */
int
ho_bitmap_draw_hline (ho_bitmap * m, const int x, const int y,
		      const int width);

/**
 delete horizontal line on bitmap
 @param m the bitmap to draw on
 @param x x-start of line
 @param y y-start of line
 @param width height of line
 @return FALSE
 */
int
ho_bitmap_delete_hline (ho_bitmap * m, const int x, const int y,
			const int width);

#endif /* HO_BITMAP_DRAW_H */
