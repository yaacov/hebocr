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

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef TRUE
#define TRUE -1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/** line equation structore used by hocr
 
 a and b in the equation are Y = a X + b
 */
typedef struct
{
	double a;
	double b;
} hocr_line_eq;

typedef enum
{
	HOCR_ERROR_OK = 0,
	HOCR_ERROR_NO_LINES_FOUND = 1,
	HOCR_ERROR_NO_FONTS_FOUND = 2,
	HOCR_ERROR_OUT_OF_MEMORY = 4,
	HOCR_ERROR_NOT_HORIZONTAL_LINE = 8
} hocr_error;

typedef enum
{
	HOCR_OCR_TYPE_REGULAR = 0,
	HOCR_OCR_TYPE_COLUMNS = 1,
	HOCR_OCR_TYPE_NIKUD = 2,
	HOCR_OCR_TYPE_TABLE = 4,
} hocr_ocr_type;

typedef enum
{
	HOCR_OUTPUT_JUST_OCR = 0,
	HOCR_OUTPUT_WITH_GRAPHICS = 1,
	HOCR_OUTPUT_WITH_DEBUG_TEXT = 2,
} hocr_output;

typedef struct
{
	char page_start_string[50];
	char page_end_string[50];
	char column_start_string[50];
	char column_end_string[50];
	char paragraph_start_string[50];
	char paragraph_end_string[50];
	char line_start_string[50];
	char line_end_string[50];
	char indent_string[50];


} hocr_format_strings;

typedef struct
{
	int x1;
	int y1;
	int x2;
	int y2;
	int hight;
	int width;
} hocr_box;

/** text buffer structore used by hocr
 
 This struct contain dynamicaly allocated string, You MUST
 use hocr_text_buffer_new and hocr_text_buffer_unref functions 
 to create this string and nuref it.
*/
typedef struct
{
	/** get the text buffer. */
	char *text;
	/** get number of chars in the text string. */
	int size;
	/** get number of chars allocated in memory. */
	int allocated_size;
} hocr_text_buffer;

/**
 @brief creats a new hocr_text_buffer struct from file 

 @return pointer to a newly allocate hocr_text_buffer, or null if can not open file.
 */
hocr_text_buffer *hocr_text_buffer_new ();

/**
 @brief free a hocr_text_buffer struct from memory

 @param text_buffer pointer to hocr_text_buffer struct.
 @return 1
 */
int hocr_text_buffer_unref (hocr_text_buffer * text_buffer);

/**
 @brief add a string to hocr_text_buffer struct

 @param text_buffer pointer to hocr_text_buffer struct.
 @param new_text pointer to the string to be added to the buffer.
 @return new size of text
 */
int hocr_text_buffer_add_string (hocr_text_buffer * text_buffer,
				 const char *new_text);

/** pixbuf structore used by hocr
	
	hocr_pixbuf can be used with common image structores,
	see this examples for use with gtk and qt toolkits -
				 
	how to use with gdk_pixbuf:
				 
    hocr_pix.n_channels = gdk_pixbuf_get_n_channels (pixbuf);
	hocr_pix.height = gdk_pixbuf_get_height (pixbuf);
	hocr_pix.width = gdk_pixbuf_get_width (pixbuf);
	hocr_pix.rowstride = gdk_pixbuf_get_rowstride (pixbuf);
	hocr_pix.pixels = (unsigned char*)(gdk_pixbuf_get_pixels (pixbuf));
	hocr_pix.brightness = 100;
				 
	how to use with QImage:

	hocr_pix.pixels = img.bits ();
	hocr_pix.height = img.size ().height ();
	hocr_pix.width = img.size ().width ();
	hocr_pix.rowstride = img.bytesPerLine ();
	hocr_pix.n_channels = img.depth () / 8;
	hocr_pix.brightness = 100;
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
} hocr_pixbuf;

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
int hocr_pixbuf_set_pixel (hocr_pixbuf * pix, int x, int y,
			   int channel, int value);

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
 @brief do ocr on a hocr_pixbuf and return the result text to text_buffer

 @param pix pointer to hocr_pixbuf struct.
 @param text_buffer pointer to an already allocated hocr_text_buffer for the results
 @param error pointer to hocr_error enum that will return exit status.
 @return 1
 */
int hocr_do_ocr (hocr_pixbuf * pix, hocr_text_buffer * text_buffer,
		 hocr_format_strings * user_format_strings,
		 hocr_output out_flags, hocr_ocr_type ocr_type,
		 hocr_error * error);

#ifdef __cplusplus
}
#endif

#endif
