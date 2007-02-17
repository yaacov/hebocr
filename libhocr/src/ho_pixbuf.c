/***************************************************************************
 *            ho_pixbuf.c
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

#include "ho_common.h"
#include "ho_pixbuf.h"

ho_pixbuf *
ho_pixbuf_new (const ho_uchar n_channels,
	       const ho_uint width, const ho_uint height,
	       const ho_uint rowstride)
{
  ho_pixbuf *pix = HO_NULL;

  /*
   * allocate memory for pixbuf 
   */
  pix = (ho_pixbuf *) malloc (sizeof (ho_pixbuf));
  if (!pix)
    {
      return HO_NULL;
    }

  /*
   * read header 
   */
  pix->n_channels = n_channels;
  pix->width = width;
  pix->height = height;
  if (rowstride)
    pix->rowstride = rowstride;
  else
    pix->rowstride = width * n_channels;

  /*
   * allocate memory for data (and set to white)
   */
  pix->data = malloc (pix->height * pix->rowstride);
  memset ((void *) (pix->data), 255, pix->height * pix->rowstride);
  if (!(pix->data))
    {
      free (pix);
      return HO_NULL;
    }

  return pix;
}

ho_pixbuf *
ho_pixbuf_clone (const ho_pixbuf * m)
{
  ho_pixbuf *m_out;

  /* allocate memory */
  m_out = ho_pixbuf_new (m->n_channels, m->width, m->height, m->rowstride);
  if (!m_out)
    return HO_NULL;

  /* copy data */
  memcpy (m_out->data, m->data, m_out->height * m_out->rowstride);

  return m_out;
}

ho_pixbuf *
ho_pixbuf_new_from_bitmap (const ho_bitmap * bit_in)
{
  ho_uint x, y;
  ho_pixbuf *pix = HO_NULL;

  pix = ho_pixbuf_new (1, bit_in->width, bit_in->height, 0);
  if (!pix)
    return HO_NULL;

  for (x = 0; x < bit_in->width; x++)
    for (y = 0; y < bit_in->height; y++)
      ho_pixbuf_set (pix, x, y, 0, 255 * (1 - ho_bitmap_get (bit_in, x, y)));

  return pix;
}

int
ho_pixbuf_draw_bitmap (ho_pixbuf * m, const ho_bitmap * bit_in,
		       const ho_uchar red, const ho_uchar green,
		       const ho_uchar blue)
{
  ho_uint x, y;

  /* sanity check */
  if (m->width != bit_in->width || m->height != bit_in->height)
    return HO_TRUE;

  /* is pixbuf color ? */
  if (m->n_channels < 3)
    {				/* gray scale */
      for (x = 0; x < bit_in->width; x++)
	for (y = 0; y < bit_in->height; y++)
	  if (ho_bitmap_get (bit_in, x, y))
	    ho_pixbuf_set (m, x, y, 0, red);
    }
  else
    {				/* color */
      for (x = 0; x < bit_in->width; x++)
	for (y = 0; y < bit_in->height; y++)
	  if (ho_bitmap_get (bit_in, x, y))
	    {
	      ho_pixbuf_set (m, x, y, 0, red);
	      ho_pixbuf_set (m, x, y, 1, green);
	      ho_pixbuf_set (m, x, y, 2, blue);
	    }
    }

  return HO_FALSE;
}

int
ho_pixbuf_draw_bitmap_at (ho_pixbuf * m, const ho_bitmap * bit_in,
			  const ho_uint x1, const ho_uint y1,
			  const ho_uchar red, const ho_uchar green,
			  const ho_uchar blue)
{
  int x, y;

  /* is pixbuf color ? */
  if (m->n_channels < 3)
    {				/* gray scale */
      for (x = 0; x < bit_in->width; x++)
	for (y = 0; y < bit_in->height; y++)
	  if ((x + x1) >= 0 && (x + x1) < m->width && (y + y1) >= 0
	      && (y + y1) < m->height)
	    if (ho_bitmap_get (bit_in, x, y))
	      ho_pixbuf_set (m, (x + x1), (y + y1), 0, red);
    }
  else
    {				/* color */
      for (x = 0; x < bit_in->width; x++)
	for (y = 0; y < bit_in->height; y++)
	  if (ho_bitmap_get (bit_in, x, y))
	    {
	      ho_pixbuf_set (m, (x + x1), (y + y1), 0, red);
	      ho_pixbuf_set (m, (x + x1), (y + y1), 1, green);
	      ho_pixbuf_set (m, (x + x1), (y + y1), 2, blue);
	    }
    }

  return HO_FALSE;
}

