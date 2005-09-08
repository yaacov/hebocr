/***************************************************************************
 *            font_layout.c
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

#include "hocr.h"
#include "magic_numbers.h"
#include "font_layout.h"

int
find_font_baseline_eq (hocr_box line, hocr_box * fonts,
		       hocr_line_eq * base_line, hocr_line_eq * top_line,
		       int avg_font_hight, int num_of_fonts)
{
	int i;
	int counter;
	int start_counter;
	int y_start_base, y_end_base;
	int x_start, x_end;
	int y_start_top, y_end_top;

	/* if no fonts then just return the line */
	if (num_of_fonts == 0)
	{
		base_line->a = 0;
		base_line->b = line.y2;
		top_line->a = 0;
		top_line->b = line.y1;
		return 1;
	}

	/* avg over first NUM_OF_FONTS_TO_AVG fonts */
	y_start_base = 0;
	y_start_top = 0;
	x_start = 0;
	y_end_base = 0;
	y_end_top = 0;
	x_end = 0;

	counter = 0;
	start_counter = 0;
	i = 0;
	while (i < num_of_fonts)
	{
		if (fonts[i].hight <
		    ((1000 + FONT_ASSEND) * avg_font_hight / 1000)
		    && fonts[i].hight >
		    ((1000 - FONT_ASSEND) * avg_font_hight / 1000))
		{
			/* take only first NUM_OF_FONTS_TO_AVG to avg */
			if (counter < NUM_OF_FONTS_TO_AVG)
			{
				y_start_base += fonts[i].y2;
				x_start += fonts[i].x2;
				y_start_top += fonts[i].y1;
				start_counter ++;
			}
			
			/* check how many regular fonts in this line */
			counter++;
		}
		i++;
	}

	/* if can't any font use the first font for horizontal lines */
	if (start_counter == 0)
	{
		base_line->a = 0;
		base_line->b = fonts[0].y2;
		top_line->a = 0;
		top_line->b = fonts[0].y1;

		return 1;
	}

	/* if here then counter is not zero */
	y_start_base /= start_counter;
	y_start_top /= start_counter;
	x_start /= start_counter;

	/* if can't find NUM_OF_FONTS_TO_AVG regular fonts at start and
	   NUM_OF_FONTS_TO_AVG at end use the horizontal lines */
	if (counter < 2 * NUM_OF_FONTS_TO_AVG)
	{
		base_line->a = 0;
		base_line->b = y_start_base;
		top_line->a = 0;
		top_line->b = y_start_top;

		return 1;
	}

	/* avg over last NUM_OF_FONTS_TO_AVG fonts */
	/* FIXME: can I put this in the first loop */
	counter = 0;
	i = num_of_fonts - 1;
	while (counter < NUM_OF_FONTS_TO_AVG && i >= 0)
	{
		if (fonts[i].hight <
		    ((1000 + FONT_ASSEND) * avg_font_hight / 1000)
		    && fonts[i].hight >
		    ((1000 - FONT_ASSEND) * avg_font_hight / 1000))
		{
			y_end_base += fonts[i].y2;
			y_end_top += fonts[i].y1;
			x_end += fonts[i].x2;
			counter++;
		}
		i--;
	}

	/* if can't find NUM_OF_FONTS_TO_AVG use the horizontal lines */
	/* FIXME: we know we can, so why check again ? */
	if (counter < NUM_OF_FONTS_TO_AVG)
	{
		base_line->a = 0;
		base_line->b = y_start_base;
		top_line->a = 0;
		top_line->b = y_start_top;
		
		return 1;
	}
	
	y_end_base /= counter;
	y_end_top /= counter;
	x_end /= counter;

	/* delta x is small return horizontal line */
	/* FIXME: we know it is not, so why check again ? */
	if ((x_end - x_start) == 0)
	{
		base_line->a = 0;
		base_line->b = y_start_base;
		top_line->a = 0;
		top_line->b = y_start_top;
		
		return 1;
	}

	/* make line equation */
	base_line->a =
		(double) (y_end_base -
			  y_start_base) / (double) (x_end - x_start);

	top_line->a =
		(double) (y_end_top -
			  y_start_top) / (double) (x_end - x_start);

	/* FIXME: assume line is horizonatal and parallel ? */
	base_line->a =
		(base_line->a < top_line->a) ? base_line->a : top_line->a;
	top_line->a = base_line->a;

	base_line->b = y_start_base - base_line->a * x_start;
	top_line->b = y_start_top - top_line->a * x_start;

	return 0;
}

/*
 font position classes
 */

/* -1 assend 0 normal 1 sunk */
int
get_font_top_class (hocr_box font, hocr_line_eq top_line, int avg_font_hight)
{
	int assend = font.y1 - hocr_line_eq_get_y (top_line, font.x1);

	if (assend < (-FONT_ASSEND * avg_font_hight / 1000))
		return 1;
	if (assend > (FONT_ASSEND * avg_font_hight / 1000))
		return -1;

	return 0;
}

/* -1 assend 0 normal 1 sunk */
int
get_font_base_class (hocr_box font, hocr_line_eq base_line,
		     int avg_font_hight)
{
	int assend = font.y2 - hocr_line_eq_get_y (base_line, font.x1);

	if (assend < (-FONT_ASSEND * avg_font_hight / 1000))
		return -1;
	if (assend > (FONT_ASSEND * avg_font_hight / 1000))
		return 1;

	return 0;
}

/* -1 short 0 normal 1 ling */
int
get_font_hight_class (int font_hight, int avg_font_hight)
{
	if (font_hight < (SHORT_FONT * avg_font_hight / 1000))
		return -1;
	if (font_hight > (LONG_FONT * avg_font_hight / 1000))
		return 1;

	return 0;
}

/* -1 thin 0 normal 1 wide */
int
get_font_width_class (int font_width, int avg_font_width)
{
	if (font_width < THIN_FONT * avg_font_width / 1000)
		return -1;
	if (font_width > 2 * avg_font_width)
		return 1;

	return 0;
}
