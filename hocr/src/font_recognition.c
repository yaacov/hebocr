
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

//#define DEBUG

/* 
 font markers
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hocr.h"
#include "consts.h"
#include "hocr_object.h"
#include "font_recognition.h"

/**
 */

int
has_black_right_bottom_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int x, y;
	int sum;

	sum = 0;
	/* check a 6*6 triangle */
	for (x = font.x2; x > (font.x2 - 2); x--)
		for (y = font.y2; y > (font.y2 - (x - (font.x1 - 2))); y--)
		{
			sum += (hocr_pixbuf_get_object (pix, x, y) ==
				obj) ? 1 : 0;
		}

	if (sum < 3)
		return 0;

	return 1;
}

int
has_black_left_bottom_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int x, y;
	int sum;

	/* look at bottom right */
	sum = 0;
	/* check a 6*6 triangle */
	for (x = font.x1; x < (font.x1 + 4); x++)
		for (y = font.y2; y > (font.y2 - ((font.x1 + 4) - x)); y--)
		{
			sum += ((hocr_pixbuf_get_object (pix, x, y) ==
				 obj) ? 1 : 0);
		}

	if (sum == 0)
		return 0;
	return 1;
}

int
has_black_left_top_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int x, y;
	int sum;

	/* look at bottom right */
	sum = 0;
	/* check a 6*6 triangle */
	for (x = font.x1; x < (font.x1 + 4); x++)
		for (y = font.y1; y < (font.y2 + ((font.x1 + 4) - x)); y++)
		{
			sum += ((hocr_pixbuf_get_object (pix, x, y) ==
				 obj) ? 1 : 0);
		}

	if (sum == 0)
		return 0;
	return 1;
}

int
has_black_right_top_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int x, y;
	int sum;

	sum = 0;

	/* check a 6*6 triangle */
	for (x = font.x2; x > (font.x2 - 4); x--)
		for (y = font.y1; y < (font.y2 + (x - (font.x2 - 4))); y++)
		{
			sum += ((hocr_pixbuf_get_object (pix, x, y) ==
				 obj) ? 1 : 0);
		}

	if (sum == 0)
		return 0;

	return 1;
}

int
count_vertical_bars (hocr_pixbuf * pix, hocr_box font, int y_pos, int *first_x,
		     int *last_x, unsigned int obj)
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
		new_color =
			((hocr_pixbuf_get_object (pix, x, y) == obj) ? 1 : 0);

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
		       int *first_y, int *last_y, unsigned int obj)
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
		new_color =
			((hocr_pixbuf_get_object (pix, x, y) == obj) ? 1 : 0);

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
is_empty (hocr_pixbuf * pix, int x1, int y1, int x2, int y2, unsigned int obj)
{
	int x, y;
	int sum = 0;

	/* check for black pixels */
	for (x = x1; x < x2; x++)
		for (y = y1; y < y2; y++)
		{
			sum += ((hocr_pixbuf_get_object (pix, x, y) ==
				 obj) ? 1 : 0);
		}

	return (sum == 0) ? 1 : 0;
}

int
is_full (hocr_pixbuf * pix, int x1, int y1, int x2, int y2, unsigned int obj)
{
	int x, y;
	int sum = 0;

	/* check for black pixels */
	for (x = x1; x < x2; x++)
		for (y = y1; y < y2; y++)
		{
			sum += ((hocr_pixbuf_get_object (pix, x, y) ==
				 obj) ? 1 : 0);
		}

	return (sum >= ((x2 - x1) * (y2 - y1 - 1) * 3.0 / 4.0)) ? 1 : 0;
}

int
find_horizontal_path (hocr_pixbuf * pix, int x1, int y1, int x2, int y2,
		      unsigned int obj)
{
	int x, y;
	int sum;

	for (x = x1; x < x2; x++)
	{
		sum = 0;

		for (y = y1; y < y2; y++)
		{
			sum += (hocr_pixbuf_get_object (pix, x, y) ==
				obj) ? 0 : 1;
		}

		if (sum < 2)
			return 0;
	}

	return 1;
}

int
find_vertical_path (hocr_pixbuf * pix, int x1, int y1, int x2, int y2,
		    unsigned int obj)
{
	int x, y;
	int sum;

	for (y = y1; y < y2; y++)
	{
		sum = 0;

		for (x = x1; x < x2; x++)
		{
			sum += ((hocr_pixbuf_get_object (pix, x, y) ==
				 obj) ? 1 : 0);
		}

		if ((x2 - x1) != 0)
			sum = sum / (x2 - x1);

		if (sum > 0)
			return 0;
	}

	return 1;
}

int
find_horizontal_notch_to_right_down (hocr_pixbuf * pix, int x1, int y1,
				     int x2, int y2, unsigned int obj)
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
			sum = ((hocr_pixbuf_get_object (pix, x, y) ==
				obj) ? 1 : 0);
			x--;
		}

		if (max > (x2 - x + 1))
			return 1;

		if (max < (x2 - x))
			max = (x2 - x);
	}

	return 0;
}

int
find_small_horizontal_notch_to_right_down (hocr_pixbuf * pix, int x1, int y1,
					   int x2, int y2, unsigned int obj)
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
			sum = ((hocr_pixbuf_get_object (pix, x, y) ==
				obj) ? 1 : 0);
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
				    int y2, unsigned int obj)
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
			sum = ((hocr_pixbuf_get_object (pix, x, y) ==
				obj) ? 1 : 0);
			x++;
		}

		if (max > ((x - x1) + 1))
			return 1;

		if (max < (x - x1))
			max = (x - x1);
	}

	return 0;
}

int
find_horizontal_notch_to_left_up (hocr_pixbuf * pix, int x1, int y1, int x2,
				  int y2, unsigned int obj)
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
			sum = ((hocr_pixbuf_get_object (pix, x, y) ==
				obj) ? 1 : 0);
			x++;
		}

		if (max > (x - x1 + 1))
			return 1;

		if (max < (x - x1))
			max = (x - x1);
	}

	return 0;
}

int
find_horizontal_notch_to_right_up (hocr_pixbuf * pix, int x1, int y1, int x2,
				   int y2, unsigned int obj)
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
			sum = ((hocr_pixbuf_get_object (pix, x, y) ==
				obj) ? 1 : 0);
			x--;
		}

		if (max > (x2 - x + 1))
			return 1;

		if (max < (x2 - x))
			max = (x2 - x);
	}

	return 0;
}

int
find_vertical_notch_down_to_left (hocr_pixbuf * pix, int x1, int y1, int x2,
				  int y2, unsigned int obj)
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
			sum = ((hocr_pixbuf_get_object (pix, x, y) ==
				obj) ? 1 : 0);
			sum += hocr_pixbuf_get_pixel (pix, x + 1, y);
			sum += hocr_pixbuf_get_pixel (pix, x + 2, y);
			y--;
		}

		if (max > (y2 - y + 1))
		{
			return 1;

		}
		if (max < (y2 - y))
			max = (y2 - y);
	}

	return 0;
}

int
find_vertical_notch_down_to_right (hocr_pixbuf * pix, int x1, int y1, int x2,
				   int y2, unsigned int obj)
{
	int x, y;

	int sum;
	int max = 0;

	for (x = x1; x < x2; x++)
	{
		sum = 0;
		y = y2 - 3;
		while (y > y1 && sum < 2)
		{
			sum = ((hocr_pixbuf_get_object (pix, x, y) ==
				obj) ? 1 : 0);
			sum += hocr_pixbuf_get_pixel (pix, x + 1, y);
			sum += hocr_pixbuf_get_pixel (pix, x + 2, y);
			y--;
		}

		if (max > (y2 - y + 1))
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
				int y2, unsigned int obj)
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
			sum = ((hocr_pixbuf_get_object (pix, x, y) ==
				obj) ? 1 : 0);
			y++;
		}

		if (max > (y - y1 + 1))
			return 1;

		if (max < (y - y1))
			max = (y - y1);
	}

	return 0;
}

int
find_vertical_notch_up_to_right (hocr_pixbuf * pix, int x1, int y1, int x2,
				 int y2, unsigned int obj)
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
			sum = ((hocr_pixbuf_get_object (pix, x, y) ==
				obj) ? 1 : 0);
			y++;
		}

		if (max > (y - y1 + 1))
			return 1;

		if (max < (y - y1))
			max = (y - y1);
	}

	return 0;
}

int
find_vertical_double_notch_up_to_right (hocr_pixbuf * pix, int x1, int y1,
					int x2, int y2, unsigned int obj)
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
			sum = ((hocr_pixbuf_get_object (pix, x, y) ==
				obj) ? 1 : 0);
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
thin_lines (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
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
		sum = ((hocr_pixbuf_get_object (pix, x, y) == obj) ? 1 : 0);
		y++;
	}
	start = y;
	while (y < font.y2 && sum == 1)
	{
		sum = ((hocr_pixbuf_get_object (pix, x, y) == obj) ? 1 : 0);
		y++;
	}
	end = y;

	if ((end - start) < 4)
		return 1;

	return 0;
}

int
find_tet_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int x, y;
	int sum = 0;

	/* look at middle of font */
	y = font.y1 + font.hight / 2;
	x = font.x2;

	/* get inside the font */
	while (sum == 0 && x > font.x1)
	{
		sum = ((hocr_pixbuf_get_object (pix, x, y) == obj) ? 1 : 0);
		x--;
	}
	if (x == font.x1)
		return 0;

	while (sum == 1 && x > font.x1)
	{
		sum = ((hocr_pixbuf_get_object (pix, x, y) == obj) ? 1 : 0);
		x--;
	}
	if (x == font.x1)
		return 0;

	/* get to top of inside */
	while (sum == 0 && x > font.x1 && y > font.y1)
	{
		/* try up */
		y--;
		sum = ((hocr_pixbuf_get_object (pix, x, y) == obj) ? 1 : 0);
		if (sum == 0)
			continue;
		/* up did not work */
		y++;
		/* try left */
		x--;
		sum = ((hocr_pixbuf_get_object (pix, x, y) == obj) ? 1 : 0);
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
		sum = ((hocr_pixbuf_get_object (pix, x, y) == obj) ? 1 : 0);
		if (sum == 0)
			continue;
		/* left did not work return */
		x++;
		/* try down */
		y++;
		sum = ((hocr_pixbuf_get_object (pix, x, y) == obj) ? 1 : 0);
	}

	return 0;
}

