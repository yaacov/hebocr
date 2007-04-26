/***************************************************************************
 *            hocr.c
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "hocr.h"
#include "ho_gtk.h"

gchar *image_in_filename = NULL;
gchar *text_out_filename = NULL;
gchar *data_out_filename = NULL;
gchar *image_out_path = NULL;
gboolean version = FALSE;
gboolean debug = FALSE;
gboolean verbose = FALSE;
gboolean no_gtk = FALSE;
gboolean dir_ltr = FALSE;

gint threshold = 0;
gint adaptive_threshold = 0;
gint adaptive_threshold_type = 0;
gint scale_by = 0;
gboolean do_not_clean_image = FALSE;
gboolean remove_dots = FALSE;
gboolean remove_images = FALSE;
gboolean fix_broken_fonts = FALSE;
gboolean fix_ligated_fonts = FALSE;

gint paragraph_setup = 0;
gint slicing_threshold = 0;
gint slicing_width = 0;

gboolean show_grid = FALSE;
gboolean save_bw = FALSE;
gboolean save_layout = FALSE;
gboolean save_fonts = FALSE;
gboolean debug_font_filter_list = FALSE;
gint debug_font_filter = 0;

gboolean only_image_proccesing = FALSE;
gboolean only_layout_analysis = FALSE;
gboolean only_save_fonts = FALSE;

gint spacing_code = 0;
gint lang_code = 0;
gint font_code = 0;

GError *error = NULL;

/* black and white text image */
ho_bitmap *m_page_text = NULL;

/* text layout */
ho_layout *l_page = NULL;

/* text for output */
gchar *text_out = NULL;

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

static gchar *font_filters_list = "hocr - Hebrew OCR utility\n\
%s\n\
Font filters list\n\
 1. main font.\n\
 2. second part.\n\
 3. holes in font.\n\
 4. horizontal bars in font.\n\
 5. vertical bars in font.\n\
 6. 45 deg. diagonals in font.\n\
 7. 135 deg. diagonals in font.\n\
 8. thined font.\n\
 9. top font edges.\n\
 10. bottom font edges.\n\
 11. left font edges.\n\
 12. right font edges.\n\
 13. top font notches.\n\
 14. bottom font notches.\n\
 15. left font notches.\n\
 16. right font notches.\n\
";

static GOptionEntry file_entries[] = {
  {"images-out-path", 'O', 0, G_OPTION_ARG_FILENAME, &image_out_path,
   "use PATH for output images", "PATH"},
  {"data-out", 'u', 0, G_OPTION_ARG_FILENAME, &data_out_filename,
   "use FILE as output data file name", "FILE"},
  {NULL}
};

static GOptionEntry image_entries[] = {
  {"thresholding-type", 'T', 0, G_OPTION_ARG_INT,
   &adaptive_threshold_type,
   "thresholding type, 0 normal, 1 none, 2 fine",
   "NUM"},
  {"threshold", 't', 0, G_OPTION_ARG_INT, &threshold,
   "use NUM as threshold value, 1..100",
   "NUM"},
  {"adaptive-threshold", 'a', 0, G_OPTION_ARG_INT, &adaptive_threshold,
   "use NUM as adaptive threshold value, 1..100",
   "NUM"},
  {"scale", 's', 0, G_OPTION_ARG_INT, &scale_by,
   "scale input image by SCALE 1..9, 0 auto",
   "SCALE"},
  {"do-not-clean", 'n', 0, G_OPTION_ARG_NONE, &do_not_clean_image,
   "do not try to remove artefacts from image",
   NULL},
  {"remove-halfton", 'r', 0, G_OPTION_ARG_NONE, &remove_dots,
   "remove halfton dots from input image",
   NULL},
  {"remove-images", 'R', 0, G_OPTION_ARG_NONE, &remove_images,
   "remove images from input image",
   NULL},
  {"fix-broken-fonts", 'e', 0, G_OPTION_ARG_NONE, &fix_broken_fonts,
   "try to link broken fonts",
   NULL},
  {"fix-ligated-fonts", 'E', 0, G_OPTION_ARG_NONE, &fix_ligated_fonts,
   "try to break ligated fonts",
   NULL},
  {NULL}
};

