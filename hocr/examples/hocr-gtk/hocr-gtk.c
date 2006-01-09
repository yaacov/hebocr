
/***************************************************************************
 *            hocr-gui.c
 *
 *  Fri Aug 12 20:31:48 2005
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

#include <unistd.h>

#include <glib.h>
#include <gtk/gtk.h>

#include "interface.h"
#include "callbacks.h"
int
print_help ()
{
	printf ("hocr-gtk %s - Hebrew OCR program\n", VERSION);
	printf ("http://hocr.berlios.de\n");
	printf ("USAGE: hocr-gtk [OPTION]... [-o][-i file]\n\n");

	printf ("Help options\n\
  -h                                     Show this help message\n");
	printf ("Application options\n\
  -c,                                    Do OCR on picture\n\
  -c,                                    Color text boxes\n\
  -d,                                    Use internal dictionary to guess\n\
                                         misread fonts.\n\
  -n,                                    Try to guess nikud for fonts.\n\
  -s,                                    Use spaces for tabs.\n\
  -t,                                    Indent indented lines.\n");

	printf ("GTK+\n\
  --gdk-debug=FLAGS                      Gdk debugging flags to set\n\
  --gdk-no-debug=FLAGS                   Gdk debugging flags to unset\n\
  --display=DISPLAY                      X display to use\n\
  --screen=SCREEN                        X screen to use\n\
  --sync                                 Make X calls synchronous\n\
  --name=NAME                            Program name as used by the window\n\
                                         manager\n\
  --class=CLASS                          Program class as used by the window\n\
                                         manager\n\
  --gtk-debug=FLAGS                      Gtk+ debugging flags to set\n\
  --gtk-no-debug=FLAGS                   Gtk+ debugging flags to unset\n\
  --g-fatal-warnings                     Make all warnings fatal\n\
  --gtk-module=MODULE                    Load an additional Gtk module\n\
\n");

	return 0;
}

int
main (int argc, char *argv[])
{
	GtkWidget *window1;
	char c;

#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	gtk_init (&argc, &argv);

	/* init thread system if not init already */
	if (!g_thread_supported ())
		g_thread_init (NULL);

	/* create main window */
	window1 = create_window1 ();

	/* no pixbuf at start */
	pixbuf = NULL;
	vis_pixbuf = NULL;

	/* get command line arguments */
	usr_cmd_line_args = FALSE;
	color_text_box_arg = FALSE;
	color_misread_arg = FALSE;
	ocr_arg = FALSE;
	use_dict_arg = FALSE;
	use_nikud_arg = FALSE;
	gboolean use_indent_arg = FALSE;

	use_spaces_arg = FALSE;

	while ((c = getopt (argc, argv, "ocdnsthi:")) != EOF)
	{
		switch (c)
		{
		case 'i':
			if (optarg)
			{
				pixbuf = gdk_pixbuf_new_from_file (optarg,
								   NULL);

				/* check if realy a picture file */
				if (pixbuf)
				{
					char title[255];

					/* set the window title */
					g_snprintf (title, 255, "%s - %s",
						    _("hocr-gtk"), optarg);
					gtk_window_set_title (GTK_WINDOW
							      (window1), title);

					on_toolbutton_zoom_fit_clicked (NULL,
									NULL);
				}
				else
				{
					printf ("%s: %s %s\n", _("hocr-gtk"),
						_("can't open picture file -"),
						optarg);
				}
			}
			break;
		case 'c':
			color_text_box_arg = TRUE;
			color_misread_arg = TRUE;
			usr_cmd_line_args = TRUE;
			break;
		case 'o':
			ocr_arg = TRUE;
			usr_cmd_line_args = TRUE;
			break;
		case 'd':
			use_dict_arg = TRUE;
			usr_cmd_line_args = TRUE;
			break;
		case 'n':
			use_nikud_arg = TRUE;
			usr_cmd_line_args = TRUE;
			break;
		case 's':
			use_spaces_arg = TRUE;
			usr_cmd_line_args = TRUE;
			break;
		case 't':
			use_indent_arg = TRUE;
			usr_cmd_line_args = TRUE;
			break;
		case 'h':
			print_help ();

			return 0;
			break;
		default:
			print_help ();

			return 0;
			break;
		}
	}

	/* if got cmd line args set menu items */
	if (usr_cmd_line_args)
	{
		/* color boxes ? */
		gtk_check_menu_item_set_active
			(GTK_CHECK_MENU_ITEM (color_text_box),
			 color_text_box_arg);

		/* color misread fonts ? */
		gtk_check_menu_item_set_active
			(GTK_CHECK_MENU_ITEM (color_misread),
			 color_misread_arg);

		/* do ocr ? */
		gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (ocr),
						ocr_arg);

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
	}

	/* readf rc file */
	get_rc_file ();

	/* run program */
	gtk_widget_show (window1);

	gtk_main ();
	return 0;
}
