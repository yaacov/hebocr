/***************************************************************************
 *            ho_obj.h
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

#ifndef HO_OBJ_H
#define HO_OBJ_H 1

#include <ho_common.h>

/* hocr_object */
typedef struct
{
  ho_usint index;
  double weight;
  ho_uint x;
  ho_uint y;
  ho_uint width;
  ho_uint height;
} ho_obj;

/* hocr_object_list */
typedef struct
{
  ho_usint size;
  ho_usint allocated_size;
  ho_obj *objects;
} ho_objlist;

ho_objlist *ho_objlist_new ();

int ho_objlist_free (ho_objlist * object_list);

int
ho_objlist_add (ho_objlist * object_list, double weight,
		 ho_uint x, ho_uint y, ho_uint width, ho_uint height);

ho_usint ho_objlist_get_index (ho_objlist * object_list, ho_usint index);

int
ho_objlist_add_pixel (ho_objlist * object_list, ho_usint index,
		       ho_uint x, ho_uint y);

int
ho_objlist_link (ho_objlist * object_list, ho_usint index1,
		  ho_usint index2);

int ho_objlist_clean (ho_objlist * object_list, ho_usint ** map);

int ho_objlist_print (ho_objlist * object_list);

int
ho_objlist_statistics (ho_objlist * object_list,
			     ho_uint min_height, ho_uint max_height,
			     ho_uint min_width, ho_uint max_width,
			     ho_usint * counter,
			     double *weight_avg, double *weight_com,
			     double *weight_min, double *weight_max,
			     ho_uint * height_avg, ho_uint * height_com,
			     ho_uint * height_min, ho_uint * height_max,
			     ho_uint * width_avg, ho_uint * width_com,
			     ho_uint * width_min, ho_uint * width_max);

#endif /* HO_OBJ_H */
