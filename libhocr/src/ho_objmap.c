/***************************************************************************
 *            ho_objmap.c
 *
 *  Fri Aug 12 20:13:33 2005
 *  Copyright  2005-2007  Yaacov Zamir, Free Software Foundation
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

#include "ho_common.h"
#include "ho_obj.h"
#include "ho_objmap.h"

ho_objmap *
ho_objmap_new (const ho_uint width, const ho_uint height)
{
  ho_objmap *m_new = HO_NULL;

  /*
   * allocate memory for pixbuf 
   */
  m_new = (ho_objmap *) malloc (sizeof (ho_objmap));
  if (!m_new)
    {
      return HO_NULL;
    }

  /* read header */
  m_new->width = width;
  m_new->height = height;

  /* allocate memory for data (and set to zero) */
  m_new->map =
    (ho_usint *) calloc (m_new->height * m_new->width, sizeof (ho_usint));
  if (!(m_new->map))
    {
      free (m_new);
      return HO_NULL;
    }

  /* allocate initial object list */
  m_new->obj_list = ho_objlist_new ();
  if (!(m_new->obj_list))
    {
      free (m_new->map);
      free (m_new);
      return HO_NULL;
    }

  return m_new;
}

int
ho_objmap_free (ho_objmap * m)
{
  if (!m)
    return HO_TRUE;

  ho_objlist_free (m->obj_list);
  if (m->map)
    free (m->map);
  free (m);

  return HO_FALSE;
}

int
ho_objmap_clean (ho_objmap * m)
{
  ho_uint x, y, k;
  ho_usint index;
  ho_usint *map = NULL;

  /* relax the object matrix */
  for (x = 0; x < m->width; x++)
    for (y = 0; y < m->height; y++)
      {

	k = ho_objmap_get (m, x, y);
	if (k)
	  {
	    index = ho_objlist_get_index ((m->obj_list), k - 1);
	    ho_objmap_set (m, x, y, index + 1);
	  }
      }

  /* clean the object list */
  ho_objlist_clean ((m->obj_list), &map);

  /* re relax the object matrix */
  for (x = 0; x < m->width; x++)
    for (y = 0; y < m->height; y++)
      {
	k = ho_objmap_get (m, x, y);
	if (k)
	  {
	    index = map[k - 1];
	    ho_objmap_set (m, x, y, index + 1);
	  }
      }

  /* free all temporary memory */
  if (map)
    free (map);

  return HO_FALSE;
}

ho_objmap *
ho_objmap_new_from_bitmap (const ho_bitmap * bit_in)
{
  ho_objmap *m_new = HO_NULL;

  ho_uint x, y, k;
  double sum;
  double neigbors[4];
  ho_usint min;
  ho_usint index, indexes[4];
  ho_usint *map = NULL;

  /* allocate memory */
  m_new = ho_objmap_new (bit_in->width, bit_in->height);

  /* link all concted black pixels */
  for (x = 1; x < bit_in->width; x++)
    for (y = 1; y < (bit_in->height - 1); y++)
      {
	/* is pixel black ? */
	if (ho_bitmap_get (bit_in, x, y))
	  {
	    /* get neigbors */
	    neigbors[0] = ho_objmap_get (m_new, x - 1, y - 1);
	    neigbors[1] = ho_objmap_get (m_new, x - 1, y - 0);
	    neigbors[2] = ho_objmap_get (m_new, x - 1, y + 1);
	    neigbors[3] = ho_objmap_get (m_new, x - 0, y - 1);

	    sum = neigbors[0] + neigbors[1] + neigbors[2] + neigbors[3];

	    /* if no neigbor is black */
	    if (sum == 0)
	      {
		if (ho_objlist_add ((m_new->obj_list), 1, x, y, 1, 1))
		  {
		    /* can't add to objects :-( */
		    ho_objmap_free (m_new);
		    return HO_NULL;
		  }

		/* in the matrix we insert index+1 */
		ho_objmap_set (m_new, x, y, (m_new->obj_list)->size);
	      }
	    /* at least one neigbor is black */
	    else
	      {
		/* find the index of the minimal frind */
		min = 0;
		for (k = 0; k < 4; k++)
		  {
		    if (neigbors[k])
		      {
			indexes[k] =
			  ho_objlist_get_index ((m_new->obj_list),
						neigbors[k] - 1);
			if (!min || (indexes[k] < (min - 1)))
			  /* indexes goes 0.. and min goes 1.. */
			  min = (indexes[k] + 1);
		      }
		    else
		      indexes[k] = 0;
		  }
		/* min 1.. -> 0.. */
		min--;

		/* set the current pixel in the objects matrix to that index */
		ho_objmap_set (m_new, x, y, min + 1);
		ho_objlist_add_pixel ((m_new->obj_list), min, x, y);

		/* link all object to that index */
		for (k = 0; k < 4; k++)
		  {
		    if (indexes[k])
		      ho_objlist_link ((m_new->obj_list), min, indexes[k]);
		  }
	      }
	  }
      }

  /* relax and clean the objmap */
  ho_objmap_clean (m_new);

  return m_new;
}

