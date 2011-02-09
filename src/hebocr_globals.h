#ifndef HEBOCR_GLOBALS_H
#define HEBOCR_GLOBALS_H


/***************************************************************************
 * hebocr_globals.h
 *
 *  Sun Jan 16 13:34
 *  Copyright  2011  Diego Iastrubni <diegoiast@gmail.com
 ****************************************************************************/

/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRUE
#	define TRUE -1
#endif

#ifndef FALSE
#	define FALSE 0
#endif

#ifndef NULL
#	define NULL ((void*)0)
#endif

#ifndef HEBOCR_UNUSED
#	define HEBOCR_UNUSED(f)(void)(f)
#endif

#ifndef HEBOCR_SQUARE
#	define HEBOCR_SQUARE(x) ((x)*(x))
#endif

#ifndef BUILD
#	define BUILD "Development build"
#endif 

#ifndef VERSION
#	define VERSION "0.12"
#endif    

typedef struct HEBOCR_IMAGE_OPTIONS{
	unsigned char scale;
	unsigned char auto_scale;	// do try to autoscale
	double rotate;			// rotation angle
	unsigned char auto_rotate;	// do try to autorotate
	unsigned char adaptive;		// type of thresholding to use. 0-normal,1-no,2-fine.
	unsigned char threshold;	// threshold the threshold to use 0..100 (0-auto)
	unsigned char a_threshold;	// threshold to use for adaptive thresholding 0..100 (0-auto)
} HEBOCR_IMAGE_OPTIONS;

typedef struct HEBOCR_LAYOUT_OPTIONS {
	int font_spacing_code;		//  -3 tight .. 0 .. 3 spaced
	int paragraph_setup;		// free text blocks or boxed in columns
	int slicing_threshold;		// percent of line fill to cut fonts
	int slicing_width;		// what is a font wide
	int line_leeway;
	unsigned char dir_ltr;		// true=ltr false=rtl
	unsigned char html;		// 1 -> output format is html, 0 -> text
} HEBOCR_LAYOUT_OPTIONS;

typedef struct HEBOCR_FONT_OPTIONS {
	int font_code;			// code for the font to use (use 0)
	unsigned char nikud;		// recognize nikud in image
	unsigned char do_linguistics;	// unused
} HEBOCR_FONT_OPTIONS;



#endif // HEBOCR_GLOBALS_H
