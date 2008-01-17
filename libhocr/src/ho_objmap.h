
/***************************************************************************
 *            ho_objmap.h
 *
 *  Fri Aug 12 20:13:33 2005
 *  Copyright  2005-2007  Yaacov Zamir
 *  <kzamir@walla.co.il>
 ****************************************************************************/

/*  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HO_OBJMAP_H
#define HO_OBJMAP_H 1

#include <ho_bitmap.h>

/* hocr objmap set/get macros */
#define ho_objmap_set(m,x,y,val) (((m)->map)[(x) + (y) * (m)->width] = (val))
#define ho_objmap_get(m,x,y) (((m)->map)[(x) + (y) * (m)->width])

#define ho_objmap_get_size(m) ((m)->obj_list->size)
#define ho_objmap_get_object(m,i) ((m)->obj_list->objects[(i)])

#define ho_objmap_get_width(m) ((m)->width)
#define ho_objmap_get_height(m) ((m)->height)

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

/* hocr_object_map */
typedef struct
{
  int x;
  int y;
  int height;
  int width;
  int *map;
  ho_objlist *obj_list;
} ho_objmap;

ho_objlist *ho_objlist_new ();

int ho_objlist_free (ho_objlist * object_list);

int
ho_objlist_add (ho_objlist * object_list, double weight,
  int x, int y, int width, int height);

int ho_objlist_get_index (ho_objlist * object_list, int index);

int ho_objlist_add_pixel (ho_objlist * object_list, int index, int x, int y);

int ho_objlist_link (ho_objlist * object_list, int index1, int index2);

int ho_objlist_clean (ho_objlist * object_list, int **map);

int ho_objlist_clean_by_reading_index (ho_objlist * object_list, int **map);

int ho_objlist_print (ho_objlist * object_list);

int
ho_objlist_statistics (ho_objlist * object_list,
  int min_height, int max_height,
  int min_width, int max_width,
  int *counter,
  double *weight_avg, double *weight_com,
  double *weight_min, double *weight_max,
  int *height_avg, int *height_com,
  int *height_min, int *height_max,
  int *width_avg, int *width_com, int *width_min, int *width_max);

/**
 new ho_pixbuf 
 @param hight hight of pixbuf in pixels
 @param width width of pixbuf in pixels
 @return newly allocated ho_pixbuf
 */
ho_objmap *ho_objmap_new (const int width, const int height);

/**
 free an ho_objmap
 @param m pointer to an ho_objmap
 @return FALSE
 */
int ho_objmap_free (ho_objmap * m);

/**
 clean the ho_objmap index list 
 @param m pointer to an ho_objmap
 @return FALSE
 */
int ho_objmap_clean (ho_objmap * m);

/**
 sort the ho_objmap by reading index
 @param m pointer to an ho_objmap
 @return FALSE
 */
int ho_objmap_sort_by_reading_index (ho_objmap * m, const unsigned char col,
  const unsigned char dir_ltr);

/**
 new ho_objmap from ho_bitmap
 @param bit_in pointer to an ho_bitmap image
 @return newly allocated ho_objmap
 */
ho_objmap *ho_objmap_new_from_bitmap (const ho_bitmap * bit_in);

/**
 guess font metrics
 @param m pointer to an ho_objmap
 @param min_height only objects with this minimal height are used
 @param max_height only objects with this maximal height are used
 @param min_width only objects with this minimal width are used
 @param max_width only objects with this maximal width are used
 @param height pointer to return the guessed font average  height
 @param width pointer to return the guessed font average  width
 @return FALSE
 */
int ho_objmap_font_metrix (const ho_objmap * m, const int min_height,
  const int max_height,
  const int min_width,
  const int max_width, int *height, int *width, unsigned char *nikud);

/**
 copy objects from objmap to bitmap by size
 @param m pointer to an ho_objmap
 @param min_height only objects with this minimal height are copied
 @param max_height only objects with this maximal height are copied
 @param min_width only objects with this minimal width are copied
 @param max_width only objects with this maximal width are copied
 @return a newly allocated bitmap
 */
ho_bitmap *ho_objmap_to_bitmap_by_size (const ho_objmap * m,
  int min_height, int max_height, int min_width, int max_width);

/**
 copy objects from objmap to bitmap by index
 @param m pointer to an ho_objmap
 @param index the index of the object to copy
 @return a newly allocated bitmap size of objmap
 */
ho_bitmap *ho_objmap_to_bitmap_by_index (const ho_objmap * m, const int index);

/**
 copy objects from objmap to bitmap by index of the object
 @param m pointer to an ho_objmap
 @param index the index of the object to copy
 @param frame size of white frame in the new bitmap
 @return a newly allocated bitmap size of object + frame
 */
ho_bitmap *ho_objmap_to_bitmap_by_index_window (const ho_objmap * m,
  const int index, const int frame);

/**
 guess reading_index
 @param m pointer to an ho_objmap
 @return FALSE
 */
int ho_objmap_update_reading_index (ho_objmap * m,
  const unsigned char n_columns, const unsigned char dir_ltr);

#endif /* HO_OBJMAP_H */
