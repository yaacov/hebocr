/***************************************************************************
 *            ho_bitmap.h
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

#ifndef HO_BITMAP_H
#define HO_BITMAP_H 1

/* hocr bitmap set/get macros */
#define ho_bitmap_get(m,x,y) (((((m)->data[(x) / 8 + (y) * (m)->rowstride]) & (0x80 >> ((x) % 8))) > 0)?1:0)
#define ho_bitmap_set(m,x,y) (((m)->data[(x) / 8 + (y) * (m)->rowstride]) |= (0x80 >> ((x) % 8)))
#define ho_bitmap_unset(m,x,y) (((m)->data[(x) / 8 + (y) * (m)->rowstride]) &= ~(0x80 >> ((x) % 8)))

#define ho_bitmap_get_x(m) ((m)->x)
#define ho_bitmap_get_y(m) ((m)->y)
#define ho_bitmap_set_x(m,new_x) ((m)->x=(new_x))
#define ho_bitmap_set_y(m,new_y) ((m)->y=(new_y))
#define ho_bitmap_get_width(m) ((m)->width)
#define ho_bitmap_get_height(m) ((m)->height)

typedef struct
{
  unsigned char type;
  int font_height;
  int font_width;

  int font_spacing;
  int line_spacing;
  int avg_line_fill;
  int com_line_fill;

  unsigned char nikud;

  int x;
  int y;
  int height;
  int width;
  int rowstride;
  unsigned char *data;
} ho_bitmap;

/**
 new ho_bitamp
 @param hight hight of pixbuf in pixels
 @param width width of pixbuf in pixels
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_new (const int width, const int height);

/**
 free an ho_bitmap
 @param m pointer to an ho_bitmap
 @return FALSE
 */
int ho_bitmap_free (ho_bitmap * m);

/**
 copy a bitmap
 @param m the bitmap to copy
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_clone (const ho_bitmap * m);

/**
 copy a window from a bitmap
 @param m the bitmap to copy
 @param x x-start of window
 @param y y-start of window
 @param width width of window
 @param height height of window
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_clone_window (const ho_bitmap * m, const int x,
				   const int y, const int width,
				   const int height);

/**
 do bitwise and of two bitmaps
 @param m_left the left hand bitmap
 @param m_right the right hand bitmap
 @return FALSE
 */
int ho_bitmap_and (ho_bitmap * m_left, const ho_bitmap * m_right);

/**
 do bitwise or of two bitmaps
 @param m_left the left hand bitmap
 @param m_right the right hand bitmap
 @return FALSE
 */
int ho_bitmap_or (ho_bitmap * m_left, const ho_bitmap * m_right);

/**
 do bitwise xor of two bitmaps
 @param m_left the left hand bitmap
 @param m_right the right hand bitmap
 @return FALSE
 */
int ho_bitmap_xor (ho_bitmap * m_left, const ho_bitmap * m_right);

/**
 do bitwise and not of two bitmaps
 @param m_left the left hand bitmap
 @param m_right the right hand bitmap
 @return FALSE
 */
int ho_bitmap_andnot (ho_bitmap * m_left, const ho_bitmap * m_right);

/**
 do bitwise copy of two bitmaps
 @param m_left the left hand bitmap
 @param m_right the right hand bitmap
 @return FALSE
 */
int ho_bitmap_copy (ho_bitmap * m_left, const ho_bitmap * m_right);

/**
 do bitwise not of a bitmap
 @param m the left hand bitmap
 @return a newly allocated bitmap 
 */
ho_bitmap *ho_bitmap_not (const ho_bitmap * m);

/**
 dilation of a a bitmap with 3x3 box
 @param m the bitmap to dilate 
 @param n dilation constant
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_dilation_n (const ho_bitmap * m, const unsigned char n);

/**
 erosion of a a bitmap with 3x3 box
 @param m the bitmap to erode 
 @param n erosion constant
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_erosion_n (const ho_bitmap * m, const unsigned char n);

/**
 take only top height black pixels of bitmap
 @param m the bitmap to erode 
 @param height the height to take
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_set_height (const ho_bitmap * m, const int height,
				 const int top, const int bottom);

/**
 take only bottom height black pixels of bitmap
 @param m the bitmap to erode 
 @param height the height to take
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_set_height_from_bottom (const ho_bitmap * m,
					     const int height, const int top,
					     const int bottom);

/**
 dilation of a a bitmap with 3x3 box
 @param m the bitmap to dilate 
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_dilation (const ho_bitmap * m);

/**
 erosion of a a bitmap with 3x3 box
 @param m the bitmap to erode
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_erosion (const ho_bitmap * m);

/**
 opening of a a bitmap with 3x3 box
 @param m the bitmap to open 
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_opening (const ho_bitmap * m);

/**
 closing of a a bitmap with 3x3 box
 @param m the bitmap to close 
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_closing (const ho_bitmap * m);

/**
 horizontaly link black dots in a bitmap
 @param m the bitmap to horizontaly link
 @patam size maximum distance
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_hlink (const ho_bitmap * m, const int size);

/**
 horizontaly erode black dots in a bitmap
 @param m the bitmap to horizontaly link
 @patam size maximum distance
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_herode (const ho_bitmap * m, const int size);

/**
 verticaly link black dots in a bitmap
 @param m the bitmap to verticaly link
 @patam size maximum distance
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_vlink (const ho_bitmap * m, const int size);

/**
 copy edges in bitmap
 @param m pointer to an ho_bitmap
 @param n width of egde
 @return a newly allocated bitmap
 */
