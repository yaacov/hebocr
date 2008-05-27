dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)

AC_INIT([libhocr], [0.10.10], [Yaacov Zamir <kzamir@walla.co.il>])
AC_CONFIG_AUX_DIR([config])

AM_INIT_AUTOMAKE([1.9 foreign])

AC_PROG_CC
AM_PROG_LIBTOOL

dnl check for pkg-config
AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

dnl ============================================================================
dnl check host system

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

dnl ============================================================================
dnl check for swig
AC_CHECK_PROG(have_swig, "swig", yes, no)

dnl ============================================================================
dnl build tiff

build_tiff=yes

AC_ARG_ENABLE([tiff], 
  [AS_HELP_STRING([--disable-tiff],
  [do not use libtiff])],
  [build_tiff=no], [build_tiff=yes])

dnl check for libtiff

if test "$build_tiff" = "yes"; then
    AC_CHECK_LIB([tiff], [TIFFOpen], build_tiff=yes, build_tiff=no)

    if test "$build_tiff" = "yes"; then
        tiff_CFLAGS=" -DUSE_TIFF"
        tiff_LIBS=" -ltiff"
   
        AC_SUBST(tiff_CFLAGS)
        AC_SUBST(tiff_LIBS)
        
        ld_tiff="$tiff_LIBS"
        
        dnl export ld_tiff librarys to pkg-config pc file 
        AC_SUBST(ld_tiff)
    fi
fi

AM_CONDITIONAL(WITH_TIFF, test "$build_tiff" = yes)

dnl ============================================================================
dnl build fftw3

build_fftw=yes

AC_ARG_ENABLE([fftw], 
  [AS_HELP_STRING([--disable-fftw],
  [do not use fftw])],
  [build_fftw=no], [build_fftw=yes])

dnl check for fftw3

if test "$build_fftw" = "yes"; then
    fftw_pkg_modules="fftw3"
    PKG_CHECK_MODULES(fftw, [$fftw_pkg_modules], build_fftw=yes, build_fftw=no)
    
    if test "$build_fftw" = "yes"; then
        fftw_CFLAGS+=" -DUSE_FFTW"
        AC_SUBST(fftw_CFLAGS)
        AC_SUBST(fftw_LIBS)
    fi
fi

AM_CONDITIONAL(WITH_FFTW, test "$build_fftw" = yes)

dnl ============================================================================
dnl check for glib

build_lib=yes

AC_ARG_ENABLE([glib], 
  [AS_HELP_STRING([--disable-glib],
  [do not use glib])],
  [build_glib=no], [build_glib=yes])

dnl check for glib

if test "$build_glib" = "yes"; then
    glib_pkg_modules="glib-2.0"
    PKG_CHECK_MODULES(glib, [$glib_pkg_modules], build_glib=yes, build_glib=no)
    
    if test "$build_glib" = "yes"; then
        glib_CFLAGS+=" -DUSE_GLIB"
        AC_SUBST(glib_CFLAGS)
        AC_SUBST(glib_LIBS)
    fi
fi

AM_CONDITIONAL(WITH_GLIB, test "$build_glib" = yes)

dnl ============================================================================
dnl check for gtk

build_gtk=yes

AC_ARG_ENABLE([gtk], 
  [AS_HELP_STRING([--disable-gtk],
  [do not use gtk])],
  [build_gtk=no], [build_gtk=yes])

dnl check for gtk

if test "$build_gtk" = "yes"; then
    gtk_pkg_modules="gtk+-2.0"
    PKG_CHECK_MODULES(gtk, [$gtk_pkg_modules], build_gtk=yes, build_gtk=no)
    
    if test "$build_gtk" = "yes"; then
        gtk_CFLAGS+=" -DUSE_GTK"
        AC_SUBST(gtk_CFLAGS)
        AC_SUBST(gtk_LIBS)
    
        requires_gtk="$gtk_pkg_modules"
    
        dnl export required gtk librarys to pkg-config pc file 
        AC_SUBST(requires_gtk)
    fi
fi

AM_CONDITIONAL(WITH_GTK, test "$build_gtk" = yes)

dnl ============================================================================
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
		  
	  else
		  AC_MSG_RESULT(no)
		  build_python=no
	  fi
fi

if test "$have_swig" = "no"; then
  build_python=no
fi

if test "$build_tiff" = "no"; then
  build_python=no
fi

if test "$build_fftw" = "no"; then
  build_python=no
fi

AM_CONDITIONAL(WITH_PYTHON, test "$build_python" = "yes")

dnl ============================================================================
dnl build hspell

build_hspell=yes

AC_ARG_ENABLE([hspell], 
  [AS_HELP_STRING([--disable-hspell],
  [do not use hspell])],
  [build_hspell=no], [build_hspell=yes])

dnl check for hspell

if test "$build_hspell" = "yes"; then
    AC_CHECK_LIB([hspell], [hspell_get_dictionary_path], build_hspell=yes, build_hspell=no, -lz)

    if test "$build_hspell" = "yes"; then
        hspell_CFLAGS=" -DUSE_HSPELL"
        hspell_LIBS=" -lhspell -lz"
       
        AC_SUBST(hspell_CFLAGS)
        AC_SUBST(hspell_LIBS)
        
        ld_hspell="$hspell_LIBS"
        
        dnl export ld_hspell librarys to pkg-config pc file 
        AC_SUBST(ld_hspell)
    fi
fi

AM_CONDITIONAL(WITH_HSPELL, test "$build_hspell" = yes)

dnl ============================================================================
dnl build hocr

build_hocr=yes

AC_ARG_ENABLE([hocr], 
  [AS_HELP_STRING([--disable-hocr],
  [do not build hocr utility program])],
  [build_hocr=no], [build_hocr=yes])

if test "$build_glib" = "no"; then
  build_hocr=no
fi

AM_CONDITIONAL(WITH_HOCR, test "$build_hocr" = "yes")

dnl ============================================================================
dnl build hocr-gtk

build_hocr_gtk=yes

AC_ARG_ENABLE([hocr-gtk], 
  [AS_HELP_STRING([--disable-hocr-gtk],
  [do not build hocr-gtk utility program])],
  [build_hocr_gtk=no], [build_hocr_gtk=yes])

if test "$build_python" = "no"; then
  build_hocr_gtk=no
fi

AM_CONDITIONAL(WITH_HOCR_GTK, test "$build_hocr_gtk" = "yes")

dnl ============================================================================
dnl create a build string

BUILD_DATE=`date +%d%m%Y`
BUILD_STR="$PACKAGE-$VERSION-$build-$BUILD_DATE"
if test "$build_tiff" = "yes"; then
BUILD_STR+="-tiff"
fi
if test "$build_fftw" = "yes"; then
BUILD_STR+="-fftw3"
fi
if test "$build_hspell" = "yes"; then
BUILD_STR+="-hspell"
fi
if test "$build_gtk" = "yes"; then
BUILD_STR+="-gtk"
fi
AC_SUBST(BUILD_STR)

dnl ============================================================================
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
echo have swig:	${have_swig}
echo pkg-config path: ${PKG_CONFIG}
echo
echo with glib: ${build_glib}
echo with gtk: ${build_gtk}
echo with hspell: ${build_hspell}
echo with libtiff: ${build_tiff}
echo with fftw: ${build_fftw}
echo
echo build python binding, requires swig, libtiff, fftw: ${build_python}
echo python site lib path: ${PY_MODULES_PATH}
echo
echo build hocr, requires glib: ${build_hocr}
echo build hocr-gtk, requires python bindings: ${build_hocr_gtk}
echo
