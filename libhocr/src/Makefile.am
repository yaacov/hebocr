## Makefile.am -- Process this file with automake to produce Makefile.in

# The libhocr library
INCLUDES = \
        $(hspell_CFLAGS) 

pkglib_LTLIBRARIES = libhocr.la libhocrgtk.la

libhocr_la_SOURCES = \
        ho_pixbuf.c \
        ho_pixbuf_pnm.c \
        ho_pixbuf_draw.c \
        ho_bitmap.c \
        ho_bitmap_pnm.c \
        ho_bitmap_draw.c \
        ho_bitmap_filter.c \
        ho_obj.c \
        ho_objmap.c \
        ho_dimentions.c \
        ho_segment.c \
        ho_font.c \
        ho_font_pnm.c \
        ho_recognize.c \
        ho_string.c \
        ho_layout.c

libhocr_la_LIBADD = \
        $(hspell_LIBS)
        
libhocr_la_LDFLAGS = -version-info 0:0:0

library_includedir = $(includedir)/libhocr
library_include_HEADERS = hocr.h \
         ho_pixbuf.h \
         ho_pixbuf_pnm.h \
         ho_pixbuf_draw.h \
         ho_bitmap.h \
         ho_bitmap_pnm.h \
         ho_bitmap_draw.h \
         ho_bitmap_filter.h \
         ho_obj.h \
         ho_objmap.h \
         ho_dimentions.h \
         ho_segment.h \
         ho_font.h \
         ho_font_pnm.h \
         ho_recognize.h \
         ho_layout.h \
         ho_string.h \
         ho_gtk.h 

# libhocrgtk, helper functions to use hocr in gtk programs
libhocrgtk_la_SOURCES = \
        ho_gtk.c

libhocrgtk_la_CFLAGS = \
        $(gtk_CFLAGS)
        
libhocrgtk_la_LIBADD = \
        $(gtk_LIBS) \
        $(libhocr_la_LIBADD) \
        -L. -lhocr
        
libhocrgtk_la_LDFLAGS = $(libhocr_la_LDFLAGS)

# A demo hocr program
bin_PROGRAMS = hocr \
         hocr-teach
         
hocr_SOURCES = main.c
hocr_CFLAGS = \
        $(gtk_CFLAGS) \
        -DBUILD=\"$(BUILD_STR)\" 
hocr_LDADD= \
        $(gtk_LIBS) \
        $(libhocr_la_LIBADD) \
        -L. -lhocr -lhocrgtk -lm

hocr_teach_SOURCES = main_teach.c
hocr_teach_CFLAGS = \
        $(gtk_CFLAGS) \
        -DBUILD=\"$(BUILD_STR)\" 
hocr_teach_LDADD= \
        $(gtk_LIBS) \
        $(libhocr_la_LIBADD) \
        -L. -lhocr -lhocrgtk -lm
