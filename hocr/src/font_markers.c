/***************************************************************************
 *            font_markers.c
 *
 *  Fri Aug 12 20:39:03 2005
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

/* 
 font markers
 */

#include <string.h>
#include <stdio.h>

#include "hocr.h"
#include "consts.h"
#include "font_markers.h"

/**
 */

int
has_black_right_bottom_mark (hocr_pixbuf * pix, hocr_box font)
{
	int x, y;
	int sum;

	sum = 0;
	/* check a 6*6 triangle */
	for (x = font.x2; x > (font.x2 - 2); x--)
		for (y = font.y2; y > (font.y2 - (x - (font.x1 - 2))); y--)
		{
			sum += hocr_pixbuf_get_pixel (pix, x, y);
		}

	if (sum < 3)
		return 0;

	return 1;
}

int
has_black_left_bottom_mark (hocr_pixbuf * pix, hocr_box font)
{
	int x, y;
	int sum;

	/* look at bottom right */
	sum = 0;
	/* check a 6*6 triangle */
	for (x = font.x1; x < (font.x1 + 4); x++)
		for (y = font.y2; y > (font.y2 - ((font.x1 + 4) - x)); y--)
		{
			sum += hocr_pixbuf_get_pixel (pix, x, y);
		}

	if (sum == 0)
		return 0;
	return 1;
}

int
has_black_left_top_mark (hocr_pixbuf * pix, hocr_box font)
{
	int x, y;
	int sum;

	/* look at bottom right */
	sum = 0;
	/* check a 6*6 triangle */
	for (x = font.x1; x < (font.x1 + 4); x++)
		for (y = font.y1; y < (font.y2 + ((font.x1 + 4) - x)); y++)
		{
			sum += hocr_pixbuf_get_pixel (pix, x, y);
		}

	if (sum == 0)
		return 0;
	return 1;
}

int
has_black_right_top_mark (hocr_pixbuf * pix, hocr_box font)
{
	int x, y;
	int sum;

	sum = 0;

	/* check a 6*6 triangle */
	for (x = font.x2; x > (font.x2 - 4); x--)
		for (y = font.y1; y < (font.y2 + (x - (font.x2 - 4))); y++)
		{
			sum += hocr_pixbuf_get_pixel (pix, x, y);
		}

	if (sum == 0)
		return 0;

	return 1;
}

int
count_vertical_bars (hocr_pixbuf * pix, hocr_box font, int y_pos,
		     int *first_x, int *last_x)
{
	int x, y;
	int current_color;
	int new_color;
	int counter = 0;

	/* look at middle of font */
	y = y_pos;

	/* start with white in mind */
	current_color = 0;
	*first_x = 0;
	*last_x = 0;

	/* go axros the font + one white place */
	for (x = font.x1; x < (font.x2 + 4); x++)
	{
		new_color = hocr_pixbuf_get_pixel (pix, x, y);

		if (new_color == 1 && *first_x == 0)
			*first_x = x;

		/* if exiting a line then count one more line */
		if (new_color == 0 && current_color == 1)
		{
			counter++;
			*last_x = x;
		}
		current_color = new_color;
	}

	return counter;
}

int
count_horizontal_bars (hocr_pixbuf * pix, hocr_box font, int x_pos,
		       int *first_y, int *last_y)
{
	int x, y;
	int current_color;
	int new_color;
	int counter = 0;

	/* look at middle of font */
	x = x_pos;

	/* start with white in mind */
	current_color = 0;
	*first_y = 0;
	*last_y = 0;

	/* go axros the font + one white place */
	for (y = font.y1; y < (font.y2 + 3); y++)
	{
		new_color = hocr_pixbuf_get_pixel (pix, x, y);

		if (new_color == 1 && *first_y == 0)
			*first_y = y;

		/* if exiting a line then count one more line */
		if (new_color == 0 && current_color == 1)
		{
			counter++;
			*last_y = y;
		}
		current_color = new_color;
	}

	return counter;
}

int
is_empty (hocr_pixbuf * pix, int x1, int y1, int x2, int y2)
{
	int x, y;
	int sum = 0;

	/* check for black pixels */
	for (x = x1; x < x2; x++)
		for (y = y1; y < y2; y++)
		{
			sum += hocr_pixbuf_get_pixel (pix, x, y);
		}

	return (sum == 0) ? 1 : 0;
}

int
find_horizontal_path (hocr_pixbuf * pix, int x1, int y1, int x2, int y2)
{
	int x, y;
	int sum;

	for (x = x1; x < x2; x++)
	{
		sum = 0;

		for (y = y1; y < y2; y++)
		{
			sum += (hocr_pixbuf_get_pixel (pix, x, y) == 0) ? 1 : 0;	/* count whites */
		}

		if (sum < 2)
			return 0;
	}

	return 1;
}

int
find_vertical_path (hocr_pixbuf * pix, int x1, int y1, int x2, int y2)
{
	int x, y;
	int sum;

	for (y = y1; y < y2; y++)
	{
		sum = 0;

		for (x = x1; x < x2; x++)
		{
			sum += hocr_pixbuf_get_pixel (pix, x, y);
		}

		sum = sum / (x2 - x1);

		if (sum > 0)
			return 0;
	}

	return 1;
}

int
find_horizontal_notch_to_right_down (hocr_pixbuf * pix, int x1, int y1,
				     int x2, int y2)
{
	int x, y;

	int sum;
	int max = 0;

	for (y = y1; y < y2; y++)
	{
		sum = 0;
		x = x2;
		while (x > x1 && sum == 0)
		{
			sum = hocr_pixbuf_get_pixel (pix, x, y);
			x--;
		}

		if (max > (x2 - x))
			return 1;

		if (max < (x2 - x))
			max = (x2 - x);
	}

	return 0;
}

