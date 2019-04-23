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
    "-w [--sw]              Source Stream Width, default is 1280. \n"
    "-h [--sh]              Source Stream High, default is 720. \n"
    "-r [--sr]              Source Stream Clock Rate, default is 15. \n"
    "-W [--dw]              Display Width, default is 320. \n"
    "-H [--dh]              Display High, default is 240. \n"
    "-X [--dx]              Display X, default is 0. \n"
    "-Y [--dy]              Display Y, default is 0. \n"
    "\nDemo: ./VDecodeTest -t 1 -s H264.dat -w 1280 -h 720 -r 15 -W 320 -H 240 -X 0 -Y 0 \n"
    "\n********************************************************************************\n";

int PrintUsage()
{
    printf("%s\n", Usage);
    return 0;
}

// 解码源帧率
static int DEC_RATE = 15;
// 解码源宽度
static int DEC_W = 1280;
// 解码源高度
static int DEC_H = 720;

// 屏幕显示宽度
static int DISP_W = 320;
// 屏幕显示高度
static int DISP_H = 240;

// 屏幕显示位置X值
static int DISP_X = 0;
// 屏幕显示位置Y值
static int DISP_Y = 0;

// 显示通道
#define DISP_CHANNEL 1
// 显示图层
#define DISP_LAYER 0

#define DEC_BUF_SIZE 128 * 1024

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

void DecH264Display(void)
{
    // 4个图层，从优先级低使能，优先级0 < 1 < 2 < 3
    // 不允许不使能Channel 0，Layer 0，直接使用Channel 0，Layer 1

    VO_LAYER_INFO VoInfo;
    memset(&VoInfo, 0, sizeof(VO_LAYER_INFO));

    VoInfo.Rect.X = DISP_X;
    VoInfo.Rect.Y = DISP_Y;
    VoInfo.Rect.W = DISP_W;
    VoInfo.Rect.H = DISP_H;

    // VO_EnableChn(DISP_CHANNEL, &VoInfo);等价于VO_EnableVideoLayer(DISP_CHANNEL, 0, &VoInfo);
    if (!VO_EnableChn(DISP_CHANNEL, &VoInfo))
        printf("VO_EnableChn Failed!!!\n");
}

