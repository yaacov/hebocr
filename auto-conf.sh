#! /bin/sh

set -x

automake --add-missing
autoreconf
automake --add-missing

mkdir build
cd build
../configure --prefix=$HOME/hebocr
make
make install
