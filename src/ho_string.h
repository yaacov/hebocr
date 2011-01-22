
/***************************************************************************
 *            ho_string.h
 *
 *  Sat Dec 10 21:00:29 2005
 *  Copyright  2005  Yacov Zamir
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

/** @file ho_string.h
    @brief libhocr C language header.
    
    libhocr - LIBrary for Hebrew Optical Character Recognition 
*/

#ifndef HO_STRING_H
#define HO_STRING_H 1

/** @struct ho_string
  @brief libhocr string struct
*/
typedef struct
{
  char *string;
  int size;
  int allocated_size;
} ho_string;

/**
 @brief creats a new ho_string struct

 @return pointer to a newly allocate ho_string or null.
 */
ho_string *ho_string_new ();

/**
 @brief free a hocr_text_buffer struct from memory

 @param s_str pointer to ho_string struct.
 @return 1
 */
int ho_string_free (ho_string * s_str);

/**
 @brief add a string to hocr_text_buffer struct

 @param s_str pointer to hocr_text_buffer struct.
 @param text pointer to the string to be added to the buffer.
 @return new size of text
 */
int ho_string_cat (ho_string * s_str, const char *text);

/**
 @brief set a string to ho_string struct

 @param s_str pointer to hocr_text_buffer struct.
 @param text pointer to the string to be set to the buffer.
 @return new size of text
 */
int ho_string_set (ho_string * s_str, const char *text);

/**
 @brief get the string of ho_string struct

 @param s_str pointer to hocr_text_buffer struct.
 @return pointer to a newly allocated string (use free to free it)
 */
char * ho_string_get (const ho_string * s_str);

/**
 @brief copy ho_string struct

 @param s_str pointer to hocr_text_buffer struct.
 @return new ho_string
 */
ho_string *ho_string_copy (const ho_string * s_str);

#endif /* HO_STRING_H */