int
find_horizontal_notch_to_left_down (hocr_pixbuf * pix, int x1, int y1, int x2,
				    int y2)
{
	int x, y;

	int sum;
	int max = 0;

	for (y = y1; y < y2; y++)
	{
		sum = 0;
		x = x1;
		while (x < x2 && sum == 0)
		{
			sum = hocr_pixbuf_get_pixel (pix, x, y);
			x++;
		}

		if (max > (x - x1))
			return 1;

		if (max < (x - x1))
			max = (x - x1);
	}

	return 0;
}

int
find_horizontal_notch_to_left_up (hocr_pixbuf * pix, int x1, int y1, int x2,
				  int y2)
{
	int x, y;

	int sum;
	int max = 0;

	for (y = y2; y > y1; y--)
	{
		sum = 0;
		x = x1;
		while (x < x2 && sum == 0)
		{
			sum = hocr_pixbuf_get_pixel (pix, x, y);
			x++;
		}

		if (max > (x - x1))
			return 1;

		if (max < (x - x1))
			max = (x - x1);
	}

	return 0;
}

int
find_horizontal_notch_to_right_up (hocr_pixbuf * pix, int x1, int y1, int x2,
				   int y2)
{
	int x, y;

	int sum;
	int max = 0;

	for (y = y2; y > y1; y--)
	{
		sum = 0;
		x = x2;
		while (x > x1 && sum == 0)
		{
			sum = hocr_pixbuf_get_pixel (pix, x, y);
			x--;
		}

		if (max > (x2 - x))
			return 1;

		if (max < (x2 - x))
			max = (x2 - x);
	}

	return 0;
}

int
find_vertical_notch_down_to_left (hocr_pixbuf * pix, int x1, int y1, int x2,
				  int y2)
{
	int x, y;

	int sum;
	int max = 0;

	for (x = x2; x > x1; x--)
	{
		sum = 0;
		y = y2 - 3;
		while (y > y1 && sum < 2)
		{
			sum = hocr_pixbuf_get_pixel (pix, x, y);
			sum += hocr_pixbuf_get_pixel (pix, x + 1, y);
			sum += hocr_pixbuf_get_pixel (pix, x + 2, y);
			y--;
		}

		if (max > (y2 - y))
		{
			return 1;

		}
		if (max < (y2 - y))
			max = (y2 - y);
	}

	return 0;
}

int
find_vertical_notch_up_to_left (hocr_pixbuf * pix, int x1, int y1, int x2,
				int y2)
{
	int x, y;

	int sum;
	int max = 0;

	for (x = x2; x > x1; x--)
	{
		sum = 0;
		y = y1;
		while (y < y2 && sum == 0)
		{
			sum = hocr_pixbuf_get_pixel (pix, x, y);
			y++;
		}

		if (max > (y - y1))
			return 1;

		if (max < (y - y1))
			max = (y - y1);
	}

	return 0;
}

int
find_vertical_notch_up_to_right (hocr_pixbuf * pix, int x1, int y1, int x2,
				 int y2)
{
	int x, y;

	int sum;
	int max = 0;

	for (x = x1; x < x2; x++)
	{
		sum = 0;
		y = y1;
		while (y < y2 && sum == 0)
		{
			sum = hocr_pixbuf_get_pixel (pix, x, y);
			y++;
		}

		if (max > (y - y1))
			return 1;

		if (max < (y - y1))
			max = (y - y1);
	}

	return 0;
}

int
find_vertical_double_notch_up_to_right (hocr_pixbuf * pix, int x1, int y1,
					int x2, int y2)
{
	int x, y;

	int sum;
	int counter = 0;
	int is_on_up_slop = 1;
	int max = 0;

	for (x = x1; x < x2; x++)
	{
		sum = 0;
		y = y1;
		while (y < y2 && sum == 0)
		{
			sum = hocr_pixbuf_get_pixel (pix, x, y);
			y++;
		}

		if (max > (y - y1) && is_on_up_slop == 0)
		{
			/* check for arteffacts */
			sum = hocr_pixbuf_get_pixel (pix, x + 1, y);
			if (sum == 0)
				continue;

			/* true nutch */
			is_on_up_slop = 1;
			counter++;
		}

		if (max < (y - y1) && is_on_up_slop == 1)
		{
			is_on_up_slop = 0;
		}

		if (max < (y - y1) || is_on_up_slop == 1)
		{
			max = (y - y1);
		}
	}

	return counter;
}

int
thin_lines (hocr_pixbuf * pix, hocr_box font)
{
	int x, y;
	int start;
	int end;
	int sum = 0;

	/* look at middle of font */
	x = font.x1 + font.width / 2;
	y = font.y1;

	/* go axros the font + one white place */
	while (y < font.y2 && sum == 0)
	{
		sum = hocr_pixbuf_get_pixel (pix, x, y);
		y++;
	}
	start = y;
	while (y < font.y2 && sum == 1)
	{
		sum = hocr_pixbuf_get_pixel (pix, x, y);
		y++;
	}
	end = y;

	if ((end - start) < 4)
		return 1;

	return 0;
}

