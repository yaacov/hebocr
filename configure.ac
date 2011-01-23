dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)

AC_INIT([hebocr], [0.12.0-dev], [Diego Iastrubni <diegoiast@gmail.com>])
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
AC_SUBST(VERSION)

dnl ============================================================================
dnl output

AC_CONFIG_FILES([
Makefile
hebocr.pc
Doxyfile
src/Makefile
examples/Makefile
examples/hocr/Makefile
examples/bindings/Makefile
bindings/Makefile
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
echo with hspell: ${build_hspell}
echo with libtiff: ${build_tiff}
echo with fftw: ${build_fftw}
echo
