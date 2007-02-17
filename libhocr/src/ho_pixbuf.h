/***************************************************************************
 *            ho_pixbuf.h
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

#ifndef HO_PIXBUF_H
#define HO_PIXBUF_H 1

#include <ho_common.h>
#include <ho_bitmap.h>
#include <ho_objmap.h>

/* utility defines */
#define ho_pixbuf_min3(a,b,c) ((((a)<(b))&&((a)<(c))) ? (a) : (((b)<(c)) ? (b) : (c)))
#define ho_pixbuf_max3(a,b,c) ((((a)>(b))&&((a)>(c))) ? (a) : (((b)>(c)) ? (b) : (c)))

 /* hocr pixbuf set/get macros */
#define ho_pixbuf_set(m,x,y,col,val) (((m)->data)[(x)*(m)->n_channels+(y)*(m)->rowstride+(col)]=(val))
#define ho_pixbuf_get(m,x,y,col) (((m)->data)[(x)*(m)->n_channels+(y)*(m)->rowstride+(col)])

#define ho_pixbuf_get_n_channels(m) ((m)->n_channels)
#define ho_pixbuf_get_width(m) ((m)->width)
#define ho_pixbuf_get_height(m) ((m)->height)
#define ho_pixbuf_get_rowstride(m) ((m)->rowstride)
#define ho_pixbuf_get_data(m) ((m)->data)

/* hocr pixbuf: copy of gtk_pixbuf */
typedef struct
{
  ho_uchar n_channels;
  ho_uint height;
  ho_uint width;
  ho_uint rowstride;
  ho_uchar *data;
} ho_pixbuf;

/**
 new ho_pixbuf 
 @param n_channels number of color channels
 @param hight hight of pixbuf in pixels
 @param width width of pixbuf in pixels
 @param rowstride number of bytes in a row
 @return newly allocated ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_new (const ho_uchar n_channels,
			  const ho_uint width, const ho_uint height,
			  const ho_uint rowstride);

/**
 clone ho_pixbuf
 @param m pointer to a ho_pixbuf image
 @return newly allocated gray ho_pixbuf
 */
ho_pixbuf *
ho_pixbuf_clone (const ho_pixbuf * m);

/**
 new ho_pixbuf from ho_bitmap
 @param bit_in pointer to an ho_bitmap image
 @return newly allocated gray ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_new_from_bitmap (const ho_bitmap * bit_in);

/**
 new ho_pixbuf from ho_objmap
 @param obj_in pointer to an ho_objmap image
 @param min minimal color value
 @param max maximal color value
 @return newly allocated color ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_new_from_objmap (const ho_objmap * obj_in,
				      const ho_uchar min, const ho_uchar max);

/**
 new rgb ho_pixbuf from non rgb pixbuf
 @param pix_in pointer the original pixbuf
 @return newly allocated rgb color ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_to_rgb (const ho_pixbuf * pix_in);

/**
 free an ho_pixbuf 
 @param pix pointer to an ho_pixbuf
 @return HO_FALSE
 */
int ho_pixbuf_free (ho_pixbuf * pix);

/**
 read ho_pixbuf from pnm file 8 or 24 bpp
 @param filenme file name of pnm file 
 @return newly allocated ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_new_from_pnm (const char *filename);

/**
 writes ho_pixbuf to pnm file
 @param pix ho_pixbuf 8 or 24 bpp
 @param filenme save as file name 
 @return HO_FALSE
 */
int ho_pixbuf_save_pnm (const ho_pixbuf * pix, const char *filename);

/**
 converts a color pixbuf to gray one
 @param pix the color ho_pixbuf
 @return newly allocated gray ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_color_to_gray (const ho_pixbuf * pix);

/**
 scale a gray pixbuf to by 2
 @param pix the input ho_pixbuf
 @return newly allocated gray ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_scale2 (const ho_pixbuf * pix);

/**
 scale a gray pixbuf to by 3
 @param pix the input ho_pixbuf
 @return newly allocated gray ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_scale3 (const ho_pixbuf * pix);

/**
 scale a gray pixbuf
 @param pix the input ho_pixbuf
 @param scale scale by this factor
 @return newly allocated gray ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_scale (const ho_pixbuf * pix, const ho_uchar scale);

/**
 get the min and max values in a gray pixbuf
 @param pix gray ho_pixbuf 
 @param min a pointer to return the min 
 @param max a pointer to return the max 
 @return HO_FALSE
 */
ho_uchar
ho_pixbuf_minmax (const ho_pixbuf * pix, ho_uchar * min, ho_uchar * max);

/**
 draw a line from x1,y1 to x2,y2
 @param pix a ho_pixbuf 
 @param x1 x cordinate of start point 
 @param y1 y cordinate of start point 
 @param x2 x cordinate of end point 
 @param y2 y cordinate of end point 
 @param red value of red channel 
 @param green value of green channel 
 @param blue value of blue channel 
 @return HO_FALSE
 */
