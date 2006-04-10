## Process this file with automake to produce Makefile.in

EXTRA_DIST = hocr.1

INCLUDES = @PACKAGE_CFLAGS@ 
LDADD = @PACKAGE_LIBS@

if WITH_HSPELL

INCLUDES += @glib_CFLAGS@ @hspell_CFLAGS@ -DWITH_HSPELL
LDADD += @glib_LIBS@ @hspell_LIBS@

endif

if WITH_GTK

INCLUDES += @gtk_CFLAGS@ -DWITH_GTK
LDADD += @gtk_LIBS@

endif

if WITH_CMD

INCLUDES += \
        -DPACKAGE_DATA_DIR=\""$(datadir)"\" \
        -DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
        -I$(top_srcdir)/src

DEPS= $(top_builddir)/src/libhocr.la
ldadd= $(top_builddir)/src/libhocr.la

bin_PROGRAMS = hocr

hocr_SOURCES = \
	hocr-cmd.c

hocr_DEPENDENCIES = $(DEPS)
hocr_LDADD = $(LDADD) $(ldadd) $(INTLLIBS)

man_MANS = hocr.1

endif
