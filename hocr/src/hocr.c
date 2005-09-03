/***************************************************************************
 *            hocr.c
 *
 *  Fri Aug 12 20:18:56 2005
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "consts.h"
#include "page_layout.h"
#include "font_layout.h"
#include "font_markers.h"
#include "hocr.h"

/* 
 internal line_eq stractures 
 */

int
hocr_line_eq_get_y (hocr_line_eq line, int x)
{
	return (int) (line.a * (double) x + line.b);
}

/* 
 internal text_buffer stractures 
 */

hocr_text_buffer *
hocr_text_buffer_new ()
{
	hocr_text_buffer *new_text_buffer;

	/* allocate memory for pixbuf */
	new_text_buffer =
		(hocr_text_buffer *) malloc (sizeof (hocr_text_buffer));

	if (!new_text_buffer)
		return 0;

	new_text_buffer->size = 0;
	new_text_buffer->allocated_size = MEMORY_CHANK_FOR_TEXT_BUFFER;

	new_text_buffer->text =
		(char *) malloc (sizeof (char) *
				 new_text_buffer->allocated_size);

	if (!new_text_buffer->text)
	{
		free (new_text_buffer);
		return 0;
	}

	(new_text_buffer->text)[0] = '\0';

	return new_text_buffer;
}

int
hocr_text_buffer_unref (hocr_text_buffer * text_buffer)
{
	if (text_buffer)
		free (text_buffer);

	return 1;
}

int
hocr_text_buffer_add_string (hocr_text_buffer * text_buffer,
			     const char *new_text)
{
	int len;
	char *new_allocated_text;

	len = strlen (new_text);

	/* check for allocated space and try to get more memory */
	if ((text_buffer->size + len) >= text_buffer->allocated_size)
	{
		new_allocated_text =
			realloc (text_buffer->text, sizeof (char) *
				 (text_buffer->allocated_size +
				  MEMORY_CHANK_FOR_TEXT_BUFFER));

		if (new_allocated_text)
		{
			text_buffer->text = new_allocated_text;
			text_buffer->allocated_size +=
				MEMORY_CHANK_FOR_TEXT_BUFFER;
		}
		else
		{
			return HOCR_ERROR_OUT_OF_MEMORY;
		}
	}

	strcat (text_buffer->text, new_text);
	text_buffer->size = strlen (text_buffer->text);

	return HOCR_ERROR_OK;
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

	if (x < 0 || x > pix->width || y < 0 || y > pix->height)
		return 0;

	pixel = pix->pixels + x * pix->n_channels + y * pix->rowstride;

	return (pixel[0] < pix->brightness) ? 1 : 0;
}

int
hocr_pixbuf_set_pixel (hocr_pixbuf * pix, int x, int y, int channel,
		       int value)
{
	unsigned char *pixel;

	if (x < 0 || x > pix->width || y < 0 || y > pix->height)
		return 0;

	pixel = pix->pixels + x * pix->n_channels + y * pix->rowstride;
	pixel[channel] = value;

	return (pixel[channel] < pix->brightness) ? 1 : 0;
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

	/* open file */
	file = fopen (filename, "r");
	if (!file)
		return NULL;

	/* allocate memory for pixbuf */
	new_pixbuf = (hocr_pixbuf *) malloc (sizeof (hocr_pixbuf));
	if (!new_pixbuf)
		return NULL;

	/* read magic number "P4" for pbm file */
	char_read = hocr_pbm_getc (file);
	if (char_read != 'P')
		return NULL;
	char_read = hocr_pbm_getc (file);
	if (char_read == '4')
		gray_scale = FALSE;
	else if (char_read == '5')
		gray_scale = TRUE;
	else
		return NULL;

	/* read header */
	new_pixbuf->n_channels = 3;
	new_pixbuf->brightness = 100;
	new_pixbuf->pixels = NULL;

	/* read width and height */
	new_pixbuf->width = hocr_pbm_getint (file);
	new_pixbuf->height = hocr_pbm_getint (file);
	new_pixbuf->rowstride = new_pixbuf->width * 3;

	/* read gray_scale dippnes */
	if (gray_scale)
		dippnes = hocr_pbm_getint (file);
	if (dippnes > 255)
		return NULL;

	/* allocate memory for data */
	new_pixbuf->pixels =
		malloc (new_pixbuf->height * new_pixbuf->rowstride);
	if (!(new_pixbuf->pixels))
		return NULL;

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

	/* return the new pixbuf to user */
	return new_pixbuf;
}

