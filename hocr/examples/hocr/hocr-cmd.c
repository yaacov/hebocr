
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
	printf ("USAGE: hocr [OPTION]... [-i] [pnm_file]\n\n");
	printf ("  If the pnm file is a single dash, or no pnm file\n\
  is given, PNM data is read from stdin\n\n");
	printf ("  -o file,   output to text file.\n");
	printf ("  -f format,   output format can be 'html' or 'text'.\n");
	printf ("  -d,   Use internal dictionary to guess misread fonts.\n");
	printf ("  -n,   Try to guess nikud for fonts.\n");
	printf ("  -s,   Use spaces for tabs.\n");
	printf ("  -t,   Indent indented lines.\n");
	printf ("  -p file,   write PPM image with boxes around detected characters.\n");
	printf ("\n");

	return 0;
}

int
save_text (char *filename, char *format_out, char *text)
{
	FILE *file;

	/* 
	 * if format string begin with 'h' assume html output 
	 */
	if (filename)
	{
		/* 
		 * save to file 
		 */
		file = fopen (filename, "w");

		/* 
		 * can't open file 
		 */
		if (!file)
		{
			printf ("hocr: can\'t save file as %s\n", filename);
			exit (0);
		}

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
		/* 
		 * no file name - print to std output 
		 */
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
main (int argc, char *argv[])
{
	int opt_i = 0;
	int opt_o = 0;
	int opt_f = 0;
	int opt_d = 0;
	int opt_n = 0;
	int opt_s = 0;
	int opt_t = 0;
	int opt_p = 0;
	char c;

	char filename_in[STRING_MAX_SIZE];
	char filename_out[STRING_MAX_SIZE];
	char pnm_filename_out[STRING_MAX_SIZE];
	char format_out[STRING_MAX_SIZE];

	hocr_pixbuf *pix;
	hocr_text_buffer *text;

	/* 
	 * default output is text file 
	 */
	format_out[0] = 't';
	/* 
	 * default input file is stdin 
	 */
	filename_in[0] = '\0';

	while ((c = getopt (argc, argv, "dnsthi:o:f:p:")) != EOF)
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
		case 'p':
			if (optarg && strlen (optarg) < STRING_MAX_SIZE)
			{
				strcpy (pnm_filename_out, optarg);
				opt_p = 1;
			}
			break;
		case 'd':
			opt_d = 1;
			break;
		case 'n':
			opt_n = 1;
			break;
		case 's':
			opt_s = 1;
			break;
		case 't':
			opt_t = 1;
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

	/* check for one extra parameter */
	if (argc == (optind + 1))
	{
		/* i already have a file name: must be an error */
		if (opt_i == 1)
		{
			print_help ();
			exit (0);
		}
		/* this is a file name */
		if (opt_i == 0 && strlen (argv[optind]) < STRING_MAX_SIZE)
		{
			/* check for command fomrat "hocr filename" */
			strcpy (filename_in, argv[optind]);

			/* some times pepole use '-' to indecate stdin */
			if (filename_in[0] == '-' && filename_in[1] == '\0')
				filename_in[0] = '\0';

			opt_i = 1;
		}
	}
	/* if not one extra parameter must be no parameters */
	else if (argc != optind)
	{
		print_help ();
		exit (0);
	}

	/* 
	 * create a new pixbuf from pbm file 
	 */
	pix = hocr_pixbuf_new_from_file (filename_in);

	if (!pix)
	{
		printf ("hocr: can\'t read file %s\n", filename_in);
		exit (0);
	}

	/** do ocr */

	/* 
	 * build ocr command 
	 */
	pix->command = HOCR_COMMAND_OCR;

	/* use dict ? */
	if (opt_d)
		pix->command |= HOCR_COMMAND_DICT;

	/* use nikud ? */
	if (opt_n)
		pix->command |= HOCR_COMMAND_NIKUD;

	/* use spaces ? */
	if (opt_s)
		pix->command |= HOCR_COMMAND_USE_SPACE_FOR_TAB;

	/* use indentation ? */
	if (opt_t)
		pix->command |= HOCR_COMMAND_USE_INDENTATION;

	/* color misread font and text boxes */
	if (opt_p)
		pix->command |=
			HOCR_COMMAND_COLOR_BOXES | HOCR_COMMAND_COLOR_MISREAD;

	/* create text buffer */
	text = hocr_text_buffer_new ();

	if (!text)
	{
		printf ("hocr: can\'t allocate memory for text out\n");
		return 0;
	}

	/* 
	 * run the ocr 
	 */
	hocr_do_ocr (pix, text);

	/* 
	 * print out the text 
	 */
	if (opt_o)
		save_text (filename_out, format_out, text->text);
	else
		save_text (NULL, format_out, text->text);

	/* 
	 * save ppm file
	 */
	if (opt_p && !hocr_pixbuf_save_as_pnm (pix, pnm_filename_out))
	{
		printf ("hocr: can't write pnm file %s\n", pnm_filename_out);
	}

	/* 
	 * unref hocr_pixbuf
	 */
	hocr_pixbuf_unref (pix);

	/* 
	 * unref text_buffer 
	 */
	hocr_text_buffer_unref (text);

	return 0;
}
