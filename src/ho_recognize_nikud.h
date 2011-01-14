
/***************************************************************************
 *            ho_recognize_nikud.h
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

/** @file ho_recognize_nikud.h
    @brief libhocr C language header.
    
    libhocr - LIBrary for Hebrew Optical Character Recognition 
*/

#ifndef HO_RECOGNIZE_NIKUD_H
#define HO_RECOGNIZE_NIKUD_H 1

#include <ho_bitmap.h>
#include <ho_objmap.h>

#define HO_NIKUD_ARRAY_IN_SIZE 24
#define HO_NIKUD_ARRAY_OUT_SIZE 16

/* font shapes codes*/
static const char *ho_nikud_array[HO_NIKUD_ARRAY_OUT_SIZE] = { "",  /* none */
  "ָ",                         /* kamatz */
  "ַ",                         /* patach */
  "ֵ",                         /* tzere */
  "ֶ",                         /* segol */
  "ֹ",                         /* cholam */
  "ּ",                         /* shorok */
  "ֻ",                         /* kobotz */
  "ִ",                         /* chirik */
  "ְ",                         /* shva */
  "ֱ",                         /* hataf segol */
  "ֲ",                         /* hataf patach */
  "ֳ",                         /* hataf kamatz */
  "ּ",                         /* dagesh */
  "ׁ",                         /* shin */
  "ׂ"                          /* sin */
};

int ho_recognize_nikud_array_in_size ();
int ho_recognize_nikud_array_out_size ();

int
ho_recognize_nikud_array_in (const ho_bitmap * m_nikud,
  const ho_bitmap * m_mask, double *array_in);

int ho_recognize_nikud_array_out (const double *array_in,
  double *array_out, int font_code);

const char *ho_recognize_array_out_to_nikud (const double *array_out);

const char *ho_recognize_nikud (const ho_bitmap * m_nikud,
  const ho_bitmap * m_mask, int font_code, const char **dagesh,
  const char **shin);

int
ho_recognize_nikud_dimentions (const ho_bitmap * m_text,
  const ho_bitmap * m_mask, double *height,
  double *width, double *top, double *bottom,
  double *top_left, double *top_mid, double *top_right,
  double *mid_left, double *mid_right,
  double *bottom_left, double *bottom_mid,
  double *bottom_right,
  double *dots_above, double *dots_below, double *dots_inside,
  double *objs_above, double *objs_below, double *objs_inside,
  double *main_obj_height, double *main_obj_width,
  double *top_dot_x, double *inside_dot_x, double *font_width_by_height);

#endif /* HO_RECOGNIZE_NIKUD_H */
