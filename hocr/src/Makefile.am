## Process this file with automake to produce Makefile.in

INCLUDES = @PACKAGE_CFLAGS@ -g -Wall

libhocr_la_CFLAGS = @PACKAGE_CFLAGS@ -g -Wall

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

libhocr_la_LIBADD = @PACKAGE_LIBS@

include_HEADERS = hocrpp.h hocr.h hocr_pixbuf.h hocr_object.h hocr_textbuffer.h
