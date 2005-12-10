/***************************************************************************
 *            page_layout.c
 *
 *  Mon Aug 15 13:33:16 2005
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

#include "hocr.h"

#ifndef __PAGE_LAYOUT_H__
#define __PAGE_LAYOUT_H__

/**
 @brief adjust lines y borders

 @param pix pointer the page hocr_pixbuf
 @param current_pos y position of cursor in this this column
 @param line_start return y start of next line
 @param line_end return y end of next line
 */
int
get_next_line_extention (hocr_pixbuf * pix, hocr_box column, int current_pos, int *line_start,
			 int *line_end);

/**
 @brief adjust fonts x borders

 @param pix pointer the page hocr_pixbuf
 @param line_start y start of this line
 @param line_end y end of this line
 @param current_pos x position of cursor in this this line
 @param font_start return x start of next font
 @param font_end return x end of next font
 */
int
get_next_font_extention (hocr_pixbuf * pix, int line_start, int line_end,
			 int current_pos, int *font_start, int *font_end,
			 int *top, int *bottom);

/**
 @brief fils an array of hocr_boxes with this page lines

 @param pix pointer the page hocr_pixbuf
 @param column the column hocr_box extentions
 @param lines the array of lines to be filled
 @param num_of_lines return the number of lines in this column
 @param max_lines maximum size of lines array
 */
int
fill_lines_array (hocr_pixbuf * pix, hocr_box column, hocr_box * lines,
		  int *num_of_lines, int max_lines);

/**
 @brief fils an array of hocr_boxes with this page columns

 @param pix pointer the page hocr_pixbuf
 @param columns the array of columns to be filled
 @param num_of_columns return the number of columns in this page
 @param max_columns maximum size of columns array
 */
int
fill_columns_array (hocr_pixbuf * pix, hocr_box * columns,
		  int *num_of_columns, int max_columns);
		  
/**
 @brief fils an array of hocr_boxs with this page fonts

 @param pix pointer the page hocr_pixbuf
 @param line the line hocr_box extentions
 @param fonts the array of fonts to be filled
 @param num_of_fonts return the number of fonts in this line
 @param max_fonts maximum size of fonts array
 */
int
fill_fonts_array (hocr_pixbuf * pix, hocr_box line, hocr_box * fonts,
		  int *num_of_fonts, int max_fonts);

#endif
