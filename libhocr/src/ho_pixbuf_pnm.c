/***************************************************************************
 *            ho_pixbuf_pnm.c
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#ifndef TRUE
#define TRUE -1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL ((void*)0)
#endif

#include "ho_pixbuf_pnm.h"

unsigned char
ho_pbm_getc (FILE * file)
{
  unsigned char ch;
  int comment = FALSE;

  do
    {
      ch = getc (file);
      if (ch == '\n')
	comment = FALSE;
      else if (ch == '#')
	comment = TRUE;
    }
  while (comment);

  return ch;
}

int
ho_pbm_getint (FILE * file)
{
  unsigned char ch;
  int i = 0;

  do
    {
      ch = ho_pbm_getc (file);
    }
  while (ch == ' ' || ch == '\n' || ch == '\t');

  do
    {
      i = (i * 10) + (ch - '0');
      ch = ho_pbm_getc (file);
    }
  while (ch >= '0' && ch <= '9');

  return i;
}

int
ho_pbm_getbit (FILE * file)
{

  static unsigned char byte = 0;
  static unsigned char mask = 0;
  int return_bit;

  if (mask == 0)
    {
      mask = 0x80;
      byte = getc (file);
    }

  return_bit = (byte & mask) ? 0 : 255;

  mask >>= 1;

  return return_bit;
}

ho_pixbuf *
ho_pixbuf_pnm_load (const char *filename)
{
  char ch1, ch2;
  ho_pixbuf *pix = NULL;
  FILE *file = NULL;
  unsigned char use_stdin = 0;
  unsigned char n_channels = 0;
  unsigned char val = 0;
  int width = 0;
  int height = 0;
  int x, y, rowstride;
  int i;

  /* if no input file name use stdin for input */
  if (!filename || filename[0] == '\0'
      || (filename[0] == '-' && filename[1] == '\0'))
    {
      /* use stdin */
      file = stdin;
      use_stdin = 1;
    }
  else
    {
      /* open file */
      file = fopen (filename, "r");
      if (!file)
	return NULL;
    }

  /* read magic number "P?" for pbm file */
  ch1 = ho_pbm_getc (file);
  ch2 = ho_pbm_getc (file);
  if (ch1 != 'P' || (ch2 != '6' && ch2 != '5' && ch2 != '4'))
    {
      /* bad magic */
      if (!use_stdin)
	fclose (file);
      return NULL;
    }

  /* read header */
  n_channels = ((ch2 == '6') ? 3 : 1);
  width = ho_pbm_getint (file);
  height = ho_pbm_getint (file);

  if (ch2 == '4')
    {
      /* read bites per pixel */

      /* create a new pixbuf */
      pix = ho_pixbuf_new (n_channels, width, height, 0);

      rowstride = 8 * (width / 8 + 1);
      if (pix)
	for (y = 0; y < height; y++)
	  for (x = 0; x < rowstride; x++)
	    {
	      val = ho_pbm_getbit (file);
	      if (x < width)
		ho_pixbuf_set (pix, x, y, 0, val);
	    }

    }
  else
    {
      /* read bytes per pixel */

      /* check for color depth */
      if (ho_pbm_getint (file) > 255)
	{
	  /* bad bits per pixel */
	  if (!use_stdin)
	    fclose (file);

	  return NULL;
	}

      /* create a new pixbuf */
      pix = ho_pixbuf_new (n_channels, width, height, 0);

      if (pix)
	fread (pix->data, 1, pix->height * pix->rowstride, file);
    }

  if (!use_stdin)
    fclose (file);

  /* return the new pixbuf to user */
  return pix;
}

int
ho_pixbuf_pnm_save (const ho_pixbuf * pix, const char *filename)
{
  FILE *file = NULL;

  if (pix->n_channels != 3 && pix->n_channels != 1)
    {
      /* bad magic */
      return TRUE;
    }

  file = fopen (filename, "wb");

  if (!file)
    return TRUE;

  /* print header */
  fprintf (file, "P%c %d %d 255\n", ((pix->n_channels == 3) ? '6' : '5'),
	   pix->width, pix->height);

  /* this might be a huge write... */
  fwrite (pix->data, 1, pix->height * pix->rowstride, file);
  fclose (file);

  return FALSE;
}
