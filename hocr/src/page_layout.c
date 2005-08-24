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
	int x, y;
	int last_raw_sum;
	int sum = 0;
	int inside_line = FALSE;

	int width, width_1_3, width_2_3;
	
	*line_end = 0;
	*line_start = current_pos;

	width = pix->width;
	width_1_3 = width / 3;
	width_2_3 = 2 * width / 3;
	
	for (y = current_pos; y < pix->height; y++)
	{
		/* get presentage coverage for this pixel line */
		last_raw_sum = sum;
		sum = 0;
		for (x = 0; x < width_1_3; x++)
		{
			sum += hocr_pixbuf_get_pixel (pix, x, y);
			sum += hocr_pixbuf_get_pixel (pix,
						       x + width_1_3, y);
			sum += hocr_pixbuf_get_pixel (pix,
						       x + width_2_3,
						       y);
		}
		
		/* check only the part with the most color on it */
		sum = 1000 * sum / width;

		/* if presantage above minmun for not in a line then we are in aline */
		if (sum >= NOT_IN_A_LINE && !inside_line)
		{
			*line_start = y;
			inside_line = TRUE;
		}
		/* if presantage below maximum for in a line then we need to find 
		   the end of the line by looking to the end of the down slop */
		else if (sum <= IN_A_LINE && 
				 inside_line && 
				 (y - *line_start) > MIN_LINE_HIGHT &&
				 (last_raw_sum - sum) <= 0)
		{
			*line_end = y;
			
			/* if here and this line has logical width then found a new line */
			/* FIXME: do we want to read BIG fonts ? 
			          see consts.h for details about MAX_LINE_HIGHT */
			if ((*line_end - *line_start) > MAX_LINE_HIGHT)
				return 1;
			
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
adjust_font_hocr_box (hocr_pixbuf * pix, hocr_box * font)
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
adjust_line_hocr_box (hocr_pixbuf * pix, hocr_box * line)
{
	/* TODO: make this more intelegent */
	line->x1 = 0;
	line->x2 = pix->width;

	return 1;
}

int
fill_lines_array (hocr_pixbuf * pix, hocr_box column, hocr_box * lines,
		  int *num_of_lines, int max_lines)
{
	/* FIXME: column is just a place holder, it does nothing now ! */

	/* for line detection */
	int line_start;
	int line_end;
	int return_value;
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

		adjust_line_hocr_box (pix, &(lines[counter]));

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
fill_fonts_array (hocr_pixbuf * pix, hocr_box line, hocr_box * fonts,
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
		adjust_font_hocr_box (pix, &(fonts[counter]));
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
