
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

#ifndef HOCR_H
#define HOCR_H 1

#ifdef __cplusplus
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS   }
#else /* !__cplusplus */
#  define BEGIN_C_DECLS
#  define END_C_DECLS
#endif /* __cplusplus */

BEGIN_C_DECLS

/* color image map */
#include <ho_pixbuf.h>

/* binary image map */
#include <ho_bitmap.h>

/* object image map */
#include <ho_objmap.h>

/* layout container for hocr segmentations */
#include <ho_layout.h>
#include <ho_dimentions.h>
#include <ho_segment.h>

/* font recognition */
#include <ho_font.h>
#include <ho_recognize.h>

/* specific font recognition */
#include <ho_recognize_font_david.h>

/* string utilities */
#include <ho_string.h>

/* wrapper function */

/**
 convert a gray pixbuf to bitmap 

 @param pix_in the input ho_pixbuf
 @param scale the scale to use
 @param no_auto_scale do not try to autoscale
 @param rotate the rotation angle to use
 @param no_auto_rotate do not try to autorotate
 @param adaptive what type of thresholding to use. 0-normal,1-no,2-fine.
 @param threshold the threshold to use 0..100 (0-auto)
 @param a_threshold the threshold to use for adaptive thresholding 0..100 (0-auto)
 @return newly allocated gray ho_bitmap
 */
  ho_bitmap *
hocr_image_processing (const ho_pixbuf * pix_in,
  const unsigned char scale,
  const unsigned char no_auto_scale,
  double rotate,
  const unsigned char no_auto_rotate,
  const unsigned char adaptive,
  const unsigned char threshold, const unsigned char a_threshold);

/**
 new ho_layout 

 @param m_in a pointer to a text bitmap
 @param font_spacing_code -3 tight .. 0 .. 3 spaced
 @param paragraph_setup free text blocks or boxed in columns
 @param slicing_threshold percent of line fill to cut fonts
 @param slicing_width what is a wide font
 @param dir true-ltr false-rtl
 @return a newly allocated and filled layout
 */
ho_layout *hocr_layout_analysis (const ho_bitmap * m_in,
  const int font_spacing_code, const int paragraph_setup,
  const int slicing_threshold, const int slicing_width,
  const unsigned char dir_ltr);

/**
 fill a text buffer with fonts recognized from a page layout

 @param l_page the page layout to recognize
 @param s_text_out the text buffer to fill
 @param html output format is html
 @return FALSE
 */
int
hocr_font_recognition (const ho_layout * l_page, ho_string * s_text_out,
  const unsigned char html);

END_C_DECLS
#endif /* HOCR_H */
