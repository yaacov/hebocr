

/***************************************************************************
 *            hocr.h
 *
 *  Fri Aug 12 20:13:33 2005
 *  Copyright  2005-2008  Yaacov Zamir
 *  <kzamir@walla.co.il>
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

/** @mainpage libhocr
 *
 * @ref README @ref HACKING @ref ChangeLog
 *
 * LIBrary for Hebrew Optical Character Recognition 
 *
 * http://hocr.berlios.de
 *
 * @section README
 * @include README
 * 
 * @section HACKING
 * @include HACKING
 * 
 * @section ChangeLog
 * @include ChangeLog
 *
 */

/** @file hocr.h
    @brief libhocr main C language header.
    
    libhocr - LIBrary for Hebrew Optical Character Recognition 
*/

#ifndef HOCR_H
#define HOCR_H 1

#ifdef __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

#ifndef BUILD
#	define BUILD "Development build"
#	define VERSION "0.12"
#endif

/* color image map */
#include <ho_pixbuf.h>

/* gray array (double) */
#include <ho_array.h>
#include <ho_array_hist.h>
#include <ho_array_stat.h>

/* binary image map */
#include <ho_bitmap.h>
#include <ho_bitmap_hist.h>

/* object image map */
#include <ho_objmap.h>

/* layout container for hocr segmentations */
#include <ho_layout.h>
#include <ho_dimentions.h>
#include <ho_segment.h>

/* font recognition */
#include <ho_font.h>
#include <ho_recognize.h>
#include <ho_recognize_nikud.h>

/* linguistics */
#include <ho_linguistics.h>

/* specific font recognition */

/* string utilities */
#include <ho_string.h>

/* wrapper function */


typedef struct HEBOCR_IMAGE_OPTIONS{
	unsigned char scale;
	unsigned char no_auto_scale;	// do not try to autoscale
	double rotate;			// rotation angle
	unsigned char no_auto_rotate;	// do not try to autorotate
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


/**
 convert a gray pixbuf to bitmap 

 @param pix_in the input ho_pixbuf
 @param options image process options
 @param progress a progress indicator 0..100
 @return newly allocated gray ho_bitmap
 */
ho_bitmap *hocr_image_processing( const ho_pixbuf* pix_in, HEBOCR_IMAGE_OPTIONS* options, int* progress );

/**
 new ho_layout 
 @param m_in a pointer to a text bitmap
 @param layout_options image layout options
 @param progress a progress indicator 0..100
 @return a newly allocated and filled layout
 */
ho_layout *hocr_layout_analysis( const ho_bitmap* m_in, HEBOCR_LAYOUT_OPTIONS layout_options, int* progress );

/**
 fill a text buffer with fonts recognized from a page layout

 @param l_page the page layout to recognize
 @param s_text_out the text buffer to fill
 @param font_options  the font options to be used when recognizing text
 @param html output HTML or plain text
 @param progress a progress indicator 0..100
 @return FALSE
 */
int hocr_font_recognition( const ho_layout* l_page, ho_string* s_text_out, HEBOCR_FONT_OPTIONS *font_options, int html, int* progress );

 /**
 do ocr on a pixbuf 

 @param pix_in the input ho_pixbuf
 @param s_text_out the text buffer to fill
 @param options image options to be used
 @param layout_options layout options to be used
 @param font_options font options to be user
 @param progress a progress indicator 0..100
 @return FALSE
 */
int hocr_do_ocr_fine( const ho_pixbuf * pix_in, ho_string * s_text_out, HEBOCR_IMAGE_OPTIONS *options, HEBOCR_LAYOUT_OPTIONS layout_options, HEBOCR_FONT_OPTIONS *font_options, int *progress);

 /**
 do ocr on a pixbuf, using default values

 @param pix_in the input ho_pixbuf
 @param s_text_out the text buffer to fill
 @param html output format is html
 @param font_code code for the font to use
 @param progress a progress indicator 0..100
 @return FALSE
 */
int hocr_do_ocr( const ho_pixbuf * pix_in, ho_string * s_text_out, const unsigned char html, int font_code, const unsigned char do_linguistics, int *progress );

/**
 return the build string

 @return build string
 */
  const char *hocr_get_build_string ();

/**
 return the version string

 @return version string
 */
  const char *hocr_get_version_string ();

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif                          /* HOCR_H */
