
/* Copyright (C) 2003-2004 Nadav Har'El and Dan Kenigsberg */

/* OCR functionality added by Yaacov Zamir, 2006 */
#ifdef WITH_HSPELL

#include <hspell.h>

/* try to find corrections for word optimized for OCR use */
void
hspell_trycorrect_ocr (struct dict_radix *dict, const char *w,
		       struct corlist *cl);

#endif
