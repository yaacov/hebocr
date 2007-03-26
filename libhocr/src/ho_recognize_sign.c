/***************************************************************************
 *            ho_recognize_sign.c
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
#include "ho_bitmap_filter.h"
#include "ho_segment.h"
#include "ho_font.h"
#include "ho_recognize.h"

#include "ho_recognize_sign.h"

/* TODO: font parts, e.g top-bar, little nose to the left ... */



/* reading signs */

double
ho_recognize_sign_dot (const double *array_in)
{
  double return_value = -1.0;

  if (array_in[54] < -0.25 && array_in[55] < -0.4 && array_in[57] < -0.2
      && (array_in[29] == -1.0 && array_in[33] == -1.0))
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_sign_comma (const double *array_in)
{
  double return_value = -1.0;

  if (array_in[54] < -0.25 && array_in[55] < -0.4 && array_in[57] < -0.2
      && (array_in[29] == -1.0 || array_in[33] == 1.0))
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_sign_minus (const double *array_in)
{
  double return_value = -1.0;

  if (array_in[54] < -0.5 && array_in[55] > -0.6)
    return_value = 1.0;

  return return_value;
}

/* hebrew fonts */

double
ho_recognize_sign_alef (const double *array_in)
{
  double return_value = -1.0;

  if (array_in[16] > -1.0 && (array_in[27] == 1.0 || array_in[28] == 1.0)
      && array_in[29] == 1.0 && (array_in[25] == 1.0
				 || (array_in[23] == 1.0
				     && array_in[24] == 1.0))
      && array_in[55] > -0.5)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_sign_bet (const double *array_in)
{
  double return_value = -1.0;

  if (array_in[55] > -0.3 && array_in[0] > -1.0 && array_in[4] > -1.0
      && array_in[12] > -1.0 && array_in[37] > -1.0 && array_in[35] == -1.0
      && array_in[31] == 1.0)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_sign_gimal (const double *array_in)
{
  double return_value = -1.0;

  return return_value;
}

double
ho_recognize_sign_dalet (const double *array_in)
{
  double return_value = -1.0;

  return return_value;
}

double
ho_recognize_sign_hey (const double *array_in)
{
  double return_value = -1.0;

  if (array_in[54] > -0.2 && array_in[54] < 0.1 && array_in[59] == 1.0
      && array_in[31] == 1.0 && array_in[0] > -1.0 && array_in[27] == 1.0
      && array_in[29] == 1.0 && array_in[4] == -1.0)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_sign_vav (const double *array_in)
{
  double return_value = -1.0;

  if (array_in[54] > -0.2 && array_in[55] < -0.4 && array_in[57] > -0.9
      && array_in[58] > -0.1 && array_in[4] == -1.0 &&
      array_in[12] > -1.0 && (array_in[35] == -1.0 || array_in[15] < -0.8))
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_sign_zayin (const double *array_in)
{
  double return_value = -1.0;

  if (array_in[54] > -0.2 && array_in[55] < -0.4 && array_in[57] > -0.9
      && array_in[58] > -0.1 && array_in[31] > -1.0 && array_in[4] == -1.0
      && !(array_in[35] == -1.0 || array_in[15] < -0.8))
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_sign_het (const double *array_in)
{
  double return_value = -1.0;

  return return_value;
}

double
ho_recognize_sign_tet (const double *array_in)
{
  double return_value = -1.0;

  return return_value;
}

double
ho_recognize_sign_yod (const double *array_in)
{
  double return_value = -1.0;

  if (array_in[54] < -0.25 && array_in[55] < -0.35 && array_in[57] > -0.2)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_sign_caf (const double *array_in)
{
  double return_value = -1.0;

  return return_value;
}

double
ho_recognize_sign_caf_sofit (const double *array_in)
{
  double return_value = -1.0;

  return return_value;
}

double
ho_recognize_sign_lamed (const double *array_in)
{
  double return_value = -1.0;

  if (array_in[54] > 0.1 && array_in[55] > -0.7 && array_in[57] > 0.1)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_sign_mem (const double *array_in)
{
  double return_value = -1.0;

  return return_value;
}

double
ho_recognize_sign_mem_sofit (const double *array_in)
{
  double return_value = -1.0;

  return return_value;
}

double
ho_recognize_sign_nun (const double *array_in)
{
  double return_value = -1.0;

  if (array_in[54] > -0.2 && array_in[55] < -0.2 && array_in[57] > -0.9
      && array_in[58] > -0.1 && array_in[4] == 1.0)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_sign_nun_sofit (const double *array_in)
{
  double return_value = -1.0;

  if (array_in[54] > -0.0 && array_in[55] < -0.4 && array_in[57] > -0.9
      && array_in[58] < -0.1 && array_in[31] > 0.0)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_sign_samech (const double *array_in)
{
  double return_value = -1.0;

  return return_value;
}

double
ho_recognize_sign_ayin (const double *array_in)
{
  double return_value = -1.0;

  return return_value;
}

double
ho_recognize_sign_pey (const double *array_in)
{
  double return_value = -1.0;

  return return_value;
}

double
ho_recognize_sign_pey_sofit (const double *array_in)
{
  double return_value = -1.0;

  return return_value;
}

double
ho_recognize_sign_tzadi (const double *array_in)
{
  double return_value = -1.0;

  return return_value;
}

double
ho_recognize_sign_tzadi_sofit (const double *array_in)
{
  double return_value = -1.0;

  return return_value;
}

double
ho_recognize_sign_kof (const double *array_in)
{
  double return_value = -1.0;

  return return_value;
}

double
ho_recognize_sign_resh (const double *array_in)
{
  double return_value = -1.0;

  return return_value;
}

double
ho_recognize_sign_shin (const double *array_in)
{
  double return_value = -1.0;

  if (array_in[23] == 1.0 && array_in[24] == 1.0 && array_in[25] == 1.0
      && array_in[29] == -1.0 && array_in[8] > -1.0)
    return_value = 1.0;

  return return_value;
}

double
ho_recognize_sign_tav (const double *array_in)
{
  double return_value = -1.0;

  return return_value;
}
