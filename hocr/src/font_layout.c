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
#include "consts.h"
#include "font_layout.h"

int
find_font_baseline_eq (hocr_box line, hocr_box * fonts,
		       hocr_line_eq * base_line, hocr_line_eq * top_line,
		       int avg_font_hight, int num_of_fonts)
{
	int i;
	int counter;
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
		return 0;
	}

	/* if short line just return horizonatal lines */
	y_start_base = 0;
	y_start_top = 0;
	for (i = 0; i < num_of_fonts; i++)
	{
		y_start_base += fonts[i].y2;
		y_start_top += fonts[i].y1;
	}
	base_line->a = 0;
	base_line->b = y_start_base / num_of_fonts;
	top_line->a = 0;
	top_line->b = y_start_top / num_of_fonts;

	/* avg over first NUM_OF_FONTS_TO_AVG fonts */
	y_start_base = 0;
	y_start_top = 0;
	x_start = 0;
	y_end_base = 0;
	y_end_top = 0;
	x_end = 0;

	counter = 0;
	i = 0;
	while (counter < NUM_OF_FONTS_TO_AVG && i < num_of_fonts)
	{
		if (fonts[i].hight <
		    ((1000 + FONT_ASSEND) * avg_font_hight / 1000)
		    && fonts[i].hight >
		    ((1000 - FONT_ASSEND) * avg_font_hight / 1000))
		{
			y_start_base += fonts[i].y2;
			x_start += fonts[i].x2;
			y_start_top += fonts[i].y1;
			counter++;
		}
		i++;
	}

	/* if can't find NUM_OF_FONTS_TO_AVG use the horizontal lines */
	if (counter < NUM_OF_FONTS_TO_AVG)
		return 0;

	y_start_base /= counter;
	y_start_top /= counter;
	x_start /= counter;

	/* avg over first NUM_OF_FONTS_TO_AVG fonts */
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
	if (counter < NUM_OF_FONTS_TO_AVG)
		return 0;

	y_end_base /= counter;
	y_end_top /= counter;
	x_end /= counter;

	/* do not divide by zero */
	if ((x_end - x_start) == 0)
		return 0;

	/* make line equation */
	base_line->a =
		(double) (y_end_base -
			  y_start_base) / (double) (x_end - x_start);
	base_line->b = y_start_base - base_line->a * x_start;

	top_line->a =
		(double) (y_end_top -
			  y_start_top) / (double) (x_end - x_start);
	top_line->b = y_start_top - top_line->a * x_start;

	return 0;
}

int
find_font_baseline (hocr_box * fonts, int avg_hight, int index,
		    int num_of_fonts)
{
	if (fonts[index].hight < 2 || fonts[index].width < 2)
		return 0;

	/* font in the middle of line */
	if (index > 1 && index < (num_of_fonts - 2))
	{
		/* is font on the left regular size */
		if (get_font_hight_class (fonts[index + 1].hight, avg_hight)
		    == 0)
			return fonts[index + 1].y2;
		/* is font on the right regular size */
		if (get_font_hight_class (fonts[index - 1].hight, avg_hight)
		    == 0)
			return fonts[index - 1].y2;
		/* is second font on the left regular size */
		if (get_font_hight_class (fonts[index + 2].hight, avg_hight)
		    == 0)
			return fonts[index + 2].y2;
		/* is second font on the right regular size */
		if (get_font_hight_class (fonts[index - 2].hight, avg_hight)
		    == 0)
			return fonts[index - 2].y2;
	}
	else
		/* is this the first or second font in the line */
	if (index < 2 && num_of_fonts > 3)
	{
		/* is font on the left regular size */
		if (get_font_hight_class (fonts[index + 1].hight, avg_hight)
		    == 0)
			return fonts[index + 1].y2;
		/* is second font on the left regular size */
		if (get_font_hight_class (fonts[index + 2].hight, avg_hight)
		    == 0)
			return fonts[index + 2].y2;
	}
	else
		/* is this the last font in the line */
	if (index > 1)
	{
		/* is font on the right regular size */
		if (get_font_hight_class (fonts[index - 1].hight, avg_hight)
		    == 0)
			return fonts[index - 1].y2;
		/* is second font on the right regular size */
		if (get_font_hight_class (fonts[index - 2].hight, avg_hight)
		    == 0)
			return fonts[index - 2].y2;
	}

	/* if no other regular font is near then base on yourself */
	return fonts[index].y2;
}

int
find_font_topline (hocr_box * fonts, int avg_hight, int index,
		   int num_of_fonts)
{
	if (fonts[index].hight < 2 || fonts[index].width < 2)
		return 0;

	/* font in the middle of line */
	if (index > 1 && index < (num_of_fonts - 2))
	{
		/* is font on the left regular size */
		if (get_font_hight_class (fonts[index + 1].hight, avg_hight)
		    == 0)
			return fonts[index + 1].y1;
		/* is font on the right regular size */
		if (get_font_hight_class (fonts[index - 1].hight, avg_hight)
		    == 0)
			return fonts[index - 1].y1;
		/* is second font on the left regular size */
		if (get_font_hight_class (fonts[index + 2].hight, avg_hight)
		    == 0)
			return fonts[index + 2].y1;
		/* is second font on the right regular size */
		if (get_font_hight_class (fonts[index - 2].hight, avg_hight)
		    == 0)
			return fonts[index - 2].y1;
	}
	else
		/* is this the first or second font in the line */
	if (index < 2 && num_of_fonts > 3)
	{
		/* is font on the left regular size */
		if (get_font_hight_class (fonts[index + 1].hight, avg_hight)
		    == 0)
			return fonts[index + 1].y1;
		/* is second font on the left regular size */
		if (get_font_hight_class (fonts[index + 2].hight, avg_hight)
		    == 0)
			return fonts[index + 2].y1;
	}
	else
		/* is this the last font in the line */
	if (index > 1)
	{
		/* is font on the right regular size */
		if (get_font_hight_class (fonts[index - 1].hight, avg_hight)
		    == 0)
			return fonts[index - 1].y1;
		/* is second font on the right regular size */
		if (get_font_hight_class (fonts[index - 2].hight, avg_hight)
		    == 0)
			return fonts[index - 2].y1;
	}


	/* if no other regular font is near then base on yourself */
	return fonts[index].y1;
}

/*
 font position classes
 */

/* -1 assend 0 normal 1 sunk */
int
get_font_top_class (int font_top, int font_topline, int avg_font_hight)
{
	int assend = font_topline - font_top;

	if (assend < (-FONT_ASSEND * avg_font_hight / 1000))
		return 1;
	if (assend > (FONT_ASSEND * avg_font_hight / 1000))
		return -1;

	return 0;
}

/* -1 assend 0 normal 1 sunk */
int
get_font_base_class (int font_bottom, int font_baseline, int avg_font_hight)
{
	int assend = font_baseline - font_bottom;

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
