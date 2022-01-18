/******************************************************************************
Copyright(c) 2016-2018 Digital Power Inc.
File name: VDecodeTest.c
Author: LiuZhengzhong
Version: 1.0.0
Date: 2018/3/9
Description: Platform of DP X5/X1H video decode test
History:
Bug report: liuzhengzhong@d-power.com.cn
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <pthread.h>
#include <stdarg.h>
#include <getopt.h>

#include <video/vd/VDecApi.h>
#include <video/vd/VoApi.h>

// 解码类型枚举
typedef enum _DecType_E
{
    DecH264 = 1,
    DecMJPEG,
    DecBUT,

} DecType_E;

DecType_E DecType;

// 根据传入参数赋值的解码源文件
static char SrcFile[64] = {0};

static const char *Usage =
    "********************************************************************************\n"
    "OVERVIEW: Video Decode Function Test Application! \n"
    "Usage: ./VDecodeTest [options] <arg> \n"
    "\nOPTIONS:\n"
    "-? [--help]            Print VDecodeTest Usage Information And Exit \n"
    "-t [--type]            Decode Type : 1.H264 2.MJPEG, default is 1 \n"
    "-s [--sf]              Source File To Decode. \n"
    "-r [--sr]              Source Stream Clock Rate, default is 15. \n"
    "-W [--dw]              Display Width, default is 320. \n"
    "-H [--dh]              Display High, default is 240. \n"
    "-X [--dx]              Display X, default is 0. \n"
    "-Y [--dy]              Display Y, default is 0. \n"
    "\nDemo: ./VDecodeTest -t 1 -s H264.dat -r 15 -W 320 -H 240 -X 0 -Y 0 \n"
    "\n********************************************************************************\n";

int PrintUsage()
{
    printf("%s\n", Usage);
    return 0;
}

// 解码源帧率
static int DEC_RATE = 30;

// 屏幕显示宽度
static int DISP_W = 320;
// 屏幕显示高度
static int DISP_H = 240;

// 屏幕显示位置X值
static int DISP_X = 0;
// 屏幕显示位置Y值
static int DISP_Y = 0;

// 显示通道
#define DISP_CHANNEL 0
// 显示图层
#define DISP_LAYER 0

#define DEC_BUF_SIZE 128 * 1024

//打开或注释该行 用于切换四分屏
// #define FOURSPLITE	4

#ifndef FOURSPLITE
#define FOURSPLITE	1
#endif

const char *DecResult[] = {
    "VDEC_RESULT_OK",
    "VDEC_FRAME_DECODED",
    "VDEC_CONTINUE",
    "VDEC_KEYFRAME_DECODED",
    "VDEC_NO_FRAME_BUFFER",
    "VDEC_NO_BITSTREAM",
    "VDEC_RESOLUTION_CHANGE",
    "VDEC_UNSUPPORTED",
    "VDEC_CHANNEL_ERROR"};

void FlushFrameBuffer(unsigned int color)
{
    int FbFd;

    FbFd = open("/dev/fb0", O_RDWR, 0);

    if (!FbFd)
    {
        printf("Failed To Open File :%s", "/dev/fb0");
        return;
    }

    struct fb_fix_screeninfo finfo;

    // 获取fb设备信息
    if (ioctl(FbFd, FBIOGET_FSCREENINFO, &finfo) < 0)
    {
        printf("Couldn't get console hardware info");
        return;
    }

    // 偏移量，按页计算，通常为0
    unsigned int FbMemOffset = (((long)finfo.smem_start) -
                                (((long)finfo.smem_start) & ~(getpagesize() - 1)));
    // 获取出的fb长度
    unsigned int FbMemLen = finfo.smem_len + FbMemOffset;
    // fb的起始地址
    unsigned char *FbMem = mmap(NULL, FbMemLen,
                                PROT_READ | PROT_WRITE, MAP_SHARED, FbFd, 0);

    if (FbMem == (unsigned char *)-1)
    {
        printf("Unable to memory map the video hardware");
        FbMem = NULL;
        return;
    }

    struct fb_var_screeninfo vinfo;

    if (ioctl(FbFd, FBIOGET_VSCREENINFO, &vinfo) < 0)
    {
        printf("Couldn't get console pixel format");
        return;
    }

    // 从驱动获取出来的宽高是错的
    printf("%d %d %d\n", vinfo.xres, vinfo.yres, finfo.line_length);

    // 屏幕宽度
    // unsigned int FbWidth = vinfo.xres;
    unsigned int FbWidth = 1024;
    // 屏幕高度
    // unsigned int FbHeight = vinfo.yres;
    unsigned int FbHeight = 600;
    // 屏幕一行字节数
    // unsigned int FbPitch = finfo.line_length;
    unsigned int FbPitch = 1024 * 4;

    // 屏幕一个点字节数
    unsigned int FbPixelByte = FbPitch / FbWidth;
    // unsigned int FbRestXOffset = 0;

    // unsigned char *FbMem2 = FbMem + FbPitch * FbHeight;

    unsigned int *tmp = (unsigned int *)FbMem;

    int i, j;

    for (i = 0; i < FbHeight; i++)
    {
        for (j = 0; j < FbWidth; j++)
        {
            tmp[j] = color;
        }
        tmp += FbPitch / FbPixelByte;
    }

    munmap(FbMem, FbMemLen);
    FbMem = NULL;

    close(FbFd);
    FbFd = -1;

    return;
}

void DecH264Display(VO_LAYER_INFO * prt)
{
    // 4个图层，从优先级低使能，优先级0 < 1 < 2 < 3
    // 不允许不使能Channel 0，Layer 0，直接使用Channel 0，Layer 1

    VO_LAYER_INFO VoInfo;
    memset(&VoInfo, 0, sizeof(VO_LAYER_INFO));
	if (prt == NULL)
	{
	    VoInfo.Rect.X = DISP_X;
	    VoInfo.Rect.Y = DISP_Y;
	    VoInfo.Rect.W = DISP_W;
	    VoInfo.Rect.H = DISP_H;
		// VO_EnableChn(DISP_CHANNEL, &VoInfo);等价于VO_EnableVideoLayer(DISP_CHANNEL, 0, &VoInfo);
        //VoInfo.Rotate = VO_ROTATE_270;
		 if (!VO_EnableChn(DISP_CHANNEL, &VoInfo))
			 printf("VO_EnableChn Failed!!!\n");
	}
	else
	{
		static int ilayer = 0;
    	memcpy(&VoInfo, prt, sizeof(VO_LAYER_INFO));
		VO_EnableVideoLayer(DISP_CHANNEL, ilayer++, &VoInfo);
	}
}

void *DecThread(void *Argv)
{
    //SetVideShowRotate(VDEC_ROTATE_270);
    VDEC_CHN_ATTR_S Attr;
    memset(&Attr, 0, sizeof(VDEC_CHN_ATTR_S));
    Attr.deType = VDEC_PAYLOAD_TYPE_H264;
	Attr.BufSize = 0x80000;//512KB
	Attr.u32PicWidth = 1920;
	Attr.u32PicHight = 1088;
    int DecChannel[FOURSPLITE];
    // 创建解码通道，attr全部为0，即全部使用默认值
    int chncnt = 0;
    for (int i = 0; i < FOURSPLITE;i++)
	{
	    DecChannel[i] = VDEC_CreateChn(&Attr);
	    if (DecChannel[i] >= 0)
	    {
	        printf("CreateChn DecChannel[%d] Successful\n",DecChannel[i]);
			chncnt++;
	    }
	    else
	    {
	        printf("Create First DecChannel[%d] Failed!\n", DecChannel[i]);
	    }
	}
	if (chncnt == 0)
		return NULL;
    FILE *Fp;

    // unsigned int FlenSize;
    unsigned int Flen;
    int i;
    unsigned char *Buf;

    Fp = fopen(SrcFile, "rb");

    if (Fp == NULL)
    {
        printf("Open SrcFile[%s] Failed!\n", SrcFile);
        return NULL;
    }

    FILE *DecFd = fopen("dec264.yuv", "wb");
    if (DecFd == NULL)
    {
        printf("Open Dec YUV file Failed!\n");
        return NULL;
    }

    Buf = (unsigned char *)malloc(DEC_BUF_SIZE);

    VDEC_STREAM_S Stream;
    memset(&Stream, 0, sizeof(VDEC_STREAM_S));

    VDEC_FRAME_S Frame;
    memset(&Frame, 0, sizeof(VDEC_FRAME_S));

    for (i = 0; i < 100; i++)
    {
        if (Fp == NULL)
        {
            printf("file null!!\n");
            break;
        }

        if (fread(&Flen, 1, sizeof(Flen), Fp) != sizeof(Flen))
        {
            printf("fread error!!\n");
            break;
        }

        if (Flen <= 0 || Flen > DEC_BUF_SIZE)
        {
            printf("file len [%d] error!!\n", Flen);
            break;
        }

        if (fread(Buf, 1, Flen, Fp) != Flen)
        {
            printf("fread error!!\n");
            break;
        }

        Stream.pu8Addr = Buf;
        Stream.u32Len = Flen;
        Stream.u64PTS = -1;

        printf("decode [%d] frame\n", i);

#if defined(FOURSPLITE) && (FOURSPLITE == 1)
		// 测试解码区域缩放
        if (i == 50)
        {

            VO_LAYER_RECT Rect;
            memset(&Rect, 0, sizeof(VO_LAYER_RECT));

            Rect.X = 100;
            Rect.Y = 100;
            Rect.W = DISP_W / 2;
            Rect.H = DISP_H / 2;
			//VO_Resize 要和解码互斥 没有加锁 所以调用需注意
            VO_Resize(DISP_CHANNEL, DISP_LAYER, &Rect);
        }
#endif


        VDEC_CHN_STAT_S Stat;
        memset(&Stat, 0, sizeof(VDEC_CHN_STAT_S));

        // 获取当前状态
        if (!VDEC_Query(DecChannel[0], &Stat))
        {
            printf("VDEC_Query Error!\n");
            continue;
        }

        if (Stat.u32FrameNum > 0)
        {
            int Ret;
            // 解码一帧数据
            Ret = VDEC_StartRecvStream(DecChannel[0], 0, 0, 0);

            switch (Ret)
            {
            case VDEC_NO_BITSTREAM:
                usleep(40000);
                break;

            case VDEC_KEYFRAME_DECODED:
            case VDEC_FRAME_DECODED:
                // 获取解码数据
                for (int n = 0;n < FOURSPLITE;n++)
            	{
            		  // 向解码器送一帧数据
			        if (!VDEC_SendStream(DecChannel[n], &Stream))
			        {
			            printf("VDEC_SendStream Error!\n");
			            usleep(10000);
			            continue;
			        }
	                if (!VDEC_GetImage(DecChannel[n], &Frame))
	                {
	                    printf("VDEC_GetImage Error!\n");
	                    continue;
	                }

	                // 由于该解码器，Y数据与UV数据物理地址并不连续，所以需要分别写入
	                // 数据量太大，仅测试使用
	                // fwrite(Frame.pVirAddr[0], 1, Frame.SrcInfo.W * Frame.SrcInfo.H, DecFd);
	                // fwrite(Frame.pVirAddr[1], 1, Frame.SrcInfo.W * Frame.SrcInfo.H / 2, DecFd);

	                if (!VO_SetZoomInWindow(DISP_CHANNEL, DISP_LAYER, &Frame.SrcInfo))
	                {
	                    printf("VO_SetZoomInWindow Error!\n");
	                    continue;
	                }

	                if (!VO_ChnShow(DISP_CHANNEL, DecChannel[n], &Frame))
	                {
	                    printf("VO_ChnShow Error!\n");
	                    continue;
	                }

	                // 释放解码数据
	                if (!VDEC_ReleaseImage(DecChannel[n], &Frame))
	                {
	                    printf("VDEC_ReleaseImage Error!\n");
	                    continue;
	                }
            	}
                break;

            default:
                continue;
            }
        }

        usleep(1000 / DEC_RATE * 1000);
    }

    if (Buf)
    {
        free(Buf);
        Buf = NULL;
    }

    fclose(DecFd);
    DecFd = NULL;

    fclose(Fp);
    Fp = NULL;

	for (int i = 0;i < FOURSPLITE;i++)
    	VDEC_DestroyChn(DecChannel[i]);

    return NULL;
}

/* This is for demo only. Should be replace with real H264 SPS/PPS/I/P encoder output */
int read_one_frame_from_demo_video_file(unsigned char *pVideoBuf,unsigned int offset,unsigned int BufSize,unsigned char *IskeyFrame,unsigned int*FramLen,unsigned int *Frame_start)
{
    int pos = 0;
    int bNeedCal = 0;
    unsigned char NalType=0;
    int idx=0;
    if(BufSize<=5)
    {
        printf("bufSize is too small\n");
        return -1;
    }
    for(pos=0;pos <= BufSize-5;pos++)
    {
        if(pVideoBuf[pos]==0x00
            &&pVideoBuf[pos+1]==0x00
            &&pVideoBuf[pos+2]==0x00
            &&pVideoBuf[pos+3]==0x01)
        {
            NalType = pVideoBuf[pos + 4] & 0x1f;
            if(NalType == 0x7)
            {
                if(bNeedCal==1)
                {
                    *FramLen=pos-idx;
                    return 0;
                }

                *IskeyFrame = 1;
                *Frame_start = offset+pos;
                bNeedCal=1;
                idx=pos;
            }
            else if(NalType ==0x1)
            {
               if(bNeedCal)
               {
                  *FramLen=pos-idx;
                  return 0;
               }
               *Frame_start=offset+pos;
               *IskeyFrame = 0;
               idx = pos;
               bNeedCal=1;
            }
        }
    }

    return 0;
}


