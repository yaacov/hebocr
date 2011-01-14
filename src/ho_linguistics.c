
/***************************************************************************
 *            ho_linguistics.c
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

#include "ho_linguistics.h"

int
ho_linguistics_adjust_array_out (double *array_out, unsigned char word_end,
  unsigned char word_start, int last_char_i)
{
  int i;

  const double factor = 0.3;

  
  if (word_end)
  {
    /* not otiuyt sofiyut */
    /* caf */
    array_out[11] -= factor;
    /* mem */
    array_out[14] -= factor;
    /* nun */
    array_out[16] -= factor;
    /* pe */
    array_out[20] -= factor;
    /* tzadik */
    array_out[22] -= factor;

  }
  else
  {
    /* sofiyut */
    /* caf */
    array_out[12] -= factor;
    /* mem */
    array_out[15] -= factor;
    /* nun */
    array_out[17] -= factor;
    /* pe */
    array_out[21] -= factor;
    /* tzadik */
    array_out[23] -= factor;
    
    /* simaney pisuk */
    array_out[28] -= factor;
    array_out[29] -= factor;
    array_out[32] -= factor;
    array_out[23] -= factor;
    array_out[34] -= factor;
    array_out[35] -= factor;
  }

  return FALSE;
}
