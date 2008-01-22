
/***************************************************************************
 *            ho_pixbuf.h
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

#ifndef HO_PIXBUF_H
#define HO_PIXBUF_H 1

#include <ho_bitmap.h>
#include <ho_objmap.h>
#include <ho_layout.h>

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
  unsigned char n_channels;
  int height;
  int width;
  int rowstride;
  unsigned char *data;
} ho_pixbuf;

/**
 new ho_pixbuf 
 @param n_channels number of color channels
 @param hight hight of pixbuf in pixels
 @param width width of pixbuf in pixels
 @param rowstride number of bytes in a row
 @return newly allocated ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_new (const unsigned char n_channels,
  const int width, const int height, const int rowstride);

/**
 clone ho_pixbuf
 @param m pointer to a ho_pixbuf image
 @return newly allocated gray ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_clone (const ho_pixbuf * m);

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
  const unsigned char min, const unsigned char max);

/**
 new ho_pixbuf from ho_layout
 @param lay_in pointer to an ho_layout image
 @param show_grid show grid on output
 @param m_text text to draw on output
 @param text_block_r red channel of text block
 @param text_block_g green channel of text block
 @param text_block_b blue channel of text block
 @param text_block_a alfa channel of text block
 @param text_block_frame_width frame width of text block
 @param line_block_r red channel of line block
 @param line_block_g green channel of line block
 @param line_block_b blue channel of line block
 @param line_block_a alfa channel of line block
 @param line_block_frame_width frame line of text block
 @param word_block_r red channel of word block
 @param word_block_g green channel of word block
 @param word_block_b blue channel of word block
 @param word_block_a alfa channel of word block
 @param word_block_frame_width frame width of word block
 @param font_block_r red channel of font block
 @param font_block_g green channel of font block
 @param font_block_b blue channel of font block
 @param font_block_a alfa channel of font block
 @param font_block_frame_width frame width of font block
 @return newly allocated color ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_new_from_layout (const ho_layout * lay_in,
  const unsigned char show_grid, const ho_bitmap * m_text,
  const unsigned char text_block_r, const unsigned char text_block_g,
  const unsigned char text_block_b, const unsigned char text_block_a,
  const unsigned char text_block_frame_width,
  const unsigned char line_block_r, const unsigned char line_block_g,
  const unsigned char line_block_b, const unsigned char line_block_a,
  const unsigned char line_block_frame_width,
  const unsigned char word_block_r, const unsigned char word_block_g,
  const unsigned char word_block_b, const unsigned char word_block_a,
  const unsigned char word_block_frame_width,
  const unsigned char font_block_r, const unsigned char font_block_g,
  const unsigned char font_block_b, const unsigned char font_block_a,
  const unsigned char font_block_frame_width);

/**
 new rgb ho_pixbuf from non rgb pixbuf
 @param pix_in pointer the original pixbuf
 @return newly allocated rgb color ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_to_rgb (const ho_pixbuf * pix_in);

/**
 free an ho_pixbuf 
 @param pix pointer to an ho_pixbuf
 @return FALSE
 */
int ho_pixbuf_free (ho_pixbuf * pix);

/**
 converts a color pixbuf to gray one
 @param pix the color ho_pixbuf
 @return newly allocated gray ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_color_to_gray (const ho_pixbuf * pix);

/**
 take the Red channel from an RGB pixbuf
 @param pix the color ho_pixbuf
 @return newly allocated gray ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_color_to_red (const ho_pixbuf * pix);

/**
 take the Green channel from an RGB pixbuf
 @param pix the color ho_pixbuf
 @return newly allocated gray ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_color_to_green (const ho_pixbuf * pix);

/**
 take the Blue channel from an RGB pixbuf
 @param pix the color ho_pixbuf
 @return newly allocated gray ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_color_to_blue (const ho_pixbuf * pix);

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
 scale a gray pixbuf to by 4
 @param pix the input ho_pixbuf
 @return newly allocated gray ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_scale4 (const ho_pixbuf * pix);

/**
 scale a gray pixbuf
 @param pix the input ho_pixbuf
 @param scale scale by this factor
 @return newly allocated gray ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_scale (const ho_pixbuf * pix, const unsigned char scale);

/**
 get the min and max values in a gray pixbuf
 @param pix gray ho_pixbuf 
 @param min a pointer to return the min 
 @param max a pointer to return the max 
 @return FALSE
 */
