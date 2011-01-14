#! /bin/sh

set -x

automake --add-missing
autoreconf
automake --add-missing

mkdir build
#cd build
#../configure --prefix=$HOME/hebocr
./configure --prefix=$HOME/hebocr
make
make install
make dist
make dist-lzma
make dist-bzip2
make dist-xz

