/***************************************************************************
 *            ho_pixbuf_draw.h
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

#ifndef HO_PIXBUF_DRAW_H
#define HO_PIXBUF_DRAW_H 1

#include <ho_bitmap.h>
#include <ho_objmap.h>
#include <ho_pixbuf.h>

/**
 draw a line from x1,y1 to x2,y2
 @param pix a ho_pixbuf 
 @param x1 x cordinate of start point 
 @param y1 y cordinate of start point 
 @param x2 x cordinate of end point 
 @param y2 y cordinate of end point 
 @param red value of red channel 
 @param green value of green channel 
 @param blue value of blue channel 
 @return FALSE
 */
int
ho_pixbuf_draw_line (ho_pixbuf * m, const int x1, const int y1,
		     const int x2, const int y2, const unsigned char red,
		     const unsigned char green, const unsigned char blue);

/**
 draw a scale line from x1,y1
 @param pix a ho_pixbuf 
 @param x1 x cordinate of start point 
 @param y1 y cordinate of start point 
 @param length length of scale line 
 @param step size of scale marks
 @param red value of red channel 
 @param green value of green channel 
 @param blue value of blue channel 
 @return FALSE
 */
int
ho_pixbuf_draw_horizontal_scale (ho_pixbuf * m, const int x1,
				 const int y1, const int length,
				 const int step, const unsigned char red,
				 const unsigned char green,
				 const unsigned char blue);

/**
  draw a scale line from x1,y1
 @param pix a ho_pixbuf 
 @param x1 x cordinate of start point 
 @param y1 y cordinate of start point 
 @param length length of scale line 
 @param step size of scale marks
 @param red value of red channel 
 @param green value of green channel 
 @param blue value of blue channel 
 @return FALSE
 */
int
ho_pixbuf_draw_vertical_scale (ho_pixbuf * m, const int x1,
			       const int y1, const int length,
			       const int step, const unsigned char red,
			       const unsigned char green,
			       const unsigned char blue);

/**
  draw a grid
 @param pix a ho_pixbuf 
 @param size size of grid boxes
 @param step size of scale marks
 @param red value of red channel 
 @param green value of green channel 
 @param blue value of blue channel 
 @return FALSE
 */
int
ho_pixbuf_draw_grid (ho_pixbuf * m, const int size, const int step,
		     const unsigned char red, const unsigned char green,
		     const unsigned char blue);

/**
  draw a bitmap
 @param m a ho_pixbuf 
 @param bit_in the bitmap to draw on the pixbuf
 @param red value of red channel 
 @param green value of green channel 
 @param blue value of blue channel 
 @param alpha value of alpha channel 
 @return FALSE
 */
int
ho_pixbuf_draw_bitmap (ho_pixbuf * m, const ho_bitmap * bit_in,
		       const unsigned char red, const unsigned char green,
		       const unsigned char blue, const unsigned char alpha);

/**
  draw a bitmap
 @param m a ho_pixbuf 
 @param bit_in the bitmap to draw on the pixbuf
 @param x1 x of upper right start of drawing
 @param y1 y of upper right start of drawing
 @param red value of red channel 
 @param green value of green channel 
 @param blue value of blue channel 
 @return FALSE
 */
int
ho_pixbuf_draw_bitmap_at (ho_pixbuf * m, const ho_bitmap * bit_in,
			  const int x1, const int y1,
			  const unsigned char red, const unsigned char green,
			  const unsigned char blue,
			  const unsigned char alpha);

#endif /* #include <ho_objmap.h> */
