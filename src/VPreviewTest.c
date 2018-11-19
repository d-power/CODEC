/******************************************************************************
Copyright(c) 2016-2018 Digital Power Inc.
File name: VPreview.c
Author: LiuZhengzhong
Version: 1.0.0
Date: 2018/08/23
Description: Video Previwe Test
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

#include <video/ve/ViApi.h>
#include <video/vd/VoApi.h>

static unsigned int InWidth = 1280;
static unsigned int InHigh = 720;
static unsigned int InBufferCount = 3;
static unsigned int DisplayWidth = 320;
static unsigned int DisplayHigh = 240;
static unsigned int DisplayX = 0;
static unsigned int DisplayY = 0;

static const char *Usage =
    "********************************************************************************\n"
    "OVERVIEW: Video Preview Function Test Application! \n"
    "Usage: ./VPreviewTest [options] <arg> \n"
    "\nOPTIONS:\n"
    "-? [--help]            Print VPreviewTest Usage Information And Exit \n"
    "-W [--iw]              Camera Input Width, default is 1280 \n"
    "-H [--ih]              Camera Input High, default is 720 \n"
    "-B [--ibufcnt]         Camera Input Buffer Count, default is 3 \n"
    "-w [--dw]              Display Width, default is 320 \n"
    "-h [--dh]              Display High, default is 240 \n"
    "-x [--dx]              Display X, default is 0 \n"
    "-y [--dy]              Display Y, default is 0 \n"
    "\nDemo: ./VPreviewTest -W 1280 -H 720 -w 320 -h 240 -x 0 -y 0 \n"
    "\n********************************************************************************\n";

int PrintUsage()
{
    printf("%s\n", Usage);
    return 0;
}

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

    // 屏幕宽度
    unsigned int FbWidth = vinfo.xres;
    // 屏幕高度
    unsigned int FbHeight = vinfo.yres;
    // 屏幕一行字节数
    unsigned int FbPitch = finfo.line_length;

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

void Preview()
{
    int DisplayChannel = 1;
    int DisplayLayer = 0;

    VO_LAYER_INFO VoInfo;
    memset(&VoInfo, 0, sizeof(VO_LAYER_INFO));

    VoInfo.Rect.X = DisplayX;
    VoInfo.Rect.Y = DisplayY;
    VoInfo.Rect.W = DisplayWidth;
    VoInfo.Rect.H = DisplayHigh;

    VO_EnableChn(DisplayChannel, &VoInfo);
    // VO_EnableVideoLayer(DisplayChannel, DisplayLayer, &VoInfo);

    // 使能设备video in设备，即/dev/video0
    if (!VI_EnableDev())
    {
        printf("VI_EnableDev() Failed!\n");
        return;
    }

    VI_DEV_ATTR_S Attr;
    memset(&Attr, 0, sizeof(VI_DEV_ATTR_S));

    Attr.enDataFmt = VI_DATA_FMT_YUV420;
    Attr.stSize.Width = InWidth;
    Attr.stSize.Height = InHigh;
    // Attr.u32Angle = Angle_VFLIP;
    Attr.u32TimePerFrame = 30;
    Attr.BuffersCnt = InBufferCount;

    // 设置video in属性
    if (!VI_SetDevAttr(&Attr))
    {
        printf("X5_VI_SetDevAttr() Failed!\n");
        return;
    }

    int Count;

    ENC_FRAME_S VI_Frame;
    VDEC_FRAME_S VO_Frame;

    memset(&VI_Frame, 0, sizeof(ENC_FRAME_S));
    memset(&VO_Frame, 0, sizeof(VDEC_FRAME_S));

    VO_Frame.SrcInfo.W = InWidth;
    VO_Frame.SrcInfo.H = InHigh;
    VO_Frame.SrcInfo.Crop_X = DisplayX;
    VO_Frame.SrcInfo.Crop_Y = DisplayY;
    VO_Frame.SrcInfo.Crop_W = InWidth;
    VO_Frame.SrcInfo.Crop_H = InHigh;
    VO_Frame.SrcInfo.Format = HWC_FORMAT_YUV420PLANAR;

    if (!VO_SetZoomInWindow(DisplayChannel, DisplayLayer, &VO_Frame.SrcInfo))
    {
        printf("VO_SetZoomInWindow Error!\n");
    }

    // 预览1000帧
    for (Count = 0; Count < 1000; Count++)
    {
        // 从video in获取一帧
        if (!VI_GetFrame(&VI_Frame))
        {
            printf("X5_VI_GetFrame() Failed!\n");
            return;
        }

        VO_Frame.u32PhyAddr[0] = (void *)VI_Frame.PhyBufferY;
        VO_Frame.u32PhyAddr[1] = (void *)VI_Frame.PhyBufferC;
        VO_Frame.u32PhyAddr[2] = (void *)(VI_Frame.PhyBufferC + InWidth * InHigh / 4);

        if (!VO_ChnShow(DisplayChannel, DisplayLayer, &VO_Frame))
        {
            printf("VO_ChnShow Error!\n");
            continue;
        }

        if (!VI_ReleaseFrame(&VI_Frame))
        {
            printf("X5_VI_ReleaseFrame() Failed!\n");
            return;
        }
    }

    VO_DisableVideoLayer(DisplayChannel, DisplayLayer);

    if (!VI_DisableDev())
    {
        printf("X5_DisableDev() Failed!\n");
    }

    return;
}

int main(int argc, char **argv)
{
    printf("Start Preview Test!\n");

    char *short_opt = "?:W:H:B:w:h:x:y:";

    static struct option longopts[] =
        {{"help", no_argument, NULL, '?'},
         {"iw", required_argument, NULL, 'W'},
         {"ih", required_argument, NULL, 'H'},
         {"ibufcnt", required_argument, NULL, 'B'},
         {"dw", required_argument, NULL, 'w'},
         {"dh", required_argument, NULL, 'h'},
         {"dx", required_argument, NULL, 'x'},
         {"dy", required_argument, NULL, 'y'},
         {NULL, 0, 0, 0}};

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, short_opt,
                              longopts, &option_index)) != -1)
    {
        switch (opt)
        {
        case '?':
            return PrintUsage();
        case 'W':
            InWidth = atoi(optarg);
            break;
        case 'H':
            InHigh = atoi(optarg);
            break;
        case 'B':
            InBufferCount = atoi(optarg);
            break;
        case 'w':
            DisplayWidth = atoi(optarg);
            break;
        case 'h':
            DisplayHigh = atoi(optarg);
            break;
        case 'x':
            DisplayX = atoi(optarg);
            break;
        case 'y':
            DisplayY = atoi(optarg);
            break;

        default:
            return PrintUsage();
        }
    }

    // 使能显示
    if (!VO_Enable())
    {
        printf("VO_Enable() Failed!\n");
        return 0;
    }

    FlushFrameBuffer(0x80000000);

    Preview();

    // 禁用显示
    VO_Disable();

    return 1;
}
