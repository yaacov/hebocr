
/***************************************************************************
 *            callbacks.h
 *
 *  Fri Aug 12 20:11:47 2005
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

#include <gtk/gtk.h>

#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

gboolean
on_window1_delete_event (GtkWidget * widget,
			 GdkEvent * event, gpointer user_data);

/* toolbar */
void
on_toolbutton_open_clicked (GtkToolButton * toolbutton, gpointer user_data);

void
on_toolbutton_apply_clicked (GtkToolButton * toolbutton, gpointer user_data);

void
on_toolbutton_save_clicked (GtkToolButton * toolbutton, gpointer user_data);

void
on_toolbutton_zoom_in_clicked (GtkToolButton * toolbutton, gpointer user_data);

void
on_toolbutton_zoom_out_clicked (GtkToolButton * toolbutton, gpointer user_data);

void
on_toolbutton_zoom_fit_clicked (GtkToolButton * toolbutton, gpointer user_data);

void
on_toolbutton_about_clicked (GtkToolButton * toolbutton, gpointer user_data);

#ifdef WITH_GTKSPELL
void
on_toolbutton_spell_clicked (GtkToolButton * toolbutton, gpointer user_data);
#endif

void
on_toolbutton_quit_clicked (GtkToolButton * toolbutton, gpointer user_data);

/* menu */
void on_open_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_apply_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_save_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_save_picture_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_xsane_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_quit_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_font_activate (GtkMenuItem * menuitem, gpointer user_data);

#ifdef WITH_GTKSPELL
void on_spell_check_activate (GtkMenuItem * menuitem, gpointer user_data);
#endif

void on_zoom_in_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_zoom_out_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_normal_size_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_about_activate (GtkMenuItem * menuitem, gpointer user_data);

void get_rc_file ();

void set_rc_file ();

#endif /* __CALLBACKS_H__ */
