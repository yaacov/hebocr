/***************************************************************************
 *            interface.c
 *
 *  Fri Aug 12 20:15:06 2005
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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <bonobo.h>
#include <gnome.h>

#include "callbacks.h"
#include "interface.h"

#define TEXT_FONT_NAME "Miriam Mono CLM 15"

GtkWidget *
create_window1 (void)
{
	GtkWidget *vbox1;
	GtkWidget *toolbar;
	GtkIconSize tmp_toolbar_icon_size;
	GtkWidget *toolbutton_open;
	GtkWidget *toolbutton_apply;
	GtkWidget *toolbutton_save;
	GtkWidget *toolitem1;
	GtkWidget *vseparator1;
	GtkWidget *toolbutton_spell;
	GtkWidget *toolbutton_zoom_in;
	GtkWidget *toolbutton_zoom_out;
	GtkWidget *toolbutton_zoom_fit;
	GtkWidget *toolitem3;
	GtkWidget *vseparator3;
	GtkWidget *toolbutton_about;
	GtkWidget *toolitem4;
	GtkWidget *vseparator4;
	GtkWidget *toolbutton_quit;
	GtkWidget *vpaned1;
	GtkWidget *scrolledwindow_image;
	GtkWidget *viewport1;
	GtkWidget *scrolledwindow_text;
	GtkTooltips *tooltips;

	PangoFontDescription *font_desc;

	tooltips = gtk_tooltips_new ();

	window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request (window1, 800, 600);
	gtk_window_set_title (GTK_WINDOW (window1), _("hocr-gui"));

	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (window1), vbox1);

	toolbar = gtk_toolbar_new ();
	gtk_widget_show (toolbar);
	gtk_box_pack_start (GTK_BOX (vbox1), toolbar, FALSE, FALSE, 0);
	gtk_toolbar_set_style (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_BOTH);
	tmp_toolbar_icon_size =
		gtk_toolbar_get_icon_size (GTK_TOOLBAR (toolbar));

	toolbutton_open =
		(GtkWidget *) gtk_tool_button_new_from_stock ("gtk-open");
	gtk_widget_show (toolbutton_open);
	gtk_container_add (GTK_CONTAINER (toolbar), toolbutton_open);
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (toolbutton_open), tooltips,
				   _("Open a new picture for the OCR"), NULL);

	toolbutton_apply =
		(GtkWidget *) gtk_tool_button_new_from_stock ("gtk-apply");
	gtk_widget_show (toolbutton_apply);
	gtk_container_add (GTK_CONTAINER (toolbar), toolbutton_apply);
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (toolbutton_apply), tooltips,
				   _("Convert picture to text"), NULL);

	toolbutton_save =
		(GtkWidget *) gtk_tool_button_new_from_stock ("gtk-save");
	gtk_widget_show (toolbutton_save);
	gtk_container_add (GTK_CONTAINER (toolbar), toolbutton_save);
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (toolbutton_save), tooltips,
				   _("Save the text created by the OCR"),
				   NULL);

	toolitem1 = (GtkWidget *) gtk_tool_item_new ();
	gtk_widget_show (toolitem1);
	gtk_container_add (GTK_CONTAINER (toolbar), toolitem1);
	vseparator1 = gtk_vseparator_new ();
	gtk_widget_show (vseparator1);
	gtk_container_add (GTK_CONTAINER (toolitem1), vseparator1);

	toolbutton_spell =
		(GtkWidget *)
		gtk_tool_button_new_from_stock ("gtk-spell-check");
	gtk_widget_show (toolbutton_spell);
	gtk_container_add (GTK_CONTAINER (toolbar), toolbutton_spell);
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (toolbutton_spell), tooltips,
				   _("Spell check the text"), NULL);

	toolbutton_zoom_in =
		(GtkWidget *) gtk_tool_button_new_from_stock ("gtk-zoom-in");
	gtk_widget_show (toolbutton_zoom_in);
	gtk_container_add (GTK_CONTAINER (toolbar), toolbutton_zoom_in);

	toolbutton_zoom_out =
		(GtkWidget *) gtk_tool_button_new_from_stock ("gtk-zoom-out");
	gtk_widget_show (toolbutton_zoom_out);
	gtk_container_add (GTK_CONTAINER (toolbar), toolbutton_zoom_out);

	toolbutton_zoom_fit =
		(GtkWidget *) gtk_tool_button_new_from_stock ("gtk-zoom-fit");
	gtk_widget_show (toolbutton_zoom_fit);
	gtk_container_add (GTK_CONTAINER (toolbar), toolbutton_zoom_fit);

	toolitem3 = (GtkWidget *) gtk_tool_item_new ();
	gtk_widget_show (toolitem3);
	gtk_container_add (GTK_CONTAINER (toolbar), toolitem3);
	vseparator3 = gtk_vseparator_new ();
	gtk_widget_show (vseparator3);
	gtk_container_add (GTK_CONTAINER (toolitem3), vseparator3);

	toolbutton_about =
		(GtkWidget *) gtk_tool_button_new_from_stock ("gtk-about");
	gtk_widget_show (toolbutton_about);
	gtk_container_add (GTK_CONTAINER (toolbar), toolbutton_about);
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (toolbutton_about), tooltips,
				   _("About this application"), NULL);

	toolitem4 = (GtkWidget *) gtk_tool_item_new ();
	gtk_widget_show (toolitem4);
	gtk_container_add (GTK_CONTAINER (toolbar), toolitem4);
	vseparator4 = gtk_vseparator_new ();
	gtk_widget_show (vseparator4);
	gtk_container_add (GTK_CONTAINER (toolitem4), vseparator4);

	toolbutton_quit =
		(GtkWidget *) gtk_tool_button_new_from_stock ("gtk-quit");
	gtk_widget_show (toolbutton_quit);
	gtk_container_add (GTK_CONTAINER (toolbar), toolbutton_quit);
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (toolbutton_quit), tooltips,
				   _("Quit this program"), NULL);

	vpaned1 = gtk_vpaned_new ();
	gtk_widget_show (vpaned1);
	gtk_container_add (GTK_CONTAINER (vbox1), vpaned1);
											 
	scrolledwindow_image = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy ((GtkScrolledWindow *)
					scrolledwindow_image,
					GTK_POLICY_AUTOMATIC,
					GTK_POLICY_AUTOMATIC);
	gtk_widget_show (scrolledwindow_image);
	gtk_container_add (GTK_CONTAINER (vpaned1), scrolledwindow_image);

	viewport1 = gtk_viewport_new (NULL, NULL);
	gtk_widget_show (viewport1);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_image), viewport1);

	image = gtk_image_new ();
	gtk_widget_show (image);
	gtk_container_add (GTK_CONTAINER (viewport1), image);

	scrolledwindow_text = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy ((GtkScrolledWindow *)
					scrolledwindow_text,
					GTK_POLICY_AUTOMATIC,
					GTK_POLICY_AUTOMATIC);
	gtk_widget_show (scrolledwindow_text);
	gtk_container_add (GTK_CONTAINER (vpaned1), scrolledwindow_text);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW
					     (scrolledwindow_text),
					     GTK_SHADOW_IN);

	textview = gtk_text_view_new ();
	gtk_widget_show (textview);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_text), textview);

	/* Change default font throughout the widget */
	font_desc = pango_font_description_from_string (TEXT_FONT_NAME);
	gtk_widget_modify_font (textview, font_desc);
	pango_font_description_free (font_desc);

	g_signal_connect ((gpointer) window1, "delete_event",
			  G_CALLBACK (on_window1_delete_event), NULL);

	g_signal_connect ((gpointer) toolbutton_open, "clicked",
			  G_CALLBACK (on_toolbutton_open_clicked), NULL);
	g_signal_connect ((gpointer) toolbutton_apply, "clicked",
			  G_CALLBACK (on_toolbutton_apply_clicked), NULL);
	g_signal_connect ((gpointer) toolbutton_save, "clicked",
			  G_CALLBACK (on_toolbutton_save_clicked), NULL);
			  
	g_signal_connect ((gpointer) toolbutton_zoom_in, "clicked",
			  G_CALLBACK (on_toolbutton_zoom_in_clicked), NULL);
	g_signal_connect ((gpointer) toolbutton_zoom_out, "clicked",
			  G_CALLBACK (on_toolbutton_zoom_out_clicked), NULL);
	g_signal_connect ((gpointer) toolbutton_zoom_fit, "clicked",
			  G_CALLBACK (on_toolbutton_zoom_fit_clicked), NULL);
			  
	g_signal_connect ((gpointer) toolbutton_about, "clicked",
			  G_CALLBACK (on_toolbutton_about_clicked), NULL);
	g_signal_connect ((gpointer) toolbutton_spell, "clicked",
			  G_CALLBACK (on_toolbutton_spell_clicked), NULL);

	g_signal_connect ((gpointer) toolbutton_quit, "clicked",
			  G_CALLBACK (on_toolbutton_quit_clicked), NULL);

	return window1;
}
