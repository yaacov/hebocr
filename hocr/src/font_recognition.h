
/***************************************************************************
 *            font_markers.h
 *
 *  Fri Aug 12 20:33:02 2005
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

/* 
 font markers
 */

#include "hocr.h"

#ifndef __FONT_MARKERS_H__
#define __FONT_MARKERS_H__

#ifdef __cplusplus
extern "C"
{
#endif

/**
 @brief recognize a font

 @param pix pointer to hocr_pixbuf struct.
 @param fonts_line an array of all fonts in this line
 @param num_of_fonts_in_line the number of fonts in this line
 @param font_index the index of current font;
 @param hocr_line_eq the two line equations (top, bottom)
 @param avg_font_hight an array of all fonts in this line
 @param avg_font_width the number of fonts in this line
 @param chars the char buffer to return;
 @return 1
 */
	int hocr_recognize_font (hocr_pixbuf * pix, hocr_box * fonts_line,
				 int num_of_fonts_in_line, int font_index,
				 hocr_line_eq line_eqs[2], int avg_font_hight,
				 int avg_font_width,
				 char *chars);

#ifdef __cplusplus
}
#endif

#endif
