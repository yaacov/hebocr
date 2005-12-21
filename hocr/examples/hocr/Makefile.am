## Process this file with automake to produce Makefile.in

EXTRA_DIST = hocr.1

if WITH_CMD

INCLUDES = \
        -DPACKAGE_DATA_DIR=\""$(datadir)"\" \
        -DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
        -I$(top_srcdir)/src @PACKAGE_CFLAGS@ 

DEPS= $(top_builddir)/src/libhocr.la
ldadd= $(top_builddir)/src/libhocr.la

bin_PROGRAMS = hocr

hocr_SOURCES = \
	hocr-cmd.c

hocr_DEPENDENCIES = $(DEPS)
hocr_LDADD = ../../src/*.o $(INTLLIBS)

hocr$(EXEEXT): $(hocr_OBJECTS) $(hocr_DEPENDENCIES)
	@rm -f hocr$(EXEEXT)
	$(CC) -o hocr$(EXEEXT) @PACKAGE_LIBS@ $(hocr_LDFLAGS) $(hocr_OBJECTS) $(hocr_LDADD) $(LIBS)

man_MANS = hocr.1

endif
