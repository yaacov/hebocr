## Process this file with automake to produce Makefile.in

libhocr_la_CFLAGS =\
	 -Wall\
     -g

lib_LTLIBRARIES = libhocr.la

libhocr_la_SOURCES = \
	hocr.c hocr.h \
	hocr_textbuffer.c hocr_textbuffer.h \
	hocr_pixbuf.c hocr_pixbuf.h \
	hocr_object.c hocr_object.h \
	font_recognition.c font_recognition.h \
	page_layout.c page_layout.h \
	font_layout.c font_layout.h \
	consts.h

libhocr_la_LDFLAGS = -version-info $(VERSION_INFO)

libhocr_la_LIBADD = 

include_HEADERS = hocr.h hocr_pixbuf.h hocr_object.h hocr_textbuffer.h
