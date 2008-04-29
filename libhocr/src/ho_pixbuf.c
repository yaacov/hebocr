
/***************************************************************************
 *            ho_pixbuf.c
 *
 *  Fri Aug 12 20:13:33 2005
 *  Copyright  2005-2007  Yaacov Zamir
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
#include <math.h>

#ifdef USE_TIFF
#include <tiffio.h>
#endif

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
#include "ho_pixbuf.h"

ho_pixbuf *
ho_pixbuf_new (const unsigned char n_channels,
  const int width, const int height, const int rowstride)
{
  ho_pixbuf *pix = NULL;

  /* 
   * allocate memory for pixbuf 
   */
  pix = (ho_pixbuf *) malloc (sizeof (ho_pixbuf));
  if (!pix)
  {
    return NULL;
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
    return NULL;
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
    return NULL;

  /* copy data */
  memcpy (m_out->data, m->data, m_out->height * m_out->rowstride);

  return m_out;
}

int
ho_pixbuf_set_data (ho_pixbuf * pix, const char *data)
{
  /* copy the pixels */
  memcpy (pix->data, data, (pix->height * pix->rowstride));

  return FALSE;
};

string_data
ho_pixbuf_get_data_string (ho_pixbuf * pix)
{
  string_data out;

  out.data = (char *) malloc (pix->height * pix->rowstride);

  if (!out.data)
  {
    out.data = NULL;
    out.size = 0;
  }
  else
  {
    memcpy (out.data, pix->data, (pix->height * pix->rowstride));

    out.data = pix->data;
    out.size = pix->height * pix->rowstride;
  }

  return out;
}

ho_pixbuf *
ho_pixbuf_new_from_bitmap (const ho_bitmap * bit_in)
{
  int x, y;
  ho_pixbuf *pix = NULL;

  pix = ho_pixbuf_new (1, bit_in->width, bit_in->height, 0);
  if (!pix)
    return NULL;

  for (x = 0; x < bit_in->width; x++)
    for (y = 0; y < bit_in->height; y++)
      ho_pixbuf_set (pix, x, y, 0, 255 * (1 - ho_bitmap_get (bit_in, x, y)));

  return pix;
}

