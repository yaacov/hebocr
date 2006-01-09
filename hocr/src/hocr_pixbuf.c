
/***************************************************************************
 *            hocr_pixbuf.c
 *
 *  Sat Dec 10 22:11:22 2005
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "consts.h"
#include "hocr_pixbuf.h"
#include "hocr_object.h"

/* 
 internal line_eq stractures 
 */

int
hocr_line_eq_get_y (hocr_line_eq line, int x)
{
	return (int) (line.a * (double) x + line.b);
}

/* 
 internal pixbuf stractures 
 */

int
hocr_pixbuf_get_n_channels (hocr_pixbuf * pix)
{
	return pix->n_channels;
}

int
hocr_pixbuf_get_height (hocr_pixbuf * pix)
{
	return pix->height;
}

int
hocr_pixbuf_get_width (hocr_pixbuf * pix)
{
	return pix->width;
}

int
hocr_pixbuf_get_rowstride (hocr_pixbuf * pix)
{
	return pix->rowstride;
}

int
hocr_pixbuf_get_brightness (hocr_pixbuf * pix)
{
	return pix->brightness;
}

unsigned char *
hocr_pixbuf_get_pixels (hocr_pixbuf * pix)
{
	return pix->pixels;
}

int
hocr_pixbuf_get_pixel (hocr_pixbuf * pix, int x, int y)
{
	unsigned char *pixel;

	if (x < 0 || x >= pix->width || y < 0 || y >= pix->height)
		return 0;

	pixel = pix->pixels + x * pix->n_channels + y * pix->rowstride;

	return (pixel[0] < pix->brightness) ? 1 : 0;
}

int
hocr_pixbuf_set_pixel (hocr_pixbuf * pix, int x, int y, int channel, int value)
{
	unsigned char *pixel;

	if (x < 0 || x >= pix->width || y < 0 || y >= pix->height)
		return 0;

	pixel = pix->pixels + x * pix->n_channels + y * pix->rowstride;
	pixel[channel] = value;

	return (pixel[channel] < pix->brightness) ? 1 : 0;
}

unsigned int
hocr_pixbuf_get_object (hocr_pixbuf * pix, int x, int y)
{
	unsigned int object_numebr;
	int rowstride = pix->rowstride / pix->n_channels;

	if (x < 0 || x >= pix->width || y < 0 || y >= pix->height)
		return 0;

	/* get the object number at x,y */
	object_numebr = pix->object_map[x + y * rowstride];

	return pix->objects[object_numebr].name;
}

unsigned int
hocr_pixbuf_set_object (hocr_pixbuf * pix, int x, int y,
			unsigned int object_number)
{

	int rowstride = pix->rowstride / pix->n_channels;

	if (x < 0 || x >= pix->width || y < 0 || y >= pix->height ||
	    object_number >= MAX_OBJECTS_IN_PAGE)
		return 0;

	pix->object_map[x + y * rowstride] = object_number;

	/* set reference count for the object */
	if (object_number)
	{
		pix->objects[object_number].weight =
			pix->objects[object_number].weight + 1;

		if (pix->objects[object_number].x1 > x)
			pix->objects[object_number].x1 = x;
		if (pix->objects[object_number].x2 < x)
			pix->objects[object_number].x2 = x;

		if (pix->objects[object_number].y1 > y)
			pix->objects[object_number].y1 = y;
		if (pix->objects[object_number].y2 < y)
			pix->objects[object_number].y2 = y;
	}

	return object_number;
}

/* hocr_pixbuf file utils */

unsigned char
hocr_pbm_getc (FILE * file)
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
hocr_pbm_getint (FILE * file)
{
	unsigned char ch;
	int i = 0;

	do
	{
		ch = hocr_pbm_getc (file);
	}
	while (ch == ' ' || ch == '\n' || ch == '\t');

	do
	{
		i = (i * 10) + (ch - '0');
		ch = hocr_pbm_getc (file);
	}
	while (ch >= '0' && ch <= '9');

	return i;
}

int
hocr_pbm_getbit (FILE * file, int gray_scale)
{

	static unsigned char byte = 0;
	static unsigned char mask = 0;
	int return_bit;

	if (gray_scale)
	{
		return_bit = getc (file);
	}
	else
	{
		if (mask == 0)
		{
			mask = 0x80;
			byte = getc (file);
		}

		return_bit = (byte & mask) ? 0 : 255;

		mask >>= 1;
	}

	return return_bit;
}

