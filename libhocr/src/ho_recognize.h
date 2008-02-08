
/***************************************************************************
 *            ho_recognize.h
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

/** @file ho_recognize.h
    @brief libhocr C language header.
    
    libhocr - LIBrary for Hebrew Optical Character Recognition 
*/

#ifndef HO_RECOGNIZE_H
#define HO_RECOGNIZE_H 1

#include <ho_bitmap.h>
#include <ho_objmap.h>

#define HO_ARRAY_IN_SIZE 104
#define HO_ARRAY_OUT_SIZE 49

/* font shapes codes*/
static const char *ho_sign_array[HO_ARRAY_OUT_SIZE] =
  { "*", "א", "ב", "ג", "ד", "ה", "ו", "ז", "ח", "ט", "י", "כ",
  "ך", "ל", "מ", "ם", "נ", "ן", "ס", "ע", "פ", "ף", "צ", "ץ",
  "ק", "ר", "ש", "ת", ".", ",",
  "'", "?", "!", ":", ";", ")", "(", "-", "+",
  "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
};

int ho_recognize_array_in_size ();
int ho_recognize_array_out_size ();

int
ho_recognize_create_array_in (const ho_bitmap * m_text,
  const ho_bitmap * m_mask, double *array_in);

int ho_recognize_create_array_out (const double *array_in, double *array_out,
  int font_code);

const char *ho_recognize_array_out_to_font (const double *array_out);

const char *ho_recognize_font (const ho_bitmap * m_text,
  const ho_bitmap * m_mask, int font_code);

int
ho_recognize_dimentions (const ho_bitmap * m_text,
  const ho_bitmap * m_mask, double *height,
  double *width, double *top, double *bottom,
  double *top_left, double *top_mid, double *top_right,
  double *mid_left, double *mid_right,
  double *bottom_left, double *bottom_mid,
  double *bottom_right, double *has_two_hlines_up,
  double *has_two_hlines_down,
  double *has_three_hlines_up, double *has_three_hlines_down);

int
ho_recognize_bars (const ho_bitmap * m_text,
  const ho_bitmap * m_mask, double *has_top_bar,
  double *has_mid_hbar, double *has_bottom_bar, double *has_left_bar,
  double *has_mid_vbar, double *has_right_bar, double *has_diagonal_bar,
  double *has_diagonal_left_bar);

int
ho_recognize_edges (const ho_bitmap * m_text,
  const ho_bitmap * m_mask,
  double *has_top_left_edge,
  double *has_mid_left_edge,
  double *has_bottom_left_edge,
  double *has_top_right_edge,
  double *has_mid_right_edge,
  double *has_bottom_right_edge,
  double *has_left_top_edge,
  double *has_mid_top_edge,
  double *has_right_top_edge,
  double *has_left_bottom_edge,
  double *has_mid_bottom_edge, double *has_right_bottom_edge);

int
ho_recognize_edges_big (const ho_bitmap * m_text,
  const ho_bitmap * m_mask,
  double *has_top_left_edge,
  double *has_mid_left_edge,
  double *has_bottom_left_edge,
  double *has_top_right_edge,
  double *has_mid_right_edge,
  double *has_bottom_right_edge,
  double *has_left_top_edge,
  double *has_mid_top_edge,
  double *has_right_top_edge,
  double *has_left_bottom_edge,
  double *has_mid_bottom_edge, double *has_right_bottom_edge);

int
ho_recognize_notches (const ho_bitmap * m_text,
  const ho_bitmap * m_mask,
  double *has_top_left_notch,
  double *has_mid_left_notch,
  double *has_bottom_left_notch,
  double *has_top_right_notch,
  double *has_mid_right_notch,
  double *has_bottom_right_notch,
  double *has_left_top_notch,
  double *has_mid_top_notch,
  double *has_right_top_notch,
  double *has_left_bottom_notch,
  double *has_mid_bottom_notch, double *has_right_bottom_notch);

int
ho_recognize_parts (const ho_bitmap * m_text,
  const ho_bitmap * m_mask, double *has_one_hole,
  double *has_two_holes, double *has_hey_part,
  double *has_dot_part, double *has_comma_part);

int
ho_recognize_ends (const ho_bitmap * m_text,
  const ho_bitmap * m_mask,
  double *has_top_left_end,
  double *has_top_mid_end,
  double *has_top_right_end,
  double *has_mid_left_end,
  double *has_mid_mid_end,
  double *has_mid_right_end,
  double *has_bottom_left_end,
  double *has_bottom_mid_end,
  double *has_bottom_right_end,
  double *has_top_left_cross,
  double *has_top_mid_cross,
  double *has_top_right_cross,
  double *has_mid_left_cross,
  double *has_mid_mid_cross,
  double *has_mid_right_cross,
  double *has_bottom_left_cross,
  double *has_bottom_mid_cross, double *has_bottom_right_cross);

int
ho_recognize_holes_dimentions (const ho_bitmap * m_text,
  const ho_bitmap * m_mask, double *height,
  double *width, double *top, double *bottom,
  double *top_left, double *top_right,
  double *bottom_left, double *bottom_right);

int
ho_recognize_holes_edges (const ho_bitmap * m_text,
  const ho_bitmap * m_mask,
  double *has_top_left_edge,
  double *has_mid_left_edge,
  double *has_bottom_left_edge,
  double *has_top_right_edge,
  double *has_mid_right_edge,
  double *has_bottom_right_edge,
  double *has_left_top_edge,
  double *has_mid_top_edge,
  double *has_right_top_edge,
  double *has_left_bottom_edge,
  double *has_mid_bottom_edge, double *has_right_bottom_edge);

#endif /* HO_RECOGNIZE_H */
