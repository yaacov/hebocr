/***************************************************************************
 *            hocr-cmd.c
 *
 *  Fri Aug 12 20:13:33 2005
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

#define STRING_MAX_SIZE 255

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hocr.h"

int
print_help ()
{
	printf ("hocr %s - Hebrew OCR program\n", VERSION);
	printf ("http://hocr.berlios.de\n");
	printf ("\n");
	printf ("USAGE: hocr -i pic_filename [-o text_filename] [-f HTML/TEXT]\n");
	printf ("\n");

	return 0;
}

int
save_text (char *filename, char *text)
{
	FILE *file;

	/* if format string begin with 'h' assume html output */
	if (filename)
	{
		/* save to file */
		file = fopen (filename, "w");

		/* can't open file */
		if (!file)
		{
			printf ("hocr: can\'t save file as %s\n", filename);
			exit (0);
		}
		fprintf (file, "%s", text);
		fclose (file);
	}
	else
	{
		printf ("hocr: can\'t save file as %s\n", filename);
		exit (0);
	}

	return 0;
}

int
print_text (char *text)
{
	/* no file name - print to std output */
	printf ("%s", text);

	return 0;
}

int
main (int argc, char *argv[])
{
	int opt_i = 0;
	int opt_o = 0;
	int opt_f = 0;
	char c;

	char filename_in[STRING_MAX_SIZE];
	char filename_out[STRING_MAX_SIZE];
	char format_out[STRING_MAX_SIZE];

	hocr_error error;
	hocr_pixbuf *pix;
	hocr_text_buffer *text_buffer;

	hocr_format_strings format_strings;

	/* default output is text file */
	format_out[0] = 't';

	while ((c = getopt (argc, argv, "hi:o:f:")) != EOF)
	{
		switch (c)
		{
		case 'i':
			if (optarg && strlen (optarg) < STRING_MAX_SIZE)
			{
				strcpy (filename_in, optarg);
				opt_i = 1;
			}
			break;
		case 'o':
			if (optarg && strlen (optarg) < STRING_MAX_SIZE)
			{
				strcpy (filename_out, optarg);
				opt_o = 1;
			}
			break;
		case 'f':
			if (optarg && strlen (optarg) < STRING_MAX_SIZE)
			{
				strcpy (format_out, optarg);
				opt_f = 1;
			}
			break;
		case 'h':
			print_help ();
			exit (0);
			break;
		default:
			print_help ();
			exit (0);
			break;
		}
	}

	if (opt_i == 0)
	{
		print_help ();
		exit (0);
	}

	/* create a new pixbuf from pbm file */
	pix = hocr_pixbuf_new_from_file (filename_in);

	if (!pix)
	{
		printf ("hocr: can\'t read file %s\n", filename_in);
		exit (0);
	}

	/* create text buffer */
	text_buffer = hocr_text_buffer_new ();

	if (!text_buffer)
	{
		printf ("hocr: can\'t allocate memory for text out\n");
		exit (0);
	}

	/* do ocr */
	if (opt_f && (format_out[0] == 'H' || format_out[0] == 'h'))
		/* html output */
	{
		strcpy (format_strings.page_start_string,
			"<html>\n<meta http-equiv=\"Content-Type\"\
content=\"text/html; charset=UTF-8\">\n\
<body dir=\"rtl\">\n\
<table>\n<tr>\n");
		strcpy (format_strings.page_end_string,
			"</tr>\n</table>\n</body>\n</html>\n");
		strcpy (format_strings.column_start_string, "<td>\n");
		strcpy (format_strings.column_end_string, "</td>\n");
		strcpy (format_strings.paragraph_start_string, "<p>\n");
		strcpy (format_strings.paragraph_end_string, "</p>\n");
		strcpy (format_strings.line_start_string, "");
		strcpy (format_strings.line_end_string, "</br>");
		strcpy (format_strings.unknown_start_string, "<b>");
		strcpy (format_strings.unknown_end_string, "</b>");

		hocr_do_ocr (pix, text_buffer, &format_strings,
			     HOCR_OUTPUT_JUST_OCR, HOCR_OCR_TYPE_COLUMNS,
			     &error);
	}
	else
		/* text output */
	{
		hocr_do_ocr (pix, text_buffer, 0, HOCR_OUTPUT_JUST_OCR,
			     HOCR_OCR_TYPE_REGULAR, &error);
	}

	/* did do_ocr return an error ? */
	if (error != HOCR_ERROR_OK)
	{
		switch (error)
		{
		case HOCR_ERROR_NO_LINES_FOUND:
			printf ("hocr: can\'t find readble lines in input\n");

			/* unref memory and exit */
			hocr_pixbuf_unref (pix);
			hocr_text_buffer_unref (text_buffer);
			exit (0);

			break;
		case HOCR_ERROR_NO_FONTS_FOUND:
			printf ("hocr: can\'t find readble fonts in input\n");

			/* unref memory and exit */
			hocr_pixbuf_unref (pix);
			hocr_text_buffer_unref (text_buffer);
			exit (0);

			break;
		case HOCR_ERROR_OUT_OF_MEMORY:
			printf ("hocr: out of memory while reading\n");
			break;
		case HOCR_ERROR_NOT_HORIZONTAL_LINE:
			printf ("hocr: found non horizontal line in text\n");
			break;
		default:
			printf ("hocr: unknown error while reading\n");
			exit (0);
			break;
		}
	}

	/* print out the text */
	if (opt_o == 1)
		save_text (filename_out, text_buffer->text);
	else
		print_text (text_buffer->text);

	/* unref memory */
	hocr_pixbuf_unref (pix);
	hocr_text_buffer_unref (text_buffer);

	return 0;
}
