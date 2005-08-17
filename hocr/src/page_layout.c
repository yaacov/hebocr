/***************************************************************************
 *            page_layout.c
 *
 *  Mon Aug 15 13:33:16 2005
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

#include "hocr.h"
#include "consts.h"
#include "page_layout.h"

/* TODO: add support for more then one squre qulumn */

int
get_next_line_extention (hocr_pixbuf * pix, int current_pos, int *line_start,
			 int *line_end)
{
	int i, x, y;
	double sum, sum1, sum2, sum3;
	int inside_line = FALSE;

	*line_end = 0;
	*line_start = current_pos;

	for (y = current_pos; y < pix->height; y++)
	{
		/* get presentage coverage for this pixel line */
		/* a line is too long for just one sume (it may be short {1/3 length} 
		 * and aligned to center, right or left ) */
		sum1 = sum2 = sum3 = 0;
		for (x = 0; x < (pix->width / 3); x++)
		{
			sum1 += hocr_pixbuf_get_pixel (pix, x, y);
			sum2 += hocr_pixbuf_get_pixel (pix,
						       x + pix->width / 3, y);
			sum3 += hocr_pixbuf_get_pixel (pix,
						       x + 2 * pix->width / 3,
						       y);
		}
		/* check only the part with the most color on it */
		sum = (sum1 > sum2) ? sum1 : sum2;
		sum = (sum > sum3) ? sum : sum3;
		sum = 1000 * sum / pix->width;

		/* if presantage covarage is less then 1 we are between text lines */
		if (sum >= NOT_IN_A_LINE && !inside_line)
		{
			*line_start = y;
			inside_line = TRUE;
		}
		else if (sum <= NOT_IN_A_LINE && inside_line)
		{
			*line_end = y;
			/* if here and this line has logical width then found a new line */
			if ((*line_end - *line_start) > MAX_LINE_HIGHT)
				return 1;
			if ((*line_end - *line_start) > MIN_LINE_HIGHT)
				return 0;
		}
	}
	return 1;
}

int
get_next_font_extention (hocr_pixbuf * pix, int line_start, int line_end,
			 int current_pos, int *font_start, int *font_end)
{
	int x, y;
	int sum;
	int inside_font = FALSE;
	/* we have to calculate line hight, we do not get it from caller */
	int line_hight = line_end - line_start;

	/* read line from right to left */
	for (x = current_pos - 1; x > 0; x--)
	{
		/* get presentage coverage for this pixel line */
		sum = 0;
		for (y = line_start; y < line_end; y++)
		{
			sum += hocr_pixbuf_get_pixel (pix, x, y);
		}
		sum = 1000 * sum / line_hight;

		/* if presantage covarage is less then 1 we are between text fonts */
		if (sum >= NOT_IN_A_FONT && !inside_font)
		{
			*font_start = x;
			inside_font = TRUE;
		}
		else if (sum <= NOT_IN_A_FONT && inside_font)
		{
			*font_end = x + 1;

			/* if here then found a new line */
			return 0;
		}
	}

	return 1;
}

int
adjust_font_box (hocr_pixbuf * pix, box * font)
{
	int x, y;
	int sum;

	sum = 0;
	/* read line from right to left */
	for (y = (font->y1 - MIN_DISTANCE_BETWEEN_LINES);
	     y < (font->y2 + MIN_DISTANCE_BETWEEN_LINES) && sum == 0; y++)
	{
		/* get presentage coverage for this pixel line */
		sum = 0;
		for (x = font->x1; x < font->x2; x++)
		{
			sum += hocr_pixbuf_get_pixel (pix, x, y);
		}
	}
	font->y1 = y - 1;

	sum = 0;

	/* read line from right to left */
	for (y = (font->y2 + MIN_DISTANCE_BETWEEN_LINES);
	     y > (font->y1 - MIN_DISTANCE_BETWEEN_LINES) && sum == 0; y--)
	{
		/* get presentage coverage for this pixel line */
		sum = 0;
		for (x = font->x1; x < font->x2; x++)
		{
			sum += hocr_pixbuf_get_pixel (pix, x, y);
		}
	}
	font->y2 = y + 1;

	return 1;
}

int
adjust_line_box (hocr_pixbuf * pix, box * line)
{
	int x, y;
	int sum;

	/* TODO: make this more intelegent */
	line->x1 = 0;
	line->x2 = pix->width;

	return 1;
}

int
fill_lines_array (hocr_pixbuf * pix, box column, box * lines,
		  int *num_of_lines, int max_lines)
{
	/* FIXME: column is just a place holder, it does nothing now ! */

	/* for line detection */
	int line_start;
	int line_end;
	int return_value;
	int width;
	int counter;

	counter = 0;

	/* get all lines in this column */
	return_value = get_next_line_extention
		(pix, 0, &line_start, &line_end);

	while (return_value == 0 && counter < max_lines)
	{
		/* insert this line to lines array */
		lines[counter].y1 = line_start;
		lines[counter].y2 = line_end;
		lines[counter].hight = (line_end - line_start);

		adjust_line_box (pix, &(lines[counter]));

		lines[counter].width =
			(lines[counter].x2 - lines[counter].x1);

		counter++;

		/* get some lee way from the end of last line */
		line_end += MIN_DISTANCE_BETWEEN_LINES;

		return_value = get_next_line_extention
			(pix, line_end, &line_start, &line_end);
	}

	*num_of_lines = counter;

	return 0;
}

int
fill_fonts_array (hocr_pixbuf * pix, box line, box * fonts,
		  int *num_of_fonts, int max_fonts)
{
	/* for gliphs detection */
	int font_start;
	int font_end;
	int return_value;
	int counter;

	counter = 0;

	/* get all lines in this column */
	return_value = get_next_font_extention (pix,
						line.y1,
						line.y2,
						line.x2,
						&font_start, &font_end);

	while (return_value == 0 && counter < max_fonts)
	{
		/* insert this font to fonts array */
		fonts[counter].x1 = font_end;	/* this is right to left sweep */
		fonts[counter].x2 = font_start;
		fonts[counter].y1 = line.y1;
		fonts[counter].y2 = line.y2;
		fonts[counter].width = (font_start - font_end);

		/* adjust font hight top and bottom borders */
		adjust_font_box (pix, &(fonts[counter]));
		fonts[counter].hight = fonts[counter].y2 - fonts[counter].y1;

		counter++;

		return_value = get_next_font_extention (pix,
							line.y1,
							line.y2,
							font_end,
							&font_start,
							&font_end);
	}

	*num_of_fonts = counter;

	return 0;
}
