/***************************************************************************
 *            hocr_pixbuf.h
 *
 *  Sat Dec 10 21:05:19 2005
 *  Copyright  2005  Yacov Zamir
 *  <kzamir@walla.co.il>
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
#include "consts.h"
#include "hocr_object.h"

#ifndef __HOCR_PIXBUF_H__
#define __HOCR_PIXBUF_H__

#define HOCR_COMMAND_COLOR_BOXES 1
#define HOCR_COMMAND_COLOR_MISREAD 2
#define HOCR_COMMAND_OCR 4
#define HOCR_COMMAND_DICT 8

/** line equation structore used by hocr
 
 a and b in the equation are Y = a X + b
 */
typedef struct
{
	double a;
	double b;
} hocr_line_eq;

typedef struct
{
	int x1;
	int y1;
	int x2;
	int y2;
	int hight;
	int width;
} hocr_box;

/** pixbuf structore used by hocr

*/
typedef struct
{
	/** number of color channels in the pixpuf (e.g. 3 for red,green,blue). */
	int n_channels;
	/** get height in pixels. */
	int height;
	/** get width in pixels. */
	int width;
	/** get number of bytes in a raw of pixels. */
	int rowstride;
	/** value from which a gray-scale pixel is considered white. */
	int brightness;
	/** raw pixpuf data. */
	unsigned char *pixels;
	/** raw object map */
	unsigned int *object_map;
	/** objects list */
	hocr_object *objects;
	/* number of objects found */
	int num_of_objects;
	
	/* I/O members */
	unsigned char command;
	unsigned char error;

} hocr_pixbuf;

/* object helper functions */

unsigned int
hocr_pixbuf_get_objects_in_box (hocr_pixbuf * pix, hocr_box box, 
	unsigned int object_array[MAX_OBJECTS_IN_FONT]);

/**
 @brief get number of channels

 @param pix pointer to hocr_pixbuf struct.
 @return number of color channels in the pixpuf (e.g. 3 for red,green,blue).
 */
int hocr_pixbuf_get_n_channels (hocr_pixbuf * pix);

/**
 @brief get height in pixels

 @param pix pointer to hocr_pixbuf struct.
 @return height of pixpuf in pixels.
 */
int hocr_pixbuf_get_height (hocr_pixbuf * pix);

/**
 @brief get width in pixels

 @param pix pointer to hocr_pixbuf struct.
 @return width of pixpuf in pixels.
 */
int hocr_pixbuf_get_width (hocr_pixbuf * pix);

/**
 @brief get number of bytes in a raw of pixels

 @param pix pointer to hocr_pixbuf struct.
 @return how many bytes are used by the struct for one raw.
 */
int hocr_pixbuf_get_rowstride (hocr_pixbuf * pix);

/**
 @brief get value from which a gray-scale pixel is considered white

 @param pix pointer to hocr_pixbuf struct.
 @return value from which a gray-scale pixel is considered white.
 */
int hocr_pixbuf_get_brightness (hocr_pixbuf * pix);

/**
 @brief get pointer to raw pixpuf data

 @param pix pointer to hocr_pixbuf struct.
 @return pointer to raw pixpuf data
 */
unsigned char *hocr_pixbuf_get_pixels (hocr_pixbuf * pix);

/**
 @brief get color of pixel

 @param pix pointer to hocr_pixbuf struct.
 @param x position of pixel on x axis
 @param y position of pixel on y axis
 @return 1 - if pixel is black, 0 - if pixel is white
 */
int hocr_pixbuf_get_pixel (hocr_pixbuf * pix, int x, int y);

/**
 @brief set color of pixel color channel

 @param pix pointer to hocr_pixbuf struct.
 @param x position of pixel on x axis
 @param y position of pixel on y axis
 @param channel the color chanell to set
 @param value the value to set the chanell to
 @return the color set
 */
int hocr_pixbuf_set_pixel (hocr_pixbuf * pix, int x, int y, int channel, int value);

unsigned int hocr_pixbuf_get_object (hocr_pixbuf * pix, int x, int y);

unsigned int hocr_pixbuf_set_object (hocr_pixbuf * pix, int x, int y,
	unsigned int object_numebr);


/* hocr_pixbuf_new_from_file function can only open pnm file 
 of type "P4/5" Portable Any Map (PNM) binary, black/white format
 */

/**
 @brief creats a new hocr_pixbuf struct from file 

 @param filename path to a pbm file, file must be binary b/w pnm file ("P4/5").
 @return pointer to a newly allocate hocr_pixbuf, or null if can not open file.
 */
hocr_pixbuf *hocr_pixbuf_new_from_file (const char *filename);

/**
 @brief free a hocr_pixbuf struct from memory

 @param pix pointer to hocr_pixbuf struct.
 @return 1
 */
int hocr_pixbuf_unref (hocr_pixbuf * pix);

/**
 @brief create an object map for a given image

 @param pix pointer to hocr_pixbuf struct.
 */
int
hocr_pixbuf_create_object_map (hocr_pixbuf * pix);

#endif
