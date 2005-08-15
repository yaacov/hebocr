## Process this file with automake to produce Makefile.in

SUBDIRS = src po tests

EXTRA_DIST = \
	autogen.sh hocr.spec.in hocr.desktop

hocrdocdir = ${prefix}/share/doc/hocr
hocrdoc_DATA = \
	README \
	AUTHORS \
	COPYING \
	NEWS \
	ChangeLog \
	INSTALL
	
hocrdesktopdir = ${prefix}/share/applications
hocrdesktop_DATA = \
	hocr.desktop
	
install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/pixmaps; then \
	  $(mkinstalldirs) $(DESTDIR)$(datadir)/pixmaps/$(PACKAGE); \
	  for pixmap in $(srcdir)/pixmaps/*; do \
	    if test -f $$pixmap; then \
	      $(INSTALL_DATA) $$pixmap $(DESTDIR)$(datadir)/pixmaps/$(PACKAGE); \
	    fi \
	  done \
	fi

dist-hook:
	if test -d pixmaps; then \
	  mkdir $(distdir)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(distdir)/pixmaps; \
	    fi \
	  done \
	fi