ho_pixbuf *
ho_pixbuf_new_from_objmap (const ho_objmap * obj_in, const ho_uchar min,
			   const ho_uchar max)
{
  ho_uint x, y;
  ho_pixbuf *pix = HO_NULL;
  ho_usint index = 0;

  /* allocate memory */
  pix = ho_pixbuf_new (3, obj_in->width, obj_in->height, 0);
  if (!pix)
    return HO_NULL;

  /* copy pixels from gsl mtirxes */
  for (x = 0; x < obj_in->width; x++)
    for (y = 0; y < obj_in->height; y++)
      {
	index = ho_objmap_get (obj_in, x, y);

	if (index)
	  {
	    /* set autput colors */
	    ho_pixbuf_set (pix, x, y, 0,
			   min + (max - min) * (index % 12) / 12);
	    ho_pixbuf_set (pix, x, y, 1, min + (max - min) * (index % 6) / 6);
	    ho_pixbuf_set (pix, x, y, 2, min + (max - min) * (index % 9) / 9);
	  }
	else
	  {
	    /* set white */
	    ho_pixbuf_set (pix, x, y, 0, 255);
	    ho_pixbuf_set (pix, x, y, 1, 255);
	    ho_pixbuf_set (pix, x, y, 2, 255);
	  }
      }

  return pix;
}

ho_pixbuf *
ho_pixbuf_to_rgb (const ho_pixbuf * pix_in)
{
  ho_uint x, y;
  ho_pixbuf *pix = HO_NULL;
  ho_uchar red, green, blue;

  /* allocate memory */
  pix = ho_pixbuf_new (3, pix_in->width, pix_in->height, 0);
  if (!pix)
    return HO_NULL;

  /* does original pix has the rgb channels ? */
  if (pix_in->n_channels < 3)
    {
      /* copy gray pixels */
      for (x = 0; x < pix_in->width; x++)
	for (y = 0; y < pix_in->height; y++)
	  {
	    red = ho_pixbuf_get (pix_in, x, y, 0);
	    ho_pixbuf_set (pix, x, y, 0, red);
	    ho_pixbuf_set (pix, x, y, 1, red);
	    ho_pixbuf_set (pix, x, y, 2, red);
	  }

    }
  else
    {
      /* copy color pixels */
      for (x = 0; x < pix_in->width; x++)
	for (y = 0; y < pix_in->height; y++)
	  {
	    red = ho_pixbuf_get (pix_in, x, y, 0);
	    green = ho_pixbuf_get (pix_in, x, y, 1);
	    blue = ho_pixbuf_get (pix_in, x, y, 2);
	    ho_pixbuf_set (pix, x, y, 0, red);
	    ho_pixbuf_set (pix, x, y, 1, green);
	    ho_pixbuf_set (pix, x, y, 2, blue);
	  }
    }

  return pix;
}

int
ho_pixbuf_free (ho_pixbuf * pix)
{
  if (!pix)
    return HO_TRUE;

  if (pix->data)
    free (pix->data);

  free (pix);

  return HO_FALSE;
}

ho_uchar
ho_pbm_getc (FILE * file)
{
  ho_uchar ch;
  int comment = HO_FALSE;

  do
    {
      ch = getc (file);
      if (ch == '\n')
	comment = HO_FALSE;
      else if (ch == '#')
	comment = HO_TRUE;
    }
  while (comment);

  return ch;
}

int
ho_pbm_getint (FILE * file)
{
  ho_uchar ch;
  int i = 0;

  do
    {
      ch = ho_pbm_getc (file);
    }
  while (ch == ' ' || ch == '\n' || ch == '\t');

  do
    {
      i = (i * 10) + (ch - '0');
      ch = ho_pbm_getc (file);
    }
  while (ch >= '0' && ch <= '9');

  return i;
}

ho_pixbuf *
ho_pixbuf_new_from_pnm (const char *filename)
{
  char ch1, ch2;
  ho_pixbuf *pix = HO_NULL;
  FILE *file = HO_NULL;
  ho_uchar use_stdin = 0;
  ho_uchar n_channels = 0;
  ho_uint width = 0;
  ho_uint height = 0;

  /* if no input file name use stdin for input */
  if (!filename || filename[0] == '\0'
      || (filename[0] == '-' && filename[1] == '\0'))
    {
      /* use stdin */
      file = stdin;
      use_stdin = 1;
    }
  else
    {
      /* open file */
      file = fopen (filename, "r");
      if (!file)
	return HO_NULL;
    }

  /* read magic number "P?" for pbm file */
  ch1 = ho_pbm_getc (file);
  ch2 = ho_pbm_getc (file);
  if (ch1 != 'P' || (ch2 != '5' && ch2 != '6'))
    {
      /* bad magic */
      if (!use_stdin)
	fclose (file);
      return HO_NULL;
    }

  /* read header */
  n_channels = ((ch2 == '6') ? 3 : 1);
  width = ho_pbm_getint (file);
  height = ho_pbm_getint (file);

  /* read bits per pixel */
  if (ho_pbm_getint (file) > 255)
    {
      /* bad bits per pixel */
      if (!use_stdin)
	fclose (file);

      return HO_NULL;
    }

  /* create a new pixbuf */
  pix = ho_pixbuf_new (n_channels, width, height, 0);

  if (pix)
    fread (pix->data, 1, pix->height * pix->rowstride, file);
  if (!use_stdin)
    fclose (file);

  /* return the new pixbuf to user */
  return pix;
}

