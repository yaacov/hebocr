/***************************************************************************
 *            hocr.c
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gnome.h>
#include "interface.h"

#include "hocr.h"

/* #define MAKE_DICT to create a new dictionary */

/* FIXME: this is a hack for normal pages 
   no more then 200 lines and 200 fonts per line
   better to use dynamic aloocation */

/* in units */
#define MAX_LINES 200
#define MAX_FONTS_IN_LINE 200

/* FIXME: this values are good for some book lauouts with 
   serif fonts using 300 dots per inch lineart scan
   better to find out this values from the current scaned page */

/* in pixels */
#define NORMAL_FONT_WIDTH 20
#define NORMAL_FONT_HIGHT 30

#define MAX_LINE_HIGHT 50
#define MIN_LINE_HIGHT 10

/* FIXME: this is in pixel convert to 1/1000 ? */
#define MIN_DISTANCE_BETWEEN_LINES 5
#define MIN_DISTANCE_BETWEEN_WORDS 10

/* in 1/1000 cover units */
#define NOT_IN_A_LINE 10
#define NOT_IN_A_FONT 30

#define SHORT_FONT 900
#define LONG_FONT 1200
#define THIN_FONT 900

#define FONT_ASSEND 100

/* page layout functions */
/* TODO: add support for more then one squre qulumn */