hocr_pixbuf *
hocr_pixbuf_new_from_file (const char *filename)
{
	int x, y;
	unsigned char *pixel;
	int bit_read;
	char char_read;
	hocr_pixbuf *new_pixbuf;
	FILE *file = NULL;
	int gray_scale = FALSE;
	int dippnes = 1;

	/* if no input file name use stdin for input */
	if (!filename || filename[0] == '\0')
	{
		file = stdin;
	}
	else
	{
		/* open file */
		file = fopen (filename, "r");
		if (!file)
			return NULL;
	}

	/* allocate memory for pixbuf */
	new_pixbuf = (hocr_pixbuf *) malloc (sizeof (hocr_pixbuf));
	if (!new_pixbuf)
	{
		if (file)
		{
			fclose (file);
		}
		return NULL;
	}

	/* read magic number "P4" for pbm file */
	char_read = hocr_pbm_getc (file);
	if (char_read != 'P')
	{
		if (file)
		{
			fclose (file);
		}
		return NULL;
	}
	char_read = hocr_pbm_getc (file);
	if (char_read == '4')
		gray_scale = FALSE;
	else if (char_read == '5')
		gray_scale = TRUE;
	else
	{
		if (file)
		{
			fclose (file);
		}
		return NULL;
	}

	/* read header */
	new_pixbuf->n_channels = 3;
	new_pixbuf->brightness = 100;
	new_pixbuf->pixels = NULL;
	new_pixbuf->object_map = NULL;
	new_pixbuf->objects = NULL;

	/* read width and height */
	new_pixbuf->width = hocr_pbm_getint (file);
	new_pixbuf->height = hocr_pbm_getint (file);
	new_pixbuf->rowstride = new_pixbuf->width * 3;

	/* read gray_scale dippnes */
	if (gray_scale)
		dippnes = hocr_pbm_getint (file);
	if (dippnes > 255)
	{
		if (file)
		{
			fclose (file);
		}
		return NULL;
	}

	/* allocate memory for data */
	new_pixbuf->pixels =
		malloc (new_pixbuf->height * new_pixbuf->rowstride);
	if (!(new_pixbuf->pixels))
	{
		if (file)
		{
			fclose (file);
		}
		return NULL;
	}

	/* read data */
	for (y = 0; y < new_pixbuf->height; y++)
	{
		for (x = 0; x < new_pixbuf->width; x++)
		{
			bit_read = hocr_pbm_getbit (file, gray_scale);
			pixel = new_pixbuf->pixels +
				x * new_pixbuf->n_channels +
				y * new_pixbuf->rowstride;
			pixel[0] = bit_read;
			pixel[1] = bit_read;
			pixel[2] = bit_read;
		}
	}

	/* if regular file - close it */
	if (file)
	{
		fclose (file);
	}

	/* return the new pixbuf to user */
	return new_pixbuf;
}

/**
 @brif writes hocr_pixbuf to ppm or pgm file

 @param pixbuf hocr_pixbuf 8 or 24 bpp
 @param filenme save as file name 
 @return 1=ok, 0=error
 */
int
hocr_pixbuf_save_as_pnm (hocr_pixbuf * pixbuf, char *filename)
{
	FILE *fp;

	fp = fopen (filename, "wb");

	if (!fp)
		return 0;

	if (pixbuf->n_channels == 3)
	{
		/* 
		 * write raw ppm
		 * ppm header: P6 <width> <height> <maxval> 
		 */
		fprintf (fp, "P6 %d %d 255\n", pixbuf->width, pixbuf->height);
	}
	else
	{
		/* 
		 * write raw pgm (assume 8 bits per pixel - one channel)
		 * 1 bpp is not supported!!
		 * pgm header: P5 <width> <height> <maxval> 
		 */
		fprintf (fp, "P5 %d %d 255\n", pixbuf->width, pixbuf->height);
	}

	/* this might be a huge write... */
	fwrite (pixbuf->pixels, 1, pixbuf->height * pixbuf->rowstride, fp);

	fclose (fp);

	return 1;
}

hocr_pixbuf *
hocr_pixbuf_new (void)
{
	hocr_pixbuf *new_pixbuf;

	/* allocate memory for pixbuf */
	new_pixbuf = (hocr_pixbuf *) malloc (sizeof (hocr_pixbuf));
	if (!new_pixbuf)
		return NULL;

	/* read header */
	new_pixbuf->n_channels = 3;
	new_pixbuf->brightness = 100;
	new_pixbuf->pixels = NULL;
	new_pixbuf->object_map = NULL;
	new_pixbuf->objects = NULL;

	/* return the new pixbuf to user */
	return new_pixbuf;
}

