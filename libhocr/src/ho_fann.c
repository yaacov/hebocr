/***************************************************************************
 *            ho_fann.c
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
#include "ho_recognize_sign.h"
#include "ho_recognize.h"

#include "ho_fann.h"

ho_fann *
ho_fann_new (const char *path)
{
  ho_fann *ann;

  ann = (ho_fann *) malloc (sizeof (ho_fann));
  if (!ann)
    return NULL;

  ann->ann = fann_create_from_file (path);

  return ann;
}

int
ho_fann_free (ho_fann * ann)
{
  if (ann && ann->ann)
    {
      fann_destroy (ann->ann);
      free (ann);
    }

  return FALSE;
}

int
ho_fann_create_array_out (const ho_fann * ann, const double *array_in,
			  double *array_out)
{
  int i;
  fann_type *f_out;

  /* run the net */
  f_out = fann_run (ann->ann, (fann_type *) array_in);

  /* copy array out */
  for (i = 0; i < HO_ARRAY_OUT_SIZE; i++)
    {
      array_out[i] = f_out[i];
    }

  return FALSE;
}

char *
ho_fann_recognize_font (const ho_fann * ann, const ho_bitmap * m_text,
			const ho_bitmap * m_mask)
{
  double array_in[HO_ARRAY_IN_SIZE];
  double array_out[HO_ARRAY_OUT_SIZE];
  char *font;
  int i;

  ho_recognize_create_array_in (m_text, m_mask, array_in);
  ho_fann_create_array_out (ann, array_in, array_out);
  font = ho_recognize_array_out_to_font (array_out);

  return font;
}
