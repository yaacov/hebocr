#!/usr/sh

intltool-extract --type=gettext/glade hocr-gtk.glade
xgettext --language=Python --keyword=_ --keyword=N_ --output=hocr-gtk.pot hocr-gtk.py hocr-gtk.glade.h
msginit --input=hocr-gtk.pot --locale=he_IL.utf-8
msginit --input=hocr-gtk.pot --locale=sv_SE.utf-8

mkdir he
mkdir sv

msgfmt --output-file=he/hocr-gtk.mo he.po
msgfmt --output-file=sv/hocr-gtk.mo sv.po
