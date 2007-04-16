/***************************************************************************
 *            hocr.h
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

/** @mainpage libhocr
 * LIBrary for Hebrew Optical Character Recognition 
 *
 * @section README
 * @include README
 * 
 * @section ChangeLog
 * @include ChangeLog
 *
 */

#ifndef HOCR_H
#define HOCR_H 1

#ifdef __cplusplus
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS   }
#else /* !__cplusplus */
#  define BEGIN_C_DECLS
#  define END_C_DECLS
#endif /* __cplusplus */

BEGIN_C_DECLS
/* binary image map */
#include <ho_bitmap.h>
/* color image map */
#include <ho_pixbuf.h>
/* object image map */
#include <ho_objmap.h>
/* layout container for hocr layout */
#include <ho_layout.h>
#include <ho_dimentions.h>
#include <ho_segment.h>
/* font recognition */
#include <ho_font.h>
#include <ho_recognize.h>
#include <ho_recognize_sign.h>
/* string utilities */
#include <ho_string.h>
  END_C_DECLS
#endif /* HOCR_H */
