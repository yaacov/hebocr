// file : hocr.i
%module hocr
%{
#include "../../src/hocrpp.h"
%}

%typemap(out) string_data {
    $result = PyString_FromStringAndSize($1.data,$1.size);
}

%include ../../src/ho_pixbuf.h
%include ../../src/ho_bitmap.h
%include ../../src/ho_objmap.h
%include ../../src/ho_layout.h
%include ../../src/ho_dimentions.h
%include ../../src/ho_segment.h
%include ../../src/ho_font.h
%include ../../src/ho_recognize.h
%include ../../src/ho_recognize_nikud.h
%include ../../src/ho_string.h

%include ../../src/hocr.h
%include ../../src/hocrpp.h

// on linux do:
// swig -python -c++ hocr.i
// g++ -fpic -c hocr_wrap.cxx -I/usr/include/python2.3
// g++ -shared .../../src/.libs/libhocr.so hocr_wrap.o -o _hocr.so
