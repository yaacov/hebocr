/***************************************************************************
 *            main.c
 *
 *  Fri Aug 12 20:13:33 2005
 *  Copyright  2005-2007  Yaacov Zamir, Free Software Foundation
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

#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "hocr.h"

gchar *image_in_filename = NULL;
gchar *text_out_filename = NULL;
gchar *image_out_path = NULL;
gboolean version = FALSE;
gboolean debug = FALSE;

gint threshold = 0;
gint adaptive_threshold = 0;
gint adaptive_threshold_type = 0;
gint scale_by = 0;
gboolean no_auto_scale = FALSE;
gboolean remove_dots = FALSE;

gint paragraph_setup = 0;
gboolean show_grid = FALSE;

gboolean only_image_proccesing = FALSE;
gboolean only_layout_analysis = FALSE;



static gchar *copyright_message = "hocr - Hebrew OCR utility\n\
%s\n\
http://hocr.berlios.de\n\
Copyright (C) 2005-2007 Yaacov Zamir <kzamir@walla.co.il>\n\
\n\
This program is free software; you can redistribute it and/or modify\n\
it under the terms of the GNU General Public License as published by\n\
the Free Software Foundation; either version 2 of the License, or\n\
(at your option) any later version.\n\
\n\
This program is distributed in the hope that it will be useful,\n\
but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\
GNU General Public License for more details.\n\
\n\
You should have received a copy of the GNU General Public License\n\
along with this program; if not, write to the Free Software\n\
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.\n";

static GOptionEntry entries[] = {
  {"image-in", 'i', 0, G_OPTION_ARG_FILENAME, &image_in_filename,
   "use FILE as input image file name", "FILE"},
  {"text-out", 'o', 0, G_OPTION_ARG_FILENAME, &text_out_filename,
   "use FILE as output text file name", "FILE"},
  {"images-out-path", 'p', 0, G_OPTION_ARG_FILENAME, &image_out_path,
   "use PATH as output images directory path", "PATH"},
  {"thresholding-type", 'a', 0, G_OPTION_ARG_INT,
   &adaptive_threshold_type,
   "thresholding type, 0-normal, 1-none, 2-best",
   "NUM"},
  {"threshold", 't', 0, G_OPTION_ARG_INT, &threshold,
   "use NUM as threshold value, 1..100",
   "NUM"},
  {"adaptive-threshold", 'T', 0, G_OPTION_ARG_INT, &adaptive_threshold,
   "use NUM as adaptive threshold value, 1..100",
   "NUM"},
  {"scale", 's', 0, G_OPTION_ARG_INT, &scale_by,
   "scale input image by SCALE 1..9, 0-auto",
   "SCALE"},
  {"remove-halfton", 'r', 0, G_OPTION_ARG_NONE, &remove_dots,
   "remove small dots in input image",
   NULL},
  {"paragraph-setup", 'P', 0, G_OPTION_ARG_INT, &paragraph_setup,
   "paragraph setup: 0-colums 1-free",
   "NUM"},
  {"save-grid", 'g', 0, G_OPTION_ARG_NONE, &show_grid,
   "save grid on output images", NULL},
  {"image-proccesing", 'I', 0, G_OPTION_ARG_NONE, &only_image_proccesing,
   "run image proccesing and exit", NULL},
  {"layout-analysis", 'L', 0, G_OPTION_ARG_NONE, &only_layout_analysis,
   "run layout analysis and exit", NULL},
  {"debug", 'd', 0, G_OPTION_ARG_NONE, &debug,
   "output debuging information while running", NULL},
  {"version", 'v', 0, G_OPTION_ARG_NONE, &version,
   "output version information and exit", NULL},
  {NULL}
};

int
hocr_printerr (const char *msg)
{
  g_printerr ("%s: %s\n\
Try `--help' for more information.\n", g_get_prgname (), msg);
  return 0;
}

ho_pixbuf *
ho_pixbuf_new_from_file (const char *filename)
{
  ho_pixbuf *pix = NULL;
  GdkPixbuf *gdk_pix = NULL;

  /* if no file name or file name is one letter, e.g. '-' 
     use internal hocr loading function */
  if (!filename || !(filename[1]))
    return ho_pixbuf_new_from_pnm (image_in_filename);

  /* else, use gdk to load the file */

  /* load gdk pixbuf from file */
  gdk_pix = gdk_pixbuf_new_from_file (filename, NULL);

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

  memcpy (pix->data, gdk_pixbuf_get_pixels (gdk_pix),
	  (pix->height * pix->rowstride));

  /* free gdk pixbuf */
  g_object_unref (gdk_pix);

  return pix;
}

