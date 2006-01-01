// file : hocr.i
%module hocr
%{
#include "../../src/hocrpp.h"
%}

%include ../../src/hocrpp.h

// on linux do:
// swig -python hocr.i
// g++ -fpic -c hocr_wrap.cxx -I/usr/include/python2.3
// g++ -shared ../../src/*.o hocr_wrap.o -o _hocr.so
