/***************************************************************************
 *            ho_obj.c
 *
 *  Fri Aug 12 20:13:33 2005
 *  Copyright  2005-2007  Yaacov Zamir
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#ifndef TRUE
#define TRUE -1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL ((void*)0)
#endif

#include "ho_obj.h"

ho_objlist *
ho_objlist_new ()
{
  ho_objlist *new_hocr_object_list;

  /* allocate memory for struct */
  new_hocr_object_list = (ho_objlist *) malloc (sizeof (ho_objlist));

  /* check for new memory */
  if (!new_hocr_object_list)
    return NULL;

  /* init struct values */
  new_hocr_object_list->size = 0;
  new_hocr_object_list->allocated_size = 1000;

  /* allocate memory for objects */
  new_hocr_object_list->objects =
    (ho_obj *) malloc (sizeof (ho_obj) *
		       new_hocr_object_list->allocated_size);

  /* if no memeory for objects free the struct */
  if (!new_hocr_object_list->objects)
    {
      free (new_hocr_object_list);
      return NULL;
    }

  return new_hocr_object_list;
}

int
ho_objlist_free (ho_objlist * object_list)
{
  if (!object_list)
    return TRUE;

  /* free the text */
  if (object_list->objects)
    free (object_list->objects);

  /* free the struct */
  free (object_list);

  return FALSE;
}

int
ho_objlist_add (ho_objlist * object_list, double weight,
		int x, int y, int width, int height)
{
  ho_obj *new_object_list_objects;

  if ((object_list->size + 1) > 2000000000)
    return TRUE;

  /* check for allocated space and try to get more memory */
  if ((object_list->size + 1) >= object_list->allocated_size)
    {
      new_object_list_objects = (ho_obj *)
	realloc (object_list->objects, sizeof (ho_obj) *
		 (object_list->allocated_size + 1000));

      /* got new memory */
      if (new_object_list_objects)
	{
	  object_list->objects = new_object_list_objects;
	  object_list->allocated_size += 1000;
	}
      /* did not get new memory */
      else
	{
	  return TRUE;
	}
    }

  /* add the new object */
  (object_list->objects)[object_list->size].index = object_list->size;
  (object_list->objects)[object_list->size].reading_index = object_list->size;
  (object_list->objects)[object_list->size].weight = weight;
  (object_list->objects)[object_list->size].x = x;
  (object_list->objects)[object_list->size].y = y;
  (object_list->objects)[object_list->size].height = height;
  (object_list->objects)[object_list->size].width = width;
  (object_list->size)++;

  return FALSE;
}

int
ho_objlist_get_index (ho_objlist * object_list, int index)
{
  while ((object_list->objects)[index].index != index)
    index = (object_list->objects)[index].index;

  return index;
}

int
ho_objlist_add_pixel (ho_objlist * object_list, int index, int x, int y)
{
  int x1 = (object_list->objects)[index].x;
  int x2 =
    (object_list->objects)[index].x + (object_list->objects)[index].width;
  int y1 = (object_list->objects)[index].y;
  int y2 =
    (object_list->objects)[index].y + (object_list->objects)[index].height;

  (object_list->objects)[index].weight++;
  if (x1 > x)
    x1 = x;
  if (y1 > y)
    y1 = y;
  if (x2 <= x)
    x2 = x + 1;
  if (y2 <= y)
    y2 = y + 1;

  (object_list->objects)[index].x = x1;
  (object_list->objects)[index].y = y1;
  (object_list->objects)[index].height = y2 - y1;
  (object_list->objects)[index].width = x2 - x1;

  return FALSE;
}

int
ho_objlist_link (ho_objlist * object_list, int index1, int index2)
{
  int index;
  int x11, x12, y11, y12;
  int x21, x22, y21, y22;

  /* just checking :-) */
  if (index1 == index2)
    return TRUE;

  /* always use the lower index */
  if (index2 < index1)
    {
      index = index1;
      index1 = index2;
      index2 = index;
    }

  /* set object1 extentions */
  x11 = (object_list->objects)[index1].x;
  x12 =
    (object_list->objects)[index1].x + (object_list->objects)[index1].width;
  y11 = (object_list->objects)[index1].y;
  y12 =
    (object_list->objects)[index1].y + (object_list->objects)[index1].height;

  /* set object2 extentions */
  x21 = (object_list->objects)[index2].x;
  x22 =
    (object_list->objects)[index2].x + (object_list->objects)[index2].width;
  y21 = (object_list->objects)[index2].y;
  y22 =
    (object_list->objects)[index2].y + (object_list->objects)[index2].height;

  /* adjust index2 index */
  (object_list->objects)[index2].index = index1;

  /* adjust index1 weight */
  (object_list->objects)[index1].weight +=
    (object_list->objects)[index2].weight;

  /* adjust index1 object cordiants */
  if (x21 < x11)
    x11 = x21;
  if (y21 < y11)
    y11 = y21;
  if (x22 > x12)
    x12 = x22;
  if (y22 > y12)
    y12 = y22;

  (object_list->objects)[index1].x = x11;
  (object_list->objects)[index1].y = y11;
  (object_list->objects)[index1].height = (y12 - y11);
  (object_list->objects)[index1].width = (x12 - x11);

  return FALSE;
}

