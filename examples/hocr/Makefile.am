EXTRA_DIST = hebocr.c

if WITH_HOCR

INCLUDES=-I$(top_srcdir)/src

DEPS= $(top_builddir)/src/libhebocr.la
ldadd= $(top_builddir)/src/libebhocr.la -lm

bin_PROGRAMS= hebocr

hebocr_SOURCES = hebocr.c
hebocr_CFLAGS = \
        $(glib_CFLAGS) \
        $(gtk_CFLAGS) \
        $(hspell_CFLAGS) \
        $(tiff_CFLAGS) \
        $(fftw_CFLAGS) \
        -DBUILD=\"$(BUILD_STR)\" 
hebocr_LDADD= \
        $(glib_LIBS) \
        $(gtk_LIBS) \
        $(hspell_LIBS) \
        $(tiff_LIBS) \
        $(fftw_LIBS) \
        $(hebocr_la_LIBADD) \
        $(top_builddir)/src/*.o -lm

endif
