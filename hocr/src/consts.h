/***************************************************************************
 *            consts.c
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


#ifndef __CONSTS_H__
#define __CONSTS_H__

/* FIXME: this is a hack for normal pages 
   no more then 200 lines and 200 fonts per line
   better to use dynamic aloocation */

/* in units */
#define MAX_COLUMNS 4
#define MAX_LINES 200
#define MAX_FONTS_IN_LINE 200

/* FIXME: this values are good for some book lauouts with 
   serif fonts using 300 dots per inch lineart scan
   better to find out this values from the current scaned page */

/* in pixels */
#define NORMAL_FONT_WIDTH 20
#define NORMAL_FONT_HIGHT 30

#define MIN_COLUMN_WIDTH 100
#define MAX_LINE_HIGHT 60
#define MIN_LINE_HIGHT 20

/* FIXME: this is in pixel convert to 1/1000 ? */
#define MIN_DISTANCE_BETWEEN_LINES 15
#define MIN_DISTANCE_BETWEEN_WORDS 10

/* in 1/1000 cover units */
#define NOT_IN_A_LINE 43
#define IN_A_LINE 62

#define NOT_IN_A_FONT 30

#define SHORT_FONT 900
#define LONG_FONT 1200
#define THIN_FONT 900

#define FONT_ASSEND 200

#define MIN_FONT_SIZE 3

/* how much memory will hocr_text_buffer will allocate when it nead memory */
#define MEMORY_CHANK_FOR_TEXT_BUFFER 500

/* for font shape library */
#define MAX_FONTS_IN_FONT_LIB 50
#define MAX_NUM_OF_CHARS_IN_FONT 10

/* how many fonts to avg when geussing line equation */
#define NUM_OF_FONTS_TO_AVG 4

/* end of consts part
 */
#endif
