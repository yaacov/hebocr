
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

#include <glib.h>
#include <glib/gprintf.h>
#include <glib/gstdio.h>

#include <gtk/gtk.h>

#ifdef WITH_GTKSPELL
#include <gtkspell/gtkspell.h>
#endif

#include "callbacks.h"
#include "interface.h"
#include "hocr.h"
#include "hocr_pixbuf.h"

static GStaticMutex mutex = G_STATIC_MUTEX_INIT;

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
	GtkTextBuffer *text_buffer = ((text_struct *) data)->text_buffer;
	hocr_text_buffer *text = ((text_struct *) data)->text;
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

	/* only one run is posible */
	if (!g_static_mutex_trylock (&mutex))
		return data;

	text_buffer = (GtkTextBuffer *) data;

	/* create hocr picture */
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
	gtk_timeout_add (100, redraw_pixbuf, (gpointer) & text_struct_instance);

	/* unlock mutex */
	g_static_mutex_unlock (&mutex);

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

	GtkWidget *preview_frame;
	GtkWidget *preview;
	GtkWidget *my_file_chooser;

	/* only one run is posible */
	if (!g_static_mutex_trylock (&mutex))
		return;

	preview_frame = gtk_frame_new ("preview");

	preview = gtk_image_new ();

	my_file_chooser =
		gtk_file_chooser_dialog_new ("hocr open image",
					     GTK_WINDOW (window1),
					     GTK_FILE_CHOOSER_ACTION_OPEN,
					     GTK_STOCK_CANCEL,
					     GTK_RESPONSE_CANCEL,
					     GTK_STOCK_OPEN,
					     GTK_RESPONSE_ACCEPT, NULL);

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

	/* unlock mutex */
	g_static_mutex_unlock (&mutex);

	return;
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
		gtk_file_chooser_dialog_new (_("hocr save text"),
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
on_save_picture_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	gint result;
	char *filename;
	GError *error = NULL;
	gchar **filename_split;
	GtkWidget *dialog;
	GtkWidget *my_file_chooser;

	/* check that user gave a suffix */
	if (!pixbuf)
	{
		dialog = gtk_message_dialog_new
			(GTK_WINDOW (window1),
			 GTK_DIALOG_DESTROY_WITH_PARENT,
			 GTK_MESSAGE_ERROR,
			 GTK_BUTTONS_CLOSE,
			 _("Error wile saving:\nNo picture to save"));

		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);

		return;
	}

	my_file_chooser =
		gtk_file_chooser_dialog_new (_("hocr save picture"),
					     GTK_WINDOW (window1),
					     GTK_FILE_CHOOSER_ACTION_SAVE,
					     GTK_STOCK_CANCEL,
					     GTK_RESPONSE_CANCEL,
					     GTK_STOCK_SAVE,
					     GTK_RESPONSE_ACCEPT, NULL);

	result = gtk_dialog_run (GTK_DIALOG (my_file_chooser));

	if (result == GTK_RESPONSE_ACCEPT)
	{
		filename = gtk_file_chooser_get_filename
			(GTK_FILE_CHOOSER (my_file_chooser));
		if (filename)
		{
			filename_split = g_strsplit (filename, ".", 2);

			/* check that user gave a suffix */
			if (!filename_split[1] || !filename_split[0])
			{
				dialog = gtk_message_dialog_new
					(GTK_WINDOW (window1),
					 GTK_DIALOG_DESTROY_WITH_PARENT,
					 GTK_MESSAGE_ERROR,
					 GTK_BUTTONS_CLOSE,
					 _
					 ("Error wile saving:\nCan't save %s, no suffix"),
					 filename);

				gtk_dialog_run (GTK_DIALOG (dialog));
				gtk_widget_destroy (dialog);
			}
			/* user did gave a suffixe */
			else
			{
				if (gdk_pixbuf_save
				    (pixbuf, filename, filename_split[1],
				     &error, NULL))
				{
					dialog = gtk_message_dialog_new
						(GTK_WINDOW (window1),
						 GTK_DIALOG_DESTROY_WITH_PARENT,
						 GTK_MESSAGE_INFO,
						 GTK_BUTTONS_CLOSE,
						 _("File %s saved"), filename);

					gtk_dialog_run (GTK_DIALOG (dialog));
					gtk_widget_destroy (dialog);
				}
				else
				{
					dialog = gtk_message_dialog_new
						(GTK_WINDOW (window1),
						 GTK_DIALOG_DESTROY_WITH_PARENT,
						 GTK_MESSAGE_ERROR,
						 GTK_BUTTONS_CLOSE,
						 "Error wile saving:\n%s",
						 error->message);

					gtk_dialog_run (GTK_DIALOG (dialog));
					gtk_widget_destroy (dialog);
				}
			}

			g_strfreev (filename_split);
		}
	}

	gtk_widget_destroy (my_file_chooser);
}

gboolean
draw_pixbuf (gpointer data)
{
	/* set the window title */
	gtk_window_set_title (GTK_WINDOW (window1), _("hocr-gtk - xsane"));

	on_toolbutton_zoom_fit_clicked (NULL, NULL);

	return FALSE;
}

