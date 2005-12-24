
/***************************************************************************
 *            interface.h
 *
 *  Fri Aug 12 20:16:24 2005
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (GETTEXT_PACKAGE, String)
#  define Q_(String) g_strip_context ((String), gettext (String))
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define Q_(String) g_strip_context ((String), (String))
#  define N_(String) (String)
#endif

#define TEXT_FONT_NAME "Sans 12"

/* main window */
GtkWidget *window1;
GtkWidget *pbar;
GtkWidget *hbox2;
GtkWidget *statusbar1;

/* menu check boxs */
GtkWidget *color_text_box;
GtkWidget *color_misread;
GtkWidget *clear_text;
GtkWidget *ocr;
GtkWidget *use_dict;
GtkWidget *use_nikud;
GtkWidget *use_spaces;
GtkWidget *use_indent;

/* image and text of main window */
GtkWidget *image;
GtkWidget *viewport1;
GtkWidget *scrolledwindow_text;
GtkWidget *textview;

/* user font */
char *font_name;

/* user pref */
gboolean color_text_box_arg;
gboolean color_misread_arg;
gboolean ocr_arg;
gboolean use_dict_arg;
gboolean use_nikud_arg;
gboolean use_indent_arg;
gboolean use_spaces_arg;

/* user interface for window */
GtkWidget *create_window1 (void);