int
find_horizintal_top_bar (hocr_pixbuf * pix, hocr_box font, unsigned int obj,
			 int *start, int *end)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;

	hocr_box box_up;

	box_up.x1 = font.x1;
	box_up.x2 = font.x2;
	box_up.y1 = font.y1 - 2;
	box_up.y2 = font.y1 + font.hight / 2;
	box_up.width = font.width;
	box_up.hight = font.hight / 2 + 2;

	*start = 0;
	*end = 0;

	/* horizontal bars */
	number_of_bars =
		count_horizontal_bars (pix, box_up, font.x1 + font.width / 3,
				       &start_of_top_bar, &end_of_top_bar, obj);

	if (number_of_bars != 1)
	{			/* if third does not work try middle */
		number_of_bars =
			count_horizontal_bars (pix, box_up,
					       font.x1 + font.width / 2,
					       &start_of_top_bar,
					       &end_of_top_bar, obj);

		if (number_of_bars != 1)
			return 0;
	}

	number_of_bars =
		count_vertical_bars (pix, box_up,
				     (start_of_top_bar + end_of_top_bar) / 2,
				     start, end, obj);

	if (number_of_bars != 1)
		return 0;

	if (*start > (font.x1 + font.width / 3) ||
	    *end < (font.x2 - font.width / 3))
		return 0;

	return 1;
}

int
find_horizintal_bottom_bar (hocr_pixbuf * pix, hocr_box font, unsigned int obj,
			    int *start, int *end)
{
	int number_of_bars;
	int end_of_bottom_bar;
	int start_of_bottom_bar;

	hocr_box box_down;

	box_down.x1 = font.x1;
	box_down.x2 = font.x2;
	box_down.y1 = font.y2 - font.hight / 2;
	box_down.y2 = font.y2 + 2;
	box_down.width = font.width;
	box_down.hight = font.hight / 2 + 2;

	*start = 0;
	*end = 0;

	/* horizontal bars */
	number_of_bars =
		count_horizontal_bars (pix, box_down, font.x1 + font.width / 3,
				       &start_of_bottom_bar, &end_of_bottom_bar,
				       obj);

	if (number_of_bars != 1)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, box_down,
				     (start_of_bottom_bar +
				      end_of_bottom_bar) / 2, start, end, obj);

	if (number_of_bars != 1)
	{
		/* if third fial try middle */
		number_of_bars =
			count_horizontal_bars (pix, box_down,
					       font.x1 + font.width / 2,
					       &start_of_bottom_bar,
					       &end_of_bottom_bar, obj);

		if (number_of_bars != 1)
			return 0;

		number_of_bars =
			count_vertical_bars (pix, box_down,
					     (start_of_bottom_bar +
					      end_of_bottom_bar) / 2, start,
					     end, obj);

		if (number_of_bars != 1)
			return 0;
	}

	return 1;
}

int
find_vertical_left_bar (hocr_pixbuf * pix, hocr_box font, unsigned int obj,
			int *start, int *end)
{
	int number_of_bars;
	int end_of_left_bar;
	int start_of_left_bar;

	hocr_box box_left;

	box_left.x1 = font.x1 - 2;
	box_left.x2 = font.x1 + font.width / 3;
	box_left.y1 = font.y1;
	box_left.y2 = font.y2;
	box_left.width = font.width / 3 + 2;
	box_left.hight = font.hight;

	*start = 0;
	*end = 0;

	/* horizontal bars */
	number_of_bars =
		count_vertical_bars (pix, box_left, font.y1 + font.hight / 2,
				     &start_of_left_bar, &end_of_left_bar, obj);

	if (number_of_bars != 1)
		return 0;

	number_of_bars =
		count_horizontal_bars (pix, box_left,
				       (start_of_left_bar +
					end_of_left_bar) / 2, start, end, obj);

	if (number_of_bars != 1)
	{
		number_of_bars =
			count_horizontal_bars (pix, box_left,
					       (start_of_left_bar +
						end_of_left_bar) / 2 - 1, start,
					       end, obj);

		if (number_of_bars != 1)
			return 0;
	}

	if (*start > (font.y1 + font.hight / 3) ||
	    *end < (font.y2 - font.hight / 3))
		return 0;

	return 1;
}

int
find_vertical_right_bar (hocr_pixbuf * pix, hocr_box font, unsigned int obj,
			 int *start, int *end)
{
	int number_of_bars;
	int end_of_right_bar;
	int start_of_right_bar;

	hocr_box box_right;

	box_right.x1 = font.x2 - font.width / 3;
	box_right.x2 = font.x2 + 2;
	box_right.y1 = font.y1;
	box_right.y2 = font.y2;
	box_right.width = font.width / 3 + 2;
	box_right.hight = font.hight;

	*start = 0;
	*end = 0;

	/* horizontal bars */
	number_of_bars =
		count_vertical_bars (pix, box_right, font.y1 + font.hight / 2,
				     &start_of_right_bar, &end_of_right_bar,
				     obj);

	if (number_of_bars != 1)
		return 0;

	number_of_bars =
		count_horizontal_bars (pix, box_right,
				       (start_of_right_bar +
					end_of_right_bar) / 2, start, end, obj);

	if (number_of_bars != 1)
	{
		number_of_bars =
			count_horizontal_bars (pix, box_right,
					       (start_of_right_bar +
						end_of_right_bar) / 2 - 1,
					       start, end, obj);

		if (number_of_bars != 1)
			return 0;
	}

	if (*start > (font.y1 + font.hight / 3) ||
	    *end < (font.y2 - font.hight / 3))
		return 0;

	return 1;
}

int
find_empty_middle (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	return is_empty (pix, font.x1 + font.width / 3,
			 font.y1 + font.hight / 3,
			 font.x2 - font.width / 3,
			 font.y2 - font.hight / 3, obj);
}

/*
 * start of font specific functions 
 */

int
has_alef_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int y_top;
	int y_bottom;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 5,
				     &y_top, &y_bottom, obj);

	if (number_of_bars < 2)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + 4 * font.hight / 5,
				     &y_top, &y_bottom, obj);

	if (number_of_bars < 2)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + 6 * font.hight / 7,
				     &y_top, &y_bottom, obj);

	if (number_of_bars < 2)
		return 0;

	if (find_horizontal_notch_to_right_down
	    (pix, font.x1 + font.width / 2, font.y1 + font.hight / 2,
	     font.x2, font.y2, obj) == 0)
		return 0;

	if (find_horizontal_notch_to_right_up
	    (pix, font.x1 + font.width / 2, font.y1,
	     font.x2, font.y2 - font.hight / 4, obj) == 0)
		return 0;

	return 1;
}

int
has_bet_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int end, start, end2, start2;
	int number_of_bars;

	/* helps if patach is atached */
	font.y2 += 3;
	font.hight += 3;

	/* not gimel */
	if (find_vertical_notch_down_to_right
	    (pix, font.x1 + font.width / 2, font.y1 + 2 * font.hight / 3,
	     font.x2, font.y2, obj) == 1)
		return 0;

	/* not tzadi */
	if (find_vertical_notch_up_to_right
	    (pix, font.x1 + font.width / 2, font.y1,
	     font.x2, font.y1 + font.hight / 3, obj) != 0)
		return 0;

	/* not caf */
	if (!find_horizintal_bottom_bar (pix, font, obj, &start, &end))
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start2, &end2, obj);

	if (number_of_bars != 1 || end < end2)
		return 0;

	/* not nun */
	if (!find_horizintal_top_bar (pix, font, obj, &start, &end))
		return 0;

	if (start > (font.x1 + font.width / 5))
		return 0;

	if (!find_small_horizontal_notch_to_right_down
	    (pix, font.x2 - font.width / 6, font.y1 + font.hight / 2,
	     font.x2 + 3, font.y2, obj))
		return 0;

	if (((double) font.hight / (double) font.width) > 1.5)
	{
		font.x1 -= 2;
		font.x2 += 2;
		font.width += 4;

		if (!find_horizintal_bottom_bar (pix, font, obj, &start, &end))
			return 0;
		if (!find_horizintal_top_bar (pix, font, obj, &start2, &end2))
			return 0;
		if (start2 > (start + 2))
			return 0;
	}

	return 1;
}

int
has_gimel_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int end_of_right_bar;
	int start_of_right_bar;
	int start_of_top_bar;
	int end_of_top_bar;

	/* not caf */
	find_horizintal_top_bar
		(pix, font, obj, &start_of_top_bar, &end_of_top_bar);

	/* not caf */
	if (find_vertical_notch_down_to_right
	    (pix, font.x1 + font.width / 3, font.y1 + font.hight / 2,
	     font.x2 + 6, font.y2 + 6, obj) != 1)
		return 0;

	/* if wide font check top bar start */
	if (font.width && ((double) font.hight / (double) font.width) < 1.7)
	{
		if (start_of_top_bar < (font.x1 + font.width / 10))
			return 0;
	}
	else
	{
		if (find_horizintal_bottom_bar
		    (pix, font, obj, &start_of_top_bar, &end_of_top_bar))
		{
			number_of_bars =
				count_vertical_bars (pix, font, font.y2 - 2,
						     &start_of_right_bar,
						     &end_of_right_bar, obj);

			if (number_of_bars == 2)
				return 1;
		}
	}

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 3,
				     &start_of_right_bar, &end_of_right_bar,
				     obj);

	if (number_of_bars != 1)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 3,
				     &start_of_right_bar, &end_of_right_bar,
				     obj);

	if (number_of_bars != 1)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start_of_right_bar, &end_of_right_bar,
				     obj);

	if (number_of_bars != 1)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y2 - 2,
				     &start_of_right_bar, &end_of_right_bar,
				     obj);

	if (number_of_bars == 2)
	{
		return 1;
	}

	return 0;
}

int
has_dalet_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
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
				       &start_of_top_bar, &end_of_top_bar, obj);

	if (number_of_bars != 1)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start_of_right_bar, &end_of_right_bar,
				     obj);

	if (number_of_bars != 1)
		return 0;

	/* this is not top bar */
	if (!find_horizintal_top_bar
	    (pix, font, obj, &x_start_of_top_bar, &x_end_of_top_bar))
		return 0;

	/* is dalet ? */
	if (end_of_right_bar >= x_end_of_top_bar)
		return 0;

	return 1;
}

