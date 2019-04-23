bin_PROGRAMS = ADecodeTest AEncodeTest AIOTest VDecodeTest VEncodeTest WaterMarkTest H2642JPEGTest

AM_CFLAGS = -I$(top_builddir)/third_party/include/
AM_CPPFLAGS = -I$(top_builddir)/third_party/include/

ADecodeTest_SOURCES = ADecodeTest.c
ADecodeTest_DEPS = 
ADecodeTest_LDADD = $(ADecodeTest_DEPS) -lAudioCodec
ADecodeTest_DEPENDENCIES = $(ADecodeTest_DEPS)

AEncodeTest_SOURCES = AEncodeTest.c
AEncodeTest_DEPS = 
AEncodeTest_LDADD = $(AEncodeTest_DEPS) -lAudioCodec
AEncodeTest_DEPENDENCIES = $(AEncodeTest_DEPS)

AIOTest_SOURCES = AIOTest.c
AIOTest_DEPS = 
AIOTest_LDADD = $(AIOTest_DEPS) -lAudioIn -lAudioOut -lAudioCore -lasound
AIOTest_DEPENDENCIES = $(AIOTest_DEPS)

VDecodeTest_SOURCES = VDecodeTest.c
VDecodeTest_DEPS = 
VDecodeTest_LDADD = $(VDecodeTest_DEPS) -lVideoDec -lcdc_base -lcdc_memory -lcdc_vdecoder -lcdc_ve -lcdc_vencoder
VDecodeTest_DEPENDENCIES = $(VDecodeTest_DEPS)

VEncodeTest_SOURCES = VEncodeTest.c
VEncodeTest_DEPS = 
VEncodeTest_LDADD = $(VEncodeTest_DEPS) -lVideoEnc -lcdc_base -lcdc_memory -lcdc_vdecoder -lcdc_ve -lcdc_vencoder
VEncodeTest_DEPENDENCIES = $(VEncodeTest_DEPS)

WaterMarkTest_FILES = WaterMarkTest.c
WaterMarkTest_SOURCE = $(WaterMarkTest_FILES)
WaterMarkDEPS = 
WaterMarkTest_LDADD = $(WaterMarkDEPS) -lcdc_base -lcdc_vdecoder -lcdc_memory -lcdc_ve -lcdc_vencoder -lVideoEnc -lVideoDec
WaterMarkTest_DEPENDENCIES = $(WaterMarkDEPS)

H2642JPEGTest_FILES = H2642JPEGTest.c
H2642JPEGTest_SOURCE = $(H2642JPEGTest_FILES)
H2642JPEGDEPS = 
H2642JPEGTest_LDADD = $(H2642JPEGDEPS) -lcdc_base -lcdc_vdecoder -lcdc_memory -lcdc_ve -lcdc_vencoder -lVideoEnc -lVideoDec
H2642JPEGTest_DEPENDENCIES = $(H2642JPEGDEPS)