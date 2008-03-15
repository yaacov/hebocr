
/***************************************************************************
 *            ho_recognize_font_1.c
 *
 *  Fri Aug 12 20:13:33 2005
 *  Copyright  2005-2008  Yaacov Zamir
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#ifndef TRUE
#define TRUE -1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL ((void*)0)
#endif

#include "ho_bitmap.h"
#include "ho_objmap.h"
#include "ho_segment.h"
#include "ho_font.h"
#include "ho_recognize.h"

#include "ho_recognize_font_1.h"

/* font recognition functions */

/* functions get an array of doubles 0..1 and return a double 0..1 
 * 0 if this does not look like the font until 1 looks exactly like the font */

double
ho_recognize_font_1_zero (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_1_one (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_1_two (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_1_three (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_1_four (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_1_five (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_1_six (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_1_seven (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_1_eight (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_1_nine (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_1_dot (const double *array_in)
{
  double return_value = 0.0;

  if (TOP < 0.3)
    return_value += 0.1;
  if (BOTTOM < 0.53 && BOTTOM > 0.47)
    return_value += 0.2;
  if (WIDTH < 0.3)
    return_value += 0.1;
  if (HEIGHT < 0.3)
    return_value += 0.2;

  if (BOTTOM_LEFT_EDGE)
    return_value -= 0.3;
  if (WIDTH > 0.3)
    return_value -= 0.3;
  if (HEIGHT > 0.36)
    return_value -= 0.3;
  if (TOP > 0.3)
    return_value -= 0.3;

  return return_value;
}

double
ho_recognize_font_1_comma (const double *array_in)
{
  double return_value = 0.0;

  if (TOP < 0.3)
    return_value += 0.1;
  if (BOTTOM < 0.53 && BOTTOM > 0.47)
    return_value += 0.2;
  if (WIDTH < 0.3)
    return_value += 0.1;
  if (HEIGHT < 0.36)
    return_value += 0.2;
  if (BOTTOM_LEFT_EDGE)
    return_value += 0.2;

  if (WIDTH > 0.3)
    return_value -= 0.3;
  if (HEIGHT > 0.36)
    return_value -= 0.3;
  if (TOP > 0.3)
    return_value -= 0.3;
  if (BOTTOM > 0.55)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_minus (const double *array_in)
{
  double return_value = 0.0;

  if (HEIGHT < 0.15)
    return_value += 0.2;
  if (TOP < 0.3)
    return_value += 0.2;
  if (BOTTOM > 0.7)
    return_value += 0.2;
  if (WIDTH_BY_HEIGHT == 1.0 && HEIGHT < 0.1)
    return_value += 0.2;

  if (HEIGHT > 0.2)
    return_value -= 0.3;
  if (WIDTH < 0.15)
    return_value -= 0.3;
  if (WIDTH_BY_HEIGHT < 0.1)
    return_value -= 0.3;

  return return_value;
}

double
ho_recognize_font_1_plus (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_1_div (const double *array_in)
{
  double return_value = 0.0;

  if (DIAGONAL_LEFT_BAR)
    return_value += 0.1;
  if (TOP_RIGHT < 0.32)
    return_value += 0.1;
  if (TOP_LEFT > 0.8)
    return_value += 0.1;
  if (BOTTOM_RIGHT > 0.9)
    return_value += 0.1;
  if (BOTTOM_LEFT < 0.11)
    return_value += 0.1;

  if (DIAGONAL_BAR)
    return_value -= 0.2;
  if (TOP_RIGHT > 0.4)
    return_value -= 0.2;
  if (TOP_LEFT < 0.7)
    return_value -= 0.2;
  if (BOTTOM_RIGHT < 0.8)
    return_value -= 0.2;
  if (BOTTOM_LEFT > 0.21)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_open (const double *array_in)
{
  double return_value = 0.0;

  if (TOP_RIGHT < 0.25)
    return_value += 0.1;
  if (TOP_LEFT > 0.75)
    return_value += 0.1;
  if (BOTTOM_RIGHT < 0.25)
    return_value += 0.1;
  if (BOTTOM_LEFT > 0.75)
    return_value += 0.1;
  if (MID_RIGHT > 0.75)
    return_value += 0.1;
  if (MID_LEFT < 0.25)
    return_value += 0.1;

  if (WIDTH > 0.3)
    return_value -= 0.2;
  if (WIDTH_BY_HEIGHT > 0.35)
    return_value -= 0.2;
  if (RIGHT_VBAR)
    return_value -= 0.2;
  if (TWO_VLINES_UP)
    return_value -= 0.2;
  if (TWO_VLINES_DOWN)
    return_value -= 0.2;
  if (LEFT_TOP_EDGE)
    return_value -= 0.2;
  if (LEFT_BOTTOM_EDGE)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_close (const double *array_in)
{
  double return_value = 0.0;

  if (TOP_RIGHT > 0.75)
    return_value += 0.1;
  if (TOP_LEFT < 0.25)
    return_value += 0.1;
  if (BOTTOM_RIGHT > 0.75)
    return_value += 0.1;
  if (BOTTOM_LEFT < 0.25)
    return_value += 0.1;
  if (MID_RIGHT < 0.25)
    return_value += 0.1;
  if (MID_LEFT > 0.75)
    return_value += 0.1;

  if (WIDTH > 0.3)
    return_value -= 0.2;
  if (WIDTH_BY_HEIGHT > 0.35)
    return_value -= 0.2;
  if (LEFT_VBAR)
    return_value -= 0.2;
  if (TWO_VLINES_UP)
    return_value -= 0.2;
  if (TWO_VLINES_DOWN)
    return_value -= 0.2;
  if (RIGHT_TOP_EDGE)
    return_value -= 0.2;
  if (RIGHT_BOTTOM_EDGE)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_tag (const double *array_in)
{
  double return_value = 0.0;

  if (TOP < 0.53 && TOP > 0.47)
    return_value += 0.1;
  if (BOTTOM > 0.60)
    return_value += 0.15;
  if (WIDTH < 0.3)
    return_value += 0.1;
  if (HEIGHT < 0.37)
    return_value += 0.15;
  if (TOP_LEFT > 0.8)
    return_value += 0.2;
  if (RIGHT_TOP_EDGE)
    return_value += 0.1;

  if (TOP_LEFT_EDGE)
    return_value -= 0.2;
  if (BOTTOM < 0.60)
    return_value -= 0.2;
  if (TOP < 0.3)
    return_value -= 0.2;
  if (WIDTH > 0.23)
    return_value -= 0.3;

  return return_value;
}

double
ho_recognize_font_1_two_tags (const double *array_in)
{
  int count_edges_top;
  int count_edges_bottom;
  double return_value = 0.0;

  count_edges_top = RIGHT_TOP_EDGE * 10.0 + MID_TOP_EDGE * 10.0 +
    LEFT_TOP_EDGE * 10;
  count_edges_bottom = RIGHT_BOTTOM_EDGE * 10.0 + MID_BOTTOM_EDGE * 10.0 +
    LEFT_BOTTOM_EDGE * 10;

  if (TOP < 0.53 && TOP > 0.45)
    return_value += 0.1;
  if (BOTTOM > 0.60)
    return_value += 0.15;
  if (WIDTH < 0.8 && WIDTH > 0.15)
    return_value += 0.1;
  if (HEIGHT < 0.37)
    return_value += 0.15;
  if (TOP_LEFT > 0.8)
    return_value += 0.2;
  if (RIGHT_TOP_EDGE)
    return_value += 0.1;
  if (count_edges_top == 2)
    return_value += 0.1;
  if (count_edges_bottom == 2)
    return_value += 0.1;

  if (TOP_LEFT_EDGE)
    return_value -= 0.2;
  if (BOTTOM < 0.60)
    return_value -= 0.2;
  if (TOP < 0.3)
    return_value -= 0.2;
  if (WIDTH > 0.23)
    return_value -= 0.3;
  if (count_edges_top != 2)
    return_value -= 0.1;
  if (count_edges_bottom != 2)
    return_value -= 0.1;

  return return_value;
}

double
ho_recognize_font_1_question (const double *array_in)
{
  double return_value = 0.0;

  if (MID_LEFT_END || TOP_LEFT_END)
    return_value += 0.1;
  if (WIDTH < 0.2)
    return_value += 0.1;
  if (DOT_PART)
    return_value += 0.1;
  if (TOP_RIGTH_EDGE || MID_RIGTH_EDGE)
    return_value += 0.1;

  if (MID_RIGHT_END || TOP_MID_END)
    return_value -= 0.2;
  if (!DOT_PART)
    return_value -= 0.1;
  if (BOTTOM < 0.45 || BOTTOM > 0.55)
    return_value -= 0.2;
  if (TOP < 0.45 || TOP > 0.55)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_exclem (const double *array_in)
{
  double return_value = 0.0;

  if (WIDTH_BY_HEIGHT < 0.4)
    return_value += 0.1;
  if (WIDTH < 0.2)
    return_value += 0.1;
  if (DOT_PART)
    return_value += 0.1;

  if (WIDTH_BY_HEIGHT > 0.4)
    return_value -= 0.1;
  if (WIDTH > 0.25)
    return_value -= 0.1;
  if (!DOT_PART)
    return_value -= 0.1;
  if (BOTTOM < 0.45 || BOTTOM > 0.6)
    return_value -= 0.2;
  if (TOP < 0.45 || TOP > 0.6)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_dot_dot (const double *array_in)
{
  double return_value = 0.0;

  if (TOP_HBAR)
    return_value += 0.1;
  if (BOTTOM_HBAR)
    return_value += 0.1;
  if (MID_RIGHT > 0.9 && MID_LEFT > 0.9)
    return_value += 0.1;
  if (WIDTH_BY_HEIGHT < 0.6)
    return_value += 0.1;

  if (!TOP_HBAR)
    return_value -= 0.2;
  if (!BOTTOM_HBAR)
    return_value -= 0.2;
  if (!(MID_RIGHT > 0.9) || !(MID_LEFT > 0.9))
    return_value -= 0.2;
  if (WIDTH_BY_HEIGHT > 0.6)
    return_value -= 0.1;

  return return_value;
}

double
ho_recognize_font_1_dot_comma (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_1_alef (const double *array_in)
{
  double return_value = 0.0;

  if (DIAGONAL_BAR > 0.5)
    return_value += 0.1;
  if (LEFT_BOTTOM_EDGE)
    return_value += 0.1;
  if (LEFT_BOTTOM_NOTCH || MID_BOTTOM_NOTCH)
    return_value += 0.1;
  if (MID_MID_CROSS)
    return_value += 0.1;
  if (TOP_LEFT_NOTCH || MID_LEFT_NOTCH)
    return_value += 0.1;
  if (MID_RIGHT_NOTCH)
    return_value += 0.1;
  if (BOTTOM_LEFT_END && BOTTOM_RIGHT_END)
    return_value += 0.1;

  if (DIAGONAL_BAR < 0.5)
    return_value -= 0.1;
  if (WIDTH < 0.45)
    return_value -= 0.1;
  if (WIDTH < 0.40 || HEIGHT < 0.40)
    return_value -= 0.2;
  if (HEIGHT > 0.55)
    return_value -= 0.2;
  if (RIGHT_VBAR)
    return_value -= 0.1;
  if (BOTTOM_HBAR)
    return_value -= 0.1;
  if (BOTTOM_MID == 1.0)
    return_value -= 0.2;
  if (MID_RIGTH_EDGE)
    return_value -= 0.1;

  return return_value;
}

double
ho_recognize_font_1_bet (const double *array_in)
{
  double return_value = 0.0;

  if (TOP_HBAR)
    return_value += 0.1;
  if (BOTTOM_HBAR)
    return_value += 0.1;
  if (RIGHT_VBAR)
    return_value += 0.1;
  if (MID_LEFT_NOTCH)
    return_value += 0.1;
  if (MID_LEFT > 0.9)
    return_value += 0.1;
  if (BOTTOM_RIGTH_EDGE)
    return_value += 0.1;
  if (BOTTOM_LEFT_END && TOP_LEFT_END && !TOP_RIGHT_END)
    return_value += 0.1;

  if (!BOTTOM_RIGTH_EDGE)
    return_value -= 0.3;
  if (MID_BOTTOM_NOTCH || RIGHT_BOTTOM_NOTCH)
    return_value -= 0.3;
  if (BOTTOM_HBAR < 0.5)
    return_value -= 0.2;
  if (LEFT_VBAR)
    return_value -= 0.2;
  if (WIDTH < 0.3)
    return_value -= 0.3;
  if (TOP_LEFT > (BOTTOM_LEFT + 0.25))
    return_value -= 0.3;
  if (MID_LEFT < 0.3)
    return_value -= 0.1;
  if (MID_BOTTOM_NOTCH || RIGHT_BOTTOM_NOTCH)
    return_value -= 0.2;
  if (BOTTOM > 0.57 || BOTTOM < 43)
    return_value -= 0.20;
  if (WIDTH_BY_HEIGHT < 0.50)
    return_value -= 0.3;
  if (BOTTOM_RIGHT > 0.33)
    return_value -= 0.3;

  return return_value;
}

double
ho_recognize_font_1_gimal (const double *array_in)
{
  double return_value = 0.0;

  if (LEFT_BOTTOM_EDGE)
    return_value += 0.1;
  if (RIGHT_BOTTOM_EDGE)
    return_value += 0.1;
  if (MID_BOTTOM_NOTCH || RIGHT_BOTTOM_NOTCH)
    return_value += 0.1;
  if (TOP_LEFT > BOTTOM_LEFT)
    return_value += 0.1;
  if (TOP_RIGHT > BOTTOM_RIGHT)
    return_value += 0.1;

  if (BOTTOM_MID > 0.5)
    return_value -= 0.3;
  if (BOTTOM > 0.57 || BOTTOM < 43)
    return_value -= 0.10;
  if (BOTTOM < 0.43)
    return_value -= 0.3;
  if (!MID_BOTTOM_NOTCH && !RIGHT_BOTTOM_NOTCH)
    return_value -= 0.2;
  if (LEFT_VBAR)
    return_value -= 0.3;
  if (WIDTH > 0.4 && MID_LEFT < 0.4)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_dalet (const double *array_in)
{
  double return_value = 0.0;

  if (TOP_HBAR)
    return_value += 0.1;
  if (RIGHT_VBAR || MID_VBAR)
    return_value += 0.1;
  if (MID_VBAR)
    return_value += 0.1;
  if (TOP_RIGTH_EDGE)
    return_value += 0.1;
  if (RIGHT_BOTTOM_EDGE)
    return_value += 0.1;
  if (MID_LEFT > 0.9)
    return_value += 0.1;

  if (BOTTOM_HBAR)
    return_value -= 0.3;
  if (LEFT_VBAR)
    return_value -= 0.3;
  if (MID_VBAR)
    return_value -= 0.1;
  if (HEY_PART)
    return_value -= 0.3;
  if (BOTTOM < 0.43)
    return_value -= 0.3;
  if (!TOP_RIGTH_EDGE)
    return_value -= 0.2;
  if (BOTTOM > 0.57 || BOTTOM < 43)
    return_value -= 0.10;
  if (WIDTH_BY_HEIGHT < 0.60)
    return_value -= 0.3;
  if (MID_BOTTOM_NOTCH || RIGHT_BOTTOM_NOTCH)
    return_value -= 0.3;
  if (HEIGHT < 0.35)
    return_value -= 0.3;
  if (BOTTOM_LEFT < 0.9)
    return_value -= 0.3;
  if (WIDTH < 0.3)
    return_value -= 0.3;
  if (TOP_RIGHT > BOTTOM_RIGHT)
    return_value -= 0.3;

  return return_value;
}

double
ho_recognize_font_1_hey (const double *array_in)
{
  double return_value = 0.0;

  if (HEY_PART)
    return_value += 0.2;
  if (TOP_LEFT_NOTCH)
    return_value += 0.15;
  if (TOP_HBAR)
    return_value += 0.1;
  if (RIGHT_VBAR)
    return_value += 0.1;

  if (BOTTOM < 0.48)
    return_value -= 0.3;
  if (BOTTOM > 0.57 || BOTTOM < 43)
    return_value -= 0.10;
  if (BOTTOM_HBAR)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_vav (const double *array_in)
{
  double return_value = 0.0;

  if (WIDTH < 0.3)
    return_value += 0.2;
  if (WIDTH < 0.25 || HEIGHT > 0.40)
    return_value += 0.2;
  if (WIDTH_BY_HEIGHT < 0.53)
    return_value += 0.1;
  if (RIGHT_VBAR || MID_VBAR)
    return_value += 0.1;
  if (BOTTOM < 0.55)
    return_value += 0.1;
  if (TOP_RIGHT > (MID_RIGHT + 0.1))
    return_value += 0.1;
  if (!TOP_MID_CROSS && !TOP_RIGHT_CROSS)
    return_value += 0.1;

  if (TOP_RIGTH_EDGE)
    return_value -= 0.1;
  if (LEFT_BOTTOM_EDGE)
    return_value -= 0.1;
  if (BOTTOM_RIGTH_EDGE)
    return_value -= 0.1;
  if (MID_BOTTOM_NOTCH || RIGHT_BOTTOM_NOTCH)
    return_value -= 0.2;
  if (BOTTOM_HBAR)
    return_value -= 0.2;
  if (BOTTOM_HBAR && WIDTH_BY_HEIGHT > 0.35)
    return_value -= 0.2;
  if (BOTTOM > 0.57 || BOTTOM < 47)
    return_value -= 0.1;
  if (BOTTOM < 0.43)
    return_value -= 0.3;
  if (TOP < 0.35)
    return_value -= 0.3;
  if (WIDTH > 0.25 && (MID_LEFT > (BOTTOM_LEFT + 0.2)))
    return_value -= 0.2;
  if (WIDTH_BY_HEIGHT > 0.53)
    return_value -= 0.1;
  if (WIDTH_BY_HEIGHT > 0.60)
    return_value -= 0.2;
  if (DOT_PART)
    return_value -= 0.2;
  if (!BOTTOM_MID_END && !BOTTOM_RIGHT_END)
    return_value -= 0.2;
  if (HEIGHT < 0.5)
    return_value -= 0.2;
  if (WIDTH_BY_HEIGHT > 0.45)
    return_value -= 0.2;
  if (ONE_HOLE)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_zayin (const double *array_in)
{
  double return_value = 0.0;

  if (WIDTH < 0.3)
    return_value += 0.2;
  if (WIDTH_BY_HEIGHT < 0.3)
    return_value += 0.1;
  if (RIGHT_VBAR)
    return_value += 0.1;
  if (MID_VBAR)
    return_value += 0.1;
  if (TOP_RIGTH_EDGE)
    return_value += 0.2;

  if (BOTTOM > 0.60)
    return_value -= 0.2;
  if (BOTTOM > 0.57 || BOTTOM < 47)
    return_value -= 0.10;
  if (BOTTOM < 0.43)
    return_value -= 0.3;

  if (LEFT_BOTTOM_EDGE)
    return_value -= 0.1;
  if (MID_BOTTOM_NOTCH || RIGHT_BOTTOM_NOTCH)
    return_value -= 0.2;
  if (BOTTOM_HBAR)
    return_value -= 0.2;
  if (MID_LEFT > 0.9)
    return_value -= 0.2;
  if (TOP_RIGHT > MID_RIGHT)
    return_value -= 0.2;
  if (DOT_PART)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_het (const double *array_in)
{
  double return_value = 0.0;

  if (TOP_HBAR)
    return_value += 0.1;
  if (LEFT_VBAR)
    return_value += 0.1;
  if (RIGHT_VBAR)
    return_value += 0.1;
  if (!BOTTOM_HBAR)
    return_value += 0.1;
  if (BOTTOM_LEFT < (MID_LEFT + 0.1))
    return_value += 0.1;
  if (TWO_VLINES_DOWN)
    return_value += 0.1;

  if (!TOP_HBAR)
    return_value -= 0.1;
  if (!LEFT_VBAR)
    return_value -= 0.1;
  if (!RIGHT_VBAR)
    return_value -= 0.1;
  if (BOTTOM_HBAR)
    return_value -= 0.2;
  if (BOTTOM_LEFT_EDGE)
    return_value -= 0.2;
  if (BOTTOM_MID < 0.9)
    return_value -= 0.2;
  if (MID_TOP_NOTCH && TOP_MID > 0.2)
    return_value -= 0.3;
  if (BOTTOM_LEFT < (MID_LEFT + 0.1))
    return_value -= 0.1;
  if (!TWO_VLINES_DOWN)
    return_value -= 0.3;
  if (RIGHT_TOP_EDGE)
    return_value -= 0.2;
  if (HEY_PART)
    return_value -= 0.2;
  if (ONE_HOLE)
    return_value -= 0.3;
  if (TOP_LEFT > (BOTTOM_LEFT + 0.1))
    return_value -= 0.1;
  if (!BOTTOM_RIGHT_END)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_tet (const double *array_in)
{
  int count_edges;
  int count_notches;
  double return_value = 0.0;

  count_edges = RIGHT_TOP_EDGE * 10.0 + MID_TOP_EDGE * 10.0 +
    LEFT_TOP_EDGE * 10;
  count_notches = RIGHT_TOP_NOTCH * 10.0 + MID_TOP_NOTCH * 10.0 +
    LEFT_TOP_NOTCH * 10;

  if (BOTTOM_HBAR)
    return_value += 0.1;
  if (LEFT_VBAR)
    return_value += 0.1;
  if (RIGHT_VBAR)
    return_value += 0.1;
  if (MID_TOP_NOTCH || LEFT_TOP_NOTCH)
    return_value += 0.2;
  if (MID_BOTTOM_EDGE)
    return_value += 0.1;
  if (!TOP_RIGHT_END)
    return_value += 0.1;
  if (count_edges == 2)
    return_value += 0.1;

  if (TOP_HBAR)
    return_value -= 0.1;
  if (BOTTOM_LEFT_EDGE)
    return_value -= 0.2;
  if (!MID_TOP_NOTCH)
    return_value -= 0.2;
  if (MID_BOTTOM_NOTCH)
    return_value -= 0.2;
  if (RIGHT_TOP_NOTCH && LEFT_TOP_NOTCH)
    return_value -= 0.2;
  if (!MID_TOP_NOTCH && !LEFT_TOP_NOTCH)
    return_value -= 0.2;
  if (LEFT_BOTTOM_NOTCH)
    return_value -= 0.2;
  if (MID_BOTTOM_NOTCH)
    return_value -= 0.2;
  if (count_edges != 2)
    return_value -= 0.1;
  if (count_edges == 3)
    return_value -= 0.1;
  if (count_notches > 1)
    return_value -= 0.2;
  if (HEIGHT > 0.55)
    return_value -= 0.2;
  if (BOTTOM_LEFT_END)
    return_value -= 0.2;
  if (TWO_HOLES)
    return_value -= 0.2;
  if (ONE_HOLE && !HOLE_RIGHT_TOP_EDGE)
    return_value -= 0.2;
  if (BOTTOM_LEFT_CROSS || BOTTOM_MID_CROSS)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_yud (const double *array_in)
{
  double return_value = 0.0;

  if (TOP < 0.53 && TOP > 0.47)
    return_value += 0.1;
  if (BOTTOM > 0.55)
    return_value += 0.1;
  if (WIDTH < 0.3)
    return_value += 0.1;
  if (HEIGHT < 0.35)
    return_value += 0.1;
  if (TOP_LEFT < 0.4)
    return_value += 0.1;
  if (TOP_HBAR)
    return_value += 0.1;
  if (MID_LEFT_NOTCH)
    return_value += 0.1;

  if (HEIGHT > 0.38)
    return_value -= 0.2;
  if (TOP_LEFT > 0.8)
    return_value -= 0.1;
  if (BOTTOM < 0.58)
    return_value -= 0.2;
  if (TOP_LEFT > BOTTOM_LEFT * 1.5)
    return_value -= 0.3;
  if (LEFT_BOTTOM_EDGE)
    return_value -= 0.1;
  if (TOP < 0.3)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_caf (const double *array_in)
{
  double return_value = 0.0;

  if (TOP_HBAR)
    return_value += 0.1;
  if (BOTTOM_HBAR)
    return_value += 0.1;
  if (RIGHT_VBAR)
    return_value += 0.1;
  if (MID_LEFT_NOTCH)
    return_value += 0.1;
  if (!BOTTOM_RIGTH_EDGE)
    return_value += 0.1;
  if (BOTTOM_HBAR && BOTTOM_LEFT_END && !BOTTOM_RIGHT_END)
    return_value += 0.1;

  if (BOTTOM_RIGTH_EDGE)
    return_value -= 0.3;
  if (MID_BOTTOM_NOTCH || RIGHT_BOTTOM_NOTCH)
    return_value -= 0.3;
  if (BOTTOM_HBAR < 0.5)
    return_value -= 0.2;
  if (LEFT_VBAR)
    return_value -= 0.2;
  if (TOP_LEFT > (BOTTOM_LEFT + 0.25))
    return_value -= 0.2;
  if (WIDTH_BY_HEIGHT < 0.50)
    return_value -= 0.3;
  if (MID_LEFT < 0.8)
    return_value -= 0.2;
  if (!BOTTOM_LEFT_END)
    return_value -= 0.2;
  if (WIDTH < 0.3)
    return_value -= 0.3;
  if (TOP_RIGHT_END)
    return_value -= 0.2;
  if (MID_TOP_NOTCH || RIGHT_TOP_NOTCH)
    return_value -= 0.2;
  if (BOTTOM > 0.57 || BOTTOM < 43)
    return_value -= 0.20;

  return return_value;
}

double
ho_recognize_font_1_caf_sofit (const double *array_in)
{
  double return_value = 0.0;

  if (TOP_HBAR)
    return_value += 0.1;
  if (RIGHT_VBAR)
    return_value += 0.1;
  if (RIGHT_BOTTOM_EDGE)
    return_value += 0.1;
  if (MID_LEFT > 0.9)
    return_value += 0.1;
  if (BOTTOM < 0.45)
    return_value += 0.3;

  if (BOTTOM_HBAR)
    return_value -= 0.3;
  if (LEFT_VBAR)
    return_value -= 0.3;
  if (MID_VBAR)
    return_value -= 0.1;
  if (HEY_PART)
    return_value -= 0.3;
  if (BOTTOM > 0.50)
    return_value -= 0.3;
  if (WIDTH < 0.3)
    return_value -= 0.3;
  if (HEIGHT < 0.52)
    return_value -= 0.3;
  if (ONE_HOLE)
    return_value -= 0.2;
  if (MID_LEFT < 0.9)
    return_value -= 0.2;
  if (BOTTOM_LEFT < 0.8)
    return_value -= 0.2;
  if (TOP_MID > 0.4)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_lamed (const double *array_in)
{
  double return_value = 0.0;

  if (TOP > 0.6)
    return_value += 0.2;
  if (TOP_HBAR || MID_HBAR)
    return_value += 0.1;
  if (HEIGHT > 0.6)
    return_value += 0.2;
  if (TOP_RIGHT > 0.5)
    return_value += 0.2;
  if (TOP_LEFT < 0.2)
    return_value += 0.1;

  if (LEFT_VBAR)
    return_value -= 0.2;
  if (TOP < 0.51)
    return_value -= 0.2;
  if (!TOP_HBAR && !MID_HBAR)
    return_value -= 0.2;
  if (BOTTOM > 0.52)
    return_value -= 0.2;
  if (WIDTH_BY_HEIGHT < 0.35)
    return_value -= 0.2;
  if (WIDTH < 0.25)
    return_value -= 0.2;
  if (BOTTOM < 0.47)
    return_value -= 0.2;
  if (DOT_PART)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_mem (const double *array_in)
{
  double return_value = 0.0;

  if (TOP_LEFT_EDGE)
    return_value += 0.1;
  if (LEFT_TOP_EDGE)
    return_value += 0.1;
  if (LEFT_BOTTOM_NOTCH || MID_BOTTOM_NOTCH)
    return_value += 0.1;
  if (MID_LEFT_NOTCH)
    return_value += 0.1;
  if (TOP_LEFT_END)
    return_value += 0.1;
  if ((MID_LEFT_END || BOTTOM_LEFT_END) && BOTTOM_MID_END)
    return_value += 0.1;

  if (DIAGONAL_BAR)
    return_value -= 0.1;
  if (TOP_HBAR)
    return_value -= 0.1;
  if (MID_RIGHT_NOTCH || BOTTOM_RIGHT_NOTCH)
    return_value -= 0.2;
  if (BOTTOM_RIGHT_END)
    return_value -= 0.1;
  if (BOTTOM > 0.60)
    return_value -= 0.2;
  if (BOTTOM_RIGHT > 0.35)
    return_value -= 0.1;
  if (MID_LEFT > 0.9)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_mem_sofit (const double *array_in)
{
  double return_value = 0.0;

  if (TOP_HBAR)
    return_value += 0.1;
  if (BOTTOM_HBAR)
    return_value += 0.1;
  if (LEFT_VBAR)
    return_value += 0.1;
  if (RIGHT_VBAR)
    return_value += 0.1;
  if (BOTTOM_LEFT < 0.25 && BOTTOM_RIGHT < 0.25 && BOTTOM_HBAR)
    return_value += 0.1;
  if ((BOTTOM_LEFT) > (BOTTOM_RIGHT + 0.2))
    return_value += 0.1;
  if (ONE_HOLE)
    return_value += 0.2;

  if ((BOTTOM_LEFT + 0.1) < (BOTTOM_RIGHT))
    return_value -= 0.3;
  if (!ONE_HOLE)
    return_value -= 0.2;
  if (BOTTOM_LEFT > 0.45 || BOTTOM_RIGHT > 0.45)
    return_value -= 0.2;
  if (BOTTOM > 0.57 || BOTTOM < 43)
    return_value -= 0.10;
  if (MID_TOP_NOTCH)
    return_value -= 0.2;
  if (!TOP_HBAR)
    return_value -= 0.3;
  if (TOP_LEFT > 0.4)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_nun (const double *array_in)
{
  double return_value = 0.0;

  if (TOP_HBAR)
    return_value += 0.1;
  if (BOTTOM_HBAR)
    return_value += 0.1;
  if (RIGHT_VBAR)
    return_value += 0.1;
  if (MID_LEFT_NOTCH)
    return_value += 0.1;
  if (!BOTTOM_RIGTH_EDGE)
    return_value += 0.1;
  if (TOP_LEFT > (BOTTOM_LEFT + 0.25))
    return_value += 0.2;
  if (WIDTH < 0.3)
    return_value += 0.1;
  if (WIDTH < 0.3 && BOTTOM_HBAR)
    return_value += 0.2;

  if (BOTTOM_RIGTH_EDGE && TOP_HBAR)
    return_value -= 0.1;
  if (MID_BOTTOM_NOTCH || RIGHT_BOTTOM_NOTCH)
    return_value -= 0.3;
  if (BOTTOM_HBAR < 0.5)
    return_value -= 0.2;
  if (LEFT_VBAR)
    return_value -= 0.2;
  if (MID_LEFT_EDGE)
    return_value -= 0.2;
  if (WIDTH > 0.3 && TOP_LEFT < (BOTTOM_LEFT + 0.25))
    return_value -= 0.35;
  if (TOP_LEFT < (BOTTOM_LEFT))
    return_value -= 0.2;
  if (TOP_LEFT < (BOTTOM_LEFT + 0.25))
    return_value -= 0.2;
  if (MID_LEFT < 0.6)
    return_value -= 0.3;
  if (BOTTOM > 0.60)
    return_value -= 0.2;
  if (BOTTOM < 0.43)
    return_value -= 0.2;
  if (MID_LEFT_END || MID_MID_END)
    return_value -= 0.3;
  if (HEIGHT > 0.55)
    return_value -= 0.2;
  if (MID_RIGHT > 0.9)
    return_value -= 0.3;

  return return_value;
}

double
ho_recognize_font_1_nun_sofit (const double *array_in)
{
  double return_value = 0.0;

  if (WIDTH < 0.3)
    return_value += 0.2;
  if (WIDTH_BY_HEIGHT < 0.3)
    return_value += 0.2;
  if (RIGHT_VBAR)
    return_value += 0.1;
  if (BOTTOM < 0.43)
    return_value += 0.3;
  if (HEIGHT > 0.53)
    return_value += 0.1;
  if (HEIGHT > 0.63)
    return_value += 0.1;
  if (BOTTOM_RIGTH_EDGE)
    return_value += 0.1;

  if (TOP_RIGTH_EDGE)
    return_value -= 0.1;
  if (LEFT_BOTTOM_EDGE)
    return_value -= 0.1;
  if (MID_BOTTOM_NOTCH || RIGHT_BOTTOM_NOTCH)
    return_value -= 0.2;
  if (BOTTOM_HBAR)
    return_value -= 0.2;
  if (HEIGHT < 0.5)
    return_value -= 0.3;
  if (BOTTOM > 0.0)
    return_value -= 0.2;
  if (WIDTH_BY_HEIGHT > 0.35)
    return_value -= 0.3;
  if (BOTTOM_LEFT < 0.15)
    return_value -= 0.1;
  if (MID_RIGHT > 0.75)
    return_value += 0.1;
  if (TOP_LEFT > 0.75)
    return_value -= 0.1;
  if ((BOTTOM_LEFT + 0.2) < BOTTOM_RIGHT)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_samech (const double *array_in)
{
  double return_value = 0.0;

  if (TOP_HBAR)
    return_value += 0.1;
  if (BOTTOM_HBAR)
    return_value += 0.1;
  if (LEFT_VBAR)
    return_value += 0.1;
  if (RIGHT_VBAR)
    return_value += 0.1;
  if (BOTTOM_LEFT > 0.45 || BOTTOM_RIGHT > 0.45)
    return_value += 0.1;
  if ((BOTTOM_LEFT + 0.2) < (BOTTOM_RIGHT))
    return_value += 0.1;
  if (ONE_HOLE)
    return_value += 0.2;

  if ((BOTTOM_LEFT) > (BOTTOM_RIGHT))
    return_value -= 0.3;
  if (BOTTOM_LEFT < 0.2 && BOTTOM_RIGHT < 0.2)
    return_value -= 0.3;
  if (BOTTOM_LEFT < 0.12)
    return_value -= 0.3;
  if (!ONE_HOLE)
    return_value -= 0.3;
  if (ONE_HOLE && HOLE_WIDTH < 0.17)
    return_value -= 0.3;
  if (TWO_HOLES)
    return_value -= 0.3;
  if (LEFT_BOTTOM_EDGE && RIGHT_BOTTOM_EDGE)
    return_value -= 0.2;
  if (MID_TOP_NOTCH)
    return_value -= 0.2;
  if (WIDTH_BY_HEIGHT < 0.60)
    return_value -= 0.3;
  if (!TOP_HBAR)
    return_value -= 0.3;
  if (TOP_LEFT > 0.4)
    return_value -= 0.2;
  if (MID_LEFT_NOTCH)
    return_value -= 0.1;
  if (BOTTOM_LEFT_NOTCH)
    return_value -= 0.1;
  if (BOTTOM_MID_CROSS)
    return_value -= 0.2;
  if (BOTTOM_LEFT_END)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_ayin (const double *array_in)
{
  double return_value = 0.0;

  if (TWO_VLINES_UP)
    return_value += 0.1;
  if (BOTTOM_LEFT_EDGE)
    return_value += 0.1;
  if (BOTTOM_LEFT < BOTTOM_RIGHT)
    return_value += 0.1;
  if (MID_TOP_NOTCH)
    return_value += 0.1;
  if (MID_LEFT_NOTCH || BOTTOM_LEFT_NOTCH)
    return_value += 0.1;
  if (MID_TOP_NOTCH)
    return_value += 0.1;
  if ((TOP_RIGHT + 0.15) < BOTTOM_RIGHT)
    return_value += 0.1;
  if (BOTTOM_LEFT_END)
    return_value += 0.1;

  if (MID_BOTTOM_NOTCH)
    return_value -= 0.2;
  if (MID_RIGHT_NOTCH || BOTTOM_RIGHT_NOTCH)
    return_value -= 0.2;
  if (TOP_HBAR)
    return_value -= 0.2;
  if (LEFT_BOTTOM_NOTCH)
    return_value -= 0.2;
  if (!BOTTOM_LEFT_END)
    return_value -= 0.2;
  if (MID_RIGHT_NOTCH)
    return_value -= 0.1;
  if (MID_RIGHT > 0.51 && BOTTOM_RIGHT < 0.2)
    return_value -= 0.1;
  if (BOTTOM > 0.55)
    return_value -= 0.2;
  if (HEY_PART)
    return_value -= 0.2;
  if (BOTTOM_LEFT > 0.3)
    return_value -= 0.2;
  if (BOTTOM_MID > 0.85)
    return_value -= 0.1;
  if (TOP > 0.56)
    return_value -= 0.2;
  if (MID_LEFT > 0.9)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_pey (const double *array_in)
{
  double return_value = 0.0;

  if (TOP_HBAR)
    return_value += 0.15;
  if (BOTTOM_HBAR)
    return_value += 0.15;
  if (RIGHT_VBAR)
    return_value += 0.15;
  if (BOTTOM_LEFT_NOTCH)
    return_value += 0.15;
  if (MID_LEFT < 0.3)
    return_value += 0.15;
  if (MID_LEFT_END || MID_MID_END)
    return_value += 0.2;
  if (BOTTOM_LEFT_END)
    return_value += 0.1;

  if (LEFT_VBAR)
    return_value -= 0.1;
  if (BOTTOM_RIGTH_EDGE)
    return_value -= 0.3;
  if (MID_BOTTOM_NOTCH || RIGHT_BOTTOM_NOTCH)
    return_value -= 0.3;
  if (BOTTOM_HBAR < 0.5)
    return_value -= 0.2;
  if (TOP_LEFT_END && !TOP_LEFT_CROSS)
    return_value -= 0.3;
  if (WIDTH_BY_HEIGHT < 0.60)
    return_value -= 0.3;
  if (!BOTTOM_LEFT_NOTCH)
    return_value -= 0.3;
  if (MID_TOP_NOTCH)
    return_value -= 0.3;
  if ((TOP_RIGHT + 0.2) < BOTTOM_RIGHT)
    return_value -= 0.2;
  if (ONE_HOLE && HOLE_TOP_LEFT > (HOLE_BOTTOM_LEFT + 0.1))
    return_value -= 0.2;
  if (ONE_HOLE && !HOLE_TOP_LEFT_EDGE)
    return_value -= 0.2;
  if (!MID_LEFT_END && !MID_MID_END && !MID_RIGHT_CROSS)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_pey_sofit (const double *array_in)
{
  double return_value = 0.0;

  if (TOP_HBAR)
    return_value += 0.1;
  if (RIGHT_VBAR)
    return_value += 0.1;
  if (RIGHT_BOTTOM_EDGE)
    return_value += 0.1;
  if (BOTTOM < 0.47)
    return_value += 0.2;
  if ((!TOP_LEFT_END && !TOP_MID_END) || TOP_LEFT_CROSS)
    return_value += 0.3;
  if (HEIGHT > 0.51)
    return_value += 0.1;

  if (BOTTOM_HBAR)
    return_value -= 0.3;
  if (LEFT_VBAR)
    return_value -= 0.3;
  if (MID_VBAR)
    return_value -= 0.1;
  if (HEY_PART)
    return_value -= 0.3;
  if (BOTTOM > 0.50)
    return_value -= 0.3;
  if (WIDTH < 0.3)
    return_value -= 0.3;
  if (TOP_LEFT_END && !TOP_LEFT_CROSS)
    return_value -= 0.3;
  if (HEIGHT < 0.5)
    return_value -= 0.3;
  if (BOTTOM > 0.50)
    return_value -= 0.2;
  if (WIDTH_BY_HEIGHT < 0.45)
    return_value -= 0.3;
  if (BOTTOM_LEFT < 0.75)
    return_value -= 0.3;
  if (!BOTTOM_RIGHT_END)
    return_value -= 0.2;
  if (BOTTOM_MID_END)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_tzadi (const double *array_in)
{
  double return_value = 0.0;

  if (DIAGONAL_BAR)
    return_value += 0.2;
  if (TWO_VLINES_UP)
    return_value += 0.1;
  if (BOTTOM_LEFT_EDGE)
    return_value += 0.1;
  if (BOTTOM_RIGTH_EDGE)
    return_value += 0.1;
  if (BOTTOM_LEFT < BOTTOM_RIGHT)
    return_value += 0.1;
  if (MID_RIGHT_NOTCH || BOTTOM_RIGHT_NOTCH)
    return_value += 0.2;

  if (BOTTOM_LEFT < BOTTOM_RIGHT)
    return_value -= 0.1;
  if (MID_BOTTOM_NOTCH)
    return_value -= 0.2;
  if (!BOTTOM_HBAR)
    return_value -= 0.2;
  if (LEFT_BOTTOM_NOTCH)
    return_value -= 0.2;
  if (BOTTOM_RIGHT_END)
    return_value -= 0.2;
  if (!BOTTOM_LEFT_END)
    return_value -= 0.2;
  if (!MID_RIGHT_NOTCH && !BOTTOM_RIGHT_NOTCH)
    return_value -= 0.2;
  if (!MID_MID_CROSS && !BOTTOM_RIGHT_CROSS)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_tzadi_sofit (const double *array_in)
{
  double return_value = 0.0;

  if (RIGHT_VBAR || LEFT_VBAR)
    return_value += 0.1;
  if (BOTTOM < 0.47)
    return_value += 0.3;
  if (LEFT_TOP_EDGE)
    return_value += 0.1;
  if (RIGHT_TOP_EDGE)
    return_value += 0.1;
  if (TOP_MID_CROSS || MID_MID_CROSS)
    return_value += 0.1;

  if (BOTTOM_HBAR)
    return_value -= 0.3;
  if (HEY_PART)
    return_value -= 0.3;
  if (BOTTOM > 0.51)
    return_value -= 0.3;
  if (WIDTH < 0.3)
    return_value -= 0.3;
  if (BOTTOM_LEFT_EDGE)
    return_value -= 0.3;
  if (TWO_VLINES_DOWN)
    return_value -= 0.3;
  if (HEIGHT < 0.5)
    return_value -= 0.3;
  if (TOP_HBAR)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_kuf (const double *array_in)
{
  double return_value = 0.0;

  if (HEY_PART)
    return_value += 0.2;
  if (TOP_LEFT_NOTCH)
    return_value += 0.1;
  if (TOP_HBAR)
    return_value += 0.1;
  if (BOTTOM < 0.47)
    return_value += 0.3;
  if (BOTTOM_LEFT_END)
    return_value += 0.1;

  if (BOTTOM_LEFT > (BOTTOM_RIGHT - 0.2))
    return_value -= 0.2;
  if (HEIGHT < 0.53)
    return_value -= 0.2;
  if (BOTTOM > 0.45)
    return_value -= 0.3;
  if (WIDTH_BY_HEIGHT < 0.60)
    return_value -= 0.3;
  if (!TOP_HBAR)
    return_value -= 0.1;
  if (BOTTOM_RIGTH_EDGE > 0.1)
    return_value -= 0.1;
  if (BOTTOM_HBAR)
    return_value -= 0.1;

  return return_value;
}

double
ho_recognize_font_1_resh (const double *array_in)
{
  double return_value = 0.0;

  if (TOP_HBAR)
    return_value += 0.1;
  if (RIGHT_VBAR)
    return_value += 0.1;
  if (!BOTTOM_HBAR)
    return_value += 0.1;
  if (!LEFT_VBAR)
    return_value += 0.1;
  if (!TOP_RIGTH_EDGE)
    return_value += 0.1;
  if (RIGHT_BOTTOM_EDGE)
    return_value += 0.1;

  if (BOTTOM_HBAR)
    return_value -= 0.3;
  if (MID_BOTTOM_NOTCH || RIGHT_BOTTOM_NOTCH)
    return_value -= 0.2;
  if (LEFT_VBAR)
    return_value -= 0.3;
  if (MID_VBAR)
    return_value -= 0.1;
  if (HEY_PART)
    return_value -= 0.3;
  if (BOTTOM > 0.60)
    return_value -= 0.3;
  if (BOTTOM < 0.45)
    return_value -= 0.3;
  if (TOP_RIGTH_EDGE)
    return_value -= 0.1;
  if (WIDTH_BY_HEIGHT < 0.30)
    return_value -= 0.3;
  if (TWO_VLINES_DOWN)
    return_value -= 0.3;
  if (BOTTOM_LEFT < 0.9)
    return_value -= 0.3;
  if (MID_LEFT < 0.9)
    return_value -= 0.3;
  if (WIDTH < 0.35 || HEIGHT < 0.35)
    return_value -= 0.2;
  if (WIDTH < 0.25)
    return_value -= 0.2;

  return return_value;
}

double
ho_recognize_font_1_shin (const double *array_in)
{
  int count_edges;
  int count_notches;
  double return_value = 0.0;

  count_edges = RIGHT_TOP_EDGE * 10.0 + MID_TOP_EDGE * 10.0 +
    LEFT_TOP_EDGE * 10;
  count_notches = RIGHT_TOP_NOTCH * 10.0 + MID_TOP_NOTCH * 10.0 +
    LEFT_TOP_NOTCH * 10;

  if (TWO_HOLES)
    return_value += 0.2;
  if (THREE_VLINES_UP)
    return_value += 0.3;
  if (LEFT_TOP_NOTCH)
    return_value += 0.1;
  if (RIGHT_TOP_NOTCH)
    return_value += 0.1;
  if (LEFT_TOP_EDGE)
    return_value += 0.1;
  if (RIGHT_TOP_EDGE)
    return_value += 0.1;
  if (count_edges == 3)
    return_value += 0.2;
  if (count_notches == 2)
    return_value += 0.2;
  if (BOTTOM_MID_CROSS)
    return_value += 0.2;

  if (MID_BOTTOM_NOTCH)
    return_value -= 0.2;
  if (!RIGHT_TOP_NOTCH && !MID_TOP_NOTCH)
    return_value -= 0.2;
  if (count_edges != 3)
    return_value -= 0.1;

  return return_value;
}

double
ho_recognize_font_1_tav (const double *array_in)
{
  double return_value = 0.0;

  if (TOP_HBAR)
    return_value += 0.1;
  if (LEFT_VBAR)
    return_value += 0.1;
  if (RIGHT_VBAR)
    return_value += 0.1;
  if (BIG_BOTTOM_LEFT_EDGE)
    return_value += 0.2;
  if (BOTTOM_LEFT_EDGE)
    return_value += 0.1;
  if (TOP_LEFT > (BOTTOM_LEFT + 0.15))
    return_value += 0.1;

  if (BOTTOM_HBAR)
    return_value -= 0.2;
  if (!BOTTOM_LEFT_EDGE)
    return_value -= 0.2;
  if (BOTTOM_MID < 0.9)
    return_value -= 0.2;
  if (MID_TOP_NOTCH || RIGHT_TOP_NOTCH)
    return_value -= 0.1;
  if (TOP_RIGHT_NOTCH || MID_RIGHT_NOTCH)
    return_value -= 0.1;
  if (WIDTH_BY_HEIGHT < 0.3)
    return_value -= 0.3;
  if (BOTTOM_LEFT > 0.5)
    return_value -= 0.3;
  if (BOTTOM_LEFT < (MID_LEFT + 0.1))
    return_value -= 0.2;

  return return_value;
}