int
ho_pixbuf_save (const ho_pixbuf * pix, const char *filename)
{
  GError *error = NULL;
  GdkPixbuf *gdk_pix = NULL;
  gchar **ext_type = NULL;
  ho_pixbuf *pix_color = NULL;

  /* is this an RGB pixbuf ? */
  pix_color = ho_pixbuf_to_rgb (pix);
  if (!pix_color)
    return TRUE;

  /* load gdk pixbuf from ho_pixbuf */
  gdk_pix = gdk_pixbuf_new_from_data (pix_color->data,
				      GDK_COLORSPACE_RGB,
				      FALSE,
				      8,
				      pix_color->width,
				      pix_color->height,
				      pix_color->rowstride, NULL, NULL);

  if (!gdk_pix)
    {
      ho_pixbuf_free (pix_color);
      return TRUE;
    }

  /* save gdk pixbuf */
  ext_type = g_strsplit (filename, ".", 2);
  if (!g_ascii_strncasecmp (ext_type[1], "jpg", 4))
    gdk_pixbuf_save (gdk_pix, filename, "jpeg", &error, NULL);
  else
    gdk_pixbuf_save (gdk_pix, filename, ext_type[1], &error, NULL);

  /* free memory */
  ho_pixbuf_free (pix_color);
  g_strfreev (ext_type);
  g_object_unref (gdk_pix);

  if (error)
    {
      g_error_free (error);
      return TRUE;
    }

  return FALSE;
}

ho_bitmap *
hocr_pixbuf_to_bitmap (ho_pixbuf * pix, ho_uchar scale, ho_uchar adaptive,
		       ho_uchar threshold, ho_uchar a_threshold)
{
  ho_pixbuf *pix_temp = NULL;
  ho_bitmap *m_bw = NULL;
  ho_bitmap *m_bw_temp = NULL;

  if (!pix)
    {
      hocr_printerr ("can't read input image\n");
      exit (1);
    }

  /* if pix is color convert to gray scale */
  if (pix->n_channels != 1)
    {
      pix_temp = ho_pixbuf_color_to_gray (pix);
      ho_pixbuf_free (pix);
      pix = pix_temp;

      if (!pix)
	{
	  hocr_printerr ("bad color in input image\n");
	  exit (1);
	}
    }

  /* streach picture grays */
  pix_temp = ho_pixbuf_linear_filter (pix);
  ho_pixbuf_free (pix);
  pix = pix_temp;

  /* scale */
  if (scale)
    {
      pix_temp = ho_pixbuf_scale (pix, scale);
      if (pix_temp)
	{
	  ho_pixbuf_free (pix);
	  pix = pix_temp;
	}
    }

  /* convert to b/w bitmap */
  switch (adaptive)
    {
    case 0:
      m_bw = ho_pixbuf_to_bitmap_adaptive (pix, threshold, 0, a_threshold);
      break;
    case 1:
      m_bw = ho_pixbuf_to_bitmap (pix, threshold);
      break;
    case 2:
      m_bw =
	ho_pixbuf_to_bitmap_adaptive_best (pix, threshold, 0, a_threshold);
      break;
    default:
      hocr_printerr ("unknown adaptive threshold type, use normal\n");
      m_bw = ho_pixbuf_to_bitmap_adaptive (pix, threshold, 0, a_threshold);
      break;
    }

  if (!m_bw)
    {
      hocr_printerr ("can't convert to black and white\n");
      exit (1);
    }

  return m_bw;
}

