/***************************************************************************
 *            ho_bitmap.c
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

#include "ho_common.h"
#include "ho_bitmap.h"
#include "ho_objmap.h"
#include "ho_bitmap_filter.h"

ho_bitmap *
ho_bitmap_filter_dilation (const ho_bitmap * m)
{
  return ho_bitmap_dilation_n (m, 1);
}

ho_bitmap *
ho_bitmap_filter_erosion (const ho_bitmap * m)
{
  return ho_bitmap_erosion_n (m, 1);
}

ho_bitmap *
ho_bitmap_filter_opening (const ho_bitmap * m)
{
  ho_bitmap *m_temp;
  ho_bitmap *m_out;

  m_temp = ho_bitmap_filter_erosion (m);
  m_out = ho_bitmap_filter_dilation (m_temp);
  ho_bitmap_free (m_temp);

  return m_out;
}

ho_bitmap *
ho_bitmap_filter_closing (const ho_bitmap * m)
{
  ho_bitmap *m_temp;
  ho_bitmap *m_out;

  m_temp = ho_bitmap_filter_dilation (m);
  m_out = ho_bitmap_filter_erosion (m_temp);
  ho_bitmap_free (m_temp);

  return m_out;
}

ho_bitmap *
hocr_bitmap_filter_hlink (ho_bitmap * m, ho_uint size)
{
  ho_bitmap *m_out;
  ho_uint x, y;
  ho_uint k;
  int l, last;

  /* allocate memory */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return HO_NULL;

  for (y = 0; y < m->height; y++)
    {
      last = -size;
      for (x = 0; x < m->width; x++)
	{
	  if (ho_bitmap_get (m, x, y))	/* black pixel */
	    {
	      ho_bitmap_set (m_out, x, y);
	      l = (int) x - last;
	      if ((l > 1) && (l < size))
		{
		  for (k = last; k < x; k++)
		    ho_bitmap_set (m_out, k, y);
		}
	      last = x;
	    }
	}
    }

  return m_out;
}

ho_bitmap *
hocr_bitmap_filter_vlink (ho_bitmap * m, ho_uint size)
{
  ho_bitmap *m_out;
  ho_uint x, y;
  ho_uint k;
  int l, last;

  /* allocate memory */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return HO_NULL;

  for (x = 0; x < m->width; x++)
    {
      last = -size;
      for (y = 0; y < m->height; y++)
	{
	  if (ho_bitmap_get (m, x, y))	/* black pixel */
	    {
	      ho_bitmap_set (m_out, x, y);
	      l = (int) y - last;
	      if ((l > 1) && (l < size))
		{
		  for (k = last; k < y; k++)
		    ho_bitmap_set (m_out, x, k);
		}
	      last = y;
	    }
	}
    }

  return m_out;
}

ho_bitmap *
ho_bitmap_filter_by_size (const ho_bitmap * m,
			  ho_uint min_height, ho_uint max_height,
			  ho_uint min_width, ho_uint max_width)
{
  ho_objmap *m_obj;
  ho_bitmap *m_out;

  /* create a new objmap */
  m_obj = ho_objmap_new_from_bitmap (m);

  if (!m_obj)
    return HO_NULL;

  m_out = ho_objmap_to_bitmap_by_size (m_obj,
				       min_height, max_height,
				       min_width, max_width);

  /* free objmap */
  ho_objmap_free (m_obj);

  return m_out;
}

ho_bitmap *
ho_bitmap_filter_boxes (const ho_bitmap * m)
{
  ho_objmap *m_obj;
  ho_bitmap *m_out;
  ho_usint index;
  ho_uint x, y, width, height;

  /* allocate memory */
  m_obj = ho_objmap_new_from_bitmap (m);
  if (!m_obj)
    return HO_NULL;
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    {
      ho_objmap_free (m_obj);
      return HO_NULL;
    }

  /* loop over all the objects and box them */
  for (index = 0; index < m_obj->obj_list->size; index++)
    {
      x = (((m_obj->obj_list)->objects)[index]).x;
      y = (((m_obj->obj_list)->objects)[index]).y;
      width = (((m_obj->obj_list)->objects)[index]).width;
      height = (((m_obj->obj_list)->objects)[index]).height;

      ho_bitmap_draw_box (m_out, x, y, width, height);
    }

  return m_out;
}

ho_bitmap *
ho_bitmap_filter_fill (const ho_bitmap * m)
{
  ho_objmap *m_obj;
  ho_bitmap *m_out;
  ho_bitmap *m_temp1;
  ho_bitmap *m_temp2;
  ho_usint index;
  ho_uint width, height;

  /* allocate memory */
  m_obj = ho_objmap_new_from_bitmap (m);
  if (!m_obj)
    return HO_NULL;

  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    {
      ho_objmap_free (m_obj);
      return HO_NULL;
    }

  /* loop over all the objects and box them */
  for (index = 0; index < m_obj->obj_list->size; index++)
    {
      /* get a dimention factor */
      width = (((m_obj->obj_list)->objects)[index]).width;
      height = (((m_obj->obj_list)->objects)[index]).height;

      /* copy only the current object to a new bitmap */
      m_temp1 = ho_objmap_to_bitmap_by_index (m_obj, index);

      /* fill the current object */
      m_temp2 = hocr_bitmap_filter_hlink (m_temp1, width / 4);
      ho_bitmap_free (m_temp1);
      m_temp1 = hocr_bitmap_filter_vlink (m_temp2, height / 4);
      ho_bitmap_free (m_temp2);

      /* add to matrix out */
      ho_bitmap_or (m_out, m_temp1);
      ho_bitmap_free (m_temp1);
    }

  return m_out;
}