int
hocr_pixbuf_clear_object_map (hocr_pixbuf * pix)
{
	int x, y;
	unsigned int i;
	int rowstride = pix->rowstride / pix->n_channels;

	/* clear object map */
	for (y = 0; y < pix->height; y++)
	{
		for (x = 0; x < pix->width; x++)
		{
			pix->object_map[x + y * rowstride] = 0;
		}
	}

	/* clear object list */
	for (i = 0; i < MAX_OBJECTS_IN_PAGE; i++)
	{
		pix->objects[i].weight = 0;
		pix->objects[i].name = i;

		pix->objects[i].x1 = pix->width;
		pix->objects[i].y1 = pix->height;
		pix->objects[i].x2 = 0;
		pix->objects[i].y2 = 0;
		pix->objects[i].hight = 0;
		pix->objects[i].width = 0;
	}

	return 0;
}

int
hocr_pixbuf_get_first_free_object (hocr_pixbuf * pix)
{
	int i;

	/* look for first empty slot in object list (0 is none object) */
	for (i = 1; i < MAX_OBJECTS_IN_PAGE; i++)
	{
		if (pix->objects[i].weight == 0)
			return i;
	}

	return 0;
}

int
hocr_pixbuf_count_objects (hocr_pixbuf * pix)
{
	int i, counter = 0;

	/* look for first empty slot in object list (0 is none object) */
	for (i = 1; i < MAX_OBJECTS_IN_PAGE; i++)
	{
		if (pix->objects[i].weight > MIN_OBJECT_WEIGHT)
			counter++;
	}

	return counter;
}

unsigned int
hocr_pixbuf_merge_objects (hocr_pixbuf * pix, unsigned int object1,
			   unsigned int object2)
{
	unsigned int temp_object;

	/* object1 is the smaller */
	if (object1 > object2)
	{
		temp_object = object1;
		object1 = object2;
		object2 = temp_object;
	}

	/* copy x1,y1 and x2,y2 to object1 */
	pix->objects[object1].x1 =
		(pix->objects[object2].x1 <
		 pix->objects[object1].x1) ? pix->objects[object2].x1 : pix->
		objects[object1].x1;
	pix->objects[object1].y1 =
		(pix->objects[object2].y1 <
		 pix->objects[object1].y1) ? pix->objects[object2].y1 : pix->
		objects[object1].y1;

	pix->objects[object1].x2 =
		(pix->objects[object2].x2 >
		 pix->objects[object1].x2) ? pix->objects[object2].x2 : pix->
		objects[object1].x2;
	pix->objects[object1].y2 =
		(pix->objects[object2].y2 >
		 pix->objects[object1].y2) ? pix->objects[object2].y2 : pix->
		objects[object1].y2;

	/* add weight of object2 to objec1 */
	pix->objects[object1].weight += pix->objects[object2].weight;

	/* set object2 name to be object1 */
	pix->objects[object2].name = object1;

	return object1;
}

