/***************************************************************************
 *            hocrpp.h
 *
 *  Fri Aug 12 20:18:34 2005
 *  Copyright  2005  Yaacov Zamir
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __HOCR_PP_H__
#define __HOCR_PP_H__

#include <hocr.h>

/**
 @brief the libhocr namespace.
 */
namespace hocr
{

	/**
	 @brief Hocr class.
	
	 class for Hebrew OCR
	 */
	class Hocr;

	/**
	 @brief Hocr class.
	
	 class for Hebrew OCR
	 */
	class Hocr
	{
	      public:
		////////////////////////////////////////
		////////////////////////////////////////

		/**
		 @brief Hocr constructor.
		 */
		Hocr ()
		{
			h = (hocr_pixbuf *) malloc (sizeof (hocr_pixbuf));
			
			h->n_channels = 3;
			h->brightness = 100;
			h->pixels = NULL;
			h->width = 0;
			h->height = 0;
			h->rowstride = 0;
		}
		
		/**
		 @brief Hocr constructor.
			
		 @param filename path to a pbm file, file must be binary b/w pnm file ("P4/5").
		 */
		Hocr (const char *filename)
		{
			h = hocr_pixbuf_new_from_file (filename);
		}

		/**
		 @brief Hocr destructor.
		 */
		 ~Hocr ()
		{
			hocr_pixbuf_unref (h);
		}

		/**
		 @brief do ocr on a hocr_pixbuf and return the result text to text_buffer

		 @param text_buffer pointer to an already allocated text buffer for the results
		 @param max_buffer_size site of allocated memory for text_buffer.
		 @return 1
		 */
		int do_ocr (char *text_buffer, int max_buffer_size)
		{
			return hocr_do_ocr (h, text_buffer,
					    max_buffer_size);
		}

		////////////////////////////////////////
		////////////////////////////////////////

		/**
		 @brief get number of channels
		
		 @return number of color channels in the pixpuf (e.g. 3 for red,green,blue).
		 */
		int get_n_channels ()
		{
			return hocr_pixbuf_get_n_channels (h);
		}

		/**
		 @brief get height in pixels
		
		 @return height of pixpuf in pixels.
		 */
		int get_height ()
		{
			return hocr_pixbuf_get_height (h);
		}

		/**
		 @brief get width in pixels
		
		 @return width of pixpuf in pixels.
		 */
		int get_width ()
		{
			return hocr_pixbuf_get_width (h);
		}

		/**
		 @brief get number of bytes in a raw of pixels
		
		 @return how many bytes are used by the struct for one raw.
		 */
		int get_rowstride ()
		{
			return hocr_pixbuf_get_rowstride (h);
		}
		/**
		 @brief get value from which a gray-scale pixel is considered white
		
		 @return value from which a gray-scale pixel is considered white.
		 */
		int get_brightness ()
		{
			return hocr_pixbuf_get_brightness (h);
		}

		/**
		 @brief get pointer to raw pixpuf data
		
		 @return pointer to raw pixpuf data
		 */
		unsigned char *get_pixels ()
		{
			return hocr_pixbuf_get_pixels (h);
		}

		//////////////////////////////////////////////
		//////////////////////////////////////////////
		
		/**
		 @brief set number of channels
		
		 @return number of color channels in the pixpuf (e.g. 3 for red,green,blue).
		 */
		int set_n_channels (int n)
		{
			h->n_channels = n;
			
			return n;
		}

		/**
		 @brief set height in pixels
		
		 @return height of pixpuf in pixels.
		 */
		int set_height (int n)
		{
			h->height = n;
			
			return n;
		}

		/**
		 @brief set width in pixels
		
		 @return width of pixpuf in pixels.
		 */
		int set_width (int n)
		{
			h->width = n;
			
			return n;
		}

		/**
		 @brief set number of bytes in a raw of pixels
		
		 @return how many bytes are used by the struct for one raw.
		 */
		int set_rowstride (int n)
		{
			h->rowstride = n;
			
			return n;
		}
		/**
		 @brief set value from which a gray-scale pixel is considered white
		
		 @return value from which a gray-scale pixel is considered white.
		 */
		int set_brightness (int n)
		{
			h->brightness = n;
			
			return n;
		}

		/**
		 @brief set pointer to raw pixpuf data
		
		 Use with care, this pointer will be automaticaly free
		 at end of scope !! do not refernce to objects you need
		 alive after scope ends !! may cuse memory loss !!
		
		 @return pointer to raw pixpuf data
		 */
		unsigned char *set_pixels (unsigned char * p)
		{
			h->pixels = p;
			
			return p;
		}
		
		//////////////////////////////////////////////
		//////////////////////////////////////////////
		
		/**
		 @brief get color of pixel
		
		 @param x position of pixel on x axis
		 @param y position of pixel on y axis
		 @return 1 - if pixel is black, 0 - if pixel is white
		 */
		int get_pixel (int x, int y)
		{
			return hocr_pixbuf_get_pixel (h, x, y);
		}

		/**
		 @brief set color of pixel color channel
		
		 @param x position of pixel on x axis
		 @param y position of pixel on y axis
		 @param channel the color chanell to set
		 @param value the value to set the chanell to
		 @return the color set
		 */
		int set_pixel (int x, int y, int channel, int value)
		{
			return hocr_pixbuf_set_pixel (h, x, y, channel,
						      value);
		}

		////////////////////////////////////////
		////////////////////////////////////////

	      private:

		hocr_pixbuf * h;

	};

}	// name space

#endif
