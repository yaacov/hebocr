/***************************************************************************
 *            ho_recognize.h
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

#ifndef HO_RECOGNIZE_H
#define HO_RECOGNIZE_H 1

#include <ho_bitmap.h>
#include <ho_objmap.h>

#define HO_ARRAY_IN_SIZE 39
#define HO_ARRAY_OUT_SIZE 39

/* font shapes codes*/
static char *ho_sign_array[HO_ARRAY_OUT_SIZE] =
  { "*", "א", "ב", "ג", "ד", "ה", "ו", "ז", "ח", "ט", "י", "כ",
  "ך", "ל", "מ", "ם", "נ", "ן", "ס", "ע", "פ", "ף", "צ", "ץ",
  "ק", "ר", "ש", "ת", ".", ",",
  "'", "?", "!", ":", ";", ")", "(", "-", "+"
};

int ho_recognize_array_in_size ();
int ho_recognize_array_out_size ();

int
ho_recognize_create_array_in (const ho_bitmap * m_text,
			      const ho_bitmap * m_mask, double *array_in);

int ho_recognize_create_array_out (const double *array_in, double *array_out);

char *ho_recognize_array_out_to_font (const double *array_out);

char *ho_recognize_font (const ho_bitmap * m_text, const ho_bitmap * m_mask);

int
ho_recognize_bars (const ho_bitmap * m_text,
		   const ho_bitmap * m_mask, double *has_top_bar,
		   double *has_bottom_bar, double *has_left_bar,
		   double *has_right_bar, double *has_diagonal_bar);

int
ho_recognize_edges (const ho_bitmap * m_text,
		    const ho_bitmap * m_mask, double *has_top_left_edge,
		    double *has_bottom_left_edge, double *has_top_right_edge,
		    double *has_bottom_right_edge, double *has_left_top_edge,
		    double *has_right_top_edge, double *has_left_bottom_edge,
		    double *has_right_bottom_edge);

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
		      double *has_mid_bottom_notch,
		      double *has_right_bottom_notch);

int
ho_recognize_parts (const ho_bitmap * m_text,
		    const ho_bitmap * m_mask, double *has_one_hole,
		    double *has_two_holes, double *has_hey_part);

int
ho_recognize_ends (const ho_bitmap * m_text,
		   const ho_bitmap * m_mask, double *has_top_left_end,
		   double *has_top_right_end, double *has_bottom_left_end,
		   double *has_bottom_right_end, double *has_mid_mid_cross,
		   double *has_mid_bottom_cross);

#endif /* HO_RECOGNIZE_H */