gpointer
run_xsane (gpointer data)
{
	GError *error = NULL;
	gchar *xsane_pipe_pathname = NULL;
	gchar *xsane_command_line = NULL;

	/* check for pipe to xsane and create if missing */
	xsane_pipe_pathname = g_strdup_printf ("%s%s%s", g_get_home_dir (),
					       G_DIR_SEPARATOR_S,
					       ".hocr-gtk-xsane-pipe.pnm");
	xsane_command_line =
		g_strdup_printf ("xsane -s -n -N '%s'", xsane_pipe_pathname);
	/* remove pipe if exist */
	g_remove (xsane_pipe_pathname);
	if (g_spawn_command_line_sync (xsane_command_line,
				       NULL, NULL, NULL, &error))
		/* read file */
	{
		GdkPixbuf *new_pixbuf = NULL;

		new_pixbuf =
			gdk_pixbuf_new_from_file (xsane_pipe_pathname, NULL);

		/* check if realy a picture file */
		if (new_pixbuf)
		{
			/* unreff old picture */
			if (pixbuf)
			{
				g_object_unref (pixbuf);
			}

			/* replace old pixbuf */
			pixbuf = new_pixbuf;

			gtk_timeout_add (100, draw_pixbuf, (gpointer) NULL);
		}
	}

	/* free path name string */
	g_free (xsane_pipe_pathname);
	g_free (xsane_command_line);
	gtk_window_deiconify (GTK_WINDOW (window1));
	return data;
}

void
on_xsane_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	/* only one run is posible */
	if (!g_static_mutex_trylock (&mutex))
		return;

	/* is xsane present */
	if (!g_find_program_in_path ("xsane"))
	{
		GtkWidget *dialog;

		dialog = gtk_message_dialog_new
			(GTK_WINDOW (window1),
			 GTK_DIALOG_DESTROY_WITH_PARENT,
			 GTK_MESSAGE_ERROR,
			 GTK_BUTTONS_CLOSE,
			 "Error wile opening xsane:\nCan't find xsane on system path.");

		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
	}
	else
	{
		/* gtk_window_iconify (GTK_WINDOW (window1)); */
		g_thread_create (run_xsane, NULL, FALSE, NULL);
	}

	/* unlock mutex */
	g_static_mutex_unlock (&mutex);

	return;
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
	GError *error = NULL;
	GdkPixbuf *hocr_logo = gdk_pixbuf_new_from_file (LOGO, &error);

	static const gchar *authors[] = {
		"Yaacov Zamir <kzamir@walla.co.il>",
		NULL
	};
	static const char *documenters[] = {
		"Yaacov Zamir <kzamir@walla.co.il>",
		NULL
	};
	static const char *artists[] = {
		"Shlomi Israel <sijproject@gmail.com>",
		NULL
	};
	gtk_show_about_dialog (GTK_WINDOW (window1),
			       "name", _("HOCR-GTK"),
			       "version", VERSION,
			       "copyright", "Copyright \xc2\xa9 2005 Yaacov Zamir", "comments", _("HOCR-GTK - Hebrew character recognition software.\n\
This project is supported by a grant form the Israeli Internet Association."), "authors", authors,
			       "documenters", documenters, "translator-credits", _("translator_credits"), "artists", artists, "website", "http://hocr.berlios.de", "logo", hocr_logo, NULL);
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
						      (int) ((double)
							     width *
							     1.2),
						      (int) ((double)
							     height *
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
						      (int) ((double)
							     width *
							     0.8),
						      (int) ((double)
							     height *
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
							     window_width
							     *
							     (double)
							     height /
							     (double)
							     width),
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
		spell = gtkspell_new_attach (GTK_TEXT_VIEW (textview),
					     NULL, NULL);
		gtkspell_set_language (spell, "he_IL", NULL);
	}
}
#endif

gboolean
on_window1_delete_event (GtkWidget * widget,
			 GdkEvent * event, gpointer user_data)
{
	set_rc_file ();
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
	return FALSE;
}