int
hocr_pixbuf_unref (hocr_pixbuf * pix)
{
	if (pix->pixels)
		free (pix->pixels);

	if (pix)
		free (pix);

	return 1;
}

/* 
 visualization helper finction
 */

int
print_font (hocr_pixbuf * pix, hocr_box font)
{
	int x, y;
	int new_color;

	for (y = font.y1; y < (font.y2 + 0); y++)
	{
		for (x = font.x1; x < (font.x2 + 1); x++)
		{
			new_color = hocr_pixbuf_get_pixel (pix, x, y);
			printf ("%d", new_color);
		}
		printf ("\n");
	}

	printf ("\n");

	return 0;
}

int
color_hocr_box_full (hocr_pixbuf * pix, hocr_box rect, int chanell, int value)
{
	int x, y;

	for (y = rect.y1; y < rect.y2; y++)
		for (x = rect.x1; x < rect.x2; x++)
		{
			hocr_pixbuf_set_pixel (pix, x, y, chanell, value);
		}

	return 0;
}

int
color_hocr_box (hocr_pixbuf * pix, hocr_box rect, int chanell, int value)
{
	int x, y;

	for (y = rect.y1; y < rect.y2; y++)
	{
		hocr_pixbuf_set_pixel (pix, rect.x1, y, chanell, value);
		hocr_pixbuf_set_pixel (pix, rect.x2, y, chanell, value);
	}

	for (x = rect.x1; x < rect.x2; x++)
	{
		hocr_pixbuf_set_pixel (pix, x, rect.y1, chanell, value);
		hocr_pixbuf_set_pixel (pix, x, rect.y2, chanell, value);
	}

	return 0;
}

int
color_hocr_line_eq (hocr_pixbuf * pix, hocr_line_eq * line, int x1, int x2,
		    int chanell, int value)
{
	int x, y;

	for (x = x1; x < x2; x++)
	{
		y = hocr_line_eq_get_y (*line, x);
		hocr_pixbuf_set_pixel (pix, x, y, chanell, value);
	}

	return 0;
}

/*
 */