int
find_tet_mark (hocr_pixbuf * pix, hocr_box font)
{
	int x, y;
	int sum = 0;

	/* look at middle of font */
	y = font.y1 + font.hight / 2;
	x = font.x2;

	/* get inside the font */
	while (sum == 0 && x > font.x1)
	{
		sum = hocr_pixbuf_get_pixel (pix, x, y);
		x--;
	}
	if (x == font.x1)
		return 0;

	while (sum == 1 && x > font.x1)
	{
		sum = hocr_pixbuf_get_pixel (pix, x, y);
		x--;
	}
	if (x == font.x1)
		return 0;

	/* get to top of inside */
	while (sum == 0 && x > font.x1 && y > font.y1)
	{
		/* try up */
		y--;
		sum = hocr_pixbuf_get_pixel (pix, x, y);
		if (sum == 0)
			continue;
		/* try left */
		x--;
		sum = hocr_pixbuf_get_pixel (pix, x, y);
		if (sum == 0)
			continue;
		/* try strait left */
		y++;
		sum = hocr_pixbuf_get_pixel (pix, x, y);
	}
	if (x == font.x1)
		return 0;
	if (y == font.y1)
		return 0;

	/* get to left of inside */
	sum = 0;
	while (sum == 0 && x > font.x1 && y < font.y2)
	{
		/* look fot a break upword */
		sum = hocr_pixbuf_get_pixel (pix, x - 1, y + 1);
		sum += hocr_pixbuf_get_pixel (pix, x - 1, y - 1);
		sum += hocr_pixbuf_get_pixel (pix, x - 1, y - 2);

		if (sum == 0)
			return 1;

		/* try left */
		x--;
		sum = hocr_pixbuf_get_pixel (pix, x, y);
		if (sum == 0)
			continue;
		/* try down */
		y++;
		sum = hocr_pixbuf_get_pixel (pix, x, y);
		if (sum == 0)
			continue;
		/* try strait down */
		x++;
		sum = hocr_pixbuf_get_pixel (pix, x, y);
	}

	return 0;
}

/**
 */

int
has_alef_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int y_top;
	int y_bottom;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 5,
				     &y_top, &y_bottom);

	if (number_of_bars != 2)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + 4 * font.hight / 5,
				     &y_top, &y_bottom);

	if (number_of_bars != 2)
		return 0;

	number_of_bars =
		count_horizontal_bars (pix, font,
				       font.x1 + font.width / 2, &y_top,
				       &y_bottom);

	if (y_bottom > (font.y2 - font.hight / 5))
		return 0;

	if (y_top < (font.y1 + font.hight / 5))
		return 0;

	return 1;
}

int
has_bet_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;


	/* horizontal bars */
	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 3,
				       &start_of_top_bar, &end_of_top_bar);

	if (number_of_bars != 2)
		return 0;

	/* horizontal path */
	if (find_horizontal_path
	    (pix, font.x1, end_of_top_bar - 1, font.x1 + font.width / 3,
	     end_of_top_bar + 4) == 0)
		return 0;

	/* vertical bars */

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + 2 * font.hight / 3,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 1)
		return 0;

	/* this is not right bar */
	if (end_of_right_bar < (font.x1 + font.width / 2))
		return 0;

	/* if not caf */
	if (find_horizontal_notch_to_right_down
	    (pix, end_of_right_bar - 3, font.y1 + 2 * font.hight / 3,
	     end_of_right_bar + 1, font.y2) == 0)
		return 0;

	if (find_vertical_notch_down_to_left
	    (pix, font.x1 + font.width / 4, font.y1 + font.hight / 2,
	     font.x1 + 3 * font.width / 4, font.y2) == 1)
		return 0;

	if (find_vertical_path
	    (pix, font.x1 + font.width / 3, font.y1,
	     font.x1 + 2 * font.width / 3, font.y1 + font.hight / 3) == 1)
		return 0;

	return 1;
}

int
has_gimel_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int end_of_right_bar;
	int start_of_right_bar;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + 2 * font.hight / 3,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 1)
		return 0;

	/* if not caf */
	//if (find_horizontal_notch_to_right_down
	//    (pix, font.x1 + font.width / 2, font.y1 + 2 * font.hight / 3, font.x2,
	//     font.y2) == 0)
	//      return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 3,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 1)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 1)
		return 0;

	/* start of gimel is tricky */

	number_of_bars =
		count_vertical_bars (pix, font, font.y2 - 1,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars == 2)
		return 1;

	number_of_bars =
		count_vertical_bars (pix, font, font.y2 - 3,
				     &start_of_right_bar, &end_of_right_bar);


	if (number_of_bars == 2)
		return 1;

	number_of_bars =
		count_vertical_bars (pix, font, font.y2 - 5,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars == 2)
		return 1;

	/* end of gimel is tricky */

	return 0;
}

int
has_dalet_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;
	int x_end_of_top_bar;
	int x_start_of_top_bar;

	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 3,
				       &start_of_top_bar, &end_of_top_bar);

	if (number_of_bars != 1)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 1)
		return 0;

	/* this is not top bar */
	if (end_of_top_bar > (font.y1 + font.hight / 2))
		return 0;

	/* is dalet ? */
	number_of_bars =
		count_vertical_bars (pix, font,
				     (end_of_top_bar + start_of_top_bar) / 2,
				     &x_start_of_top_bar, &x_end_of_top_bar);

	if (number_of_bars != 1)
		return 0;

	if (end_of_right_bar >= x_end_of_top_bar)
		return 0;

	/* if not he */
	if (is_empty
	    (pix, font.x1, end_of_top_bar + 3, start_of_right_bar - 3,
	     font.y2) == 0)
		return 0;

	return 1;
}

