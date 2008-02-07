
/***************************************************************************
 *            hocrpp.h
 *
 *  Fri Aug 12 20:13:33 2005
 *  Copyright  2005-2007, 2008  Yaacov Zamir
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

/** @file hocrpp.h
    @brief libhocr main C++ language header.
    
    libhocr - LIBrary for Hebrew Optical Character Recognition 
*/

#ifndef HOCRPP_H
#define HOCRPP_H

#include <hocr.h>

/**
 @brief the libhocr namespace.

 namespace for Hebrew Optical Character Recognition 
 */
namespace hocr
{

  /**
   @brief Hocr class.
	
   class for Hebrew Optical Character Recognition 
   */
  class Hocr;

  /**
   @brief Hocr class.
	
   class for Hebrew Optical Character Recognition 
   */
  class Hocr
  {

  public:

    /**
     @brief Hocr constructor.
     */
    Hocr (ho_pixbuf * m_in = NULL)
    {
      /* set pixbuf */
      m_pix_orig = m_in;

      /* lock free pointers */
      m_bit_bw = NULL;
      page_layout = NULL;
      text_out = NULL;

      /* set defaults */
      scale = 0;
      no_auto_scale = 0;
      rotate = 0;
      no_auto_rotate = 0;
      adaptive = 0;
      threshold = 0;
      a_threshold = 0;

      dir_ltr = 0;
      font_spacing_code = 0;
      paragraph_setup = 0;
      slicing_threshold = 0;
      slicing_width = 0;

      font_code = 0;
      html = 0;

      /* init progress */
      progress = 0;
    }

    /**
     @brief Hocr destructor.
     */
     ~Hocr ()
    {
      ho_pixbuf_free (m_pix_orig);
      ho_bitmap_free (m_bit_bw);
      ho_string_free (text_out);
    }

    /**
     @brief set pixbuf
    
     @param m_in the pixbuf to use
     @return 0
     */
    int set_pixbuf (ho_pixbuf * m_in)
    {
      /* free existing data if present */
      ho_pixbuf_free (m_pix_orig);
      ho_bitmap_free (m_bit_bw);
      ho_string_free (text_out);

      /* create a new data based on the new image */
      m_pix_orig = m_in;
      m_bit_bw = NULL;
      page_layout = NULL;
      text_out = NULL;

      progress = 0;

      return 0;
    }

    /**
     @brief set b/w bitmap
    
     @param m_in the b/w bitmap to use
     @return 0
     */
    int set_bitmap (ho_bitmap * m_in)
    {
      /* free existing data if present */
      ho_pixbuf_free (m_pix_orig);
      ho_bitmap_free (m_bit_bw);
      ho_string_free (text_out);

      /* create a new data based on the new image */
      m_pix_orig = NULL;
      m_bit_bw = m_in;
      page_layout = NULL;
      text_out = NULL;

      progress = 0;

      return 0;
    }

    /**
     @brief get pixbuf
    
     @return the object pixbuf
     */
    ho_pixbuf *get_pixbuf ()
    {
      return m_pix_orig;
    }

    /**
     @brief get bitmap
    
     @return the object bitmap
     */
    ho_bitmap *get_bitmap ()
    {
      return m_bit_bw;
    }

    /**
     @brief get bitmap pixbuf
    
     @return a pixbuf of the object bitmap
     */
    ho_pixbuf *get_bitmap_pixbuf ()
    {
      ho_pixbuf *m_pix_out = NULL;

      if (!m_bit_bw)
        return NULL;

      m_pix_out = ho_pixbuf_new_from_bitmap (m_bit_bw);

      return m_pix_out;
    }

