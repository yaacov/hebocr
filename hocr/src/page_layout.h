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

#include <gnome.h>

#include "box.h"

#ifndef __PAGE_LAYOUT_H__
#define __PAGE_LAYOUT_H__


/* TODO: add support for more then one squre column */

/**
 @brief adjust lines y borders

 @param pix pointer the page GdkPixbuf
 @param current_pos y position of cursor in this this column
 @param line_start return y start of next line
 @param line_end return y end of next line
 */
int
get_next_line_extention (GdkPixbuf * pix, int current_pos, int *line_start,
			 int *line_end);

/**
 @brief adjust fonts x borders

 @param pix pointer the page GdkPixbuf
 @param line_start y start of this line
 @param line_end y end of this line
 @param current_pos x position of cursor in this this line
 @param font_start return x start of next font
 @param font_end return x end of next font
 */
int
get_next_font_extention (GdkPixbuf * pix, int line_start, int line_end,
			 int current_pos, int *font_start, int *font_end);

/**
 @brief adjust fonts y borders

 @param pix pointer the page GdkPixbuf
 @param font pointer to the fonts extentions
 */
int
adjust_font_box (GdkPixbuf * pix, box * font);

/**
 @brief adjust line x borders

 @param pix pointer the page GdkPixbuf
 @param line pointer to the lines extentions
 */
int
adjust_line_box (GdkPixbuf * pix, box * line);

/**
 @brief fils an array of boxes with this page lines

 @param pix pointer the page GdkPixbuf
 @param column the column box extentions
 @param lines the array of lines to be filled
 @param num_of_lines return the number of lines in this column
 @param max_lines maximum size of lines array
 */
int
fill_lines_array (GdkPixbuf * pix, box column, box * lines,
		  int *num_of_lines, int max_lines);

		  /**
 @brief fils an array of boxs with this page fonts

 @param pix pointer the page GdkPixbuf
 @param line the line box extentions
 @param fonts the array of fonts to be filled
 @param num_of_fonts return the number of fonts in this line
 @param max_fonts maximum size of fonts array
 */
int
fill_fonts_array (GdkPixbuf * pix, box line, box * fonts,
		  int *num_of_fonts, int max_fonts);
		  
#endif
