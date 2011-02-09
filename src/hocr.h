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

#include "hebocr_globals.h"
#include <ho_pixbuf.h>
#include <ho_bitmap.h>
#include <ho_string.h>

ho_bitmap *hocr_image_processing( const ho_pixbuf* pix_in, HEBOCR_IMAGE_OPTIONS* image_options, int* progress );

/**
 new ho_layout 
 @param m_in a pointer to a text bitmap
 @param layout_options image layout options
 @param progress a progress indicator 0..100
 @return a newly allocated and filled layout
 */
ho_layout *hocr_layout_analysis( const ho_bitmap* m_in, HEBOCR_LAYOUT_OPTIONS* layout_options, int* progress );

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
int hocr_do_ocr_fine( const ho_pixbuf * pix_in, ho_string * s_text_out, HEBOCR_IMAGE_OPTIONS *options, HEBOCR_LAYOUT_OPTIONS *layout_options, HEBOCR_FONT_OPTIONS *font_options, int *progress);

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
