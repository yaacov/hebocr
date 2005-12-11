
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
#include "font_recognition.h"
#include "hocr.h"

/* 
 visualization helper finction
 */

int
print_font (hocr_pixbuf * pix, hocr_box font)
{
	int x, y;
	int new_color;

	for (x = font.x1; x <= font.x2; x++)
	{
		printf ("---");
	}
	printf ("\n");

	for (y = font.y1; y <= font.y2; y++)
	{
		for (x = font.x1; x <= font.x2; x++)
		{
			if (new_color = hocr_pixbuf_get_object (pix, x, y))
				printf ("%3d", new_color);
			else
				printf ("   ");
		}
		printf ("\n");
	}

	for (x = font.x1; x <= font.x2; x++)
	{
		printf ("---");
	}
	printf ("\n");

	printf ("\n");

	return 0;
}

int
color_hocr_box_full (hocr_pixbuf * pix, hocr_box rect, int chanell, int value,
		     int only_main_object)
{
	int x, y;
	unsigned int obj = 0;
	unsigned int object_array[MAX_OBJECTS_IN_FONT];

	if (only_main_object)
	{
		obj = hocr_pixbuf_get_objects_in_box (pix, rect, object_array);
	}

	for (y = rect.y1 + 1; y <= (rect.y2 - 1); y++)
		for (x = rect.x1 + 1; x <= (rect.x2 - 1); x++)
		{
			if (!obj || obj == hocr_pixbuf_get_object (pix, x, y))
				hocr_pixbuf_set_pixel (pix, x, y, chanell,
						       value);
		}

	return 0;
}

