
/* 
 * hocr_c.c
 *
 * example file for libhocr.
 * compile: gcc -I/usr/include/libhocr -lm -lhocr -o hocr_c hocr_c.c
 * use: hocr_c IMAGE_FILENAME
 *
 * Copyright (C) 2008  Yaacov Zamir <kzamir@walla.co.il>
 */

/*  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <hocr.h>

int
main (int argc, char *argv[])
{
  ho_pixbuf *pix = NULL;
  ho_string *text = NULL;
  int progress;

  /* load the pnm picture pointed to by argv[1] */
  /* NOTE: this function only load pnm images, you can use the hocr-gtk
   * functions to load other image types */
  pix = ho_pixbuf_pnm_load (argv[1]);

  /* get a new text buffer */
  text = ho_string_new ();

  /* do ocr on picture */
  /* NOTE: we do not use the progress indicator in this program. to use it,
   * create a new thread and sample the value of progress periodically */
  hocr_do_ocr (pix, text, 0, 0, &progress);

  /* print out the text */
  printf (text->string);

  /* free picture */
  ho_pixbuf_free (pix);

  /* free string */
  ho_string_free (text);

  return 0;
}