int
has_he_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;

	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 2,
				       &start_of_top_bar, &end_of_top_bar, obj);

	if (number_of_bars != 1)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + 3 * font.hight / 4,
				     &start_of_right_bar, &end_of_right_bar,
				     obj);

	if (number_of_bars != 2)
		return 0;

	/* this is not top bar */
	if (end_of_top_bar > (font.y1 + font.hight / 2))
		return 0;

	/* if not het */
	if (find_horizontal_path
	    (pix, font.x1, end_of_top_bar - 1, font.x1 + font.width / 2,
	     end_of_top_bar + 5, obj) == 0)
		return 0;

	return 1;
}

int
has_vav_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;
	int x_end_of_top_bar;
	int x_start_of_top_bar;

	/* helps if patach is atached */
	font.y1 = pix->objects[obj].y1;
	font.y2 = pix->objects[obj].y2;
	font.hight = font.y2 - font.y1;

	/* chek if this is realy is a thin font */
	if (font.width && ((double) font.hight / (double) font.width) < 1.9)
		return 0;

	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 2,
				       &start_of_top_bar, &end_of_top_bar, obj);

	// if (number_of_bars != 1)
	// return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 3,
				     &start_of_right_bar, &end_of_right_bar,
				     obj);

	if (number_of_bars != 1)
		return 0;

	/* this is not top bar */
	if (end_of_top_bar > (font.y1 + font.hight / 2))
		return 0;

	/* is zain */
	number_of_bars =
		count_vertical_bars (pix, font,
				     (end_of_top_bar + start_of_top_bar) / 2,
				     &x_start_of_top_bar, &x_end_of_top_bar,
				     obj);

	if (number_of_bars != 1)
		return 0;

	if (end_of_right_bar < (x_end_of_top_bar - 3))
		return 0;

	/* nun */
	if (find_horizintal_bottom_bar
	    (pix, font, obj, &x_start_of_top_bar, &x_end_of_top_bar))
		return 0;

	return 1;
}

int
has_zain_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int end_of_right_bar;
	int start_of_right_bar;

	/* helps if patach is atached */
	font.y2 -= 2;
	font.hight -= 2;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start_of_right_bar, &end_of_right_bar,
				     obj);

	if (number_of_bars != 1)
		return 0;

	/* check to see if not nun */
	if (find_vertical_path
	    (pix, start_of_right_bar - 5, font.y1 + font.hight / 2,
	     start_of_right_bar + 2, font.y2, obj) == 0)
		return 0;

	/* check to see if not vav */
	if (find_horizontal_notch_to_right_up
	    (pix, end_of_right_bar - 2, font.y1, font.x2,
	     font.y1 + font.hight / 2, obj) == 0)
		return 0;

	return 1;
}

int
has_het_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int start, end;

	/* helps if patach is atached */
	font.y1 = pix->objects[obj].y1;
	font.y2 = pix->objects[obj].y2;
	font.hight = font.y2 - font.y1;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + 2 * font.hight / 3,
				     &start, &end, obj);

	if (number_of_bars != 2)
		return 0;

	/* if not tav */
	if (find_horizontal_notch_to_left_down
	    (pix, font.x1, font.y1 + 2 * font.hight / 3,
	     font.x1 + font.width / 2, font.y2, obj) != 0)
		return 0;

	/* if not mem */
	if (find_vertical_path
	    (pix, end - 7, font.y1 + 2 * font.hight / 3, end - 2,
	     font.y2, obj) == 0)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + 2 * font.hight / 3,
				     &start, &end, obj);

	if (number_of_bars != 2)
		return 0;

	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 2,
				       &start, &end, obj);

	if (start > (font.y1 + font.hight / 4))
		return 0;

	if (end > (font.y1 + font.hight / 2))
		return 0;

	if (number_of_bars != 1)
		return 0;

	return 1;
}

int
has_tet_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int start, end;
	hocr_box box_down;
	int start_of_bottom_bar, end_of_bottom_bar;

	/* helps if patach is atached */
	font.y2 += 6;
	font.hight += 6;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start, &end, obj);

	if (number_of_bars != 2)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + 4 * font.hight / 7,
				     &start, &end, obj);

	if (number_of_bars != 2)
		return 0;

	if (find_tet_mark (pix, font, obj) == 0)
		return 0;

	if (thin_lines (pix, font, obj) == 0 && find_vertical_notch_down_to_left
	    (pix, font.x1, font.y1 + 2 * font.hight / 3,
	     font.x1 + font.width / 4, font.y2, obj) == 1)
		return 0;

	if (thin_lines (pix, font, obj) == 1 && find_vertical_notch_down_to_left
	    (pix, font.x1, font.y1 + 2 * font.hight / 3,
	     font.x1 + font.width / 2, font.y2, obj) == 1)
		return 0;

	if (find_horizontal_notch_to_left_down
	    (pix, font.x1, font.y1 + 2 * font.hight / 3,
	     font.x1 + font.width / 2, font.y2, obj) == 1)
		return 0;

	if (thin_lines == 0 && find_vertical_notch_down_to_left
	    (pix, font.x1, font.y1 + font.hight / 2,
	     font.x1 + font.width / 2, font.y2, obj) == 0)
		return 0;

	if (has_black_left_bottom_mark (pix, font, obj) == 1)
		return 0;

	/* tet has bottom bar */
	box_down.x1 = font.x1;
	box_down.x2 = font.x2;
	box_down.y1 = font.y2 - font.hight / 3;
	box_down.y2 = font.y2 + 2;
	box_down.width = font.width;
	box_down.hight = font.hight / 3 + 2;

	/* horizontal bars */
	number_of_bars =
		count_horizontal_bars (pix, box_down, font.x1 + font.width / 2,
				       &start_of_bottom_bar, &end_of_bottom_bar,
				       obj);

	if (number_of_bars != 1)
		return 0;

	return 1;
}

int
has_yud_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;

	if (find_horizontal_notch_to_left_up
	    (pix, font.x1, font.y1, font.x1 + font.width / 2,
	     font.y1 + font.hight / 2, obj) == 0)
		return 0;

	/* horizontal bars */
	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start_of_top_bar, &end_of_top_bar, obj);

	if (number_of_bars != 1)
		return 0;

	return 1;
}

int
has_kaf_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;
	int start, end, start2, end2;

	/* helps if patach is atached */
	font.y1 = pix->objects[obj].y1;
	font.y2 = pix->objects[obj].y2;
	font.hight = font.y2 - font.y1;

	/* kaf is wide font */
	if (font.width && ((double) font.hight / (double) font.width) > 2.0)
		return 0;

	/* horizontal bars */
	number_of_bars =
		count_horizontal_bars (pix, font, font.x1 + font.width / 2,
				       &start_of_top_bar, &end_of_top_bar, obj);

	if (number_of_bars != 2)
		return 0;

	if (find_vertical_path
	    (pix, font.x1 + font.width / 3, font.y1,
	     font.x1 + 2 * font.width / 3, font.y1 + font.hight / 3, obj) == 1)
		return 0;

	/* vertical bars */

	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start_of_right_bar, &end_of_right_bar,
				     obj);

	if (number_of_bars != 1)
		return 0;

	/* this is not right bar */
	if (end_of_right_bar < (font.x1 + font.width / 2))
		return 0;

	/* not pe */
	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 3,
				     &start_of_right_bar, &end_of_right_bar,
				     obj);

	if (number_of_bars != 1)
	{
		number_of_bars =
			count_vertical_bars (pix, font,
					     font.y1 + 2 * font.hight / 5,
					     &start_of_right_bar,
					     &end_of_right_bar, obj);

		if (number_of_bars != 1)
			return 0;
	}

	if (((double) font.hight / (double) font.width) > 1.9)
	{
		font.x1 -= 2;
		font.x2 += 2;
		font.width += 4;

		if (!find_horizintal_bottom_bar (pix, font, obj, &start, &end))
			return 0;
		if (!find_horizintal_top_bar (pix, font, obj, &start2, &end2))
			return 0;
		if (start2 > (start + 1))
			return 0;
	}

	return 1;
}

int
has_kaf_sofit_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{

	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;

	/* helps if patach is atached */
	font.y1 = pix->objects[obj].y1;
	font.y2 = pix->objects[obj].y2;
	font.hight = font.y2 - font.y1;

	/* if not tall or too tall font not kaf sofit */
	if (((double) font.hight / (double) font.width) < 1.5 ||
	    ((double) font.hight / (double) font.width) > 3.1)
	{
		return 0;
	}

	number_of_bars =
		count_horizontal_bars (pix, font,
				       font.x1 + font.width / 2,
				       &start_of_top_bar, &end_of_top_bar, obj);

	if (number_of_bars != 1)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + font.hight / 2,
				     &start_of_right_bar,
				     &end_of_right_bar, obj);

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
	     font.y2, obj) == 0)
		return 0;

	/* if not tzadi sofit */
	if (find_vertical_path
	    (pix, font.x1 + font.width / 3, font.y1,
	     font.x1 + 2 * font.width / 3, font.y1 + font.hight / 8, obj) == 1)
		return 0;

	return 1;
}

int
has_lamed_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars, start_of_right_bar, end_of_right_bar;

	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + 2 * font.hight / 3,
				     &start_of_right_bar,
				     &end_of_right_bar, obj);

	if (number_of_bars != 1)
		return 0;

	return 1;
}

int
has_mem_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;

	/* helps if patach is atached */
	font.y1 = pix->objects[obj].y1;
	font.y2 = pix->objects[obj].y2;
	font.hight = font.y2 - font.y1;

	if (font.width < 10 || font.hight < 15)
		return 0;

	/* vertical bars */
	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + 2 * font.hight / 3,
				     &start_of_right_bar,
				     &end_of_right_bar, obj);

	/* horizontal bars */
	number_of_bars =
		count_horizontal_bars (pix, font,
				       font.x1 + 2 * font.width / 3,
				       &start_of_top_bar, &end_of_top_bar, obj);

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

	if (find_vertical_path
	    (pix, start_of_right_bar + 1, font.y1 + font.hight / 2,
	     end_of_right_bar - 3, font.y2, obj) == 0)
		return 0;

	if (!find_vertical_notch_down_to_left (pix, font.x1, font.y1+font.hight/2, font.x1+ font.width/3, font.y2, obj))
		return 0;
	
	return 1;
}

