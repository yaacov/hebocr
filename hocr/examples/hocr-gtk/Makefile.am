## Process this file with automake to produce Makefile.in

EXTRA_DIST = hocr.desktop

if WITH_GTK

if WITH_GTKSPELL

INCLUDES = \
        -DPACKAGE_DATA_DIR=\""$(datadir)"\" -DWITH_GTKSPELL\
        -DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
        @PACKAGE_CFLAGS@ @gtk_CFLAGS@ @gtkspell_CFLAGS@ -I$(top_srcdir)/src

#if WITH_WIN32
#hocr_gtk_LDADD = ../../src/*.o @PACKAGE_LIBS@ @gtk_LIBS@ @gtkspell_LIBS@ $(INTLLIBS)
#else
hocr_gtk_LDADD = $(ldadd) @PACKAGE_LIBS@ @gtk_LIBS@ @gtkspell_LIBS@ $(INTLLIBS)
#endif

else

INCLUDES = \
        -DPACKAGE_DATA_DIR=\""$(datadir)"\" \
        -DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
        @PACKAGE_CFLAGS@ @gtk_CFLAGS@ -I$(top_srcdir)/src

#if WITH_WIN32
#hocr_gtk_LDADD = ../../src/*.o @PACKAGE_LIBS@ @gtk_LIBS@ $(INTLLIBS)
#else
hocr_gtk_LDADD = $(ldadd) @PACKAGE_LIBS@ @gtk_LIBS@ $(INTLLIBS)
#endif

endif

DEPS= $(top_builddir)/src/libhocr.la
ldadd= $(top_builddir)/src/libhocr.la

bin_PROGRAMS = hocr-gtk

hocr_gtk_SOURCES = \
	hocr-gtk.c \
	interface.c interface.h \
	callbacks.c callbacks.h \
	hocr1-128.xpm

hocr_gtk_DEPENDENCIES = $(DEPS)

#if WITH_WIN32

#hocr-gtk$(EXEEXT): $(hocr_gtk_OBJECTS) $(hocr_gtk_DEPENDENCIES)
#	@rm -f hocr-gtk$(EXEEXT)
#	$(CC) -o hocr-gtk$(EXEEXT) $(hocr_gtk_LDFLAGS) $(hocr_gtk_OBJECTS) $(hocr_gtk_LDADD) $(LIBS)

#endif

hocrdesktopdir = ${prefix}/share/applications
hocrdesktop_DATA = \
	hocr.desktop

endif
