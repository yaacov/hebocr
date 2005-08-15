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

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "callbacks.h"
#include "interface.h"
#include "hocr.h"

GdkPixbuf *pixbuf = NULL;
GtkTextBuffer *text_buffer = NULL;

void
open_pic (char *filename)
{
	pixbuf = gdk_pixbuf_new_from_file (filename, NULL);

	if (!pixbuf)
	{
		g_print ("hocr: can't open file %s\n\n", filename);
		exit (0);
	}
}

int
save_text (char *filename)
{
	gchar *text;
	gboolean include_hidden_chars = FALSE;
	GtkTextIter start;
	GtkTextIter end;
	FILE *file;

	gtk_text_buffer_get_bounds (text_buffer, &start, &end);

	text = gtk_text_buffer_get_text (text_buffer, &start, &end,
					 include_hidden_chars);

	if (filename)
	{
		/* save to file */
		file = fopen (filename, "w");
		g_fprintf (file, "%s", text);
		fclose (file);
	}
	else
	{
		/* no file name - print to std output */
		g_printf ("%s", text);
	}

	g_free (text);
	return 0;
}

void
apply ()
{
	if (pixbuf)
	{
		do_ocr (pixbuf, text_buffer);
	}
}

int
print_help ()
{
	g_print ("\nhocr %s - Hebrew OCR program\n", VERSION);
	g_print ("USAGE: hocr -f pic_filename.jpg [-o text_filename.txt]\n");
	g_print ("\n");
}

int
main (int argc, char *argv[])
{
	int opt_f = 0;
	int opt_o = 0;
	char c;

	char filename_in[255];
	char filename_out[255];

	while ((c = getopt (argc, argv, "f:o:")) != EOF)
	{
		switch (c)
		{
		case 'f':
			opt_f = 1;
			if (optarg)
				g_strlcpy (filename_in, optarg, 255);
			break;
		case 'o':
			opt_o = 1;
			if (optarg)
				g_strlcpy (filename_out, optarg, 255);
			break;
		default:
			print_help ();
			exit (0);
			break;
		}
	}

	if (opt_f == 0)
	{
		print_help ();
		exit (0);
	}

	gnome_program_init (PACKAGE, VERSION, LIBGNOMEUI_MODULE,
			    argc, argv,
			    GNOME_PARAM_APP_DATADIR, PACKAGE_DATA_DIR, NULL);

	/* this is a gnome program - create widgets */
	text_buffer = gtk_text_buffer_new (NULL);

	/* open a file do ocr and dump the results */
	open_pic (filename_in);
	apply ();
	if (opt_o == 1)
		save_text (filename_out);
	else
		save_text (NULL);

	/* unref the alocated memory */
	if (pixbuf)
	{
		g_object_unref (pixbuf);
		pixbuf = NULL;
	}
	
	if (text_buffer)
	{
		g_object_unref (text_buffer);
		pixbuf = NULL;
	}

	/* exit */
	return 0;
}
