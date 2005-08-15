## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = hocr-gui hocr

hocr_gui_SOURCES = \
	hocr-gui.c \
	interface.c interface.h \
	callbacks.c callbacks.h \
	hocr.c hocr.h \
	font_markers.c font_markers.h \
	page_layout.c page_layout.h \
	font_layout.c font_layout.h \
	consts.h \
	box.h

hocr_gui_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

hocr_SOURCES = \
	hocr-cmd.c \
	hocr.c hocr.h \
	font_markers.c font_markers.h \
	page_layout.c page_layout.h \
	font_layout.c font_layout.h \
	consts.h \
	box.h

hocr_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)
