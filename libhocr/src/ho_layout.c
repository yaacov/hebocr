/***************************************************************************
 *            ho_layout.c
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

#include "ho_segment.h"
#include "ho_layout.h"

ho_layout *
ho_layout_new (const ho_bitmap * m_page_text, const unsigned char type)
{
  ho_layout *l_new;

  /* allocate layout memory */
  l_new = (ho_layout *) malloc (sizeof (ho_layout));
  if (!l_new)
    {
      return NULL;
    }
  /* copy text bitmap into layout */
  l_new->m_page_text = ho_bitmap_clone (m_page_text);
  if (!l_new->m_page_text)
    {
      free (l_new);
      return NULL;
    }

  l_new->type = type;

  /* link all pointers to NULL */
  l_new->n_blocks = 0;
  l_new->m_page_blocks_mask = NULL;
  l_new->n_lines = NULL;
  l_new->m_blocks_text = NULL;
  l_new->m_blocks_lines_mask = NULL;
  l_new->n_words = NULL;
  l_new->m_lines_text = NULL;
  l_new->m_lines_line_mask = NULL;
  l_new->m_lines_words_mask = NULL;

  return l_new;
}

int
ho_layout_free (ho_layout * l_page)
{
  int block_index, line_index;

  for (block_index = 0; block_index < l_page->n_blocks; block_index++)
    {
      for (line_index = 0; line_index < l_page->n_lines[block_index];
	   line_index++)
	{
	  /* free words bitmaps */
	  if (l_page->m_lines_words_mask[block_index][line_index])
	    ho_bitmap_free (l_page->
			    m_lines_words_mask[block_index][line_index]);
	  if (l_page->m_lines_text[block_index][line_index])
	    ho_bitmap_free (l_page->m_lines_text[block_index][line_index]);
	  if (l_page->m_lines_line_mask[block_index][line_index])
	    ho_bitmap_free (l_page->
			    m_lines_line_mask[block_index][line_index]);
	}

      /* free line bitmaps */
      if (l_page->m_blocks_lines_mask[block_index])
	ho_bitmap_free (l_page->m_blocks_lines_mask[block_index]);
      if (l_page->m_blocks_text[block_index])
	ho_bitmap_free (l_page->m_blocks_text[block_index]);

      /* free line arrays */
      if (l_page->m_lines_words_mask[block_index])
	free (l_page->m_lines_words_mask[block_index]);
      if (l_page->m_lines_text[block_index])
	free (l_page->m_lines_text[block_index]);
      if (l_page->m_lines_line_mask[block_index])
	free (l_page->m_lines_line_mask[block_index]);
      if (l_page->n_words[block_index])
	free (l_page->n_words[block_index]);
    }

  /* free block bitmaps */
  if (l_page->m_page_blocks_mask)
    ho_bitmap_free (l_page->m_page_blocks_mask);

  /* free block arrays */
  if (l_page->m_blocks_text)
    free (l_page->m_blocks_text);
  if (l_page->m_lines_text)
    free (l_page->m_lines_text);
  if (l_page->m_lines_line_mask)
    free (l_page->m_lines_line_mask);
  if (l_page->m_lines_words_mask)
    free (l_page->m_lines_words_mask);
  if (l_page->n_words)
    free (l_page->n_words);
  if (l_page->m_blocks_lines_mask)
    free (l_page->m_blocks_lines_mask);
  if (l_page->n_lines)
    free (l_page->n_lines);

  /* free page elements */
  if (l_page->m_page_text)
    ho_bitmap_free (l_page->m_page_text);

  /* free this page */
  if (l_page)
    free (l_page);

  return FALSE;
}

int
ho_layout_create (ho_layout * l_page)
{
  /* the the create functions untill all the layout is created */

  return FALSE;
}

