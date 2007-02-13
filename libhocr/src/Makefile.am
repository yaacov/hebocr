## Makefile.am -- Process this file with automake to produce Makefile.in

# The libhocr library
INCLUDES = \
        $(hspell_CFLAGS) 

pkglib_LTLIBRARIES = libhocr.la

libhocr_la_SOURCES = \
        ho_pixbuf.c \
        ho_bitmap.c \
        ho_bitmap_filter.c \
        ho_obj.c \
        ho_objmap.c

libhocr_la_LIBADD = \
        $(hspell_LIBS)
        
libhocr_la_LDFLAGS = -version-info 0:0:0

library_includedir = $(includedir)/libhocr
library_include_HEADERS = hocr.h \
         ho_common.h \
         ho_pixbuf.h \
         ho_bitmap.h \
         ho_bitmap_filter.h \
         ho_obj.h \
         ho_objmap.h

# A demo hocr program
bin_PROGRAMS = hocr
hocr_SOURCES = main.c
hocr_CFLAGS = \
        $(gtk_CFLAGS) \
        -DBUILD=\"$(BUILD_STR)\" 
hocr_LDADD= \
        $(gtk_LIBS) \
        $(libhocr_pixbuf_la_LIBADD) \
        -L. -lhocr
