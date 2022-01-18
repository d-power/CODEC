#/bin/sh

show_help()
{
	echo "  -h help information:"
	echo "  -vd make video decode test"
	echo "  -ve make video encode test"
	echo "  -ae make audio encode test"
	echo "  -ad make audio decode test"
	echo "  -aio make audio input output test"
	echo "  -all make all test file"
	echo "  -rm remove all exec file"
}

build_vd()
{
	arm-linux-gnueabi-gcc VDecodeTest.c -o VDecodeTest -L../lib -L../lib/depend -I../include -lx5vd -lcdc_base -lcdc_memory -lcdc_vdecoder -lcdc_vd_h264 -lcdc_vd_mjpeg -lcdc_vd_mjpegs -lcdc_ve -lpthread
}

build_ve()
{
	arm-linux-gnueabi-gcc VEncodeTest.c -o VEncodeTest -L../lib -L../lib/depend -I../include -lx5ve -lcdc_base -lcdc_memory -lcdc_vencoder -lcdc_ve -lpthread
}

build_ad()
{
	arm-linux-gnueabi-gcc ADecodeTest.c -o ADecodeTest -L../lib -L../lib/depend -I../include -lx5ad -lavcodec -lavutil -lavformat -lmp3lame -lfdk-aac -lAudioCore -lasound -lpthread
}

build_ae()
{
	arm-linux-gnueabi-gcc AEncodeTest.c -o AEncodeTest -L../lib -L../lib/depend -I../include -lx5ae -lavcodec -lavutil -lavformat -lmp3lame -lfdk-aac -lAudioCore -lasound -lpthread
}

build_aio()
{
	arm-linux-gnueabi-gcc AIOTest.c -o AIOTest -L../lib -L../lib/depend -I../include -lx5ae -lx5ad -lavcodec -lavutil -lavformat -lmp3lame -lfdk-aac -lAudioCore -lasound -lpthread
}


case $1 in
	"-h")
		show_help
		;;
	"-vd")
		build_vd
		;;
	"-ve")
		build_ve
		;;
	"-ad")
		build_ad
		;;
	"-ae")
		build_ae
		;;
	"-aio")
		build_aio
		;;
	"-all")
		build_vd
		build_ve
		build_ad
		build_ae
		build_aio
		;;
	"-rm")
		rm AIOTest
		rm AEncodeTest
		rm ADecodeTest
		rm VEncodeTest
		rm VDecodeTest
		;;
	*)
		show_help
		;;
esac