int
ho_layout_create_block_mask (ho_layout * l_page)
{
  ho_objmap *o_map_blocks = NULL;
  int i;

  /* set the font dimentions info in the main text matrix */
  ho_dimentions_font_width_height_nikud (l_page->m_page_text, 6, 200, 6, 200);
  /* set line_spacing in the main text matrix */
  ho_dimentions_line_spacing (l_page->m_page_text);
  /* create the blocks mask */
  l_page->m_page_blocks_mask =
    ho_segment_paragraphs (l_page->m_page_text, l_page->type);

  /* count blocks */
  o_map_blocks = ho_objmap_new_from_bitmap (l_page->m_page_blocks_mask);

  /* set number of blocks */
  l_page->n_blocks = ho_objmap_get_size (o_map_blocks);

  ho_objmap_free (o_map_blocks);

  /* allocate memory for all other layout components */
  l_page->m_blocks_lines_mask =
    (ho_bitmap **) malloc (l_page->n_blocks * sizeof (ho_bitmap *));
  if (!l_page->m_blocks_lines_mask)
    return TRUE;
  l_page->m_blocks_text =
    (ho_bitmap **) malloc (l_page->n_blocks * sizeof (ho_bitmap *));
  if (!l_page->m_blocks_text)
    return TRUE;

  l_page->n_lines = (int *) malloc (l_page->n_blocks * sizeof (int));
  if (!l_page->n_lines)
    return TRUE;

  /* allocate an array of word masks arrays */
  l_page->m_lines_text =
    (ho_bitmap ***) malloc (l_page->n_blocks * sizeof (ho_bitmap **));
  if (!l_page->m_lines_text)
    return TRUE;
  l_page->m_lines_line_mask =
    (ho_bitmap ***) malloc (l_page->n_blocks * sizeof (ho_bitmap **));
  if (!l_page->m_lines_line_mask)
    return TRUE;
  l_page->m_lines_words_mask =
    (ho_bitmap ***) malloc (l_page->n_blocks * sizeof (ho_bitmap **));
  if (!l_page->m_lines_words_mask)
    return TRUE;

  l_page->n_words = (int **) malloc (l_page->n_blocks * sizeof (int *));
  if (!l_page->n_words)
    return TRUE;

  /* link all free pointers to NULL */
  for (i = 0; i < l_page->n_blocks; i++)
    {
      l_page->m_blocks_lines_mask[i] = NULL;
      l_page->m_blocks_text[i] = NULL;
      l_page->n_lines[i] = 0;
      l_page->m_lines_words_mask[i] = NULL;
      l_page->m_lines_text[i] = NULL;
      l_page->m_lines_line_mask[i] = NULL;
      l_page->n_words[i] = NULL;
    }

  return FALSE;
}

int
ho_layout_create_line_mask (ho_layout * l_page, const int block_index)
{
  ho_bitmap *m_block_text = NULL;
  ho_bitmap *m_temp = NULL;
  ho_objmap *o_map_blocks = NULL;
  int x, y, height, width;
  int i;

  m_block_text = ho_layout_get_block_text (l_page, block_index);

  /* set the font dimentions info in the main text block */
  ho_dimentions_font_width_height_nikud (m_block_text, 6, 200, 6, 200);
  /* set line_spacing in the main text block */
  ho_dimentions_line_spacing (m_block_text);
  /* create the lines mask */
  l_page->m_blocks_lines_mask[block_index] = ho_segment_lines (m_block_text);

  l_page->m_blocks_text[block_index] = m_block_text;

  /* count lines */
  o_map_blocks =
    ho_objmap_new_from_bitmap (l_page->m_blocks_lines_mask[block_index]);

  /* set number of lines */
  l_page->n_lines[block_index] = ho_objmap_get_size (o_map_blocks);

  ho_objmap_free (o_map_blocks);

  /* allocate an array of word masks arrays */
  l_page->m_lines_text[block_index] =
    (ho_bitmap **) malloc (l_page->n_lines[block_index] *
			   sizeof (ho_bitmap *));
  if (!l_page->m_lines_text[block_index])
    return TRUE;
  l_page->m_lines_line_mask[block_index] =
    (ho_bitmap **) malloc (l_page->n_lines[block_index] *
			   sizeof (ho_bitmap *));
  if (!l_page->m_lines_line_mask[block_index])
    return TRUE;

  l_page->m_lines_words_mask[block_index] =
    (ho_bitmap **) malloc (l_page->n_lines[block_index] *
			   sizeof (ho_bitmap *));
  if (!l_page->m_lines_words_mask[block_index])
    return TRUE;

  l_page->n_words[block_index] =
    (int *) malloc (l_page->n_lines[block_index] * sizeof (int));
  if (!l_page->n_words[block_index])
    return TRUE;

  /* link all free pointers to NULL */
  for (i = 0; i < l_page->n_lines[block_index]; i++)
    {
      l_page->m_lines_words_mask[block_index][i] = NULL;
      l_page->m_lines_text[block_index][i] = NULL;
      l_page->m_lines_line_mask[block_index][i] = NULL;
      l_page->n_words[block_index][i] = 0;
    }

  return FALSE;
}