void *DecThread(void *Argv)
{
    VDEC_CHN_ATTR_S attr;
    memset(&attr, 0, sizeof(VDEC_CHN_ATTR_S));

    int DecChannel;
    // 创建解码通道，attr全部为0，即全部使用默认值
    DecChannel = VDEC_CreateChn(&attr);

    if (DecChannel >= 0)
    {
        printf("CreateChn Successful\n");
    }
    else
    {
        printf("Create First DecChannel[%d] Failed!\n", DecChannel);
        return NULL;
    }

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

    Buf = (unsigned char *)malloc(DEC_BUF_SIZE);

    VDEC_STREAM_S Stream;
    memset(&Stream, 0, sizeof(VDEC_STREAM_S));

    VDEC_FRAME_S Frame;
    memset(&Frame, 0, sizeof(VDEC_FRAME_S));

    for (i = 0; i < 500; i++)
    {
        if (Fp == NULL)
            break;

        if (fread(&Flen, 1, sizeof(Flen), Fp) != sizeof(Flen))
            break;

        if (Flen <= 0 || Flen > DEC_BUF_SIZE)
            break;

        if (fread(Buf, 1, Flen, Fp) != Flen)
            break;

        Stream.pu8Addr = Buf;
        Stream.u32Len = Flen;
        Stream.u64PTS = -1;

        // 向解码器送一帧数据
        if (!VDEC_SendStream(DecChannel, &Stream))
        {
            printf("VDEC_SendStream Error!\n");
            usleep(10000);
            continue;
        }

        VDEC_CHN_STAT_S Stat;
        memset(&Stat, 0, sizeof(VDEC_CHN_STAT_S));

        // 获取当前状态
        if (!VDEC_Query(DecChannel, &Stat))
        {
            printf("VDEC_Query Error!\n");
            continue;
        }

        if (Stat.u32FrameNum > 0)
        {
            int Ret;
            // 解码一帧数据
            Ret = VDEC_StartRecvStream(DecChannel, 0, 0, 0);

            switch (Ret)
            {
            case VDEC_NO_BITSTREAM:
                usleep(40000);
                break;

            case VDEC_KEYFRAME_DECODED:
            case VDEC_FRAME_DECODED:
                // 获取解码数据
                if (!VDEC_GetImage(DecChannel, &Frame))
                {
                    printf("VDEC_GetImage Error!\n");
                    continue;
                }

                // 测试解码区域缩放
                if (i == 50)
                {
                    VO_LAYER_RECT Rect;
                    memset(&Rect, 0, sizeof(VO_LAYER_RECT));

                    Rect.X = 100;
                    Rect.Y = 100;
                    Rect.W = DISP_W / 2;
                    Rect.H = DISP_H / 2;
                    
                    VO_Resize(DISP_CHANNEL, DISP_LAYER, &Rect);
                }
                
                if (!VO_SetZoomInWindow(DISP_CHANNEL, DISP_LAYER, &Frame.SrcInfo))
                {
                    printf("VO_SetZoomInWindow Error!\n");
                    continue;
                }

                if (!VO_ChnShow(DISP_CHANNEL, DISP_LAYER, &Frame))
                {
                    printf("VO_ChnShow Error!\n");
                    continue;
                }

                // 释放解码数据
                if (!VDEC_ReleaseImage(DecChannel, &Frame))
                {
                    printf("VDEC_ReleaseImage Error!\n");
                    continue;
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

    fclose(Fp);
    Fp = NULL;

    VDEC_DestroyChn(DecChannel);

    return NULL;
}

void DecH264VideoStream(void)
{
    pthread_t DecPid;

    DecH264Display();

    pthread_create(&DecPid, NULL, DecThread, NULL);

    pthread_join(DecPid, NULL);

    VO_DisableVideoLayer(DISP_CHANNEL, DISP_LAYER);
}

void DecJpgPicuter(void)
{
    VO_LAYER_INFO VoInfo;
    memset(&VoInfo, 0, sizeof(VO_LAYER_INFO));

    VoInfo.Rect.X = DISP_X;
    VoInfo.Rect.Y = DISP_Y;
    // 可以在解码部分缩放，也可以在显示部分缩放
    VoInfo.Rect.W = DEC_W;
    VoInfo.Rect.H = DEC_H;

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
    attr.u32PicWidth = DISP_W;
    attr.u32PicHight = DISP_H;

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
    if (!VDEC_Query(DecChannel, &Stat))
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

            // 用户在VDEC_GetImage成功后，可通过
            // Frame.pVirAddr获取YUV数据地址
            // 若解码格式YV12，Frame.pVirAddr[1]是V地址，Frame.pVirAddr[2]是U地址
            // 若解码格式NV21，Frame.pVirAddr[1]是VU混合地址

            if (!VO_SetZoomInWindow(0, 0, &Frame.SrcInfo))
            {
                printf("VO_SetZoomInWindow Error!\n");
            }

            if (!VO_ChnShow(0, 0, &Frame))
            {
                printf("VO_ChnShow Error!\n");
            }

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
    char *short_opt = "?:t:s:w:h:r:W:H:X:Y:";

    static struct option longopts[] =
        {{"help", no_argument, NULL, '?'},
         {"type", required_argument, NULL, 't'},
         {"sf", required_argument, NULL, 's'},
         {"sw", required_argument, NULL, 'w'},
         {"sh", required_argument, NULL, 'h'},
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

        case 'w':
            DEC_W = atoi(optarg);
            break;

        case 'h':
            DEC_H = atoi(optarg);
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
    FlushFrameBuffer(0x80000000);

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

    // FlushFrameBuffer(0x1FAAAAAA);

    // 禁用显示
    VO_Disable();
    // 反初始化解码器
    VDEC_DeInit();

    return 0;
}