int
hocr_pixbuf_create_object_map (hocr_pixbuf * pix)
{
	int i, x, y;
	unsigned int object, object1, object2, object3, object4;
	int rowstride = pix->rowstride / pix->n_channels;

	/* check for already allocated objects */
	if (pix->object_map || pix->objects)
		return -1;

	/* allocate memory for object_map */
	pix->object_map =
		malloc (pix->height * rowstride * sizeof (unsigned int));
	if (!(pix->object_map))
		return -1;

	/* allocate memory for objects list */
	pix->objects = malloc (MAX_OBJECTS_IN_PAGE * sizeof (hocr_object));
	if (!(pix->objects))
		return -1;

	/* clear the objects list */
	hocr_pixbuf_clear_object_map (pix);

	/* fill the object map */
	for (y = 1; y < pix->height; y++)
	{
		for (x = 1; x < pix->width; x++)
		{
			/* progress the progress indicator */
			pix->progress =
				((double) y * pix->width +
				 (double) x) / ((double) pix->height *
						(double) pix->width) * 127.0;
			/* if this is part of an object */
			if (hocr_pixbuf_get_pixel (pix, x, y) == 1)
			{
				/* look at neigbors */
				object1 =
					pix->objects[pix->
						     object_map[x - 1 +
								y *
								rowstride]].
					name;
				object2 =
					pix->objects[pix->
						     object_map[x +
								(y -
								 1) *
								rowstride]].
					name;
				object3 =
					pix->objects[pix->
						     object_map[x - 1 +
								(y -
								 1) *
								rowstride]].
					name;
				object4 =
					pix->objects[pix->
						     object_map[x + 1 +
								(y -
								 1) *
								rowstride]].
					name;
				/* check that neigbors are from the same */
				if (object1 && object2 && object3
				    && (object1 != object2))
				{
					object = hocr_pixbuf_merge_objects (pix,
									    object1,
									    object2);
					object = hocr_pixbuf_merge_objects (pix,
									    object1,
									    object3);
					object1 = object;
					object2 = object;
					object3 = object;
				}
				else if (object1 && object2 && !object3
					 && (object1 != object2))
				{
					object = hocr_pixbuf_merge_objects (pix,
									    object1,
									    object2);
					object1 = object;
					object2 = object;
				}
				else if (object1 && !object2 && object3
					 && (object1 != object3))
				{
					object = hocr_pixbuf_merge_objects (pix,
									    object1,
									    object3);
					object1 = object;
					object3 = object;
				}
				else if (!object1 && object2 && !object3
					 && object4 && (object2 != object4))
				{
					object = hocr_pixbuf_merge_objects (pix,
									    object2,
									    object4);
					object2 = object;
					object4 = object;
				}
				else if (object1 && !object2 && !object3
					 && object4 && (object1 != object4))
				{
					object = hocr_pixbuf_merge_objects (pix,
									    object1,
									    object4);
					object1 = object;
					object4 = object;
				}

				/* fill the object map */
				if (!object1 && !object2 && !object3
				    && !object4)
				{
					object = hocr_pixbuf_get_first_free_object (pix);
					hocr_pixbuf_set_object (pix, x, y,
								object);
				}
				else if (object1)
				{
					hocr_pixbuf_set_object (pix, x, y,
								object1);
				}
				else if (object2)
				{
					hocr_pixbuf_set_object (pix, x, y,
								object2);
				}
				else if (object3)
				{
					hocr_pixbuf_set_object (pix, x, y,
								object3);
				}
				else if (object4)
				{
					hocr_pixbuf_set_object (pix, x, y,
								object4);
				}
			}
		}
	}

	/* remove duplicate objects */
	for (y = 0; y < pix->height; y++)
	{
		for (x = 0; x < pix->width; x++)
		{
			pix->object_map[x + y * rowstride] =
				pix->objects[pix->
					     object_map[x +
							y * rowstride]].name;
		}
	}

	/* set hight and width */
	for (i = 1; i < MAX_OBJECTS_IN_PAGE; i++)
	{
		if (i == pix->objects[i].name)
		{
			pix->objects[i].hight =
				pix->objects[i].y2 - pix->objects[i].y1;
			pix->objects[i].width =
				pix->objects[i].x2 - pix->objects[i].x1;
			if (pix->objects[i].weight < 6
			    || pix->objects[i].hight < 1
			    || pix->objects[i].width < 1)
				pix->objects[i].name = 0;
		}
		else
		{
			pix->objects[i].weight = 0;
		}
	}

	/* count objects (bigger then MIN_OBJECT_WEIGHT) in pixbuf */
	pix->num_of_objects = hocr_pixbuf_count_objects (pix);
	return 0;
}