int
ho_objlist_clean (ho_objlist * object_list, int **map)
{
  ho_objlist *temp_object_list;
  int new_index, i;

  /* allocate temporary list */
  temp_object_list = ho_objlist_new ();
  if (!temp_object_list)
    return TRUE;

  *map = (int *) calloc (object_list->size, sizeof (int));
  if (!(*map))
    {
      ho_objlist_free (temp_object_list);
      return TRUE;
    }

  /* copy only valid objects to temporary list */
  new_index = 0;
  for (i = 0; i < (object_list->size); i++)
    {
      /* this index does not point to anything */
      (*map)[i] = 0;
      if ((object_list->objects)[i].index == i)
	{
	  ho_objlist_add (temp_object_list,
			  ((object_list->objects)[i]).weight,
			  ((object_list->objects)[i]).x,
			  ((object_list->objects)[i]).y,
			  ((object_list->objects)[i]).width,
			  ((object_list->objects)[i]).height);
	  /* this index point to a new index */
	  (*map)[i] = new_index;
	  /* increas index */
	  new_index++;
	}
    }

  /* copy back objects from temporary list */
  object_list->size = 0;
  for (i = 0; i < (temp_object_list->size); i++)
    {
      ho_objlist_add (object_list,
		      ((temp_object_list->objects)[i]).weight,
		      ((temp_object_list->objects)[i]).x,
		      ((temp_object_list->objects)[i]).y,
		      ((temp_object_list->objects)[i]).width,
		      ((temp_object_list->objects)[i]).height);
    }

  /* unref the temporary list */
  ho_objlist_free (temp_object_list);

  return FALSE;
}

int
ho_objlist_clean_by_reading_index (ho_objlist * object_list, int **map)
{
  ho_objlist *temp_object_list;
  int new_index, i, j;

  /* allocate temporary list */
  temp_object_list = ho_objlist_new ();
  if (!temp_object_list)
    return TRUE;

  *map = (int *) calloc (object_list->size, sizeof (int));
  if (!(*map))
    {
      ho_objlist_free (temp_object_list);
      return TRUE;
    }

  /* map by reading index */
  new_index = 0;
  for (i = 0; i < (object_list->size); i++)
    {
      for (j = 0; j < (object_list->size); j++)
	if ((object_list->objects)[j].reading_index == i)
	  {
	    /* this index is this reading order */
	    (*map)[j] = i;

	    ho_objlist_add (temp_object_list,
			    ((object_list->objects)[j]).weight,
			    ((object_list->objects)[j]).x,
			    ((object_list->objects)[j]).y,
			    ((object_list->objects)[j]).width,
			    ((object_list->objects)[j]).height);
	  }
    }

  /* copy back objects from temporary list */
  object_list->size = 0;
  for (i = 0; i < (temp_object_list->size); i++)
    {
      ho_objlist_add (object_list,
		      ((temp_object_list->objects)[i]).weight,
		      ((temp_object_list->objects)[i]).x,
		      ((temp_object_list->objects)[i]).y,
		      ((temp_object_list->objects)[i]).width,
		      ((temp_object_list->objects)[i]).height);
    }

  /* unref the temporary list */
  ho_objlist_free (temp_object_list);

  return FALSE;
}

int
ho_objlist_print (ho_objlist * object_list)
{
  int i;

  printf ("list has %d objects\n---w\th\tw\n---\n", object_list->size);

  for (i = 1; i < (object_list->size); i++)
    {
      printf ("%0.0f\t%d\t%d\n", ((object_list->objects)[i]).weight,
	      ((object_list->objects)[i]).height,
	      ((object_list->objects)[i]).width);
    }

  return FALSE;
}