static GOptionEntry segmentation_entries[] = {
  {"colums setup", 'c', 0, G_OPTION_ARG_INT, &paragraph_setup,
   "colums setup: 1 free, 2.. #colums, 0 auto",
   "NUM"},
  {"slicing", 'x', 0, G_OPTION_ARG_INT, &slicing_threshold,
   "use NUM as font slicing threshold, 1..250",
   "NUM"},
  {"slicing-width", 'X', 0, G_OPTION_ARG_INT, &slicing_width,
   "use NUM as font slicing width, 50..250",
   "NUM"},
  {NULL}
};

static GOptionEntry debug_entries[] = {
  {"draw-grid", 'g', 0, G_OPTION_ARG_NONE, &show_grid,
   "draw grid on output images", NULL},
  {"save-bw", 'b', 0, G_OPTION_ARG_NONE, &save_bw,
   "save proccesd bw image", NULL},
  {"save-bw-exit", 'B', 0, G_OPTION_ARG_NONE, &only_image_proccesing,
   "save proccesd bw image and exit", NULL},
  {"save-layout", 'l', 0, G_OPTION_ARG_NONE, &save_layout,
   "save layout image", NULL},
  {"save-layout-exit", 'L', 0, G_OPTION_ARG_NONE, &only_layout_analysis,
   "save layout image and exit", NULL},
  {"save-fonts", 'f', 0, G_OPTION_ARG_NONE, &save_fonts,
   "save fonts", NULL},
  {"save-fonts-exit", 'F', 0, G_OPTION_ARG_NONE, &only_save_fonts,
   "save fonts images and exit", NULL},
  {"debug", 'd', 0, G_OPTION_ARG_NONE, &verbose,
   "print debuging information while running", NULL},
  {"debug-extra", 'D', 0, G_OPTION_ARG_NONE, &debug,
   "print extra debuging information", NULL},
  {"font-filter", 'y', 0, G_OPTION_ARG_INT, &debug_font_filter,
   "debug a font filter, use filter NUM, 1..16", "NUM"},
  {"font-filter-list", 'Y', 0, G_OPTION_ARG_NONE, &debug_font_filter_list,
   "print a list of debug a font filters", NULL},
  {NULL}
};

static GOptionEntry entries[] = {
  {"image-in", 'i', 0, G_OPTION_ARG_FILENAME, &image_in_filename,
   "use FILE as input image file name", "FILE"},
  {"text-out", 'o', 0, G_OPTION_ARG_FILENAME, &text_out_filename,
   "use FILE as output text file name", "FILE"},
  /* this option is not useful fot the moment
     {"ltr", 'z', 0, G_OPTION_ARG_NONE, &dir_ltr,
     "left to right text", NULL}, */
  {"no-gtk", 'N', 0, G_OPTION_ARG_NONE, &no_gtk,
   "do not use gtk for file input and output", NULL},
  {"version", 'v', 0, G_OPTION_ARG_NONE, &version,
   "print version information and exit", NULL},
  {NULL}
};

int
hocr_printerr (const char *msg)
{
  g_printerr ("%s: %s\n\
Try `--help' for more information.\n", g_get_prgname (), msg);
  return TRUE;
}

int
hocr_cmd_parser (int *argc, char **argv[])
{
  GOptionContext *context;
  GOptionGroup *group;

  /* get user args */
  context = g_option_context_new ("- Hebrew OCR utility");

  group =
    g_option_group_new ("file", "File options", "Show file options", NULL,
			NULL);
  g_option_group_add_entries (group, file_entries);
  g_option_context_add_group (context, group);

  group =
    g_option_group_new ("image-proccesing", "Image proccesing options",
			"Show image proccesing options", NULL, NULL);
  g_option_group_add_entries (group, image_entries);
  g_option_context_add_group (context, group);

  group =
    g_option_group_new ("segmentation", "Segmentation options",
			"Show segmentation options", NULL, NULL);
  g_option_group_add_entries (group, segmentation_entries);
  g_option_context_add_group (context, group);

  group =
    g_option_group_new ("debug", "Debug options", "Show debug options", NULL,
			NULL);
  g_option_group_add_entries (group, debug_entries);
  g_option_context_add_group (context, group);

  g_option_context_add_main_entries (context, entries, PACKAGE_NAME);
  g_option_context_parse (context, argc, argv, &error);

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

  if (debug_font_filter_list)
    {
      g_printf (font_filters_list, BUILD);
      exit (0);
    }

  if (!no_gtk && !image_in_filename)
    {
      hocr_printerr ("no input image file name (use -n for pipeing)");
      exit (1);
    }

  if (debug || verbose)
    g_print ("%s - Hebrew OCR utility\n", PACKAGE_STRING);

  /* sanity check */
  if (adaptive_threshold_type > 2 || adaptive_threshold_type < 0)
    {
      hocr_printerr ("unknown thresholding type using normal settings");
      adaptive_threshold_type = 0;
    }

  if (scale_by > 9 || scale_by < 0)
    {
      hocr_printerr ("unknown scaling factor using auto settings");
      scale_by = 0;
    }

  if (paragraph_setup > 4 || paragraph_setup < 0)
    {
      hocr_printerr ("unknown paragraph setup using auto settings");
      paragraph_setup = 0;
    }

  if (threshold > 100 || threshold < 0)
    {
      hocr_printerr ("unknown thresholding value using auto settings");
      threshold = 0;
    }

  if (adaptive_threshold > 100 || adaptive_threshold < 0)
    {
      hocr_printerr ("unknown thresholding value using auto settings");
      adaptive_threshold = 0;
    }

  return FALSE;
}

