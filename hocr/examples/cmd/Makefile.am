## Process this file with automake to produce Makefile.in

if WITH_CMD

INCLUDES = \
        -DPACKAGE_DATA_DIR=\""$(datadir)"\" \
        -DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
        -I$(top_srcdir)/src

DEPS= $(top_builddir)/src/libhocr.la
ldadd= $(top_builddir)/src/libhocr.la -lm

bin_PROGRAMS = hocr

hocr_SOURCES = \
	hocr-cmd.c

hocr_DEPENDENCIES = $(DEPS)
hocr_LDADD = $(ldadd) $(INTLLIBS)

endif