int
ho_objlist_statistics (ho_objlist * object_list,
		       int min_height, int max_height,
		       int min_width, int max_width,
		       int *counter,
		       double *weight_avg, double *weight_com,
		       double *weight_min, double *weight_max,
		       int *height_avg, int *height_com,
		       int *height_min, int *height_max,
		       int *width_avg, int *width_com,
		       int *width_min, int *width_max)
{
  /* historam boxes are 0 .. 300 -> 0 .. 60 (5 values into 1 histogram box) 
     for 1D attributes and 0 .. 1500 -> 0 .. 60 (25 into 1) for 2D attributes */
  double weight_histogram[60];
  int height_histogram[60];
  int width_histogram[60];
  double weight;
  double weight_sum;
  int height;
  double height_sum;
  int width;
  double width_sum;
  int i, histogram_index;
  int histogram_index_weight = 0;
  int histogram_index_height = 0;
  int histogram_index_width = 0;

  /* clean histograms */
  for (i = 0; i < 60; i++)
    {
      weight_histogram[i] = 0.0;
      height_histogram[i] = 0;
      width_histogram[i] = 0;
    }

  /* clean initial sum, min and max values */
  weight = ((object_list->objects)[0]).weight;
  width = ((object_list->objects)[0]).width;
  height = ((object_list->objects)[0]).height;

  *weight_min = *weight_max = weight_sum = weight;
  *width_min = *width_max = width_sum = width;
  *height_min = *height_max = height_sum = height;

  (*counter) = 0;

  for (i = 1; i < (object_list->size); i++)
    {
      /* get values */
      weight = ((object_list->objects)[i]).weight;
      width = ((object_list->objects)[i]).width;
      height = ((object_list->objects)[i]).height;

      if (width < min_width || width > max_width || height < min_height
	  || height > max_height)
	continue;

      (*counter)++;

      /* set min max and sum */
      weight_sum += weight;
      if ((*weight_min) > weight)
	*weight_min = weight;
      if ((*weight_max) < weight)
	*weight_max = weight;

      width_sum += width;
      if ((*width_min) > width)
	*width_min = width;
      if ((*width_max) < width)
	*width_max = width;

      height_sum += height;
      if ((*height_min) > height)
	*height_min = height;
      if ((*height_max) < height)
	*height_max = height;

      /* set histograms */
      histogram_index = weight / 25;
      if (histogram_index < 60)
	(weight_histogram[histogram_index])++;
      histogram_index = height / 5;
      if (histogram_index < 60)
	(height_histogram[histogram_index])++;
      histogram_index = width / 5;
      if (histogram_index < 60)
	(width_histogram[histogram_index])++;
    }

  /* set average values */
  if (*counter)
    {
      *weight_avg = weight_sum / (*counter);
      *height_avg = height_sum / (*counter);
      *width_avg = width_sum / (*counter);
    }

  /* set common values */
  for (i = 0; i < 60; i++)
    {
      if (weight_histogram[histogram_index_weight] < weight_histogram[i])
	histogram_index_weight = i;
      if (height_histogram[histogram_index_height] < height_histogram[i])
	histogram_index_height = i;
      if (width_histogram[histogram_index_width] < width_histogram[i])
	histogram_index_width = i;
    }

  /* set common values */
  *weight_com = (double) histogram_index_weight *25.0 + 12;
  *height_com = histogram_index_height * 5 + 2;
  *width_com = histogram_index_width * 5 + 2;

  /* if fond some common height & width values fine-tune the height & width */
  if (height_histogram[histogram_index_height] > 0)
    {
      /* empty the height & width histogram */
      for (i = 0; i < 60; i++)
	{
	  height_histogram[i] = 0;
	  width_histogram[i] = 0;
	}

      for (i = 1; i < (object_list->size); i++)
	{
	  /* get values */
	  height = ((object_list->objects)[i]).height;
	  width = ((object_list->objects)[i]).width;

	  if (width < min_width || width > max_width || height < min_height
	      || height > max_height)
	    continue;

	  /* check for common only height */
	  if (height >= (*height_com) - 2 && height <= (*height_com) + 2)
	    {
	      (height_histogram[height - (*height_com) + 2])++;
	    }

	  /* check for common only width */
	  if (width >= (*width_com) - 2 && width <= (*width_com) + 2)
	    {
	      (width_histogram[width - (*width_com) + 2])++;
	    }
	}

      /* re-set common values */
      histogram_index_height = 0;
      histogram_index_width = 0;
      for (i = 0; i < 5; i++)
	{
	  if (height_histogram[histogram_index_height] < height_histogram[i])
	    histogram_index_height = i;
	  if (width_histogram[histogram_index_width] < width_histogram[i])
	    histogram_index_width = i;
	}

      /* re-set height common values */
      *height_com = histogram_index_height + (*height_com) - 2;
      *width_com = histogram_index_width + (*width_com) - 2;
    }

  return FALSE;
}
