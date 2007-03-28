/***************************************************************************
 *            main_teach.c
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
#include <floatfann.h>

#include "hocr.h"
#include "ho_gtk.h"

gchar *data_filename = NULL;
gchar *net_in_filename = NULL;
gchar *net_filename = NULL;
gboolean version = FALSE;
GError *error = NULL;
gint max_epochs = 0;

static gchar *copyright_message = "hocr-teach - Hebrew OCR teaching utility\n\
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
  {"data-in", 'u', 0, G_OPTION_ARG_FILENAME, &data_filename,
   "use FILE as input data file name", "FILE"},
  {"net-out", 'n', 0, G_OPTION_ARG_FILENAME, &net_filename,
   "use FILE as output net file name", "FILE"},
  {"net-in", 'N', 0, G_OPTION_ARG_FILENAME, &net_in_filename,
   "use FILE as input net file name", "FILE"},
  {"max-epochs", 'm', 0, G_OPTION_ARG_INT, &max_epochs,
   "maximum number of epochs to run",
   "SCALE"},
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

hocr_cmd_parser (int *argc, char **argv[])
{
  GOptionContext *context;

  /* get user args */
  context = g_option_context_new ("- Hebrew teaching OCR utility.");

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

  if (!data_filename || !net_filename)
    {
      hocr_printerr ("missing data or net file name");
      exit (1);
    }

  if (!max_epochs)
    max_epochs = 4000;

  return FALSE;
}

int
main (int argc, char *argv[])
{
  const float desired_error = (const float) 0.00001;
  const unsigned int epochs_between_reports = 10;
  unsigned int layers[3] = { HO_ARRAY_IN_SIZE, HO_ARRAY_IN_SIZE / 2 + 3,
    1
  };
  struct fann *ann = NULL;

  /* do argument analyzing */
  hocr_cmd_parser (&argc, &argv);

  g_print ("%s - Hebrew OCR teaching utility\n", PACKAGE_STRING);

  /* if user has a seed network, use that one */
  if (net_in_filename)
    {
      g_print ("Loading memory net file %s\n", net_in_filename);
      ann = fann_create_from_file (net_in_filename);
    }
  else
    {
      g_print ("Creating a new memory net\n");
      ann = fann_create_standard_array (3, layers);
    }

  if (!ann)
    return -1;

  /* set net function to work in hocr's -1 .. 1 range */
  /* activation_functions with -1..1 range: 
     FANN_SIGMOID_SYMMETRIC,  FANN_LINEAR_PIECE_SYMMETRIC,
     FANN_ELLIOT_SYMMETRIC */
  fann_set_activation_function_hidden (ann, FANN_ELLIOT_SYMMETRIC);
  fann_set_activation_function_output (ann, FANN_ELLIOT_SYMMETRIC);

  /* train net */
  fann_train_on_file (ann, data_filename, (unsigned int) max_epochs,
		      epochs_between_reports, desired_error);

  g_print ("Saving memory net file %s\n", net_filename);
  fann_save (ann, net_filename);

  fann_destroy (ann);

  return 0;
}