int
ho_pixbuf_save_pnm (const ho_pixbuf * pix, const char *filename)
{
  FILE *file = HO_NULL;

  if (pix->n_channels != 3 && pix->n_channels != 1)
    {
      /* bad magic */
      return HO_TRUE;
    }

  file = fopen (filename, "wb");

  if (!file)
    return HO_TRUE;

  /* print header */
  fprintf (file, "P%c %d %d 255\n", ((pix->n_channels == 3) ? '6' : '5'),
	   pix->width, pix->height);

  /* this might be a huge write... */
  fwrite (pix->data, 1, pix->height * pix->rowstride, file);
  fclose (file);

  return HO_FALSE;
}

ho_pixbuf *
ho_pixbuf_color_to_gray (const ho_pixbuf * pix)
{
  ho_pixbuf *pix_gray = HO_NULL;
  ho_uint x, y;
  ho_uchar red, green, blue;

  /* does input has the rgb channels ? */
  if (pix->n_channels < 3)
    return HO_NULL;

  /* allocate memory */
  pix_gray = ho_pixbuf_new (1, pix->width, pix->height, 0);
  if (!pix_gray)
    return HO_NULL;

  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
      {
	red = ho_pixbuf_get (pix, x, y, 0);
	green = ho_pixbuf_get (pix, x, y, 1);
	blue = ho_pixbuf_get (pix, x, y, 2);

	ho_pixbuf_set (pix_gray, x, y, 0,
		       (ho_pixbuf_max3 (red, green, blue) +
			ho_pixbuf_min3 (red, green, blue)) / 2.0);
      }

  return pix_gray;
}

ho_pixbuf *
ho_pixbuf_scale2 (const ho_pixbuf * pix)
{
  ho_pixbuf *pix_scaled = HO_NULL;
  ho_uint x, y;
  ho_uchar neighbors[4];

  /* is input gray ? */
  if (pix->n_channels != 1 || pix->width < 3 || pix->height < 3)
    return HO_NULL;

  /* allocate memory */
  pix_scaled = ho_pixbuf_new (1, pix->width * 2 - 2, pix->height * 2 - 2, 0);
  if (!pix_scaled)
    return HO_NULL;

  for (x = 0; x < pix->width - 1; x++)
    for (y = 0; y < pix->height - 1; y++)
      {
	neighbors[0] = (pix->data)[x + y * pix->rowstride];
	neighbors[1] = (pix->data)[x + 1 + y * pix->rowstride];
	neighbors[2] = (pix->data)[x + (y + 1) * pix->rowstride];
	neighbors[3] = (pix->data)[x + 1 + (y + 1) * pix->rowstride];

	(pix_scaled->data)[(x * 2) + (y * 2) * pix_scaled->rowstride] =
	  neighbors[0];
	(pix_scaled->data)[(x * 2) + 1 + (y * 2) * pix_scaled->rowstride] =
	  (neighbors[0] + neighbors[1]) / 2;
	(pix_scaled->data)[(x * 2) + ((y * 2) + 1) * pix_scaled->rowstride] =
	  (neighbors[0] + neighbors[2]) / 2;
	(pix_scaled->data)[(x * 2) + 1 +
			   ((y * 2) + 1) * pix_scaled->rowstride] =
	  (neighbors[0] + neighbors[1] + neighbors[2] + neighbors[3]) / 4;
      }

  return pix_scaled;
}

