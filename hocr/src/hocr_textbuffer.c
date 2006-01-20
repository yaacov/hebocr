
/***************************************************************************
 *            hocr_textbuffer.c
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "consts.h"
#include "hocr_textbuffer.h"

/* 
 * how much memory (bytes) hocr_text_buffer will 
 * allocate when it nead new memory 
 */
#define MEMORY_CHANK_FOR_TEXT_BUFFER 500

/* 
 * internal text_buffer stractures 
 */

hocr_text_buffer *
hocr_text_buffer_new ()
{
	hocr_text_buffer *new_text_buffer;

	/* allocate memory for struct */
		new_text_buffer =
		(hocr_text_buffer *) malloc (sizeof (hocr_text_buffer));

	/* check for new memory */
	if (!new_text_buffer)
		return 0;

	/* init struct values */
	new_text_buffer->size = 0;
	new_text_buffer->allocated_size = MEMORY_CHANK_FOR_TEXT_BUFFER;

	/* allocate memory for text */
	new_text_buffer->text =
		(char *) malloc (sizeof (char) *
				 new_text_buffer->allocated_size);

	/* if no memeory for text free the struct (no need for it with no text) 
	 */
	if (!new_text_buffer->text)
	{
		free (new_text_buffer);
		return 0;
	}

	/* init the text to zero length null terminated string */
	(new_text_buffer->text)[0] = '\0';

	return new_text_buffer;
}

int
hocr_text_buffer_unref (hocr_text_buffer * text_buffer)
{
	/* free the text */
	if (text_buffer->text)
		free (text_buffer->text);

	/* free the struct */
	if (text_buffer)
		free (text_buffer);

	return 1;
}

int
hocr_text_buffer_add_string (hocr_text_buffer * text_buffer,
			     const char *new_text)
{
	int len;
	char *new_allocated_text;

	len = strlen (new_text);

	/* check for allocated space and try to get more memory */
	if ((text_buffer->size + len) >= text_buffer->allocated_size)
	{
		new_allocated_text =
			realloc (text_buffer->text, sizeof (char) *
				 (text_buffer->allocated_size +
				  MEMORY_CHANK_FOR_TEXT_BUFFER));

		/* got new memory */
		if (new_allocated_text)
		{
			text_buffer->text = new_allocated_text;
			text_buffer->allocated_size +=
				MEMORY_CHANK_FOR_TEXT_BUFFER;
		}
		/* did not get new memory */
		else
		{
			return -1;
		}
	}

	/* add the new string */
	strcat (text_buffer->text, new_text);
	text_buffer->size = strlen (text_buffer->text);

	return 0;
}
