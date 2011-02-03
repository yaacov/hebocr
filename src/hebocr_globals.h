#ifndef HEBOCR_GLOBALS_H
#define HEBOCR_GLOBALS_H


/***************************************************************************
 * hebocr_globals.h
 *
 *  Sun Jan 16 13:34
 *  Copyright  2011  Diego Iastrubni <diegoiast@gmail.com
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

#ifndef TRUE
#	define TRUE -1
#endif

#ifndef FALSE
#	define FALSE 0
#endif

#ifndef NULL
#	define NULL ((void*)0)
#endif

#ifndef HEBOCR_UNUSED
#	define HEBOCR_UNUSED(f)(void)(f)
#endif

#ifndef HEBOCR_SQUARE
#	define HEBOCR_SQUARE(x) ((x)*(x))
#endif

#ifndef BUILD
#	define BUILD "Development build"
#endif 

#ifndef VERSION
#	define VERSION "0.12"
#endif    


#endif // HEBOCR_GLOBALS_H
