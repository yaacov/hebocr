dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)

AC_INIT([libhocr], [0.11.0], [Yaacov Zamir <kzamir@walla.co.il>])
AC_CONFIG_AUX_DIR([config])

AM_INIT_AUTOMAKE([1.9 foreign])

AC_PROG_CC
AM_PROG_LIBTOOL

dnl check for pkg-config
AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

dnl check hosr system
host_code=1
case ${host} in
  *linux*)
    host_code=1
  ;;
  *mingw32*)
    host_code=2
  ;;
  *darwin*)
    host_code=3
  ;;
esac
AM_CONDITIONAL(LINUX_HOST, test $host_code = 1 )
AM_CONDITIONAL(MINGW_HOST, test $host_code = 2 )
AM_CONDITIONAL(DARWIN_HOST, test $host_code = 3 )

dnl gtk: load picture non-pnm file formats in hocr utility program

gtk_pkg_modules="gtk+-2.0"
PKG_CHECK_MODULES(gtk, [$gtk_pkg_modules], gtk_PKG="found", exit)
AC_SUBST(gtk_CFLAGS)
AC_SUBST(gtk_LIBS)

requires="$gtk_pkg_modules"

dnl export required librarys to pkg-config pc file 
AC_SUBST(requires)

dnl check for swig
AC_CHECK_PROG(have_swig, "swig", yes, no)

dnl check for python
AC_CHECK_PROG(have_python, "python", yes, no)

build_python=$have_python

AC_ARG_ENABLE([python], 
  [AS_HELP_STRING([--disable-python],
  [do not build python binding])],
  [build_python=no], [build_python=$have_python])

AC_ARG_WITH(python-sitelib-dir, 
  [AS_HELP_STRING([--with-python-sitelib-dir=PATH],
  [path to python site lib])])

if test "$have_python" = "yes"; then
	  PY_PREFIX=`python -c 'import sys ; print sys.prefix'`
	  PY_VERSION=`python -c 'import sys ; print sys.version[[0:3]]'`
	  AC_MSG_CHECKING(for $PY_PREFIX/include/python$PY_VERSION/Python.h)
	  if test -f $PY_PREFIX/include/python$PY_VERSION/Python.h; then
		  AC_MSG_RESULT(yes)
		  PY_CFLAGS="-I$PY_PREFIX/include/python$PY_VERSION"
		  
		  if test "$with_python_sitelib_dir" = "" ; then
		    PY_MODULES_PATH="$PY_PREFIX/lib/python$PY_VERSION/site-packages"
		  else
		    PY_MODULES_PATH=$with_python_sitelib_dir
		  fi
		  
		  AC_SUBST([PY_VERSION])
		  AC_SUBST([PY_CFLAGS])
		  AC_SUBST([PY_MODULES_PATH])
		  if test "$have_swig" = "no"; then
			build_python=no
		  fi
	  else
		  AC_MSG_RESULT(no)
		  build_python=no
	  fi
fi

AM_CONDITIONAL(WITH_PYTHON, test "$build_python" = "yes")

dnl check for hspell

AC_CHECK_LIB([hspell], [hspell_get_dictionary_path], build_hspell=yes, build_hspell=no, -lz)

if test "$build_hspell" = "yes"; then
    hspell_CFLAGS=" -DUSE_HSPELL"
    hspell_LIBS=" -lhspell -lz"
   
    AC_SUBST(hspell_CFLAGS)
    AC_SUBST(hspell_LIBS)
fi

AM_CONDITIONAL(WITH_HSPELL, test "$build_hspell" = yes)

dnl check for libtiff

AC_CHECK_LIB([tiff], [TIFFOpen], build_tiff=yes, exit)

if test "$build_tiff" = "yes"; then
    tiff_CFLAGS=" -DUSE_TIFF"
    tiff_LIBS=" -ltiff"
   
    AC_SUBST(tiff_CFLAGS)
    AC_SUBST(tiff_LIBS)
fi

AM_CONDITIONAL(WITH_TIFF, test "$build_tiff" = yes)

dnl create a build string

BUILD_DATE=`date +%d%m%Y`
BUILD_STR="$PACKAGE-$VERSION-$build-$BUILD_DATE"
AC_SUBST(BUILD_STR)

dnl output

AC_CONFIG_FILES([
Makefile
libhocr.pc
libhocr-gtk.pc
Doxyfile
src/Makefile
examples/Makefile
examples/hocr/Makefile
examples/hocr-gtk/Makefile
examples/bindings/Makefile
bindings/Makefile
bindings/python/Makefile
docs/Makefile
docs/man/Makefile
docs/man/man1/Makefile
docs/man/man3/Makefile
])

AC_OUTPUT

dnl print status

echo
echo $PACKAGE $VERSION - Hebrew Optical Character Recognition library
echo OS: ${host}
echo build: $BUILD_STR
echo prefix: $prefix
echo compiler: ${CC}
echo
echo with hspell: ${build_hspell}
echo with libtiff: ${build_tiff}
echo
echo build hocr: yes
echo build hocr-gtk: yes
echo
echo have swig:	${have_swig}
echo build python binding: ${build_python}
echo
echo python site lib path: $PY_MODULES_PATH
echo
