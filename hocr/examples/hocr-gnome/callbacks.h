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
  
#include <gnome.h>

gboolean
on_window1_delete_event (GtkWidget * widget,
			 GdkEvent * event, gpointer user_data);

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

void
on_toolbutton_spell_clicked (GtkToolButton * toolbutton, gpointer user_data);

void
on_toolbutton_quit_clicked (GtkToolButton * toolbutton, gpointer user_data);
