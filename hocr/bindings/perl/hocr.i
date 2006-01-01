// file : hocr.i
%module hocr
%{
#include "../../src/hocrpp.h"
%}

%include ../../src/hocrpp.h

// on linux do:
// swig -perl hocr.i
// g++ -fpic -c hocr_wrap.cxx -I/usr/lib/perl5/5.8.6/i686-linux/CORE/
// g++ -shared ../../src/*.o hocr_wrap.o -o _hocr.so
