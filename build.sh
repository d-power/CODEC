#!/bin/sh

export PATH=$PATH:/home/sharedir/X5/arm-toolchain/ext-toolchain/bin
rm config.sub
aclocal
autoheader
libtoolize --automake --copy --debug --force
automake --add-missing
autoconf

#rm -rf Release

mkdir Release
mkdir -p Release/lib/depend


./configure --prefix=$(pwd)/Release \
        --host=arm-linux-gnueabi \
        CFLAGS="-I$(pwd)/third_party/include/  -g2  -Wall -march=armv7-a -mfpu=neon -D_GNU_SOURCE " \
        CXXFLAGS="-I$(pwd)/third_party/include/  -g2  -Wall -march=armv7-a -mfpu=neon -D_GNU_SOURCE" \
        LDFLAGS="-L$(pwd)/third_party/lib/" \
        LIBS="-lpthread -ldl" \
        --disable-static \
        --enable-shared
