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

#include <gnome.h>

#ifndef __HOCR_H__

typedef struct
{
	int x1;
	int y1;
	int x2;
	int y2;
	int hight;
	int width;
} box;

int
get_next_line_extention (GdkPixbuf * pix, int current_pos, int *line_start,
			 int *line_end);

int
get_next_font_extention (GdkPixbuf * pix, int line_start, int line_end,
			 int current_pos, int *font_start, int *font_end);

int adjust_font_box (GdkPixbuf * pix, box * font);

int
fill_lines_array (GdkPixbuf * pix, int width, box * lines,
		  int *num_of_lines, int *avg_hight, int max_lines);

int
fill_fonts_array (GdkPixbuf * pix, box line, box * fonts,
		  int *num_of_fonts, int *avg_hight, int *avg_width,
		  int max_fonts);

/* -1 short 0 normal 1 ling */
int get_font_hight_class (int font_hight, int avg_font_hight);

/* -1 thin 0 normal 1 wide */
int get_font_width_class (int font_width, int avg_font_width);

/* -1 assend 0 normal 1 sunk */
int get_font_top_class (int font_top, int font_topline, int avg_font_hight);

/* -1 assend 0 normal 1 sunk */
int
get_font_base_class (int font_bottom, int font_baseline, int avg_font_hight);

int
find_font_baseline (box * fonts, int avg_hight, int index, int num_of_fonts);

int
find_font_topline (box * fonts, int avg_hight, int index, int num_of_fonts);

/* user only use this function */
int do_ocr (GdkPixbuf * pix, GtkTextBuffer * text_buffer);

#endif
