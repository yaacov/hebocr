INCLUDES=-I$(top_srcdir)/src

DEPS= $(top_builddir)/src/libhocr.la
ldadd= $(top_builddir)/src/libhocr.la -lm

bin_PROGRAMS= hocr

hocr_SOURCES = hocr.c
hocr_CFLAGS = \
        $(gtk_CFLAGS) \
        -DBUILD=\"$(BUILD_STR)\" 
hocr_LDADD= \
        $(gtk_LIBS) \
        $(libhocr_la_LIBADD) \
        $(top_builddir)/src/*.o -lm
