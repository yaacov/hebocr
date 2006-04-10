
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

#ifdef WITH_HSPELL
#include <glib.h>
#include <glib/gstdio.h>
#include <hspell.h>
#endif

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
hocr_text_buffer_set_string (hocr_text_buffer * text_buffer,
			     const char *new_text)
{
	int len;
	char *new_allocated_text;

	len = strlen (new_text);

	/* check for allocated space and try to get more memory */
	new_allocated_text =
		(char *) malloc (sizeof (char) *
				 (len + MEMORY_CHANK_FOR_TEXT_BUFFER));

	/* got new memory */
	if (new_allocated_text)
	{
		if (text_buffer->text)
			free (text_buffer->text);
		text_buffer->text = new_allocated_text;
		text_buffer->allocated_size = len +
			MEMORY_CHANK_FOR_TEXT_BUFFER;
	}
	/* did not get new memory */
	else
	{
		return -1;
	}

	/* add the new string */
	strcpy (text_buffer->text, new_text);
	text_buffer->size = strlen (text_buffer->text);

	return 0;
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
				 (text_buffer->allocated_size + len +
				  MEMORY_CHANK_FOR_TEXT_BUFFER));

		/* got new memory */
		if (new_allocated_text)
		{
			text_buffer->text = new_allocated_text;
			text_buffer->allocated_size += len +
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

int
hocr_text_buffer_add_char (hocr_text_buffer * text_buffer, const char new_char)
{
	char *new_allocated_text;

	/* check for allocated space and try to get more memory */
	if ((text_buffer->size + 1) >= text_buffer->allocated_size)
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

	/* add the new char */
	(text_buffer->text)[text_buffer->size] = new_char;
	(text_buffer->text)[text_buffer->size + 1] = '\0';
	(text_buffer->size)++;

	return 0;
}

hocr_text_buffer *
hocr_text_buffer_copy (const hocr_text_buffer const *text_buffer)
{
	hocr_text_buffer *new_text = NULL;

	new_text = hocr_text_buffer_new ();

	hocr_text_buffer_add_string (new_text, text_buffer->text);

	return new_text;
}

#ifdef WITH_HSPELL

hocr_text_buffer *
hocr_text_buffer_copy_convert (const hocr_text_buffer const *text_buffer,
			       const gchar * to_codeset,
			       const gchar * from_codeset)
{
	hocr_text_buffer *new_text = NULL;
	gsize len_in, len_out;
	char *converted_text = NULL;
	char *new_allocated_text = NULL;
	GError *error = NULL;

	new_text = hocr_text_buffer_new ();

	converted_text = g_convert_with_fallback (text_buffer->text,
						  -1,
						  to_codeset, from_codeset,
						  "", &len_in, &len_out,
						  &error);

	/* get memory for new text buffer */
	new_allocated_text =
		realloc (new_text->text, sizeof (char) *
			 (new_text->allocated_size + len_out +
			  MEMORY_CHANK_FOR_TEXT_BUFFER));

	/* got new memory */
	if (new_allocated_text)
	{
		new_text->text = new_allocated_text;
		new_text->allocated_size +=
			len_out + MEMORY_CHANK_FOR_TEXT_BUFFER;

		/* copy converted_text */
		g_stpcpy (new_text->text, converted_text);
	}

	g_free (converted_text);

	return new_text;
}

int
hocr_text_buffer_save (const hocr_text_buffer * const text_buffer,
		       const char *const filename)
{
	FILE *file;

	if (filename && filename[0])
	{
		/* 
		 * save to file 
		 */
		file = g_fopen (filename, "w");

		/* 
		 * can't open file 
		 */
		if (!file)
		{
			return -1;
		}

		g_fprintf (file, "%s", text_buffer->text);

		fclose (file);
	}
	else
	{
		/* 
		 * no file name - print to std output 
		 */
		g_print ("%s", text_buffer->text);
	}

	return 0;
}

hocr_text_buffer *
hocr_text_buffer_copy_hspell (const hocr_text_buffer const *text_buffer)
{
	hocr_text_buffer *new_text = NULL;
	char ch = '\0';
	unsigned char word_index = 0;
	unsigned int text_index = 0;
	char word[256];
	const char *hspelled_word;
	struct dict_radix *hspell_dict = NULL;
	struct corlist cl;
	unsigned char res;
	int preflen;

	/* try to set a new hspell session */
	hspell_init (&hspell_dict, HSPELL_OPT_DEFAULT);

	new_text = hocr_text_buffer_new ();

	/* loop over all the text */
	text_index = 0;
	ch = (text_buffer->text)[text_index];
	while (ch)
	{
		/* coopy white spaces */
		while (ch && (g_ascii_isspace (ch) || g_ascii_ispunct (ch)))
		{
			hocr_text_buffer_add_char (new_text, ch);
			text_index++;
			ch = (text_buffer->text)[text_index];
		}

		/* copy next word */
		word_index = 0;
		while (ch && !(g_ascii_isspace (ch) || g_ascii_ispunct (ch)))
		{
			word[word_index] = ch;
			word_index++;
			text_index++;
			ch = (text_buffer->text)[text_index];
		}
		word[word_index] = '\0';

		/* insert word using hspell */
		/* if spelled correctly just copy */
		res = hspell_check_word (hspell_dict, word, &preflen);

		/* if not correct try gimatria */
		if (res != 1)
		{
			res = hspell_is_canonic_gimatria (word);
			if (res != 0)
				res = 1;
		}

		/* if spelled  */
		if (res == 1)
		{
			hocr_text_buffer_add_string (new_text, word);
		}
		else
		{
			/* get suggestions */
			corlist_init (&cl);

			hspell_trycorrect_ocr (hspell_dict, word, &cl);

			/* did we get suggestions ? */
			if (corlist_n (&cl) > 0)
			{
				/* FIME: take only the first suggestion ? */
				hspelled_word = corlist_str (&cl, 0);
				hocr_text_buffer_add_string (new_text,
							     hspelled_word);
			}
			/* no suggestion -> just copy */
			else
			{
				hocr_text_buffer_add_string (new_text, word);
			}

			/* free the list */
			corlist_free (&cl);
		}
	}

	/* deleting the dict is not posible on hspell ver. < v.0.8 */
#if (HSPELL_VERSION_MAJOR > 0) || (HSPELL_VERSION_MINOR >= 8)
	hspell_uninit (hspell_dict);
#endif

	return new_text;
}

#endif
