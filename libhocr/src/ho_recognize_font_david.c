
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

  if (array_in[0] < 0.20)
    return_value += 0.1;
  if (array_in[1] < 0.20)
    return_value += 0.1;
  if (array_in[2] > 0.9 && array_in[2] < 1.1)
    return_value += 0.05;
  if (array_in[3] < 0.20)
    return_value += 0.1;
  if (array_in[4] > 0.52 && array_in[4] < 0.48)
    return_value += 0.05;
  
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

  if (array_in[13] > 0.9)
    return_value += 0.3;
  if (array_in[47] > 0.9)
    return_value += 0.1;
  
  return return_value;
}

double
ho_recognize_font_david_bet (const double *array_in)
{
  double return_value = 0.0;

  if (array_in[9] > 0.9)
    return_value += 0.08;
  if (array_in[10] > 0.9)
    return_value += 0.08;
  if (array_in[11] < 0.1)
    return_value += 0.08;
  if (array_in[12] < 0.1)
    return_value += 0.08;
  
  if (array_in[20] > 0.9)
    return_value += 0.15;
  if (array_in[28] > 0.9)
    return_value += 0.05;
  
  if (array_in[37] > 0.9)
    return_value -= 0.3;
  if (array_in[38] > 0.9)
    return_value -= 0.3;
  
  return return_value;
}

double
ho_recognize_font_david_gimal (const double *array_in)
{
  double return_value = 0.0;

  if (array_in[18] > 0.9)
    return_value += 0.1;
  if (array_in[28] > 0.9)
    return_value += 0.1;
  if (array_in[29] > 0.9)
    return_value += 0.1;
  
  if (array_in[37] > 0.9)
    return_value += 0.2;
  if (array_in[38] > 0.9)
    return_value += 0.2;
  
  if (array_in[24] > 0.9 && array_in[25] > 0.9)
    return_value -= 0.2;
  if (array_in[34] > 0.9)
    return_value -= 0.2;
  
  return return_value;
}

double
ho_recognize_font_david_dalet (const double *array_in)
{
  double return_value = 0.0;

  if (array_in[9] > 0.9)
    return_value += 0.05;
  if (array_in[10] < 0.1)
    return_value += 0.05;
  if (array_in[11] < 0.1)
    return_value += 0.05;
  if (array_in[12] > 0.9)
    return_value += 0.05;
  
  if (array_in[18] > 0.9)
    return_value += 0.1;
  
  if (array_in[24] > 0.9)
    return_value -= 0.3;
  
  if (array_in[59])
    return_value -= 0.3;
  
  return return_value;
}

double
ho_recognize_font_david_hey (const double *array_in)
{
  double return_value = 0.0;
  
  if (array_in[59])
    return_value += 0.3;
  
  if (array_in[9] > 0.9)
    return_value += 0.05;
  if (array_in[10] < 0.1)
    return_value += 0.05;
  if (array_in[11] < 0.1)
    return_value += 0.05;
  if (array_in[12] > 0.9)
    return_value += 0.05;
  
  return return_value;
}

double
ho_recognize_font_david_vav (const double *array_in)
{
  double return_value = 0.0;

  if (array_in[2] < 0.4)
    return_value += 0.2;
  if (array_in[2] < 0.5)
    return_value += 0.2;
  
  if (array_in[9] > 0.9)
    return_value += 0.05;
  if (array_in[10] < 0.1)
    return_value += 0.05;
  if (array_in[11] < 0.1)
    return_value += 0.05;
  if (array_in[12] > 0.9)
    return_value += 0.05;
  
  if (array_in[18] > 0.9)
    return_value -= 0.1;
  
  if (array_in[2] > 0.5)
    return_value -= 0.1;
  
  if (array_in[37] > 0.9 || array_in[38] > 0.9)
    return_value -= 0.2;
  
  return return_value;
}

double
ho_recognize_font_david_zayin (const double *array_in)
{
  double return_value = 0.0;

  if (array_in[2] < 0.4)
    return_value += 0.1;
  if (array_in[2] < 0.5)
    return_value += 0.1;
  
  if (array_in[9] > 0.9)
    return_value += 0.05;
  if (array_in[10] < 0.1)
    return_value += 0.05;
  if (array_in[11] < 0.1)
    return_value += 0.05;
  if (array_in[12] > 0.9)
    return_value += 0.05;
  
  if (array_in[18] > 0.9)
    return_value += 0.2;
  
  if (array_in[2] > 0.8)
    return_value -= 0.5;
  
  if (array_in[24] > 0.9)
    return_value -= 0.2;
  if (array_in[37] > 0.9 || array_in[38] > 0.9)
    return_value -= 0.2;
  
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
