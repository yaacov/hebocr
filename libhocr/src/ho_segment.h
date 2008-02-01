
/***************************************************************************
 *            ho_segment.h
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

/** @file ho_segment.h
    @brief libhocr C language header.
    
    libhocr - LIBrary for Hebrew Optical Character Recognition 
*/

#ifndef HO_SEGMENT_H
#define HO_SEGMENT_H 1

#include <ho_bitmap.h>
#include <ho_objmap.h>

/**
 return a bitmap of the paragraphs, 
 font and line dimentions _must_ be set
 @param m pointer to an ho_bitmap
 @param box squre (boxes) or free formed text blocks 
 @param font_height_factor_min filter objects height min
 @param font_height_factor_max filter objects height max
 @param font_width_factor_min filter objects width min
 @param font_width_factor_max filter objects width max
 @param horizontal_link_factor link horizontaly factor
 @param vertical_link_factor link verticaly factor
 @return a newly allocated bitmap
 */
ho_bitmap *ho_segment_paragraphs_fine (const ho_bitmap * m,
  const unsigned char box, const double font_height_factor_min,
  const double font_height_factor_max, const double font_width_factor_min,
  const double font_width_factor_max, const double horizontal_link_factor,
  const double vertical_link_factor);

/**
 return a bitmap of the paragraphs, 
 font and line dimentions _must_ be set
 @param m pointer to an ho_bitmap
 @param box squre (boxes) or free formed text blocks 
 @return a newly allocated bitmap
 */
ho_bitmap *ho_segment_paragraphs (const ho_bitmap * m, const unsigned char box);

/**
 return a bitmap of the lines
 @param m pointer to an ho_bitmap
 @param font_height_factor_min filter objects height min
 @param font_height_factor_max filter objects height max
 @param font_width_factor_min filter objects width min
 @param font_width_factor_max filter objects width max
 @param link_arg factor for linking letters in line
 @param link_arg_2 factor for linking letters in line
 @param extend_arg factor for extemding lines horizontaly
 @param extend_arg_2 factor for extemding lines horizontaly
 @param erode_arg factor for eroding line sourface
 @return a newly allocated bitmap
 */
ho_bitmap *ho_segment_lines_fine (const ho_bitmap * m,
  const double font_height_factor_min, const double font_height_factor_max,
  const double font_width_factor_min, const double font_width_factor_max,
  const double link_arg, const double link_arg_2,
  const double extend_arg, const double extend_arg_2, const double erode_arg);

/**
 return a bitmap of the lines
 @param m pointer to an ho_bitmap
 @return a newly allocated bitmap
 */
ho_bitmap *ho_segment_lines (const ho_bitmap * m);

/**
 return a bitmap of the words
 @param m pointer to an ho_bitmap of line
 @param m_line_map pointer to an ho_bitmap of line box
 @param horizontal_link_factor horizontal link factor
 @param top_frame_factor top frame factor
 @param bottom_frame_factor bottom frame factor
 @return a newly allocated bitmap
 */
ho_bitmap *ho_segment_words_fine (const ho_bitmap * m,
  const ho_bitmap * m_line_map, const double horizontal_link_factor,
  const double top_frame_factor, const double bottom_frame_factor);

/**
 return a bitmap of the words
 @param m pointer to an ho_bitmap of text
 @param m_line_map pointer to an ho_bitmap of line
 @param font_spacing_code -1:tight 0:normal 1:spaced fonts
 @return a newly allocated bitmap
 */
ho_bitmap *ho_segment_words (const ho_bitmap * m, const ho_bitmap * m_line_map,
  const char font_spacing_code);

/**
 return a bitmap of the interfont spaces
 @param m pointer to an ho_bitmap of text
 @param m_line_map pointer to an ho_bitmap of line
 @param slicing_threshold percent of line fill declared space between fonts
 @param slicing_width percent of font width above try to choop font in two
 @return a newly allocated bitmap
 */
ho_bitmap *ho_segment_fonts (const ho_bitmap * m,
  const ho_bitmap * m_line_map,
  const unsigned char slicing_threshold, const unsigned char slicing_width);

#endif /* HO_SEGMENT_H */