void *DecThread22(void *Argv)
{
    VDEC_CHN_ATTR_S Attr;
    memset(&Attr, 0, sizeof(VDEC_CHN_ATTR_S));
    // Attr.deType = VDEC_PAYLOAD_TYPE_H264;
	//Attr.BufSize = 0x80000;//512KB
	Attr.u32PicWidth = 800;
	Attr.u32PicHight = 1280;
    int DecChannel[FOURSPLITE];
    // 创建解码通道，attr全部为0，即全部使用默认值
    int chncnt = 0;
    for (int i = 0; i < FOURSPLITE;i++)
	{
	    DecChannel[i] = VDEC_CreateChn(&Attr);
	    if (DecChannel[i] >= 0)
	    {
	        printf("CreateChn DecChannel[%d] Successful\n",DecChannel[i]);
			chncnt++;
	    }
	    else
	    {
	        printf("Create First DecChannel[%d] Failed!\n", DecChannel[i]);
	    }
	}
	if (chncnt == 0)
		return NULL;


    unsigned int Flen;
    int i=0;

    FILE *streamBin_fp = fopen(SrcFile, "rb");
    if((streamBin_fp == NULL))
    {
        printf("can't read live video file %s\n",SrcFile);
        pthread_exit(0);
    }
    fseek(streamBin_fp, 0, SEEK_END);
    unsigned int  file_size = ftell(streamBin_fp);
    fseek(streamBin_fp, 0, SEEK_SET);
    unsigned char * pVideoBuf = (unsigned char *)malloc(file_size);
    fread(pVideoBuf, 1, file_size, streamBin_fp);

    unsigned int FrameLen=0,Frame_start=0;
    unsigned int offset=0;
    unsigned int IsKeyFrame=0;

    FILE *DecFd = fopen("dec264.yuv", "wb");
    if (DecFd == NULL)
    {
        printf("Open Dec YUV file Failed!\n");
        //return NULL;
    }

    VDEC_STREAM_S Stream;
    memset(&Stream, 0, sizeof(VDEC_STREAM_S));

    VDEC_FRAME_S Frame;
    memset(&Frame, 0, sizeof(VDEC_FRAME_S));

    while(1)
    {
        offset=Frame_start+FrameLen;
        if(offset>=file_size)
        {
            offset=0;
        }
        read_one_frame_from_demo_video_file(pVideoBuf+offset,offset,file_size-offset,(unsigned char *)&IsKeyFrame,&FrameLen,&Frame_start);

        Stream.pu8Addr = pVideoBuf+Frame_start;
        Stream.u32Len = FrameLen;
        Stream.u64PTS = -1;

        printf("decode2 [%d] frame len:%d Frame_start:%d FrameLen:%d\n", i++, file_size, Frame_start, FrameLen);
        i++;
        if (i == 50)
        {

            VO_LAYER_RECT Rect;
            memset(&Rect, 0, sizeof(VO_LAYER_RECT));

            Rect.X = 0;
            Rect.Y = 0;
            Rect.W = 800;//DISP_W / 2;
            Rect.H = 1280;//DISP_H / 2;
			//VO_Resize 要和解码互斥 没有加锁 所以调用需注意
            //VO_Resize(DISP_CHANNEL, DISP_LAYER, &Rect);
        }

        VDEC_CHN_STAT_S Stat;
        memset(&Stat, 0, sizeof(VDEC_CHN_STAT_S));

        // 获取当前状态
        if (!VDEC_Query(DecChannel[0], &Stat))
        {
            printf("VDEC_Query Error!\n");
            continue;
        }

        if (Stat.u32FrameNum > 0)
        {
            int Ret;
            // 解码一帧数据
            Ret = VDEC_StartRecvStream(DecChannel[0], 0, 0, 0);

            switch (Ret)
            {
            case VDEC_NO_BITSTREAM:
                usleep(40000);
                break;

            case VDEC_KEYFRAME_DECODED:
            case VDEC_FRAME_DECODED:
                // 获取解码数据
                for (int n = 0;n < FOURSPLITE;n++)
            	{
            		  // 向解码器送一帧数据
			        if (!VDEC_SendStream(DecChannel[n], &Stream))
			        {
			            printf("VDEC_SendStream Error!\n");
			            usleep(10000);
			            continue;
			        }
	                if (!VDEC_GetImage(DecChannel[n], &Frame))
	                {
	                    printf("VDEC_GetImage Error!\n");
	                    continue;
	                }
	                // 由于该解码器，Y数据与UV数据物理地址并不连续，所以需要分别写入
	                // 数据量太大，仅测试使用
	                //fwrite(Frame.pVirAddr[0], 1, Frame.SrcInfo.W * Frame.SrcInfo.H , DecFd);
	                //fwrite(Frame.pVirAddr[1], 1, Frame.SrcInfo.W * Frame.SrcInfo.H / 2, DecFd);

                    //printf("Frame.SrcInfo.W :%d Frame.SrcInfo.H :%d u32Stride:%d  x:%d y:%d w:%d h:%d\n", Frame.SrcInfo.W , Frame.SrcInfo.H, Frame.u32Stride, \
                    //     Frame.SrcInfo.Crop_X, Frame.SrcInfo.Crop_Y, Frame.SrcInfo.Crop_W, Frame.SrcInfo.Crop_H);

	                if (!VO_SetZoomInWindow(DISP_CHANNEL, DISP_LAYER, &Frame.SrcInfo))
	                {
	                    printf("VO_SetZoomInWindow Error!\n");
	                    continue;
	                }

                    //Frame.erotate = VDEC_ROTATE_270;
	                if (!VO_ChnShow(DISP_CHANNEL, DecChannel[n], &Frame))
	                {
	                    printf("VO_ChnShow Error!\n");
	                    //continue;
	                }

	                // 释放解码数据
	                if (!VDEC_ReleaseImage(DecChannel[n], &Frame))
	                {
	                    printf("VDEC_ReleaseImage Error!\n");
	                    continue;
	                }

            	}
                break;

            default:
                continue;
            }
        }

        //usleep(1000 / DEC_RATE * 1000);
        usleep(20 * 1000);
        //break;
    }

    free(pVideoBuf);

    if(DecFd)
    {
        fclose(DecFd);
        DecFd = NULL;
    }


    fclose(streamBin_fp);
    streamBin_fp = NULL;

	for (int i = 0;i < FOURSPLITE;i++)
    	VDEC_DestroyChn(DecChannel[i]);

    return NULL;
}


