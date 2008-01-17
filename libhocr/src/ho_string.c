/***************************************************************************
 *            ho_string.c
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef TRUE
#define TRUE -1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL ((void*)0)
#endif

#include "ho_string.h"

/* 
 * how much memory (bytes) hocr_string_buffer will 
 * allocate when it nead new memory 
 */
#define MEMORY_CHANK_FOR_TEXT_BUFFER 500

/* 
 * internal string_buffer stractures 
 */

ho_string *
ho_string_new ()
{
  ho_string *new_string_buffer;

  /* allocate memory for struct */
  new_string_buffer = (ho_string *) malloc (sizeof (ho_string));

  /* check for new memory */
  if (!new_string_buffer)
    return NULL;

  /* init struct values */
  new_string_buffer->size = 0;
  new_string_buffer->allocated_size = MEMORY_CHANK_FOR_TEXT_BUFFER;

  /* allocate memory for string */
  new_string_buffer->string =
    (char *) malloc (sizeof (char) * new_string_buffer->allocated_size);

  /* if no memeory for string free the struct (no need for it with no string) 
   */
  if (!new_string_buffer->string)
    {
      free (new_string_buffer);
      return NULL;
    }

  /* init the string to zero length null terminated string */
  (new_string_buffer->string)[0] = '\0';

  return new_string_buffer;
}

int
ho_string_free (ho_string * string_buffer)
{
  /* free the string */
  if (string_buffer->string)
    free (string_buffer->string);

  /* free the struct */
  if (string_buffer)
    free (string_buffer);

  return FALSE;
}

int
ho_string_set (ho_string * string_buffer, const char *new_string)
{
  int len;
  char *new_allocated_string;

  len = strlen (new_string);

  /* check for allocated space and try to get more memory */
  new_allocated_string =
    (char *) malloc (sizeof (char) * (len + MEMORY_CHANK_FOR_TEXT_BUFFER));

  /* got new memory */
  if (new_allocated_string)
    {
      if (string_buffer->string)
	free (string_buffer->string);
      string_buffer->string = new_allocated_string;
      string_buffer->allocated_size = len + MEMORY_CHANK_FOR_TEXT_BUFFER;
    }
  /* did not get new memory */
  else
    {
      return TRUE;
    }

  /* add the new string */
  strcpy (string_buffer->string, new_string);
  string_buffer->size = strlen (string_buffer->string);

  return FALSE;
}

int
ho_string_cat (ho_string * string_buffer, const char *new_string)
{
  int len;
  char *new_allocated_string;

  len = strlen (new_string);

  /* check for allocated space and try to get more memory */
  if ((string_buffer->size + len) >= string_buffer->allocated_size)
    {
      new_allocated_string =
	realloc (string_buffer->string, sizeof (char) *
		 (string_buffer->allocated_size + len +
		  MEMORY_CHANK_FOR_TEXT_BUFFER));

      /* got new memory */
      if (new_allocated_string)
	{
	  string_buffer->string = new_allocated_string;
	  string_buffer->allocated_size += len + MEMORY_CHANK_FOR_TEXT_BUFFER;
	}
      /* did not get new memory */
      else
	{
	  return TRUE;
	}
    }

  /* add the new string */
  strcat (string_buffer->string, new_string);
  string_buffer->size = strlen (string_buffer->string);

  return FALSE;
}

ho_string *
ho_string_copy (const ho_string * string_buffer)
{
  ho_string *new_string = NULL;

  new_string = ho_string_new ();

  ho_string_cat (new_string, string_buffer->string);

  return new_string;
}
