/***************************************************************************
 *            hocr-cmd.c
 *
 *  Fri Aug 12 20:13:33 2005
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

#define STRING_MAX_SIZE 255

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hocr.h"

int
print_help ()
{
  printf ("hocr %s - Hebrew OCR program\n", VERSION);
  printf ("http://hocr.berlios.de\n");
  printf ("USAGE: hocr -i pic_filename [-o text_filename] [-f html/text]\n");
  printf ("\n");

  return 0;
}

int
save_text (char *filename, char *format_out, char *text)
{
  FILE *file;

  /*
   * if format string begin with 'h' assume html output 
   */
  if (filename)
    {
      /*
       * save to file 
       */
      file = fopen (filename, "w");

      /*
       * can't open file 
       */
      if (!file)
	{
	  printf ("hocr: can\'t save file as %s\n", filename);
	  exit (0);
	}

      if (format_out[0] == 'h')
	fprintf (file, "<html>\n<meta http-equiv=\"Content-Type\" \
content=\"text/html; charset=UTF-8\">\n \
<body dir=\"rtl\"><pre>\n");

      fprintf (file, "%s", text);

      if (format_out[0] == 'h')
	fprintf (file, "</pre></body>\n</html>\n");
      fclose (file);
    }
  else
    {
      /*
       * no file name - print to std output 
       */
      if (format_out[0] == 'h')
	printf ("<html>\n<meta http-equiv=\"Content-Type\" \
content=\"text/html; charset=UTF-8\">\n \
<body dir=\"rtl\"><pre>\n");

      printf ("%s", text);

      if (format_out[0] == 'h')
	printf ("</pre></body>\n</html>\n");
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  int opt_i = 0;
  int opt_o = 0;
  int opt_f = 0;
  char c;

  char filename_in[STRING_MAX_SIZE];
  char filename_out[STRING_MAX_SIZE];
  char format_out[STRING_MAX_SIZE];

  hocr_pixbuf *pix;
  hocr_text_buffer *text;

  /*
   * default output is text file 
   */
  format_out[0] = 't';

  while ((c = getopt (argc, argv, "hi:o:f:")) != EOF)
    {
      switch (c)
	{
	case 'i':
	  if (optarg && strlen (optarg) < STRING_MAX_SIZE)
	    {
	      strcpy (filename_in, optarg);
	      opt_i = 1;
	    }
	  break;
	case 'o':
	  if (optarg && strlen (optarg) < STRING_MAX_SIZE)
	    {
	      strcpy (filename_out, optarg);
	      opt_o = 1;
	    }
	  break;
	case 'f':
	  if (optarg && strlen (optarg) < STRING_MAX_SIZE)
	    {
	      strcpy (format_out, optarg);
	      opt_f = 1;
	    }
	  break;
	case 'h':
	  print_help ();
	  exit (0);
	  break;
	default:
	  print_help ();
	  exit (0);
	  break;
	}
    }

  if (opt_i == 0)
    {
      print_help ();
      exit (0);
    }

  /*
   * create a new pixbuf from pbm file 
   */
  pix = hocr_pixbuf_new_from_file (filename_in);

  if (!pix)
    {
      printf ("hocr: can\'t read file %s\n", filename_in);
      exit (0);
    }

  /** do ocr */

  /*
   * build ocr command 
   */
  pix->command = HOCR_COMMAND_OCR;

  /* create text buffer */
  text = hocr_text_buffer_new ();

  if (!text)
  {
    printf ("hocr-gnome: can\'t allocate memory for text out\n");
    return 0;
  }

  /*
   * run the ocr 
   */
  hocr_do_ocr (pix, text);

  /*
   * unref memory 
   */
  hocr_pixbuf_unref (pix);

  /*
   * print out the text 
   */
  if (opt_o == 1)
    save_text (filename_out, format_out, text->text);
  else
    save_text (NULL, format_out, text->text);

  /* unref text_buffer */
  hocr_text_buffer_unref (text);
  
  return 0;
}
