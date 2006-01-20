
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

#include <stdlib.h>

#include "hocr.h"
#include "consts.h"
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
				sum += hocr_pixbuf_get_is_object (pix, x + i,
								  y);
				sum += hocr_pixbuf_get_is_object (pix, x + i,
								  y +
								  hight_1_3);
				sum += hocr_pixbuf_get_is_object (pix, x + i,
								  y +
								  hight_2_3);
			}
		}

		/* if presantage above minmun for not in a line then we are in
		 * aline */
		if (!inside_column
		    && (x <= MIN_DISTANCE_BETWEEN_WORDS || sum > 0))
		{
			*column_end = x + MIN_DISTANCE_BETWEEN_WORDS;
			inside_column = TRUE;
		}
		/* if presantage below maximum for in a line then we need to
		 * find the end of the line by looking to the end of the down
		 * slop */
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
clean_object_list (hocr_pixbuf * pix, char *object_list, unsigned int length)
{
	unsigned int i;

	for (i = 1; i < length; i++)
		object_list[i] = 0;

	return 0;
}

int
binary_and_two_object_lists (hocr_pixbuf * pix, char *object_list1,
			     char *object_list2, char *result, unsigned int length)
{
	int counter = 0;
	unsigned int i;

	for (i = 1; i < length; i++)
		if (pix->objects[i].hight >= pix->avg_hight_of_objects)
			counter += (result[i] =
				    (object_list1[i] & object_list2[i]));

	return (counter);
}

int
count_object_list (hocr_pixbuf * pix, char *object_list, unsigned int length)
{
	int counter = 0;
	unsigned int i;

	for (i = 1; i < length; i++)
		if (pix->objects[i].hight >= pix->avg_hight_of_objects)
			counter += object_list[i];

	return (counter);
}

int
get_next_line_extention (hocr_pixbuf * pix, hocr_box column, int current_pos,
			 int *line_start, int *line_end)
{
	int x, y;
	int y_step = 1;
	int sum1 = 0;
	int sum2 = 0;
	int sum_of_binary_and_list = 0;
	int inside_line = FALSE;
	char *objects_list1;
	char *objects_list2;
	char *objects_list3;
	int width, width_1_3, width_2_3;

	/* defaults */
	*line_end = column.y2;
	*line_start = current_pos;

	/* allocate memory */
	objects_list1 = (char *) malloc (pix->num_of_max_object + 1);
	if (!objects_list1)
	{
		return 1;
	}
	objects_list2 = (char *) malloc (pix->num_of_max_object + 1);
	if (!objects_list2)
	{
		free (objects_list1);
		return 1;
	}
	objects_list3 = (char *) malloc (pix->num_of_max_object + 1);
	if (!objects_list3)
	{
		free (objects_list1);
		free (objects_list2);
		return 1;
	}

	width = column.width;
	width_1_3 = width / 3;
	width_2_3 = 2 * width / 3;

	inside_line = FALSE;
	*line_start = current_pos;

	for (y = current_pos; y < column.y2; y += y_step)
	{
		/* get presentage coverage for this pixel line */
		clean_object_list (pix, objects_list1, pix->num_of_max_object);
		clean_object_list (pix, objects_list2, pix->num_of_max_object);

		/* FIXME: jump 5 pixels each line ? */
		for (x = column.x1; x < (column.x1 + width_1_3); x++)
		{
			objects_list1[hocr_pixbuf_get_object (pix, x, y)] = 1;
			objects_list1[hocr_pixbuf_get_object
				      (pix, x + width_1_3, y)] = 1;
			objects_list1[hocr_pixbuf_get_object
				      (pix, x + width_2_3, y)] = 1;

			objects_list2[hocr_pixbuf_get_object
				      (pix, x, y + y_step)] = 1;
			objects_list2[hocr_pixbuf_get_object
				      (pix, x + width_1_3, y + y_step)] = 1;
			objects_list2[hocr_pixbuf_get_object
				      (pix, x + width_2_3, y + y_step)] = 1;

#ifdef DEBUG
			if (x < column.x1 + 10)
			{
				hocr_pixbuf_set_pixel (pix, x, y,
						       inside_line ? 2 : 3, 0);
			}
#endif
		}

		/* how many objects ? */
		sum1 = count_object_list (pix, objects_list1,
					  pix->num_of_max_object);
		sum2 = count_object_list (pix, objects_list2,
					  pix->num_of_max_object);

		/* how many overlapping objects ? */
		sum_of_binary_and_list =
			binary_and_two_object_lists (pix, objects_list1,
						     objects_list2,
						     objects_list3,
						     pix->num_of_max_object);

		/* if presantage below maximum for in a line then we need to
		 * find the end of the line by looking to the end of the down
		 * slop */
		if (inside_line
		    && (((double) sum1 > 1.5 * (double) sum2)
			|| (sum_of_binary_and_list < NOT_IN_A_LINE))
		    && (*line_end - *line_start) > pix->avg_hight_of_objects)
		{
			*line_end = y;
			free (objects_list1);
			free (objects_list2);
			free (objects_list3);
			return 0;
		}

		/* if presantage above minmun for not in a line then we are in
		 * aline */
		if (!inside_line
		    && (((double) sum2 > (double) sum1)
			|| (sum_of_binary_and_list > NOT_IN_A_LINE)))
		{
			*line_start = y + y_step;
			inside_line = TRUE;
			y += pix->avg_hight_of_objects;
		}
	}

	*line_end = column.y2;

	free (objects_list1);
	free (objects_list2);
	free (objects_list3);

	return 1;
}

