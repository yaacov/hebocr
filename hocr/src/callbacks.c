/***************************************************************************
 *            callbacks.c
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gnome.h>
#include <gtkspell/gtkspell.h>

#include "callbacks.h"
#include "interface.h"
#include "hocr.h"

GdkPixbuf *pixbuf = NULL;
GdkPixbuf *vis_pixbuf = NULL;

static void
update_preview_cb (GtkFileChooser * file_chooser, gpointer data)
{
	GtkWidget *preview;
	char *filename;
	gboolean have_preview;
	GdkPixbuf *prev_pixbuf = NULL;
	
	preview = GTK_WIDGET (data);
	filename = gtk_file_chooser_get_preview_filename (file_chooser);
	
	prev_pixbuf =
		gdk_pixbuf_new_from_file_at_size (filename, 128, 128, NULL);
	have_preview = (prev_pixbuf != NULL);
	g_free (filename);

	gtk_image_set_from_pixbuf (GTK_IMAGE (preview), prev_pixbuf);
	
	if (prev_pixbuf)
	{
		gdk_pixbuf_unref (prev_pixbuf);
		vis_pixbuf = NULL;
	}

	gtk_file_chooser_set_preview_widget_active (file_chooser,
						    have_preview);
}

void
on_toolbutton_open_clicked (GtkToolButton * toolbutton, gpointer user_data)
{
	gint result;
	char *filename;
	//GdkPixbuf * pixbuf;
	GtkWidget *preview_frame = gtk_frame_new ("preview");
	GtkWidget *preview = gtk_image_new ();
	GtkWidget *my_file_chooser =
		gtk_file_chooser_dialog_new ("hocr open image",
					     GTK_WINDOW (window1),
					     GTK_FILE_CHOOSER_ACTION_OPEN,
					     GTK_STOCK_CANCEL,
					     GTK_RESPONSE_CANCEL,
					     GTK_STOCK_OPEN,
					     GTK_RESPONSE_ACCEPT,
					     NULL);

	gtk_widget_show (preview);
	gtk_container_add (GTK_CONTAINER (preview_frame), preview);
	gtk_file_chooser_set_preview_widget
		(GTK_FILE_CHOOSER (my_file_chooser), preview_frame);
	g_signal_connect (my_file_chooser, "update-preview",
			  G_CALLBACK (update_preview_cb), preview);

	result = gtk_dialog_run (GTK_DIALOG (my_file_chooser));

	if (result == GTK_RESPONSE_ACCEPT)
	{
		filename = gtk_file_chooser_get_preview_filename
			(GTK_FILE_CHOOSER (my_file_chooser));

		if (pixbuf)
		{
			gdk_pixbuf_unref (pixbuf);
			pixbuf = NULL;
		}
		
		pixbuf = gdk_pixbuf_new_from_file (filename, NULL);
		g_free (filename);

		on_toolbutton_zoom_fit_clicked (NULL, NULL);
	}

	gtk_widget_destroy (my_file_chooser);
}

int
save_text (char *filename)
{
	gchar *text;
	gboolean include_hidden_chars = FALSE;
	GtkTextIter start;
	GtkTextIter end;
	GtkTextBuffer *buffer;
	FILE *file;

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview));

	gtk_text_buffer_get_bounds (buffer, &start, &end);

	text = gtk_text_buffer_get_text (buffer, &start, &end,
					 include_hidden_chars);

	/* save to file */
	file = fopen (filename, "w");
	g_fprintf (file, "%s", text);
	fclose (file);

	g_free (text);
	return 0;
}

void
on_toolbutton_save_clicked (GtkToolButton * toolbutton, gpointer user_data)
{
	gint result;
	char *filename;

	GtkWidget *my_file_chooser =
		gtk_file_chooser_dialog_new ("hocr save text",
					     GTK_WINDOW (window1),
					     GTK_FILE_CHOOSER_ACTION_SAVE,
					     GTK_STOCK_CANCEL,
					     GTK_RESPONSE_CANCEL,
					     GTK_STOCK_SAVE,
					     GTK_RESPONSE_ACCEPT,
					     NULL);

	result = gtk_dialog_run (GTK_DIALOG (my_file_chooser));

	if (result == GTK_RESPONSE_ACCEPT)
	{
		filename = gtk_file_chooser_get_filename
			(GTK_FILE_CHOOSER (my_file_chooser));
		save_text (filename);
	}

	gtk_widget_destroy (my_file_chooser);
}

void
on_toolbutton_apply_clicked (GtkToolButton * toolbutton, gpointer user_data)
{
	GtkTextBuffer *text_buffer;
	int width, height;
	
	height = gdk_pixbuf_get_height (vis_pixbuf);
	width = gdk_pixbuf_get_width (vis_pixbuf);

	text_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview));

	if (pixbuf)
	{
		do_ocr (pixbuf, text_buffer);

		if (vis_pixbuf)
		{
			gdk_pixbuf_unref (vis_pixbuf);
			vis_pixbuf = NULL;
		}

		vis_pixbuf = gdk_pixbuf_scale_simple (pixbuf,
						      width, height,
						      GDK_INTERP_BILINEAR);

		gtk_image_set_from_pixbuf (GTK_IMAGE (image), vis_pixbuf);
	}
}

