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
#define ho_bitmap_get(m,x,y) ((((m)->data[(x) / 8 + (y) * (m)->rowstride]) & (0x80 >> ((x) % 8))) > 0)
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

#endif /* HO_BITMAP_H */