int
has_mem_sofit_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int end_of_bottom_bar;
	int start_of_bottom_bar;
	int end_of_right_bar;
	int start_of_right_bar;

	/* helps if patach is atached */
	/* helps if patach is atached */
	font.y1 = pix->objects[obj].y1;
	font.y2 = pix->objects[obj].y2;
	font.hight = font.y2 - font.y1;

	/* is sqare */
	number_of_bars =
		count_horizontal_bars (pix, font,
				       font.x1 + font.width / 2,
				       &start_of_bottom_bar,
				       &end_of_bottom_bar, obj);

	if (number_of_bars != 2)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + font.hight / 2,
				     &start_of_right_bar,
				     &end_of_right_bar, obj);

	if (number_of_bars != 2)
		return 0;

	/* not ayin */
	if (find_horizontal_notch_to_left_down
	    (pix, font.x1, font.y2 - font.hight / 2,
	     font.x1 + font.width / 3, font.y2, obj) == 1)
		return 0;

	/* not tet */
	if (find_tet_mark (pix, font, obj))
		return 0;

	/* not shamech */
	font.x2 = font.x1 + font.width / 2;
	font.width = font.width / 2;
	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + font.hight / 2,
				     &start_of_right_bar,
				     &end_of_right_bar, obj);
	font.x1 = start_of_right_bar;
	font.width = font.x2 - font.x1;
	font.y2 = end_of_bottom_bar;
	font.hight = font.y2 - font.y1;

	if (has_black_left_bottom_mark (pix, font, obj) != 1)
		return 0;

	return 1;
}

int
has_nun_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int end_of_right_bar;
	int start_of_right_bar;
	int end_of_top_bar;
	int start_of_top_bar;

	/* helps if patach is atached */
	font.y1 = pix->objects[obj].y1;
	font.y2 = pix->objects[obj].y2;
	font.hight = font.y2 - font.y1;

	/* not caf */
	find_horizintal_top_bar
		(pix, font, obj, &start_of_top_bar, &end_of_top_bar);

	/* not tzadi */
	if (find_vertical_notch_up_to_left (pix,
					    font.x1, font.y1,
					    font.x1 + font.width / 2,
					    font.y1 + font.hight / 3,
					    obj)
	    && find_vertical_notch_up_to_right (pix,
						font.x1 +
						font.width / 2, font.y1,
						font.x2,
						font.y1 + font.hight / 3, obj))
		return 0;

	/* if wide font check top bar start */
	if (font.width && ((double) font.hight / (double) font.width) < 2.1)
	{
		if (start_of_top_bar < (font.x1 + font.width / 7))
			return 0;
	}
	else
	{
		if (find_horizintal_bottom_bar
		    (pix, font, obj, &start_of_top_bar, &end_of_top_bar))
		{
			number_of_bars =
				count_vertical_bars (pix, font,
						     font.y2 - 2,
						     &start_of_right_bar,
						     &end_of_right_bar, obj);

			if (number_of_bars == 1)
				return 1;
		}
	}

	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + font.hight / 2,
				     &start_of_right_bar,
				     &end_of_right_bar, obj);

	if (number_of_bars != 1)
		return 0;

	if (end_of_right_bar < (font.x2 - font.width / 6))
		return 0;

	/* horizontal bars */
	number_of_bars =
		count_horizontal_bars (pix, font,
				       font.x1 + font.width / 2,
				       &start_of_top_bar, &end_of_top_bar, obj);

	if (!thin_lines (pix, font, obj) && number_of_bars != 2)
	{
		number_of_bars =
			count_horizontal_bars (pix, font,
					       font.x1 + font.width / 3,
					       &start_of_top_bar,
					       &end_of_top_bar, obj);

		if (!thin_lines (pix, font, obj) && number_of_bars != 2)
			return 0;
	}

	if (font.width && thin_lines (pix, font, obj)
	    && (font.hight / font.width) < 2)
		return 0;

	if (end_of_top_bar < (font.y2 - font.hight / 6))
		return 0;

	/* not pe */
	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 3,
				     &start_of_right_bar, &end_of_right_bar,
				     obj);
	if (number_of_bars != 1)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + 2 * font.hight / 5,
				     &start_of_right_bar,
				     &end_of_right_bar, obj);

	if (number_of_bars != 1)
		return 0;

	return 1;
}

int
has_nun_sofit_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	if (font.width && (((double) font.hight / (double) font.width) < 2.5))
		return 0;

	return 1;
}

int
has_sameh_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int end_of_bottom_bar;
	int start_of_bottom_bar;
	int end_of_right_bar;
	int start_of_right_bar;

	/* helps if patach is atached */
	font.y1 = pix->objects[obj].y1;
	font.y2 = pix->objects[obj].y2;
	font.hight = font.y2 - font.y1;

	/* is sqare */
	number_of_bars =
		count_horizontal_bars (pix, font,
				       font.x1 + font.width / 2,
				       &start_of_bottom_bar,
				       &end_of_bottom_bar, obj);

	if (number_of_bars != 2)
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + font.hight / 2,
				     &start_of_right_bar,
				     &end_of_right_bar, obj);

	if (number_of_bars != 2)
		return 0;

	/* not ayin */
	if (find_horizontal_notch_to_left_down
	    (pix, font.x1, font.y2 - font.hight / 2,
	     font.x1 + font.width / 3, font.y2, obj) == 1)
		return 0;

	/* not tet */
	if (find_tet_mark (pix, font, obj))
		return 0;

	/* not mem sofit */
	font.x2 = font.x1 + font.width / 2;
	font.width = font.width / 2;
	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + font.hight / 2,
				     &start_of_right_bar,
				     &end_of_right_bar, obj);
	font.x1 = start_of_right_bar;
	font.width = font.x2 - font.x1;
	font.y2 = end_of_bottom_bar;
	font.hight = font.y2 - font.y1;

	if (has_black_left_bottom_mark (pix, font, obj) == 1)
		return 0;

	return 1;
}

int
has_ayin_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int end_of_right_bar;
	int start_of_right_bar;

	/* helps if patach is atached */
	font.y1 = pix->objects[obj].y1;
	font.y2 = pix->objects[obj].y2;
	font.hight = font.y2 - font.y1;

	/* vertical bars */
	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + font.hight / 3,
				     &start_of_right_bar,
				     &end_of_right_bar, obj);

	if (number_of_bars != 2)
	{
		number_of_bars =
			count_vertical_bars (pix, font,
					     font.y1 + font.hight / 2,
					     &start_of_right_bar,
					     &end_of_right_bar, obj);

		if (number_of_bars != 2)
			return 0;
	}

	if (find_horizontal_notch_to_left_down
	    (pix, font.x1, font.y2 - font.hight / 2,
	     font.x1 + font.width / 3, font.y2, obj) != 1)
		return 0;

	/* not tzadi */
	if (find_horizontal_notch_to_right_down
	    (pix, font.x2 - font.width / 2, font.y2 - font.hight / 2,
	     font.x2, font.y2, obj) == 1)
		return 0;

	if (has_black_left_bottom_mark (pix, font, obj) == 0)
		return 0;

	/* not pe */

	if (find_vertical_notch_up_to_right
	    (pix, font.x2 - font.width / 2, font.y1, font.x2,
	     font.y1 + font.hight / 3, obj) != 1)
		return 0;

	/* not tav */
	if (find_vertical_notch_down_to_right
	    (pix, font.x2 - font.width / 3, font.y2 - font.hight / 3,
	     font.x2, font.y2, obj) == 1)
		return 0;

	return 1;
}

int
has_pe_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;

	// * helps if patach is atached */
	font.y1 = pix->objects[obj].y1;
	font.y2 = pix->objects[obj].y2;
	font.hight = font.y2 - font.y1;

	/* vertical bars */
	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + font.hight / 6,
				     &start_of_top_bar, &end_of_top_bar, obj);

	/* not ayin */
	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + font.hight / 2,
				     &start_of_right_bar,
				     &end_of_right_bar, obj);
	if (end_of_right_bar < (font.x1 + 2 * font.width / 3))
		return 0;
	if (end_of_right_bar < end_of_top_bar)
		return 0;

	/* horizontal bars */
	number_of_bars =
		count_horizontal_bars (pix, font,
				       font.x1 + font.width / 2,
				       &start_of_top_bar, &end_of_top_bar, obj);

	if (end_of_top_bar < (font.y1 + 2 * font.hight / 3))
		return 0;
	if (start_of_top_bar > (font.y1 + font.hight / 3))
		return 0;

	/* not tzadi */
	if (find_horizontal_notch_to_right_down
	    (pix, font.x1, font.y1 + 2 * font.hight / 3, font.x2,
	     font.y2, obj) == 1)
		return 0;

	/* not shin */
	if (find_horizontal_notch_to_left_down
	    (pix, font.x1, font.y1 + font.hight / 2,
	     font.x1 + font.width / 2, font.y2, obj) != 1)
		return 0;

	/* not tzadi */
	if (find_horizontal_notch_to_right_up
	    (pix, font.x1 + font.width / 2, font.y1, font.x2,
	     font.y1 + font.hight / 3, obj) == 1)
		return 0;

	/* pe can never be too thin */
	if (((double) font.hight / (double) font.width) > 2.8)
	{
		return 0;
	}

	return 1;
}

int
has_pe_sofit_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int end_of_right_bar;
	int start_of_right_bar;

	/* helps if patach is atached */
	font.y1 = pix->objects[obj].y1;
	font.y2 = pix->objects[obj].y2;
	font.hight = font.y2 - font.y1;

	/* vertical bars */
	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + font.hight / 3,
				     &start_of_right_bar,
				     &end_of_right_bar, obj);
	if (number_of_bars != 2)
		return 0;
	if (find_horizontal_path
	    (pix, font.x1, font.y1 + font.hight / 3,
	     font.x1 + 2 * font.width / 3, font.y2 - 3, obj) == 0)
		return 0;
	if (has_black_right_bottom_mark (pix, font, obj) == 0)
		return 0;
	if (has_black_left_bottom_mark (pix, font, obj) == 1)
		return 0;
	if (find_vertical_path
	    (pix, font.x1 + font.width / 3, font.y1,
	     font.x1 + 2 * font.width / 3, font.y1 + font.hight / 3, obj) == 1)
		return 0;
	if (is_empty
	    (pix, font.x1, font.y1 + 2 * font.hight / 3,
	     font.x1 + font.width / 2, font.y2, obj) == 0)
		return 0;

	/* not tzadi */
	if (find_horizontal_notch_to_right_up
	    (pix, font.x1 + font.width / 2, font.y1, font.x2,
	     font.y1 + font.hight / 5, obj) == 1)
		return 0;

	return 1;
}

