
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
find_font_topline (hocr_box * fonts, int avg_hight, int index, int num_of_fonts)
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

int
find_font_baseline_eq (hocr_box line, hocr_box * fonts,
		       hocr_line_eq * base_line, hocr_line_eq * top_line,
		       int avg_font_hight, int num_of_fonts)
{
	int i;
	int start_counter, end_counter;
	int x_start, x_end;
	int y_start_down, y_end_down;
	int y_start_up, y_end_up;
	int font_hight;

	/* skip first two letters, they may be line numbering */
	int start_at = (num_of_fonts > NUM_OF_FONTS_TO_AVG * 4) ? 2 : 0;

	/* if not enogh fonts then just return the line */
	if (num_of_fonts < NUM_OF_FONTS_TO_AVG)
	{
		/* top line is more aqurate in most hebrew textst */
		base_line->a = 0;
		base_line->b = line.y1 + avg_font_hight + 1;
		top_line->a = 0;
		top_line->b = line.y1;
		return 1;
	}

	/* avg over NUM_OF_FONTS_TO_AVG fonts */
	y_start_up = 0;
	y_end_up = 0;
	y_start_down = 0;
	y_end_down = 0;
	x_start = 0;
	x_end = 0;
	font_hight = 0;

	start_counter = 0;
	end_counter = 0;
	/* skip first two letters, they may be line numbering */
	i = start_at;
	while (i < num_of_fonts)
	{
		if (fonts[i].hight <
		    ((1000 + 1.7 * FONT_ASSEND) * avg_font_hight / 1000)
		    && fonts[i].hight >
		    ((1000 - 1.5 * FONT_ASSEND) * avg_font_hight / 1000))
		{
			/* take only first NUM_OF_FONTS_TO_AVG to avg */
			if (start_counter < NUM_OF_FONTS_TO_AVG)
			{
				y_start_up += fonts[i].y1;
				y_start_down += fonts[i].y2;
				x_start += fonts[i].x2;
				font_hight += fonts[i].hight;
				start_counter++;
			}
		}
		i++;
	}

	/* skip first two letters, they may be line numbering */
	i = num_of_fonts - start_at;
	while (i > 0)
	{
		i--;
		if (fonts[i].hight <
		    ((1000 + 1.7 * FONT_ASSEND) * avg_font_hight / 1000)
		    && fonts[i].hight >
		    ((1000 - 1.5 * FONT_ASSEND) * avg_font_hight / 1000))
		{
			/* take only last NUM_OF_FONTS_TO_AVG to avg */
			if (end_counter < NUM_OF_FONTS_TO_AVG)
			{
				y_end_up += fonts[i].y1;
				y_end_down += fonts[i].y2;
				x_end += fonts[i].x1;
				font_hight += fonts[i].hight;
				end_counter++;
			}
		}
	}

	/* if can't any font use the first font for horizontal lines */
	if (start_counter == 0 || end_counter == 0)
	{
		/* top line is more aqurate in most hebrew textst */
		base_line->a = 0;
		base_line->b = line.y1 + avg_font_hight + 1;
		top_line->a = 0;
		top_line->b = line.y1;
		return 1;
	}

	/* if here then counter is not zero, i can avarage */
	y_start_up /= start_counter;
	y_start_down /= start_counter;
	x_start /= start_counter;
	y_end_up /= end_counter;
	y_end_down /= end_counter;
	x_end /= end_counter;
	font_hight /= (end_counter + start_counter);

	/* delta x is small return horizontal line */
	if ((x_end - x_start) == 0)
	{
		/* top line is more aqurate in most hebrew textst */
		base_line->a = 0;
		base_line->b = line.y1 + font_hight + 1;
		top_line->a = 0;
		top_line->b = line.y1;
		return 1;
	}

	/* make line equation (x_end - x_start) is not zero */
	top_line->a = (double) (y_end_up - y_start_up) / (double) (x_end - x_start);
	base_line->a = (double) (y_end_down - y_start_down) / (double) (x_end - x_start);
	
	top_line->b = y_start_up - top_line->a * x_start;
	base_line->b = y_start_down - base_line->a * x_start + 1;

	return 0;
}