unsigned char
ho_pixbuf_minmax (const ho_pixbuf * pix, unsigned char *min,
  unsigned char *max);

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
ho_bitmap *ho_pixbuf_to_bitmap (const ho_pixbuf * pix, unsigned char threshold);

/**
 convert a gray pixbuf to bitmap using adaptive thresholding
 @param pix the input ho_pixbuf
 @param threshold the threshold to use 0..100
 @param size block size for the adaptive steps
 @param adaptive_threshold the threshold to use for adaptive thresholding 0..100
 @return newly allocated gray ho_bitmap
 */
ho_bitmap *ho_pixbuf_to_bitmap_adaptive (const ho_pixbuf * pix,
  unsigned char threshold,
  unsigned char size, unsigned char adaptive_threshold);

/**
 convert a gray pixbuf to bitmap using better adaptive thresholding
 @param pix the input ho_pixbuf
 @param threshold the threshold to use 0..100
 @param size block size for the adaptive steps
 @param adaptive_threshold the threshold to use for adaptive thresholding 0..100
 @return newly allocated gray ho_bitmap
 */
ho_bitmap *ho_pixbuf_to_bitmap_adaptive_fine (const ho_pixbuf *
  pix,
  unsigned char threshold,
  unsigned char size, unsigned char adaptive_threshold);

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
  const unsigned char scale,
  const unsigned char adaptive,
  const unsigned char threshold, const unsigned char a_threshold);

/**
 read ho_pixbuf from pnm file 8 or 24 bpp
 @param filenme file name of pnm file 
 @return newly allocated ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_pnm_load (const char *filename);

/**
 writes ho_pixbuf to pnm file
 @param pix ho_pixbuf 8 or 24 bpp
 @param filenme save as file name 
 @return FALSE
 */
int ho_pixbuf_pnm_save (const ho_pixbuf * pix, const char *filename);

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
 @return FALSE
 */
int
ho_pixbuf_draw_line (ho_pixbuf * m, const int x1, const int y1,
  const int x2, const int y2, const unsigned char red,
  const unsigned char green, const unsigned char blue);

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
 @return FALSE
 */
int
ho_pixbuf_draw_horizontal_scale (ho_pixbuf * m, const int x1,
  const int y1, const int length,
  const int step, const unsigned char red,
  const unsigned char green, const unsigned char blue);

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
 @return FALSE
 */
int
ho_pixbuf_draw_vertical_scale (ho_pixbuf * m, const int x1,
  const int y1, const int length,
  const int step, const unsigned char red,
  const unsigned char green, const unsigned char blue);

/**
  draw a grid
 @param pix a ho_pixbuf 
 @param size size of grid boxes
 @param step size of scale marks
 @param red value of red channel 
 @param green value of green channel 
 @param blue value of blue channel 
 @return FALSE
 */
int
ho_pixbuf_draw_grid (ho_pixbuf * m, const int size, const int step,
  const unsigned char red, const unsigned char green, const unsigned char blue);

/**
  draw a bitmap
 @param m a ho_pixbuf 
 @param bit_in the bitmap to draw on the pixbuf
 @param red value of red channel 
 @param green value of green channel 
 @param blue value of blue channel 
 @param alpha value of alpha channel 
 @return FALSE
 */
int
ho_pixbuf_draw_bitmap (ho_pixbuf * m, const ho_bitmap * bit_in,
  const unsigned char red, const unsigned char green,
  const unsigned char blue, const unsigned char alpha);

/**
  draw RGB bitmap
 @param m a ho_pixbuf 
 @param bit_in_red the bitmap to draw on the pixbuf red channel
 @param bit_in_green the bitmap to draw on the pixbuf green channel
 @param bit_in_blue the bitmap to draw on the pixbuf blue channel
 @return FALSE
 */
int
ho_pixbuf_draw_rgb_bitmap (ho_pixbuf * m, const ho_bitmap * bit_in_red,
  const ho_bitmap * bit_in_green, const ho_bitmap * bit_in_blue);

/**
  draw a bitmap
 @param m a ho_pixbuf 
 @param bit_in the bitmap to draw on the pixbuf
 @param x1 x of upper right start of drawing
 @param y1 y of upper right start of drawing
 @param red value of red channel 
 @param green value of green channel 
 @param blue value of blue channel 
 @return FALSE
 */
int
ho_pixbuf_draw_bitmap_at (ho_pixbuf * m, const ho_bitmap * bit_in,
  const int x1, const int y1,
  const unsigned char red, const unsigned char green,
  const unsigned char blue, const unsigned char alpha);

#endif /* HO_PIXBUF_H */
