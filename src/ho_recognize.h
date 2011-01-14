
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
#define HO_ARRAY_OUT_SIZE 51

/* font shapes codes */
static const char *ho_sign_array[HO_ARRAY_OUT_SIZE] =
  { "*", "א", "ב", "ג", "ד", "ה", "ו", "ז", "ח", "ט", "י", "כ",
  "ך", "ל", "מ", "ם", "נ", "ן", "ס", "ע", "פ", "ף", "צ", "ץ",
  "ק", "ר", "ש", "ת", ".", ",",
  "'", "\"", "?", "!", ":", ";", ")", "(", "-", "+", "/",
  "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
};

int ho_recognize_array_in_size ();

int ho_recognize_array_out_size ();

/* make array_in more readble */

#define HEIGHT (array_in[0])
#define WIDTH (array_in[1])
#define WIDTH_BY_HEIGHT (array_in[2])

#define TOP (array_in[3])
#define BOTTOM (array_in[4])

#define TOP_LEFT (array_in[5])
#define TOP_MID (array_in[6])
#define TOP_RIGHT (array_in[7])

#define MID_LEFT (array_in[8])
#define MID_RIGHT (array_in[9])

#define BOTTOM_LEFT (array_in[10])
#define BOTTOM_MID (array_in[11])
#define BOTTOM_RIGHT (array_in[12])

#define TWO_VLINES_UP (array_in[13])
#define TWO_VLINES_DOWN (array_in[14])
#define THREE_VLINES_UP (array_in[15])
#define THREE_VLINES_DOWN (array_in[16])

#define TOP_HBAR (array_in[17])
#define MID_HBAR (array_in[18])
#define BOTTOM_HBAR (array_in[19])

#define LEFT_VBAR (array_in[20])
#define MID_VBAR (array_in[21])
#define RIGHT_VBAR (array_in[22])

#define DIAGONAL_BAR (array_in[23])
#define DIAGONAL_LEFT_BAR (array_in[24])

#define TOP_LEFT_EDGE (array_in[25])
#define MID_LEFT_EDGE (array_in[26])
#define BOTTOM_LEFT_EDGE (array_in[27])

#define TOP_RIGTH_EDGE (array_in[28])
#define MID_RIGTH_EDGE (array_in[29])
#define BOTTOM_RIGTH_EDGE (array_in[30])

#define LEFT_TOP_EDGE (array_in[31])
#define MID_TOP_EDGE (array_in[32])
#define RIGHT_TOP_EDGE (array_in[33])

#define LEFT_BOTTOM_EDGE (array_in[34])
#define MID_BOTTOM_EDGE (array_in[35])
#define RIGHT_BOTTOM_EDGE (array_in[36])

#define TOP_LEFT_NOTCH (array_in[37])
#define MID_LEFT_NOTCH (array_in[38])
#define BOTTOM_LEFT_NOTCH (array_in[39])

#define TOP_RIGHT_NOTCH (array_in[40])
#define MID_RIGHT_NOTCH (array_in[41])
#define BOTTOM_RIGHT_NOTCH (array_in[42])

#define LEFT_TOP_NOTCH (array_in[43])
#define MID_TOP_NOTCH (array_in[44])
#define RIGHT_TOP_NOTCH (array_in[45])

#define LEFT_BOTTOM_NOTCH (array_in[46])
#define MID_BOTTOM_NOTCH (array_in[47])
#define RIGHT_BOTTOM_NOTCH (array_in[48])

#define TOP_LEFT_END (array_in[49])
#define TOP_MID_END (array_in[50])
#define TOP_RIGHT_END (array_in[51])

#define MID_LEFT_END (array_in[52])
#define MID_MID_END (array_in[53])
#define MID_RIGHT_END (array_in[54])

#define BOTTOM_LEFT_END (array_in[55])
#define BOTTOM_MID_END (array_in[56])
#define BOTTOM_RIGHT_END (array_in[57])

#define TOP_LEFT_CROSS (array_in[58])
#define TOP_MID_CROSS (array_in[59])
#define TOP_RIGHT_CROSS (array_in[60])

#define MID_LEFT_CROSS (array_in[61])
#define MID_MID_CROSS (array_in[62])
#define MID_RIGHT_CROSS (array_in[63])

#define BOTTOM_LEFT_CROSS (array_in[64])
#define BOTTOM_MID_CROSS (array_in[65])
#define BOTTOM_RIGHT_CROSS (array_in[66])

#define ONE_HOLE (array_in[67])
#define TWO_HOLES (array_in[68])
#define HEY_PART (array_in[69])
#define DOT_PART (array_in[70])
#define COMMA_PART (array_in[71])

#define HOLE_TOP_LEFT_EDGE (array_in[72])
#define HOLE_MID_LEFT_EDGE (array_in[73])
#define HOLE_BOTTOM_LEFT_EDGE (array_in[74])

#define HOLE_TOP_RIGTH_EDGE (array_in[75])
#define HOLE_MID_RIGTH_EDGE (array_in[76])
#define HOLE_BOTTOM_RIGTH_EDGE (array_in[77])

#define HOLE_LEFT_TOP_EDGE (array_in[78])
#define HOLE_MID_TOP_EDGE (array_in[79])
#define HOLE_RIGHT_TOP_EDGE (array_in[80])

#define HOLE_LEFT_BOTTOM_EDGE (array_in[81])
#define HOLE_MID_BOTTOM_EDGE (array_in[82])
#define HOLE_RIGHT_BOTTOM_EDGE (array_in[83])

#define HOLE_HEIGHT (array_in[84])
#define HOLE_WIDTH (array_in[85])

#define HOLE_TOP (array_in[86])
#define HOLE_BOTTOM (array_in[87])

#define HOLE_TOP_LEFT (array_in[88])
#define HOLE_TOP_RIGHT (array_in[89])

#define HOLE_BOTTOM_LEFT (array_in[90])
#define HOLE_BOTTOM_RIGHT (array_in[91])

#define BIG_TOP_LEFT_EDGE (array_in[92])
#define BIG_MID_LEFT_EDGE (array_in[93])
#define BIG_BOTTOM_LEFT_EDGE (array_in[94])

#define BIG_TOP_RIGTH_EDGE (array_in[95])
#define BIG_MID_RIGTH_EDGE (array_in[96])
#define BIG_BOTTOM_RIGTH_EDGE (array_in[97])

#define BIG_LEFT_TOP_EDGE (array_in[98])
#define BIG_MID_TOP_EDGE (array_in[99])
#define BIG_RIGHT_TOP_EDGE (array_in[100])

#define BIG_LEFT_BOTTOM_EDGE (array_in[101])
#define BIG_MID_BOTTOM_EDGE (array_in[102])
#define BIG_RIGHT_BOTTOM_EDGE (array_in[103])

int
ho_recognize_create_array_in (const ho_bitmap * m_text,
  const ho_bitmap * m_mask, double *array_in);

int ho_recognize_create_array_out (const double *array_in, double *array_out,
  int font_code);

const char *ho_recognize_array_out_to_font (const double *array_out,
  int *this_char_i);

const char *ho_recognize_font (const ho_bitmap * m_text,
  const ho_bitmap * m_mask, int font_code, unsigned char do_linguistics,
  unsigned char word_end, unsigned char word_start, int *this_char_i,
  int last_char_i);

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