int
has_tzadi_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;
	int end_of_bottom_bar;
	int start_of_bottom_bar;
	hocr_box box_down;

	/* helps if patach is atached */
	font.y1 = pix->objects[obj].y1;
	font.y2 = pix->objects[obj].y2;
	font.hight = font.y2 - font.y1;

	if (find_horizontal_notch_to_left_down
	    (pix, font.x1, font.y2 - font.hight / 2,
	     font.x1 + font.width / 3, font.y2, obj) != 1)
		return 0;

	if (find_horizontal_notch_to_right_up
	    (pix, font.x1 + font.width / 2, font.y1, font.x2,
	     font.y1 + font.hight / 2, obj) == 0)
		return 0;

	/* tzadi has a bottom bar */
	box_down.x1 = font.x1;
	box_down.x2 = font.x2;
	box_down.y1 = font.y2 - font.hight / 3;
	box_down.y2 = font.y2 + 2;
	box_down.width = font.width;
	box_down.hight = font.hight / 3 + 2;

	/* horizontal bars */
	number_of_bars =
		count_horizontal_bars (pix, box_down,
				       font.x1 + font.width / 3,
				       &start_of_bottom_bar,
				       &end_of_bottom_bar, obj);

	if (number_of_bars != 1)
		return 0;

	/* vertical bars */
	number_of_bars =
		count_vertical_bars (pix, font,
				     start_of_bottom_bar - 3,
				     &start_of_right_bar,
				     &end_of_right_bar, obj);
	if (number_of_bars != 1)
		return 0;

	if (find_horizontal_path
	    (pix, font.x1, font.y1 + (double) font.hight / 3.0,
	     font.x1 + 1 * font.width / 4, font.y2 - 3, obj) == 0)
		return 0;

	number_of_bars =
		count_horizontal_bars (pix, font,
				       font.x1 + font.width / 4,
				       &start_of_top_bar, &end_of_top_bar, obj);
	if (number_of_bars == 1)
		return 0;

	return 1;
}

int
has_tzadi_sofit_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int end_of_right_bar;
	int start_of_right_bar;

	/* helps if patach is atached */
	font.y1 = pix->objects[obj].y1;
	font.y2 = pix->objects[obj].y2;
	font.hight = font.y2 - font.y1;

	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + font.hight / 6,
				     &start_of_right_bar,
				     &end_of_right_bar, obj);
	if (number_of_bars != 2)
	{
		number_of_bars =
			count_vertical_bars (pix, font,
					     font.y1 + 1,
					     &start_of_right_bar,
					     &end_of_right_bar, obj);
		if (number_of_bars != 2)
			return 0;
	}

	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + 5 * font.hight / 6,
				     &start_of_right_bar,
				     &end_of_right_bar, obj);
	if (number_of_bars != 1)
		return 0;

	if (!thin_lines (pix, font, obj))
	{
		if (find_horizontal_notch_to_left_down
		    (pix, font.x1,
		     font.y2 - font.hight / 3, font.x1 + font.width / 6,
		     font.y2, obj) == 1)
			return 0;
	}

	return 1;
}

int
has_kof_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;

	/* helps if patach is atached */
	font.y1 = pix->objects[obj].y1;
	font.y2 = pix->objects[obj].y2;
	font.hight = font.y2 - font.y1;

	number_of_bars =
		count_horizontal_bars (pix, font,
				       font.x1 + font.width / 2,
				       &start_of_top_bar, &end_of_top_bar, obj);

	if (number_of_bars != 2)
		return 0;
	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + 7 * font.hight / 8,
				     &start_of_right_bar,
				     &end_of_right_bar, obj);
	if (number_of_bars != 1)
		return 0;
	if (end_of_right_bar > (font.x1 + font.width / 2))
		return 0;

	if (find_vertical_path
	    (pix, font.x1 + font.width / 3, font.y1,
	     font.x1 + 2 * font.width / 3, font.y1 + font.hight / 3, obj) == 1)
		return 0;

	/* not tzadi */
	if (find_vertical_notch_up_to_left (pix,
					    font.x1, font.y1,
					    font.x1 + font.width / 2,
					    font.y1 + font.hight / 3,
					    obj)
	    && find_vertical_notch_up_to_right (pix,
						font.x1 +
						font.width / 2, font.y1,
						font.x2,
						font.y1 + font.hight / 3, obj))
		return 0;

	return 1;
}

int
has_resh_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;
	int end_of_right_bar;
	int start_of_right_bar;
	int x_end_of_top_bar;
	int x_start_of_top_bar;

	/* resh is wide font */
	if (font.width && ((double) font.hight / (double) font.width) > 2.5)
		return 0;

	number_of_bars =
		count_horizontal_bars (pix, font,
				       font.x1 + font.width / 2,
				       &start_of_top_bar, &end_of_top_bar, obj);
	if (number_of_bars != 1)
		return 0;
	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + font.hight / 2,
				     &start_of_right_bar,
				     &end_of_right_bar, obj);
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
				     (end_of_top_bar +
				      start_of_top_bar) / 2,
				     &x_start_of_top_bar,
				     &x_end_of_top_bar, obj);
	if (number_of_bars != 1)
		return 0;
	if (end_of_right_bar < x_end_of_top_bar)
		return 0;
	/* if not he */
	if (is_empty
	    (pix, font.x1, end_of_top_bar + 4, start_of_right_bar - 4,
	     font.y2, obj) == 0)
		return 0;
	return 1;
}

int
has_shin_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int start, end;

	/* helps if patach is atached */
	font.y2 += 3;
	font.hight += 3;

	/* start of font */
	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + font.hight / 2, &start,
				     &end, obj);
	if (number_of_bars != 3)
	{
		number_of_bars =
			count_vertical_bars (pix, font,
					     font.y1 + font.hight / 3,
					     &start, &end, obj);
		if (number_of_bars != 3)
			return 0;
	}

	number_of_bars =
		find_vertical_double_notch_up_to_right (pix,
							font.x1 +
							font.width / 4,
							font.y1,
							font.x2,
							font.y1 +
							font.hight / 2, obj);
	// if (number_of_bars == 1)
	// return 0;

	return 1;
}

int
has_shin_two_parts_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int start, end;

	/* start of font */
	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + font.hight / 2, &start,
				     &end, obj);
	if (number_of_bars < 2)
		return 0;

	if (find_horizintal_top_bar (pix, font, obj, &start, &end) != 0)
		return 0;

	/* not ayin */
	if (find_horizontal_notch_to_left_down
	    (pix, font.x1, font.y2 - font.hight / 2,
	     font.x1 + font.width / 3, font.y2, obj) == 1)
		return 0;

	return 1;
}

int
has_tav_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int start, end;

	/* helps if patach is atached */
	font.y1 = pix->objects[obj].y1;
	font.y2 = pix->objects[obj].y2;
	font.hight = font.y2 - font.y1;

	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + font.hight / 2, &start,
				     &end, obj);
	if (number_of_bars != 2)
		return 0;
	/* if tav */
	if (find_vertical_path
	    (pix, start - 2, font.y1 + 2 * font.hight / 3, start + 2,
	     font.y2, obj) == 1)
		return 0;
	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + 2 * font.hight / 3,
				     &start, &end, obj);
	if (number_of_bars != 2)
		return 0;
	number_of_bars =
		count_horizontal_bars (pix, font,
				       font.x1 + font.width / 2, &start,
				       &end, obj);
	if (start > (font.y1 + font.hight / 6))
		return 0;
	if (end > (font.y1 + font.hight / 2))
		return 0;
	if (number_of_bars != 1)
		return 0;
	return 1;
}

int
has_open_brace_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int start, end;

	/* brace is a tall font */
	if (((double) font.hight / (double) font.width) < 2.5)
	{
		return 0;
	}

	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + font.hight / 2, &start,
				     &end, obj);

	if (number_of_bars != 1)
		return 0;

	if (end < (font.x1 + font.width / 3))
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + 5 * font.hight / 6,
				     &start, &end, obj);

	if (number_of_bars != 1)
		return 0;

	if (start > (font.x1 + font.width / 3))
		return 0;

	number_of_bars =
		count_horizontal_bars (pix, font,
				       font.x1 + font.width / 6, &start,
				       &end, obj);

	if (number_of_bars != 2)
		return 0;

	return 1;
}

int
has_close_brace_mark (hocr_pixbuf * pix, hocr_box font, unsigned int obj)
{
	int number_of_bars;
	int start, end;

	/* brace is a tall font */
	if (((double) font.hight / (double) font.width) < 2.5)
	{
		return 0;
	}

	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + font.hight / 2, &start,
				     &end, obj);

	if (number_of_bars != 1)
		return 0;

	if (start > (font.x1 + 2 * font.width / 3))
		return 0;

	number_of_bars =
		count_vertical_bars (pix, font,
				     font.y1 + 5 * font.hight / 6,
				     &start, &end, obj);

	if (number_of_bars != 1)
		return 0;

	if (end < (font.x1 + 2 * font.width / 3))
		return 0;

	number_of_bars =
		count_horizontal_bars (pix, font,
				       font.x1 + 5 * font.width / 6,
				       &start, &end, obj);

	if (number_of_bars != 2)
		return 0;

	if (find_horizontal_notch_to_left_up
	    (pix, font.x1, font.y1, font.x2,
	     font.y1 + font.hight / 3, obj))
		return 0;

	if (find_horizontal_notch_to_left_down
	    (pix, font.x1, font.y1 + 2 * font.hight / 3,
	     font.x2, font.y2, obj))
		return 0;

	return 1;
}

/**
 * font recognition code
 */

