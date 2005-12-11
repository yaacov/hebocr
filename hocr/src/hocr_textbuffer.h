
/***************************************************************************
 *            hocr_textbuffer.h
 *
 *  Sat Dec 10 21:00:29 2005
 *  Copyright  2005  Yacov Zamir
 *  <kzamir@walla.co.il>
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "consts.h"

#ifndef __HOCR_TEXTBUFFER_H__
#define __HOCR_TEXTBUFFER_H__

/** text buffer structore used by hocr
 
 This struct contain dynamicaly allocated string, You MUST
 use hocr_text_buffer_new and hocr_text_buffer_unref functions 
 to create this string and nuref it.
*/
typedef struct
{

	/** get the text buffer. */
	char *text;

	/** get number of chars in the text string. */
	int size;

	/** get number of chars allocated in memory. */
	int allocated_size;
} hocr_text_buffer;

/**
 @brief creats a new hocr_text_buffer struct from file 

 @return pointer to a newly allocate hocr_text_buffer, or null if can not open file.
 */
hocr_text_buffer *hocr_text_buffer_new ();

/**
 @brief free a hocr_text_buffer struct from memory

 @param text_buffer pointer to hocr_text_buffer struct.
 @return 1
 */
int hocr_text_buffer_unref (hocr_text_buffer * text_buffer);

/**
 @brief add a string to hocr_text_buffer struct

 @param text_buffer pointer to hocr_text_buffer struct.
 @param new_text pointer to the string to be added to the buffer.
 @return new size of text
 */
int hocr_text_buffer_add_string (hocr_text_buffer * text_buffer,
				 const char *new_text);

#endif
