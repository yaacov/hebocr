
/***************************************************************************
 *            ho_layout.h
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

/** @file ho_layout.h
    @brief libhocr C language header.
    
    libhocr - LIBrary for Hebrew Optical Character Recognition 
*/

#ifndef HO_LAYOUT_H
#define HO_LAYOUT_H 1

#include <ho_bitmap.h>
#include <ho_objmap.h>

/** @struct ho_layout
  @brief libhocr layout struct
*/
typedef struct
{
  /* paragraph settings type */
  char font_spacing_code;
  unsigned char type;
  unsigned char dir;
  int number_of_fonts;
  
  /* black and white text image */
  ho_bitmap *m_page_text;
  ho_bitmap *m_page_blocks_mask;

  /* line text and masks arrays [number of text blocks] */
  int n_blocks;
  ho_bitmap **m_blocks_text;
  ho_bitmap **m_blocks_lines_mask;

  /* words text and masks arrays [number of text blocks][number of lines in
   * block] */
  int *n_lines;
  ho_bitmap ***m_lines_text;
  ho_bitmap ***m_lines_line_mask;
  ho_bitmap ***m_lines_words_mask;

  /* number of words in a line */
  int **n_words;
  ho_bitmap ****m_words_text;
  ho_bitmap ****m_words_line_mask;
  ho_bitmap ****m_words_font_mask;

  int ***n_fonts;

} ho_layout;

/**
 new ho_layout 
 @param m_page_text a pointer to a text bitmap
 @param font_spacing_code -3 tight .. 0 .. 3 spaced
 @param type free text blocks or boxed 
 @param dir true-ltr false-rtl
 @return a newly allocated layout
 */
ho_layout *ho_layout_new (const ho_bitmap * m_page_text,
  const char font_spacing_code, const unsigned char type, const unsigned char dir);

/**
 free a ho_layout
 @param l_page a pointer to a ho_layout 
 @return FALSE
 */
int ho_layout_free (ho_layout * l_page);

/**
 create a text blocks mask and count blocks in n_blocks
 @param l_page a pointer to a ho_layout 
 @return FALSE
 */
int ho_layout_create_block_mask (ho_layout * l_page);

/**
 create a text lines mask and count lines in n_lines[block_index]
 @param l_page a pointer to a ho_layout 
 @param block_index the block_index to work on
 @return FALSE
 */
int ho_layout_create_line_mask (ho_layout * l_page, const int block_index);

/**
 create a text word mask and count words in n_words[block_index][line_index]
 @param l_page a pointer to a ho_layout
 @param block_index the block_index to work on
 @param line_index the line_index to work on
 @return FALSE
 */
int ho_layout_create_word_mask (ho_layout * l_page, const int block_index,
  const int line_index);

/**
 create a text font mask and count fonts in n_fontss[block_index][line_index][word_index]
 @param l_page a pointer to a ho_layout
 @param block_index the block_index to work on
 @param line_index the line_index to work on
 @param word_index the word_index to work on
 @param slicing_threshold the slicing threshold
 @param slicing_width the slicing width
 @return FALSE
 */
int ho_layout_create_font_mask (ho_layout * l_page, const int block_index,
  const int line_index, const int word_index,
  const unsigned char slicing_threshold, const unsigned char slicing_width);

/**
 new ho_bitamp of the text in a layout block
 
 @param l_page the ho_layout to use
 @param block_index block index
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_layout_get_block_text (const ho_layout * l_page, int block_index);

/**
 new ho_bitamp of the text in a layout line
 
 @param l_page the ho_layout to use
 @param block_index block index
 @param line_index line index
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_layout_get_line_text (const ho_layout * l_page, int block_index,
  int line_index);

/**
 new ho_bitamp of the text in a layout word
 
 @param l_page the ho_layout to use
 @param block_index block index
 @param line_index line index
 @param word_index word index
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_layout_get_word_text (const ho_layout * l_page, int block_index,
  int line_index, int word_index);

/**
 new ho_bitamp of the text in a layout font
 
 @param l_page the ho_layout to use
 @param block_index block index
 @param line_index line index
 @param word_index word index
 @param font_index font index
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_layout_get_font_text (const ho_layout * l_page, int block_index,
  int line_index, int word_index, int font_index);

/**
 new ho_bitamp of the line in a layout line
 
 @param l_page the ho_layout to use
 @param block_index block index
 @param line_index line index
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_layout_get_line_line_mask (const ho_layout * l_page, int block_index,
  int line_index);

/**
 new ho_bitamp of the line in a layout word
 
 @param l_page the ho_layout to use
 @param block_index block index
 @param line_index line index
 @param word_index word index
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_layout_get_word_line_mask (const ho_layout * l_page, int block_index,
  int line_index, int word_index);

/**
 new ho_bitamp of the line in a layout font
 
 @param l_page the ho_layout to use
 @param block_index block index
 @param line_index line index
 @param word_index word index
 @param font_index font index
 @return newly allocated ho_bitmap
 */
ho_bitmap *ho_layout_get_font_line_mask (const ho_layout * l_page, int block_index,
  int line_index, int word_index, int font_index);

#endif /* HO_LAYOUT_H */
