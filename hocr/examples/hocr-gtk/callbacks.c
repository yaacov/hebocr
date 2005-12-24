
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

#include <gtk/gtk.h>
#include <glib/gprintf.h>

#ifdef WITH_GTKSPELL
#include <gtkspell/gtkspell.h>
#endif

#include "callbacks.h"
#include "interface.h"
#include "hocr.h"

GdkPixbuf *pixbuf = NULL;
GdkPixbuf *vis_pixbuf = NULL;

typedef struct _text_struct
{
	GtkTextBuffer *text_buffer;
	hocr_text_buffer *text;
} text_struct;

gboolean
draw_progress_bar (gpointer data)
{
	hocr_pixbuf *hocr_pix = (hocr_pixbuf *) data;

	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (pbar),
				       (double) hocr_pix->progress / 256.0);

	return TRUE;
}

gboolean
redraw_pixbuf (gpointer data)
{
	int width, height;
	GtkTextBuffer *text_buffer = ((text_struct*)data)->text_buffer;
	hocr_text_buffer *text = ((text_struct*)data)->text;
	GtkTextIter iter;
	
	height = gdk_pixbuf_get_height (vis_pixbuf);
	width = gdk_pixbuf_get_width (vis_pixbuf);

	if (pixbuf)
	{
		if (vis_pixbuf)
		{
			g_object_unref (vis_pixbuf);
			vis_pixbuf = NULL;
		}

		vis_pixbuf = gdk_pixbuf_scale_simple (pixbuf, width,
						      height,
						      GDK_INTERP_BILINEAR);

		gtk_image_set_from_pixbuf (GTK_IMAGE (image), vis_pixbuf);
	}

	/* insert the text to the text editor */
	gtk_text_buffer_get_end_iter (text_buffer, &iter);

	gtk_text_buffer_insert (text_buffer, &iter, text->text, -1);
	
	/* unref text_buffer */
	hocr_text_buffer_unref (text);

	gtk_widget_queue_draw (textview);
	gtk_widget_queue_draw (image);

	return FALSE;
}

gpointer
ocr_thread (gpointer data)
{
	static GtkTextBuffer *text_buffer;
	static hocr_text_buffer *text;
	static hocr_pixbuf *hocr_pix;
	static text_struct text_struct_instance;

	guint timeout_id;

	text_buffer = (GtkTextBuffer *) data;
	
	hocr_pix = hocr_pixbuf_new ();	/* get an empty hocr_pix */
	if (!hocr_pix)
	{
		printf ("hocr-gtk: can\'t allocate memory for picture\n");
		return 0;
	}

	/* create text buffer */
	text = hocr_text_buffer_new ();
	if (!text)
	{
		printf ("hocr-gtk: can\'t allocate memory for text out\n");
		return 0;
	}

	/* clear text before ocr ? */
	if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM (clear_text)))
		gtk_text_buffer_set_text (text_buffer, "", -1);

	/* init command */
	hocr_pix->command = 0;

	/* color boxes ? */
	if (gtk_check_menu_item_get_active
	    (GTK_CHECK_MENU_ITEM (color_text_box)))
		hocr_pix->command |= HOCR_COMMAND_COLOR_BOXES;

	/* color misread fonts ? */
	if (gtk_check_menu_item_get_active
	    (GTK_CHECK_MENU_ITEM (color_misread)))
		hocr_pix->command |= HOCR_COMMAND_COLOR_MISREAD;

	/* do ocr ? */
	if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM (ocr)))
		hocr_pix->command |= HOCR_COMMAND_OCR;

	/* use dict ? */
	if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM (use_dict)))
		hocr_pix->command |= HOCR_COMMAND_DICT;

	/* use nikud ? */
	if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM (use_nikud)))
		hocr_pix->command |= HOCR_COMMAND_NIKUD;

	/* use spaces ? */
	if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM (use_spaces)))
		hocr_pix->command |= HOCR_COMMAND_USE_SPACE_FOR_TAB;

	/* use indentation ? */
	if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM (use_indent)))
		hocr_pix->command |= HOCR_COMMAND_USE_INDENTATION;

	hocr_pix->n_channels = gdk_pixbuf_get_n_channels (pixbuf);
	hocr_pix->height = gdk_pixbuf_get_height (pixbuf);
	hocr_pix->width = gdk_pixbuf_get_width (pixbuf);
	hocr_pix->rowstride = gdk_pixbuf_get_rowstride (pixbuf);
	hocr_pix->pixels = (unsigned char *) (gdk_pixbuf_get_pixels (pixbuf));

	/* open the ocr thread */
	hocr_pix->progress = 0;
	hocr_pix->progress_phase = 0;

	/* set time out function to draw the progress bar */
	timeout_id =
		gtk_timeout_add (200, draw_progress_bar, (gpointer) hocr_pix);

	/* call the ocr function */
	hocr_do_ocr (hocr_pix, text);

	/* remove time out function to draw the progress bar */
	gtk_timeout_remove (timeout_id);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (pbar), 0.0);

	/* unref hocr_pixbuf */
	hocr_pix->pixels = NULL;
	hocr_pixbuf_unref (hocr_pix);

	/* redraw the pixbuf */
	text_struct_instance.text = text;
	text_struct_instance.text_buffer = text_buffer;
	gtk_timeout_add (100, redraw_pixbuf, (gpointer) &text_struct_instance);

	return data;
}

