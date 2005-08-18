/***************************************************************************
 *            hocr.h
 *
 *  Fri Aug 12 20:18:34 2005
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

#ifndef __HOCR_H__
#define __HOCR_H__

#ifndef TRUE
#define TRUE -1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef struct
{
	int x1;
	int y1;
	int x2;
	int y2;
	int hight;
	int width;
} box;

typedef struct
{
	int n_channels;
	int height;
	int width;
	int rowstride;
	int brightness;
	unsigned char *pixels;
} hocr_pixbuf;

int hocr_pixbuf_get_n_channels (hocr_pixbuf * pix);
int hocr_pixbuf_get_height (hocr_pixbuf * pix);
int hocr_pixbuf_get_width (hocr_pixbuf * pix);
int hocr_pixbuf_get_rowstride (hocr_pixbuf * pix);
int hocr_pixbuf_get_brightness (hocr_pixbuf * pix);
unsigned char *hocr_pixbuf_get_pixels (hocr_pixbuf * pix);

int hocr_pixbuf_get_pixel (hocr_pixbuf * pix, int x, int y);
int hocr_pixbuf_set_pixel (hocr_pixbuf * pix, int x, int y, int channel, int value);

/* this function open pnm file 
 of type "P4" Portable Any Map:
 binary, black/white 
 */
hocr_pixbuf *hocr_pixbuf_new_from_file (const char *filename);

int hocr_pixbuf_unref (hocr_pixbuf * pix);

/* user only use this function */
int hocr_do_ocr (hocr_pixbuf * pix, char *text_buffer, int max_buffer_size);

#endif
