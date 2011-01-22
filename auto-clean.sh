#! /bin/sh

set -x

make distclean

hg status | egrep 'Makefile.in|\.mo|~|.pot|\.glade\.h' | awk '{ print $2 }' | xargs rm || true
rm -fr build
rm -fr config
rm -fr autom4te.cache/
rm -f aclocal.m4
rm -f configure