int
has_he_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;

	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 2,
				       &start_of_top_bar, &end_of_top_bar);

	if (number_of_bars != 1)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + 3 * font.hight / 4,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 2)
		return 0;

	/* this is not top bar */
	if (end_of_top_bar > (font.y1 + font.hight / 2))
		return 0;

	/* if not het */
	if (find_horizontal_path
	    (pix, font.x1, end_of_top_bar - 1, font.x1 + font.width / 2,
	     end_of_top_bar + 5) == 0)
		return 0;

	return 1;
}

int
has_vav_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;
	int x_end_of_top_bar;
	int x_start_of_top_bar;

	/* chek if this is realy is a thin font */
	if ((double) font.hight / (double) font.width < 2.1)
		return 0;

	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 2,
				       &start_of_top_bar, &end_of_top_bar);

	if (number_of_bars != 1)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 1)
		return 0;

	/* this is not top bar */
	if (end_of_top_bar > (font.y1 + font.hight / 2))
		return 0;

	/* this is not right bar */
	if (start_of_right_bar < (font.x2 - font.width / 2))
		return 0;

	/* is zain */
	number_of_bars =
		count_vertical_bars (pix, font,
				     (end_of_top_bar + start_of_top_bar) / 2,
				     &x_start_of_top_bar, &x_end_of_top_bar);

	if (number_of_bars != 1)
		return 0;

	if (end_of_right_bar < (x_end_of_top_bar - 3))
		return 0;

	/* if not he */
	if (is_empty
	    (pix, font.x1, end_of_top_bar + 4, start_of_right_bar - 4,
	     font.y2) == 0)
		return 0;

	return 1;
}

int
has_zain_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int end_of_right_bar;
	int start_of_right_bar;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 1)
		return 0;

	/* check to see if not nun */
	if (find_vertical_path
	    (pix, start_of_right_bar - 5, font.y1 + font.hight / 2,
	     start_of_right_bar + 2, font.y2) == 0)
		return 0;

	/* check to see if not vav */
	if (find_horizontal_notch_to_right_up
	    (pix, end_of_right_bar - 2, font.y1, font.x2,
	     font.y1 + font.hight / 2) == 0)
		return 0;

	return 1;
}

int
has_het_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int start, end;

	if (font.width < 15 || font.hight < 15)
		return 0;

	if (font.hight / font.width > 1)
		return 0;

	/* start of font */

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + 2 * font.hight / 3,
				     &start, &end);

	if (number_of_bars != 2)
		return 0;

	/* if not tav */
	if (find_vertical_path
	    (pix, start - 2, font.y1 + 2 * font.hight / 3, start + 2,
	     font.y2) == 0)
		return 0;

	/* if not mem */
	if (find_vertical_path
	    (pix, end - 7, font.y1 + 2 * font.hight / 3, end - 2,
	     font.y2) == 0)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + 2 * font.hight / 3,
				     &start, &end);

	if (number_of_bars != 2)
		return 0;


	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 2,
				       &start, &end);

	if (start > (font.y1 + 3))
		return 0;

	if (end > (font.y1 + 10))
		return 0;

	if (number_of_bars != 1)
		return 0;

	return 1;
}

int
has_tet_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int start, end;

	/* start of font */

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start, &end);

	if (number_of_bars == 1)
		return 0;

	number_of_bars =
		find_vertical_double_notch_up_to_right (pix,
							font.x1 +
							font.width / 8,
							font.y1, font.x2,
							font.y1 +
							font.hight / 2);

	if (number_of_bars != 1)
		return 0;

	if (find_tet_mark (pix, font) == 0)
		return 0;

	if (thin_lines (pix, font) == 0 && find_vertical_notch_down_to_left
	    (pix, font.x1, font.y1 + 2 * font.hight / 3,
	     font.x1 + font.width / 4, font.y2) == 1)
		return 0;

	if (thin_lines (pix, font) == 1 && find_vertical_notch_down_to_left
	    (pix, font.x1, font.y1 + 2 * font.hight / 3,
	     font.x1 + font.width / 2, font.y2) == 1)
		return 0;

	if (find_horizontal_notch_to_left_down
	    (pix, font.x1, font.y1 + 2 * font.hight / 3,
	     font.x1 + font.width / 2, font.y2) == 1)
		return 0;

	if (thin_lines == 0 && find_vertical_notch_down_to_left
	    (pix, start + 4, font.y1,
	     font.x1 + font.width / 2, font.y1 + font.hight / 2) == 0)
		return 0;

	return 1;
}

int
has_yud_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;

	if (find_horizontal_notch_to_left_up
	    (pix, font.x1, font.y1, font.x1 + font.width / 2,
	     font.y1 + 2 * font.hight / 3) == 0)
		return 0;

	/* horizontal bars */
	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start_of_top_bar, &end_of_top_bar);

	if (number_of_bars != 1)
		return 0;

	return 1;
}

int
has_kaf_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;

	if ((double) font.hight / (double) font.width > 1.7)
		return 0;

	/* horizontal bars */
	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 2,
				       &start_of_top_bar, &end_of_top_bar);

	if (number_of_bars != 2)
		return 0;

	if (find_vertical_path
	    (pix, font.x1 + font.width / 3, font.y1,
	     font.x1 + 2 * font.width / 3, font.y1 + font.hight / 3) == 1)
		return 0;

	/* vertical bars */

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 1)
		return 0;

	/* this is not right bar */
	if (end_of_right_bar < (font.x1 + font.width / 2))
		return 0;

	return 1;
}

