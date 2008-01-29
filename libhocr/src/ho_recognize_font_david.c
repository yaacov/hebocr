
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

double
ho_recognize_font_david_dot (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the bottom */
  if (array_in[0] < 0.30 && array_in[0] > 0.05 &&
    array_in[1] < 0.30 && array_in[1] > 0.05 &&
    array_in[3] < 0.22 && array_in[3] > 0.00
    && array_in[4] < 0.65 && array_in[4] > 0.35 &&
    array_in[5] < 2 * array_in[7])
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_comma (const double *array_in)
{
  double return_value = 0.0;

  /* small font near the bottom */
  if (array_in[0] < 0.30 && array_in[0] > 0.05 &&
    array_in[1] < 0.20 && array_in[1] > 0.10 &&
    array_in[3] < 0.22 && array_in[3] >= 0.00
    && array_in[4] < 0.65 && array_in[4] > 0.35 &&
    array_in[5] >= 2 * array_in[7])
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_minus (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.2 && array_in[0] > 0.00 &&
    array_in[1] < 1.10 && array_in[1] > 0.10 && array_in[8] < 0.8 &&
    /* bars */
    /* edges */
    /* notches */
    /* ends */
    /* crosses */
    array_in[52] < 0.5 &&
    /* holes */
    array_in[57] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_tag (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.30 && array_in[0] > 0.05 &&
    array_in[1] < 0.25 && array_in[1] > 0.05 &&
    array_in[3] < 0.58 && array_in[3] > 0.42 &&
    array_in[4] < 0.85 && array_in[4] > 0.45 && array_in[7] < 0.5 &&
    /* bars */
    /* edges */
    (array_in[25] > 0.5 || array_in[24] > 0.5) &&
    /* notches */
    array_in[31] < 0.5 && array_in[32] < 0.5 &&
    /* ends */
    (array_in[41] > 0.5 || array_in[40] > 0.5) &&
    (array_in[46] > 0.5 || array_in[45] > 0.5 || array_in[42] > 0.5
      || array_in[43] > 0.5) &&
    /* crosses */
    array_in[54] < 0.5 && array_in[55] < 0.5 &&
    /* holes */
    array_in[57] < 0.5 && array_in[58] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_question (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.55 && array_in[0] > 0.40 &&
    array_in[1] < 0.45 && array_in[1] > 0.05 &&
    array_in[3] < 0.55 && array_in[3] > 0.45 &&
    array_in[4] < 0.60 && array_in[4] > 0.45 &&
    /* bars */
    array_in[10] < 0.5 &&
    /* edges */
    (array_in[81] > 0.5 || array_in[82] > 0.5) &&
    /* notches */
    /* ends */
    array_in[46] > 0.5 && array_in[41] < 0.5 &&
    /* crosses */
    array_in[52] < 0.5 &&
    /* holes */
    array_in[57] < 0.5 && array_in[58] < 0.5 && array_in[96] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_exclem (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.55 && array_in[0] > 0.40 &&
    array_in[1] < 0.20 && array_in[1] > 0.05 &&
    array_in[3] < 0.60 && array_in[3] > 0.45 &&
    array_in[4] < 0.60 && array_in[4] > 0.45 &&
    /* bars */
    /* edges */
    array_in[82] > 0.5 &&
    /* notches */
    /* ends */
    array_in[46] > 0.5 && array_in[40] > 0.5 &&
    /* crosses */
    array_in[52] < 0.5 &&
    /* holes */
    array_in[57] < 0.5 && array_in[58] < 0.5 && array_in[59] < 0.5
    && array_in[96] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_dot_dot (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.55 && array_in[0] > 0.30 &&
    array_in[1] < 0.20 && array_in[1] > 0.05 &&
    array_in[3] < 0.50 && array_in[3] > 0.30 &&
    array_in[4] < 0.55 && array_in[4] > 0.45 &&
    /* bars */
    /* edges */
    (array_in[82] > 0.5 || array_in[81] > 0.5) &&
    /* notches */
    /* ends */
    array_in[46] > 0.5 && array_in[40] > 0.5 && array_in[43] < 0.5 &&
    /* crosses */
    array_in[52] < 0.5 &&
    /* holes */
    array_in[57] < 0.5 && array_in[58] < 0.5 && array_in[59] < 0.5
    && array_in[96] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_dot_comma (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.55 && array_in[0] > 0.40 &&
    array_in[1] < 0.20 && array_in[1] > 0.05 &&
    array_in[3] < 0.50 && array_in[3] > 0.30 &&
    array_in[4] < 0.50 && array_in[4] > 0.30 &&
    /* bars */
    /* edges */
    (array_in[82] > 0.5 || array_in[81] > 0.5) &&
    /* notches */
    /* ends */
    array_in[46] > 0.5 && array_in[40] > 0.5 &&
    /* crosses */
    array_in[52] < 0.5 &&
    /* holes */
    array_in[57] < 0.5 && array_in[58] < 0.5 && array_in[59] < 0.5
    && array_in[101] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_alef (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.60 && array_in[0] > 0.40 &&
    array_in[1] < 0.60 && array_in[1] > 0.30 &&
    array_in[3] < 0.60 && array_in[3] > 0.40 &&
    array_in[4] < 0.60 && array_in[4] > 0.40 && array_in[8] < 0.8 &&
    array_in[92] > 0.1 &&
    /* bars */
    array_in[13] > 0.5 &&
    (array_in[10] < 0.5 ||
      (array_in[45] > 0.5 && array_in[47] > 0.5) || array_in[91] > 0.5 ||
      (array_in[39] > 0.5 && array_in[41] > 0.5 && array_in[45] > 0.5 &&
        array_in[47] > 0.5 && array_in[52] > 0.5)) &&
    /* edges */
    (array_in[21] > 0.5 || array_in[22] > 0.5) &&
    (array_in[22] > 0.5 || array_in[23] > 0.5 ||
      array_in[41] > 0.5 || array_in[18] > 0.5) &&
    /* notches */
    array_in[31] > 0.5 && array_in[34] > 0.5 &&
    /* crosses */
    array_in[52] > 0.5 &&
    /* hline counter */
    (array_in[98] > 0.5 || array_in[95] > 0.5) &&
    /* holes */
    array_in[59] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_bet (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.60 && array_in[0] > 0.40 &&
    array_in[1] < 0.60 && array_in[1] > 0.30 &&
    array_in[3] < 0.55 && array_in[3] > 0.40 &&
    array_in[4] < 0.60 && array_in[4] > 0.40 && array_in[7] < 0.5 &&
    array_in[6] > array_in[8] && array_in[94] * 1.2 > array_in[8] &&
    /* bars */
    array_in[9] > 0.5 && array_in[10] > 0.5 &&
    /* edges */
    array_in[20] > 0.5 && array_in[80] > 0.5 && array_in[82] > 0.5 &&
    /* notches */
    array_in[34] < 0.5 && array_in[33] < 0.5 &&
    array_in[37] < 0.5 && array_in[38] < 0.5 &&
    /* ends */
    (array_in[39] > 0.5 || array_in[40] > 0.5) && array_in[45] > 0.5 &&
    /* crosses */
    /* holes */
    array_in[57] < 0.5 && array_in[58] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_gimal (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.60 && array_in[0] > 0.40 &&
    array_in[1] < 0.50 && array_in[1] > 0.20 &&
    array_in[3] < 0.60 && array_in[3] > 0.40 &&
    array_in[4] < 0.60 && array_in[4] > 0.40 && array_in[95] < 0.5 &&
    /* bars */
    array_in[14] < 0.5 && array_in[11] < 0.5 &&
    /* edges */
    array_in[26] > 0.5 && array_in[15] > 0.5 &&
    /* notches */
    array_in[34] < 0.5 && array_in[33] < 0.5 &&
    (array_in[37] > 0.5 || array_in[38] > 0.5 || array_in[26] > 0.5) &&
    /* ends */
    array_in[42] < 0.5 && array_in[43] < 0.5 &&
    (array_in[45] > 0.5 || array_in[46] > 0.5) &&
    (array_in[39] > 0.5 || array_in[40] > 0.5) &&
    /* crosses */
    (array_in[91] > 0.5 || (array_in[37] > 0.5) ||
      (array_in[52] > 0.5 || array_in[53] > 0.5 || array_in[55] > 0.5)) &&
    /* holes */
    array_in[57] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_dalet (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.60 && array_in[0] > 0.40 &&
    array_in[1] < 0.60 && array_in[1] > 0.30 &&
    array_in[3] < 0.55 && array_in[3] > 0.45 &&
    array_in[4] < 0.58 && array_in[4] >= 0.50 && array_in[7] > 0.45 &&
    /* bars */
    array_in[9] > 0.5 && array_in[10] < 0.5 &&
    /* edges */
    array_in[83] > 0.5 &&
    /* notches */
    /* ends */
    (array_in[46] > 0.5 || array_in[47] > 0.5) &&
    (array_in[39] > 0.5 || array_in[40]) &&
    /* crosses */
    /* holes */
    array_in[57] < 0.5 && array_in[58] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_hey (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.60 && array_in[0] > 0.40 &&
    array_in[1] < 0.60 && array_in[1] > 0.30 &&
    array_in[3] < 0.60 && array_in[3] > 0.40 &&
    array_in[4] < 0.60 && array_in[4] > 0.40 && array_in[8] < 0.5 &&
    /* bars */
    /* edges */
    array_in[15] > 0.5 && array_in[26] > 0.5 &&
    /* notches */
    /* ends */
    (array_in[39] > 0.5 || array_in[5] < 0.25) &&
    array_in[45] > 0.5 && array_in[47] > 0.5 &&
    /* crosses */
    array_in[52] < 0.5 && array_in[55] < 0.5 &&
    /* holes */
    array_in[57] < 0.5 && array_in[59] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_vav (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.55 && array_in[0] > 0.40 &&
    array_in[1] < 0.28 && array_in[1] > 0.05 &&
    array_in[3] < 0.55 && array_in[3] > 0.42 &&
    array_in[4] < 0.58 && array_in[4] >= 0.42 && array_in[7] > 0.5 &&
    /* bars */
    array_in[12] > 0.5 && (array_in[10] < 0.5 || array_in[11] < 0.5) &&
    /* edges */
    array_in[83] < 0.5 &&
    /* notches */
    /* ends */
    (array_in[47] > 0.5 || array_in[46] > 0.5) &&
    /* crosses */
    array_in[49] < 0.5 &&
    /* holes */
    array_in[57] < 0.5 && array_in[58] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_zayin (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.55 && array_in[0] > 0.40 &&
    array_in[1] < 0.43 && array_in[1] > 0.05 &&
    array_in[3] < 0.55 && array_in[3] > 0.45 &&
    array_in[4] < 0.60 && array_in[4] > 0.45 &&
    /* bars */
    array_in[10] < 0.5 &&
    /* edges */
    array_in[83] > 0.5 && array_in[82] < 0.5 &&
    /* notches */
    /* ends */
    (array_in[46] > 0.5 || array_in[47] > 0.5) &&
    (array_in[40] > 0.5 || array_in[39] > 0.5 || array_in[41] > 0.5) &&
    !(array_in[45] > 0.5 && array_in[46] < 0.5 && array_in[47] > 0.5) &&
    /* crosses */
    array_in[52] < 0.5 &&
    /* holes */
    array_in[57] < 0.5 && array_in[58] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_het (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.60 && array_in[0] > 0.40 &&
    array_in[1] < 0.60 && array_in[1] > 0.30 &&
    array_in[3] < 0.60 && array_in[3] > 0.40 &&
    array_in[4] < 0.60 && array_in[4] > 0.40 && array_in[8] < 0.5 &&
    /* bars */
    array_in[9] > 0.5 && array_in[10] < 0.5 &&
    /* edges */
    array_in[24] > 0.5 && array_in[26] > 0.5 &&
    (array_in[82] < 0.5) && array_in[93] < array_in[7] * 3.1 &&
    /* notches */
    /* ends */
    array_in[45] > 0.5 && array_in[46] < 0.5 && array_in[47] > 0.5 &&
    /* crosses */
    array_in[52] < 0.5 && array_in[55] < 0.5 &&
    /* holes */
    array_in[57] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_tet (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.60 && array_in[0] > 0.40 &&
    array_in[1] < 0.60 && array_in[1] > 0.30 &&
    array_in[3] < 0.60 && array_in[3] > 0.40 &&
    array_in[4] < 0.60 && array_in[4] > 0.40 &&
    /* bars */
    /* edges */
    ((array_in[57] > 0.5 && array_in[66] > 0.5 &&
        array_in[68] > 0.5) ||
      (array_in[45] < 0.5 && array_in[91] < 0.5 && array_in[82] < 0.5 &&
        array_in[34] > 0.5)) &&
    /* notches */
    array_in[36] < 0.5 && array_in[37] < 0.5 &&
    /* ends */
    (array_in[39] > 0.5 || (array_in[57] > 0.5 && array_in[5] < 0.30)) &&
    ((array_in[46] < 0.5 && array_in[47] < 0.5) || array_in[10] > 0.5) &&
    !(array_in[39] > 0.5 && array_in[40] > 0.5 && array_in[41] > 0.5) &&
    /* crosses */
    array_in[52] < 0.5 &&
    (array_in[55] < 0.5 || array_in[10] > 0.5) && array_in[54] < 0.5 &&
    /* holes */
    array_in[59] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_yud (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.40 && array_in[0] > 0.18 &&
    array_in[1] < 0.30 && array_in[1] > 0.12 &&
    array_in[3] < 0.58 && array_in[3] > 0.42 &&
    array_in[4] < 0.85 && array_in[4] > 0.45 && array_in[7] > 0.5 &&
    /* bars */
    array_in[9] > 0.5 && array_in[10] < 0.5 &&
    array_in[11] < 0.5 && array_in[12] > 0.5 && array_in[14] < 0.5 &&
    /* edges */
    (array_in[25] > 0.5 || array_in[26] > 0.5) &&
    /* notches */
    array_in[31] < 0.5 && array_in[32] < 0.5 &&
    /* ends */
    (array_in[46] > 0.5 || array_in[47] > 0.5) && array_in[45] < 0.5 &&
    /* crosses */
    array_in[54] < 0.5 && array_in[55] < 0.5 &&
    /* holes */
    array_in[57] < 0.5 && array_in[58] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_caf (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.60 && array_in[0] > 0.40 &&
    array_in[1] < 0.60 && array_in[1] > 0.30 &&
    array_in[3] < 0.55 && array_in[3] > 0.40 &&
    array_in[4] < 0.60 && array_in[4] > 0.40 && array_in[7] < 0.5 &&
    /* bars */
    array_in[9] > 0.5 && array_in[10] > 0.5 &&
    /* edges */
    (array_in[20] < 0.5 || array_in[94] < array_in[8] * 1.1) &&
    array_in[80] > 0.5 && array_in[82] > 0.5 &&
    /* notches */
    /* ends */
    (array_in[39] > 0.5 ||
      (array_in[40] && array_in[5] < 0.25)) &&
    (array_in[45] > 0.5 || array_in[46] > 0.5) &&
    /* crosses */
    /* holes */
    array_in[57] < 0.5 && array_in[58] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_caf_sofit (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.85 && array_in[0] > 0.50 &&
    array_in[1] < 0.60 && array_in[1] > 0.30 &&
    array_in[3] < 0.55 && array_in[3] > 0.45 &&
    array_in[4] < 0.45 && array_in[4] > 0.15 && array_in[7] > 0.45 &&
    /* bars */
    array_in[9] > 0.5 && array_in[10] < 0.5 &&
    /* edges */
    array_in[80] > 0.5 &&
    /* notches */
    /* ends */
    (array_in[46] > 0.5 || array_in[47] > 0.5) && array_in[39] > 0.5 &&
    /* crosses */
    array_in[51] < 0.5 && array_in[52] < 0.5 && array_in[53] < 0.5 &&
    /* holes */
    array_in[57] < 0.5 && array_in[58] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_lamed (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.85 && array_in[0] > 0.50 &&
    array_in[1] < 0.50 && array_in[1] > 0.25 &&
    array_in[3] < 0.85 && array_in[3] > 0.50 &&
    array_in[4] < 0.60 && array_in[4] > 0.41 && array_in[6] > 0.5 &&
    /* bars */
    array_in[9] < 0.5 && array_in[10] < 0.5 &&
    /* edges */
    array_in[17] > 0.5 &&
    /* notches */
    /* ends */
    /* crosses */
    /* holes */
    array_in[57] < 0.5 && array_in[58] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_mem (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.60 && array_in[0] > 0.40 &&
    array_in[1] < 0.60 && array_in[1] > 0.30 &&
    array_in[3] < 0.60 && array_in[3] > 0.40 &&
    array_in[4] < 0.60 && array_in[4] > 0.40 && array_in[8] < 0.5 &&
    /* bars */
    /* edges */
    array_in[15] > 0.5 && array_in[24] > 0.5 &&
    /* notches */
    (array_in[33] > 0.5 || array_in[34] > 0.5 ||
      array_in[27] > 0.5 || array_in[28] > 0.5) &&
    (array_in[36] > 0.5 || array_in[37] > 0.5) && array_in[31] < 0.5 &&
    /* ends */
    array_in[39] > 0.5 && (array_in[47] < 0.5 || array_in[46] > 0.5) &&
    /* (array_in[42] > 0.5 || array_in[45] > 0.5) && */
    /* crosses */
    (array_in[48] > 0.5 || array_in[49] > 0.5 ||
      array_in[51] > 0.5 || array_in[52] > 0.5) &&
    /* holes */
    array_in[59] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_mem_sofit (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.60 && array_in[0] > 0.40 &&
    array_in[1] < 0.60 && array_in[1] > 0.30 &&
    array_in[3] < 0.60 && array_in[3] > 0.40 &&
    array_in[4] < 0.60 && array_in[4] > 0.40 &&
    !(array_in[8] >= 0.35 || array_in[7] >= 0.35) &&
    /* bars */
    array_in[9] > 0.5 && array_in[10] > 0.5 &&
    array_in[13] < 0.5 && array_in[14] < 0.5 &&
    /* edges */
    array_in[26] < 0.5 &&
    /* notches */
    /* ends */
    /* crosses */
    ((array_in[51] < 0.5 && array_in[52] < 0.5) || (array_in[45] < 0.5)) &&
    /* holes */
    array_in[57] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_nun (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    ((array_in[0] < 0.60 && array_in[0] > 0.40 &&
        array_in[1] < 0.50 && array_in[1] > 0.20 &&
        array_in[3] < 0.60 && array_in[3] > 0.30 &&
        array_in[4] < 0.60 && array_in[4] > 0.40 &&
        array_in[5] > 1.5 * array_in[7]) ||
      (array_in[0] < 0.60 && array_in[0] > 0.40 &&
        array_in[1] <= 0.30 && array_in[1] > 0.15 &&
        array_in[3] < 0.60 && array_in[3] > 0.40 &&
        array_in[4] < 0.60 && array_in[4] > 0.40)) &&
    /* bars */
    array_in[10] > 0.5 && array_in[11] < 0.5 && array_in[14] < 0.5 &&
    /* edges */
    array_in[26] < 0.5 &&
    /* notches */
    array_in[37] < 0.5 && array_in[38] < 0.5 &&
    /* ends */
    array_in[42] < 0.5 && array_in[43] < 0.5 &&
    /* crosses */
    array_in[51] < 0.5 && array_in[52] < 0.5 &&
    array_in[54] < 0.5 && array_in[55] < 0.5 &&
    /* holes */
    array_in[57] < 0.5 && array_in[59] < 0.5 &&
    array_in[96] < 0.5 && array_in[101] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_nun_sofit (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.85 && array_in[0] > 0.40 &&
    array_in[1] < 0.33 && array_in[1] > 0.05 &&
    array_in[3] < 0.55 && array_in[3] > 0.42 &&
    array_in[4] <= 0.48 && array_in[4] > 0.15 && array_in[7] > 0.5 &&
    /* bars */
    array_in[10] < 0.5 &&
    /* edges */
    array_in[82] < 0.5 &&
    /* notches */
    /* ends */
    (array_in[46] > 0.5 || array_in[47] > 0.5) &&
    /* crosses */
    array_in[52] < 0.5 &&
    /* holes */
    array_in[57] < 0.5 && array_in[58] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_samech (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.60 && array_in[0] > 0.40 &&
    array_in[1] < 0.60 && array_in[1] > 0.30 &&
    array_in[3] < 0.60 && array_in[3] > 0.40 &&
    array_in[4] < 0.60 && array_in[4] > 0.40 &&
    (array_in[8] >= 0.3 || array_in[7] >= 0.3) &&
    /* bars */
    array_in[9] > 0.5 && array_in[10] > 0.5 &&
    array_in[13] < 0.5 && array_in[14] < 0.5 &&
    /* edges */
    array_in[22] < 0.5 && array_in[23] < 0.5 && array_in[26] < 0.5 &&
    /* notches */
    /* ends */
    array_in[41] < 0.5 && array_in[82] < 0.5 &&
    /* crosses */
    array_in[51] < 0.5 && array_in[52] < 0.5 &&
    /* holes */
    array_in[57] > 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_ayin (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.67 && array_in[0] > 0.40 &&
    array_in[1] < 0.60 && array_in[1] > 0.30 &&
    array_in[3] < 0.60 && array_in[3] > 0.40 &&
    array_in[4] < 0.60 && array_in[4] > 0.30 && array_in[8] < 0.8 &&
    /* bars */
    /* edges */
    array_in[26] < 0.5 &&
    /* notches */
    array_in[31] < 0.5 && array_in[34] > 0.5 && array_in[37] < 0.5 &&
    /* ends */
    ((array_in[57] > 0.5 && array_in[73] < 0.2) ||
      ((array_in[39] > 0.5 || array_in[40] > 0.5)
        && (array_in[41] > 0.5 || array_in[40] > 0.5))) && array_in[45] > 0.5 &&
    /* crosses */
    (array_in[52] > 0.5 || array_in[53] > 0.5 || array_in[55] > 0.5
      || array_in[56] > 0.5) &&
    /* holes */
    array_in[59] < 0.5)
    return_value = 1.0;
  return return_value;
}

double
ho_recognize_font_david_pey (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.60 && array_in[0] > 0.40 &&
    array_in[1] < 0.60 && array_in[1] > 0.25 &&
    array_in[3] < 0.60 && array_in[3] > 0.40 &&
    array_in[4] < 0.60 && array_in[4] > 0.40 &&
    /* bars */
    array_in[10] > 0.5 &&
    /* edges */
    array_in[26] < 0.5 &&
    /* notches */
    array_in[37] < 0.5 && array_in[38] < 0.5 &&
    /* ends */
    (array_in[42] > 0.5 || array_in[43] > 0.5) && array_in[47] < 0.5 &&
    /* crosses */
    array_in[52] < 0.5 &&
    /* holes */
    array_in[57] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_pey_sofit (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.80
    && array_in[0] > 0.50
    && array_in[1] < 0.60
    && array_in[1] > 0.25
    && array_in[3] < 0.60
    && array_in[3] > 0.40
    && array_in[4] < 0.50 && array_in[4] > 0.10 && array_in[7] >= 1.0 &&
    /* bars */
    array_in[9] > 0.5 &&
    /* edges */
    array_in[26] > 0.5 &&
    /* notches */
    array_in[38] < 0.5 &&
    /* ends */
    (array_in[42] > 0.5 || array_in[43] > 0.5) && array_in[47] > 0.5 &&
    /* crosses */
    array_in[51] < 0.5 && array_in[52] < 0.5 &&
    /* holes */
    array_in[57] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;
  return return_value;
}

double
ho_recognize_font_david_tzadi (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.60 && array_in[0] > 0.40 &&
    array_in[1] < 0.60 && array_in[1] > 0.30 &&
    array_in[3] < 0.60 && array_in[3] > 0.40 &&
    array_in[4] < 0.60 && array_in[4] > 0.40 && array_in[8] < 0.8 &&
    /* bars */
    (array_in[13] > 0.5 ||
      (array_in[95] < 0.1 && array_in[45] > 0.5 && array_in[47] < 0.5)) &&
    array_in[10] > 0.5 &&
    /* edges */
    /* array_in[21] > 0.5 && array_in[23] > 0.5 && */
    array_in[24] < 0.5 && array_in[26] < 0.5 &&
    /* notches */
    array_in[34] > 0.5 && array_in[37] < 0.5 &&
    /* ends */
    (array_in[41] > 0.5 || array_in[40] > 0.5) &&
    (array_in[45] > 0.5 || array_in[46] > 0.5) &&
    /* crosses */
    (array_in[52] > 0.5 || array_in[55] > 0.5 || array_in[56] > 0.5) &&
    /* holes */
    array_in[57] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;
  return return_value;
}

double
ho_recognize_font_david_tzadi_sofit (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.80 && array_in[0] > 0.50 &&
    array_in[1] < 0.60 && array_in[1] > 0.25 &&
    array_in[3] < 0.60 && array_in[3] > 0.40 &&
    array_in[4] < 0.50 && array_in[4] > 0.10 &&
    (array_in[7] > 0.5 || array_in[8] > 0.5) &&
    /* bars */
    (array_in[9] < 0.5 || (array_in[83] > 0.5 && array_in[34] > 0.5) ||
      (array_in[39] > 0.5 && array_in[41] > 0.5 && array_in[52] > 0.5)) &&
    /* edges */
    (array_in[24] > 0.5 || array_in[25] > 0.5 || array_in[26] > 0.5) &&
    !(array_in[24] > 0.5 && array_in[26] > 0.5) &&
    /* notches */
    array_in[37] < 0.5 && array_in[38] < 0.5 &&
    /* ends */
    array_in[39] > 0.5 &&
    (array_in[41] > 0.5 || (array_in[83] && array_in[34])) &&
    /* crosses */
    (array_in[51] > 0.5 || array_in[52] > 0.5) &&
    /* holes */
    array_in[57] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_kuf (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.90 && array_in[0] > 0.50 &&
    array_in[1] < 0.60 && array_in[1] > 0.30 &&
    array_in[3] < 0.60 && array_in[3] > 0.40 &&
    array_in[4] < 0.50 && array_in[4] > 0.20 && array_in[7] < 0.5
    && array_in[8] > 0.5 &&
    /* bars */
    array_in[9] > 0.5 &&
    /* edges */
    array_in[15] > 0.5 && array_in[23] < 0.5 && array_in[24] > 0.5 &&
    /* notches */
    ((array_in[57] > 0.5 && array_in[79] > 0.5 && array_in[60] > 0.5) ||
      (array_in[27] > 0.5 || array_in[28] > 0.5)) &&
    array_in[30] < 0.5 && array_in[31] < 0.5 &&
    /* ends */
    /* array_in[39] > 0.5 && */
    array_in[41] < 0.5 &&
    (array_in[45] > 0.5 || (array_in[7] < 0.2 && array_in[8] > 0.8))
    /* crosses */
    /* array_in[49] < 0.5 && */
    /* holes */
    /* array_in[59] > 0.5 */ )
    return_value = 1.0;
  return return_value;
}

double
ho_recognize_font_david_resh (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.55
    && array_in[0] > 0.40
    && array_in[1] < 0.55
    && array_in[1] >= 0.28
    && array_in[3] < 0.60
    && array_in[3] > 0.45
    && array_in[4] < 0.60 && array_in[4] > 0.45 && array_in[7] > 0.5 &&
    /* bars */
    array_in[9] > 0.5 && array_in[12] > 0.5 && array_in[10] < 0.5 &&
    /* edges */
    array_in[83] < 0.5 &&
    /* notches */
    /* ends */
    array_in[47] > 0.5 &&
    /* crosses */
    array_in[49] < 0.5 &&
    /* holes */
    array_in[57] < 0.5 && array_in[58] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;
  return return_value;
}

double
ho_recognize_font_david_shin (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.60 && array_in[0] > 0.40 &&
    array_in[1] < 0.60 && array_in[1] > 0.40 &&
    array_in[3] < 0.60 && array_in[3] > 0.40 &&
    array_in[4] < 0.60 && array_in[4] > 0.40 && array_in[7] < 0.5 &&
    /* bars */
    /* edges */
    ((array_in[22] > 0.5
        || array_in[40] > 0.5
        || array_in[49] > 0.5)
      || (array_in[57] > 0.5 && array_in[73] < 0.20) || array_in[58] > 0.5) &&
    /* notches */
    (array_in[28] < 0.5 || array_in[45] < 0.5) && array_in[37] < 0.5 &&
    /* ends */
    array_in[46] < 0.5 && array_in[47] < 0.5 &&
    ((array_in[39] > 0.5
        && array_in[40] > 0.5
        && array_in[41] > 0.5)
      || array_in[49] > 0.5 || (array_in[57] && array_in[73] < 0.20) ||
      (array_in[41] > 0.5 && array_in[39] < 0.5 &&
        array_in[42] < 0.5 && array_in[45] < 0.5 &&
        array_in[55] > 0.5 && array_in[52] < 0.5)) &&
    /* crosses */
    ((array_in[54] > 0.5
        || array_in[55] > 0.5) || (array_in[57] > 0.5
        && array_in[73] < 0.25)) &&
    /* holes */
    array_in[59] < 0.5)
    return_value = 1.0;

  if (
    /* dimentions */
    array_in[0] < 0.60 && array_in[0] > 0.40 &&
    array_in[1] < 0.70 && array_in[1] > 0.40 &&
    array_in[3] < 0.60 && array_in[3] > 0.40 &&
    array_in[4] < 0.60 && array_in[4] > 0.40 && array_in[7] < 0.5 &&
    /* bars */
    /* edges */
    /* notches */
    /* ends */
    array_in[39] > 0.5 && array_in[40] > 0.5 &&
    array_in[41] > 0.5 && array_in[45] < 0.5 &&
    array_in[46] < 0.5 && array_in[47] < 0.5 &&
    /* crosses */
    array_in[52] < 0.5 && array_in[53] < 0.5 &&
    (array_in[54] > 0.5 || array_in[55] > 0.5) &&
    /* holes */
    array_in[57] < 0.5 && array_in[58] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_font_david_tav (const double *array_in)
{
  double return_value = 0.0;

  if (
    /* dimentions */
    array_in[0] < 0.60
    && array_in[0] > 0.40
    && array_in[1] < 0.60
    && array_in[1] > 0.30
    && array_in[3] < 0.60
    && array_in[3] > 0.40
    && array_in[4] < 0.60 && array_in[4] > 0.40 && array_in[8] < 0.5 &&
    /* bars */
    array_in[9] > 0.5 && array_in[10] < 0.5 &&
    /* edges */
    array_in[26] > 0.5 &&
    (array_in[82] > 0.5 || array_in[5] > array_in[7] * 2) &&
    /* notches */
    /* ends */
    array_in[45] > 0.5 && array_in[46] < 0.5 && array_in[47] > 0.5 &&
    /* crosses */
    array_in[52] < 0.5 && array_in[55] < 0.5 &&
    /* holes */
    array_in[57] < 0.5 && array_in[59] < 0.5)
    return_value = 1.0;
  return return_value;
}