void DecH264VideoStream(void)
{
    pthread_t DecPid;

#if defined(FOURSPLITE) && (FOURSPLITE == 4)
	int w = DISP_W / 2;
	int h = DISP_H / 2;
	int x = DISP_X;
	int y = DISP_Y;
	VO_LAYER_INFO info1 = {x,y,w,h};
	VO_LAYER_INFO info2 = {x+w,y,w,h};
	VO_LAYER_INFO info3 = {x,y+h,w,h};
	VO_LAYER_INFO info4 = {x+w,y+h,w,h};
    DecH264Display(&info1);
	DecH264Display(&info2);
	DecH264Display(&info3);
	DecH264Display(&info4);
#else
	DecH264Display(NULL);
#endif
    pthread_create(&DecPid, NULL, DecThread22, NULL);

    pthread_join(DecPid, NULL);

#if defined(FOURSPLITE) && (FOURSPLITE == 4)
	//后创建的 要先关闭 避免图像一块一块的消失
    VO_DisableVideoLayer(DISP_CHANNEL, 3);
	VO_DisableVideoLayer(DISP_CHANNEL, 2);
	VO_DisableVideoLayer(DISP_CHANNEL, 1);
#endif
	VO_DisableVideoLayer(DISP_CHANNEL, 0);
}