int
has_kaf_sofit_mark (hocr_pixbuf * pix, hocr_box font)
{

	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;

	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 2,
				       &start_of_top_bar, &end_of_top_bar);

	if (number_of_bars != 1)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 1)
		return 0;

	/* this is not top bar */
	if (end_of_top_bar > (font.y1 + font.hight / 2))
		return 0;

	/* this is not right bar */
	if (start_of_right_bar < (font.x2 - font.width / 2))
		return 0;

	/* if not he */
	if (is_empty
	    (pix, font.x1, end_of_top_bar + 4, start_of_right_bar - 4,
	     font.y2) == 0)
		return 0;

	return 1;
}

int
has_lamed_mark (hocr_pixbuf * pix, hocr_box font)
{

	return 1;
}

int
has_mem_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;

	if (font.width < 10 || font.hight < 15)
		return 0;

	/* vertical bars */
	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + 2 * font.hight / 3,
				     &start_of_right_bar, &end_of_right_bar);

	//if (number_of_bars != 2)
	//      return 0;

	/* horizontal bars */
	number_of_bars =
		count_horizontal_bars (pix, font,
				       font.x1 + 2 * font.width / 3,
				       &start_of_top_bar, &end_of_top_bar);

	if (number_of_bars != 2)
		return 0;

	if (end_of_right_bar < (font.x1 + 2 * font.width / 3))
		return 0;

	if (start_of_right_bar > (font.x1 + font.width / 3))
		return 0;

	if (end_of_top_bar < (font.y1 + 2 * font.hight / 3))
		return 0;

	if (start_of_top_bar > (font.y1 + font.hight / 3))
		return 0;

	//if (has_black_right_bottom_mark (pix, font) == 1)
	//      return 0;

	//if (has_black_left_bottom_mark (pix, font) == 0)
	//      return 0;

	//if (find_vertical_notch_down_to_left
	//    (pix, font.x1, font.y1 + 2 * font.hight / 3, font.x1 + font.width / 5,
	//     font.y2) == 0)
	//      return 0;

	if (find_vertical_path
	    (pix, start_of_right_bar + 1, font.y1 + font.hight / 2,
	     end_of_right_bar - 3, font.y2) == 0)
		return 0;

	//number_of_bars = find_vertical_double_notch_up_to_right (pix, font.x1 + font.width /3, font.y1, font.x2, font.y2);

	//if (number_of_bars == 1)
	//      return 1;

	return 1;
}

int
has_mem_sofit_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;

	/* vertical bars */
	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 2)
		return 0;

	/* horizontal bars */
	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 2,
				       &start_of_top_bar, &end_of_top_bar);

	if (number_of_bars != 2)
		return 0;

	if (find_horizontal_path
	    (pix, font.x1, font.y1, font.x1 + font.width / 4, font.y2) == 1)
		return 0;

	//if (has_black_right_bottom_mark (pix, font) == 0)
	//      return 0;

	//if (find_vertical_notch_down_to_left
	//    (pix, font.x1 + font.width / 2, font.y1 + font.hight / 2,
	//     end_of_right_bar - 5, font.y2) == 1)
	//      return 0;

	if (thin_lines (pix, font) == 1 && find_vertical_notch_down_to_left
	    (pix, font.x1 + font.width / 2, font.y1 + font.hight / 2,
	     end_of_right_bar - 2, font.y2) == 1)
		return 0;

	return 1;
}

int
has_nun_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int end_of_right_bar;
	int start_of_right_bar;
	int end_of_top_bar;
	int start_of_top_bar;

	if ((double) font.hight / (double) font.width < 1.5)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 1)
		return 0;

	if (end_of_right_bar < font.x2 - font.width / 6)
		return 0;

	/* horizontal bars */
	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 2,
				       &start_of_top_bar, &end_of_top_bar);

	if (thin_lines (pix, font) == 0 && number_of_bars != 2)
		return 0;

	if (thin_lines (pix, font) == 1 && font.hight / font.width < 2)
		return 0;

	if (end_of_top_bar < font.y2 - font.hight / 6)
		return 0;

	return 1;
}

int
has_nun_sofit_mark (hocr_pixbuf * pix, hocr_box font)
{

	return 1;
}

int
has_sameh_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;

	/* vertical bars */
	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 2)
		return 0;

	/* horizontal bars */
	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 2,
				       &start_of_top_bar, &end_of_top_bar);

	if (number_of_bars != 2)
		return 0;

	if (find_horizontal_path
	    (pix, start_of_right_bar, start_of_top_bar + 2,
	     font.x1 + font.width / 2, end_of_top_bar - 2) == 1)
		return 0;

	//if (has_black_right_bottom_mark (pix, font) == 0)
	//      return 0;

	if (thin_lines (pix, font) == 0 && find_vertical_notch_down_to_left
	    (pix, font.x1 + font.width / 2, font.y1 + font.hight / 2,
	     end_of_right_bar - 2, font.y2) == 0)
		return 0;

	if (thin_lines (pix, font) == 1 && find_vertical_notch_down_to_left
	    (pix, font.x1 + font.width / 2, font.y1 + font.hight / 2,
	     end_of_right_bar - 2, font.y2) == 0)
		return 0;

	number_of_bars =
		find_vertical_double_notch_up_to_right (pix,
							font.x1 +
							font.width / 4,
							font.y1, font.x2,
							font.y1 +
							font.hight / 2);

	if (number_of_bars != 0)
		return 0;

	return 1;
}

