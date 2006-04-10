
/* Copyright (C) 2003-2004 Nadav Har'El and Dan Kenigsberg */

/* OCR functionality added by Yaacov Zamir, 2006 */

#ifdef WITH_HSPELL

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hspell.h>

#define TRYBUF if(hspell_check_word(dict, buf, &preflen)) corlist_add(cl, buf)

/* In the past, we used to use snprintf for this splicing needed for
   hspell_trycorrect. But it turns out that snprintf, when given the %.*s
   format, counts locale "characters", and not bytes. When the locale was
   UTF8, this made it count wrong, despite us knowing here that we only
   deal with iso-8859-8. So let's implement this functionality on our own.
   This is ugly :(

   This function splices together the first s1len characters of s1, then
   two characters c1,c2 (or nothing if c is 0) and the string s2.
*/
static inline void
splice (char *buf, int size, const char *s1, int s1len,
	char c1, char c2, const char *s2)
{
	int len = s1len;

	if (len >= size)
		len = size - 1;
	strncpy (buf, s1, len);
	if (len + 1 >= size)
	{
		buf[len] = '\0';
		return;
	}
	else if (c1)
	{
		buf[len++] = c1;
	}
	if (len + 1 >= size)
	{
		buf[len] = '\0';
		return;
	}
	else if (c2)
	{
		buf[len++] = c2;
	}
	if (s2)
	{
		strncpy (buf + len, s2, size - len - 1);
		buf[size - 1] = '\0';	/* in case the last command truncated */
	}
	else
	{
		buf[len] = '\0';
	}

	return;
}

/* try to find corrections for word by spliting it to two words */
void
hspell_trysplit (struct dict_radix *dict, const char *w, struct corlist *cl, int split_only_at_sofiot)
{
	char buf[30];
	int i;
	int len = strlen (w), preflen;

	/* check if start of w is a word */
	for (i = 2; i < len; i++)
	{
		if (!split_only_at_sofiot || w[i - 1] == 'ך' || w[i - 1] == 'ם' || 
			w[i - 1] == 'ן' || w[i - 1] == 'ף' ||w[i - 1] == 'ץ')
		{
			/* copy original word to buf */
			strncpy (buf, w, 30);
			/* end buf in the middle */
			buf[i] = '\0';

			/* if start of w is a real word add to cl */
			if (hspell_check_word (dict, buf, &preflen))
			{
				buf[i] = ' ';
				strncpy (&(buf[i + 1]), &(w[i]), len - i);

				corlist_add (cl, buf);
			}
		}
	}

	/* check if end of w is a word */
	for (i = 2; i < (len - 1); i++)
	{
		if (!split_only_at_sofiot || w[i - 1] == 'ך' || w[i - 1] == 'ם' || 
			w[i - 1] == 'ן' || w[i - 1] == 'ף' ||w[i - 1] == 'ץ')
		{
			/* copy original word to buf */
			strncpy (buf, &(w[i]), 30);

			/* if start of w is a real word add to cl */
			if (hspell_check_word (dict, buf, &preflen))
			{
				strncpy (buf, w, i);
				buf[i] = ' ';
					strncpy (&(buf[i + 1]), &(w[i]), len - i);
				corlist_add (cl, buf);
			}
		}
	}

	return;
}

/* try to find corrections for word using similar fonts table */
void
hspell_trycorrect_using_similar_table (struct dict_radix *dict, const char *w,
				       struct corlist *cl, char **similar)
{
	char buf[30];
	int i;
	int len = strlen (w), preflen;