int
main (int argc, char *argv[])
{
  GError *error = NULL;
  GOptionContext *context = NULL;
  gchar *text_out = NULL;
  ho_usint index;

  ho_pixbuf *pix = NULL;
  ho_pixbuf *pix_temp = NULL;

  ho_bitmap *m_bw = NULL;
  ho_bitmap *m_bw_temp = NULL;
  ho_bitmap *m_paragraphs = NULL;
  ho_bitmap *m_lines = NULL;

  ho_objmap *m_obj = NULL;

  ho_usint height;
  ho_usint width;
  ho_usint interline_height;
  ho_uchar nikud = FALSE;

  /* init gtk */
  gtk_init (&argc, &argv);

  /* start of argument analyzing section 
   */

  /* get user args */
  context = g_option_context_new ("- Hebrew OCR utility.");

  g_option_context_add_main_entries (context, entries, PACKAGE_NAME);
  g_option_context_parse (context, &argc, &argv, &error);

  /* free allocated memory */
  g_option_context_free (context);

  if (error)
    {
      hocr_printerr ("unrecognized option");
      exit (1);
    }

  if (version)
    {
      g_printf (copyright_message, BUILD);
      exit (0);
    }

  if (debug)
    g_print ("%s - Hebrew OCR utility\n", PACKAGE_STRING);

  /* end of argument analyzing section */

  /* start of image proccesing section 
   */
  if (debug)
    g_print ("start image proccesing.\n");

  /* read image and do color proccesing. if possible use the 
     gtk version (from main.c) that can load more image formats */
  pix = ho_pixbuf_new_from_file (image_in_filename);

  m_bw =
    hocr_pixbuf_to_bitmap (pix, scale_by, adaptive_threshold_type,
			   threshold, adaptive_threshold);

  /* do we need to auto scale ? */
  if (!no_auto_scale && !scale_by)
    {
      /* get fonts size for autoscale */
      ho_bitmap_filter_font_metrix (m_bw, 6, 200, 6, 200, &height, &width,
				    &nikud);


      /* if fonts are too small and user wants auto scale, re-scale image */
      if (height < 10)
	scale_by = 4;
      else if (height < 25)
	scale_by = 2;

      if (debug)
	if (scale_by)
	  g_print (" auto scale by %d.\n", scale_by);
	else
	  g_print (" no auto scale neaded.\n");

      if (scale_by)
	{
	  /* re-create bitmap */
	  ho_bitmap_free (m_bw);
	  m_bw =
	    hocr_pixbuf_to_bitmap (pix, scale_by,
				   adaptive_threshold_type, threshold,
				   adaptive_threshold);
	}
    }

  /* remove halfton dots  
     this also imply that input image is b/w */
  if (remove_dots)
    {
      if (debug)
	g_print (" remove halfton dots.\n");

      m_bw_temp = ho_bitmap_filter_remove_dots (m_bw, 4, 4);
      if (m_bw_temp)
	{
	  ho_bitmap_free (m_bw);
	  m_bw = m_bw_temp;
	  m_bw_temp = NULL;
	}
    }

  /* if debug mode or only image proccesing, save b/w image */
  if (only_image_proccesing || debug)
    {
      gchar *filename;

      pix_temp = ho_pixbuf_new_from_bitmap (m_bw);

      if (show_grid)
	ho_pixbuf_draw_grid (pix_temp, 120, 30, 0, 0, 0);

      /* create file name */
      filename = g_strdup_printf ("%s-I.jpeg", image_out_path);

      /* save to file system */
      if (filename)
	{
	  ho_pixbuf_save (pix_temp, filename);
	  ho_pixbuf_free (pix_temp);
	  g_free (filename);
	}
    }

  if (debug)
    g_print ("end image proccesing.\n");

  /* if user want save the b/w picture image proccesing produced */
  if (only_image_proccesing)
    {
      /* free images */
      ho_bitmap_free (m_bw);
      ho_pixbuf_free (pix);

      /* free file names */
      if (image_in_filename)
	g_free (image_in_filename);
      if (image_out_path)
	g_free (image_out_path);
      if (text_out_filename)
	g_free (text_out_filename);

      exit (0);
    }

  /* end of image proccesing section */

  /* start of layout analyzing section
   */
  if (debug)
    g_print ("start image layout analysis.\n");

  /* if we used auto scale we allready have font metrix */
  if (!no_auto_scale && scale_by)
    ho_bitmap_filter_font_metrix (m_bw, 6, 200, 6, 200, &height, &width,
				  &nikud);

  if (debug)
    if (nikud)
      g_print (" font height %d, width %d. with nikud.\n", height, width);
    else
      g_print (" font height %d, width %d. without nikud.\n", height, width);

  /* get interline height */
  ho_bitmap_filter_line_metrix (m_bw, height, width, nikud,
				&interline_height);

  if (debug)
    g_print (" inter-line space height %d\n", interline_height);

  /* get paragraphs */
  switch (paragraph_setup)
    {
    case 0:			/* colums */
      m_paragraphs =
	ho_bitmap_filter_paragraphs (m_bw, height, width, nikud,
				     interline_height, TRUE);
      break;
    case 1:			/* free setup */
      m_paragraphs =
	ho_bitmap_filter_paragraphs (m_bw, height, width, nikud,
				     interline_height, FALSE);
      break;
    default:			/* defaults to error */
      hocr_printerr ("unrecognized paragraph setup");
      exit (1);
      break;
    }

  /* sort paragraph by reading order */

  /* get lines in each paragraph */
  m_obj = ho_objmap_new_from_bitmap (m_paragraphs);
  for (index = 0; index < ho_objmap_get_size (m_obj); index++)
    {
      if (debug)
	g_print (" proccesing layout of paragraph %d\n", index + 1);

      /* get the paragraph bitmap */
      /* we need to be more cerful with free form paragraphs */
      switch (paragraph_setup)
	{
	case 0:		/* colums */

	  break;
	case 1:		/* free setup */

	  break;
	default:		/* defaults to error */
	  hocr_printerr ("unrecognized paragraph setup");
	  exit (1);
	  break;
	}

      /* get paragraph metrix */

    }

  /* free pragraph objects */
  ho_objmap_free (m_obj);

  if (debug)
    g_print ("end of image layout analysis.\n");

  /* if debug mode or only layout analysis, save layout image */
  if (only_layout_analysis || debug)
    {
      gchar *filename;

      /* create paragraph layout pixbuf */
      m_obj = ho_objmap_new_from_bitmap (m_paragraphs);
      pix_temp = ho_pixbuf_new_from_objmap (m_obj, 150, 255);

      /* add grid to layout pixbuf */
      if (show_grid)
	ho_pixbuf_draw_grid (pix_temp, 120, 30, 255, 0, 255);

      /* add text to layout pixbuf */
      m_bw_temp = ho_bitmap_clone (m_bw);
      ho_bitmap_and (m_bw_temp, m_paragraphs);
      ho_pixbuf_draw_bitmap (pix_temp, m_bw_temp, 20, 20, 20);
      ho_bitmap_free (m_bw_temp);

      /* create file name */
      filename = g_strdup_printf ("%s-L.jpeg", image_out_path);

      /* save to file system */
      if (filename)
	{
	  ho_pixbuf_save (pix_temp, filename);
	  ho_pixbuf_free (pix_temp);
	  g_free (filename);
	}
    }

  /* if user only want bw image exit now */
  if (only_layout_analysis)
    {
      /* free images */
      ho_bitmap_free (m_bw);
      ho_objmap_free (m_obj);
      ho_pixbuf_free (pix);

      /* free file names */
      if (image_in_filename)
	g_free (image_in_filename);
      if (image_out_path)
	g_free (image_out_path);
      if (text_out_filename)
	g_free (text_out_filename);

      exit (0);
    }

  /* start text recognition section 
   */
  if (debug)
    g_print ("start text recognition.\n");

  /* for each paragraph get lines */

  /* for each line get words */

  /* for each word get letters probability */

  /* use letters probability and linguistic knowlage to get words */

  if (debug)
    g_print ("end text recognition.\n");

  /* end text recognition section  */

  /* start user output section 
   */

  /* save text */
  if (text_out_filename)
    {
      g_file_set_contents (text_out_filename, text_out, -1, &error);

      if (error)
	{
	  hocr_printerr ("can't write text to file");
	}
    }

  /* end of user putput section */

  /* start of memory cleanup section 
   */

  if (debug)
    g_print ("free memory.\n");

  /* free matrixes */
  ho_bitmap_free (m_bw);
  ho_bitmap_free (m_paragraphs);

  /* free file names */
  if (image_in_filename)
    g_free (image_in_filename);
  if (image_out_path)
    g_free (image_out_path);
  if (text_out_filename)
    g_free (text_out_filename);

  return 0;
}
