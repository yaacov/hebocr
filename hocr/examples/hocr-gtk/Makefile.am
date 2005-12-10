## Process this file with automake to produce Makefile.in

EXTRA_DIST = hocr.desktop

if WITH_GTK

if WITH_GTKSPELL

INCLUDES = \
        -DPACKAGE_DATA_DIR=\""$(datadir)"\" -DWITH_GTKSPELL\
        -DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
        @gtk_CFLAGS@ @gtkspell_CFLAGS@ -I$(top_srcdir)/src

hocr_gtk_LDADD = $(ldadd) @gtk_LIBS@ @gtkspell_LIBS@ $(INTLLIBS)

else

INCLUDES = \
        -DPACKAGE_DATA_DIR=\""$(datadir)"\" \
        -DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
        @gtk_CFLAGS@ -I$(top_srcdir)/src

hocr_gtk_LDADD = $(ldadd) @gtk_LIBS@ $(INTLLIBS)

endif

DEPS= $(top_builddir)/src/libhocr.la
ldadd= $(top_builddir)/src/libhocr.la -lm

bin_PROGRAMS = hocr-gtk

hocr_gtk_SOURCES = \
	hocr-gtk.c \
	interface.c interface.h \
	callbacks.c callbacks.h 

hocr_gtk_DEPENDENCIES = $(DEPS)

hocrdesktopdir = ${prefix}/share/applications
hocrdesktop_DATA = \
	hocr.desktop

endif