int
get_next_font_extention (hocr_pixbuf * pix, int line_start, int line_end,
			 int current_pos, int *font_start,
			 int *font_end, int *top, int *bottom)
{
	int x, y;
	int sum;
	unsigned int object;

	for (x = current_pos - MIN_DISTANCE_BETWEEN_FONTS; x > 0; x--)
	{
		/* get presentage coverage for this pixel line */
		sum = 0;
		for (y = line_start - pix->avg_hight_of_objects / 2;
		     y < (line_end + pix->avg_hight_of_objects / 2); y++)
		{
			object = hocr_pixbuf_get_object (pix, x, y);

			if (object)
			{
				*font_start = pix->objects[object].x2;
				*font_end = pix->objects[object].x1;
				*bottom = pix->objects[object].y2;
				*top = pix->objects[object].y1;

				/* check that font is inside picture */
				if (pix->objects[object].weight == 0
				    || (*font_start - *font_end) < 3
				    || (*bottom - *top) < 3
				    || *bottom > (pix->height - 5) || *top < 5
				    || *font_start > (pix->width - 5)
				    || *font_end < 5)
					continue;

				return 0;
			}
		}

	}

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
		column_end += MIN_DISTANCE_BETWEEN_COLUMNS;

		return_value = get_next_column_extention
			(pix, column_start, &column_start, &column_end);
	}

	/* if no column found, return page */
	if (!counter)
	{
		counter = 1;
		columns[0].x1 = 0;
		columns[0].y1 = 0;
		columns[0].x2 = pix->width;
		columns[0].y2 = pix->height;
		columns[0].width = pix->width;
		columns[0].hight = pix->height;
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
		lines[counter].x2 = column.x2;
		lines[counter].x1 = column.x1;
		lines[counter].hight = (lines[counter].y2 - lines[counter].y1);
		lines[counter].width = (lines[counter].x2 - lines[counter].x1);

		counter++;

		/* get some lee way from the end of last line */
		line_end += 2 * pix->avg_hight_of_objects / 3;

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
	int top;
	int bottom;
	int return_value;
	int counter;

	counter = 0;

	/* get all lines in this column */
	return_value = get_next_font_extention (pix,
						line.y1,
						line.y2,
						line.x2,
						&font_start, &font_end, &top,
						&bottom);

	while (return_value == 0 && font_start > line.x1 && counter < max_fonts)
	{
		/* insert this font to fonts array */
		fonts[counter].x1 = font_end;	/* this is right to left sweep */
		fonts[counter].x2 = font_start;
		fonts[counter].y1 = top;
		fonts[counter].y2 = bottom;
		fonts[counter].width = (font_start - font_end);
		fonts[counter].hight = fonts[counter].y2 - fonts[counter].y1;

		counter++;

		return_value = get_next_font_extention (pix,
							line.y1,
							line.y2,
							font_end,
							&font_start, &font_end,
							&top, &bottom);
	}

	*num_of_fonts = counter;

	return 0;
}
