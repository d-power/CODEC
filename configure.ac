-e # -*- Autoconf -*-

 # Process this file with autoconf to produce a configure script.

 AC_PREREQ([2.69]) AC_INIT([CODEC], [1.0.0], [liuzhengzhong@d-power.com.cn])

 CODEC_MAJOR_VERSION=1
 CODEC_MINOR_VERSION=0
 CODEC_MICRO_VERSION=0

 CODEC_VERSION=$CODEC_MAJOR_VERSION.$CODEC_MINOR_VERSION.$CODEC_MICRO_VERSION

 AC_SUBST(CODEC_VERSION)

 AC_CANONICAL_SYSTEM

 AM_INIT_AUTOMAKE()
 AC_CONFIG_HEADERS([include/config.h])

 AC_PROG_LIBTOOL
 AC_PROG_CC
 AC_PROG_CXX
 AC_PROG_RANLIB
 AC_PROG_INSTALL
 AC_PROG_LN_S
 AC_PROG_MAKE_SET

 AC_HEADER_STDC
 AC_HEADER_DIRENT
 AC_CHECK_HEADERS(sys/types.h sys/time.h)

 AC_CHECK_DECLS([strlen, strdup])

 AC_C_CONST
 AC_C_INLINE
 AC_TYPE_SIZE_T
 AC_HEADER_TIME
 AC_STRUCT_TM

 AC_FUNC_ALLOCA
 AC_FUNC_MEMCMP
 AC_FUNC_MMAP
 AC_FUNC_VPRINTF
 AC_CHECK_FUNCS(time localtime memset munmap)

 AC_OUTPUT(
 Makefile
 src/Makefile
 test/Makefile
 )


