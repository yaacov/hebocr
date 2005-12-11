
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

#include "consts.h"
#include "hocr_textbuffer.h"
#include "hocr_pixbuf.h"
#include "hocr_object.h"

#ifndef __HOCR_H__
#define __HOCR_H__

#ifdef __cplusplus
extern "C"
{
#endif

	int print_font (hocr_pixbuf * pix, hocr_box font);

/* user only use this function */

/**
 @brief do ocr on a hocr_pixbuf and return the result text to text_buffer

 @param pix pointer to hocr_pixbuf struct.
 @param text_buffer pointer to an already allocated text buffer for the results
 @param max_buffer_size site of allocated memory for text_buffer.
 @return 1
 */
	int hocr_do_ocr (hocr_pixbuf * pix, hocr_text_buffer * text_buffer);

#ifdef __cplusplus
}
#endif

#endif
