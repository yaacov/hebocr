
/***************************************************************************
 *            hocr_object.h
 *
 *  Sat Dec 10 22:15:22 2005
 *  Copyright  2005  User
 *  Email
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __HOCR_OBJECT_H__
#define __HOCR_OBJECT_H__

typedef struct
{
	int x1;
	int y1;
	int x2;
	int y2;
	int weight;
	int hight;
	int width;
	unsigned int name;
} hocr_object;

/* object helper functions */
int clean_object_array (unsigned int *object_array, int size);

int
is_in_object_array (unsigned int obj,
		    unsigned int *object_array, int size);

int count_object_array (unsigned int *object_array, int size);

#endif
