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
count_obliqe_bars_minus (hocr_pixbuf * pix, hocr_box font,
			 int *first_x, int *last_x)
{
	int x, y;
	int a, b;		/* y=ax+b */
	int current_color;
	int new_color;
	int counter = 0;

	/* get the line equation */
	a = font.hight / font.width;	/* -45 deg */
	b = 0;			/* from top left corner */

	/* start with white in mind */
	current_color = 0;
	*first_x = 0;
	*last_x = 0;

	/* go axros the font + one white place */
	for (x = font.x1; x < (font.x2 + 4); x++)
	{
		y = font.y1 + a * (x - font.x1) + b;
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


/**
 */

int
count_obliqe_bars_plus (hocr_pixbuf * pix, hocr_box font,
			int *first_x, int *last_x)
{
	int x, y;
	int a, b;		/* y=ax+b */
	int current_color;
	int new_color;
	int counter = 0;

	/* get the line equation */
	a = -font.hight / font.width;	/* -45 deg */
	b = font.hight;		/* from top left corner */

	/* start with white in mind */
	current_color = 0;
	*first_x = 0;
	*last_x = 0;

	/* go axros the font + one white place */
	for (x = font.x1; x < (font.x2 + 4); x++)
	{
		y = font.y1 + a * (x - font.x1) + b;
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

/**
 */

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
has_alef_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_bet_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_gimel_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_dalet_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_he_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_vav_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_zain_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_het_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_tet_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_yud_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{
	int number_of_bars;
	int end_of_top_bar;
	int start_of_top_bar;

	//if (find_horizontal_notch_to_left_up
	//    (pix, font.x1, font.y1, font.x1 + font.width / 2,
	//     font.y1 + 2 * font.hight / 3) == 0)
	//      return 0;

	/* horizontal bars */
	number_of_bars =
		count_vertical_bars (pix, font, font.y1 + font.hight / 2,
				     &start_of_top_bar, &end_of_top_bar);

	if (number_of_bars != 1)
		return 0;

	return 1;
}

int
has_kaf_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_kaf_sofit_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{


	return 0;
}

int
has_lamed_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 1;
}

int
has_mem_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_mem_sofit_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_nun_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_nun_sofit_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 1;
}

int
has_sameh_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_ayin_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_pe_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_pe_sofit_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_tzadi_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_tzadi_sofit_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_kof_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_resh_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_shin_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_tav_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	return 0;
}

int
has_nekuda_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{
	if ((double) font.hight / (double) font.width > 1.8)
		return 0;

	return 1;
}

int
has_psik_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{
	return 1;
}

int
has_quat_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	if (marks[5] != 1)
		return 0;
	
	return 1;
}

int
has_double_quat_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{
	if (marks[5] != 2)
		return 0;

	return 1;
}

int
has_exlem_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{
	int number_of_bars;
	int start, end;

	if ((double) font.hight / (double) font.width <= 1.0)
		return 0;

	return 1;
}

int
has_dots_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{
	return 1;
}

int
has_question_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{

	int number_of_bars;
	int start, end;

	if ((double) font.hight / (double) font.width > 1)
		return 0;

	return 1;
}

int
has_makaf_mark (hocr_pixbuf * pix, hocr_box font, int *marks)
{
	if (marks[5] != 1)
		return 0;

	if (font.width > font.hight)
		return 1;

	return 0;
}

/**
 */

int
print_marks (hocr_pixbuf * pix, hocr_box font, int *marks)
{
	int start, end;

	marks[1] =
		count_horizontal_bars (pix, font,
				       font.x1 + 1 * font.width / 3, &start,
				       &end);
	marks[2] =
		count_horizontal_bars (pix, font,
				       font.x1 + 1 * font.width / 2, &start,
				       &end);
	marks[3] =
		count_horizontal_bars (pix, font,
				       font.x1 + 1 * font.width / 3, &start,
				       &end);

	marks[4] =
		count_vertical_bars (pix, font, font.y1 + 1 * font.hight / 3,
				     &start, &end);
	marks[5] =
		count_vertical_bars (pix, font, font.y1 + 1 * font.hight / 2,
				     &start, &end);
	marks[6] =
		count_vertical_bars (pix, font, font.y1 + 2 * font.hight / 3,
				     &start, &end);

	marks[7] = count_obliqe_bars_minus (pix, font, &start, &end);
	marks[8] = count_obliqe_bars_plus (pix, font, &start, &end);

	return 0;
}

