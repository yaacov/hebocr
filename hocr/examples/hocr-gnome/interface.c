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

	/* menu */
	GtkWidget *menubar1;
	GtkWidget *menuitem1;
	GtkWidget *menuitem1_menu;
	GtkWidget *open;
	GtkWidget *apply;
	GtkWidget *save;
	GtkWidget *separatormenuitem1;
	GtkWidget *quit;
	GtkWidget *menuitem2;
	GtkWidget *menuitem2_menu;
	GtkWidget *font;
	GtkWidget *separator2;
	GtkWidget *spell_check;
	GtkWidget *separator3;
	GtkWidget *menuitem3;
	GtkWidget *menuitem3_menu;
	GtkWidget *zoom_in;
	GtkWidget *zoom_out;
	GtkWidget *normal_size;
	GtkWidget *menuitem4;
	GtkWidget *menuitem4_menu;
	GtkWidget *about;
	GtkAccelGroup *accel_group;

	/* toolbar */
	GtkWidget *toolbar;
	GtkIconSize tmp_toolbar_icon_size;
	GtkWidget *toolbutton_open;
	GtkWidget *toolbutton_apply;
	GtkWidget *toolbutton_save;
	GtkWidget *toolitem1;
	GtkWidget *vseparator1;
	GtkWidget *toolbutton_spell;
	GtkWidget *toolitem2;
	GtkWidget *vseparator2;
	GtkWidget *toolbutton_zoom_in;
	GtkWidget *toolbutton_zoom_out;
	GtkWidget *toolbutton_zoom_fit;
	GtkWidget *toolitem3;
	GtkWidget *vseparator3;
	GtkWidget *toolbutton_about;
	GtkWidget *toolitem4;
	GtkWidget *vseparator4;
	GtkWidget *toolbutton_quit;

	/* image */
	GtkWidget *vpaned1;
	GtkWidget *scrolledwindow_image;

	/* text */
	GtkWidget *viewport1;
	GtkWidget *scrolledwindow_text;
	GtkTooltips *tooltips;

	PangoFontDescription *font_desc;

	tooltips = gtk_tooltips_new ();
	accel_group = gtk_accel_group_new ();

	window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request (window1, 800, 600);
	gtk_window_set_title (GTK_WINDOW (window1), _("hocr-gui"));

	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (window1), vbox1);

	/* menu */

	menubar1 = gtk_menu_bar_new ();
	gtk_widget_show (menubar1);
	gtk_box_pack_start (GTK_BOX (vbox1), menubar1, FALSE, FALSE, 0);

	menuitem1 = gtk_menu_item_new_with_mnemonic (_("_File"));
	gtk_widget_show (menuitem1);
	gtk_container_add (GTK_CONTAINER (menubar1), menuitem1);

	menuitem1_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem1), menuitem1_menu);

	open = gtk_image_menu_item_new_from_stock ("gtk-open", accel_group);
	gtk_widget_show (open);
	gtk_container_add (GTK_CONTAINER (menuitem1_menu), open);

	apply = gtk_image_menu_item_new_from_stock ("gtk-apply", accel_group);
	gtk_widget_show (apply);
	gtk_container_add (GTK_CONTAINER (menuitem1_menu), apply);

	save = gtk_image_menu_item_new_from_stock ("gtk-save", accel_group);
	gtk_widget_show (save);
	gtk_container_add (GTK_CONTAINER (menuitem1_menu), save);

	separatormenuitem1 = gtk_separator_menu_item_new ();
	gtk_widget_show (separatormenuitem1);
	gtk_container_add (GTK_CONTAINER (menuitem1_menu),
			   separatormenuitem1);
	gtk_widget_set_sensitive (separatormenuitem1, FALSE);

	quit = gtk_image_menu_item_new_from_stock ("gtk-quit", accel_group);
	gtk_widget_show (quit);
	gtk_container_add (GTK_CONTAINER (menuitem1_menu), quit);

	menuitem2 = gtk_menu_item_new_with_mnemonic (_("_Edit"));
	gtk_widget_show (menuitem2);
	gtk_container_add (GTK_CONTAINER (menubar1), menuitem2);

	menuitem2_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem2), menuitem2_menu);

	font = gtk_image_menu_item_new_from_stock ("gtk-select-font",
						   accel_group);
	gtk_widget_show (font);
	gtk_container_add (GTK_CONTAINER (menuitem2_menu), font);

	separator2 = gtk_separator_menu_item_new ();
	gtk_widget_show (separator2);
	gtk_container_add (GTK_CONTAINER (menuitem2_menu), separator2);
	gtk_widget_set_sensitive (separator2, FALSE);

	spell_check =
		gtk_image_menu_item_new_from_stock ("gtk-spell-check",
						    accel_group);
	gtk_widget_show (spell_check);
	gtk_container_add (GTK_CONTAINER (menuitem2_menu), spell_check);

	separator3 = gtk_separator_menu_item_new ();
	gtk_widget_show (separator3);
	gtk_container_add (GTK_CONTAINER (menuitem2_menu), separator3);
	gtk_widget_set_sensitive (separator3, FALSE);

	columns = gtk_check_menu_item_new_with_mnemonic (_("Columns"));
	gtk_widget_show (columns);
	gtk_container_add (GTK_CONTAINER (menuitem2_menu), columns);
	gtk_tooltips_set_tip (tooltips, columns,
			      _("Check for columns in scaned picture"), NULL);
	gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (columns), TRUE);

	nikud = gtk_check_menu_item_new_with_mnemonic (_("Nikud"));
	gtk_widget_show (nikud);
	gtk_container_add (GTK_CONTAINER (menuitem2_menu), nikud);
	gtk_tooltips_set_tip (tooltips, nikud,
			      _("Check for nikud in scaned picture"), NULL);
	gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (nikud), TRUE);

	graphics = gtk_check_menu_item_new_with_mnemonic (_("Graphics"));
	gtk_widget_show (graphics);
	gtk_container_add (GTK_CONTAINER (menuitem2_menu), graphics);
	gtk_tooltips_set_tip (tooltips, graphics, _("Show graphical output"),
			      NULL);
	gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (graphics), TRUE);

	ocr = gtk_check_menu_item_new_with_mnemonic (_("OCR"));
	gtk_widget_show (ocr);
	gtk_container_add (GTK_CONTAINER (menuitem2_menu), ocr);
	gtk_tooltips_set_tip (tooltips, ocr,
			      _("Try to recognize fonts in scaned text"),
			      NULL);
	gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (ocr), TRUE);

	menuitem3 = gtk_menu_item_new_with_mnemonic (_("_View"));
	gtk_widget_show (menuitem3);
	gtk_container_add (GTK_CONTAINER (menubar1), menuitem3);

	menuitem3_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem3), menuitem3_menu);

	zoom_in =
		gtk_image_menu_item_new_from_stock ("gtk-zoom-in",
						    accel_group);
	gtk_widget_show (zoom_in);
	gtk_container_add (GTK_CONTAINER (menuitem3_menu), zoom_in);

	zoom_out =
		gtk_image_menu_item_new_from_stock ("gtk-zoom-out",
						    accel_group);
	gtk_widget_show (zoom_out);
	gtk_container_add (GTK_CONTAINER (menuitem3_menu), zoom_out);

	normal_size =
		gtk_image_menu_item_new_from_stock ("gtk-zoom-100",
						    accel_group);
	gtk_widget_show (normal_size);
	gtk_container_add (GTK_CONTAINER (menuitem3_menu), normal_size);

	menuitem4 = gtk_menu_item_new_with_mnemonic (_("_Help"));
	gtk_widget_show (menuitem4);
	gtk_container_add (GTK_CONTAINER (menubar1), menuitem4);

	menuitem4_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem4), menuitem4_menu);

	about = gtk_menu_item_new_with_mnemonic (_("_About"));
	gtk_widget_show (about);
	gtk_container_add (GTK_CONTAINER (menuitem4_menu), about);
	gtk_widget_add_accelerator (about, "activate", accel_group,
				    GDK_B, (GdkModifierType) GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	/* toolbar */
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

	toolitem2 = (GtkWidget *) gtk_tool_item_new ();
	gtk_widget_show (toolitem2);
	gtk_container_add (GTK_CONTAINER (toolbar), toolitem2);
	vseparator2 = gtk_vseparator_new ();
	gtk_widget_show (vseparator2);
	gtk_container_add (GTK_CONTAINER (toolitem2), vseparator2);

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

	/* image */
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

	/* text */
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

	/* main window */
	g_signal_connect ((gpointer) window1, "delete_event",
			  G_CALLBACK (on_window1_delete_event), NULL);

	/* toolbar */
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

	/* menu */
	g_signal_connect ((gpointer) open, "activate",
			  G_CALLBACK (on_open_activate), NULL);
	g_signal_connect ((gpointer) apply, "activate",
			  G_CALLBACK (on_apply_activate), NULL);
	g_signal_connect ((gpointer) save, "activate",
			  G_CALLBACK (on_save_activate), NULL);
	g_signal_connect ((gpointer) quit, "activate",
			  G_CALLBACK (on_quit_activate), NULL);
	g_signal_connect ((gpointer) font, "activate",
			  G_CALLBACK (on_font_activate), NULL);
	g_signal_connect ((gpointer) spell_check, "activate",
			  G_CALLBACK (on_spell_check_activate), NULL);
	g_signal_connect ((gpointer) columns, "activate",
			  G_CALLBACK (on_columns_activate), NULL);
	g_signal_connect ((gpointer) nikud, "activate",
			  G_CALLBACK (on_nikud_activate), NULL);
	g_signal_connect ((gpointer) graphics, "activate",
			  G_CALLBACK (on_graphics_activate), NULL);
	g_signal_connect ((gpointer) ocr, "activate",
			  G_CALLBACK (on_ocr_activate), NULL);
	g_signal_connect ((gpointer) zoom_in, "activate",
			  G_CALLBACK (on_zoom_in_activate), NULL);
	g_signal_connect ((gpointer) zoom_out, "activate",
			  G_CALLBACK (on_zoom_out_activate), NULL);
	g_signal_connect ((gpointer) normal_size, "activate",
			  G_CALLBACK (on_normal_size_activate), NULL);
	g_signal_connect ((gpointer) about, "activate",
			  G_CALLBACK (on_about_activate), NULL);

	gtk_window_add_accel_group (GTK_WINDOW (window1), accel_group);

	return window1;
}