ho_pixbuf *
ho_pixbuf_scale3 (const ho_pixbuf * pix)
{
  ho_pixbuf *pix_scaled = HO_NULL;
  ho_uint x, y;
  ho_uchar neighbors[4];

  /* is input gray ? */
  if (pix->n_channels != 1 || pix->width < 3 || pix->height < 3)
    return HO_NULL;

  /* allocate memory */
  pix_scaled = ho_pixbuf_new (1, pix->width * 3 - 3, pix->height * 3 - 3, 0);
  if (!pix_scaled)
    return HO_NULL;

  for (x = 0; x < pix->width - 1; x++)
    for (y = 0; y < pix->height - 1; y++)
      {
	neighbors[0] = (pix->data)[x + y * pix->rowstride];
	neighbors[1] = (pix->data)[x + 1 + y * pix->rowstride];
	neighbors[2] = (pix->data)[x + (y + 1) * pix->rowstride];
	neighbors[3] = (pix->data)[x + 1 + (y + 1) * pix->rowstride];

	(pix_scaled->data)[(x * 3) + (y * 3) * pix_scaled->rowstride] =
	  neighbors[0];

	(pix_scaled->data)[(x * 3) + 1 + (y * 3) * pix_scaled->rowstride] =
	  (neighbors[0] * 2 + neighbors[1]) / 3;
	(pix_scaled->data)[(x * 3) + 2 + (y * 3) * pix_scaled->rowstride] =
	  (neighbors[0] + neighbors[1] * 2) / 3;

	(pix_scaled->data)[(x * 3) + ((y * 3) + 1) * pix_scaled->rowstride] =
	  (neighbors[0] * 2 + neighbors[2]) / 3;
	(pix_scaled->data)[(x * 3) + ((y * 3) + 2) * pix_scaled->rowstride] =
	  (neighbors[0] + neighbors[2] * 2) / 3;

	(pix_scaled->data)[(x * 3) + 1 +
			   ((y * 3) + 1) * pix_scaled->rowstride] =
	  (neighbors[0] * 3 + neighbors[1] + neighbors[2] + neighbors[3]) / 6;
	(pix_scaled->data)[(x * 3) + 2 +
			   ((y * 3) + 1) * pix_scaled->rowstride] =
	  (neighbors[0] + neighbors[1] * 3 + neighbors[2] + neighbors[3]) / 6;
	(pix_scaled->data)[(x * 3) + 1 +
			   ((y * 3) + 2) * pix_scaled->rowstride] =
	  (neighbors[0] + neighbors[1] + neighbors[2] * 3 + neighbors[3]) / 6;
	(pix_scaled->data)[(x * 3) + 2 +
			   ((y * 3) + 2) * pix_scaled->rowstride] =
	  (neighbors[0] + neighbors[1] + neighbors[2] + neighbors[3] * 3) / 6;
      }

  return pix_scaled;
}

ho_pixbuf *
ho_pixbuf_scale (const ho_pixbuf * pix, const ho_uchar scale)
{
  ho_pixbuf *pix_temp1 = HO_NULL;
  ho_pixbuf *pix_temp2 = HO_NULL;

  /* is input sane ? */
  if (scale < 2 || pix->n_channels != 1 || pix->width < 3 || pix->height < 3)
    return HO_NULL;

  if (scale == 2)
    return ho_pixbuf_scale2 (pix);

  if (scale == 3)
    return ho_pixbuf_scale3 (pix);

  if (scale < 6)		/* 4 and 5 */
    {
      pix_temp1 = ho_pixbuf_scale2 (pix);
      pix_temp2 = ho_pixbuf_scale2 (pix_temp1);
      ho_pixbuf_free (pix_temp1);

      return pix_temp2;
    }

  if (scale == 6)
    {
      pix_temp1 = ho_pixbuf_scale3 (pix);
      pix_temp2 = ho_pixbuf_scale2 (pix_temp1);
      ho_pixbuf_free (pix_temp1);

      return pix_temp2;
    }

  if (scale < 9)		/* 7 and 8 */
    {
      pix_temp1 = ho_pixbuf_scale2 (pix);
      pix_temp2 = ho_pixbuf_scale2 (pix_temp1);
      ho_pixbuf_free (pix_temp1);
      pix_temp1 = ho_pixbuf_scale2 (pix_temp2);
      ho_pixbuf_free (pix_temp2);

      return pix_temp1;
    }

  if (scale == 9)
    {
      pix_temp1 = ho_pixbuf_scale3 (pix);
      pix_temp2 = ho_pixbuf_scale3 (pix_temp1);
      ho_pixbuf_free (pix_temp1);

      return pix_temp2;
    }

  return HO_NULL;
}

ho_uchar
ho_pixbuf_minmax (const ho_pixbuf * pix, ho_uchar * min, ho_uchar * max)
{
  ho_uint x, y;

  *min = 0;
  *max = 255;

  /* is input gray ? */
  if (pix->n_channels != 1)
    return HO_TRUE;

  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
      {
	if (*min > (pix->data)[x + y * pix->rowstride])
	  *min = (pix->data)[x + y * pix->rowstride];
	if (*max < (pix->data)[x + y * pix->rowstride])
	  *max = (pix->data)[x + y * pix->rowstride];
      }

  return HO_FALSE;
}