ho_pixbuf *
ho_pixbuf_new_from_objmap (const ho_objmap * obj_in, const unsigned char min,
  const unsigned char max)
{
  int x, y;
  ho_pixbuf *pix = NULL;
  int index = 0;

  /* allocate memory */
  pix = ho_pixbuf_new (3, obj_in->width, obj_in->height, 0);
  if (!pix)
    return NULL;

  /* copy pixels from gsl mtirxes */
  for (x = 0; x < obj_in->width; x++)
    for (y = 0; y < obj_in->height; y++)
    {
      index = ho_objmap_get (obj_in, x, y);

      if (index)
      {
        /* set autput colors */
        ho_pixbuf_set (pix, x, y, 0, min + (max - min) * (index % 12) / 12);
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
ho_pixbuf_new_from_layout (const ho_layout * lay_in,
  const unsigned char show_grid, const ho_bitmap * m_text,
  const unsigned char text_block_r, const unsigned char text_block_g,
  const unsigned char text_block_b, const unsigned char text_block_a,
  const unsigned char text_block_frame_width,
  const unsigned char line_block_r, const unsigned char line_block_g,
  const unsigned char line_block_b, const unsigned char line_block_a,
  const unsigned char line_block_frame_width,
  const unsigned char word_block_r, const unsigned char word_block_g,
  const unsigned char word_block_b, const unsigned char word_block_a,
  const unsigned char word_block_frame_width,
  const unsigned char font_block_r, const unsigned char font_block_g,
  const unsigned char font_block_b, const unsigned char font_block_a,
  const unsigned char font_block_frame_width)
{
  ho_pixbuf *pix_out = NULL;
  ho_bitmap *m_block_frame = NULL;
  ho_bitmap *m_word_text = NULL;
  ho_bitmap *m_word_mask = NULL;
  ho_bitmap *m_word_font_mask = NULL;
  int block_index;
  int line_index;
  int word_index;

  /* allocate */
  pix_out =
    ho_pixbuf_new (3, lay_in->m_page_blocks_mask->width,
    lay_in->m_page_blocks_mask->height, 0);

  /* add text blocks */
  m_block_frame =
    ho_bitmap_edge (lay_in->m_page_blocks_mask, text_block_frame_width);
  ho_pixbuf_draw_bitmap (pix_out, m_block_frame, text_block_r, text_block_g,
    text_block_b, text_block_a);
  ho_bitmap_free (m_block_frame);

  /* loop on all text blocks */
  for (block_index = 0; block_index < lay_in->n_blocks; block_index++)
  {
    for (line_index = 0; line_index < lay_in->n_lines[block_index];
      line_index++)
    {
      /* draw words */
      ho_pixbuf_draw_bitmap (pix_out,
        lay_in->m_lines_words_mask[block_index][line_index], word_block_r,
        word_block_g, word_block_b, word_block_a);

      /* draw lines */
      m_block_frame =
        ho_bitmap_edge (lay_in->m_lines_line_mask[block_index][line_index],
        line_block_frame_width);

      ho_pixbuf_draw_bitmap (pix_out, m_block_frame, line_block_r,
        line_block_g, line_block_b, line_block_a);

      ho_bitmap_free (m_block_frame);

      /* draw fonts */
      for (word_index = 0;
        word_index < lay_in->n_words[block_index][line_index]; word_index++)
      {
        m_word_font_mask =
          lay_in->m_words_font_mask[block_index][line_index][word_index];

        ho_pixbuf_draw_bitmap (pix_out, m_word_font_mask, font_block_r,
          font_block_g, font_block_b, font_block_a);
      }
    }
  }

  /* add grid */
  if (show_grid)
    ho_pixbuf_draw_grid (pix_out, 120, 30, 255, 0, 0);

  /* add text */
  if (m_text)
    ho_pixbuf_draw_bitmap (pix_out, m_text, 0, 0, 0, 255);

  return pix_out;
}

ho_pixbuf *
ho_pixbuf_to_rgb (const ho_pixbuf * pix_in)
{
  int x, y;
  ho_pixbuf *pix = NULL;
  unsigned char red, green, blue;

  /* allocate memory */
  pix = ho_pixbuf_new (3, pix_in->width, pix_in->height, 0);
  if (!pix)
    return NULL;

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
    return TRUE;

  if (pix->data)
    free (pix->data);

  free (pix);

  return FALSE;
}

ho_pixbuf *
ho_pixbuf_color_to_gray (const ho_pixbuf * pix)
{
  ho_pixbuf *pix_gray = NULL;
  int x, y;
  unsigned char red, green, blue;

  /* does input has the rgb channels ? */
  if (pix->n_channels < 3)
    return NULL;

  /* allocate memory */
  pix_gray = ho_pixbuf_new (1, pix->width, pix->height, 0);
  if (!pix_gray)
    return NULL;

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
ho_pixbuf_color_to_red (const ho_pixbuf * pix)
{
  ho_pixbuf *pix_red = NULL;
  int x, y;
  unsigned char red;

  /* does input has the rgb channels ? */
  if (pix->n_channels < 3)
    return NULL;

  /* allocate memory */
  pix_red = ho_pixbuf_new (1, pix->width, pix->height, 0);
  if (!pix_red)
    return NULL;

  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
    {
      red = ho_pixbuf_get (pix, x, y, 0);

      ho_pixbuf_set (pix_red, x, y, 0, red);
    }

  return pix_red;
}

ho_pixbuf *
ho_pixbuf_color_to_green (const ho_pixbuf * pix)
{
  ho_pixbuf *pix_green = NULL;
  int x, y;
  unsigned char green;

  /* does input has the rgb channels ? */
  if (pix->n_channels < 3)
    return NULL;

  /* allocate memory */
  pix_green = ho_pixbuf_new (1, pix->width, pix->height, 0);
  if (!pix_green)
    return NULL;

  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
    {
      green = ho_pixbuf_get (pix, x, y, 1);

      ho_pixbuf_set (pix_green, x, y, 0, green);
    }

  return pix_green;
}

ho_pixbuf *
ho_pixbuf_color_to_blue (const ho_pixbuf * pix)
{
  ho_pixbuf *pix_blue = NULL;
  int x, y;
  unsigned char blue;

  /* does input has the rgb channels ? */
  if (pix->n_channels < 3)
    return NULL;

  /* allocate memory */
  pix_blue = ho_pixbuf_new (1, pix->width, pix->height, 0);
  if (!pix_blue)
    return NULL;

  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
    {
      blue = ho_pixbuf_get (pix, x, y, 2);

      ho_pixbuf_set (pix_blue, x, y, 0, blue);
    }

  return pix_blue;
}

ho_pixbuf *
ho_pixbuf_scale2 (const ho_pixbuf * pix)
{
  ho_pixbuf *pix_scaled = NULL;
  int x, y;
  unsigned char neighbors[4];

  /* is input gray ? */
  if (pix->n_channels != 1 || pix->width < 3 || pix->height < 3)
    return NULL;

  /* allocate memory */
  pix_scaled = ho_pixbuf_new (1, pix->width * 2 - 2, pix->height * 2 - 2, 0);
  if (!pix_scaled)
    return NULL;

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
  ho_pixbuf *pix_scaled = NULL;
  int x, y;
  unsigned char neighbors[4];

  /* is input gray ? */
  if (pix->n_channels != 1 || pix->width < 3 || pix->height < 3)
    return NULL;

  /* allocate memory */
  pix_scaled = ho_pixbuf_new (1, pix->width * 3 - 3, pix->height * 3 - 3, 0);
  if (!pix_scaled)
    return NULL;

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
ho_pixbuf_scale4 (const ho_pixbuf * pix)
{
  ho_pixbuf *pix_scaled2 = NULL;
  ho_pixbuf *pix_scaled4 = NULL;

  pix_scaled2 = ho_pixbuf_scale2 (pix);
  if (!pix_scaled2)
    return NULL;

  pix_scaled4 = ho_pixbuf_scale2 (pix_scaled2);
  ho_pixbuf_free (pix_scaled2);

  return pix_scaled4;
}

ho_pixbuf *
ho_pixbuf_scale (const ho_pixbuf * pix, const unsigned char scale)
{
  /* is input sane ? */
  if (scale < 2 || pix->n_channels != 1 || pix->width < 3 || pix->height < 3)
    return NULL;

  if (scale == 2)
    return ho_pixbuf_scale2 (pix);

  if (scale == 3)
    return ho_pixbuf_scale3 (pix);

  /* FIXME: scale more them 4 ? */
  if (scale >= 4)
    return ho_pixbuf_scale4 (pix);

  return NULL;
}

unsigned char
ho_pixbuf_minmax (const ho_pixbuf * pix, unsigned char *min, unsigned char *max)
{
  int x, y;

  *min = 0;
  *max = 255;

  /* is input gray ? */
  if (pix->n_channels != 1)
    return TRUE;

  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++)
    {
      if (*min > (pix->data)[x + y * pix->rowstride])
        *min = (pix->data)[x + y * pix->rowstride];
      if (*max < (pix->data)[x + y * pix->rowstride])
        *max = (pix->data)[x + y * pix->rowstride];
    }

  return FALSE;
}

ho_pixbuf *
ho_pixbuf_linear_filter (const ho_pixbuf * pix)
{
  ho_pixbuf *pix_gray = NULL;
  int x, y;
  unsigned char min, max;

  /* is input gray ? */
  if (pix->n_channels != 1)
    return NULL;

  /* allocate memory */
  pix_gray = ho_pixbuf_new (1, pix->width, pix->height, 0);
  if (!pix_gray)
    return NULL;

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
ho_pixbuf_to_bitmap (const ho_pixbuf * pix, unsigned char threshold)
{
  ho_pixbuf * gray_pix = NULL;
  ho_bitmap *m_out = NULL;
  int x, y;

  /* convert threshold from 0..100 to 0..255 */
  threshold = 255 * threshold / 100;

  /* if pix is color convert to gray scale */
  if (pix->n_channels > 1)
    gray_pix = ho_pixbuf_color_to_gray (pix);
  else
    gray_pix = ho_pixbuf_clone (pix);
  
  if (!threshold)
    threshold = 153;

  /* allocate memory */
  m_out = ho_bitmap_new (gray_pix->width, gray_pix->height);
  if (!m_out)
    return NULL;

  /* copy data */
  for (x = 0; x < gray_pix->width; x++)
    for (y = 0; y < gray_pix->height; y++)
      if ((gray_pix->data)[x + y * gray_pix->rowstride] < threshold)
        ho_bitmap_set (m_out, x, y);

  /* free gray pix */
  ho_pixbuf_free (gray_pix);
  
  return m_out;
}

ho_bitmap *
ho_pixbuf_to_bitmap_adaptive (const ho_pixbuf *
  pix,
  unsigned char threshold, unsigned char size, unsigned char adaptive_threshold)
{
  ho_bitmap *m_out = NULL;
  int x, y;
  int i, j;
  unsigned char locale_thereshold;
  double sum;
  int hlf_size = size / 2;
  double size_aquare = size * size;
  double factor = (double) adaptive_threshold / 100.0;

  /* convert threshold from 0..100 to 0..255 */
  threshold = 255 * threshold / 100;

  /* is input gray ? */
  if (pix->n_channels != 1)
    return NULL;

  /* set defaults */
  if (!threshold)
    threshold = 0;

  if (!factor)
    factor = 6.0 / 8.0;

  if (!size)
  {
    size = 70;
    hlf_size = size / 2;
    size_aquare = size * size;
  }

  /* allocate memory */
  m_out = ho_bitmap_new (pix->width, pix->height);
  if (!m_out)
    return NULL;

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
          sum += ho_pixbuf_get (pix, x + i, y + j, 0);

      /* calculate locale threshold */
      locale_thereshold = (unsigned char) (factor * sum / size_aquare);
      if (locale_thereshold < threshold)
        locale_thereshold = threshold;

      for (i = -hlf_size; i < 0; i++)
        for (j = -hlf_size; j < 0; j++)
          if (ho_pixbuf_get (pix, x + i, y + j, 0) < locale_thereshold)
            ho_bitmap_set (m_out, x + i, y + j);

    }

  return m_out;
}

ho_bitmap *
ho_pixbuf_to_bitmap_adaptive_fine (const ho_pixbuf *
  pix,
  unsigned char threshold, unsigned char size, unsigned char adaptive_threshold)
{
  ho_pixbuf *m_thresholds = NULL;
  ho_bitmap *m_out = NULL;
  int x, y;
  int i, j;
  unsigned char locale_thereshold;
  double sum, first_row, last_row;
  double hlf_size = size / 2.0;
  double size_aquare = size * size;
  double factor = (double) adaptive_threshold / 100.0;

  /* convert threshold from 0..100 to 0..255 */
  threshold = 255 * threshold / 100;

  /* is input gray ? */
  if (pix->n_channels != 1)
    return NULL;

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
    return NULL;

  m_thresholds = ho_pixbuf_new (1, pix->width, pix->height, 0);
  if (!m_thresholds)
  {
    ho_bitmap_free (m_out);
    return NULL;
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
      first_row += (pix->data)[(x + i) + (y - (int) hlf_size) * pix->rowstride];
    }
    last_row = 0;
    for (i = -hlf_size; i < hlf_size; i++)
    {
      last_row += (pix->data)[(x + i) + (y + (int) hlf_size) * pix->rowstride];
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
      if (ho_pixbuf_get (pix, x, y, 0) < ho_pixbuf_get (m_thresholds, x, y, 0))
        ho_bitmap_set (m_out, x, y);

  ho_pixbuf_free (m_thresholds);

  return m_out;
}

ho_bitmap *
ho_pixbuf_to_bitmap_wrapper (const ho_pixbuf * pix_in,
  const unsigned char scale,
  const unsigned char adaptive,
  const unsigned char threshold, const unsigned char a_threshold)
{
  ho_pixbuf *pix = NULL;
  ho_pixbuf *pix_temp = NULL;
  ho_bitmap *m_bw = NULL;
  unsigned char nead_to_free_pix = FALSE;

  /* if pix is color convert to gray scale */
  if (pix_in->n_channels > 1)
    pix = ho_pixbuf_color_to_gray (pix_in);
  else
    pix = ho_pixbuf_clone (pix_in);

  if (!pix)
    return NULL;

  /* streach picture grays */
  pix_temp = ho_pixbuf_linear_filter (pix);
  ho_pixbuf_free (pix);
  if (!pix_temp)
    return NULL;
  pix = pix_temp;

  /* scale */
  if (scale > 1)
  {
    pix_temp = ho_pixbuf_scale (pix, scale);
    ho_pixbuf_free (pix);
    if (!pix_temp)
      return NULL;
    pix = pix_temp;
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
    m_bw = ho_pixbuf_to_bitmap_adaptive_fine (pix, threshold, 0, a_threshold);
    break;
  default:
    m_bw = ho_pixbuf_to_bitmap_adaptive (pix, threshold, 0, a_threshold);
    break;
  }

  ho_pixbuf_free (pix);

  return m_bw;
}

int
ho_pixbuf_draw_bitmap (ho_pixbuf * m, const ho_bitmap * bit_in,
  const unsigned char red, const unsigned char green,
  const unsigned char blue, const unsigned char alpha)
{
  int x, y;
  unsigned char new_red;
  unsigned char new_green;
  unsigned char new_blue;

  /* check for bitmap origin, if this is a sub window draw at its origin */
  if (bit_in->x != 0 || bit_in->y != 0 || m->width > bit_in->width
    || m->height > bit_in->height)
    ho_pixbuf_draw_bitmap_at (m, bit_in, bit_in->x, bit_in->y, red, green,
      blue, alpha);

  /* sanity check */
  if (m->width != bit_in->width || m->height != bit_in->height)
    return TRUE;

  /* is pixbuf color ? */
  if (m->n_channels < 3)
  {                             /* gray scale */
    for (x = 0; x < bit_in->width; x++)
      for (y = 0; y < bit_in->height; y++)
        if (ho_bitmap_get (bit_in, x, y))
        {
          new_red =
            alpha * red / 255 + (255 - alpha) * ho_pixbuf_get (m, x, y,
            0) / 255;
          ho_pixbuf_set (m, x, y, 0, new_red);
        }
  }
  else
  {                             /* color */
    for (x = 0; x < bit_in->width; x++)
      for (y = 0; y < bit_in->height; y++)
        if (ho_bitmap_get (bit_in, x, y))
        {
          new_red =
            alpha * red / 255 + (255 - alpha) * ho_pixbuf_get (m, x, y,
            0) / 255;
          new_green =
            alpha * green / 255 + (255 - alpha) * ho_pixbuf_get (m, x, y,
            1) / 255;
          new_blue =
            alpha * blue / 255 + (255 - alpha) * ho_pixbuf_get (m, x, y,
            2) / 255;

          ho_pixbuf_set (m, x, y, 0, new_red);
          ho_pixbuf_set (m, x, y, 1, new_green);
          ho_pixbuf_set (m, x, y, 2, new_blue);
        }
  }

  return FALSE;
}

int
ho_pixbuf_draw_bitmap_at (ho_pixbuf * m, const ho_bitmap * bit_in,
  const int x1, const int y1,
  const unsigned char red, const unsigned char green,
  const unsigned char blue, const unsigned char alpha)
{
  int x, y;
  unsigned char new_red;
  unsigned char new_green;
  unsigned char new_blue;

  /* is pixbuf color ? */
  if (m->n_channels < 3)
  {                             /* gray scale */
    for (x = 0; x < bit_in->width; x++)
      for (y = 0; y < bit_in->height; y++)
        if ((x + x1) >= 0 && (x + x1) < m->width && (y + y1) >= 0
          && (y + y1) < m->height)
          if (ho_bitmap_get (bit_in, x, y))
          {
            new_red =
              alpha * red / 255 + (255 - alpha) * ho_pixbuf_get (m,
              (x + x1), (y + y1), 0) / 255;
            ho_pixbuf_set (m, (x + x1), (y + y1), 0, new_red);
          }
  }
  else
  {                             /* color */
    for (x = 0; x < bit_in->width; x++)
      for (y = 0; y < bit_in->height; y++)
        if ((x + x1) >= 0 && (x + x1) < m->width && (y + y1) >= 0
          && (y + y1) < m->height)
          if (ho_bitmap_get (bit_in, x, y))
          {
            new_red =
              alpha * red / 255 + (255 - alpha) * ho_pixbuf_get (m,
              (x + x1), (y + y1), 0) / 255;
            new_green =
              alpha * green / 255 + (255 - alpha) * ho_pixbuf_get (m,
              (x + x1), (y + y1), 1) / 255;
            new_blue =
              alpha * blue / 255 + (255 - alpha) * ho_pixbuf_get (m,
              (x + x1), (y + y1), 2) / 255;

            ho_pixbuf_set (m, (x + x1), (y + y1), 0, new_red);
            ho_pixbuf_set (m, (x + x1), (y + y1), 1, new_green);
            ho_pixbuf_set (m, (x + x1), (y + y1), 2, new_blue);
          }
  }

  return FALSE;
}

int
ho_pixbuf_draw_line (ho_pixbuf * m, const int x1, const int y1,
  const int x2, const int y2, const unsigned char red,
  const unsigned char green, const unsigned char blue)
{
  double x, step_x;
  double y, step_y;
  int x_start = x1;
  int x_end = x2;
  int y_start = y1;
  int y_end = y2;

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
      ((double) x_end - (double) x_start) / ((double) y_end - (double) y_start);
    for (y = y_start; y <= y_end; y++)
    {
      (m->data)[(int) x * m->n_channels + (int) y * m->rowstride] = red;
      if (m->n_channels >= 3)
      {
        (m->data)[(int) x * m->n_channels + (int) y * m->rowstride + 1] = green;
        (m->data)[(int) x * m->n_channels + (int) y * m->rowstride + 2] = blue;
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
      ((double) y_end - (double) y_start) / ((double) x_end - (double) x_start);
    for (x = x_start; x <= x_end; x++)
    {
      (m->data)[(int) x * m->n_channels + (int) y * m->rowstride] = red;
      if (m->n_channels >= 3)
      {
        (m->data)[(int) x * m->n_channels + (int) y * m->rowstride + 1] = green;
        (m->data)[(int) x * m->n_channels + (int) y * m->rowstride + 2] = blue;
      }
      y += step_y;
    }
  }

  return FALSE;
}

int
ho_pixbuf_draw_horizontal_scale (ho_pixbuf * m, const int x1,
  const int y1, const int length,
  const int step, const unsigned char red,
  const unsigned char green, const unsigned char blue)
{
  int x;

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

  return FALSE;
}

int
ho_pixbuf_draw_vertical_scale (ho_pixbuf * m, const int x1,
  const int y1, const int length,
  const int step, const unsigned char red,
  const unsigned char green, const unsigned char blue)
{
  int y;

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

  return FALSE;
}

int
ho_pixbuf_draw_grid (ho_pixbuf * m, const int size, const int step,
  const unsigned char red, const unsigned char green, const unsigned char blue)
{
  int i;

  /* check fot matrix size */
  if (m->width < 120 || m->height < 120)
    return TRUE;

  /* adding grid */
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

  return FALSE;
}

int
ho_pixbuf_draw_rgb_bitmap (ho_pixbuf * m, const ho_bitmap * bit_in_red,
  const ho_bitmap * bit_in_green, const ho_bitmap * bit_in_blue)
{
  int x, y;
  unsigned char new_red;
  unsigned char new_green;
  unsigned char new_blue;

  /* sanity check */
  if (m->width != bit_in_red->width || m->height != bit_in_red->height)
    return TRUE;

  /* is pixbuf color ? */
  if (m->n_channels < 3)
    return TRUE;

  for (x = 0; x < bit_in_red->width; x++)
    for (y = 0; y < bit_in_red->height; y++)
    {
      new_red = 255 * ho_bitmap_get (bit_in_red, x, y);
      new_green = 255 * ho_bitmap_get (bit_in_green, x, y);
      new_blue = 255 * ho_bitmap_get (bit_in_blue, x, y);

      ho_pixbuf_set (m, x, y, 0, new_red);
      ho_pixbuf_set (m, x, y, 1, new_green);
      ho_pixbuf_set (m, x, y, 2, new_blue);
    }

  return FALSE;
}

unsigned char
ho_pbm_getc (FILE * file)
{
  unsigned char ch;
  int comment = FALSE;

  do
  {
    ch = getc (file);
    if (ch == '\n')
      comment = FALSE;
    else if (ch == '#')
      comment = TRUE;
  }
  while (comment);

  return ch;
}

int
ho_pbm_getint (FILE * file)
{
  unsigned char ch;
  int i = 0;

  /* pass on white spaces */
  do
  {
    ch = ho_pbm_getc (file);
  }
  while (ch == ' ' || ch == '\n' || ch == '\t');

  /* read a number */
  do
  {
    i = (i * 10) + (ch - '0');
    ch = ho_pbm_getc (file);
  }
  while (ch >= '0' && ch <= '9');

  return i;
}

int
ho_pbm_getbit (FILE * file)
{

  static unsigned char byte = 0;
  static unsigned char mask = 0;
  int return_bit;

  /* if file = 0, reset mask */
  if (!file)
  {
    mask = 0;
    return 0;
  }

  /* if we need a new byte read it from file */
  if (mask == 0)
  {
    mask = 0x80;
    byte = getc (file);
  }

  /* get the bit */
  return_bit = (byte & mask) ? 0 : 255;

  /* update mask */
  mask >>= 1;

  return return_bit;
}

ho_pixbuf *
ho_pixbuf_pnm_load (const char *filename)
{
  char ch1, ch2;
  ho_pixbuf *pix = NULL;
  FILE *file = NULL;
  unsigned char use_stdin = 0;
  unsigned char n_channels = 0;
  unsigned char val = 0;
  int width = 0;
  int height = 0;
  int x, y, rowstride;
  int i;

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
      return NULL;
  }

  /* read magic number "P?" for pbm file */
  ch1 = ho_pbm_getc (file);
  ch2 = ho_pbm_getc (file);
  if (ch1 != 'P' || (ch2 != '6' && ch2 != '5' && ch2 != '4'))
  {
    /* bad magic */
    if (!use_stdin)
      fclose (file);
    return NULL;
  }

  /* read header */
  n_channels = ((ch2 == '6') ? 3 : 1);
  width = ho_pbm_getint (file);
  height = ho_pbm_getint (file);

  if (ch2 == '4')
  {
    /* read bites per pixel */

    /* create a new pixbuf */
    pix = ho_pixbuf_new (n_channels, width, height, 0);

    rowstride = 8 * (width / 8 + 1);
    if (pix)
      for (y = 0; y < height; y++)
      {
        /* reset mask to 0 on a new line */
        ho_pbm_getbit (0);

        /* get line data */
        for (x = 0; x < rowstride; x++)
        {
          val = ho_pbm_getbit (file);
          if (x < width)
            ho_pixbuf_set (pix, x, y, 0, val);
        }
      }
  }
  else
  {
    /* read bytes per pixel */

    /* check for color depth */
    if (ho_pbm_getint (file) > 255)
    {
      /* bad bits per pixel */
      if (!use_stdin)
        fclose (file);

      return NULL;
    }

    /* create a new pixbuf */
    pix = ho_pixbuf_new (n_channels, width, height, 0);

    if (pix)
      fread (pix->data, 1, pix->height * pix->rowstride, file);
  }

  if (!use_stdin)
    fclose (file);

  /* return the new pixbuf to user */
  return pix;
}

int
ho_pixbuf_pnm_save (const ho_pixbuf * pix, const char *filename)
{
  FILE *file = NULL;

  if (pix->n_channels != 3 && pix->n_channels != 1)
  {
    /* bad magic */
    return TRUE;
  }

  file = fopen (filename, "wb");

  if (!file)
    return TRUE;

  /* print header */
  fprintf (file, "P%c %d %d 255\n", ((pix->n_channels == 3) ? '6' : '5'),
    pix->width, pix->height);

  /* this might be a huge write... */
  fwrite (pix->data, 1, pix->height * pix->rowstride, file);
  fclose (file);

  return FALSE;
}

#ifdef USE_TIFF
ho_pixbuf *
ho_pixbuf_bw_tiff_load (const char *filename)
{
  /* This function is copied from a web tutorial in:
    http://www-128.ibm.com/developerworks/linux/library/l-libtiff/
  */
  int x, y;
  ho_pixbuf *pix = NULL;
    
  TIFF *image;
  uint16 photo, bps, spp, fillorder;
  uint32 width;
  uint32 height;
  uint32 rowstride;
  tsize_t stripSize;
  unsigned long imageOffset, result;
  int stripMax, stripCount;
  char *buffer, tempbyte;
  unsigned long bufferSize, count;
  
  /* Open the TIFF image */
  if((image = TIFFOpen(filename, "r")) == NULL){
    /* no file */
    return NULL;
  }
  
  /* Check that it is of a type that we support */
  if((TIFFGetField(image, TIFFTAG_BITSPERSAMPLE, &bps) == 0) || (bps != 1)){
    /* not a 1 bit image */
    TIFFClose(image);
    return NULL;
  }
  
  if((TIFFGetField(image, TIFFTAG_SAMPLESPERPIXEL, &spp) == 0) || (spp != 1)){
    /* not a black and white image */
    TIFFClose(image);
    return NULL;
  }
  
  /* Read in the possibly multiple strips */
  stripSize = TIFFStripSize (image);
  stripMax = TIFFNumberOfStrips (image);
  imageOffset = 0;
  
  bufferSize = TIFFNumberOfStrips (image) * stripSize;
  if((buffer = (char *) malloc(bufferSize)) == NULL){
    /* not memory */
    TIFFClose(image);
    return NULL;
  }
  
  for (stripCount = 0; stripCount < stripMax; stripCount++){
    if((result = TIFFReadEncodedStrip (image, stripCount,
				      buffer + imageOffset,
				      stripSize)) == -1){
      /* read error */
      free (buffer);
      TIFFClose(image);
      return NULL;
    }

    imageOffset += result;
  }
  
  /* Deal with photometric interpretations */
  if(TIFFGetField(image, TIFFTAG_PHOTOMETRIC, &photo) == 0){
    /* we can't know if black is black or white :-( */
    /* asume min is white */
    photo = PHOTOMETRIC_MINISWHITE;
    return NULL;
  }
  
  if(photo != PHOTOMETRIC_MINISWHITE){
    /* Flip bits */
    for(count = 0; count < bufferSize; count++)
      buffer[count] = ~buffer[count];
  }
  
  /* Deal with fillorder */
  if(TIFFGetField(image, TIFFTAG_FILLORDER, &fillorder) == 0){
    /* we can't know if bits are msb or lsb ordered :-( */
    /* asume msb */
    fillorder = FILLORDER_MSB2LSB;
  }
  
  if(fillorder != FILLORDER_MSB2LSB){
    /* We need to swap bits -- ABCDEFGH becomes HGFEDCBA */
    for(count = 0; count < bufferSize; count++){
      tempbyte = 0;
      if(buffer[count] & 128) tempbyte += 1;
      if(buffer[count] & 64) tempbyte += 2;
      if(buffer[count] & 32) tempbyte += 4;
      if(buffer[count] & 16) tempbyte += 8;
      if(buffer[count] & 8) tempbyte += 16;
      if(buffer[count] & 4) tempbyte += 32;
      if(buffer[count] & 2) tempbyte += 64;
      if(buffer[count] & 1) tempbyte += 128;
      buffer[count] = tempbyte;
    }
  }
  
  /* get image width */
  if(TIFFGetField(image, TIFFTAG_IMAGEWIDTH, &width) == 0){
    /* we can't know image width :-( */
    free (buffer);
    TIFFClose(image);
    return NULL;
  }
  /* get image height */
  if(TIFFGetField(image, TIFFTAG_IMAGELENGTH, &height) == 0){
    /* we can't know image height :-( */
    free (buffer);
    TIFFClose(image);
    return NULL;
  }
  rowstride = width / 8;
    
  /* create the gray pixbuf */
  pix = ho_pixbuf_new (1, width, height, 0);
  if (!pix)
  {
    free (buffer);
    TIFFClose(image);
    return NULL;
  }

  /* copy data */
  for (x = 0; x < width; x++)
    for (y = 0; y < height; y++)
      ho_pixbuf_set (pix, x, y, 0, 255 * (1 - 
          ((((buffer[x / 8 + y * rowstride]) & (0x80 >> (x % 8))) > 0)?1:0)
        ));
    
  /* free buffer and close file */
  free (buffer);
  TIFFClose(image);
  
  return pix;
}
#endif /* USE_TIFF */

#ifdef USE_TIFF
int
ho_pixbuf_bw_tiff_save (const ho_pixbuf * pix, const char *filename)
{
  ho_bitmap * m = NULL;

  /* create a bitmap */
  m = ho_pixbuf_to_bitmap (pix, 0);
  if (!m)
    return TRUE;
  
  /* save bitmap */
  ho_bitmap_tiff_save (m, filename);
  
  /* free bitmap */
  ho_bitmap_free (m);
  
  return FALSE;
}
#endif /* USE_TIFF */
