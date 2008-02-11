// file : hocr.i
%module hocr
%{
#include "../../src/hocrpp.h"
#include "../../src/ho_gtk.h"
%}

%include ../../src/ho_pixbuf.h
%include ../../src/ho_bitmap.h
%include ../../src/ho_objmap.h
%include ../../src/ho_layout.h
%include ../../src/ho_dimentions.h
%include ../../src/ho_segment.h
%include ../../src/ho_font.h
%include ../../src/ho_recognize.h
%include ../../src/ho_string.h

%include ../../src/ho_gtk.h
%include ../../src/hocr.h
%include ../../src/hocrpp.h

// on linux do:
// swig -python -c++ hocr.i
// g++ -fpic -c hocr_wrap.cxx `pkg-config --cflags gtk+-2.0` -I/usr/include/python2.3
// g++ -shared `pkg-config --libs gtk+-2.0`.../../src/.libs/libhocr.so ../../src/.libs/libhocrgtk.so  hocr_wrap.o -o _hocr.so
