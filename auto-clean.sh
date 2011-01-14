#! /bin/sh

set -x

make distclean

hg status | grep Makefile.in | awk '{ print $2 }' | xargs rm || true
rm -fr build
rm -fr config
rm -fr autom4te.cache/
rm -f aclocal.m4
rm -f configure