ho_bitmap *
hocr_bitmap_filter_hlink_objecs (ho_bitmap * m, ho_uint size,
				 ho_uint max_height)
{
  ho_bitmap *m_out;
  ho_bitmap *m_temp;

  /* get only the thin objects */
  m_temp = ho_bitmap_filter_by_size (m, 5, max_height, 5, m->width / 2);

  /* hlink the thin objects */
  m_out = hocr_bitmap_filter_hlink (m_temp, size);

  /* add the rest of the large objects */
  ho_bitmap_or (m_out, m);

  ho_bitmap_free (m_temp);

  return m_out;
}

ho_bitmap *
ho_bitmap_filter_remove_dots (const ho_bitmap * m,
			      const ho_uchar erosion_n,
			      const ho_uchar dilation_n)
{
  ho_uint x, y;
  ho_uchar sum;
  ho_bitmap *m_temp;
  ho_bitmap *m_out;
  ho_objmap *m_obj;
  ho_usint index;
  ho_uint width, height;

  /* allocate memory */
  m_out = ho_bitmap_new (m->width, m->height);
  if (!m_out)
    return HO_NULL;

  /* connect all the small dots */
  m_temp = ho_bitmap_filter_dilation (m);

  /* check the size of objects */
  m_obj = ho_objmap_new_from_bitmap (m_temp);
  ho_bitmap_free (m_temp);

  for (x = 1; x < (m->width - 1); x++)
    for (y = 1; y < (m->height - 1); y++)
      {

	/* check the size of this pixel's object */
	index = ho_objmap_get (m_obj, x, y);
	if (index)
	  {
	    width = (((m_obj->obj_list)->objects)[index - 1]).width;
	    height = (((m_obj->obj_list)->objects)[index - 1]).height;
	  }
	else
	  {
	    width = 0;
	    height = 0;
	  }

	/* in a big object do erosion */
	if (width > m->width / 4 || height > m->height / 4)
	  {
	    if (ho_bitmap_get (m, x, y))	/* black pixel */
	      {
		sum = ho_bitmap_get (m, x - 1, y - 1) +
		  ho_bitmap_get (m, x - 1, y) +
		  ho_bitmap_get (m, x - 1, y + 1) +
		  ho_bitmap_get (m, x, y - 1) +
		  ho_bitmap_get (m, x, y + 1) +
		  ho_bitmap_get (m, x + 1, y - 1) +
		  ho_bitmap_get (m, x + 1, y) +
		  ho_bitmap_get (m, x + 1, y + 1);
		/* n number of white pixels or more */
		if ((8 - sum) < erosion_n)
		  ho_bitmap_set (m_out, x, y);
	      }
	  }
	else			/* if in a small object do dilation */
	  {
	    if (!ho_bitmap_get (m, x, y))	/* white pixel */
	      {
		sum = ho_bitmap_get (m, x - 1, y - 1) +
		  ho_bitmap_get (m, x - 1, y) +
		  ho_bitmap_get (m, x - 1, y + 1) +
		  ho_bitmap_get (m, x, y - 1) +
		  ho_bitmap_get (m, x, y + 1) +
		  ho_bitmap_get (m, x + 1, y - 1) +
		  ho_bitmap_get (m, x + 1, y) +
		  ho_bitmap_get (m, x + 1, y + 1);
		/* n number of black neighbors or more */
		if (sum >= dilation_n)
		  ho_bitmap_set (m_out, x, y);
	      }
	    else
	      ho_bitmap_set (m_out, x, y);
	  }
      }

  ho_objmap_free (m_obj);

  return m_out;
}

int
ho_bitmap_filter_font_metrix (const ho_bitmap * m, const ho_uint min_height,
			      const ho_uint max_height,
			      const ho_uint min_width,
			      const ho_uint max_width, ho_usint * height,
			      ho_usint * width, ho_uchar * nikud)
{
  int return_val;
  ho_objmap *m_obj = NULL;

  /* create an object map from b/w image */
  m_obj = ho_objmap_new_from_bitmap (m);

  /* get fonts size */
  return_val =
    ho_objmap_font_metrix (m_obj, min_height, max_height, min_width,
			   max_width, height, width, nikud);

  ho_objmap_free (m_obj);

  return return_val;
}