int
ho_pixbuf_draw_line (ho_pixbuf * m, const ho_uint x1, const ho_uint y1,
		     const ho_uint x2, const ho_uint y2, const ho_uchar red,
		     const ho_uchar green, const ho_uchar blue)
{
  double x, step_x;
  double y, step_y;
  ho_uint x_start = x1;
  ho_uint x_end = x2;
  ho_uint y_start = y1;
  ho_uint y_end = y2;

  step_x = ((double) x2 - (double) x1) * ((double) x2 - (double) x1);
  step_y = ((double) y2 - (double) y1) * ((double) y2 - (double) y1);

  if (step_y > step_x)
    {
      /* vertical line */
      if (y1 > y2)
	{
	  x_start = x2;
	  x_end = x1;
	  y_start = y2;
	  y_end = y1;
	}

      x = (double) x_start;
      step_x =
	((double) x_end - (double) x_start) / ((double) y_end -
					       (double) y_start);
      for (y = y_start; y <= y_end; y++)
	{
	  (m->data)[(int) x * m->n_channels + (int) y * m->rowstride] = red;
	  if (m->n_channels >= 3)
	    {
	      (m->data)[(int) x * m->n_channels + (int) y * m->rowstride +
			1] = green;
	      (m->data)[(int) x * m->n_channels + (int) y * m->rowstride +
			2] = blue;
	    }
	  x += step_x;
	}
    }
  else
    {
      /* horizontal line */
      if (x1 > x2)
	{
	  x_start = x2;
	  x_end = x1;
	  y_start = y2;
	  y_end = y1;
	}

      y = (double) y_start;
      step_y =
	((double) y_end - (double) y_start) / ((double) x_end -
					       (double) x_start);
      for (x = x_start; x <= x_end; x++)
	{
	  (m->data)[(int) x * m->n_channels + (int) y * m->rowstride] = red;
	  if (m->n_channels >= 3)
	    {
	      (m->data)[(int) x * m->n_channels + (int) y * m->rowstride +
			1] = green;
	      (m->data)[(int) x * m->n_channels + (int) y * m->rowstride +
			2] = blue;
	    }
	  y += step_y;
	}
    }

  return HO_FALSE;
}

int
ho_pixbuf_draw_horizontal_scale (ho_pixbuf * m, const ho_uint x1,
				 const ho_uint y1, const ho_uint length,
				 const ho_uint step, const ho_uchar red,
				 const ho_uchar green, const ho_uchar blue)
{
  ho_uint x;

  for (x = 0; x <= length; x++)
    {
      ho_pixbuf_set (m, x1 + x, y1, 0, red);
      if (m->n_channels >= 3)
	{
	  ho_pixbuf_set (m, x1 + x, y1, 1, green);
	  ho_pixbuf_set (m, x1 + x, y1, 2, blue);
	}
      /* draw a vertical line evry step */
      if (!(x % step))
	{
	  ho_pixbuf_set (m, x1 + x, y1 - 4, 0, red);
	  ho_pixbuf_set (m, x1 + x, y1 - 3, 0, red);
	  ho_pixbuf_set (m, x1 + x, y1 - 2, 0, red);
	  ho_pixbuf_set (m, x1 + x, y1 + 2, 0, red);
	  ho_pixbuf_set (m, x1 + x, y1 + 3, 0, red);
	  ho_pixbuf_set (m, x1 + x, y1 + 4, 0, red);

	  if (m->n_channels >= 3)
	    {
	      ho_pixbuf_set (m, x1 + x, y1 - 4, 1, green);
	      ho_pixbuf_set (m, x1 + x, y1 - 3, 1, green);
	      ho_pixbuf_set (m, x1 + x, y1 - 2, 1, green);
	      ho_pixbuf_set (m, x1 + x, y1 + 2, 1, green);
	      ho_pixbuf_set (m, x1 + x, y1 + 3, 1, green);
	      ho_pixbuf_set (m, x1 + x, y1 + 4, 1, green);

	      ho_pixbuf_set (m, x1 + x, y1 - 4, 2, blue);
	      ho_pixbuf_set (m, x1 + x, y1 - 3, 2, blue);
	      ho_pixbuf_set (m, x1 + x, y1 - 2, 2, blue);
	      ho_pixbuf_set (m, x1 + x, y1 + 2, 2, blue);
	      ho_pixbuf_set (m, x1 + x, y1 + 3, 2, blue);
	      ho_pixbuf_set (m, x1 + x, y1 + 4, 2, blue);
	    }
	}
    }

  return HO_FALSE;
}

