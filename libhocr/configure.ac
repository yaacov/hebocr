dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)

AC_INIT([libhocr], [0.10.3], [Yaacov Zamir <kzamir@walla.co.il>])
AC_CONFIG_AUX_DIR([config])

AM_INIT_AUTOMAKE([1.9 foreign])

AC_PROG_CC
AM_PROG_LIBTOOL

AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

dnl gtk: load picture non-pnm file formats in hocr utility program

gtk_pkg_modules="gtk+-2.0"
PKG_CHECK_MODULES(gtk, [$gtk_pkg_modules], gtk_PKG="found", exit)
AC_SUBST(gtk_CFLAGS)
AC_SUBST(gtk_LIBS)

requires="$gtk_pkg_modules"

dnl hspell: hebrew speller

hspell_pkg_modules="hspell"
AC_CHECK_LIB(hspell, hspell_get_dictionary_path, hspell_PKG="found", exit, -lz)
hspell_CFLAGS=
hspell_LIBS=" -lhspell -lz"
AC_SUBST(hspell_CFLAGS)
AC_SUBST(hspell_LIBS)

dnl we require hspell but it does not use pkg-config
dnl requires="$requires $hspell_pkg_modules"

dnl fann: learning

dnl fann_pkg_modules="fann >= 2.0.0"
dnl PKG_CHECK_MODULES(fann, [$fann_pkg_modules], fann_PKG="found", exit)
dnl AC_SUBST(fann_CFLAGS)
dnl AC_SUBST(fann_LIBS)

dnl requires="$requires $fann_pkg_modules"

dnl export required librarys to pkg-config pc file 
AC_SUBST(requires)

dnl create a build string

BUILD_DATE=`date +%d%m%Y`
BUILD_STR="$PACKAGE-$VERSION-$build-$BUILD_DATE"
AC_SUBST(BUILD_STR)

dnl output

AC_CONFIG_FILES([
Makefile
libhocr.pc
Doxyfile
src/Makefile
www/Makefile
www/art/Makefile
])

AC_OUTPUT

dnl print status

echo
echo $PACKAGE $VERSION - Hebrew Optical Character Recognition library
echo build: $BUILD_STR
echo prefix: $prefix
echo