int
hocr_recognize_font (hocr_pixbuf * pix, hocr_box * fonts_line,
		     int num_of_fonts_in_line, int font_index,
		     hocr_line_eq line_eqs[2], int avg_font_hight,
		     int avg_font_width, char *chars, unsigned char command)
{
	int i;
	hocr_box font = fonts_line[font_index];
	hocr_box next_font =
		fonts_line[(font_index + 1) % num_of_fonts_in_line];
	hocr_box prev_font =
		fonts_line[(num_of_fonts_in_line + font_index -
			    1) % num_of_fonts_in_line];
	hocr_box box;
	unsigned int obj = 0;
	unsigned int box_obj = 0;
	unsigned int object_array[MAX_OBJECTS_IN_FONT];
	unsigned int box_object_array[MAX_OBJECTS_IN_FONT];

	int low_line_y;
	int high_line_y;
	int number_of_object_in_font;
	int number_of_object_in_box;
	int short_font;
	int tall_font;
	int thin_font;
	int wide_font;
	int assending_font;	/* ל */
	int dessending_font;	/* ך */
	int high_font;		/* ' */
	int low_font;		/* , */
	int has_top_bar_font, top_bar_start, top_bar_end;
	int has_bottom_bar_font, bottom_bar_start, bottom_bar_end;
	int has_left_bar_font, left_bar_start, left_bar_end;
	int has_right_bar_font, right_bar_start, right_bar_end;
	int has_empty_middle_font;
	int horizontal_font;
	int vertical_font;
	int regular_font;
	int two_part_font = FALSE;
	int found_nikud = FALSE;

	/* reset chars array */
	chars[0] = '\0';

	/* get line y */
	high_line_y = hocr_line_eq_get_y (line_eqs[1], font.x1);
	low_line_y = hocr_line_eq_get_y (line_eqs[0], font.x1);

	/* check if font is in the line ? */
	if (font.y1 > low_line_y || font.y2 < high_line_y)
	{
		/* this is not a font, may be nikud ? */
		if (next_font.x2 >= font.x1 || prev_font.x1 <= font.x2)
			return 0;
		/* check the main object (in the same x) inside the line */
		font.y1 = high_line_y;
		font.y2 = low_line_y;
		font.hight = font.y2 - font.y1;
		/* recalc font for the object inside line */
		obj = hocr_pixbuf_get_objects_in_box (pix, font, object_array);
		font.x1 = pix->objects[obj].x1;
		font.x2 = pix->objects[obj].x2;
		font.y1 = pix->objects[obj].y1;
		font.y2 = pix->objects[obj].y2;
		font.hight = font.y2 - font.y1;
		font.width = font.x2 - font.x1;
		/* FIXME: check that this is not the next font */
	}

	/* set box */
	box.x1 = font.x1;
	box.x2 = font.x2;
	box.y1 = high_line_y;
	box.y2 = low_line_y - 1;
	box.hight = font.y2 - font.y1;
	box.width = font.x2 - font.x1;
	/* count objects in font */
	obj = hocr_pixbuf_get_objects_in_box (pix, font, object_array);
	number_of_object_in_font = count_object_array (object_array);
	/* count objects in box */
	box_obj = hocr_pixbuf_get_objects_in_box (pix, box, box_object_array);
	number_of_object_in_box = count_object_array (box_object_array);
	/* get font proportions */
	short_font = font.hight < (0.8 * (double) avg_font_hight);
	tall_font = font.hight > (1.2 * (double) avg_font_hight);
	thin_font = font.width < (0.85 * (double) avg_font_width);
	wide_font = font.width > (1.2 * (double) avg_font_width);
	assending_font =
		(font.y1 < (high_line_y - (0.1 * (double) avg_font_hight)));
	dessending_font =
		(font.y2 > (low_line_y + (0.1 * (double) avg_font_hight)));
	high_font = (font.y2 < (high_line_y + 0.8 * (double) avg_font_hight));
	low_font = (font.y1 > (high_line_y + 0.5 * (double) avg_font_hight));
	/* get font rugh shape */
	has_empty_middle_font = find_empty_middle (pix, font, obj);
	has_top_bar_font =
		find_horizintal_top_bar (pix, font, obj, &top_bar_start,
					 &top_bar_end);
	has_bottom_bar_font =
		find_horizintal_bottom_bar (pix, font, obj,
					    &bottom_bar_start, &bottom_bar_end);
	has_left_bar_font =
		find_vertical_left_bar (pix, font, obj, &left_bar_start,
					&left_bar_end);
	has_right_bar_font =
		find_vertical_right_bar (pix, font, obj,
					 &right_bar_start, &right_bar_end);

	/* we asume font has non zero size */
	if (font.width && font.hight)
	{
		horizontal_font = (font.width / font.hight) >= 2;
		vertical_font = (font.hight / font.width) >= 2;
	}
	else
	{
		return -1;
	}

	/** look for none letter marks */
	/* flat font - */
	if (horizontal_font && short_font)
	{
		if (high_font)
		{
			sprintf (chars, "-");
		}
		else if (low_font)
		{
			sprintf (chars, "_");
		}
	}

	/* check for holam */
	if (!chars[0] && (command & HOCR_COMMAND_NIKUD))
	{
		if (font.y2 < high_line_y
		    && font.hight < avg_font_hight / 6
		    && font.width < avg_font_width / 5)
			sprintf (chars, "ֹ");
	}

	/* dot and comma */
	if (!chars[0])
	{
		if (short_font && low_font && thin_font &&
		    number_of_object_in_font == 1
		    && number_of_object_in_box == 1)
		{
			if (find_horizontal_notch_to_left_down
			    (pix, font.x1, font.y1 + font.hight / 2,
			     font.x1 + font.width / 2, font.y2, obj) == 1)
				sprintf (chars, ",");
			else
				sprintf (chars, ".");
		}
	}

	/* short high marks: ' yud and holam */
	if (!chars[0])
	{
		if (short_font && high_font && thin_font &&
		    number_of_object_in_font == 1
		    && number_of_object_in_box == 1)
		{
			/* check that this is not yud */
			if (find_horizontal_notch_to_left_up
			    (pix, font.x1, font.y1,
			     font.x1 + font.width / 2,
			     font.y1 + 3.0 * (double) font.hight / 4.0,
			     obj) == 0)
			{
				sprintf (chars, "'");
			}
			/* if looking for nikud check holam haser */
			else if ((command & HOCR_COMMAND_NIKUD)
				 && font.y2 <
				 (high_line_y + avg_font_hight / 5))
			{
				sprintf (chars, "ּ");
				found_nikud = TRUE;
			}
			/* yud is also high thin font */
			else if (font.width <= (4 * avg_font_width / 5))
			{
				sprintf (chars, "י");
			}
		}
	}

	/* only if looking for nikud? check suf pasuk */
	if (!chars[0])
	{
		if (!tall_font && !high_font && !low_font
		    && (font.width && font.hight / font.width > 4)
		    && number_of_object_in_font == 1
		    && number_of_object_in_box == 1)
			sprintf (chars, "|");
	}

	/* two part non letter marks: !?:; */
	if (!chars[0])
	{
		if (number_of_object_in_box == 2
		    && number_of_object_in_font == 1)
		{
			hocr_box upper_object_box;
			hocr_box lower_object_box;
			int upper_object_is_dot;
			int lower_object_is_dot;
			int start, end;

			upper_object_box.x1 =
				pix->objects[box_object_array[0]].x1;
			upper_object_box.y1 =
				pix->objects[box_object_array[0]].y1;
			upper_object_box.x2 =
				pix->objects[box_object_array[0]].x2;
			upper_object_box.y2 =
				pix->objects[box_object_array[0]].y2;
			upper_object_box.width =
				pix->objects[box_object_array[0]].width;
			upper_object_box.hight =
				pix->objects[box_object_array[0]].hight;
			lower_object_box.x1 =
				pix->objects[box_object_array[1]].x1;
			lower_object_box.y1 =
				pix->objects[box_object_array[1]].y1;
			lower_object_box.x2 =
				pix->objects[box_object_array[1]].x2;
			lower_object_box.y2 =
				pix->objects[box_object_array[1]].y2;
			lower_object_box.width =
				pix->objects[box_object_array[1]].width;
			lower_object_box.hight =
				pix->objects[box_object_array[1]].hight;
			upper_object_is_dot =
				((upper_object_box.hight <=
				  avg_font_hight / 3)
				 && (upper_object_box.y1 > high_line_y)
				 && (upper_object_box.y2 <=
				     (high_line_y + 2 * avg_font_hight / 3)));
			lower_object_is_dot =
				((lower_object_box.hight <=
				  avg_font_hight / 3)
				 && (lower_object_box.width <=
				     avg_font_width / 2)
				 && (lower_object_box.y1 >
				     (high_line_y + low_line_y) / 2)
				 &&
				 (find_horizontal_notch_to_left_down
				  (pix, lower_object_box.x1,
				   lower_object_box.y1 +
				   lower_object_box.hight / 2,
				   lower_object_box.x1 +
				   lower_object_box.width / 2,
				   lower_object_box.y2,
				   box_object_array[1]) != 1));

			/* : */
			if (lower_object_is_dot && upper_object_is_dot)
			{
				sprintf (chars, ":");
				two_part_font = TRUE;
			}
			/* ; */
			else if (upper_object_is_dot)
			{
				sprintf (chars, ";");
				two_part_font = TRUE;
			}
			else if (lower_object_is_dot)
			{
				/* ? */
				if (upper_object_box.y2 >
				    (high_line_y + low_line_y) / 2
				    &&
				    (((upper_object_box.width /
				       lower_object_box.width) >= 2)
				     ||
				     (count_vertical_bars
				      (pix, upper_object_box,
				       upper_object_box.y1 +
				       upper_object_box.hight / 3,
				       &start, &end,
				       box_object_array[0]) == 2)
				     || (start >
					 upper_object_box.x1 +
					 upper_object_box.width / 2)))
				{
					sprintf (chars, "?");
					two_part_font = TRUE;
				}
				/* ! */
				else if (upper_object_box.y2 >
					 (high_line_y + low_line_y) / 2
					 && thin_font)
				{
					sprintf (chars, "!");
					two_part_font = TRUE;
				}
			}
		}
	}

	/* check for hebrew fonts */

	/* check for assending or dessending thin fonts ( and ) */
	if (!chars[0])
	{
		if ((assending_font || dessending_font) && !high_font)
		{
			if (has_open_brace_mark (pix, font, obj))
			{
				sprintf (chars, "(");
			}
			else if (has_close_brace_mark (pix, font, obj))
			{
				sprintf (chars, ")");
			}
		}
	}

	/* check for assending font ל */
	if (!chars[0])
	{
		if (assending_font)
		{
			if (has_lamed_mark (pix, font, obj)
			    && !high_font && tall_font)
			{
				sprintf (chars, "ל");
			}
		}
	}

	/* check for two part fonts ה ק ש */
	if (!chars[0])
	{
		if (number_of_object_in_font >= 2)
		{
			i = 1;
			/* theck for dagesh */
			if (number_of_object_in_font > 2)
			{
				if (pix->objects[object_array[i]].
				    hight < (0.2 * (double) avg_font_hight))
					i = 2;
			}

			/* check that second part look like a | */
			if (pix->objects[object_array[i]].width
			    && pix->objects[object_array[i]].y1 >
			    high_line_y
			    && pix->objects[object_array[i]].width
			    && ((double) pix->objects[object_array[i]].
				hight /
				(double) pix->objects[object_array[i]].
				width) > 1.5
			    && pix->objects[object_array[i]].x2 <
			    (font.x1 + font.width / 2))
			{
				/* if seconde part is dessending it's kof */
				if (pix->objects[object_array[i]].y2 >
				    (pix->objects[object_array[0]].y2 + 3))
				{
					sprintf (chars, "ק");
					two_part_font = TRUE;
				}
				else if (has_resh_mark (pix, font, obj)
					 || has_dalet_mark (pix, font, obj))
				{
					sprintf (chars, "ה");
					two_part_font = TRUE;
				}
			}
			/* else check fot pe and shin */
			/* the middle part of shin is sometimes saperate object 
			 */
			else if (has_shin_two_parts_mark (pix, font, obj))
			{
				sprintf (chars, "ש");
				two_part_font = TRUE;
			}
		}
	}

	/* check fot high fonts י ' holam (check that it is not kotz of big
	 * font) */
	if (!chars[0])
	{
		if (high_font && short_font &&
		    pix->objects[box_obj].weight <
		    1.2 * (double) pix->objects[obj].weight)
		{
			/* check that this is not yud */
			if (font.hight && (font.width / font.hight) > 0
			    && find_horizontal_notch_to_left_up (pix,
								 font.
								 x1,
								 font.
								 y1,
								 font.
								 x1 +
								 font.
								 width /
								 2,
								 font.
								 y1 +
								 3.0 *
								 (double)
								 font.
								 hight /
								 4.0, obj) == 0)
			{
				sprintf (chars, "'");
			}
			/* if looking for nikud check holam haser */
			else if ((command & HOCR_COMMAND_NIKUD)
				 && font.y2 <
				 (high_line_y + avg_font_hight / 5))
			{
				sprintf (chars, "ּ");
				found_nikud = TRUE;
			}
			/* yud is also high thin font */
			else if (font.width <= (4 * avg_font_width / 5))
			{
				sprintf (chars, "י");
			}
		}
	}

	/* high and thin fonts desending from buttom */
	if (!chars[0])
	{
		if (tall_font && thin_font && dessending_font &&
		    !low_font && !high_font)
		{
			if (has_nun_sofit_mark (pix, font, obj))
			{
				/* nun sofit */
				sprintf (chars, "ן");
			}
			else if (has_kaf_sofit_mark (pix, font, obj)
				 && font.width && (font.hight / font.width) < 3)
			{
				/* kaf sofit */
				sprintf (chars, "ך");
			}
			else if (has_tzadi_sofit_mark (pix, font, obj))
			{
				/* tzadi */
				sprintf (chars, "ץ");
				return 0;
			}
		}
	}

	/* high fonts desending from buttom */
	if (!chars[0])
	{
		if (tall_font && dessending_font && !low_font && !high_font)
		{
			/* fonts with top bar */
			if (has_top_bar_font)
			{
				if (has_kof_mark (pix, font, obj))
				{
					sprintf (chars, "ק");
				}
				else if (has_kaf_sofit_mark (pix, font, obj))
				{
					sprintf (chars, "ך");
				}
				else if (has_pe_sofit_mark (pix, font, obj))
				{
					sprintf (chars, "ף");
				}
				else if (has_tzadi_sofit_mark (pix, font, obj))
				{
					sprintf (chars, "ץ");
				}
				else if (has_ayin_mark (pix, font, obj))
				{
					sprintf (chars, "ע");
				}
			}
			else
			{
				if (has_ayin_mark (pix, font, obj))
				{
					sprintf (chars, "ע");
				}
				else if (has_tzadi_sofit_mark (pix, font, obj))
				{
					sprintf (chars, "ץ");
				}
				else if (has_pe_sofit_mark (pix, font, obj))
				{
					sprintf (chars, "ף");
				}

			}
		}
	}

	/* if here then did not find a match in tall font maybe it's an
	 * attached nikud ? cut it */
	if (!chars[0])
	{
		/* TODO: check for attached nikud */
		if (font.y1 < high_line_y)
			font.y1 = high_line_y;
		if (font.y2 > low_line_y)
			font.y2 = low_line_y;
		font.hight = font.y2 - font.y1;
		tall_font = FALSE;
	}

	regular_font = !tall_font && !short_font && !high_font && !low_font;

	/* thin fonts */
	/* gimel , vav , zayin , tet , nun */
	if (!chars[0])
	{
		if (thin_font && regular_font)
		{
			if (has_gimel_mark (pix, font, obj))
			{
				sprintf (chars, "ג");
			}
			else if (has_vav_mark (pix, font, obj))
			{
				sprintf (chars, "ו");
			}
			else if (has_zain_mark (pix, font, obj))
			{
				sprintf (chars, "ז");
			}
			else if (has_tet_mark (pix, font, obj))
			{
				sprintf (chars, "ט");
			}
			else if (has_nun_mark (pix, font, obj))
			{
				sprintf (chars, "נ");
			}
		}
	}

	/* check for font with top bottom right and left bars */
	/* mem sofit, samech */
	if (!chars[0] && regular_font && has_top_bar_font
	    && has_bottom_bar_font && has_right_bar_font && has_left_bar_font)
	{
		if (has_mem_sofit_mark (pix, font, obj))
		{
			sprintf (chars, "ם");
		}
		else if (has_sameh_mark (pix, font, obj))
		{
			sprintf (chars, "ס");
		}
	}

	/* check for font with top right and left bars */
	/* het and tav */
	if (!chars[0] && regular_font && has_top_bar_font
	    && has_right_bar_font && has_left_bar_font)
	{
		if (has_het_mark (pix, font, obj))
		{
			sprintf (chars, "ח");
		}
		else if (has_mem_mark (pix, font, obj))
		{
			sprintf (chars, "מ");
		}
		else if (has_tav_mark (pix, font, obj))
		{
			sprintf (chars, "ת");
		}
	}

	/* check for font with top right and bottom bars */
	/* bet caf nun pe */
	if (!chars[0] && regular_font && has_top_bar_font
	    && has_right_bar_font && has_bottom_bar_font)
	{
		if (has_bet_mark (pix, font, obj))
		{
			sprintf (chars, "ב");
		}
		else if (has_gimel_mark (pix, font, obj))
		{
			sprintf (chars, "ג");
		}
		else if (has_kaf_mark (pix, font, obj))
		{
			sprintf (chars, "כ");
		}
		else if (has_nun_mark (pix, font, obj))
		{
			sprintf (chars, "נ");
		}
		else if (has_pe_mark (pix, font, obj))
		{
			sprintf (chars, "פ");
		}
	}

	/* check for font with top and right bars */
	/* gimel dalet he */
	if (!chars[0] && regular_font && has_top_bar_font && has_right_bar_font)
	{
		if (has_gimel_mark (pix, font, obj))
		{
			sprintf (chars, "ג");
		}
		else if (has_dalet_mark (pix, font, obj))
		{
			sprintf (chars, "ד");
		}
		else if (has_he_mark (pix, font, obj))
		{
			sprintf (chars, "ה");
		}
	}

	/* check for font with bottom bars */
	/* bet tet nun ayin pe tzadi shin */
	if (!chars[0] && regular_font && has_bottom_bar_font)
	{
		if (has_bet_mark (pix, font, obj))
		{
			sprintf (chars, "ב");
		}
		else if (has_tet_mark (pix, font, obj))
		{
			sprintf (chars, "ט");
		}
		else if (has_kaf_mark (pix, font, obj))
		{
			sprintf (chars, "כ");
		}
		else if (has_nun_mark (pix, font, obj))
		{
			sprintf (chars, "נ");
		}
		else if (has_ayin_mark (pix, font, obj))
		{
			sprintf (chars, "ע");
		}
		else if (has_pe_mark (pix, font, obj))
		{
			sprintf (chars, "פ");
		}
		else if (has_tzadi_mark (pix, font, obj))
		{
			sprintf (chars, "צ");
		}
		else if (has_shin_mark (pix, font, obj))
		{
			sprintf (chars, "ש");
		}
	}

	/* check all regular fonts alphbeticaly */
	/* if found then finished */
	if (!chars[0] && regular_font)
	{
		/* first look for fonts not looked for */
		if (has_alef_mark (pix, font, obj))
		{
			sprintf (chars, "א");
		}
		else if (has_mem_mark (pix, font, obj))
		{
			sprintf (chars, "מ");
		}
		else if (has_ayin_mark (pix, font, obj))
		{
			sprintf (chars, "ע");
		}
		else if (has_pe_mark (pix, font, obj))
		{
			sprintf (chars, "פ");
		}
		else if (has_tzadi_mark (pix, font, obj))
		{
			sprintf (chars, "צ");
		}

		/* other fonts */
		else if (has_bet_mark (pix, font, obj))
		{
			sprintf (chars, "ב");
		}
		else if (has_gimel_mark (pix, font, obj))
		{
			sprintf (chars, "ג");
		}
		else if (has_dalet_mark (pix, font, obj))
		{
			sprintf (chars, "ד");
		}
		else if (has_zain_mark (pix, font, obj))
		{
			sprintf (chars, "ז");
		}
		else if (has_het_mark (pix, font, obj))
		{
			sprintf (chars, "ח");
		}
		else if (has_tet_mark (pix, font, obj))
		{
			sprintf (chars, "ט");
		}
		else if (has_kaf_mark (pix, font, obj))
		{
			sprintf (chars, "כ");
		}
		else if (has_mem_sofit_mark (pix, font, obj))
		{
			sprintf (chars, "ם");
		}
		else if (has_nun_mark (pix, font, obj))
		{
			sprintf (chars, "נ");
		}
		else if (has_sameh_mark (pix, font, obj))
		{
			sprintf (chars, "ס");
		}
		else if (has_pe_mark (pix, font, obj))
		{
			sprintf (chars, "פ");
		}
		else if (has_resh_mark (pix, font, obj))
		{
			sprintf (chars, "ר");
		}
		else if (has_shin_mark (pix, font, obj))
		{
			sprintf (chars, "ש");
		}
		else if (has_tav_mark (pix, font, obj))
		{
			sprintf (chars, "ת");
		}
	}

	/* check for nikud */

	if ((command & HOCR_COMMAND_NIKUD) && chars[0])
	{
		hocr_box under_font_box;
		int number_of_object_under_font;
		unsigned int under_font_object;
		unsigned int under_font_object_array[MAX_OBJECTS_IN_FONT];

		hocr_box over_font_box;
		int number_of_object_over_font;
		unsigned int over_font_object;
		unsigned int over_font_object_array[MAX_OBJECTS_IN_FONT];

		/* if looking for nikud look for dagesh */
		if (!two_part_font && number_of_object_in_box == 2
		    && number_of_object_in_font == 2)
		{
			if (pix->objects[object_array[1]].width <
			    (avg_font_width / 3)
			    && pix->objects[object_array[1]].hight <
			    (avg_font_width / 3))
			{
				strcat (chars, "ּ");
			}
			else if (!found_nikud
				 && pix->objects[object_array[1]].
				 width > (avg_font_width / 3)
				 && pix->objects[object_array[1]].
				 hight < (avg_font_width / 3))
			{
				strcat (chars, "ַ");
				found_nikud = TRUE;
			}
			else if (!found_nikud
				 && pix->objects[object_array[1]].
				 width > (avg_font_width / 3)
				 && pix->objects[object_array[1]].
				 hight > (avg_font_width / 3))
			{
				strcat (chars, "ָ");
				found_nikud = TRUE;
			}
		}
		else if (two_part_font && number_of_object_in_box == 3
			 && number_of_object_in_font == 3)
		{
			if (pix->objects[object_array[2]].width <
			    (avg_font_width / 3)
			    && pix->objects[object_array[2]].hight <
			    (avg_font_width / 3))
			{
				strcat (chars, "ּ");
			}
			else if (!found_nikud
				 && pix->objects[object_array[1]].
				 width > (avg_font_width / 3)
				 && pix->objects[object_array[2]].
				 hight < (avg_font_width / 3))
			{
				strcat (chars, "ַ");
				found_nikud = TRUE;
			}
			else if (!found_nikud
				 && pix->objects[object_array[1]].
				 width > (avg_font_width / 3)
				 && pix->objects[object_array[2]].
				 hight > (avg_font_width / 3))
			{
				strcat (chars, "ָ");
				found_nikud = TRUE;
			}
		}

		/* if looking for nikud look under font */

		/* build the under font box */
		under_font_box.x1 = font.x1 - 2;
		under_font_box.y1 = low_line_y;
		under_font_box.x2 = font.x2 + 2;
		under_font_box.y2 = low_line_y + avg_font_hight;
		under_font_box.width = font.width + 4;
		under_font_box.hight = avg_font_hight;

		under_font_object =
			hocr_pixbuf_get_objects_inside_box (pix,
							    under_font_box,
							    under_font_object_array);
		number_of_object_under_font =
			count_object_array (under_font_object_array);

		/* one sign under font can be kamatz patach hirik */
		if (!found_nikud && number_of_object_under_font == 1)
		{
			/* hirik */
			if (pix->objects[under_font_object].hight <
			    (avg_font_hight / 3)
			    && pix->objects[under_font_object].width <
			    (avg_font_width / 3))
			{
				strcat (chars, "ִ");
				found_nikud = TRUE;
			}
			/* patach */
			else if (pix->objects[under_font_object].hight <
				 (avg_font_hight / 4))
			{
				strcat (chars, "ַ");
				found_nikud = TRUE;
			}
			/* fused shva ? */
			else if (pix->objects[under_font_object].width <
				 (avg_font_width / 3))
			{
				strcat (chars, "ְ");
				found_nikud = TRUE;
			}
			/* kamatz */
			else
			{
				strcat (chars, "ָ");
				found_nikud = TRUE;
			}
		}
		/* shva or tzere */
		else if (!found_nikud && number_of_object_under_font == 2)
		{
			if (pix->objects[under_font_object_array[0]].
			    hight < (avg_font_hight / 3)
			    && pix->objects[under_font_object_array[0]].
			    y2 > pix->objects[under_font_object_array[1]].y1)
			{
				strcat (chars, "ֵ");
				found_nikud = TRUE;
			}
			else if (pix->
				 objects[under_font_object_array[0]].
				 hight < (avg_font_hight / 3)
				 && pix->
				 objects[under_font_object_array[0]].
				 y2 <
				 pix->objects[under_font_object_array[1]].y1)
			{
				strcat (chars, "ְ");
				found_nikud = TRUE;
			}

		}
		/* segol, kubutz, hataf patach, hataf kamatz */
		else if (!found_nikud && number_of_object_under_font == 3)
		{
			/* segul kubutz */
			if (pix->objects[under_font_object].width <
			    (avg_font_width / 3))
			{
				/* kubutx */
				if (pix->
				    objects[under_font_object_array[0]].
				    y1 <
				    pix->
				    objects[under_font_object_array[1]].
				    y1
				    && pix->
				    objects[under_font_object_array[1]].
				    y1 <
				    pix->objects[under_font_object_array[2]].y1)
				{
					strcat (chars, "ֻ");
					found_nikud = TRUE;
				}
				/* segul */
				else
				{
					strcat (chars, "ֶ");
					found_nikud = TRUE;
				}
			}
			/* hataf */
			else
			{
				/* pathach */
				if (pix->objects[under_font_object].
				    hight < (avg_font_hight / 4))
				{
					strcat (chars, "ֲ");
					found_nikud = TRUE;
				}
				/* kamatz */
				else
				{
					strcat (chars, "ֳ");
					found_nikud = TRUE;
				}
			}
		}
		/* hataf tzere */
		else if (!found_nikud && number_of_object_under_font == 5)
		{
			if (pix->objects[under_font_object_array[0]].
			    hight < (avg_font_hight / 3))
			{
				strcat (chars, "ֱ");
			}
		}

		/* look above font */
		/* build the under font box */
		over_font_box.x1 = font.x1 - 2;
		over_font_box.y1 = high_line_y - avg_font_hight / 2;
		over_font_box.x2 = font.x2 + 2;
		over_font_box.y2 = high_line_y;
		over_font_box.width = font.width + 4;
		over_font_box.hight = avg_font_hight / 2;

		over_font_object =
			hocr_pixbuf_get_objects_inside_box (pix,
							    over_font_box,
							    over_font_object_array);
		number_of_object_over_font =
			count_object_array (over_font_object_array);

		/* one sign over font can be shin sin or holam */
		if (number_of_object_over_font == 1)
		{
			/* holam */
			if (pix->objects[over_font_object].hight <
			    (avg_font_hight / 3)
			    && pix->objects[over_font_object].width <
			    (avg_font_width / 3) && (thin_font
						     || (pix->
							 objects
							 [over_font_object].
							 x2 <
							 (over_font_box.
							  x1 +
							  avg_font_width / 2))))
			{
				strcat (chars, "ֹ");
				found_nikud = TRUE;
			}
		}
	}

	/* check for other letters (rashi english numbers ...) */

	/* if here and no char then no font found */
	if (!chars[0])
	{
		if (font.width > 2 * avg_font_width)
		{
			/* may be two fused fonts */
			sprintf (chars, "**");
		}
		else if ((font_index != 0
			  && font_index != (num_of_fonts_in_line - 1))
			 && (font.x1 < next_font.x2 || font.x2 > prev_font.x1))
		{
			/* may be an arteffact or part of font */
			chars[0] = '\0';
		}
		else
		{
			/* just don't konw this font */
			sprintf (chars, "*");
		}
	}

/* DEBUG: print debug info */
#ifdef DEBUG

	print_font (pix, font);
	printf ("font %d %d obj %d %d\n\n", font.hight, font.width,
		pix->objects[obj].hight, pix->objects[obj].width);
	printf ("number of objects in font %d\n", number_of_object_in_font);
	printf ("number of objects in box %d\n", number_of_object_in_box);
	printf ("short font %d\n", short_font);
	printf ("tall font %d\n", tall_font);
	printf ("thin font %d\n", thin_font);
	printf ("wide font %d\n", wide_font);
	printf ("assending font %d\n", assending_font);
	printf ("dessending font %d\n", dessending_font);
	printf ("high font %d\n", high_font);
	printf ("low font %d\n", low_font);
	printf ("has_empty_middle_font %d\n", has_empty_middle_font);
	printf ("has_top_bar_font %d\n", has_top_bar_font);
	printf ("has_bottom_bar_font %d\n", has_bottom_bar_font);
	printf ("has_left_bar_font %d\n", has_left_bar_font);
	printf ("has_right_bar_font %d\n", has_right_bar_font);
	printf ("vertical_font %d\n", vertical_font);
	printf ("horizontal_font %d\n", horizontal_font);

	printf ("fonts -\n");
	printf ("א mark %d\n", has_alef_mark (pix, font, obj));
	printf ("ב mark %d\n", has_bet_mark (pix, font, obj));
	printf ("ג mark %d\n", has_gimel_mark (pix, font, obj));
	printf ("ד mark %d\n", has_dalet_mark (pix, font, obj));
	printf ("ה mark %d\n", has_he_mark (pix, font, obj));
	printf ("ו mark %d\n", has_vav_mark (pix, font, obj));
	printf ("ז mark %d\n", has_zain_mark (pix, font, obj));
	printf ("ח mark %d\n", has_het_mark (pix, font, obj));
	printf ("ט mark %d\n", has_tet_mark (pix, font, obj));
	printf ("י mark %d\n", has_yud_mark (pix, font, obj));
	printf ("כ mark %d\n", has_kaf_mark (pix, font, obj));
	printf ("ך mark %d\n", has_kaf_sofit_mark (pix, font, obj));
	printf ("ל mark %d\n", has_lamed_mark (pix, font, obj));
	printf ("מ mark %d\n", has_mem_mark (pix, font, obj));
	printf ("ם mark %d\n", has_mem_sofit_mark (pix, font, obj));
	printf ("נ mark %d\n", has_nun_mark (pix, font, obj));
	printf ("ן mark %d\n", has_nun_sofit_mark (pix, font, obj));
	printf ("ס mark %d\n", has_sameh_mark (pix, font, obj));
	printf ("ע mark %d\n", has_ayin_mark (pix, font, obj));
	printf ("פ mark %d\n", has_pe_mark (pix, font, obj));
	printf ("ף mark %d\n", has_pe_sofit_mark (pix, font, obj));
	printf ("צ mark %d\n", has_tzadi_mark (pix, font, obj));
	printf ("ץ mark %d\n", has_tzadi_sofit_mark (pix, font, obj));
	printf ("ק mark %d\n", has_kof_mark (pix, font, obj));
	printf ("ר mark %d\n", has_resh_mark (pix, font, obj));
	printf ("ש mark %d\n", has_shin_mark (pix, font, obj));
	printf ("ש mark %d\n", has_shin_two_parts_mark (pix, font, obj));
	printf ("ת mark %d\n", has_tav_mark (pix, font, obj));

	printf ("found font: '%s'\n", chars);
	printf ("--------------------------\n\n");

#endif
	return 0;
}
