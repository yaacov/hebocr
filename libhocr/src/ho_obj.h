/***************************************************************************
 *            ho_obj.h
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

#ifndef HO_OBJ_H
#define HO_OBJ_H 1

/* hocr_object */
typedef struct
{
  int index;
  int reading_index;
  double weight;
  int x;
  int y;
  int width;
  int height;
} ho_obj;

/* hocr_object_list */
typedef struct
{
  int size;
  int allocated_size;
  ho_obj *objects;
} ho_objlist;

ho_objlist *ho_objlist_new ();

int ho_objlist_free (ho_objlist * object_list);

int
ho_objlist_add (ho_objlist * object_list, double weight,
		 int x, int y, int width, int height);

int ho_objlist_get_index (ho_objlist * object_list, int index);

int
ho_objlist_add_pixel (ho_objlist * object_list, int index,
		       int x, int y);

int
ho_objlist_link (ho_objlist * object_list, int index1,
		  int index2);

int ho_objlist_clean (ho_objlist * object_list, int ** map);

int
ho_objlist_clean_by_reading_index (ho_objlist * object_list, int ** map);

int ho_objlist_print (ho_objlist * object_list);

int
ho_objlist_statistics (ho_objlist * object_list,
			     int min_height, int max_height,
			     int min_width, int max_width,
			     int * counter,
			     double *weight_avg, double *weight_com,
			     double *weight_min, double *weight_max,
			     int * height_avg, int * height_com,
			     int * height_min, int * height_max,
			     int * width_avg, int * width_com,
			     int * width_min, int * width_max);

#endif /* HO_OBJ_H */