int
hocr_do_ocr (hocr_pixbuf * pix, hocr_text_buffer * text_buffer,
	     hocr_output out_flags, hocr_error * error)
{
	hocr_box columns[MAX_COLUMNS];
	hocr_box lines[MAX_COLUMNS][MAX_LINES];
	hocr_line_eq line_eqs[MAX_COLUMNS][MAX_LINES][2];
	hocr_box fonts[MAX_COLUMNS][MAX_LINES][MAX_FONTS_IN_LINE];

	int num_of_fonts[MAX_COLUMNS][MAX_LINES];
	int num_of_lines[MAX_COLUMNS];

	int num_of_columns_in_page;
	int num_of_lines_in_page;
	int num_of_fonts_in_page;

	int avg_font_hight_in_page;
	int avg_line_hight_in_page;
	int avg_font_width_in_page;

	int c;
	int i, j, k;
	int y1, y2;

	/* font position classes */
	int base_class;
	int top_class;
	int hight_class;
	int width_class;
	int end_of_line;
	int end_of_word;
	int add_space;
	int end_of_paragraph;
	int last_was_quat;

	/* FIXME: what size is the new string to add ? */
	char chars[MAX_NUM_OF_CHARS_IN_FONT];

	int marks[25];

	/* init the error to OK */
	if (error)
		*error = HOCR_ERROR_OK;

	/* page layout recognition */

	/* get all columns in this page */
	fill_columns_array (pix, columns, &num_of_columns_in_page,
			    MAX_COLUMNS);

	/* get all lines in this column */
	num_of_lines_in_page = 0;
	for (c = 0; c < num_of_columns_in_page; c++)
	{
		num_of_lines[c] = 0;
		fill_lines_array (pix, columns[c], lines[c],
				  &(num_of_lines[c]), MAX_LINES);
		num_of_lines_in_page += num_of_lines[c];
	}

	if (num_of_lines_in_page == 0)
	{
		/* is it O.K. to have no lines in the page ? */
		if (error)
			*error = *error | HOCR_ERROR_NOT_HORIZONTAL_LINE;
		return 1;
	}

	/* get avg_line_hight_in_page */
	avg_line_hight_in_page = 0;
	for (c = 0; c < num_of_columns_in_page; c++)
	{
		for (i = 0; i < num_of_lines[c]; i++)
		{
			avg_line_hight_in_page += lines[c][i].hight;
		}
	}
	avg_line_hight_in_page =
		avg_line_hight_in_page / num_of_lines_in_page;

	/* get all fonts for all the lines */
	for (c = 0; c < num_of_columns_in_page; c++)
	{
		for (i = 0; i < num_of_lines[c]; i++)
		{
			fill_fonts_array (pix, lines[c][i],
					  fonts[c][i],
					  &(num_of_fonts[c][i]),
					  MAX_FONTS_IN_LINE);
		}
	}

	/* get size statistics for all fonts for all the lines */
	num_of_fonts_in_page = 0;
	avg_font_hight_in_page = 0;
	avg_font_width_in_page = 0;
	for (c = 0; c < num_of_columns_in_page; c++)
	{
		for (i = 0; i < num_of_lines[c]; i++)
		{
			if (lines[c][i].hight >
			    (avg_line_hight_in_page -
			     1.5 * MIN_DISTANCE_BETWEEN_LINES))
			{
				for (j = 0; j < num_of_fonts[c][i]; j++)
				{

					num_of_fonts_in_page++;
					avg_font_width_in_page +=
						fonts[c][i][j].width;
					avg_font_hight_in_page +=
						fonts[c][i][j].hight;

				}
			}
		}
	}

	if (num_of_fonts_in_page != 0)
	{
		avg_font_width_in_page /= num_of_fonts_in_page;
		avg_font_hight_in_page /= num_of_fonts_in_page;
	}
	else
	{
		/* is it O.K. to have no fonts in the page ? */
		if (error)
			*error = *error | HOCR_ERROR_NOT_HORIZONTAL_LINE;
		return 1;
	}

	/* get lines equations for non horizontal lines */
	for (c = 0; c < num_of_columns_in_page; c++)
	{
		for (i = 0; i < num_of_lines[c]; i++)
		{
			if (num_of_fonts[c][i] == 0)
				continue;

			find_font_baseline_eq (lines[c][i], fonts[c][i],
					       &(line_eqs[c][i][0]),
					       &(line_eqs[c][i][1]),
					       avg_font_hight_in_page,
					       num_of_fonts[c][i]);

			/* if line is very not horizontal return error */
			if ((line_eqs[c][i][0].a * line_eqs[c][i][0].a) >
			    (1.0 / 9.0))
			{
				if (error)
					*error = *error |
						HOCR_ERROR_NOT_HORIZONTAL_LINE;

				num_of_fonts[c][i] = 0;
			}

			/* if this line is not high it is nikud line */
			if ((line_eqs[c][i][0].b - line_eqs[c][i][1].b) <
			    (avg_font_hight_in_page -
			     MIN_DISTANCE_BETWEEN_LINES))
			{
				num_of_fonts[c][i] = 0;
			}
		}
	}

	/* color the results of page layout functions */
	if (out_flags & HOCR_OUTPUT_WITH_GRAPHICS)
		for (c = 0; c < num_of_columns_in_page; c++)
		{
			for (i = 0; i < num_of_lines[c]; i++)
			{
				if (num_of_fonts[c][i] == 0)
					continue;

				/* color line boxes */
				color_hocr_line_eq (pix, &(line_eqs[c][i][0]),
						    lines[c][i].x1,
						    lines[c][i].x2, 2, 0);
				color_hocr_line_eq (pix, &(line_eqs[c][i][1]),
						    lines[c][i].x1,
						    lines[c][i].x2, 2, 100);

				/* color individual font boxes */
				for (j = 0; j < num_of_fonts[c][i]; j++)
				{
					color_hocr_box (pix, fonts[c][i][j],
							1, 0);
				}
			}
		}

	/* page layout is complite start of font recognition */

	/* get all you know of each font */
	for (c = 0; c < num_of_columns_in_page; c++)
	{
		for (i = 0; i < num_of_lines[c]; i++)
		{
			for (j = 0; j < num_of_fonts[c][i]; j++)
			{
				/* get font position above/below line */
				base_class =
					get_font_base_class (fonts[c][i][j],
							     line_eqs[c][i]
							     [0],
							     avg_font_hight_in_page);
				top_class =
					get_font_top_class (fonts[c][i][j],
							    line_eqs[c][i][1],
							    avg_font_hight_in_page);

				/* get font x, y size compared to other fonts in page */
				hight_class =
					get_font_hight_class (fonts[c][i][j].
							      hight,
							      avg_font_hight_in_page);
				width_class =
					get_font_width_class (fonts[c][i][j].
							      width,
							      avg_font_width_in_page);

				/* get font position in word, e.g. is last
				 * or is before non letter (psik, nekuda ...) */
				end_of_line = (j + 1) == num_of_fonts[c][i];
				add_space = !end_of_line
					&& (fonts[c][i][j].x1 -
					    fonts[c][i][j + 1].x2) >
					MIN_DISTANCE_BETWEEN_WORDS;
				/* FIXME: !(top_class == -1) only covers words that end
				 * with ",.-" but what if word ends with "?!:" ... ? */
				end_of_word = ((end_of_line || add_space)
					       && !(top_class == -1))
					||
					(get_font_top_class
					 (fonts[c][i][j + 1],
					  line_eqs[c][i][1],
					  avg_font_hight_in_page) == -1);

			/**
			 */

				/* get font markers */
				hocr_guess_font (pix, fonts[c][i][j],
						 base_class,
						 hocr_line_eq_get_y (line_eqs
								     [c][i]
								     [0],
								     fonts[c]
								     [i][j].
								     x2),
						 hocr_line_eq_get_y (line_eqs
								     [c][i]
								     [1],
								     fonts[c]
								     [i][j].
								     x1),
						 top_class, hight_class,
						 width_class, end_of_word,
						 chars,
						 MAX_NUM_OF_CHARS_IN_FONT);

				/* if chars is ' wait to the next char and if it is ' too
				 * add " once */
				if (chars[0] == '\'' && chars[1] == 0)
				{
					if (last_was_quat)
					{
						sprintf (chars, "\"");
						last_was_quat = FALSE;

					}
					else
					{
						sprintf (chars, "");
						last_was_quat = TRUE;
					}
				}
				else
				{
					if (last_was_quat)
					{
						/* output ' to text buffer, stop if out of memory for the text buffer */
						if (hocr_text_buffer_add_string (text_buffer, "\'") == HOCR_ERROR_OUT_OF_MEMORY)
						{
							if (error)
								*error = HOCR_ERROR_OUT_OF_MEMORY;
						}
					}

					last_was_quat = FALSE;
				}

				/* if no font dont try to insert it */
				if (chars[0])
					/* output font to text buffer, stop if out of memory for the text buffer */
					if (hocr_text_buffer_add_string
					    (text_buffer,
					     chars) ==
					    HOCR_ERROR_OUT_OF_MEMORY)
					{
						if (error)
							*error = HOCR_ERROR_OUT_OF_MEMORY;
					}

				/* if user want printout print all you know about this char */
				if (out_flags & HOCR_OUTPUT_WITH_DEBUG_TEXT)
				{
					printf ("Font %d %d %d\n", c, i, j);

					/* print the font, this take a lot of time, remove if not needed */
					print_font (pix, fonts[c][i][j]);

					printf ("base class %d, top class %d\n", base_class, top_class);

					/* print out font x, y size compared to other fonts in page */
					printf ("hight class %d, width class %d\n", hight_class, width_class);

					/* print out font position in word, e.g. is last
					 * or is before non letter (psik, nekuda ...) */
					printf ("end of line %d, add space %d, end of word %d\n", end_of_line, add_space, end_of_word);

					/* print marks */
					print_marks (pix, fonts[c][i][j],
						     marks);

					printf ("font markers: %d%d %d%d %d%d\n", marks[1], marks[2], marks[3], marks[4], marks[5], marks[6]);

					/* print out end of font */
					printf ("font is %s\n", chars);

					printf ("=======================\n");
				}

				/* color unknown fonts in the pixbuf */
				if (chars[0] == '_')
					color_hocr_box_full (pix,
							     fonts[c][i][j],
							     1, 255);

				/* check for end of word and end of line */
				if (add_space)
				{
					hocr_text_buffer_add_string
						(text_buffer, " ");
				}
				if (end_of_line)
				{
					hocr_text_buffer_add_string
						(text_buffer, "\n");
				}
			}
		}
	}

	return 0;		/* the ocr thing need rewriting just leave it for now */
}
