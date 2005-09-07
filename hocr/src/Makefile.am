## Process this file with automake to produce Makefile.in

libhocr_la_CFLAGS =\
	-Wall\
	-g

lib_LTLIBRARIES = libhocr.la

libhocr_la_SOURCES = \
	hocr.c hocr.h \
	font_recognition.c font_recognition.h \
	page_layout.c page_layout.h \
	font_layout.c font_layout.h \
	magic_numbers.h

libhocr_la_LDFLAGS = -version-info $(VERSION_INFO)

libhocr_la_LIBADD = 

include_HEADERS = hocr.h hocrpp.h