int
color_hocr_box (hocr_pixbuf * pix, hocr_box rect, int chanell, int value)
{
	int x, y;

	for (y = rect.y1; y <= rect.y2; y++)
	{
		hocr_pixbuf_set_pixel (pix, rect.x1, y, chanell, value);
		hocr_pixbuf_set_pixel (pix, rect.x2, y, chanell, value);
	}

	for (x = rect.x1; x <= rect.x2; x++)
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
hocr_do_ocr (hocr_pixbuf * pix, hocr_text_buffer * text_buffer)
{
	hocr_box columns[MAX_COLUMNS];
	hocr_box lines[MAX_COLUMNS][MAX_LINES];
	hocr_line_eq line_eqs[MAX_COLUMNS][MAX_LINES][2];
	hocr_box fonts[MAX_COLUMNS][MAX_LINES][MAX_FONTS_IN_LINE];

	int num_of_fonts[MAX_COLUMNS][MAX_LINES];
	int num_of_lines[MAX_COLUMNS];

	int num_of_columns_in_page = 0;
	int num_of_lines_in_page = 0;
	int num_of_fonts_in_page = 0;
	int num_of_regular_fonts_in_page = 0;

	int avg_line_hight_in_page = 0;
	int avg_line_x_start_in_column[MAX_COLUMNS];
	int avg_diff_between_lines_in_page = 0;
	int avg_diff_between_fonts_in_page = 0;
	int avg_font_width_in_page = 0;
	int avg_font_hight_in_page = 0;
	int avg_regular_font_width_in_page = 0;
	int avg_regular_font_hight_in_page = 0;

	int c;
	int i, j, k;
	int y1, y2;

	/* need this to put in the text_buffer */
	char chars[10];
	int end_of_word;

	/* create and fill the object map */
	hocr_pixbuf_create_object_map (pix);

	/* get columns for this page */
	fill_columns_array (pix, columns, &num_of_columns_in_page, MAX_COLUMNS);

	/* get lines in this column */
	num_of_lines_in_page = 0;

	for (c = 0; c < num_of_columns_in_page; c++)
	{
		num_of_lines[c] = 0;

		fill_lines_array (pix, columns[c], lines[c],
				  &(num_of_lines[c]), MAX_LINES);
		num_of_lines_in_page += num_of_lines[c];
	}

	/* get avg_line_hight_in_page */
	avg_line_hight_in_page = 0;
	avg_diff_between_lines_in_page = 0;

	for (c = 0; c < num_of_columns_in_page; c++)
	{
		avg_line_x_start_in_column[c] = 0;

		for (i = 0; i < num_of_lines[c]; i++)
		{
			avg_line_x_start_in_column[c] += lines[c][i].x2;
			avg_line_hight_in_page += lines[c][i].hight;
			if (i < (num_of_lines[c] - 1))
				avg_diff_between_lines_in_page +=
					(lines[c][i + 1].y1 - lines[c][i].y2);
		}

		if (num_of_lines[c] != 0)
		{
			avg_line_x_start_in_column[c] =
				avg_line_x_start_in_column[c] / num_of_lines[c];
		}
	}

	avg_line_hight_in_page = avg_line_hight_in_page / num_of_lines_in_page;
	avg_diff_between_lines_in_page =
		avg_diff_between_lines_in_page /
		(num_of_lines_in_page - num_of_columns_in_page + 1);

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
	num_of_regular_fonts_in_page = 0;
	avg_font_hight_in_page = 0;
	avg_font_width_in_page = 0;
	avg_diff_between_fonts_in_page = 0;

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

					num_of_regular_fonts_in_page++;
					avg_font_width_in_page +=
						fonts[c][i][j].width;
					avg_font_hight_in_page +=
						fonts[c][i][j].hight;
					if (j < (num_of_fonts[c][i] - 1))
					{
						avg_diff_between_fonts_in_page
							+=
							(fonts[c][i][j].x1 -
							 fonts[c][i][(j +
								      1)].x2);
					}
				}
			}
		}
	}

	if (num_of_regular_fonts_in_page != 0)
	{
		avg_font_width_in_page /= num_of_regular_fonts_in_page;
		avg_font_hight_in_page /= num_of_regular_fonts_in_page;

		if (num_of_regular_fonts_in_page != 1)
			avg_diff_between_fonts_in_page /=
				(num_of_regular_fonts_in_page - 1);
	}

	/* avg over regular fonts only to get better avg_font_hight_in_page */
	num_of_regular_fonts_in_page = 0;
	avg_regular_font_hight_in_page = 0;
	avg_regular_font_hight_in_page = 0;
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
					if (fonts[c][i][j].hight <
					    ((1000 +
					      FONT_ASSEND) *
					     avg_font_hight_in_page /
					     1000)
					    && fonts[c][i][j].hight >
					    ((1000 -
					      FONT_ASSEND) *
					     avg_font_hight_in_page / 1000))
					{
						num_of_regular_fonts_in_page++;
						avg_regular_font_width_in_page
							+= fonts[c][i][j].width;
						avg_regular_font_hight_in_page
							+= fonts[c][i][j].hight;
					}
				}
			}
		}
	}

	if (num_of_regular_fonts_in_page != 0)
	{
		avg_regular_font_width_in_page /= num_of_regular_fonts_in_page;
		avg_regular_font_hight_in_page /= num_of_regular_fonts_in_page;
	}

	/* get line equations for non horizontal lines */
	for (c = 0; c < num_of_columns_in_page; c++)
	{
		for (i = 0; i < num_of_lines[c]; i++)
		{
			if (num_of_fonts[c][i] == 0)
				continue;
			find_font_baseline_eq (lines[c]
					       [i],
					       fonts[c]
					       [i],
					       &
					       (line_eqs
						[c][i]
						[0]),
					       &
					       (line_eqs
						[c][i]
						[1]),
					       avg_regular_font_hight_in_page,
					       num_of_fonts[c][i]);
			/* if line is very not horizontal return error */
			if ((line_eqs[c][i][0].a *
			     line_eqs[c][i][0].a) > (1.0 / 10.0))
			{
				num_of_fonts[c][i] = 0;
			}

			/* if this line do not logical hight then dont parse it 
			 */
			if ((line_eqs[c][i][0].b -
			     line_eqs[c][i][1].b) <
			    (avg_font_hight_in_page -
			     MIN_DISTANCE_BETWEEN_LINES)
			    || (lines[c][i].y2 -
				lines[c][i].y1) > 2 * NORMAL_FONT_HIGHT)
			{
				num_of_fonts[c][i] = 0;
			}
		}
	}

	/* color the results of page layout functions */
	if (pix->command & HOCR_COMMAND_COLOR_BOXES)
	{
		for (c = 0; c < num_of_columns_in_page; c++)
		{
			color_hocr_box (pix, columns[c], 1, 0);
			for (i = 0; i < num_of_lines[c]; i++)
			{
				if (num_of_fonts[c][i] == 0)
					continue;
				/* color line boxes */
				color_hocr_line_eq (pix,
						    &(line_eqs
						      [c][i]
						      [0]),
						    lines[c]
						    [i].x1,
						    lines[c][i].x2, 2, 0);
				color_hocr_line_eq (pix,
						    &(line_eqs
						      [c][i]
						      [1]),
						    lines[c]
						    [i].x1,
						    lines[c][i].x2, 2, 100);
				/* color individual font boxes */
				for (j = 0; j < num_of_fonts[c][i]; j++)
				{
					/* if arteffact do not recognize */
					if (fonts[c][i][j].width < 3
					    || fonts[c][i][j].hight < 3)
						continue;
					if (fonts[c][i][j].width >
					    (3.5 *
					     (double)
					     avg_regular_font_width_in_page)
					    || fonts[c][i][j].hight >
					    (3.5 *
					     (double)
					     avg_regular_font_hight_in_page))
						continue;
					color_hocr_box (pix,
							fonts[c][i][j], 1, 0);
				}
			}
		}
	}

	/* do ocr ? */
	if ((pix->command & HOCR_COMMAND_OCR) == 0)
		return 0;
	/* get all fonts for all the lines */
	for (c = 0; c < num_of_columns_in_page; c++)
	{
		for (i = 0; i < num_of_lines[c]; i++)
		{
			for (j = 0; j < num_of_fonts[c][i]; j++)
			{
				/* check for end of word */
				end_of_word =
					((fonts[c][i][j].x1 -
					  fonts[c][i][(j +
						       1) %
						      num_of_fonts[c]
						      [i]].x2) >
					 MIN_DISTANCE_BETWEEN_WORDS);
				/* if arteffact do not recognize */
				if (fonts[c][i][j].width < 3
				    || fonts[c][i][j].hight < 3)
				{
					/* check for end of word */
					if (end_of_word)
						hocr_text_buffer_add_string
							(text_buffer, " ");
					continue;
				}

				if (fonts[c][i][j].width >
				    (3.5 *
				     (double)
				     avg_regular_font_width_in_page)
				    || fonts[c][i][j].hight >
				    (3.5 *
				     (double) avg_regular_font_hight_in_page))
				{
					/* check for end of word */
					if (end_of_word)
						hocr_text_buffer_add_string
							(text_buffer, " ");
					continue;
				}

				/* recognize font */
				hocr_recognize_font (pix,
						     fonts[c]
						     [i],
						     num_of_fonts
						     [c][i], j,
						     line_eqs
						     [c][i],
						     avg_regular_font_hight_in_page,
						     avg_regular_font_width_in_page,
						     chars);
				/* color unknown fonts in the pixbuf */
				if (chars[0] == '*'
				    && (pix->
					command & HOCR_COMMAND_COLOR_MISREAD))
					color_hocr_box_full (pix,
							     fonts[c]
							     [i][j], 1,
							     255, TRUE);
#define DEBUG
#ifdef DEBUG
				printf ("found font: %d,%d,%d - '%s'\n",
					c, i, j, chars);
				printf ("--------------------------\n\n",
					chars);
#endif
				/* add new recognizzed fonts to text */
				hocr_text_buffer_add_string
					(text_buffer, chars);
				/* check for end of word */
				if (end_of_word)
					hocr_text_buffer_add_string
						(text_buffer, " ");
			}

			/* end of line */
			hocr_text_buffer_add_string (text_buffer, "\n");
			/* check for end of paragraph */
			if (2 *
			    avg_diff_between_lines_in_page
			    < (lines[c][i + 1].y1 - lines[c][i + 0].y2))
				hocr_text_buffer_add_string (text_buffer, "\n");
		}

		/* end of column */
		hocr_text_buffer_add_string (text_buffer, "\n");
	}

	return 0;
}