int
get_next_line_extention (GdkPixbuf * pix, int current_pos, int *line_start,
			 int *line_end)
{
	int width, height, rowstride, n_channels;
	guchar *pixels, *pixel;
	int i, x, y;
	int sum, sum1, sum2, sum3;
	int inside_line = FALSE;

	/* get pixbuf stats */
	n_channels = gdk_pixbuf_get_n_channels (pix);
	height = gdk_pixbuf_get_height (pix);
	width = gdk_pixbuf_get_width (pix);
	rowstride = gdk_pixbuf_get_rowstride (pix);
	pixels = gdk_pixbuf_get_pixels (pix);

	for (y = current_pos; y < height; y++)
	{
		/* get presentage coverage for this pixel line */
		/* a line is too long for just one sume (it may be short {1/3 length} 
		 * and aligned to center, right or left ) */
		sum1 = sum2 = sum3 = 0;
		for (x = 0; x < (width / 3); x++)
		{
			pixel = pixels + x * n_channels + y * rowstride;
			sum1 += (pixel[0] < 100) ? 1 : 0;
			pixel = pixels + (x + width / 3) * n_channels +
				y * rowstride;
			sum2 += (pixel[0] < 100) ? 1 : 0;
			pixel = pixels + (x + 2 * width / 3) * n_channels +
				y * rowstride;
			sum3 += (pixel[0] < 100) ? 1 : 0;
		}
		/* check only the part with the most color on it */
		sum = (sum1 > sum2) ? sum1 : sum2;
		sum = (sum > sum3) ? sum : sum3;
		sum = 1000 * sum / width;

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
get_next_font_extention (GdkPixbuf * pix, int line_start, int line_end,
			 int current_pos, int *font_start, int *font_end)
{
	int width, height, rowstride, n_channels;
	guchar *pixels, *pixel;
	int x, y;
	int sum;
	int inside_font = FALSE;
	/* we have to calculate line hight, we do not get it from caller */
	int line_hight = line_end - line_start;

	/* get pixbuf stats */
	n_channels = gdk_pixbuf_get_n_channels (pix);
	height = gdk_pixbuf_get_height (pix);
	width = gdk_pixbuf_get_width (pix);
	rowstride = gdk_pixbuf_get_rowstride (pix);
	pixels = gdk_pixbuf_get_pixels (pix);
	/* read line from right to left */
	for (x = current_pos - 1; x > 0; x--)
	{
		/* get presentage coverage for this pixel line */
		sum = 0;
		for (y = line_start; y < line_end; y++)
		{
			pixel = pixels + x * n_channels + y * rowstride;
			sum += (pixel[0] < 100) ? 1 : 0;
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
adjust_font_box (GdkPixbuf * pix, box * font)
{
	int width, height, rowstride, n_channels;
	guchar *pixels, *pixel;
	int x, y;
	int sum;

	/* get pixbuf stats */
	n_channels = gdk_pixbuf_get_n_channels (pix);
	height = gdk_pixbuf_get_height (pix);
	width = gdk_pixbuf_get_width (pix);
	rowstride = gdk_pixbuf_get_rowstride (pix);
	pixels = gdk_pixbuf_get_pixels (pix);

	sum = 0;
	/* read line from right to left */
	for (y = (font->y1 - MIN_DISTANCE_BETWEEN_LINES);
	     y < (font->y2 + MIN_DISTANCE_BETWEEN_LINES) && sum == 0; y++)
	{
		/* get presentage coverage for this pixel line */
		sum = 0;
		for (x = font->x1; x < font->x2; x++)
		{
			pixel = pixels + x * n_channels + y * rowstride;
			sum += (pixel[0] < 100) ? 1 : 0;
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
			pixel = pixels + x * n_channels + y * rowstride;
			sum += (pixel[0] < 100) ? 1 : 0;
		}
	}
	font->y2 = y + 1;

	return 1;
}

int
fill_lines_array (GdkPixbuf * pix, int width, box * lines,
		  int *num_of_lines, int *avg_hight, int max_lines)
{
	/* for line detection */
	int line_start;
	int line_end;
	int return_value;

	/* for line statistics */
	int line_counter;
	int sum_of_lines_hight;

	/* set initial values */
	line_end = 0;
	line_start = 0;
	line_counter = 0;
	sum_of_lines_hight = 0;

	/* get all lines in this column */
	return_value = get_next_line_extention
		(pix, line_end, &line_start, &line_end);
	while (return_value == 0 && line_counter < max_lines)
	{
		/* insert this line to lines array */
		lines[line_counter].x1 = 0;
		lines[line_counter].x2 = width;
		lines[line_counter].y1 = line_start;
		lines[line_counter].y2 = line_end;
		lines[line_counter].hight = (line_end - line_start);
		lines[line_counter].width = width;
		sum_of_lines_hight += lines[line_counter].hight;
		line_counter++;

		/* get some lee way from the end of last line */
		line_end += MIN_DISTANCE_BETWEEN_LINES;
		return_value = get_next_line_extention
			(pix, line_end, &line_start, &line_end);
	}

	/* get the number of lines and avg line hight before you leave */
	*num_of_lines = line_counter;
	*avg_hight = sum_of_lines_hight / line_counter;

	return 0;
}

int
fill_fonts_array (GdkPixbuf * pix, box line, box * fonts,
		  int *num_of_fonts, int *avg_hight, int *avg_width,
		  int max_fonts)
{
	/* for gliphs detection */
	int font_start;
	int font_end;
	int return_value;

	/* for gliphs statistics */
	int font_counter;
	int sum_of_fonts_hight;
	int sum_of_fonts_width;

	/* set initial values */
	font_end = line.x2;	/* start at the rigth of line */
	font_start = 0;
	font_counter = 0;
	sum_of_fonts_hight = 0;
	sum_of_fonts_width = 0;

	/* get all lines in this column */
	return_value = get_next_font_extention (pix,
						line.y1,
						line.y2,
						font_end,
						&font_start, &font_end);

	while (return_value == 0 && font_counter < max_fonts)
	{
		/* insert this line to lines array */
		fonts[font_counter].x1 = font_end;	/* this is right to left sweep */
		fonts[font_counter].x2 = font_start;
		fonts[font_counter].y1 = line.y1;
		fonts[font_counter].y2 = line.y2;
		fonts[font_counter].width = (font_start - font_end);

		/* adjust font hight top and bottom borders */
		adjust_font_box (pix, &(fonts[font_counter]));
		fonts[font_counter].hight =
			fonts[font_counter].y2 - fonts[font_counter].y1;

		sum_of_fonts_hight += fonts[font_counter].hight;
		sum_of_fonts_width += fonts[font_counter].width;
		font_counter++;

		return_value = get_next_font_extention (pix,
							line.y1,
							line.y2,
							font_end,
							&font_start,
							&font_end);
	}

	/* get the number of lines and avg line hight before you leave */
	*num_of_fonts = font_counter;
	*avg_hight = sum_of_fonts_hight / font_counter;
	*avg_width = sum_of_fonts_width / font_counter;

	return 0;
}

int
find_font_baseline (box * fonts, int avg_hight, int index, int num_of_fonts)
{
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
find_font_topline (box * fonts, int avg_hight, int index, int num_of_fonts)
{
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

/* 
 visualization helper finction
 */

int
color_box (GdkPixbuf * pix, box rect, int chanell, int value)
{
	int width, height, rowstride, n_channels;
	guchar *pixels, *pixel;
	int x, y;

	/* get pixbuf stats */
	n_channels = gdk_pixbuf_get_n_channels (pix);
	rowstride = gdk_pixbuf_get_rowstride (pix);
	pixels = gdk_pixbuf_get_pixels (pix);

	for (y = rect.y1; y < rect.y2; y++)
		for (x = rect.x1; x < rect.x2; x++)
		{
			pixel = pixels + x * n_channels + y * rowstride;
			pixel[chanell] = value;
		}

	return 0;
}

int
do_ocr (GdkPixbuf * pix, GtkTextBuffer * text_buffer)
{
	box lines[MAX_LINES];
	box fonts[MAX_LINES][MAX_FONTS_IN_LINE];
	int num_of_lines;
	int avg_line_hight;
	int num_of_fonts[MAX_LINES];
	int avg_font_hight;
	int avg_font_width;
	int avg_font_hight_in_page;
	int avg_font_width_in_page;
	int i, j;
	int x, y, y1, y2;
	int width;

	/* font position classes */
	int base_class;
	int top_class;
	int hight_class;
	int width_class;
	int end_of_line;
	int end_of_word;
	int end_of_paragraph;

	/* font shape markers */
	int alef_mark;
	int bet_mark;
	int gimel_mark;
	int dalet_mark;
	int he_mark;
	int vav_mark;
	int zain_mark;
	int het_mark;
	int tet_mark;
	int yud_mark;
	int kaf_mark;
	int kaf_sofit_mark;
	int lamed_mark;
	int mem_mark;
	int mem_sofit_mark;
	int nun_mark;
	int nun_sofit_mark;
	int sameh_mark;
	int ayin_mark;
	int pe_mark;
	int pe_sofit_mark;
	int tzadi_mark;
	int tzadi_sofit_mark;
	int kof_mark;
	int resh_mark;
	int shin_mark;
	int tav_mark;

	int quat_mark;
	int double_quat_mark;
	int exlem_mark;
	int question_mark;

	/* need this to put in the text_buffer */
	char chars[10];
	GtkTextIter iter;

	/* get pixbuf width */
	width = gdk_pixbuf_get_width (pix);

	/* get all lines in this column */
	fill_lines_array (pix, width, lines,
			  &num_of_lines, &avg_line_hight, MAX_LINES);
	avg_font_hight_in_page = 0;
	avg_font_width_in_page = 0;

	/* get all fonts for all the lines */
	for (i = 0; i < num_of_lines; i++)
	{
		fill_fonts_array (pix, lines[i],
				  fonts[i],
				  &(num_of_fonts[i]),
				  &avg_font_hight,
				  &avg_font_width, MAX_FONTS_IN_LINE);
		avg_font_hight_in_page += avg_font_hight;
		avg_font_width_in_page += avg_font_width;
	}

	/* get avg values per page */
	avg_font_hight_in_page = avg_font_hight_in_page / num_of_lines;
	avg_font_width_in_page = avg_font_width_in_page / num_of_lines;

	/* get all fonts for all the lines */
	for (i = 0; i < num_of_lines; i++)
	{
		for (j = 0; j < num_of_fonts[i]; j++)
		{
			/* do not waist time on arteffacts */
			if (fonts[i][j].width < 2 || fonts[i][j].hight < 2)
				continue;

			y1 = find_font_topline (fonts[i],
						avg_font_hight_in_page,
						j, num_of_fonts[i]);
			y2 = find_font_baseline (fonts[i],
						 avg_font_hight_in_page,
						 j, num_of_fonts[i]);

			/* font position and size markers 
			 */
			base_class =
				get_font_base_class (fonts[i][j].y2,
						     y2,
						     avg_font_hight_in_page);
			top_class =
				get_font_top_class (fonts[i][j].y1,
						    y1,
						    avg_font_hight_in_page);
			hight_class =
				get_font_hight_class (fonts[i][j].
						      hight,
						      avg_font_hight_in_page);
			width_class =
				get_font_width_class (fonts[i][j].
						      width,
						      avg_font_width_in_page);

			/* line markers */
			end_of_line = (j == (num_of_fonts[i] - 1)) ? 1 : 0;
			end_of_paragraph = 0;
			if (end_of_line == 0)
			{
				end_of_word =
					(fonts[i][j].x1 -
					 fonts[i][j + 1].x2) >
					MIN_DISTANCE_BETWEEN_WORDS;
			}
			else
			{
				if ((i < num_of_lines - 1)
				    && ((lines[i + 1].y1 - lines[i].y2) >
					avg_line_hight))
					end_of_paragraph = 1;

				end_of_word = 1;
			}

			/* font shape markers */

			/* TODO: this shuld be moved to the right place 
			 * and not doen unnesseraly for all fonts */

			alef_mark = has_alef_mark (pix, fonts[i][j]);
			bet_mark = has_bet_mark (pix, fonts[i][j]);
			gimel_mark = has_gimel_mark (pix, fonts[i][j]);
			dalet_mark = has_dalet_mark (pix, fonts[i][j]);
			he_mark = has_he_mark (pix, fonts[i][j]);
			vav_mark = has_vav_mark (pix, fonts[i][j]);
			zain_mark = has_zain_mark (pix, fonts[i][j]);
			het_mark = has_het_mark (pix, fonts[i][j]);
			tet_mark = has_tet_mark (pix, fonts[i][j]);
			yud_mark = has_yud_mark (pix, fonts[i][j]);
			kaf_mark = has_kaf_mark (pix, fonts[i][j]);
			kaf_sofit_mark =
				has_kaf_sofit_mark (pix, fonts[i][j]);
			lamed_mark = has_lamed_mark (pix, fonts[i][j]);
			mem_mark = has_mem_mark (pix, fonts[i][j]);
			mem_sofit_mark =
				has_mem_sofit_mark (pix, fonts[i][j]);
			nun_mark = has_nun_mark (pix, fonts[i][j]);
			nun_sofit_mark =
				has_nun_sofit_mark (pix, fonts[i][j]);
			sameh_mark = has_sameh_mark (pix, fonts[i][j]);
			ayin_mark = has_ayin_mark (pix, fonts[i][j]);
			pe_mark = has_pe_mark (pix, fonts[i][j]);
			pe_sofit_mark = has_pe_sofit_mark (pix, fonts[i][j]);
			tzadi_mark = has_tzadi_mark (pix, fonts[i][j]);
			tzadi_sofit_mark =
				has_tzadi_sofit_mark (pix, fonts[i][j]);
			kof_mark = has_kof_mark (pix, fonts[i][j]);
			resh_mark = has_resh_mark (pix, fonts[i][j]);
			shin_mark = has_shin_mark (pix, fonts[i][j]);
			tav_mark = has_tav_mark (pix, fonts[i][j]);

			quat_mark = has_quat_mark (pix, fonts[i][j]);
			double_quat_mark =
				has_double_quat_mark (pix, fonts[i][j]);
			exlem_mark = has_exlem_mark (pix, fonts[i][j]);
			question_mark = has_question_mark (pix, fonts[i][j]);

			/* if wide then arteffact */

			if (width_class == 1)
			{
				/* arteffact */
				g_sprintf (chars, "--");
			}

			/* small fonts */

			else if (hight_class == -1
				 && top_class == 1 && base_class == 1)
			{
				/* '-' */
				g_sprintf (chars, "-");
			}
			else if (hight_class == -1 && top_class == 0)
			{
				if (yud_mark == 1)
				{
					/* yud */
					g_sprintf (chars, "י");
				}
				else if (double_quat_mark == 1)
				{
					g_sprintf (chars, "\"");
				}
				else if (quat_mark == 1)
				{
					g_sprintf (chars, "\'");
				}
				else
				{
					g_sprintf (chars, "*");
				}
			}
			else if ((hight_class == -1) && (top_class == 1)
				 && (base_class == 0))
			{
				/* period */
				g_sprintf (chars, ".");
			}
			else if ((hight_class == -1) && (top_class == 1)
				 && (base_class == -1))
			{
				/* we assume comma */
				g_sprintf (chars, ",");
			}

			/* high fonts desending from top */

			else if (hight_class == 1 && top_class == -1)
			{
				/* lamed */
				g_sprintf (chars, "ל");
			}

			/* high and thin fonts desending from buttom */

			else if (hight_class == 1 && width_class == -1
				 && top_class == 0)
			{
				/* nun sofit */
				g_sprintf (chars, "ן");
			}

			/* high fonts desending from buttom */

			else if (hight_class == 1 && width_class == 0
				 && top_class == 0)
			{
				if (resh_mark == 1 || dalet_mark == 1)
				{
					/* kaf sofit */
					g_sprintf (chars, "ך");
				}
				else if (ayin_mark == 1)
				{
					/*  ayin */
					g_sprintf (chars, "ע");
				}
				else if (pe_sofit_mark == 1)
				{
					/*  pe sofit */
					g_sprintf (chars, "ף");
				}
				else if (tzadi_sofit_mark == 1)
				{
					/* tzadi */

					g_sprintf (chars, "ץ");
				}
				else if (kof_mark == 1)
				{
					/* kuf */
					g_sprintf (chars, "ק");
				}
				else
				{
					g_sprintf (chars, "*");
				}
			}

			/* thin fonts */

			else if (width_class == -1)
			{
				/* gimel 2, vav 5, zayin 6, tet 8, nun 15 */

				if (exlem_mark == 1)
				{
					/* tet */
					g_sprintf (chars, "!");
				}
				else if (question_mark == 1)
				{
					/* gimel */
					g_sprintf (chars, "?");
				}
				else if (gimel_mark == 1)
				{
					/* gimel */
					g_sprintf (chars, "ג");
				}
				else if (tet_mark == 1)
				{
					/* tet */
					g_sprintf (chars, "ט");
				}
				else if (ayin_mark == 1)
				{
					/* ayin */
					g_sprintf (chars, "ע");
				}
				else if (nun_mark == 1)
				{
					/* nun */
					g_sprintf (chars, "נ");
				}
				else if (vav_mark == 1)
				{
					/* vav */
					g_sprintf (chars, "ו");
				}
				else if (resh_mark == 1)
				{
					/* resh */
					g_sprintf (chars, "ר");
				}
				else if (zain_mark == 1)
				{
					/* zayin */
					g_sprintf (chars, "ז");
				}
				else
				{
					g_sprintf (chars, "*");
				}
			}
			/* regular fonts */
			/* TODO: sort fonts by statistical number of appearences in text */
			else
			{

				if (alef_mark == 1)
				{
					g_sprintf (chars, "א");
				}
				else if (bet_mark == 1)
				{
					g_sprintf (chars, "ב");
				}
				else if (gimel_mark == 1)
				{
					g_sprintf (chars, "ג");
				}
				else if (dalet_mark == 1)
				{
					g_sprintf (chars, "ד");
				}
				else if (he_mark == 1)
				{
					g_sprintf (chars, "ה");
				}
				else if (het_mark == 1)
				{
					g_sprintf (chars, "ח");
				}
				else if (tet_mark == 1)
				{
					g_sprintf (chars, "ט");
				}
				else if (kaf_mark == 1)
				{
					g_sprintf (chars, "כ");
				}
				else if (mem_mark == 1)
				{
					g_sprintf (chars, "מ");
				}
				else if (mem_sofit_mark)
				{
					g_sprintf (chars, "ם");
				}
				//else if (nun_mark == 1)
				//{
				//	g_sprintf (chars, "נ");
				//}
				else if (sameh_mark == 1)
				{
					g_sprintf (chars, "ס");
				}
				else if (ayin_mark)
				{
					g_sprintf (chars, "ע");
				}
				else if (pe_mark == 1)
				{
					g_sprintf (chars, "פ");
				}
				else if (tzadi_mark)
				{
					g_sprintf (chars, "צ");
				}
				else if (resh_mark == 1)
				{
					g_sprintf (chars, "ר");
				}
				else if (shin_mark == 1)
				{
					g_sprintf (chars, "ש");
				}
				else if (tav_mark == 1)
				{
					g_sprintf (chars, "ת");
				}
				else
				{
					g_sprintf (chars, "*");
				}
			}

			/* check for end of word and end of line */
			if (end_of_word == 1)
			{
				g_strlcat (chars, " ", 10);
			}
			if (end_of_line == 1)
			{
				g_strlcat (chars, "\n", 10);
			}
			if (end_of_paragraph == 1)
			{
				g_strlcat (chars, "\n", 10);
			}

			/* visual aids to see font box on screen 
			 * color_box (pix, fonts[i][j], 1, 0);
			 * print_font (pix, fonts[i][j]); */

			/* insert the string to text buffer */
			gtk_text_buffer_get_end_iter (text_buffer, &iter);
			gtk_text_buffer_insert (text_buffer,
						&iter, chars, -1);
		}

		/* print the buffer to screen if you want visual fidback 
		 * gtk_image_set_from_pixbuf (GTK_IMAGE (image), pix); */
	}

	return 0;
}
