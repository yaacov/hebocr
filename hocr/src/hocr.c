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

#include <glib.h>
#include <glib/gprintf.h>
#include <string.h>

#include "consts.h"
#include "page_layout.h"
#include "font_layout.h"
#include "font_markers.h"
#include "hocr.h"

/* 
 internal pixbuf stractures 
 */

int
hocr_pixbuf_get_n_channels (hocr_pixbuf * pix)
{
	return pix->n_channels;
}

int
hocr_pixbuf_get_height (hocr_pixbuf * pix)
{
	return pix->height;
}

int
hocr_pixbuf_get_width (hocr_pixbuf * pix)
{
	return pix->width;
}

int
hocr_pixbuf_get_rowstride (hocr_pixbuf * pix)
{
	return pix->rowstride;
}

unsigned char *
hocr_pixbuf_get_pixels (hocr_pixbuf * pix)
{
	return pix->pixels;
}

/* 
 visualization helper finction
 */

int
print_font (hocr_pixbuf * pix, box font)
{
	int width, height, rowstride, n_channels;
	unsigned char *pixels, *pixel;
	int x, y;
	int new_color;

	/* get pixbuf stats */
	n_channels = hocr_pixbuf_get_n_channels (pix);
	height = hocr_pixbuf_get_height (pix);
	width = hocr_pixbuf_get_width (pix);
	rowstride = hocr_pixbuf_get_rowstride (pix);
	pixels = hocr_pixbuf_get_pixels (pix);

	for (y = font.y1; y < (font.y2 + 0); y++)
	{
		for (x = font.x1; x < (font.x2 + 1); x++)
		{
			pixel = pixels + x * n_channels + y * rowstride;
			new_color = (pixel[0] < 100) ? 1 : 0;
			g_print ("%d", new_color);
		}
		g_print ("\n");
	}

	g_print ("\n");

	return 0;
}

int
color_box (hocr_pixbuf * pix, box rect, int chanell, int value)
{
	int width, height, rowstride, n_channels;
	unsigned char *pixels, *pixel;
	int x, y;

	/* get pixbuf stats */
	n_channels = hocr_pixbuf_get_n_channels (pix);
	rowstride = hocr_pixbuf_get_rowstride (pix);
	pixels = hocr_pixbuf_get_pixels (pix);

	for (y = rect.y1; y < rect.y2; y++)
		for (x = rect.x1; x < rect.x2; x++)
		{
			pixel = pixels + x * n_channels + y * rowstride;
			pixel[chanell] = value;
		}

	return 0;
}

/*
 */

int
hocr_do_ocr (hocr_pixbuf * pix, char *text_buffer, int max_buffer_size)
{
	box column;
	/* box column; is a place holder to a time when we add column support */
	box lines[MAX_LINES];
	box fonts[MAX_LINES][MAX_FONTS_IN_LINE];

	int num_of_fonts[MAX_LINES];
	int num_of_lines;
	int num_of_fonts_in_page;

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
	int makaf_mark;

	int unknown;

	/* need this to put in the text_buffer */
	int len;
	char chars[10];
	char tmp_chars[10];
	
	/* get all lines in this column */
	fill_lines_array (pix, column, lines, &num_of_lines, MAX_LINES);

	/* get all fonts for all the lines */
	for (i = 0; i < num_of_lines; i++)
	{
		fill_fonts_array (pix, lines[i],
				  fonts[i],
				  &(num_of_fonts[i]), MAX_FONTS_IN_LINE);
	}

	/* get size statistics for all fonts for all the lines */
	num_of_fonts_in_page = 0;
	avg_font_hight_in_page = 0;
	avg_font_width_in_page = 0;
	for (i = 0; i < num_of_lines; i++)
	{
		for (j = 0; j < num_of_fonts[i]; j++)
		{
			num_of_fonts_in_page++;
			avg_font_width_in_page += fonts[i][j].width;
			avg_font_hight_in_page += fonts[i][j].hight;
		}
	}

	if (num_of_fonts_in_page != 0)
	{
		avg_font_width_in_page /= num_of_fonts_in_page;
		avg_font_hight_in_page /= num_of_fonts_in_page;
	}

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
					2 * avg_font_hight_in_page))
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
			makaf_mark = has_makaf_mark (pix, fonts[i][j]);

			unknown = 0;

			/* if wide then arteffact */

			if (width_class == 1)
			{
				/* arteffact */
				g_sprintf (chars, "--");
				unknown = 1;
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
				if (makaf_mark == 1)
				{

					/* '-' */
					g_sprintf (chars, "-");
				}
				else if (yud_mark == 1)
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
					unknown = 1;
				}
			}
			else if ((hight_class == -1) && (top_class == 1)
				 && (base_class == 0))
			{
				/* period */
				g_sprintf (chars, ".");
			}
			else if ((hight_class == -1)
				 && (top_class == 1) && (base_class == -1))
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
					unknown = 1;
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
					unknown = 1;
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
				//      g_sprintf (chars, "נ");
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
					unknown = 1;
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

			if (unknown == 1)
				color_box (pix, fonts[i][j], 1, 255);

			/* if quat mark check for doubel quat */
			if (chars[0] == '\'')
			{
g_print ("ddd");
				
				len = strlen (text_buffer);
				tmp_chars[0] = text_buffer[len - 1];

				if (tmp_chars[0] == '\'')
				{
					text_buffer[len - 1] = '\0';
					chars[0] = '\"';
				}
			}
g_print ("%s", chars);
			g_strlcat (text_buffer, chars, max_buffer_size);

		}

	}

	return 0;
}