/**
 */

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
	int marks[25];

	/* if font is too small this is artefact */
	if (font.hight < MIN_FONT_SIZE && font.width < MIN_FONT_SIZE)
	{
		sprintf (font_string, "");
		return 1;
	}

	/* extract font marks */
	/* TODO: fond a better place to do this */
	print_marks (pix, font, marks);

	/* normal width, high, assend over top of line fonts */
	if (hight_class == 1 && top_class == 1)
	{
		if (has_lamed_mark (pix, font, marks))
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
		if (has_nun_sofit_mark (pix, font, marks))
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
		if (has_kaf_sofit_mark (pix, font, marks))
		{
			sprintf (font_string, "ך");
			return 0;
		}

		if (has_ayin_mark (pix, font, marks))
		{
			sprintf (font_string, "ע");
			return 0;
		}

		if (has_pe_sofit_mark (pix, font, marks))
		{
			sprintf (font_string, "ף");
			return 0;
		}

		if (has_tzadi_sofit_mark (pix, font, marks))
		{
			sprintf (font_string, "ץ");
			return 0;
		}

		if (has_kof_mark (pix, font, marks))
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

			if (has_dots_mark (pix, font, marks))
			{
				sprintf (font_string, ":");
				return 0;
			}

			sprintf (font_string, "_");
			return 1;
		}
		else
		{
			if (has_makaf_mark (pix, font, marks))
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
		if (has_nekuda_mark (pix, font, marks))
		{
			sprintf (font_string, ".");
			return 0;
		}

		if (has_psik_mark (pix, font, marks))
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
		if (has_psik_mark (pix, font, marks))
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
			if (has_question_mark (pix, font, marks))
			{
				sprintf (font_string, "?");
				return 0;
			}

			if (has_exlem_mark (pix, font, marks))
			{
				sprintf (font_string, "!");
				return 0;
			}

			sprintf (font_string, "_");
			return 1;
		}
		else
		{
			if (has_makaf_mark (pix, font, marks))
			{
				sprintf (font_string, "-");
				return 0;
			}

			if (has_yud_mark (pix, font, marks))
			{
				sprintf (font_string, "י");
				return 0;
			}

			if (has_quat_mark (pix, font, marks))
			{
				sprintf (font_string, "\'");
				return 0;
			}

			if (has_double_quat_mark (pix, font, marks))
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
		if (has_gimel_mark (pix, font, marks))
		{
			sprintf (font_string, "ג");
			return 0;
		}

		if (has_vav_mark (pix, font, marks))
		{
			sprintf (font_string, "ו");
			return 0;
		}

		if (has_zain_mark (pix, font, marks))
		{
			sprintf (font_string, "ז");
			return 0;
		}

		if (has_tet_mark (pix, font, marks))
		{
			sprintf (font_string, "ט");
			return 0;
		}

		if (has_nun_mark (pix, font, marks))
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
		if (has_alef_mark (pix, font, marks))
		{
			sprintf (font_string, "א");
			return 0;
		}

		if (has_bet_mark (pix, font, marks))
		{
			sprintf (font_string, "ב");
			return 0;
		}

		if (has_gimel_mark (pix, font, marks))
		{
			sprintf (font_string, "ג");
			return 0;
		}

		if (has_dalet_mark (pix, font, marks))
		{
			sprintf (font_string, "ד");
			return 0;
		}

		if (has_he_mark (pix, font, marks))
		{
			sprintf (font_string, "ה");
			return 0;
		}

		if (has_het_mark (pix, font, marks))
		{
			sprintf (font_string, "ח");
			return 0;
		}

		if (has_tet_mark (pix, font, marks))
		{
			sprintf (font_string, "ט");
			return 0;
		}

		if (has_kaf_mark (pix, font, marks))
		{
			sprintf (font_string, "כ");
			return 0;
		}

		if (has_mem_mark (pix, font, marks))
		{
			sprintf (font_string, "מ");
			return 0;
		}

		if (has_mem_sofit_mark (pix, font, marks))
		{
			sprintf (font_string, "ם");
			return 0;
		}

		if (has_sameh_mark (pix, font, marks))
		{
			sprintf (font_string, "ס");
			return 0;
		}

		if (has_ayin_mark (pix, font, marks))
		{
			sprintf (font_string, "ע");
			return 0;
		}

		if (has_pe_mark (pix, font, marks))
		{
			sprintf (font_string, "פ");
			return 0;
		}

		if (has_tzadi_mark (pix, font, marks))
		{
			sprintf (font_string, "צ");
			return 0;
		}

		if (has_resh_mark (pix, font, marks))
		{
			sprintf (font_string, "ר");
			return 0;
		}

		if (has_shin_mark (pix, font, marks))
		{
			sprintf (font_string, "ש");
			return 0;
		}

		if (has_tav_mark (pix, font, marks))
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