int
ho_layout_create_word_mask (ho_layout * l_page, const int block_index,
			    const int line_index)
{
  ho_objmap *o_map_blocks = NULL;
  ho_bitmap *m_line_text = NULL;
  ho_bitmap *m_line_line_mask = NULL;
  int x, y, height, width;
  int i;

  m_line_text = ho_layout_get_line_text (l_page, block_index, line_index);
  ho_dimentions_font_width_height_nikud (m_line_text, 6, 200, 6, 200);
  l_page->m_lines_text[block_index][line_index] = m_line_text;

  /* create the words mask */
  m_line_line_mask =
    ho_layout_get_line_line_mask (l_page, block_index, line_index);
  l_page->m_lines_line_mask[block_index][line_index] = m_line_line_mask;

  m_line_text->line_spacing =
    l_page->m_blocks_text[block_index]->line_spacing;
  ho_dimentions_font_spacing (m_line_text, m_line_line_mask);

  l_page->m_lines_words_mask[block_index][line_index] =
    ho_segment_words (m_line_text, m_line_line_mask);

  /* count words */
  o_map_blocks =
    ho_objmap_new_from_bitmap (l_page->
			       m_lines_words_mask[block_index][line_index]);

  /* set number of words */
  l_page->n_words[block_index][line_index] =
    ho_objmap_get_size (o_map_blocks);

  ho_objmap_free (o_map_blocks);

  return FALSE;
}

ho_bitmap *
ho_layout_get_block_text (ho_layout * l_page, int block_index)
{
  ho_bitmap *m_block_text = NULL;
  ho_bitmap *m_temp = NULL;
  ho_objmap *o_map_blocks = NULL;
  int x, y, height, width;

  /* get paragraph objmap */
  o_map_blocks = ho_objmap_new_from_bitmap (l_page->m_page_blocks_mask);

  /* sort paragraph by reading order */
  ho_objmap_sort_by_reading_index (o_map_blocks, l_page->type);

  x = ho_objmap_get_object (o_map_blocks, block_index).x;
  y = ho_objmap_get_object (o_map_blocks, block_index).y;
  width = ho_objmap_get_object (o_map_blocks, block_index).width;
  height = ho_objmap_get_object (o_map_blocks, block_index).height;

  /* get sum leeway */
  x -= l_page->m_page_text->font_width;
  y -= l_page->m_page_text->font_height;
  width += l_page->m_page_text->font_width * 2;
  height += 5 * l_page->m_page_text->font_height / 2;

  /* sanity check */
  if (x < 0)
    x = 0;
  if (x + width > l_page->m_page_text->width)
    width = l_page->m_page_text->width - x;
  if (y < 0)
    y = 0;
  if (y + height > l_page->m_page_text->height)
    height = l_page->m_page_text->height - y;

  if (l_page->type)
    {
      m_block_text =
	ho_bitmap_clone_window (l_page->m_page_text, x, y, width, height);
    }
  else
    {
      m_temp = ho_objmap_to_bitmap_by_index (o_map_blocks, block_index);
      ho_bitmap_and (m_temp, l_page->m_page_text);
      m_block_text = ho_bitmap_clone_window (m_temp, x, y, width, height);
      ho_bitmap_free (m_temp);
    }

  ho_objmap_free (o_map_blocks);

  return m_block_text;
}

