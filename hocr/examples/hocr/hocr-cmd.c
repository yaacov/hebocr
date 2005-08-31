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

/* TODO: this all file is just a quick hack to get things done
         need to rewrite the whole thing NOT using gnome */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hocr.h"

int
save_text (char *filename, char *format_out, char *text)
{
	FILE *file;

	/* if format string begin with 'h' assume html output */
	if (filename)
	{
		/* save to file */
		file = fopen (filename, "w");

		if (format_out[0] == 'h')
			fprintf (file,
				   "<html>\n<meta http-equiv=\"Content-Type\" \
content=\"text/html; charset=UTF-8\">\n \
<body dir=\"rtl\"><pre>\n");

		fprintf (file, "%s", text);

		if (format_out[0] == 'h')
			fprintf (file, "</pre></body>\n</html>\n");
		fclose (file);
	}
	else
	{
		/* no file name - print to std output */
		if (format_out[0] == 'h')
			printf ("<html>\n<meta http-equiv=\"Content-Type\" \
content=\"text/html; charset=UTF-8\">\n \
<body dir=\"rtl\"><pre>\n");

		printf ("%s", text);

		if (format_out[0] == 'h')
			printf ("</pre></body>\n</html>\n");
	}

	return 0;
}

int
print_help ()
{
	printf ("hocr %s - Hebrew OCR program\n", VERSION);
	printf ("USAGE: hocr -i pic_filename [-o text_filename] [-f html/text]\n");
	printf ("\n");
	
	return 0;
}

int
main (int argc, char *argv[])
{
	int opt_i = 0;
	int opt_o = 0;
	int opt_f = 0;
	char c;

	char filename_in[255];
	char filename_out[255];
	char format_out[255];

	hocr_pixbuf * pix;
	hocr_text_buffer* text_buffer;
	
	/* default output is text file */
	format_out[0] = 't';

	while ((c = getopt (argc, argv, "hi:o:f:")) != EOF)
	{
		switch (c)
		{
		case 'i':
			opt_i = 1;
			if (optarg)
				strcpy (filename_in, optarg);
			break;
		case 'o':
			opt_o = 1;
			if (optarg)
				strcpy (filename_out, optarg);
			break;
		case 'f':
			opt_f = 1;
			if (optarg)
				strcpy (format_out, optarg);
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
	hocr_do_ocr (pix, text_buffer, HOCR_OUTPUT_JUST_OCR, 0);
	
	/* print out the text */
	if (opt_o == 1)
		save_text (filename_out, format_out, text_buffer->text);
	else
		save_text (NULL, format_out, text_buffer->text);
	
	/* unref memory */
	hocr_pixbuf_unref (pix);
	hocr_text_buffer_unref (text_buffer);
	
	return 0;
}