int
ho_pixbuf_draw_line (ho_pixbuf * m, const ho_uint x1, const ho_uint y1,
		     const ho_uint x2, const ho_uint y2, const ho_uchar red,
		     const ho_uchar green, const ho_uchar blue);

/**
 draw a scale line from x1,y1
 @param pix a ho_pixbuf 
 @param x1 x cordinate of start point 
 @param y1 y cordinate of start point 
 @param length length of scale line 
 @param step size of scale marks
 @param red value of red channel 
 @param green value of green channel 
 @param blue value of blue channel 
 @return HO_FALSE
 */
int
ho_pixbuf_draw_horizontal_scale (ho_pixbuf * m, const ho_uint x1,
				 const ho_uint y1, const ho_uint length,
				 const ho_uint step, const ho_uchar red,
				 const ho_uchar green, const ho_uchar blue);

/**
  draw a scale line from x1,y1
 @param pix a ho_pixbuf 
 @param x1 x cordinate of start point 
 @param y1 y cordinate of start point 
 @param length length of scale line 
 @param step size of scale marks
 @param red value of red channel 
 @param green value of green channel 
 @param blue value of blue channel 
 @return HO_FALSE
 */
int
ho_pixbuf_draw_vertical_scale (ho_pixbuf * m, const ho_uint x1,
			       const ho_uint y1, const ho_uint length,
			       const ho_uint step, const ho_uchar red,
			       const ho_uchar green, const ho_uchar blue);

/**
  draw a grid
 @param pix a ho_pixbuf 
 @param size size of grid boxes
 @param step size of scale marks
 @param red value of red channel 
 @param green value of green channel 
 @param blue value of blue channel 
 @return HO_FALSE
 */
int
ho_pixbuf_draw_grid (ho_pixbuf * m, const ho_uint size, const ho_uint step,
		     const ho_uchar red, const ho_uchar green,
		     const ho_uchar blue);

/**
  draw a bitmap
 @param m a ho_pixbuf 
 @param bit_in the bitmap to draw on the pixbuf
 @param red value of red channel 
 @param green value of green channel 
 @param blue value of blue channel 
 @return HO_FALSE
 */
int
ho_pixbuf_draw_bitmap (ho_pixbuf * m, const ho_bitmap * bit_in,
		       const ho_uchar red, const ho_uchar green,
		       const ho_uchar blue);

/**
  draw a bitmap
 @param m a ho_pixbuf 
 @param bit_in the bitmap to draw on the pixbuf
 @param x1 x of upper right start of drawing
 @param y1 y of upper right start of drawing
 @param red value of red channel 
 @param green value of green channel 
 @param blue value of blue channel 
 @return HO_FALSE
 */
int
ho_pixbuf_draw_bitmap_at (ho_pixbuf * m, const ho_bitmap * bit_in,
			  const ho_uint x1, const ho_uint y1,
			  const ho_uchar red, const ho_uchar green,
			  const ho_uchar blue);
        
/**
 aplay a linear filter to a gray pixbuf 
 @param pix the input ho_pixbuf
 @return newly allocated gray ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_linear_filter (const ho_pixbuf * pix);

/**
 convert a gray pixbuf to bitmap 
 @param pix the input ho_pixbuf
 @param threshold the threshold to use 0..100
 @return newly allocated gray ho_bitmap
 */
ho_bitmap *ho_pixbuf_to_bitmap (const ho_pixbuf * pix, ho_uchar threshold);

/**
 convert a gray pixbuf to bitmap using adaptive thresholding
 @param pix the input ho_pixbuf
 @param threshold the threshold to use 0..100
 @param size block size for the adaptive steps
 @param adaptive_threshold the threshold to use for adaptive thresholding 0..100
 @return newly allocated gray ho_bitmap
 */
ho_bitmap *ho_pixbuf_to_bitmap_adaptive (const ho_pixbuf * pix,
					 ho_uchar threshold, ho_uchar size,
					 ho_uchar adaptive_threshold);

/**
 convert a gray pixbuf to bitmap using better adaptive thresholding
 @param pix the input ho_pixbuf
 @param threshold the threshold to use 0..100
 @param size block size for the adaptive steps
 @param adaptive_threshold the threshold to use for adaptive thresholding 0..100
 @return newly allocated gray ho_bitmap
 */
ho_bitmap *ho_pixbuf_to_bitmap_adaptive_best (const ho_pixbuf *
					      pix,
					      ho_uchar threshold,
					      ho_uchar size,
					      ho_uchar adaptive_threshold);

/**
 convert a gray pixbuf to bitmap wrapper function
 @param pix_in the input ho_pixbuf
 @param scale the scale to use
 @param adaptive what type of thresholding to use
 @param threshold the threshold to use 0..100
 @param a_threshold the threshold to use for adaptive thresholding 0..100
 @return newly allocated gray ho_bitmap
 */
ho_bitmap *ho_pixbuf_to_bitmap_wrapper (const ho_pixbuf * pix_in,
					const ho_uchar scale,
					const ho_uchar adaptive,
					const ho_uchar threshold,
					const ho_uchar a_threshold);

#endif /* HO_PIXBUF_H */