ho_bitmap *
ho_layout_get_line_text (ho_layout * l_page, int block_index, int line_index)
{
  ho_bitmap *m_line_text = NULL;
  ho_bitmap *m_temp = NULL;
  ho_bitmap *m_temp2 = NULL;
  ho_objmap *o_map_lines = NULL;
  int x, y, height, width;
  int i;

  /* get paragraph objmap */
  o_map_lines =
    ho_objmap_new_from_bitmap (l_page->m_blocks_lines_mask[block_index]);

  /* sort lines by reading order */
  ho_objmap_sort_by_reading_index (o_map_lines, 2);

  x = ho_objmap_get_object (o_map_lines, line_index).x;
  y = ho_objmap_get_object (o_map_lines, line_index).y;
  width = ho_objmap_get_object (o_map_lines, line_index).width;
  height = ho_objmap_get_object (o_map_lines, line_index).height;

  /* get sum leeway */
  x -= l_page->m_blocks_text[block_index]->font_width;
  y -= l_page->m_blocks_text[block_index]->font_height;
  width += l_page->m_blocks_text[block_index]->font_width * 2;
  height += 5 * l_page->m_blocks_text[block_index]->font_height / 2;

  /* sanity check */
  if (x < 0)
    x = 0;
  if (x + width > l_page->m_blocks_text[block_index]->width)
    width = l_page->m_blocks_text[block_index]->width - x;
  if (y < 0)
    y = 0;
  if (y + height > l_page->m_blocks_text[block_index]->height)
    height = l_page->m_blocks_text[block_index]->height - y;

  m_temp = ho_objmap_to_bitmap_by_index (o_map_lines, line_index);
  m_temp2 = ho_bitmap_set_height (m_temp,
				  4 * l_page->m_blocks_text[block_index]->
				  font_height / 3,
				  2 * l_page->m_blocks_text[block_index]->
				  font_height / 3,
				  2 * l_page->m_blocks_text[block_index]->
				  font_height / 3);
  ho_bitmap_free (m_temp);

  ho_objmap_free (o_map_lines);
  ho_bitmap_and (m_temp2, l_page->m_blocks_text[block_index]);

  m_line_text = ho_bitmap_clone_window (m_temp2, x, y, width, height);

  ho_bitmap_free (m_temp2);

  return m_line_text;
}

ho_bitmap *
ho_layout_get_line_line_mask (ho_layout * l_page, int block_index,
			      int line_index)
{
  ho_bitmap *m_line_mask = NULL;
  ho_bitmap *m_temp = NULL;
  ho_bitmap *m_temp2 = NULL;
  ho_objmap *o_map_lines = NULL;
  int x, y, height, width;
  int i;

  /* get paragraph objmap */
  o_map_lines =
    ho_objmap_new_from_bitmap (l_page->m_blocks_lines_mask[block_index]);

  /* sort lines by reading order */
  ho_objmap_sort_by_reading_index (o_map_lines, 2);

  x = ho_objmap_get_object (o_map_lines, line_index).x;
  y = ho_objmap_get_object (o_map_lines, line_index).y;
  width = ho_objmap_get_object (o_map_lines, line_index).width;
  height = ho_objmap_get_object (o_map_lines, line_index).height;

  /* get sum leeway */
  x -= l_page->m_blocks_text[block_index]->font_width;
  y -= l_page->m_blocks_text[block_index]->font_height;
  width += l_page->m_blocks_text[block_index]->font_width * 2;
  height += 5 * l_page->m_blocks_text[block_index]->font_height / 2;

  /* sanity check */
  if (x < 0)
    x = 0;
  if (x + width > l_page->m_blocks_text[block_index]->width)
    width = l_page->m_blocks_text[block_index]->width - x;
  if (y < 0)
    y = 0;
  if (y + height > l_page->m_blocks_text[block_index]->height)
    height = l_page->m_blocks_text[block_index]->height - y;

  m_temp = ho_objmap_to_bitmap_by_index (o_map_lines, line_index);

  ho_objmap_free (o_map_lines);

  m_line_mask = ho_bitmap_clone_window (m_temp, x, y, width, height);
  ho_bitmap_free (m_temp);

  return m_line_mask;
}