/* FIXME: this function use globals */
ho_bitmap *
hocr_load_input_bitmap ()
{
  ho_pixbuf *pix = NULL;
  ho_bitmap *m_bw = NULL;

  ho_bitmap *m_bw_temp = NULL;
  ho_bitmap *m_bw_temp2 = NULL;

  /* read image from file */
  if (no_gtk)
    pix = ho_pixbuf_pnm_load (image_in_filename);
  else
    pix = ho_gtk_pixbuf_load (image_in_filename);

  if (!pix)
    {
      hocr_printerr ("can't read input image\n");
      exit (1);
    }

  if (debug || verbose)
    g_print (" input image is %d by %d pixels, with %d color channels\n",
	     pix->width, pix->height, pix->n_channels);

  m_bw =
    ho_pixbuf_to_bitmap_wrapper (pix, scale_by, adaptive_threshold_type,
				 threshold, adaptive_threshold);

  if (!m_bw)
    {
      hocr_printerr ("can't convert to black and white\n");
      exit (1);
    }

  /* prompt user scale */
  if (debug && scale_by)
    if (scale_by == 1)
      g_print (" user no scaling.\n", scale_by);
    else
      g_print (" user scale by %d.\n", scale_by);

  /* do we need to auto scale ? */
  if (!scale_by)
    {
      /* get fonts size for autoscale */
      if (ho_dimentions_font_width_height_nikud (m_bw, 6, 200, 6, 200))
	{
	  hocr_printerr ("can't create object map\n");
	  exit (1);
	}

      /* if fonts are too small and user wants auto scale, re-scale image */
      if (m_bw->font_height < 10)
	scale_by = 4;
      else if (m_bw->font_height < 25)
	scale_by = 2;
      else
	scale_by = 1;

      if (debug)
	if (scale_by > 1)
	  g_print (" auto scale by %d.\n", scale_by);
	else
	  g_print (" no auto scale neaded.\n");

      if (scale_by > 1)
	{
	  /* re-create bitmap */
	  ho_bitmap_free (m_bw);
	  m_bw =
	    ho_pixbuf_to_bitmap_wrapper (pix, scale_by,
					 adaptive_threshold_type, threshold,
					 adaptive_threshold);

	  if (!m_bw)
	    {
	      hocr_printerr
		("can't convert to black and white after auto scaleing \n");
	      exit (1);
	    }

	}
    }

  /* remove very small and very large things */
  if (!do_not_clean_image)
    {
      m_bw_temp =
	ho_bitmap_filter_by_size (m_bw, 3, 3 * m_bw->height / 4, 3,
				  3 * m_bw->width / 4);
      if (m_bw_temp)
	{
	  ho_bitmap_free (m_bw);
	  m_bw = m_bw_temp;
	  m_bw_temp = NULL;
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

  /* fix_broken_fonts */
  if (fix_broken_fonts)
    {
      if (debug)
	g_print (" try to fix broken fonts.\n");

      m_bw_temp = ho_bitmap_dilation (m_bw);
      if (m_bw_temp)
	{
	  ho_bitmap_free (m_bw);
	  m_bw = m_bw_temp;
	  m_bw_temp = NULL;
	}
    }

  /* fix_ligated_fonts */
  if (fix_ligated_fonts)
    {
      if (debug)
	g_print (" try to fix ligated fonts.\n");

      m_bw_temp = ho_bitmap_opening (m_bw);
      if (m_bw_temp)
	{
	  ho_bitmap_free (m_bw);
	  m_bw = m_bw_temp;
	  m_bw_temp = NULL;
	}
    }

  /* remove too big and too small objects */
  if (remove_images)
    {
      if (debug)
	g_print (" remove images.\n");

      /* adaptive threshold braks big objects use adaptive_threshold_type = 1 (none) */
      m_bw_temp = ho_pixbuf_to_bitmap_wrapper (pix, scale_by,
					       1, threshold,
					       adaptive_threshold);
      if (!m_bw_temp)
	{
	  hocr_printerr
	    ("can't convert to black and white for image removal \n");
	  exit (1);
	}

      /* get fonts size for autoscale */
      if (ho_dimentions_font_width_height_nikud (m_bw, 6, 200, 6, 200))
	{
	  hocr_printerr ("can't create object map\n");
	  exit (1);
	}

      /* remove big objects */
      m_bw_temp2 = ho_bitmap_filter_by_size (m_bw_temp,
					     m_bw->font_height * 4,
					     m_bw_temp->height,
					     m_bw->font_width * 9,
					     m_bw_temp->width);

      if (!m_bw_temp2)
	{
	  hocr_printerr
	    ("can't convert to black and white for image removal \n");
	  exit (1);
	}

      ho_bitmap_andnot (m_bw, m_bw_temp2);
      ho_bitmap_free (m_bw_temp2);

      /* remove small objects */
      m_bw_temp2 = ho_bitmap_filter_by_size (m_bw_temp,
					     1,
					     m_bw->font_height / 5,
					     1, m_bw->font_width / 8);
      ho_bitmap_free (m_bw_temp);

      if (!m_bw_temp2)
	{
	  hocr_printerr
	    ("can't convert to black and white for image removal \n");
	  exit (1);
	}

      ho_bitmap_andnot (m_bw, m_bw_temp2);
      ho_bitmap_free (m_bw_temp2);
    }

  /* free input pixbuf */
  ho_pixbuf_free (pix);

  return m_bw;
}

/* FIXME: this functions use globals */

int
hocr_exit ()
{
  int block_index;
  int line_index;

  /* allert user */
  if (debug)
    g_print ("free memory.\n");

  /* free image matrixes */
  ho_bitmap_free (m_page_text);

  /* free page layout  masks */
  ho_layout_free (l_page);

  /* free file names */
  if (image_in_filename)
    g_free (image_in_filename);
  if (image_out_path)
    g_free (image_out_path);
  if (text_out_filename)
    g_free (text_out_filename);
  if (data_out_filename)
    g_free (data_out_filename);

  /* exit program */
  exit (0);

  return FALSE;
}

int
main (int argc, char *argv[])
{
  int number_of_fonts;
  ho_string *s_text_out = NULL;
  ho_string *s_data_out = NULL;

  /* start of argument analyzing section 
   */

  hocr_cmd_parser (&argc, &argv);

  /* init gtk */
  if (!no_gtk)
    gtk_init (&argc, &argv);

  /* end of argument analyzing section */

  /* start of image proccesing section 
   */
  if (debug || verbose)
    g_print ("start image proccesing.\n");

  /* load the image from input filename */
  m_page_text = hocr_load_input_bitmap ();

  if (debug || verbose)
    if (scale_by > 1)
      g_print
	(" procced image is %d by %d pixels (origianl scaled by %d), one color bit.\n",
	 m_page_text->width, m_page_text->height, scale_by);
    else
      g_print (" procced image is %d by %d pixels, one color bit.\n",
	       m_page_text->width, m_page_text->height);

  /* if only image proccesing or save b/w image */
  if (save_bw || only_image_proccesing)
    {
      gchar *filename;
      ho_pixbuf *pix_out;

      pix_out = ho_pixbuf_new_from_bitmap (m_page_text);

      if (show_grid)
	ho_pixbuf_draw_grid (pix_out, 120, 30, 0, 0, 0);

      /* create file name */
      if (no_gtk)
	filename = g_strdup_printf ("%s-image-bw.pgm", image_out_path);
      else
	filename = g_strdup_printf ("%s-image-bw.jpeg", image_out_path);

      /* save to file system */
      if (filename)
	{
	  if (no_gtk)
	    ho_pixbuf_pnm_save (pix_out, filename);
	  else
	    ho_gtk_pixbuf_save (pix_out, filename);

	  /* free locale memory */
	  ho_pixbuf_free (pix_out);
	  g_free (filename);
	}
    }

  if (debug || verbose)
    g_print ("end image proccesing.\n");

  /* if user want save the b/w picture image proccesing produced */
  if (only_image_proccesing)
    hocr_exit ();

  /* end of image proccesing section */
  /* remember: by now you have allocated and not freed: m_page_text */

  /* start of layout analyzing section
   */
  if (debug || verbose)
    g_print ("start image layout analysis.\n");

  if (!paragraph_setup)
    {
      int cols;

      cols = ho_dimentions_get_columns (m_page_text);
      if (cols > 1)
	{
	  paragraph_setup = cols;
	  if (debug)
	    g_print ("  guessing %d columns.\n", cols);
	}
      else if (debug)
	g_print ("  guessing one column.\n");
    }

  l_page = ho_layout_new (m_page_text, paragraph_setup != 1, dir_ltr);

  ho_layout_create_block_mask (l_page);

  if (debug || verbose)
    g_print ("  found %d blocks.\n", l_page->n_blocks);

  /* start counting fonts */
  number_of_fonts = 0;

  /* look for lines inside blocks */
  {
    int block_index;
    int line_index;
    int word_index;

    for (block_index = 0; block_index < l_page->n_blocks; block_index++)
      {
	if (debug || verbose)
	  g_print ("  analyzing block %d.\n", block_index + 1);
	ho_layout_create_line_mask (l_page, block_index);

	if (debug || verbose)
	  g_print ("    found %d lines.\n", l_page->n_lines[block_index]);

	if (debug)
	  g_print
	    ("    font height %d width %d, line spacing %d\n",
	     l_page->m_blocks_text[block_index]->font_height,
	     l_page->m_blocks_text[block_index]->font_width,
	     l_page->m_blocks_text[block_index]->line_spacing);

	/* look for words inside line */
	for (line_index = 0; line_index < l_page->n_lines[block_index];
	     line_index++)
	  {
	    if (debug || verbose)
	      g_print ("      analyzing line %d.\n", line_index + 1);
	    ho_layout_create_word_mask (l_page, block_index, line_index);

	    if (debug)
	      g_print ("        line avg fill %d common fill %d\n",
		       l_page->m_lines_text[block_index][line_index]->
		       avg_line_fill,
		       l_page->m_lines_text[block_index][line_index]->
		       com_line_fill);

	    if (debug)
	      g_print ("        found %d words. font spacing %d\n",
		       l_page->n_words[block_index][line_index],
		       l_page->m_lines_text[block_index][line_index]->
		       font_spacing);

	    /* look for fonts inside word */
	    for (word_index = 0;
		 word_index < l_page->n_words[block_index][line_index];
		 word_index++)
	      {
		if (debug)
		  g_print ("        analyzing word %d.\n", word_index + 1);
		ho_layout_create_font_mask (l_page, block_index, line_index,
					    word_index, slicing_threshold,
					    slicing_width);

		if (debug)
		  g_print ("          found %d fonts.\n",
			   l_page->
			   n_fonts[block_index][line_index][word_index]);

		/* count fonts */
		number_of_fonts += l_page->
		  n_fonts[block_index][line_index][word_index];
	      }
	  }
      }
  }

  if (debug || verbose)
    g_print ("end of image layout analysis.\n");

  /* if only layout analysis or save layout image */
  if (save_layout || only_layout_analysis)
    {
      gchar *filename = NULL;
      ho_pixbuf *pix_out = NULL;
      ho_bitmap *m_block_frame = NULL;
      ho_bitmap *m_word_text = NULL;
      ho_bitmap *m_word_mask = NULL;
      ho_bitmap *m_word_font_mask = NULL;
      int block_index;
      int line_index;
      int word_index;

      /* allocate */
      pix_out = ho_pixbuf_new (3, m_page_text->width, m_page_text->height, 0);

      /* add text blocks */
      m_block_frame = ho_bitmap_edge (l_page->m_page_blocks_mask, 5);
      ho_pixbuf_draw_bitmap (pix_out, m_block_frame, 0, 0, 255, 150);
      ho_bitmap_free (m_block_frame);

      /* loop on all text blocks */
      for (block_index = 0; block_index < l_page->n_blocks; block_index++)
	{
	  for (line_index = 0; line_index < l_page->n_lines[block_index];
	       line_index++)
	    {
	      ho_pixbuf_draw_bitmap (pix_out,
				     l_page->
				     m_lines_words_mask[block_index]
				     [line_index], 255, 240, 0, 180);

	      m_block_frame =
		ho_bitmap_edge (l_page->
				m_lines_line_mask[block_index][line_index],
				5);

	      ho_pixbuf_draw_bitmap (pix_out, m_block_frame, 255, 0, 0, 255);

	      ho_bitmap_free (m_block_frame);

	      for (word_index = 0;
		   word_index < l_page->n_words[block_index][line_index];
		   word_index++)
		{
		  m_word_font_mask =
		    l_page->
		    m_words_font_mask[block_index][line_index][word_index];

		  ho_pixbuf_draw_bitmap (pix_out, m_word_font_mask, 0, 250, 0,
					 235);
		}
	    }
	}

      /* add grid */
      if (show_grid)
	ho_pixbuf_draw_grid (pix_out, 120, 30, 255, 0, 0);

      /* add text in black */
      ho_pixbuf_draw_bitmap (pix_out, m_page_text, 0, 0, 0, 255);

      /* create file name */
      if (no_gtk)
	filename = g_strdup_printf ("%s-image-segments.pgm", image_out_path);
      else
	filename = g_strdup_printf ("%s-image-segments.jpeg", image_out_path);

      /* save to file system */
      if (filename)
	{
	  if (no_gtk)
	    ho_pixbuf_pnm_save (pix_out, filename);
	  else
	    ho_gtk_pixbuf_save (pix_out, filename);

	  ho_pixbuf_free (pix_out);
	  g_free (filename);
	}
    }

  /* if user only want layout image exit now */
  if (only_layout_analysis)
    hocr_exit ();

  /* start of word recognition section
   */
  if (debug || verbose)
    g_print ("start word recognition section.\n");

  if (debug || verbose)
    g_print ("  found %d fonts in page.\n", number_of_fonts);

  {
    int i;
    int font_number = 0;
    int block_index;
    int line_index;
    int word_index;
    int font_index;

    ho_pixbuf *pix_out = NULL;
    ho_bitmap *m_text = NULL;
    ho_bitmap *m_mask = NULL;
    ho_bitmap *m_font_mask = NULL;
    ho_bitmap *m_font_filter = NULL;
    ho_bitmap *m_font_test = NULL;
    ho_bitmap *m_edge = NULL;

    ho_objmap *o_fonts = NULL;

    gchar *filename = NULL;
    gsize length;
    gsize terminator_pos;
    GError *error = NULL;

    /* start the text out */
    s_text_out = ho_string_new ();
    if (!s_text_out)
      {
	hocr_printerr ("can't allocate text memory");
	hocr_exit ();
      }

    /* start the data out */
    if (data_out_filename)
      {
	s_data_out = ho_string_new ();
	if (!s_data_out)
	  {
	    hocr_printerr ("can't allocate data memory");
	    hocr_exit ();
	  }

	/* init the first line of data file */
	text_out =
	  g_strdup_printf ("%d %d %d\n", number_of_fonts,
			   ho_recognize_array_in_size (),
			   ho_recognize_array_out_size ());
	ho_string_cat (s_data_out, text_out);
	g_free (text_out);

      }

    for (block_index = 0; block_index < l_page->n_blocks; block_index++)
      {
	for (line_index = 0; line_index < l_page->n_lines[block_index];
	     line_index++)
	  {
	    for (word_index = 0;
		 word_index < l_page->n_words[block_index][line_index];
		 word_index++)
	      {
		for (font_index = 0;
		     font_index <
		     l_page->n_fonts[block_index][line_index][word_index];
		     font_index++)
		  {
		    /* count recognized fonts */
		    font_number++;

		    if (verbose || debug)
		      g_print ("    recognizing font %d (%d).\n", font_number,
			       number_of_fonts);

		    if (debug)
		      g_print ("    in - block:%d line:%d word:%d font:%d.\n",
			       block_index + 1, line_index + 1,
			       word_index + 1, font_index + 1);

		    /* get the font */
		    m_text =
		      ho_layout_get_font_text (l_page, block_index,
					       line_index, word_index,
					       font_index);
		    m_mask =
		      ho_layout_get_font_line_mask (l_page, block_index,
						    line_index, word_index,
						    font_index);

		    /* get font main sign */
		    m_font_mask = ho_font_main_sign (m_text, m_mask);

		    if (m_mask && m_font_mask)
		      {
			/* get nikud */
			m_font_filter = ho_bitmap_clone (m_text);
			ho_bitmap_andnot (m_font_filter, m_font_mask);

			/* debug the font filter functions: */
			if (save_fonts && debug_font_filter)
			  {
			    switch (debug_font_filter)
			      {
			      case 1:
				m_font_test =
				  ho_font_main_sign (m_font_mask, m_mask);
				break;
			      case 2:
				m_font_test =
				  ho_font_second_object (m_font_mask, m_mask);
				break;
			      case 3:
				m_font_test =
				  ho_font_holes (m_font_mask, m_mask);
				break;
			      case 4:
				m_font_test =
				  ho_font_hbars (m_font_mask, m_mask);
				break;
			      case 5:
				m_font_test =
				  ho_font_vbars (m_font_mask, m_mask);
				break;
			      case 6:
				m_font_test =
				  ho_font_diagonal (m_font_mask, m_mask);
				break;
			      case 7:
				m_font_test =
				  ho_font_diagonal_left (m_font_mask, m_mask);
				break;
			      case 8:
				m_font_test =
				  ho_font_thin (m_font_mask, m_mask);
				break;
			      case 9:
				m_font_test =
				  ho_font_edges_top (m_font_mask, m_mask);
				break;
			      case 10:
				m_font_test =
				  ho_font_edges_bottom (m_font_mask, m_mask);
				break;
			      case 11:
				m_font_test =
				  ho_font_edges_left (m_font_mask, m_mask);
				break;
			      case 12:
				m_font_test =
				  ho_font_edges_right (m_font_mask, m_mask);
				break;
			      case 13:
				m_font_test =
				  ho_font_notch_top (m_font_mask, m_mask);
				break;
			      case 14:
				m_font_test =
				  ho_font_notch_bottom (m_font_mask, m_mask);
				break;
			      case 15:
				m_font_test =
				  ho_font_notch_left (m_font_mask, m_mask);
				break;
			      case 16:
				m_font_test =
				  ho_font_notch_right (m_font_mask, m_mask);
				break;
			      default:
				m_font_test = NULL;
				break;
			      }
			  }
		      }

		    /* recognize the font and send it out */

		    /* if user ask, dump font images to disk */
		    if (save_fonts && m_mask && m_font_filter && m_font_mask)
		      {
			filename =
			  g_strdup_printf ("%s-font-%d.pnm", image_out_path,
					   font_number);

			/* if user want to debug font filters, printout the filtered font */
			if (m_font_test && debug_font_filter)
			  ho_font_pnm_save (m_font_mask, m_font_test, m_mask,
					    filename);
			else
			  ho_font_pnm_save (m_font_mask, m_font_filter,
					    m_mask, filename);

			g_free (filename);
		      }

		    /* get data on font */
		    if (m_font_mask && m_mask)
		      {
			char *font;

			/* if debug printout font data stream */
			if (debug)
			  {
			    /* FIXME: waht if ho_recognize_array_in/out_size() > 100 ? */
			    double array_in[100];
			    double array_out[100];

			    ho_recognize_create_array_in (m_font_mask, m_mask,
							  array_in);
			    ho_recognize_create_array_out (array_in,
							   array_out);

			    g_print ("array_in:\n");
			    for (i = 0; i < ho_recognize_array_in_size ();
				 i++)
			      {
				g_print ("%02.2f\t", array_in[i]);
				if (!((i + 1) % 10))
				  g_print ("\n");
			      }
			    g_print ("\n");

			    g_print ("array_out:\n");
			    for (i = 0; i < ho_recognize_array_out_size ();
				 i++)
			      {
				g_print ("%02.2f\t", array_out[i]);
				if (!((i + 1) % 10))
				  g_print ("\n");
			      }
			    g_print ("\n");
			  }

			/* if user want to dump data to file */
			if (data_out_filename && m_mask && m_font_filter
			    && m_font_mask)
			  {
			    /* FIXME: waht if ho_recognize_array_out/out_size() > 100 ? */
			    double array_in[100];
			    double array_out[100];
			    double min, max;

			    ho_recognize_create_array_in (m_font_mask, m_mask,
							  array_in);
			    ho_recognize_create_array_out (array_in,
							   array_out);

			    /* print out the number of the font */
			    text_out =
			      g_strdup_printf ("\nfont %d:\n ", font_number);
			    ho_string_cat (s_data_out, text_out);
			    g_free (text_out);

			    /* add this font line */
			    for (i = 0; i < ho_recognize_array_in_size ();
				 i++)
			      {
				min = array_in[i] - 0.13;
				max = array_in[i] + 0.13;
				if (min < 0.0)
				  min = 0.0;
				if (max > 1.0)
				  max = 1.0;

				if (!((i + 1) % 5))
				  text_out =
				    g_strdup_printf ("{%02.1f, %02.1f},\n ",
						     min, max);
				else
				  text_out =
				    g_strdup_printf ("{%02.1f, %02.1f}, ",
						     min, max);

				ho_string_cat (s_data_out, text_out);
				g_free (text_out);
			      }

			    ho_string_cat (s_data_out, "\n");

			    /* add this font array in line */
			    for (i = 0; i < ho_recognize_array_in_size ();
				 i++)
			      {
				if (!((i + 1) % 10))
				  text_out =
				    g_strdup_printf ("[%02d] %02.1f,\n", i,
						     array_in[i]);
				else
				  text_out =
				    g_strdup_printf ("[%02d] %02.1f, ", i,
						     array_in[i]);

				ho_string_cat (s_data_out, text_out);
				g_free (text_out);
			      }

			    ho_string_cat (s_data_out, "\n");

			    /* add this font array out line */
			    for (i = 0; i < ho_recognize_array_out_size ();
				 i++)
			      {
				if (!((i + 1) % 10))
				  text_out =
				    g_strdup_printf ("[%02d] %02.1f,\n", i,
						     array_out[i]);
				else
				  text_out =
				    g_strdup_printf ("[%02d] %02.1f, ", i,
						     array_out[i]);

				ho_string_cat (s_data_out, text_out);
				g_free (text_out);
			      }

			    ho_string_cat (s_data_out, "\n");

			  }

			/* insert font to text out */
			if (m_font_mask && m_mask)
			  {
			    font = ho_recognize_font (m_font_mask, m_mask);

			    ho_string_cat (s_text_out, font);
			    /* if debug print out the font */
			    if (debug)
			      g_print ("font :%s\n", font);
			  }
			else
			  {
			    ho_string_cat (s_text_out, "#");
			    /* if debug print out the font */
			    if (debug)
			      g_print ("font : \n");
			  }
		      }

		    /* free bitmaps and others */
		    ho_bitmap_free (m_font_filter);
		    ho_bitmap_free (m_font_test);
		    ho_bitmap_free (m_font_mask);
		    ho_bitmap_free (m_text);
		    ho_bitmap_free (m_mask);

		    m_text = m_mask = m_font_filter = m_font_mask = NULL;

		    /* oh ... */

		  }		/* end of fonts loop */
		/* add a space after a word to text out */
		ho_string_cat (s_text_out, " ");

	      }			/* end of words loop */
	    /* add a line-end after a line to text out */
	    ho_string_cat (s_text_out, "\n");
	  }			/* end of lines loop */
	/* add a line-end after a block end */
	ho_string_cat (s_text_out, "\n");

      }				/* end of blocks loop */

  }

  if (debug || verbose)
    g_print ("end word recognition section.\n");
  /* end of word recognition section */

  /* start user output section 
   */

  if (debug || verbose)
    g_print ("start output section.\n");

  /* convert text from ho_string to (gchar *) */
  text_out = g_strdup_printf ("%s", s_text_out->string);
  ho_string_free (s_text_out);

  /* save text */
  if (text_out_filename && text_out)
    {
      /* if filename is '-' print to stdout */
      if (text_out_filename[0] == '-' && text_out_filename[1] == '\0')
	g_printf (text_out);
      else
	g_file_set_contents (text_out_filename, text_out, -1, &error);

      if (error)
	{
	  hocr_printerr ("can't write text to file");
	}
    }

  g_free (text_out);
  text_out = NULL;

  /* convert data from ho_string to (gchar *) */
  if (s_data_out)
    {
      text_out = g_strdup_printf ("%s", s_data_out->string);
      ho_string_free (s_data_out);
    }

  /* save data */
  if (data_out_filename && text_out)
    {
      g_file_set_contents (data_out_filename, text_out, -1, &error);

      if (error)
	{
	  hocr_printerr ("can't write data to file");
	}
    }

  if (text_out)
    g_free (text_out);

  if (debug || verbose)
    g_print ("end output section.\n");

  /* end of user putput section */

  /* start of memory cleanup section 
   */

  hocr_exit ();

  return FALSE;
}
