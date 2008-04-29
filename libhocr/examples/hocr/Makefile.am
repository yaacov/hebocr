EXTRA_DIST = hocr.c

if WITH_HOCR

INCLUDES=-I$(top_srcdir)/src

DEPS= $(top_builddir)/src/libhocr.la
ldadd= $(top_builddir)/src/libhocr.la -lm

bin_PROGRAMS= hocr

hocr_SOURCES = hocr.c
hocr_CFLAGS = \
        $(glib_CFLAGS) \
        $(gtk_CFLAGS) \
        $(hspell_CFLAGS) \
        $(tiff_CFLAGS) \
        -DBUILD=\"$(BUILD_STR)\" 
hocr_LDADD= \
        $(glib_LIBS) \
        $(gtk_LIBS) \
        $(hspell_LIBS) \
        $(tiff_LIBS) \
        $(libhocr_la_LIBADD) \
        $(top_builddir)/src/*.o -lm

endif
