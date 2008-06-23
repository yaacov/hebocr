## Makefile.am -- Process this file with automake to produce Makefile.in

# The libhocr library
INCLUDES = \
        $(hspell_CFLAGS) \
        $(tiff_CFLAGS) \
        $(fftw_CFLAGS) \
        -DBUILD=\"$(BUILD_STR)\" 

lib_LTLIBRARIES = libhocr.la

libhocr_la_SOURCES = \
        hocr.c \
        ho_pixbuf.c \
        ho_array.c \
        ho_array_hist.c \
        ho_array_stat.c \
        ho_bitmap.c \
        ho_bitmap_hist.c \
        ho_objmap.c \
        ho_dimentions.c \
        ho_segment.c \
        ho_font.c \
        ho_recognize.c \
        ho_recognize_nikud.c \
        fonts/ho_recognize_font_1.c \
        fonts/ho_recognize_font_2.c \
        fonts/ho_recognize_font_3.c \
        fonts/ho_recognize_font_4.c \
        fonts/ho_recognize_font_5.c \
        fonts/ho_recognize_font_6.c \
        ho_string.c \
        ho_layout.c
     
libhocr_la_LDFLAGS = -version-info 0:0:0

libhocr_la_LIBADD = \
        $(hspell_LIBS) \
        $(tiff_LIBS) \
        $(fftw_LIBS)
        
library_includedir = $(includedir)/libhocr
library_include_HEADERS = \
         hocr.h \
         hocrpp.h \
         ho_pixbuf.h \
         ho_array.h \
         ho_array_hist.h \
         ho_array_stat.h \
         ho_bitmap.h \
         ho_bitmap_hist.h \
         ho_objmap.h \
         ho_dimentions.h \
         ho_segment.h \
         ho_font.h \
         ho_recognize.h \
         ho_recognize_nikud.h \
         fonts/ho_recognize_font_1.h \
         fonts/ho_recognize_font_2.h \
         fonts/ho_recognize_font_3.h \
         fonts/ho_recognize_font_4.h \
         fonts/ho_recognize_font_5.h \
         fonts/ho_recognize_font_6.h \
         ho_layout.h \
         ho_string.h \
         ho_gtk.h 

# libhocrgtk, helper functions to use hocr in gtk programs
if WITH_GTK

lib_LTLIBRARIES += libhocrgtk.la

libhocrgtk_la_SOURCES = \
        ho_gtk.c

libhocrgtk_la_CFLAGS = \
        $(gtk_CFLAGS)
        
libhocrgtk_la_LIBADD = \
        $(gtk_LIBS) \
        $(libhocr_la_LIBADD) \
        -L. -lhocr
        
libhocrgtk_la_LDFLAGS = $(libhocr_la_LDFLAGS)

endif
