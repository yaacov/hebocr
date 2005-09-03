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


/**
 */

/* font shape markers array*/

int hocr_guess_font (hocr_pixbuf * pix, hocr_box font, int base_class,
		     int base, int top,
		     int top_class,
		     int hight_class,
		     int width_class,
		     int end_of_word, char *font_string,
		     int max_chars_in_font_string);

int print_marks (hocr_pixbuf * pix, hocr_box font, int* marks);

/**
 */

#endif