ho_bitmap *ho_bitmap_edge (const ho_bitmap * m, const int n);

/**
 return the ratio fill/volume value of a rectangle in the bitmap
 @param m pointer to an ho_bitmap
 @param x x of rectangel
 @param y y of rectangel
 @param width width of rectangel
 @param height height of rectangel
 @return the ratio value
 */
double
ho_bitmap_get_fill (const ho_bitmap * m, const int x, const int y,
		    const int width, const int height);

/**
 writes ho_bitmap to pnm file
 @param m ho_bitmap 1 bpp
 @param filenme save as file name 
 @return FALSE
 */
int ho_bitmap_pnm_save (const ho_bitmap * m, const char *filename);

/**
 horizontaly link short objects in a bitmap
 @param m the bitmap to horizontaly link
 @patam size maximum distance
 @patam max_height maximum hight of objects to link
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_bitmap_filter_hlink (ho_bitmap * m, int size, int max_height);

/**
 copy objects from bitmap to bitmap by size
 @param m pointer to an ho_bitmap
 @param min_height only objects with this minimal height are copied
 @param max_height only objects with this maximal height are copied
 @param min_width only objects with this minimal width are copied
 @param max_width only objects with this maximal width are copied
 @return a newly allocated bitmap
 */
ho_bitmap *ho_bitmap_filter_by_size (const ho_bitmap * m,
				     int min_height, int max_height,
				     int min_width, int max_width);

/**
 copy boxed objects from bitmap
 @param m pointer to an ho_bitmap
 @return a newly allocated bitmap
 */
ho_bitmap *ho_bitmap_filter_boxes (const ho_bitmap * m, const int leeway_down,
				   const int leeway_up);

/**
 copy filled objects from bitmap
 @param m pointer to an ho_bitmap
 @param leeway add this leeway downwords to the box
 @return a newly allocated bitmap
 */
ho_bitmap *ho_bitmap_filter_fill (const ho_bitmap * m);

/**
 take height top pixels from objects in bitmap
 @param m pointer to an ho_bitmap
 @param hight of new objects
 @return a newly allocated bitmap
 */
ho_bitmap *ho_bitmap_filter_set_height (const ho_bitmap * m, const int height,
					const int top, const int bottom);

/**
 take height bottom pixels from objects in bitmap
 @param m pointer to an ho_bitmap
 @param hight of new objects
 @return a newly allocated bitmap
 */
ho_bitmap *ho_bitmap_filter_set_height_from_bottom (const ho_bitmap * m,
						    const int height,
						    const int top,
						    const int bottom);

/**
 take extend objects lateraly
 @param m pointer to an ho_bitmap
 @param ext_width width of lateral extention
 @return a newly allocated bitmap
 */
ho_bitmap *ho_bitmap_filter_obj_extend_lateraly (const ho_bitmap * m,
						 const int ext_width);

/**
 remove halftone dots from bitmap
 @param m pointer to an ho_bitmap
 @param erosion_n the erosion operator factor
 @param dilation_n the dilation operator factor
 @return a newly allocated bitmap
 */
ho_bitmap *ho_bitmap_filter_remove_dots (const ho_bitmap * m,
					 const unsigned char erosion_n,
					 const unsigned char dilation_n);

/**
 count the number of objects in a bitmap
 @param m pointer to an ho_bitmap
 @return the number of objects in a bitmap
 */
int ho_bitmap_filter_count_objects (const ho_bitmap * m);

/**
 draw a black box on bitmap
 @param m the bitmap to draw on
 @param x x-start of box
 @param y y-start of box
 @param width width of box
 @param height height of box
 @return FALSE
 */
int
ho_bitmap_draw_box (ho_bitmap * m, const int x, const int y,
		    const int width, const int height);

/**
 draw a black empty box on bitmap
 @param m the bitmap to draw on
 @param x x-start of box
 @param y y-start of box
 @param width width of box
 @param height height of box
 @return FALSE
 */
int
ho_bitmap_draw_box_empty (ho_bitmap * m, const int x, const int y,
			  const int width, const int height);

/**
 draw vertical line on bitmap
 @param m the bitmap to draw on
 @param x x-start of line
 @param y y-start of line
 @param height height of line
 @return FALSE
 */
int
ho_bitmap_draw_vline (ho_bitmap * m, const int x, const int y,
		      const int height);

/**
 delete vertical line on bitmap
 @param m the bitmap to draw on
 @param x x-start of line
 @param y y-start of line
 @param height height of line
 @return FALSE
 */
int
ho_bitmap_delete_vline (ho_bitmap * m, const int x, const int y,
			const int height);

/**
 draw horizontal line on bitmap
 @param m the bitmap to draw on
 @param x x-start of line
 @param y y-start of line
 @param width height of line
 @return FALSE
 */
int
ho_bitmap_draw_hline (ho_bitmap * m, const int x, const int y,
		      const int width);

/**
 delete horizontal line on bitmap
 @param m the bitmap to draw on
 @param x x-start of line
 @param y y-start of line
 @param width height of line
 @return FALSE
 */
int
ho_bitmap_delete_hline (ho_bitmap * m, const int x, const int y,
			const int width);

#endif /* HO_BITMAP_H */
