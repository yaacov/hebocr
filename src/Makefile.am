## Makefile.am -- Process this file with automake to produce Makefile.in

# The libhocr library
INCLUDES = \
        $(hspell_CFLAGS) \
        $(tiff_CFLAGS) \
        $(fftw_CFLAGS) \
        -DBUILD=\"$(BUILD_STR)\" \
        -DVERSION=\"$(VERSION)\"

lib_LTLIBRARIES = libhebocr.la

libhebocr_la_SOURCES = \
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
        ho_string.c \
        ho_linguistics.c \
        ho_layout.c
     
libhebocr_la_LDFLAGS = -version-info 0:0:0

libhebocr_la_LIBADD = \
        $(hspell_LIBS) \
        $(tiff_LIBS) \
        $(fftw_LIBS)
        
library_includedir = $(includedir)/hebocr
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
         ho_layout.h \
         ho_string.h \
         ho_linguistics.h

