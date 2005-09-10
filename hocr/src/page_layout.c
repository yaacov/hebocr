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
#include "magic_numbers.h"
#include "page_layout.h"

int
get_next_column_extention (hocr_pixbuf * pix, int current_pos,
			   int *column_start, int *column_end)
{
	int x, y, i;
	int sum;
	int inside_column = FALSE;

	int hight, hight_1_3, hight_2_3;

	*column_end = current_pos;
	*column_start = 0;

	hight = pix->height;
	hight_1_3 = hight / 3;
	hight_2_3 = 2 * hight / 3;

	for (x = current_pos - MIN_DISTANCE_BETWEEN_WORDS; x > 0;
	     x -= MIN_DISTANCE_BETWEEN_WORDS / 2)
	{
		/* get presentage coverage for this pixel line */
		sum = 0;
		for (y = 0; y < hight_1_3; y++)
		{
			for (i = 0; i < 2 * MIN_DISTANCE_BETWEEN_WORDS; i++)
			{
				sum += hocr_pixbuf_get_pixel (pix, x + i, y);
				sum += hocr_pixbuf_get_pixel (pix, x + i,
							      y + hight_1_3);
				sum += hocr_pixbuf_get_pixel (pix, x + i,
							      y + hight_2_3);
			}
		}

		/* if presantage above minmun for not in a line then we are in aline */
		if (!inside_column
		    && (x <= MIN_DISTANCE_BETWEEN_WORDS || sum > 0))
		{
			*column_end = x + MIN_DISTANCE_BETWEEN_WORDS;
			inside_column = TRUE;
		}
		/* if presantage below maximum for in a line then we need to find 
		 * the end of the line by looking to the end of the down slop */
		else if ((x <= MIN_DISTANCE_BETWEEN_WORDS || sum == 0) &&
			 inside_column)
		{
			*column_start = x;
			return 0;
		}
	}

	return 1;
}

int
get_next_line_extention (hocr_pixbuf * pix, hocr_box column, int current_pos,
			 int *line_start, int *line_end)
{
	int x, y;
	int last_raw_sum;
	int sum = 0;
	int inside_line = FALSE;

	int width, width_1_3, width_2_3;

	*line_end = column.y2;
	*line_start = current_pos;

	width = column.width;
	width_1_3 = width / 3;
	width_2_3 = 2 * width / 3;

	inside_line = FALSE;
	*line_start = current_pos;

	for (y = current_pos; y < column.y2; y++)
	{
		/* get presentage coverage for this pixel line */
		last_raw_sum = sum;
		sum = 0;
		for (x = 0; x < width_1_3; x++)
		{
			sum += hocr_pixbuf_get_pixel (pix, x, y);
			sum += hocr_pixbuf_get_pixel (pix, x + width_1_3, y);
			sum += hocr_pixbuf_get_pixel (pix, x + width_2_3, y);
		}

		/* check only the part with the most color on it */
		sum = 1000 * sum / width;

		/* if presantage below maximum for in a line then we need to find 
		 * the end of the line by looking to the end of the down slop */
		if (sum <= IN_A_LINE &&
		    inside_line &&
		    (y - *line_start) > MIN_LINE_HIGHT &&
		    (last_raw_sum - sum) <= 0)
		{
			*line_end = y;
			return 0;
		}

		/* if presantage above minmun for not in a line then we are in aline */
		if (sum >= NOT_IN_A_LINE && !inside_line)
		{
			*line_start = y;
			inside_line = TRUE;
		}
	}

	*line_end = column.y2;
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
			*font_start = x - 1;
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
	int found_nikud;

	/* adjust font hight and width */
	font->x2 += 1;
	font->x1 -= 0;
	font->width = font->x2 - font->x1;
	font->hight = font->y2 - font->y1;

	/* i do not think this is a font */
	if (font->hight > MAX_LINE_HIGHT || font->hight < MIN_LINE_HIGHT ||
	    font->width > NORMAL_FONT_WIDTH * 3
	    || font->width < MIN_FONT_SIZE)
		return 1;

	/* go down until found a font */
	sum = 0;
	for (y = font->y1; y < font->y2 && sum == 0; y++)
	{
		/* get presentage coverage for this pixel line */
		sum = 0;
		for (x = font->x1; x <= font->x2; x++)
		{
			sum += hocr_pixbuf_get_pixel (pix, x, y);
		}
	}

	/* go up until out of a font */
	sum = 1;
	for (; y > (font->y1 - font->hight) && sum != 0; y--)
	{
		/* get presentage coverage for this pixel line */
		sum = 0;
		for (x = font->x1; x <= font->x2; x++)
		{
			sum += hocr_pixbuf_get_pixel (pix, x, y);
		}
	}
	font->y1 = y + 2;

	/* go up until found a font */
	sum = 0;
	for (y = font->y2; y > font->y1 && sum == 0; y--)
	{
		/* get presentage coverage for this pixel line */
		sum = 0;
		for (x = font->x1; x <= font->x2; x++)
		{
			sum += hocr_pixbuf_get_pixel (pix, x, y);
		}
	}

	/* go down until out of a font */
	sum = 1;
	for (; y < (font->y2 + font->hight) && sum != 0; y++)
	{
		/* get presentage coverage for this pixel line */
		sum = 0;
		for (x = font->x1; x <= font->x2; x++)
		{
			sum += hocr_pixbuf_get_pixel (pix, x, y);
		}
	}
	
	font->y2 = y - 2;
	
	/* check for nikud under the font */
	found_nikud = TRUE;

	while (found_nikud)
	{
		sum = 1;
		/* read line from right to left */
		for (y = font->y2 - 1; y > font->y1 && sum != 0; y--)
		{
			/* get presentage coverage for this pixel line */
			sum = 0;
			for (x = font->x1; x <= font->x2; x++)
			{
				sum += hocr_pixbuf_get_pixel (pix, x, y);
			}
		}
		/* some times bet and caf look like resh with patach
		 * but patach is smaller ~3/4 than lower bar of kaf and bet */
		sum = 0;
		for (x = font->x1; x <= font->x2; x++)
		{
			sum += hocr_pixbuf_get_pixel (pix, x, font->y2 - 3);
		}

		found_nikud = (y > font->y1 + (font->hight / 2)
			       && sum < 4 * font->width / 5);

		if (found_nikud)
		{
			font->y2 = y;
		}
	}

	/* check for nikud above the font */
	found_nikud = TRUE;

	while (found_nikud)
	{
		sum = 1;
		/* read line from right to left */
		for (y = font->y1 + 1; y < font->y2 && sum != 0; y++)
		{
			/* get presentage coverage for this pixel line */
			sum = 0;
			for (x = font->x1; x <= font->x2; x++)
			{
				sum += hocr_pixbuf_get_pixel (pix, x, y);
			}
		}
		/* some times bet and caf look like resh with patach
		 * but patach is smaller ~3/4 than lower bar of kaf and bet */
		sum = 0;
		for (x = font->x1; x <= font->x2; x++)
		{
			sum += hocr_pixbuf_get_pixel (pix, x, font->y2 - 3);
		}

		found_nikud = (y < font->y2 - (font->hight / 2)
			       && sum < 4 * font->width / 5);

		if (found_nikud)
		{
			font->y1 = y;
		}
	}

	font->hight = font->y2 - font->y1;

	return 1;
}