int
ho_pixbuf_draw_vertical_scale (ho_pixbuf * m, const ho_uint x1,
			       const ho_uint y1, const ho_uint length,
			       const ho_uint step, const ho_uchar red,
			       const ho_uchar green, const ho_uchar blue)
{
  ho_uint y;

  for (y = 0; y <= length; y++)
    {
      ho_pixbuf_set (m, x1, y1 + y, 0, red);
      if (m->n_channels >= 3)
	{
	  ho_pixbuf_set (m, x1, y1 + y, 1, green);
	  ho_pixbuf_set (m, x1, y1 + y, 2, blue);
	}
      /* draw a vertical line evry step */
      if (!(y % step))
	{
	  ho_pixbuf_set (m, x1 - 4, y1 + y, 0, red);
	  ho_pixbuf_set (m, x1 - 3, y1 + y, 0, red);
	  ho_pixbuf_set (m, x1 - 2, y1 + y, 0, red);
	  ho_pixbuf_set (m, x1 + 2, y1 + y, 0, red);
	  ho_pixbuf_set (m, x1 + 3, y1 + y, 0, red);
	  ho_pixbuf_set (m, x1 + 4, y1 + y, 0, red);

	  if (m->n_channels >= 3)
	    {
	      ho_pixbuf_set (m, x1 - 4, y1 + y, 1, green);
	      ho_pixbuf_set (m, x1 - 3, y1 + y, 1, green);
	      ho_pixbuf_set (m, x1 - 2, y1 + y, 1, green);
	      ho_pixbuf_set (m, x1 + 2, y1 + y, 1, green);
	      ho_pixbuf_set (m, x1 + 3, y1 + y, 1, green);
	      ho_pixbuf_set (m, x1 + 4, y1 + y, 1, green);

	      ho_pixbuf_set (m, x1 - 4, y1 + y, 2, blue);
	      ho_pixbuf_set (m, x1 - 3, y1 + y, 2, blue);
	      ho_pixbuf_set (m, x1 - 2, y1 + y, 2, blue);
	      ho_pixbuf_set (m, x1 + 2, y1 + y, 2, blue);
	      ho_pixbuf_set (m, x1 + 3, y1 + y, 2, blue);
	      ho_pixbuf_set (m, x1 + 4, y1 + y, 2, blue);
	    }
	}
    }

  return HO_FALSE;
}

int
ho_pixbuf_draw_grid (ho_pixbuf * m, const ho_uint size, const ho_uint step,
		     const ho_uchar red, const ho_uchar green,
		     const ho_uchar blue)
{
  ho_uint i;

  /* check fot matrix size */
  if (m->width < 120 || m->height < 120)
    return HO_TRUE;

  /* adding  grid */
  for (i = 60; i < (m->width - 60); i += size)
    {
      ho_pixbuf_draw_vertical_scale (m, i, 30, m->height - 60, step, red,
				     green, blue);
    }

  for (i = 60; i < (m->height - 60); i += size)
    {
      ho_pixbuf_draw_horizontal_scale (m, 30, i, m->width - 60, step, red,
				       green, blue);
    }

  return HO_FALSE;
}

ho_pixbuf *
ho_pixbuf_linear_filter (const ho_pixbuf * pix)
{
  ho_pixbuf *pix_gray = HO_NULL;
  ho_uint x, y;
  ho_uchar min, max;

  /* is input gray ? */
  if (pix->n_channels != 1)
    return HO_NULL;

  /* allocate memory */
  pix_gray = ho_pixbuf_new (1, pix->width, pix->height, 0);
  if (!pix_gray)
    return HO_NULL;

  ho_pixbuf_minmax (pix, &min, &max);
  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
      {
	(pix_gray->data)[x + y * pix_gray->rowstride] =
	  255 * ((pix->data)[x + y * pix->rowstride] - min) / (max - min);
      }

  return pix_gray;
}

ho_bitmap *
ho_pixbuf_to_bitmap (const ho_pixbuf * pix, ho_uchar threshold)
{
  ho_bitmap *m_out = HO_NULL;
  ho_uint x, y;

  /* convert threshold from 0..100 to 0..255 */
  threshold = 255 * threshold / 100;

  /* is input gray ? */
  if (pix->n_channels != 1)
    return HO_NULL;
  if (!threshold)
    threshold = 153;

  /* allocate memory */
  m_out = ho_bitmap_new (pix->width, pix->height);
  if (!m_out)
    return HO_NULL;

  /* copy data */
  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
      if ((pix->data)[x + y * pix->rowstride] < threshold)
	ho_bitmap_set (m_out, x, y);

  return m_out;
}

