/***************************************************************************
 *            font_markers.h
 *
 *  Fri Aug 12 20:33:02 2005
 *  Copyright  2005  Yaacov Zamir
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/* 
 font markers
 */

#include <gnome.h>

#include "box.h"

#ifndef __FONT_MARKERS_H__
#define __FONT_MARKERS_H__


/**
 */

int alef_mark (GdkPixbuf * pix, box font);
int has_bet_mark (GdkPixbuf * pix, box font);
int has_gimel_mark (GdkPixbuf * pix, box font);
int has_dalet_mark (GdkPixbuf * pix, box font);
int has_he_mark (GdkPixbuf * pix, box font);
int has_vav_mark (GdkPixbuf * pix, box font);
int has_zain_mark (GdkPixbuf * pix, box font);
int has_het_mark (GdkPixbuf * pix, box font);
int has_tet_mark (GdkPixbuf * pix, box font);
int has_yud_mark (GdkPixbuf * pix, box font);
int has_kaf_mark (GdkPixbuf * pix, box font);
int has_kaf_sofit_mark (GdkPixbuf * pix, box font);
int has_lamed_mark (GdkPixbuf * pix, box font);
int has_mem_mark (GdkPixbuf * pix, box font);
int has_mem_sofit_mark (GdkPixbuf * pix, box font);
int has_nun_mark (GdkPixbuf * pix, box font);
int has_nun_sofit_mark (GdkPixbuf * pix, box font);
int has_sameh_mark (GdkPixbuf * pix, box font);
int has_ayin_mark (GdkPixbuf * pix, box font);
int has_pe_mark (GdkPixbuf * pix, box font);
int has_pe_sofit_mark (GdkPixbuf * pix, box font);
int has_tzadi_mark (GdkPixbuf * pix, box font);
int has_tzadi_sofit_mark (GdkPixbuf * pix, box font);
int has_kof_mark (GdkPixbuf * pix, box font);
int has_resh_mark (GdkPixbuf * pix, box font);
int has_shin_mark (GdkPixbuf * pix, box font);
int has_tav_mark (GdkPixbuf * pix, box font);

int has_quat_mark (GdkPixbuf * pix, box font);
int has_double_quat_mark (GdkPixbuf * pix, box font);
int has_exlem_mark (GdkPixbuf * pix, box font);
int has_question_mark (GdkPixbuf * pix, box font);
int has_makaf_mark (GdkPixbuf * pix, box font);

/**
 */

#endif