int
ho_bitmap_filter_line_metrix (const ho_bitmap * m,
			      const ho_uint font_height,
			      const ho_uint font_width,
			      const ho_uchar nikud,
			      ho_usint * interline_height)
{
  int return_val;
  ho_objmap *m_obj = NULL;

  ho_bitmap *m_clean;
  ho_bitmap *m_temp1;
  ho_bitmap *m_temp2;
  ho_bitmap *m_out;
  ho_uint x, y;
  ho_uchar nikud_ret;
  ho_usint width;

  /* if nikud we need to be more careful */
  if (nikud)
    m_clean = ho_bitmap_filter_by_size (m,
					font_height / 2, font_height * 4,
					font_width / 3, font_width * 5);
  else
    m_clean = ho_bitmap_filter_by_size (m,
					font_height / 4, font_height * 4,
					font_width / 4, font_width * 5);

  /* get common height betwine lines */
  m_temp1 = hocr_bitmap_filter_hlink (m_clean, font_width * 4);
  if (!m_temp1)
    return HO_TRUE;
  ho_bitmap_free (m_clean);

  m_temp2 = hocr_bitmap_filter_vlink (m_temp1, font_height * 4);
  if (!m_temp2)
    return HO_TRUE;

  ho_bitmap_andnot (m_temp2, m_temp1);
  ho_bitmap_free (m_temp1);

  /* cut the betwine lines objects to word size objects */
  for (x = font_width * 3; x < m->width; x += font_width * 3)
    for (y = 0; y < m->height; y++)
      ho_bitmap_unset (m_temp2, x, y);

  /* create an object map from b/w image */
  m_obj = ho_objmap_new_from_bitmap (m_temp2);
  ho_bitmap_free (m_temp2);

  /* get interline size */
  return_val =
    ho_objmap_font_metrix (m_obj, font_height / 2, font_height * 4,
			   font_width * 2, font_width * 4, interline_height,
			   &width, &nikud_ret);

  ho_objmap_free (m_obj);

  /* sanity chack */
  if (*interline_height < font_height || *interline_height > font_height * 4)
    *interline_height = font_height * 1.5;

  return return_val;
}

ho_bitmap *
ho_bitmap_filter_paragraphs (const ho_bitmap * m,
			     const ho_uchar font_height,
			     const ho_uchar font_width,
			     const ho_uchar nikud,
			     ho_usint interline_height, const ho_uchar box)
{
  ho_bitmap *m_clean;
  ho_bitmap *m_temp1;
  ho_bitmap *m_out;
  ho_uint x, y;

  /* if nikud we need to be more careful */
  if (nikud)
    m_clean = ho_bitmap_filter_by_size (m,
					font_height / 2, font_height * 4,
					font_width / 3, font_width * 5);
  else
    m_clean = ho_bitmap_filter_by_size (m,
					font_height / 4, font_height * 4,
					font_width / 4, font_width * 5);

  /* link paragraphs */
  m_temp1 = hocr_bitmap_filter_vlink (m_clean, interline_height * 1.2);
  if (!m_temp1)
    return HO_NULL;

  m_out = hocr_bitmap_filter_hlink (m_temp1, font_width * 2);
  if (!m_out)
    return HO_NULL;

  ho_bitmap_free (m_temp1);

  if (box)
    m_temp1 = ho_bitmap_filter_boxes (m_out);
  else
    m_temp1 = ho_bitmap_filter_fill (m_out);

  ho_bitmap_free (m_out);
  ho_bitmap_free (m_clean);

  /* try and link one/two line broken paragraphs */
  m_out = hocr_bitmap_filter_hlink_objecs (m_temp1, font_width * 6,
					   font_height * 2 +
					   interline_height * 2);

  ho_bitmap_free (m_temp1);

  /* check all paragraphs for artefacts, too small or large */
  m_temp1 =
    ho_bitmap_filter_by_size (m_out, 2 * font_height / 3, m->height,
			      font_width * 3, m->width);

  ho_bitmap_free (m_out);

  return m_temp1;
}

ho_bitmap *
ho_bitmap_filter_lines (const ho_bitmap * m,
			     const ho_uchar font_height,
			     const ho_uchar font_width,
			     const ho_uchar nikud,
			     const ho_usint interline_height)
{
  ho_bitmap *m_clean;
  ho_bitmap *m_temp1;
  ho_bitmap *m_out;
  ho_uint x, y;

  //m_clean = ho_bitmap_filter_by_size (m,
	//				4 * font_height / 5, font_height * 1.1,
	//				font_width / 3, font_width * 5);
  m_clean = ho_bitmap_filter_by_size (m,
					4 * font_height / 5, font_height * 1.1,
					font_width / 3, font_width * 5);
  
  /* link lines */
  m_out = hocr_bitmap_filter_hlink (m_clean, font_width * 18);
  if (!m_out)
    return HO_NULL;

 //m_temp1 = ho_bitmap_filter_fill (m_out);

 // ho_bitmap_free (m_out);
  ho_bitmap_free (m_clean);

  /* check all paragraphs for artefacts, too small or large */
 //m_out =
  //  ho_bitmap_filter_by_size (m_temp1, font_height / 2, font_height * 4,
	//		      font_width , m->width);

 // ho_bitmap_free (m_temp1);

  return m_out;
}
