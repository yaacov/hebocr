## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = hocr-gui

hocr_gui_SOURCES = \
	main.c \
	interface.c interface.h \
	callbacks.c callbacks.h \
	hocr.c hocr.h \
	font_markers.c font_markers.h

hocr_gui_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

