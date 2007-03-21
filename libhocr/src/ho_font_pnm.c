/***************************************************************************
 *            ho_font_pnm.c
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

#include "ho_bitmap.h"
#include "ho_pixbuf.h"
#include "ho_pixbuf_pnm.h"
#include "ho_objmap.h"
#include "ho_bitmap_filter.h"
#include "ho_segment.h"
#include "ho_font.h"

#include "ho_font_pnm.h"

int
ho_font_pnm_save (const ho_bitmap * m_text, const ho_bitmap * m_nikud,
		  const ho_bitmap * m_mask, const char *filename)
{
  FILE *file = NULL;
  ho_pixbuf *pix = NULL;
  int x, y;
  unsigned char red, green, blue;

  /* create pixbuf */
  pix = ho_pixbuf_new (3, m_text->width, m_text->height, 0);
  if (!pix)
    return TRUE;

  /* fill the pixbuf */
  for (x = 0; x < m_text->width; x++)
    for (y = 0; y < m_text->height; y++)
      {
	red = ho_bitmap_get (m_text, x, y) * 255;
	green = ho_bitmap_get (m_nikud, x, y) * 255;
	blue = ho_bitmap_get (m_mask, x, y) * 255;

	ho_pixbuf_set (pix, x, y, 0, red);
	ho_pixbuf_set (pix, x, y, 1, green);
	ho_pixbuf_set (pix, x, y, 2, blue);
      }

  /* open file for writing */
  file = fopen (filename, "wb");

  if (!file)
    {
      ho_pixbuf_free (pix);
      return TRUE;
    }

  /* print header */
  fprintf (file, "P6 %d %d 255\n", pix->width, pix->height);

  /* this might be a huge write... */
  fwrite (pix->data, 1, pix->height * pix->rowstride, file);
  fclose (file);

  ho_pixbuf_free (pix);

  return FALSE;
}

int
ho_font_pnm_load (ho_bitmap ** m_text, ho_bitmap ** m_nikud,
		  ho_bitmap ** m_mask, const char *filename)
{
  ho_pixbuf *pix = NULL;
  int x, y;
  unsigned char red, green, blue;

  /* load pix from file */
  pix = ho_pixbuf_pnm_load (filename);
  if (!pix)
    return TRUE;

  if (pix->n_channels != 3)
    {
      ho_pixbuf_free (pix);
      return TRUE;
    }

  /* allocate the font bitmaps */
  (*m_text) = ho_bitmap_new (pix->width, pix->height);
  (*m_nikud) = ho_bitmap_new (pix->width, pix->height);
  (*m_mask) = ho_bitmap_new (pix->width, pix->height);

  /* set contents */
  for (x = 0; x < (*m_text)->width; x++)
    for (y = 0; y < (*m_text)->height; y++)
      {
	red = ho_pixbuf_get (pix, x, y, 0);
	green = ho_pixbuf_get (pix, x, y, 1);
	blue = ho_pixbuf_get (pix, x, y, 2);

	if (red)
	  ho_bitmap_set ((*m_text), x, y);
	if (green)
	  ho_bitmap_set ((*m_nikud), x, y);
	if (blue)
	  ho_bitmap_set ((*m_mask), x, y);

      }

  ho_pixbuf_free (pix);
  return FALSE;
}
