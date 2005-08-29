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
		       int num_of_fonts)
{
	int number_of_fonts_to_avg = 8;

	int i;
	int counter;
	int y_start_base, y_end_base;
	int x_start_base, x_end_base;
	int y_start_top, y_end_top;
	int x_start_top, x_end_top;

	/* if short line assume horizonatal */
	if (num_of_fonts < number_of_fonts_to_avg)
	{
		base_line->a = 0;
		base_line->b = line.y2;
		top_line->a = 0;
		top_line->b = line.y1;
		return 1;
	}

	/* get avarage base of first 8 fonts */
	y_start_base = 0;
	x_start_base = 0;
	y_start_top = 0;
	x_start_top = 0;
	counter = 0;
	for (i = 0; i < number_of_fonts_to_avg; i++)
	{
		if (fonts[i].hight > MIN_LINE_HIGHT)
		{
			y_start_base += fonts[i].y2;
			x_start_base += fonts[i].x2;
			y_start_top += fonts[i].y1;
			x_start_top += fonts[i].x1;
			counter++;
		}
	}
	if (counter == 0)
	{
		base_line->a = 0;
		base_line->b = line.y2;
		top_line->a = 0;
		top_line->b = line.y1;
		return 1;
	}

	y_start_base = y_start_base / counter;
	x_start_base = x_start_base / counter;
	y_start_top = y_start_top / counter;
	x_start_top = x_start_top / counter;
	
	/* get avarage base of last 8 fonts */
	y_end_base = 0;
	x_end_base = 0;
	y_end_top = 0;
	x_end_top = 0;
	counter = 0;
	for (i = (num_of_fonts - number_of_fonts_to_avg); i < num_of_fonts;
	     i++)
	{
		if (fonts[i].hight > MIN_LINE_HIGHT)
		{
			y_end_base += fonts[i].y2;
			x_end_base += fonts[i].x2;
			y_end_top += fonts[i].y1;
			x_end_top += fonts[i].x1;
			counter++;
		}
	}
	if (counter == 0)
	{
		base_line->a = 0;
		base_line->b = line.y2;
		top_line->a = 0;
		top_line->b = line.y1;
		return 1;
	}

	y_end_base = y_end_base / counter;
	x_end_base = x_end_base / counter;
	y_end_top = y_end_top / counter;
	x_end_top = x_end_top / counter;

	/* if short line assume horizonatal */
	if ((x_start_base - y_end_base) < 30)
	{
		base_line->a = 0;
		base_line->b = line.y2;
		top_line->a = 0;
		top_line->b = line.y1;
		return 1;
	}

	base_line->a =
		(double) (y_end_base -
			  y_start_base) / (double) (y_end_base -
							 x_start_base);
	base_line->b = y_start_base - base_line->a * x_start_base;

	top_line->a =
		(double) (y_end_top -
			  y_start_top) / (double) (y_end_top -
							 x_start_top);
		top_line->b = y_start_top - top_line->a * x_start_top;
	
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
