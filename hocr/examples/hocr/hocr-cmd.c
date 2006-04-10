
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

	printf ("USAGE: hocr [OPTION]... [-i] [file]\n\n");
	printf ("  If the file is a single dash, or no file\n\
  is given, PNM data is read from stdin\n\n");
	printf ("  -o file,   output to text file.\n");
#ifdef WITH_HSPELL
	printf ("  -e encoding,   output encoding can be 'utf-8' or 'iso8859-8' ...\n");
	printf ("  -d,   Use internal dictionary to guess misread fonts.\n");
#endif
	printf ("  -n,   Try to guess nikud for fonts.\n");
	printf ("  -s,   Use spaces for tabs.\n");
	printf ("  -t,   Indent indented lines.\n");
	printf ("  -p file,   write image with boxes around detected characters.\n");
	printf ("\n");

	return 0;
}

int
hocr_cmd_text_buffer_save (const hocr_text_buffer * const text_buffer,
			   const char *const filename)
{
	FILE *file;

	if (filename && filename[0])
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
			return -1;
		}

		fprintf (file, "%s", text_buffer->text);

		fclose (file);
	}
	else
	{
		/* 
		 * no file name - print to std output 
		 */
		printf ("%s", text_buffer->text);
	}

	return 0;
}

int
main (int argc, char *argv[])
{
	int opt_i = 0;
	int opt_o = 0;
	int opt_e = 0;
	int opt_d = 0;
	int opt_n = 0;
	int opt_s = 0;
	int opt_t = 0;
	int opt_p = 0;
	char c;

	char filename_in[STRING_MAX_SIZE];
	char filename_out[STRING_MAX_SIZE];
	char pnm_filename_out[STRING_MAX_SIZE];
	char encoding_out[STRING_MAX_SIZE];

	hocr_pixbuf *pix = NULL;
	hocr_text_buffer *text;

#ifdef WITH_GTK
	gtk_init (&argc, &argv);
#endif

	/* 
	 * default input file is stdin 
	 */
	filename_in[0] = '\0';

	/* 
	 * default encoding is "utf-8"
	 */
	encoding_out[0] = '\0';

	while ((c = getopt (argc, argv, "dnsthi:o:e:p:")) != EOF)
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
		case 'e':
			if (optarg && strlen (optarg) < STRING_MAX_SIZE)
			{
				strcpy (encoding_out, optarg);
				opt_e = 1;
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
	 * create a new pixbuf from file 
	 */
#ifdef WITH_GTK
	if (filename_in && filename_in[0])
	{
		pix = hocr_pixbuf_gtk_new_from_file (filename_in);
	}
	if (!pix)
#endif
	{
		pix = hocr_pixbuf_new_from_file (filename_in);
	}

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

#ifdef WITH_HSPELL

	/* do encoding change */
	if (opt_e)
	{
		hocr_text_buffer *text_buffer = NULL;

		/* convert to encoding_out */
		text_buffer = hocr_text_buffer_copy_convert (text,
							     encoding_out,
							     "utf-8");

		/* copy encoded text back to text */
		hocr_text_buffer_set_string (text, text_buffer->text);

		/* free temporary utf text buffer */
		hocr_text_buffer_unref (text_buffer);
	}

	/* 
	 * print out the text
	 */
	hocr_text_buffer_save (text, filename_out);

#else
	/* 
	 * print out the text
	 */
	hocr_cmd_text_buffer_save (text, filename_out);
#endif

	/* 
	 * save pic file
	 */
#ifdef WITH_GTK
	if (opt_p && pnm_filename_out && pnm_filename_out[0])
	{
		char *type;

		/* look for '.' */
		type = pnm_filename_out;
		while (type && type[0] != '.')
			type++;

		/* if no type save as jpeg */
		if (!type || strlen (type) > 5)
		{
			if (!hocr_pixbuf_gtk_save_as_file
			    (pix, pnm_filename_out, "jpeg"))
			{
				printf ("hocr: can't write file %s\n",
					pnm_filename_out);
			}
		}
		/* found type */
		else
		{
			/* lose the dot */
			type++;
			
			if (!hocr_pixbuf_gtk_save_as_file
			    (pix, pnm_filename_out, type))
			{
				printf ("hocr: can't write file %s\n",
					pnm_filename_out);
			}
		}
	}
	else
#endif
	{
		if (opt_p && !hocr_pixbuf_save_as_pnm (pix, pnm_filename_out))
		{
			printf ("hocr: can't write pnm file %s\n",
				pnm_filename_out);
		}
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
