#!/bin/sh
rm config.sub
aclocal
autoheader
libtoolize --automake --copy --debug --force
automake --add-missing
autoconf
mkdir Release
mkdir -p Release/lib/depend
mkdir -p Release/test
./configure --prefix=$(pwd)/Release \
        --host=arm-linux-gnueabi \
        CFLAGS="-O2" \
        CXXFLAGS="-O2" \
        LDFLAGS="-L$(pwd)/third_party/lib" \
        LIBS="-lpthread" \
        --enable-static \
        --enable-shared
