/***************************************************************************
 *            ho_gtk.c
 *
 *  Fri Aug 12 20:13:33 2005
 *  Copyright  2005-2007  Yaacov Zamir
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
 */

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "ho_gtk.h"

ho_pixbuf *
ho_gtk_pixbuf_from_gdk (const GdkPixbuf * gdk_pix)
{
  ho_pixbuf *pix = NULL;

  if (!gdk_pix)
    {
      return NULL;
    }

  /* create hocr pixbuf from gkd pixbuf */
  pix = ho_pixbuf_new (gdk_pixbuf_get_n_channels
		       (gdk_pix), gdk_pixbuf_get_width
		       (gdk_pix), gdk_pixbuf_get_height (gdk_pix),
		       gdk_pixbuf_get_rowstride (gdk_pix));

  if (!pix)
    return NULL;

  /* copy the pixels */
  memcpy (pix->data, gdk_pixbuf_get_pixels (gdk_pix),
	  (pix->height * pix->rowstride));

  return pix;
}

void
ho_gdk_free_pixels (guchar * pixels, gpointer data)
{
  free (pixels);
}

GdkPixbuf *
ho_gtk_pixbuf_to_gdk (const ho_pixbuf * hocr_pix)
{
  GdkPixbuf *pix = NULL;
  ho_pixbuf *pix_color = NULL;
  ho_uchar *data;

  /* gdk has only RGB pixbufs */
  pix_color = ho_pixbuf_to_rgb (hocr_pix);
  if (!pix_color)
    return NULL;

  /* create data */
  data = malloc (pix_color->height * pix_color->rowstride);
  if (!data)
    {
      ho_pixbuf_free (pix_color);
      return NULL;
    }
  memcpy (data, pix_color->data, pix_color->height * pix_color->rowstride);

  /* load gdk pixbuf from ho_pixbuf */
  pix = gdk_pixbuf_new_from_data (data,
				  GDK_COLORSPACE_RGB,
				  FALSE,
				  8,
				  ho_pixbuf_get_width (pix_color),
				  ho_pixbuf_get_height (pix_color),
				  ho_pixbuf_get_rowstride (pix_color),
				  ho_gdk_free_pixels, NULL);

  ho_pixbuf_free (pix_color);

  return pix;
}

ho_pixbuf *
ho_gtk_pixbuf_new_from_file (const char *filename)
{
  ho_pixbuf *pix = NULL;
  GdkPixbuf *gdk_pix = NULL;

  /* load gdk pixbuf from file */
  gdk_pix = gdk_pixbuf_new_from_file (filename, NULL);

  if (!gdk_pix)
    {
      return NULL;
    }

  /* create hocr pixbuf from gkd pixbuf */
  pix = ho_gtk_pixbuf_from_gdk (gdk_pix);

  /* free gdk pixbuf */
  g_object_unref (gdk_pix);

  return pix;
}

int
ho_gtk_pixbuf_save (const ho_pixbuf * pix, const char *filename)
{
  GError *error = NULL;
  GdkPixbuf *gdk_pix = NULL;
  gchar **ext_type = NULL;

  /* load gdk pixbuf from ho_pixbuf */
  gdk_pix = ho_gtk_pixbuf_to_gdk (pix);
  if (!gdk_pix)
    return TRUE;

  /* save gdk pixbuf */
  ext_type = g_strsplit (filename, ".", 2);
  if (!g_ascii_strncasecmp (ext_type[1], "jpg", 4))
    gdk_pixbuf_save (gdk_pix, filename, "jpeg", &error, NULL);
  else
    gdk_pixbuf_save (gdk_pix, filename, ext_type[1], &error, NULL);
  
  /* free memory */
  g_strfreev (ext_type);
  g_object_unref (gdk_pix);
  
  if (error)
    {
      g_error_free (error);
      return TRUE;
    }

  return FALSE;
}
