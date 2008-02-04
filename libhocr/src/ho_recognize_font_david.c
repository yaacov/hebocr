
/***************************************************************************
 *            ho_recognize_font_david.c
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

#include "ho_recognize_font_david.h"

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
#define TWO_HOLEs (array_in[68])
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

/* font recognition functions */

/* functions get an array of doubles 0..1 and return a double 0..1 
 * 0 if this does not look like the font until 1 looks exactly like the font */

double
ho_recognize_font_david_dot (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_comma (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_minus (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_tag (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_question (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_exclem (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_dot_dot (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_dot_comma (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_alef (const double *array_in)
{
  double return_value = 0.0;

  if (DIAGONAL_BAR > 0.5)
    return_value += 0.2;
  
  return return_value;
}

double
ho_recognize_font_david_bet (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_gimal (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_dalet (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_hey (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_vav (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_zayin (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_het (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_tet (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_yud (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_caf (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_caf_sofit (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_lamed (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_mem (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_mem_sofit (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_nun (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_nun_sofit (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_samech (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_ayin (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_pey (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_pey_sofit (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_tzadi (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_tzadi_sofit (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_kuf (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_resh (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_shin (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_font_david_tav (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}
