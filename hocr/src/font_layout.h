/***************************************************************************
 *            font_layout.h
 *
 *  Fri Aug 12 20:18:56 2005
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

#ifndef __FONT_LAYOUT_H__
#define __FONT_LAYOUT_H__

/* 
 helper functios
 */

int
find_font_baseline (hocr_box * fonts, int avg_hight, int index, int num_of_fonts);

int
find_font_topline (hocr_box * fonts, int avg_hight, int index, int num_of_fonts);

/*
 font position classes
 */

/* -1 assend 0 normal 1 sunk */
int
get_font_top_class (int font_top, int font_topline, int avg_font_hight);

/* -1 assend 0 normal 1 sunk */
int
get_font_base_class (int font_bottom, int font_baseline, int avg_font_hight);

/* -1 short 0 normal 1 ling */
int
get_font_hight_class (int font_hight, int avg_font_hight);

/* -1 thin 0 normal 1 wide */
int
get_font_width_class (int font_width, int avg_font_width);

#endif