int
has_ayin_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int end_of_right_bar;
	int start_of_right_bar;

	/* vertical bars */
	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 3,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 2)
		return 0;

	if (thin_lines (pix, font) == 0 && find_horizontal_path
	    (pix, font.x1, font.y1 + font.hight / 3, font.x1 + font.width / 3,
	     font.y2 - 3) == 0)
		return 0;

	if (find_horizontal_path
	    (pix, font.x1, font.y1 + 2 * font.hight / 3,
	     font.x1 + 2 * font.width / 3, font.y2 - 3) == 1)
		return 0;

	if (find_horizontal_path
	    (pix, font.x1 + 4 * font.width / 5, font.y1 + 3 * font.hight / 8,
	     font.x2, font.y1 + 5 * font.hight / 8) == 1)
		return 0;


	if (has_black_left_bottom_mark (pix, font) == 0)
		return 0;

	if (find_vertical_path
	    (pix, font.x1 + font.width / 3, font.y1 + font.hight / 2,
	     font.x1 + 2 * font.width / 3, font.y2) == 1)
		return 0;

	return 1;
}

int
has_pe_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;

	/* vertical bars */
	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 4,
				     &start_of_right_bar, &end_of_right_bar);

	/* horizontal bars */
	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 2,
				       &start_of_top_bar, &end_of_top_bar);

	if (end_of_right_bar < (font.x1 + 2 * font.width / 3))
		return 0;

	if (end_of_top_bar < (font.y1 + 2 * font.hight / 3))
		return 0;

	if (start_of_top_bar > (font.y1 + font.hight / 3))
		return 0;

	/*  horizontal notch */
	if (find_horizontal_notch_to_right_down
	    (pix, font.x1, font.y1 + font.hight / 2, end_of_right_bar + 1,
	     font.y2) == 1)
		return 0;

	if (find_horizontal_path
	    (pix, font.x1, font.y1 + 2 * font.hight / 3,
	     font.x1 + 3 * font.width / 4, font.y2 - 3) == 0)
		return 0;

	//if (find_vertical_notch_down_to_left
	//    (pix, font.x1, font.y1 + 2 * font.hight / 3, font.x1 + font.width / 2,
	//     font.y2) == 1)
	//      return 0;

	return 1;
}

int
has_pe_sofit_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int end_of_right_bar;
	int start_of_right_bar;

	/* vertical bars */
	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 3,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 2)
		return 0;

	if (find_horizontal_path
	    (pix, font.x1, font.y1 + font.hight / 3,
	     font.x1 + 2 * font.width / 3, font.y2 - 3) == 0)
		return 0;

	if (has_black_right_bottom_mark (pix, font) == 0)
		return 0;

	if (has_black_left_bottom_mark (pix, font) == 1)
		return 0;

	if (find_vertical_path
	    (pix, font.x1 + font.width / 3, font.y1,
	     font.x1 + 2 * font.width / 3, font.y1 + font.hight / 3) == 1)
		return 0;

	if (is_empty
	    (pix, font.x1, font.y1 + 2 * font.hight / 3,
	     font.x1 + font.width / 2, font.y2) == 0)
		return 0;

	return 1;
}

int
has_tzadi_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;

	/* vertical bars */
	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 1)
		return 0;

	if (find_horizontal_path
	    (pix, font.x1, font.y1 + font.hight / 3,
	     font.x1 + 1 * font.width / 2, font.y2 - 3) == 0)
		return 0;

	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 3,
				       &start_of_top_bar, &end_of_top_bar);

	if (number_of_bars == 1)
		return 0;

	if (has_black_right_bottom_mark (pix, font) == 0)
		return 0;

	//if (find_vertical_path
	//    (pix, font.x1 + font.width / 3, font.y1,
	//     font.x1 + 2 * font.width / 3, font.y1 + font.hight / 3) == 0)
	//      return 0;

	return 1;
}

int
has_tzadi_sofit_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;

	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 2,
				       &start_of_top_bar, &end_of_top_bar);

	//if (number_of_bars != 1)
	//      return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 6,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 2)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + 6 * font.hight / 7,
				     &start_of_right_bar, &end_of_right_bar);

	//if (number_of_bars != 1)
	//      return 0;

	/* this is not top bar */
	//if (end_of_top_bar > (font.y1 + font.hight / 2))
	//      return 0;

	/* if not het */
	//if (find_horizontal_path
	//    (pix, font.x1, start_of_top_bar + 2, font.x1 + font.width / 2,
	//     start_of_top_bar + 5) == 0)
	//      return 0;

	//if (find_vertical_path
	//    (pix, font.x1 + font.width / 3, font.y1,
	//     font.x1 + 2 * font.width / 3, font.y1 + font.hight / 6) == 0)
	//      return 0;

	return 1;
}

int
has_kof_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;

	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 2,
				       &start_of_top_bar, &end_of_top_bar);

	//if (number_of_bars != 1)
	//      return 0;

	//number_of_bars =
	//      count_vertical_bars (pix, font, font.y1 + font.hight / 3,
	//                           &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 2)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + 7 * font.hight / 8,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 1)
		return 0;

	if (end_of_right_bar > (font.x1 + font.width / 2))
		return 0;

	/* this is not top bar */
	//if (end_of_top_bar > (font.y1 + font.hight / 2))
	//      return 0;

	if (find_vertical_path
	    (pix, font.x1 + font.width / 3, font.y1,
	     font.x1 + 2 * font.width / 3, font.y1 + font.hight / 3) == 1)
		return 0;

	return 1;
}