int
ho_objmap_font_metrix (const ho_objmap * m, const ho_uint min_height,
		       const ho_uint max_height,
		       const ho_uint min_width,
		       const ho_uint max_width, ho_usint * height,
		       ho_usint * width, ho_uchar * nikud)
{
  ho_uint i, j;
  ho_usint counter;
  double weight_avg;
  double weight_com;
  double weight_min;
  double weight_max;
  ho_uint height_avg;
  ho_uint height_com;
  ho_uint height_min;
  ho_uint height_max;
  ho_uint width_avg;
  ho_uint width_com;
  ho_uint width_min;
  ho_uint width_max;

  *nikud = HO_FALSE;

  /* get stats */
  ho_objlist_statistics (m->obj_list,
			 min_height, max_height,
			 min_width, max_width,
			 &counter,
			 &weight_avg, &weight_com,
			 &weight_min, &weight_max,
			 &height_avg, &height_com,
			 &height_min, &height_max,
			 &width_avg, &width_com, &width_min, &width_max);

  /* does it look like nikud ? */
  if (height_avg < (2 * height_com / 3))
    {
      *nikud = HO_TRUE;
    }

  /* did we find the nikud ? */
  if (height_com < (2 * height_avg / 3))
    {
      *nikud = HO_TRUE;

      /* re-get stats, now limit minial size */
      ho_objlist_statistics (m->obj_list, 3 * height_avg / 2, 300,
			     3 * width_avg / 2, 300, &counter,
			     &weight_avg, &weight_com, &weight_min,
			     &weight_max, &height_avg, &height_com,
			     &height_min, &height_max, &width_avg,
			     &width_com, &width_min, &width_max);
    }

  /* is width logical ? */
  if (height_com > 2 * width_com)
    width_com = 2 * height_com / 3;

  *height = height_com;
  *width = width_com;

  return HO_FALSE;
}

ho_bitmap *
ho_objmap_to_bitmap_by_size (const ho_objmap * m,
			     ho_uint min_height, ho_uint max_height,
			     ho_uint min_width, ho_uint max_width)
{
  ho_uint x, y;
  ho_usint index;
  ho_uint height;
  ho_uint width;
  ho_bitmap *m_out;

  /* allocate memory */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return HO_NULL;

  for (x = 0; x < m->width; x++)
    for (y = 0; y < m->height; y++)
      {
	index = ho_objmap_get (m, x, y);

	if (index)
	  {
	    /* on objmap objects are 1.. and on objlist 0.. */
	    index--;

	    width = (((m->obj_list)->objects)[index]).width;
	    height = (((m->obj_list)->objects)[index]).height;

	    if (width < min_width || width > max_width || height < min_height
		|| height > max_height)
	      continue;

	    ho_bitmap_set (m_out, x, y);
	  }
      }

  return m_out;
}

ho_bitmap *
ho_objmap_to_bitmap_by_index (const ho_objmap * m, const ho_usint index)
{
  ho_uint x, y;
  ho_bitmap *m_out;
  ho_usint current_index;
  ho_uint height;
  ho_uint width;

  /* is this object realy an object ? */
  if (index >= (m->obj_list)->size)
    return HO_NULL;

  width = (((m->obj_list)->objects)[index]).width;
  height = (((m->obj_list)->objects)[index]).height;

  /* is this object realy a big object ? */
  if (width < 2 || height < 2)
    return HO_NULL;

  /* allocate memory */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return HO_NULL;

  for (x = 0; x < m->width; x++)
    for (y = 0; y < m->height; y++)
      {
	current_index = ho_objmap_get (m, x, y);

	if ((current_index - 1) == index)
	  {
	    ho_bitmap_set (m_out, x, y);
	  }
      }

  return m_out;
}

ho_bitmap *
ho_objmap_to_bitmap_by_index_window (const ho_objmap * m,
				     const ho_usint index,
				     const ho_uint frame)
{
  ho_uint x, y;
  ho_uint x1, y1;
  ho_bitmap *m_out;
  ho_usint current_index;
  ho_uint height;
  ho_uint width;

  /* is this object realy an object ? */
  if (index >= (m->obj_list)->size)
    return HO_NULL;

  width = (((m->obj_list)->objects)[index]).width;
  height = (((m->obj_list)->objects)[index]).height;

  /* is this object realy a big object ? */
  if (width < 2 || height < 2)
    return HO_NULL;

  /* allocate memory */
  m_out = ho_bitmap_new (width + 2 * frame, height + 2 * frame);
  if (!m_out)
    return HO_NULL;

  x1 = (((m->obj_list)->objects)[index]).x;
  y1 = (((m->obj_list)->objects)[index]).y;

  for (x = 0; x < width; x++)
    for (y = 0; y < height; y++)
      {
	current_index = ho_objmap_get (m, x + x1, y + y1);

	if ((current_index - 1) == index)
	  {
	    ho_bitmap_set (m_out, x + frame, y + frame);
	  }
      }

  return m_out;
}