int
do_ocr (GdkPixbuf * pixbuf, GtkTextBuffer * text_buffer)
{
	/* set ocr as a thread */
	g_thread_create (ocr_thread, (gpointer) text_buffer, FALSE, NULL);

	return 1;
}

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

	if (prev_pixbuf)
	{
		gtk_image_set_from_pixbuf (GTK_IMAGE (preview), prev_pixbuf);
		g_object_unref (prev_pixbuf);
		vis_pixbuf = NULL;
	}

	gtk_file_chooser_set_preview_widget_active (file_chooser, have_preview);
}

void
on_toolbutton_open_clicked (GtkToolButton * toolbutton, gpointer user_data)
{
	gint result;
	char *filename;
	char title[255];

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
			g_object_unref (pixbuf);
			pixbuf = NULL;
		}

		pixbuf = gdk_pixbuf_new_from_file (filename, NULL);

		/* set the window title */
		g_snprintf (title, 255, "%s - %s", _("hocr-gtk"), filename);
		gtk_window_set_title (GTK_WINDOW (window1), title);

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
			g_object_unref (vis_pixbuf);
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

	gtk_show_about_dialog (GTK_WINDOW (window1), "name", _("HOCR-GTK"),
			       "version", VERSION,
			       "copyright",
			       "Copyright \xc2\xa9 2005 Yaacov Zamir",
			       "comments",
			       _
			       ("HOCR-GTK - Hebrew character recognition software"),
			       "authors", authors, "documenters", documenters,
			       "translator-credits", _("translator_credits"),
			       NULL);
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
			g_object_unref (vis_pixbuf);
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
on_toolbutton_zoom_out_clicked (GtkToolButton * toolbutton, gpointer user_data)
{
	int width, height;

	height = gdk_pixbuf_get_height (vis_pixbuf);
	width = gdk_pixbuf_get_width (vis_pixbuf);

	if (pixbuf)
	{
		if (vis_pixbuf)
		{
			g_object_unref (vis_pixbuf);
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
on_toolbutton_zoom_fit_clicked (GtkToolButton * toolbutton, gpointer user_data)
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
			g_object_unref (vis_pixbuf);
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

#ifdef WITH_GTKSPELL
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
#endif

gboolean
on_window1_delete_event (GtkWidget * widget,
			 GdkEvent * event, gpointer user_data)
{
	if (pixbuf)
	{
		g_object_unref (pixbuf);
		pixbuf = NULL;
	}

	if (vis_pixbuf)
	{
		g_object_unref (vis_pixbuf);
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
		g_object_unref (pixbuf);
		pixbuf = NULL;
	}

	if (vis_pixbuf)
	{
		g_object_unref (vis_pixbuf);
		vis_pixbuf = NULL;
	}

	g_free (font_name);

	gtk_main_quit ();
}

/* menu */
void
on_open_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	on_toolbutton_open_clicked (NULL, NULL);
}

void
on_apply_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	on_toolbutton_apply_clicked (NULL, NULL);
}

void
on_save_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	on_toolbutton_save_clicked (NULL, NULL);
}

void
on_quit_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	on_toolbutton_quit_clicked (NULL, NULL);
}

void
on_font_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	GtkWidget *fsd;
	PangoFontDescription *font_desc;
	gint result;

	/* create font dialog */
	fsd = gtk_font_selection_dialog_new (_("Set text font"));
	gtk_font_selection_dialog_set_font_name (GTK_FONT_SELECTION_DIALOG
						 (fsd), font_name);

	/* run dialog */
	result = gtk_dialog_run (GTK_DIALOG (fsd));
	switch (result)
	{
	case GTK_RESPONSE_OK:
		/* get the new font name */
		g_free (font_name);
		font_name = g_strdup (gtk_font_selection_dialog_get_font_name
				      (GTK_FONT_SELECTION_DIALOG (fsd)));

		/* Change default font throughout the text widget */
		font_desc = pango_font_description_from_string (font_name);

		gtk_widget_modify_font (textview, font_desc);
		pango_font_description_free (font_desc);

		break;
	default:
		/* do nothing since dialog was cancelled */
		break;
	}

	/* free dialog */
	gtk_widget_destroy (fsd);
}

#ifdef WITH_GTKSPELL
void
on_spell_check_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	on_toolbutton_spell_clicked (NULL, NULL);
}
#endif

void
on_zoom_in_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	on_toolbutton_zoom_in_clicked (NULL, NULL);
}

void
on_zoom_out_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	on_toolbutton_zoom_out_clicked (NULL, NULL);
}

void
on_normal_size_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	on_toolbutton_zoom_fit_clicked (NULL, NULL);
}

void
on_about_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	on_toolbutton_about_clicked (NULL, NULL);
}
