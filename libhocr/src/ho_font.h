
/***************************************************************************
 *            ho_font.h
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

/** @file ho_font.h
    @brief libhocr C language header.
    
    libhocr - LIBrary for Hebrew Optical Character Recognition 
*/

#ifndef HO_FONT_H
#define HO_FONT_H 1

#include <ho_bitmap.h>
#include <ho_objmap.h>

/**
 return a bitmap of the main signs in the font
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_main_sign (const ho_bitmap * m_text,
  const ho_bitmap * m_mask);

/**
 return a bitmap of the second object sign in the font
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_second_object (const ho_bitmap * m_text,
  const ho_bitmap * m_mask);

/**
 return a bitmap of holes in the bitmap
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_holes (const ho_bitmap * m_text, const ho_bitmap * m_mask);

/**
 return a bitmap of horizontal bars found in the bitmap
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_hbars (const ho_bitmap * m_text, const ho_bitmap * m_mask);

/**
 return a bitmap of vertical bars found in the bitmap
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_vbars (const ho_bitmap * m_text, const ho_bitmap * m_mask);

/**
 return a bitmap of -45 deg. diagonals found in the bitmap
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_diagonal (const ho_bitmap * m_text,
  const ho_bitmap * m_mask);

/**
 return a bitmap of +45 deg. diagonals found in the bitmap
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_diagonal_left (const ho_bitmap * m_text,
  const ho_bitmap * m_mask);

/**
 return a thinned bitmap without fixing artefacts
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_thin_naive (const ho_bitmap * m_text,
  const ho_bitmap * m_mask);

/**
 return a thinned bitmap and fix artefacts
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_thin (const ho_bitmap * m_text, const ho_bitmap * m_mask);

/**
 return a bitmap of cross points in input image
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_cross (const ho_bitmap * m_text, const ho_bitmap * m_mask);

/**
 return a bitmap of end points in input image
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_ends (const ho_bitmap * m_text, const ho_bitmap * m_mask);

/**
 return a bitmap of top side picks in the font
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_edges_top (const ho_bitmap * m_text,
  const ho_bitmap * m_mask);

/**
 return a bitmap of big top side picks in the font
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_edges_top_big (const ho_bitmap * m_text,
  const ho_bitmap * m_mask);

/**
 return a bitmap of bottom side picks in the font
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_edges_bottom (const ho_bitmap * m_text,
  const ho_bitmap * m_mask);

/**
 return a bitmap of big bottom side picks in the font
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */

ho_bitmap *ho_font_edges_bottom_big (const ho_bitmap * m_text,
  const ho_bitmap * m_mask);

/**
 return a bitmap of left side picks in the font
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_edges_left (const ho_bitmap * m_text,
  const ho_bitmap * m_mask);

/**
 return a bitmap of big left side picks in the font
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_edges_left_big (const ho_bitmap * m_text,
  const ho_bitmap * m_mask);

/**
 return a bitmap of right side picks in the font
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_edges_right (const ho_bitmap * m_text,
  const ho_bitmap * m_mask);

/**
 return a bitmap of big right side picks in the font
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_edges_right_big (const ho_bitmap * m_text,
  const ho_bitmap * m_mask);

/**
 return a bitmap of top side notches in the font
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_notch_top (const ho_bitmap * m_text,
  const ho_bitmap * m_mask);

/**
 return a bitmap of bottom side notches in the font
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_notch_bottom (const ho_bitmap * m_text,
  const ho_bitmap * m_mask);

/**
 return a bitmap of left side notches in the font
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_notch_left (const ho_bitmap * m_text,
  const ho_bitmap * m_mask);

/**
 return a bitmap of right side notches in the font
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_notch_right (const ho_bitmap * m_text,
  const ho_bitmap * m_mask);

/**
 return a bitmap of a filter 
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @param filter_index the index of the filter to use
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_filter (const ho_bitmap * m_text,
  const ho_bitmap * m_mask, int filter_index);

/**
 return a bitmap of a filter used on font holes
 @param m_text pointer to the text bitmap
 @param m_mask pointer to the line map
 @param filter_index the index of the filter to use
 @return a newly allocated bitmap
 */
ho_bitmap *ho_font_holes_filter (const ho_bitmap * m_text,
  const ho_bitmap * m_mask, int filter_index);

/**
 writes font to file
 
 @param m_text ho_bitmap of main font
 @param m_nikud ho_bitmap of nikud
 @param m_mask ho_bitmap of line
 @param filename save as file name 
 @return FALSE
 */
int
ho_font_pnm_save (const ho_bitmap * m_text, const ho_bitmap * m_nikud,
  const ho_bitmap * m_mask, const char *filename);

/**
 read ho_pixbuf from file
 
 @param m_text pointer to return ho_bitmap of main font
 @param m_nikud pointer to return ho_bitmap of nikud
 @param m_mask pointer to return ho_bitmap of line
 @param filename file name
 @return newly allocated ho_pixbuf
 */
int
ho_font_pnm_load (ho_bitmap ** m_text, ho_bitmap ** m_nikud,
  ho_bitmap ** m_mask, const char *filename);

#endif /* HO_FONT_H */
