## Makefile.am -- Process this file with automake to produce Makefile.in

SUBDIRS = src examples bindings docs

EXTRA_DIST = \
        Doxyfile HACKING

pkgconfigdir = $(libdir)/pkgconfig

if WITH_GTK
pkgconfig_DATA = hebocr.pc hebocr-gtk.pc
else
pkgconfig_DATA = hebocr.pc
endif

hebocrdocdir = ${prefix}/share/doc/hebocr
hebocrdoc_DATA = \
        README \
        AUTHORS \
        COPYING \
        NEWS \
        ChangeLog \
        INSTALL \
        HACKING
