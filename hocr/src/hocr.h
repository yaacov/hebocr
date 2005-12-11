
/***************************************************************************
 *            hocr.h
 *
 *  Fri Aug 12 20:18:34 2005
 *  Copyright  2005  Yaacov Zamir
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "consts.h"
#include "hocr_textbuffer.h"
#include "hocr_pixbuf.h"
#include "hocr_object.h"

#ifndef __HOCR_H__
#define __HOCR_H__

#ifdef __cplusplus
extern "C"
{
#endif

/**
 @brief print the font to std out

 @param pix pointer to hocr_pixbuf struct.
 @param font the font to print
 @return 1
 */
	int print_font (hocr_pixbuf * pix, hocr_box font);

/**
 @brief fill a box on hocr_pixbuf

 @param pix pointer to hocr_pixbuf struct.
 @param rect the rectangle to color
 @param chanell the color channel to color
 @param value set the channel to this value
 @param only_main_object if set to TRUE color only main object
 @return 1
 */
	int color_hocr_box_full (hocr_pixbuf * pix, hocr_box rect, int chanell,
				 int value, int only_main_object);

/**
 @brief drow box borders on hocr_pixbuf

 @param pix pointer to hocr_pixbuf struct.
 @param rect the rectangle to color
 @param chanell the color channel to color
 @param value set the channel to this value
 @return 1
 */
int
color_hocr_box (hocr_pixbuf * pix, hocr_box rect, int chanell, int value);

/**
 @brief drow a nearly horizontal line on hocr_pixbuf

 @param pix pointer to hocr_pixbuf struct.
 @param line the line to color
 @param x1 from x
 @param x2 to x
 @param chanell the color channel to color
 @param value set the channel to this value
 @return 1
 */
int
color_hocr_line_eq (hocr_pixbuf * pix, hocr_line_eq * line, int x1, int x2,
		    int chanell, int value);

/* user only use this function */

/**
 @brief do ocr on a hocr_pixbuf and return the result text to text_buffer

 @param pix pointer to hocr_pixbuf struct.
 @param text_buffer pointer to an already allocated text buffer for the results
 @param max_buffer_size site of allocated memory for text_buffer.
 @return 1
 */
	int hocr_do_ocr (hocr_pixbuf * pix, hocr_text_buffer * text_buffer);

#ifdef __cplusplus
}
#endif

#endif