void
on_toolbutton_quit_clicked (GtkToolButton * toolbutton, gpointer user_data)
{
	set_rc_file ();
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
	gtk_font_selection_dialog_set_font_name
		(GTK_FONT_SELECTION_DIALOG (fsd), font_name);
	/* run dialog */
	result = gtk_dialog_run (GTK_DIALOG (fsd));
	switch (result)
	{
	case GTK_RESPONSE_OK:
		/* get the new font name */
		g_free (font_name);
		font_name =
			g_strdup
			(gtk_font_selection_dialog_get_font_name
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

void
set_rc_file ()
{
	GKeyFile *key_file;
	gchar *pathname = NULL;
	GError *error = NULL;
	gchar *content;

	/* do not use file if has command line args */
	if (usr_cmd_line_args)
		return;
	/* get menu items */
	/* color boxes ? */
	color_text_box_arg = gtk_check_menu_item_get_active
		(GTK_CHECK_MENU_ITEM (color_text_box));
	/* color misread fonts ? */
	color_misread_arg = gtk_check_menu_item_get_active
		(GTK_CHECK_MENU_ITEM (color_misread));
	/* do ocr ? */
	ocr_arg = gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM (ocr));
	/* use dict ? */
	use_dict_arg =
		gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM (use_dict));
	/* use nikud ? */
	use_nikud_arg =
		gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM
						(use_nikud));
	/* use spaces ? */
	use_spaces_arg =
		gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM
						(use_spaces));
	/* use indentation ? */
	use_indent_arg =
		gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM
						(use_indent));
	/* get path */
	pathname = g_strdup_printf ("%s%s%s", g_get_home_dir (),
				    G_DIR_SEPARATOR_S, ".hocr-gtk.rc");
	/* create new key file data */
	key_file = g_key_file_new ();
	/* color boxes ? */
	g_key_file_set_boolean (key_file, "hocr-gtk",
				"color_text_box", color_text_box_arg);
	/* color misread fonts ? */
	g_key_file_set_boolean (key_file, "hocr-gtk",
				"color_misread_arg", color_misread_arg);
	/* do ocr ? */
	g_key_file_set_boolean (key_file, "hocr-gtk", "ocr_arg", ocr_arg);
	/* use dict ? */
	g_key_file_set_boolean (key_file, "hocr-gtk",
				"use_dict_arg", use_dict_arg);
	/* use nikud ? */
	g_key_file_set_boolean (key_file, "hocr-gtk",
				"use_nikud_arg", use_nikud_arg);
	/* use spaces ? */
	g_key_file_set_boolean (key_file, "hocr-gtk",
				"use_spaces_arg", use_spaces_arg);
	/* use indentation ? */
	g_key_file_set_boolean (key_file, "hocr-gtk",
				"use_indent_arg", use_indent_arg);
	/* font name */
	g_key_file_set_string (key_file, "hocr-gtk", "font_name", font_name);
	/* save data */
	content = g_key_file_to_data (key_file, NULL, NULL);
	g_file_set_contents (pathname, content, -1, &error);
	g_key_file_free (key_file);
	g_free (content);
	g_free (pathname);
}

void
get_rc_file ()
{
	gchar *pathname = NULL;
	GError *error = NULL;
	PangoFontDescription *font_desc;

	/* do not use file if has command line args */
	if (usr_cmd_line_args)
		return;
	/* get path */
	pathname = g_strdup_printf ("%s%s%s", g_get_home_dir (),
				    G_DIR_SEPARATOR_S, ".hocr-gtk.rc");
	/* is file exist ? */
	if (g_file_test (pathname, G_FILE_TEST_EXISTS))
	{
		GKeyFile *key_file;

		/* create new key file data */
		key_file = g_key_file_new ();
		if (g_key_file_load_from_file (key_file, pathname, 0, &error))
		{
			/* color boxes ? */
			color_text_box_arg =
				g_key_file_get_boolean (key_file,
							"hocr-gtk",
							"color_text_box",
							&error);
			/* color misread fonts ? */
			color_misread_arg =
				g_key_file_get_boolean (key_file,
							"hocr-gtk",
							"color_misread_arg",
							&error);
			/* do ocr ? */
			ocr_arg =
				g_key_file_get_boolean (key_file, "hocr-gtk",
							"ocr_arg", &error);
			/* use dict ? */
			use_dict_arg =
				g_key_file_get_boolean (key_file,
							"hocr-gtk",
							"use_dict_arg", &error);
			/* use nikud ? */
			use_nikud_arg =
				g_key_file_get_boolean (key_file,
							"hocr-gtk",
							"use_nikud_arg",
							&error);
			/* use spaces ? */
			use_spaces_arg =
				g_key_file_get_boolean (key_file,
							"hocr-gtk",
							"use_spaces_arg",
							&error);
			/* use indentation ? */
			use_indent_arg =
				g_key_file_get_boolean (key_file,
							"hocr-gtk",
							"use_indent_arg",
							&error);
			/* font name */
			if (font_name)
				g_free (font_name);
			font_name =
				g_key_file_get_string (key_file, "hocr-gtk",
						       "font_name", &error);
		}

		g_key_file_free (key_file);
	}
	else
	{
		/* if no file create one */
		set_rc_file ();
	}

	/* set menu items */

	/* color boxes ? */
	gtk_check_menu_item_set_active
		(GTK_CHECK_MENU_ITEM (color_text_box), color_text_box_arg);
	/* color misread fonts ? */
	gtk_check_menu_item_set_active
		(GTK_CHECK_MENU_ITEM (color_misread), color_misread_arg);
	/* do ocr ? */
	gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (ocr), ocr_arg);
	/* use dict ? */
	gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (use_dict),
					use_dict_arg);
	/* use nikud ? */
	gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (use_nikud),
					use_nikud_arg);
	/* use spaces ? */
	gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM
					(use_spaces), use_spaces_arg);
	/* use indentation ? */
	gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM
					(use_indent), use_indent_arg);
	/* Change default font throughout the text widget */
	font_desc = pango_font_description_from_string (font_name);
	gtk_widget_modify_font (textview, font_desc);
	pango_font_description_free (font_desc);
	g_free (pathname);
	return;
}