	/* try to add a missing em kri'a - yud or vav */
	for (i = 1; i < len; i++)
	{
		splice (buf, sizeof (buf), w, i, 'י', 0, w + i);
		TRYBUF;
		splice (buf, sizeof (buf), w, i, 'ו', 0, w + i);
		TRYBUF;
	}
	/* try to remove an em kri'a - yud or vav */
	/* NOTE: in hspell.pl the loop was from i=0 to i<len... */
	for (i = 1; i < len - 1; i++)
	{
		if (w[i] == 'י' || w[i] == 'ו')
		{
			splice (buf, sizeof (buf), w, i, 0, 0, w + i + 1);
			TRYBUF;
		}
	}
	/* try to add or remove an aleph (is that useful?) */
	/* TODO: don't add an aleph next to yud or non-double vav, as it can't
	 * be an em kria there? */
	for (i = 1; i < len; i++)
	{
		splice (buf, sizeof (buf), w, i, 'א', 0, w + i);
		TRYBUF;
	}
	for (i = 1; i < len - 1; i++)
	{
		if (w[i] == 'א')
		{
			splice (buf, sizeof (buf), w, i, 0, 0, w + i + 1);
			TRYBUF;
		}
	}
	/* try to replace similarly sounding (for certain people) letters: */
	for (i = 0; i < len; i++)
	{
		int group;
		char *g;

		for (group = 0;
		     group < (sizeof (similar) / sizeof (similar[0])); group++)
		{
			for (g = similar[group]; *g && *g != w[i]; g++) ;
			;
			if (*g)
			{
				/* character in group - try the other ones in
				 * this group! */
				for (g = similar[group]; *g; g++)
				{
					if (*g == w[i])
						continue;
					if (i > 0 && w[i] == 'ו'
					    && w[i + 1] == 'ו')
						splice (buf, sizeof (buf), w, i,
							*g, 0, w + i + 2);
					else if (*g == 'ו')
						splice (buf, sizeof (buf), w, i,
							'ו', 'ו', w + i + 1);
					else
						splice (buf, sizeof (buf), w, i,
							*g, 0, w + i + 1);
					TRYBUF;
				}
			}
		}
	}
	/* try to replace a non-final letter at the end of the word by its
	 * final form and vice versa (useful check for abbreviations) */
	strncpy (buf, w, sizeof (buf));
	switch (w[len - 1])
	{

		buf[len - 1] = 'כ';
		break;

		buf[len - 1] = 'מ';
		break;

		buf[len - 1] = 'נ';
		break;

		buf[len - 1] = 'צ';
		break;

		buf[len - 1] = 'פ';
		break;

		buf[len - 1] = 'ך';
		break;

		buf[len - 1] = 'ם';
		break;

		buf[len - 1] = 'ן';
		break;

		buf[len - 1] = 'ץ';
		break;

		buf[len - 1] = 'ף';
		break;
	}
	if (buf[len - 1] != w[len - 1])
	{
		TRYBUF;
	}
	/* try to make the word into an acronym (add " before last character */
	if (len >= 2)
	{
		splice (buf, sizeof (buf), w, len - 1, '"', w[len - 1], 0);
		TRYBUF;
	}
	/* try to make the word into an abbreviation (add ' at the end) */
	snprintf (buf, sizeof (buf), "%s'", w);
	TRYBUF;
}

/* try to find corrections for word optimized for OCR use */
void
hspell_trycorrect_ocr (struct dict_radix *dict, const char *w,
		       struct corlist *cl)
{
	/* regular spell check */
	static char *similar_1[] = { "העא", "גה", "כח", "תט", "צס", "שס",
		"כק", "בו", "פב"
	};
	/* replace unknown '*' chars */
	static char *similar_2[] = { "*י", "*ג", "*ס", "*ט", "*ע", "ש*",
		"*ק", "* ", "*ד", "*ת"
	};

	/* look alike glifs */
	static char *similar_3[] = { "בנ", "בפ", "נון", "עמפ", "זו", "שט",
	"םט", "עחנץ" , "םש" , "מספם" , "מט" , "סט" , "אג" ,"גנ" , "רך" , "כנ" , "כב" , "תח"
	};

	/* first thing is to do regular spell check on the text */
	hspell_trycorrect_using_similar_table (dict, w, cl, similar_1);

	/* try to replace unknown '*' chars */
	hspell_trycorrect_using_similar_table (dict, w, cl, similar_2);

	/* add suggestions created by using similar glifs */
	hspell_trycorrect_using_similar_table (dict, w, cl, similar_3);

	/* add suggestions by splitting at otiot sofiot*/
	hspell_trysplit (dict, w, cl, 1);

	/* add suggestions by splitting at any ot*/
	hspell_trysplit (dict, w, cl, 0);

	return;
}

#endif
