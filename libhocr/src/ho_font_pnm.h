/***************************************************************************
 *            ho_font_pnm.h
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

#ifndef HO_FONT_PNM_H
#define HO_FONT_PNM_H 1

#include <ho_bitmap.h>
#include <ho_objmap.h>

int
ho_font_pnm_save (const ho_bitmap * m_text, const ho_bitmap * m_nikud,
		  const ho_bitmap * m_mask, const char *filename);

int
ho_font_pnm_load (ho_bitmap ** m_text, ho_bitmap ** m_nikud,
		  ho_bitmap ** m_mask, const char *filename);

#endif /* HO_FONT_PNM_H */