void DecJpgPicuter(void)
{
    VO_LAYER_INFO VoInfo;
    memset(&VoInfo, 0, sizeof(VO_LAYER_INFO));

    VoInfo.Rect.X = DISP_X;
    VoInfo.Rect.Y = DISP_Y;
    // 可以在解码部分缩放，也可以在显示部分缩放
    VoInfo.Rect.W = DISP_W;
    VoInfo.Rect.H = DISP_H;

    unsigned int Channel = DISP_CHANNEL;

    // 使用显示通道0，图层0
    VO_EnableChn(Channel, &VoInfo);

    VDEC_CHN_ATTR_S attr;
    memset(&attr, 0, sizeof(VDEC_CHN_ATTR_S));

    attr.deType = VDEC_PAYLOAD_TYPE_MJPEG;
    // 使用默认值
    attr.BufSize = 0;
    attr.FormatType = FORMAT_YV12;
    // 可以不按比例缩放解码
    attr.u32PicWidth = 1280;//DISP_W;
    attr.u32PicHight = 720;//DISP_H;

    int DecChannel;

    // 创建解码通道
    DecChannel = VDEC_CreateChn(&attr);

    if (DecChannel >= 0)
    {
        printf("CreateChn Successful\n");
    }
    else
    {
        printf("Create Jpg DecChannel Failed!\n");
        return;
    }

    FILE *Fd;
    Fd = fopen(SrcFile, "rb");

    if (Fd == NULL)
    {
        printf("Open SrcFile[%s] Failed!\n", SrcFile);
        return;
    }

    fseek(Fd, 0, SEEK_END);
    int Len = ftell(Fd);

    unsigned char *Buf = (unsigned char *)malloc(Len);

    fseek(Fd, 0, SEEK_SET);
    fread(Buf, 1, Len, Fd);

    VDEC_STREAM_S Stream;
    memset(&Stream, 0, sizeof(VDEC_STREAM_S));

    VDEC_FRAME_S Frame;
    memset(&Frame, 0, sizeof(VDEC_FRAME_S));

    Stream.pu8Addr = Buf;
    Stream.u32Len = Len;
    Stream.u64PTS = -1;

    if (!VDEC_SendStream(DecChannel, &Stream)) // 向解码器送一帧数据
    {
        printf("VDEC_SendStream Error!\n");
    }

    VDEC_CHN_STAT_S Stat;
    memset(&Stat, 0, sizeof(VDEC_CHN_STAT_S));

    // 获取当前状态
    if (!VDEC_Query(0, &Stat))
    {
        printf("VDEC_Query Error!\n");
    }

    if (Stat.u32FrameNum > 0)
    {
        int Ret;
        // 解码一帧数据
        Ret = VDEC_StartRecvStream(DecChannel, 0, 1, 0);

        switch (Ret)
        {
        case VDEC_NO_BITSTREAM:
            printf("VDEC_NO_BITSTREAM\n");
            break;

        case VDEC_KEYFRAME_DECODED:
        case VDEC_FRAME_DECODED:
            // 获取解码数据
            if (!VDEC_GetImage(DecChannel, &Frame))
            {
                printf("VDEC_GetImage Error!\n");
            }

            printf("after decode, frame width = %d height = %d\n", Frame.SrcInfo.W, Frame.SrcInfo.H);

            FILE *DecFd = fopen("jpgdec.yuv", "wb");
            if (DecFd)
            {
                fwrite(Frame.pVirAddr[0], 1, Frame.SrcInfo.W * Frame.SrcInfo.H * 3 / 2, DecFd);
                fclose(DecFd);
            }

            // 用户在VDEC_GetImage成功后，可通过
            // Frame.pVirAddr获取YUV数据地址
            // 若解码格式YV12，Frame.pVirAddr[1]是V地址，Frame.pVirAddr[2]是U地址
            // 若解码格式NV21，Frame.pVirAddr[1]是VU混合地址

            if (!VO_SetZoomInWindow(0, 0, &Frame.SrcInfo))
            {
                printf("VO_SetZoomInWindow Error!\n");
            }

            if (!VO_ChnShow(Channel, 0, &Frame))
            {
                printf("VO_ChnShow Error!\n");
            }

            sleep(3);

            if (!VDEC_ReleaseImage(DecChannel, &Frame)) // 释放解码数据
            {
                printf("VDEC_ReleaseImage Error!\n");
            }

            break;

        default:
            break;
        }
    }

    if (Buf)
    {
        free(Buf);
        Buf = NULL;
    }

    fclose(Fd);
    Fd = NULL;
    VDEC_DestroyChn(DecChannel);
}