ho_bitmap *
ho_pixbuf_to_bitmap_adaptive (const ho_pixbuf *
			      pix,
			      ho_uchar threshold,
			      ho_uchar size, ho_uchar adaptive_threshold)
{
  ho_bitmap *m_out = HO_NULL;
  ho_uint x, y;
  int i, j;
  ho_uchar locale_thereshold;
  double sum;
  double hlf_size = size / 2.0;
  double size_aquare = size * size;
  double factor = (double) adaptive_threshold / 100.0;

  /* convert threshold from 0..100 to 0..255 */
  threshold = 255 * threshold / 100;

  /* is input gray ? */
  if (pix->n_channels != 1)
    return HO_NULL;

  /* set defaults */
  if (!threshold)
    threshold = 0;

  if (!factor)
    factor = 6.0 / 8.0;

  if (!size)
    {
      size = 70;
      hlf_size = size / 2.0;
      size_aquare = size * size;
    }

  /* allocate memory */
  m_out = ho_bitmap_new (pix->width, pix->height);
  if (!m_out)
    return HO_NULL;

  /* init threshold matrix to 0.5 */
  for (x = 0; x < hlf_size; x++)
    for (y = 0; y < pix->height; y++)
      if (ho_pixbuf_get (pix, x, y, 0) < 127)
	ho_bitmap_set (m_out, x, y);
  for (x = pix->width - size; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
      if (ho_pixbuf_get (pix, x, y, 0) < 127)
	ho_bitmap_set (m_out, x, y);
  for (x = 0; x < pix->width; x++)
    for (y = 0; y < hlf_size; y++)
      if (ho_pixbuf_get (pix, x, y, 0) < 127)
	ho_bitmap_set (m_out, x, y);
  for (x = 0; x < pix->width; x++)
    for (y = pix->height - size; y < pix->height; y++)
      if (ho_pixbuf_get (pix, x, y, 0) < 127)
	ho_bitmap_set (m_out, x, y);

  /* copy data */
  for (x = hlf_size; x < pix->width - hlf_size; x += hlf_size)
    for (y = hlf_size; y < pix->height - hlf_size; y += hlf_size)
      {
	/* get avg */
	sum = 0;
	for (i = -hlf_size; i < hlf_size; i++)
	  for (j = -hlf_size; j < hlf_size; j++)
	    {
	      sum += ho_pixbuf_get (pix, x + i, y + j, 0);
	    }

	/* calculate locale threshold */
	locale_thereshold = (sum / size_aquare) * factor;
	if (locale_thereshold < threshold)
	  locale_thereshold = threshold;

	/* do copy */
	for (i = -hlf_size; i < 0; i++)
	  for (j = -hlf_size; j < 0; j++)
	    if (ho_pixbuf_get (pix, x + i, y + j, 0) < locale_thereshold)
	      ho_bitmap_set (m_out, x + i, y + j);
      }

  return m_out;
}

ho_bitmap *
ho_pixbuf_to_bitmap_adaptive_best (const ho_pixbuf *
				   pix,
				   ho_uchar threshold,
				   ho_uchar size, ho_uchar adaptive_threshold)
{
  ho_pixbuf *m_thresholds = HO_NULL;
  ho_bitmap *m_out = HO_NULL;
  ho_uint x, y;
  int i, j;
  ho_uchar locale_thereshold;
  double sum, first_row, last_row;
  double hlf_size = size / 2.0;
  double size_aquare = size * size;
  double factor = (double) adaptive_threshold / 100.0;

  /* convert threshold from 0..100 to 0..255 */
  threshold = 255 * threshold / 100;

  /* is input gray ? */
  if (pix->n_channels != 1)
    return HO_NULL;

  /* set defaults */
  if (!threshold)
    threshold = 0;

  if (!factor)
    factor = 6.0 / 8.0;

  if (!size)
    {
      size = 70;
      hlf_size = size / 2.0;
      size_aquare = size * size;
    }

  /* allocate memory */
  m_out = ho_bitmap_new (pix->width, pix->height);
  if (!m_out)
    return HO_NULL;

  m_thresholds = ho_pixbuf_new (1, pix->width, pix->height, 0);
  if (!m_thresholds)
    {
      ho_bitmap_free (m_out);
      return HO_NULL;
    }

  /* init threshold matrix to 0.5 */
  for (x = 0; x < hlf_size; x++)
    for (y = 0; y < pix->height; y++)
      ho_pixbuf_set (m_thresholds, x, y, 0, 127);
  for (x = pix->width - hlf_size; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
      ho_pixbuf_set (m_thresholds, x, y, 0, 127);
  for (x = 0; x < pix->width; x++)
    for (y = 0; y < hlf_size; y++)
      ho_pixbuf_set (m_thresholds, x, y, 0, 127);
  for (x = 0; x < pix->width; x++)
    for (y = pix->height - hlf_size; y < pix->height; y++)
      ho_pixbuf_set (m_thresholds, x, y, 0, 127);

  /* create threshold matix */
  for (x = hlf_size; x < pix->width - hlf_size; x++)
    {
      /* calculate sum of first erea */
      y = hlf_size;
      sum = 0;
      for (i = -hlf_size; i < hlf_size; i++)
	for (j = -hlf_size; j < hlf_size; j++)
	  {
	    sum += (pix->data)[(x + i) + (y + j) * pix->rowstride];
	  }

      /* get the sum of elemnts in first row of area */
      first_row = 0;
      for (i = -hlf_size; i < hlf_size; i++)
	{
	  first_row +=
	    (pix->data)[(x + i) + (y - (int) hlf_size) * pix->rowstride];
	}
      last_row = 0;
      for (i = -hlf_size; i < hlf_size; i++)
	{
	  last_row +=
	    (pix->data)[(x + i) + (y + (int) hlf_size) * pix->rowstride];
	}

      /* calculate locale threshold */
      locale_thereshold = (sum / size_aquare) * factor;
      if (locale_thereshold < threshold)
	ho_pixbuf_set (m_thresholds, x, y, 0, threshold);
      else
	ho_pixbuf_set (m_thresholds, x, y, 0, locale_thereshold);

      /* run rest of the height */
      for (y = hlf_size + 1; y < pix->height - (int) hlf_size; y++)
	{
	  /* get avg */
	  sum += last_row - first_row;
	  first_row = 0;
	  for (i = -hlf_size; i < hlf_size; i++)
	    {
	      first_row +=
		(pix->data)[(x + i) + (y - (int) hlf_size) * pix->rowstride];
	    }
	  last_row = 0;
	  for (i = -hlf_size; i < hlf_size; i++)
	    {
	      last_row +=
		(pix->data)[(x + i) + (y + (int) hlf_size) * pix->rowstride];
	    }

	  /* calculate locale threshold */
	  locale_thereshold = (sum / size_aquare) * factor;
	  if (locale_thereshold < threshold)
	    ho_pixbuf_set (m_thresholds, x, y, 0, threshold);
	  else
	    ho_pixbuf_set (m_thresholds, x, y, 0, locale_thereshold);
	}
    }

  /* do thresholding */
  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
      if (ho_pixbuf_get (pix, x, y, 0) <
	  ho_pixbuf_get (m_thresholds, x, y, 0))
	ho_bitmap_set (m_out, x, y);

  ho_pixbuf_free (m_thresholds);

  return m_out;
}


ho_bitmap *
ho_pixbuf_to_bitmap_wrapper (const ho_pixbuf * pix_in, const ho_uchar scale,
			     const ho_uchar adaptive,
			     const ho_uchar threshold,
			     const ho_uchar a_threshold)
{
  ho_pixbuf *pix = HO_NULL;
  ho_pixbuf *pix_temp = HO_NULL;
  ho_bitmap *m_bw = HO_NULL;
  ho_uchar nead_to_free_pix = HO_FALSE;

  /* if pix is color convert to gray scale */
  if (pix_in->n_channels > 1)
    pix = ho_pixbuf_color_to_gray (pix_in);
  else
    pix = ho_pixbuf_clone (pix_in);

  if (!pix)
    return HO_NULL;

  /* streach picture grays */
  pix_temp = ho_pixbuf_linear_filter (pix);
  if (pix_temp)
    {
      ho_pixbuf_free (pix);
      pix = pix_temp;
    }

  /* scale */
  if (scale)
    {
      pix_temp = ho_pixbuf_scale (pix, scale);
      if (pix_temp)
	{
	  ho_pixbuf_free (pix);
	  pix = pix_temp;
	}
    }

  /* convert to b/w bitmap */
  switch (adaptive)
    {
    case 0:
      m_bw = ho_pixbuf_to_bitmap_adaptive (pix, threshold, 0, a_threshold);
      break;
    case 1:
      m_bw = ho_pixbuf_to_bitmap (pix, threshold);
      break;
    case 2:
      m_bw =
	ho_pixbuf_to_bitmap_adaptive_best (pix, threshold, 0, a_threshold);
      break;
    default:
      m_bw = ho_pixbuf_to_bitmap_adaptive (pix, threshold, 0, a_threshold);
      break;
    }

  ho_pixbuf_free (pix);

  return m_bw;
}
