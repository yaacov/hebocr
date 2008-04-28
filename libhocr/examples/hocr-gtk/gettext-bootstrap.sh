#!/usr/sh

# make locale dir

mkdir -p he
mkdir -p sv

# hocr-gtk

intltool-extract --type=gettext/glade hocr-gtk.glade
xgettext --language=Python --keyword=_ --keyword=N_ --output=hocr-gtk.pot hocr-gtk hocr-gtk.glade.h

if ( test -e he_hocr-gtk.po ); then
  msgmerge --update he_hocr-gtk.po hocr-gtk.pot
else
  msginit --input=hocr-gtk.pot --output he_hocr-gtk.po --locale=he_IL.utf-8
fi

if ( test -e sv_hocr-gtk.po ); then
  msgmerge --update sv_hocr-gtk.po hocr-gtk.pot
else
  msginit --input=hocr-gtk.pot --output sv_hocr-gtk.po --locale=sv_SE.utf-8
fi

msgfmt --output-file=he/hocr-gtk.mo he_hocr_gtk.po
msgfmt --output-file=sv/hocr-gtk.mo sv_hocr_gtk.po

# sane-pygtk

intltool-extract --type=gettext/glade sane-pygtk.glade
xgettext --language=Python --keyword=_ --keyword=N_ --output=sane-pygtk.pot sane-pygtk sane-pygtk.glade.h

if ( test -e he_sane-pygtk.po ); then
  msgmerge --update he_sane-pygtk.po sane-pygtk.pot
else
  msginit --input=sane-pygtk.pot --output he_sane-pygtk.po --locale=he_IL.utf-8
fi

if ( test -e sv_sane-pygtk.po ); then
  msgmerge --update sv_sane-pygtk.po sane-pygtk.pot
else
  msginit --input=sane-pygtk.pot --output sv_sane-pygtk.po --locale=sv_SE.utf-8
fi

msgfmt --output-file=he/sane-pygtk.mo he_sane-pygtk.po
msgfmt --output-file=sv/sane-pygtk.mo sv_sane-pygtk.po