int main(int argc, char **argv)
{
    char *short_opt = "?:t:s:r:W:H:X:Y:";

    static struct option longopts[] =
        {{"help", no_argument, NULL, '?'},
         {"type", required_argument, NULL, 't'},
         {"sf", required_argument, NULL, 's'},
         {"sr", required_argument, NULL, 'r'},
         {"dw", required_argument, NULL, 'W'},
         {"dh", required_argument, NULL, 'H'},
         {"dx", required_argument, NULL, 'X'},
         {"dy", required_argument, NULL, 'Y'},
         {NULL, 0, 0, 0}};

    int opt;
    int option_index = 0;

    DecType = DecH264;

    while ((opt = getopt_long(argc, argv, short_opt,
                              longopts, &option_index)) != -1)
    {
        switch (opt)
        {
        case '?':
            return PrintUsage();

        case 't':
            DecType = (DecType_E)atoi(optarg);
            break;

        case 's':
            strcpy(SrcFile, optarg);
            break;

        case 'r':
            DEC_RATE = atoi(optarg);
            break;

        case 'W':
            DISP_W = atoi(optarg);
            break;

        case 'H':
            DISP_H = atoi(optarg);
            break;

        case 'X':
            DISP_X = atoi(optarg);
            break;

        case 'Y':
            DISP_Y = atoi(optarg);
            break;

        default:
            return PrintUsage();
        }
    }

    // 初始化解码器
    VDEC_Init();
    // 使能显示
    VO_Enable();

    // 最上层刷黑，使之透明
    FlushFrameBuffer(0x00000000);

    switch (DecType)
    {
    case DecH264:
        DecH264VideoStream();
        break;
    case DecMJPEG:
        DecJpgPicuter();
        break;
    default:
        break;
    }

    // FlushFrameBuffer(0x00000000);

    // 禁用显示
    VO_Disable();
    // 反初始化解码器
    VDEC_DeInit();

    return 0;
}
