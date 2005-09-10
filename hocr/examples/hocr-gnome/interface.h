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

#define TEXT_FONT_NAME "Miriam Mono CLM 15"

/* main window */
GtkWidget *window1;

/* menu check boxs */
GtkWidget *columns;
/*GtkWidget *nikud;*/
GtkWidget *graphics;
GtkWidget *html;
GtkWidget *ocr;

/* image and text of main window */
GtkWidget *image;
GtkWidget *textview;

/* user interface for window */ 
GtkWidget *create_window1 (void);
