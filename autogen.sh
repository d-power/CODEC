#/bin/sh

case $1 in
	"-ProName")
		Name=$2
		;;
	*)
		echo "Must Specify Project Name, Use -ProName"
        exit
		;;
esac


mkdir include
mkdir src
mkdir test


configure='# -*- Autoconf -*-\n\n
# Process this file with autoconf to produce a configure script.\n\n
AC_PREREQ([2.69])
AC_INIT(['$Name'], [1.0.0], [liuzhengzhong@d-power.com.cn])\n\n
'$Name'_MAJOR_VERSION=1\n
'$Name'_MINOR_VERSION=0\n
'$Name'_MICRO_VERSION=0\n\n
'$Name'_VERSION=$'$Name'_MAJOR_VERSION.$'$Name'_MINOR_VERSION.$'$Name'_MICRO_VERSION\n\n
AC_SUBST('$Name'_VERSION)\n\n
AC_CANONICAL_SYSTEM\n\n
AM_INIT_AUTOMAKE()\n
AC_CONFIG_HEADERS([include/config.h])\n\n
AC_PROG_LIBTOOL\n
AC_PROG_CC\n
AC_PROG_CXX\n
AC_PROG_RANLIB\n
AC_PROG_INSTALL\n
AC_PROG_LN_S\n
AC_PROG_MAKE_SET\n\n
AC_HEADER_STDC\n
AC_HEADER_DIRENT\n
AC_CHECK_HEADERS(sys/types.h sys/time.h)\n\n
AC_CHECK_DECLS([strlen, strdup])\n\n
AC_C_CONST\n
AC_C_INLINE\n
AC_TYPE_SIZE_T\n
AC_HEADER_TIME\n
AC_STRUCT_TM\n\n
AC_FUNC_ALLOCA\n
AC_FUNC_MEMCMP\n
AC_FUNC_MMAP\n
AC_FUNC_VPRINTF\n
AC_CHECK_FUNCS(time localtime memset munmap)\n\n
AC_OUTPUT(\n
Makefile\n
src/Makefile\n
test/Makefile\n
)\n\n'

echo -e $configure > configure.ac

echo 'AUTOMAKE_OPTIONS=foreign

PROJECT_PATH=$(shell /bin/pwd)

AM_CFLAGS = -I$(PROJECT_PATH)/include
AM_CPPFLAGS = -I$(PROJECT_PATH)/include

SUBDIRS = src test' > Makefile.am

echo '' > src/Makefile.am
echo 'bin_PROGRAMS = TestDemo

TestDemo_FILES = TestDemo.c
TestDemo_SOURCES = $(TestDemo_FILES)
TestDemoDEPS =
TestDemo_LDADD = $(TestDemoDEPS)
TestDemo_DEPENDENCIES = $(TestDemoDEPS)' > test/Makefile.am

echo '#!/bin/sh
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
        CFLAGS="-g2" \
        CXXFLAGS="-g2" \
        LDFLAGS="" \
        LIBS="" \
        --enable-static \
        --enable-shared' > build.sh

echo '#include <stdio.h>

int main() {
    printf("HelloWorld");
}' > test/TestDemo.c

aclocal
autoheader
libtoolize --automake --copy --debug --force
automake --add-missing
autoconf

chmod -R 777 *