int
has_resh_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;
	int x_end_of_top_bar;
	int x_start_of_top_bar;

	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 2,
				       &start_of_top_bar, &end_of_top_bar);

	if (number_of_bars != 1)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start_of_right_bar, &end_of_right_bar);

	if (number_of_bars != 1)
		return 0;

	/* this is not top bar */
	if (end_of_top_bar > (font.y1 + font.hight / 2))
		return 0;

	/* this is not right bar */
	if (start_of_right_bar < (font.x2 - font.width / 2))
		return 0;

	/* is zain */
	number_of_bars =
		count_vertical_bars (pix, font,
				     (end_of_top_bar + start_of_top_bar) / 2,
				     &x_start_of_top_bar, &x_end_of_top_bar);

	if (number_of_bars != 1)
		return 0;

	if (end_of_right_bar < x_end_of_top_bar)
		return 0;

	/* if not he */
	if (is_empty
	    (pix, font.x1, end_of_top_bar + 4, start_of_right_bar - 4,
	     font.y2) == 0)
		return 0;

	return 1;
}

int
has_shin_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int start, end;

	if (font.width < 15 || font.hight < 15)
		return 0;

	if (font.hight / font.width > 1)
		return 0;

	/* start of font */

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start, &end);

	if (number_of_bars != 3)
		return 0;

	number_of_bars =
		find_vertical_double_notch_up_to_right (pix,
							font.x1 +
							font.width / 4,
							font.y1, font.x2,
							font.y1 +
							font.hight / 2);

	if (number_of_bars == 1)
		return 0;

	return 1;
}

int
has_tav_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int start, end;

	/* regular font */
	//if (font.hight / font.width > 1)
	//      return 0;

	/* start of font */

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start, &end);

	if (number_of_bars != 2)
		return 0;

	/* if tav */
	if (find_vertical_path
	    (pix, start - 2, font.y1 + 2 * font.hight / 3, start + 2,
	     font.y2) == 1)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + 2 * font.hight / 3,
				     &start, &end);

	if (number_of_bars != 2)
		return 0;


	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 2,
				       &start, &end);

	if (start > (font.y1 + font.hight / 6))
		return 0;

	if (end > (font.y1 + font.hight / 2))
		return 0;

	if (number_of_bars != 1)
		return 0;

	return 1;
}

int
has_nekuda_mark (hocr_pixbuf * pix, hocr_box font)
{
	if ((double)font.hight / (double) font.width > 1.8)
		return 0;
	
	return 1;
}

int
has_psik_mark (hocr_pixbuf * pix, hocr_box font)
{
	return 1;
}

int
has_quat_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int start, end;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start, &end);

	if (number_of_bars != 1)
		return 0;

	if (find_horizontal_notch_to_left_up
	    (pix, font.x1, font.y1, font.x1 + font.width / 2,
	     font.y1 + font.hight / 2) == 1)
		return 0;

	return 1;
}

int
has_double_quat_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int start, end;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start, &end);

	if (number_of_bars != 2)
		return 0;

	return 1;
}

int
has_exlem_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int start, end;

	if (font.hight / font.width <= 1)
		return 0;

	return 1;
}

int
has_dots_mark (hocr_pixbuf * pix, hocr_box font)
{
	return 1;
}

int
has_question_mark (hocr_pixbuf * pix, hocr_box font)
{

	int number_of_bars;
	int start, end;

	if (font.hight / font.width > 1)
		return 0;

	return 1;
}

int
has_makaf_mark (hocr_pixbuf * pix, hocr_box font)
{
	int number_of_bars;
	int start, end;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start, &end);

	if (number_of_bars != 1)
		return 0;

	if (font.width > font.hight)
		return 1;

	return 0;
}