int
adjust_line_hocr_box (hocr_pixbuf * pix, hocr_box column, hocr_box * line)
{
	int sum;
	int x, y;

	sum = 0;
	for (x = column.x1; x < column.x2 && sum == 0; x++)
	{
		/* get presentage coverage for this pixel line */
		sum = 0;
		for (y = line->y1; y < line->y2; y++)
		{
			sum += hocr_pixbuf_get_pixel (pix, x, y);
		}
	}
	line->x1 = x - 2;

	sum = 0;
	for (x = column.x2; x > column.x1 && sum == 0; x--)
	{
		/* get presentage coverage for this pixel line */
		sum = 0;
		for (y = line->y1; y < line->y2; y++)
		{
			sum += hocr_pixbuf_get_pixel (pix, x, y);
		}
	}
	line->x2 = x + 2;

	return 1;
}

int
fill_columns_array (hocr_pixbuf * pix, hocr_box * columns,
		    int *num_of_columns, int max_columns)
{
	/* for line detection */
	int column_start;
	int column_end;
	int return_value;
	int counter;

	counter = 0;

	/* get all lines in this column */
	return_value = get_next_column_extention
		(pix, pix->width, &column_start, &column_end);

	while (return_value == 0 && counter < max_columns)
	{
		/* a thin column is not a "real" column */
		if ((column_end - column_start) > MIN_COLUMN_WIDTH)
		{
			/* insert this line to lines array */
			columns[counter].y1 = 0;
			columns[counter].y2 = pix->height;
			columns[counter].x1 = column_start;
			columns[counter].x2 = column_end;
			columns[counter].width = (column_end - column_start);
			columns[counter].hight =
				(columns[counter].y2 - columns[counter].y1);

			counter++;
		}
		/* a thin column is probably a part of the last column */
		else
		{
			columns[counter].x1 = column_start;
			columns[counter].width = (column_end - column_start);
		}

		/* get some lee way from the end of last line */
		column_end += MIN_DISTANCE_BETWEEN_LINES;

		return_value = get_next_column_extention
			(pix, column_start, &column_start, &column_end);
	}

	*num_of_columns = counter;

	return 0;
}

int
fill_lines_array (hocr_pixbuf * pix, hocr_box column, hocr_box * lines,
		  int *num_of_lines, int max_lines)
{
	/* for line detection */
	int line_start;
	int line_end;
	int return_value;
	int counter;

	counter = 0;

	/* get all lines in this column */
	return_value = get_next_line_extention
		(pix, column, column.y1, &line_start, &line_end);

	while (return_value == 0 && counter < max_lines)
	{
		/* insert this line to lines array */
		lines[counter].y1 = line_start;
		lines[counter].y2 = line_end;
		lines[counter].hight = (line_end - line_start);

		adjust_line_hocr_box (pix, column, &(lines[counter]));

		lines[counter].width =
			(lines[counter].x2 - lines[counter].x1);

		counter++;

		/* get some lee way from the end of last line */
		line_end += MIN_DISTANCE_BETWEEN_LINES;

		return_value = get_next_line_extention
			(pix, column, line_end, &line_start, &line_end);
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

	while (return_value == 0 && font_start > line.x1
	       && counter < max_fonts)
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
