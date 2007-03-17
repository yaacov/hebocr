/***************************************************************************
 *            ho_recognize.c
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

int
ho_recognize_hbar_up (const ho_bitmap * m_text, const ho_bitmap * m_mask,
		      int *height, int *start, int *end)
{
  return FALSE;
}

int
ho_recognize_hbar_down (const ho_bitmap * m_text, const ho_bitmap * m_mask,
			int *height, int *start, int *end)
{
  return FALSE;
}

int
ho_recognize_vbar_right (const ho_bitmap * m_text, const ho_bitmap * m_mask,
			 int *width, int *start, int *end)
{
  return FALSE;
}

int
ho_recognize_vbar_left (const ho_bitmap * m_text, const ho_bitmap * m_mask,
			int *width, int *start, int *end)
{
  return FALSE;
}

int
ho_recognize_dbar (const ho_bitmap * m_text, const ho_bitmap * m_mask,
		   int *width, int *start, int *end)
{
  return FALSE;
}

#include "ho_recognize.h"