int
hocr_guess_font (hocr_pixbuf * pix, hocr_box font, int base_class,
		 int base, int top,
		 int top_class,
		 int hight_class,
		 int width_class,
		 int end_of_word, char *font_string,
		 int max_chars_in_font_string)
{
	int number_of_bars, number_of_bars2;
	int start, end;
	hocr_box under_the_font;

	/* if font is too small this is artefact */
	if (font.hight < MIN_FONT_SIZE && font.width < MIN_FONT_SIZE)
	{
		sprintf (font_string, "");
		return 1;
	}

	/* normal width, high, assend over top of line fonts */
	if (hight_class == 1 && top_class == 1)
	{
		if (has_lamed_mark (pix, font))
		{
			sprintf (font_string, "ל");
			return 0;
		}

		sprintf (font_string, "_");
		return 1;
	}

	/* thin width, high, desend below base of line fonts */
	if (width_class == -1 && hight_class == 1 && base_class == 1)
	{
		if (has_nun_sofit_mark (pix, font))
		{
			sprintf (font_string, "ן");
			return 0;
		}

		sprintf (font_string, "_");
		return 1;
	}

	/* normal width, high, desend below base of line fonts */
	if (width_class == 0 && hight_class == 1 && base_class == 1)
	{
		if (has_kaf_sofit_mark (pix, font))
		{
			sprintf (font_string, "ך");
			return 0;
		}

		if (has_ayin_mark (pix, font))
		{
			sprintf (font_string, "ע");
			return 0;
		}

		if (has_pe_sofit_mark (pix, font))
		{
			sprintf (font_string, "ף");
			return 0;
		}

		if (has_tzadi_sofit_mark (pix, font))
		{
			sprintf (font_string, "ץ");
			return 0;
		}

		if (has_kof_mark (pix, font))
		{
			sprintf (font_string, "ק");
			return 0;
		}

		sprintf (font_string, "_");
		return 1;
	}

	/* below top and above base lines fonts */
	if (hight_class == -1 && top_class == -1 && base_class == -1)
	{
		/* look for the dot of ? : ! under the font */
		under_the_font.x1 = font.x1;
		under_the_font.y1 = font.y2;
		under_the_font.x2 = font.x2;
		under_the_font.y2 = base;

		number_of_bars =
			count_horizontal_bars (pix, under_the_font,
					       font.x1 + font.width / 2,
					       &start, &end);

		/* found a dot under the font */
		if (number_of_bars != 0)
		{

			if (has_dots_mark (pix, font))
			{
				sprintf (font_string, ":");
				return 0;
			}

			sprintf (font_string, "_");
			return 1;
		}
		else
		{
			if (has_makaf_mark (pix, font))
			{
				sprintf (font_string, "-");
				return 0;
			}

			sprintf (font_string, "_");
			return 1;
		}

		sprintf (font_string, "_");
		return 1;
	}

	/* below top and on base lines fonts */
	if (hight_class == -1 && top_class == -1 && base_class == 0)
	{
		if (has_nekuda_mark (pix, font))
		{
			sprintf (font_string, ".");
			return 0;
		}

		if (has_psik_mark (pix, font))
		{
			sprintf (font_string, ",");
			return 0;
		}
		
		sprintf (font_string, "_");
		return 1;
	}

	/* below top and below base lines fonts */
	if (hight_class == -1 && top_class == -1 && base_class == 1)
	{
		if (has_psik_mark (pix, font))
		{
			sprintf (font_string, ",");
			return 0;
		}

		sprintf (font_string, "_");
		return 1;
	}

	/* on top and above base lines fonts */
	if (hight_class == -1 && top_class == 0 && base_class == -1)
	{
		/* look for the dot of ? : ! under the font */
		under_the_font.x1 = font.x1;
		under_the_font.y1 = font.y2;
		under_the_font.x2 = font.x2;
		under_the_font.y2 = base + 3;

		number_of_bars =
			count_horizontal_bars (pix, under_the_font,
					       font.x1 + font.width / 2,
					       &start, &end);

		number_of_bars2 =
			count_horizontal_bars (pix, under_the_font,
					       font.x1 + font.width / 3,
					       &start, &end);

		if (number_of_bars < number_of_bars2)
			number_of_bars = number_of_bars2;

		/* found a dot under the font */
		if (number_of_bars != 0)
		{
			if (has_question_mark (pix, font))
			{
				sprintf (font_string, "?");
				return 0;
			}

			if (has_exlem_mark (pix, font))
			{
				sprintf (font_string, "!");
				return 0;
			}

			sprintf (font_string, "_");
			return 1;
		}
		else
		{
			if (has_makaf_mark (pix, font))
			{
				sprintf (font_string, "-");
				return 0;
			}

			if (has_yud_mark (pix, font))
			{
				sprintf (font_string, "י");
				return 0;
			}

			if (has_quat_mark (pix, font))
			{
				sprintf (font_string, "\'");
				return 0;
			}

			if (has_double_quat_mark (pix, font))
			{
				sprintf (font_string, "\"");
				return 0;
			}

			sprintf (font_string, "_");
			return 1;
		}

		sprintf (font_string, "_");
		return 1;
	}

	/* thin fonts */
	if (hight_class == 0 && width_class == -1)
	{
		if (has_gimel_mark (pix, font))
		{
			sprintf (font_string, "ג");
			return 0;
		}

		if (has_vav_mark (pix, font))
		{
			sprintf (font_string, "ו");
			return 0;
		}

		if (has_zain_mark (pix, font))
		{
			sprintf (font_string, "ז");
			return 0;
		}

		if (has_tet_mark (pix, font))
		{
			sprintf (font_string, "ט");
			return 0;
		}

		if (has_nun_mark (pix, font))
		{
			sprintf (font_string, "נ");
			return 0;
		}

		sprintf (font_string, "_");
		return 1;
	}

	/* all other fonts */
	if (hight_class == 0 && width_class == 0)
	{
		if (has_alef_mark (pix, font))
		{
			sprintf (font_string, "א");
			return 0;
		}

		if (has_bet_mark (pix, font))
		{
			sprintf (font_string, "ב");
			return 0;
		}

		if (has_gimel_mark (pix, font))
		{
			sprintf (font_string, "ג");
			return 0;
		}

		if (has_dalet_mark (pix, font))
		{
			sprintf (font_string, "ד");
			return 0;
		}

		if (has_he_mark (pix, font))
		{
			sprintf (font_string, "ה");
			return 0;
		}

		if (has_het_mark (pix, font))
		{
			sprintf (font_string, "ח");
			return 0;
		}

		if (has_tet_mark (pix, font))
		{
			sprintf (font_string, "ט");
			return 0;
		}

		if (has_kaf_mark (pix, font))
		{
			sprintf (font_string, "כ");
			return 0;
		}

		if (has_mem_mark (pix, font))
		{
			sprintf (font_string, "מ");
			return 0;
		}

		if (has_mem_sofit_mark (pix, font))
		{
			sprintf (font_string, "ם");
			return 0;
		}

		if (has_sameh_mark (pix, font))
		{
			sprintf (font_string, "ס");
			return 0;
		}

		if (has_ayin_mark (pix, font))
		{
			sprintf (font_string, "ע");
			return 0;
		}

		if (has_pe_mark (pix, font))
		{
			sprintf (font_string, "פ");
			return 0;
		}

		if (has_tzadi_mark (pix, font))
		{
			sprintf (font_string, "צ");
			return 0;
		}

		if (has_resh_mark (pix, font))
		{
			sprintf (font_string, "ר");
			return 0;
		}

		if (has_shin_mark (pix, font))
		{
			sprintf (font_string, "ש");
			return 0;
		}

		if (has_tav_mark (pix, font))
		{
			sprintf (font_string, "ת");
			return 0;
		}

		sprintf (font_string, "_");
		return 1;
	}

	/* this font is unknown */
	sprintf (font_string, "_");

	return 1;
}