void
on_toolbutton_about_clicked (GtkToolButton * toolbutton, gpointer user_data)
{
	static const gchar *authors[] = {
		"Yaacov Zamir <kzamir@walla.co.il>",
		NULL
	};
	static const char *documenters[] = {
		"Yaacov Zamir <kzamir@walla.co.il>",
		NULL
	};

	gtk_show_about_dialog (GTK_WINDOW (window1), "name", _("HOCR"),
			       "version", VERSION,
			       "copyright",
			       "Copyright \xc2\xa9 2005 Yaacov Zamir",
			       "comments",
			       _
			       ("HOCR - Hebrew character recognition software"),
			       "authors", authors, "documenters", documenters,
			       "translator-credits", _("translator_credits"),
			       NULL);
}

void
on_toolbutton_spell_clicked (GtkToolButton * toolbutton, gpointer user_data)
{
	GtkSpell *spell = NULL;

	spell = gtkspell_get_from_text_view (GTK_TEXT_VIEW (textview));

	if (spell)
	{
		gtkspell_detach (spell);
	}
	else
	{
		spell = gtkspell_new_attach (GTK_TEXT_VIEW (textview), NULL,
					     NULL);
		gtkspell_set_language (spell, "he_IL", NULL);
	}
}

void
on_toolbutton_zoom_in_clicked (GtkToolButton * toolbutton, gpointer user_data)
{
	int width, height;

	height = gdk_pixbuf_get_height (vis_pixbuf);
	width = gdk_pixbuf_get_width (vis_pixbuf);

	if (pixbuf)
	{
		if (vis_pixbuf)
		{
			gdk_pixbuf_unref (vis_pixbuf);
			vis_pixbuf = NULL;
		}

		vis_pixbuf = gdk_pixbuf_scale_simple (pixbuf,
						      (int) ((double) width *
							     1.2),
						      (int) ((double) height *
							     1.2),
						      GDK_INTERP_BILINEAR);

		gtk_image_set_from_pixbuf (GTK_IMAGE (image), vis_pixbuf);
	}
}

void
on_toolbutton_zoom_out_clicked (GtkToolButton * toolbutton,
				gpointer user_data)
{
	int width, height;

	height = gdk_pixbuf_get_height (vis_pixbuf);
	width = gdk_pixbuf_get_width (vis_pixbuf);

	if (pixbuf)
	{
		if (vis_pixbuf)
		{
			gdk_pixbuf_unref (vis_pixbuf);
			vis_pixbuf = NULL;
		}

		vis_pixbuf = gdk_pixbuf_scale_simple (pixbuf,
						      (int) ((double) width *
							     0.8),
						      (int) ((double) height *
							     0.8),
						      GDK_INTERP_BILINEAR);

		gtk_image_set_from_pixbuf (GTK_IMAGE (image), vis_pixbuf);
	}
}

void
on_toolbutton_zoom_fit_clicked (GtkToolButton * toolbutton,
				gpointer user_data)
{
	int width, height, window_width, window_height;

	gtk_window_get_size (GTK_WINDOW (window1), &window_width,
			     &window_height);
	window_width -= 20;

	height = gdk_pixbuf_get_height (pixbuf);
	width = gdk_pixbuf_get_width (pixbuf);

	if (pixbuf)
	{
		if (vis_pixbuf)
		{
			gdk_pixbuf_unref (vis_pixbuf);
			vis_pixbuf = NULL;
		}

		vis_pixbuf = gdk_pixbuf_scale_simple (pixbuf,
						      window_width,
						      (int) ((double)
							     window_width *
							     (double) height /
							     (double) width),
						      GDK_INTERP_BILINEAR);

		gtk_image_set_from_pixbuf (GTK_IMAGE (image), vis_pixbuf);
	}
}

gboolean
on_window1_delete_event (GtkWidget * widget,
			 GdkEvent * event, gpointer user_data)
{
	if (pixbuf)
	{
		gdk_pixbuf_unref (pixbuf);
		pixbuf = NULL;
	}

	if (vis_pixbuf)
	{
		gdk_pixbuf_unref (vis_pixbuf);
		vis_pixbuf = NULL;
	}
	
	gtk_main_quit ();
	return FALSE;
}

void
on_toolbutton_quit_clicked (GtkToolButton * toolbutton, gpointer user_data)
{
	if (pixbuf)
	{
		gdk_pixbuf_unref (pixbuf);
		pixbuf = NULL;
	}

	if (vis_pixbuf)
	{
		gdk_pixbuf_unref (vis_pixbuf);
		vis_pixbuf = NULL;
	}

	gtk_main_quit ();
}
