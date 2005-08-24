## Process this file with automake to produce Makefile.in

if WITH_GUI

INCLUDES = \
        -DPACKAGE_DATA_DIR=\""$(datadir)"\" \
        -DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
        @gui_CFLAGS@ -I$(top_srcdir)/src

DEPS= $(top_builddir)/src/libhocr.la
ldadd= $(top_builddir)/src/libhocr.la -lm

bin_PROGRAMS = hocr-gnome

hocr_gnome_SOURCES = \
	hocr-gui.c \
	interface.c interface.h \
	callbacks.c callbacks.h 

hocr_gnome_DEPENDENCIES = $(DEPS)
hocr_gnome_LDADD = $(ldadd) @gui_LIBS@ $(INTLLIBS)

endif
