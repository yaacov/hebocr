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

#include <glib.h>
#include <glib/gprintf.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "callbacks.h"
#include "interface.h"
#include "hocr.h"

GdkPixbuf *pixbuf = NULL;
GtkTextBuffer *text_buffer = NULL;

int
do_ocr (GdkPixbuf * pixbuf, GtkTextBuffer * text_buffer)
{
	hocr_pixbuf hocr_pix;
	char text[1500];
	GtkTextIter iter;

	hocr_pix.n_channels = gdk_pixbuf_get_n_channels (pixbuf);
	hocr_pix.height = gdk_pixbuf_get_height (pixbuf);
	hocr_pix.width = gdk_pixbuf_get_width (pixbuf);
	hocr_pix.rowstride = gdk_pixbuf_get_rowstride (pixbuf);
	hocr_pix.pixels = (unsigned char*)(gdk_pixbuf_get_pixels (pixbuf));
	
	g_strlcpy (text, "", 1500);
	hocr_do_ocr (&hocr_pix, text, 1500);

	gtk_text_buffer_get_end_iter (text_buffer, &iter);
	gtk_text_buffer_insert (text_buffer, &iter, text, -1);

	return 1;
}

void
open_pic (char *filename)
{
	char tmp[255];

	pixbuf = gdk_pixbuf_new_from_file (filename, NULL);

	/* if this file is unreadble try to assume it is ppm */
	/* TODO: ugly, usr pixbuf loader to set ppm file type */
	if (!pixbuf)
	{
		g_snprintf (tmp, 255, "/tmp/hocr-testfile-%s.ppn", filename);
		g_rename (filename, tmp);
		pixbuf = gdk_pixbuf_new_from_file (tmp, NULL);
		g_rename (tmp, filename);
	}

	/* it is not a ppm or any other readble file then can't open :( */
	if (!pixbuf)
	{
		g_print (_("hocr: can't open file %s\n\n"), filename);
		exit (0);
	}
}

int
save_text (char *filename, char *format_out)
{
	gchar *text;
	gboolean include_hidden_chars = FALSE;
	GtkTextIter start;
	GtkTextIter end;
	FILE *file;

	gtk_text_buffer_get_bounds (text_buffer, &start, &end);

	text = gtk_text_buffer_get_text (text_buffer, &start, &end,
					 include_hidden_chars);

	/* if format string begin with 'h' assume html output */
	if (filename)
	{
		/* save to file */
		file = fopen (filename, "w");

		if (format_out[0] == 'h')
			g_fprintf (file,
				   "<html>\n<meta http-equiv=\"Content-Type\" \
content=\"text/html; charset=UTF-8\">\n \
<body dir=\"rtl\"><pre>\n");

		g_fprintf (file, "%s", text);

		if (format_out[0] == 'h')
			g_fprintf (file, "</pre></body>\n</html>\n");
		fclose (file);
	}
	else
	{
		/* no file name - print to std output */
		if (format_out[0] == 'h')
			g_printf ("<html>\n<meta http-equiv=\"Content-Type\" \
content=\"text/html; charset=UTF-8\">\n \
<body dir=\"rtl\"><pre>\n");

		g_printf ("%s", text);

		if (format_out[0] == 'h')
			g_printf ("</pre></body>\n</html>\n");
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
	g_print (_("hocr %s - Hebrew OCR program\n"), VERSION);
	g_print (_
		 ("USAGE: hocr -i pic_filename [-o text_filename] [-f html/text]\n"));
	g_print ("\n");
}

int
main (int argc, char *argv[])
{
	int opt_i = 0;
	int opt_o = 0;
	int opt_f = 0;
	int opt_html = 0;
	int opt_txt = 1;
	char c;

	char filename_in[255];
	char filename_out[255];
	char format_out[255];

	/* default output is text file */
	format_out[0] = 't';

	while ((c = getopt (argc, argv, "i:o:f:")) != EOF)
	{
		switch (c)
		{
		case 'i':
			opt_i = 1;
			if (optarg)
				g_strlcpy (filename_in, optarg, 255);
			break;
		case 'o':
			opt_o = 1;
			if (optarg)
				g_strlcpy (filename_out, optarg, 255);
			break;
		case 'f':
			opt_f = 1;
			if (optarg)
				g_strlcpy (format_out, optarg, 255);
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

	gtk_set_locale ();
	gtk_init (&argc, &argv);

	/* this is a gtk program - create widgets */
	text_buffer = gtk_text_buffer_new (NULL);

	/* open a file do ocr and dump the results */
	open_pic (filename_in);
	apply ();
	if (opt_o == 1)
		save_text (filename_out, format_out);
	else
		save_text (NULL, format_out);

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