    /**
     @brief get layout pixbuf
    
     @return a pixbuf of the object layout
     */
    ho_pixbuf *get_layout_pixbuf ()
    {
      ho_pixbuf *m_pix_out = NULL;

      /* check if we have a layout and text */
      if (!page_layout || !m_bit_bw)
        return NULL;

      /* 
       * m_pix_out = ho_pixbuf_new_from_layout (layout, show_grid, text_bitmap,
       * block_frame_red, block_frame_green, block_frame_blue, block_frame_alfa,
       * block_frame_width, line_frame_red, line_frame_green, line_frame_blue,
       * line_frame_alfa, line_frame_width, word_frame_red, word_frame_green,
       * word_frame_blue, word_frame_alfa, word_frame_width, font_frame_red,
       * font_frame_green, font_frame_blue, font_frame_alfa, font_frame_width); */
      m_pix_out =
        ho_pixbuf_new_from_layout (page_layout, 0, m_bit_bw, 0, 0, 255, 150,
        5, 255, 0, 0, 255, 5, 255, 240, 0, 180, 255, 0, 250, 0, 235, 255);

      return m_pix_out;
    }

    /**
     @brief get text
     
     @return the recognized text
     */
    const char *get_text ()
    {
      /* do we have text ? */
      if (!text_out)
        return NULL;

      return text_out->string;
    }

    /**
     @brief get progress of ocr
     
     @return the progress 0..100
     */
    int get_progress ()
    {
      return progress;
    }

    /**
     @brief do image processing
    
     @return 0
     */
    int do_image_processing ()
    {
      if (!m_pix_orig)
        return 1;

      if (m_bit_bw)
      {
        ho_bitmap_free (m_bit_bw);
        m_bit_bw = NULL;
      }

      m_bit_bw = hocr_image_processing (m_pix_orig,
        scale,
        no_auto_scale,
        rotate, no_auto_rotate, adaptive, threshold, a_threshold, &progress);

      return 0;
    }

    /**
     @brief do layout analysis
    
     @return 0
     */
    int do_layout_analysis ()
    {
      if (!m_bit_bw)
        return 1;

      if (page_layout)
      {
        ho_layout_free (page_layout);
        page_layout = NULL;
      }

      page_layout = hocr_layout_analysis (m_bit_bw,
        font_spacing_code, paragraph_setup,
        slicing_threshold, slicing_width, dir_ltr, &progress);

      return 0;
    }

    /**
     @brief do font recognition
    
     @return 0
     */
    int do_font_recognition ()
    {
      if (!page_layout)
        return 1;

      if (text_out)
      {
        ho_string_free (text_out);
        text_out = NULL;
      }

      text_out = ho_string_new ();

      hocr_font_recognition (page_layout, text_out, font_code, html, &progress);

      return 0;
    }

    /**
     @brief do ocr
    
     @return 0
     */
    int do_ocr ()
    {
      /* do all ocr stages */
      do_image_processing ();
      do_layout_analysis ();
      do_font_recognition ();

      return 0;
    }

    /**
     @brief get html param
    
     @return FALSE-no html in output, TRUE-output html text 
     */
    int get_html()
    {      
      return html;
    }
    
    /**
     @brief set html param
    
     @param html_in new value of html param
     @return FALSE-no html in output, TRUE-output html text 
     */
    int set_html(unsigned char html_in)
    {
      html = html_in;
      
      return html;
    }
    
    /**
     @brief get font param
    
     @return font code of objct
     */
    int get_font()
    {      
      return font_code;
    }
    
    /**
     @brief set font param
    
     @param font_in new value of font param
     @return font code of objct
     */
    int set_font(unsigned char font_in)
    {
      font_code = font_in;
      
      return font_code;
    }
    
  private:

    ho_pixbuf * m_pix_orig;
    ho_bitmap *m_bit_bw;
    ho_layout *page_layout;
    ho_string *text_out;

    unsigned char scale;
    unsigned char no_auto_scale;
    double rotate;
    unsigned char no_auto_rotate;
    unsigned char adaptive;
    unsigned char threshold;
    unsigned char a_threshold;

    unsigned char dir_ltr;
    int font_spacing_code;
    int paragraph_setup;
    int slicing_threshold;
    int slicing_width;

    unsigned char font_code;
    unsigned char html;

    int progress;
  };                            // class
}                               // name space

#endif /* HOCRPP_H */