int
hocr_pixbuf_clean (hocr_pixbuf * pix)
{
	int x, y;
	unsigned int object, object1, object2, object3,
		object4, object5, object6, object7, object8, object9;

	/* make shure null object has zero weight */
	pix->objects[0].weight = 0;

	/* check all pixes and clean objects that are two large or too small */
	for (y = 1; y < pix->height; y++)
	{
		for (x = 1; x < pix->width; x++)
		{
			/* if this is part of an object */
			if (pix->objects[hocr_pixbuf_get_object (pix, x, y)].
			    weight
			    && (pix->
				objects[hocr_pixbuf_get_object (pix, x, y)].
				weight < MIN_FONT_WEIGHT
				|| pix->
				objects[hocr_pixbuf_get_object (pix, x, y)].
				weight > MAX_FONT_WEIGHT))
			{
				/* clean */
				hocr_pixbuf_set_pixel (pix, x, y, 0, 0xffff);
				hocr_pixbuf_set_pixel (pix, x, y, 1, 0xffff);
				hocr_pixbuf_set_pixel (pix, x, y, 2, 0xffff);
			}

			/* get a 3x3 pixels */
			object1 = hocr_pixbuf_get_pixel (pix, x, y);
			object2 = hocr_pixbuf_get_pixel (pix, x, y + 1);
			object3 = hocr_pixbuf_get_pixel (pix, x, y + 2);

			object4 = hocr_pixbuf_get_pixel (pix, x + 1, y);
			object5 = hocr_pixbuf_get_pixel (pix, x + 1, y + 1);
			object6 = hocr_pixbuf_get_pixel (pix, x + 1, y + 2);

			object7 = hocr_pixbuf_get_pixel (pix, x + 2, y);
			object8 = hocr_pixbuf_get_pixel (pix, x + 2, y + 1);
			object9 = hocr_pixbuf_get_pixel (pix, x + 2, y + 2);

			/* remove dots */
			if (!object5)
			{

				if ((object1 && object2 && object3
				     && object4 && object6) ||
				    (object7 && object8 && object9
				     && object4 && object6) ||
				    (object1 && object4 && object7
				     && object2 && object8) ||
				    (object3 && object6 && object9
				     && object2 && object8))
				{
					object = hocr_pixbuf_get_object (pix,
									 x + 1,
									 y);
					if (!object)
						object = hocr_pixbuf_get_object
							(pix, x + 1, y + 2);

					hocr_pixbuf_set_pixel (pix, x + 1,
							       y + 1, 0, 0);
					hocr_pixbuf_set_pixel (pix, x + 1,
							       y + 1, 1, 0);
					hocr_pixbuf_set_pixel (pix, x + 1,
							       y + 1, 2, 0);
					hocr_pixbuf_set_object (pix, x + 1,
								y + 1, object);
				}
			}
			else
			{
				if ((!object1 && !object2 && !object3
				     && !object4 && !object6) ||
				    (!object7 && !object8 && !object9
				     && !object4 && !object6) ||
				    (!object1 && !object4 && !object7
				     && !object2 && !object8) ||
				    (!object3 && !object6 && !object9
				     && !object2 && !object8))
				{
					hocr_pixbuf_set_pixel (pix, x + 1,
							       y + 1, 0,
							       0xffff);
					hocr_pixbuf_set_pixel (pix, x + 1,
							       y + 1, 1,
							       0xffff);
					hocr_pixbuf_set_pixel (pix, x + 1,
							       y + 1, 2,
							       0xffff);
					hocr_pixbuf_set_object (pix, x + 1,
								y + 1, 0);
				}
			}
		}
	}

	return 0;
}

unsigned int
hocr_pixbuf_get_objects_in_box (hocr_pixbuf *
				pix, hocr_box box, unsigned int *object_array,
				int size)
{
	int x, y;
	int i = 0;
	unsigned int object = 0;
	unsigned int heviest_obj = 0;

	/* make sure none object have zero weight */
	pix->objects[0].weight = 0;
	clean_object_array (object_array, size);
	for (x = box.x1; x < box.x2; x++)
		for (y = box.y1; y < box.y2; y++)
		{
			if ((object =
			     hocr_pixbuf_get_object (pix, x, y)) &&
			    !(is_in_object_array
			      (object, object_array, size)) && (i < size))
			{
				/* add object to object array */
				object_array[i] = object;
				/* find the heviest object in the array */
				if (pix->objects[object].weight >
				    pix->objects[heviest_obj].weight)
					heviest_obj = object;
				i++;
			}
		}

	return heviest_obj;
}

unsigned int
hocr_pixbuf_get_objects_inside_box (hocr_pixbuf
				    * pix,
				    hocr_box box, unsigned int *object_array,
				    int size)
{
	int x, y;
	int i = 0;
	unsigned int object = 0;
	unsigned int heviest_obj = 0;

	/* make sure none object have zero weight */
	pix->objects[0].weight = 0;
	clean_object_array (object_array, size);
	for (x = box.x1; x < box.x2; x++)
		for (y = box.y1; y < box.y2; y++)
		{
			if ((object =
			     hocr_pixbuf_get_object (pix, x, y)) &&
			    pix->objects[object].x1 >= box.x1 &&
			    pix->objects[object].x2 <= box.x2 &&
			    pix->objects[object].y1 >= box.y1 &&
			    pix->objects[object].y2 <= box.y2 &&
			    !(is_in_object_array
			      (object, object_array, size)) && (i < size))
			{
				/* add object to object array */
				object_array[i] = object;
				/* find the heviest object in the array */
				if (pix->objects[object].weight >
				    pix->objects[heviest_obj].weight)
					heviest_obj = object;
				i++;
			}
		}

	return heviest_obj;
}

int
hocr_pixbuf_unref (hocr_pixbuf * pix)
{
	if (pix->pixels)
		free (pix->pixels);
	if (pix->object_map)
		free (pix->object_map);
	if (pix->objects)
		free (pix->objects);
	if (pix)
		free (pix);
	return 1;
}
