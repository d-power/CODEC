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
    "-h [-?|--help]         Print VDecodeTest Usage Information And Exit \n"
    "-t [--type]            Decode Type : 1.H264 2.MJPEG \n"
    "-s [--srcfile]         Source File To Decode. \n"
    "-c [--channel]         Specify Display Channel Number. \n"
    "-b [--black]           Specify 1 If Need Flush FrameBuffer Transparent. \n"
    "-W [--width]           Specify Width To Decode \n"
    "-H [--high]            Specify High To Decode \n"
    "-X [--xaxis]           Specify Xaxis To Show \n"
    "-Y [--yaxis]           Specify Yaxis To Show \n"
    "\nDemo: ./VDecodeTest -t 1 -s H264.dat -c 1 -X 100 -Y 100 -W 320 -H 240\n"
    "\n********************************************************************************\n";

int PrintUsage()
{
    printf("%s\n", Usage);
    return 0;
}

#define DEC_RATE 25

#define DEC_WIDTH 640
#define DEC_HIGH 480

// 屏幕显示宽度
// #define DISP_WIDTH 400
static int DISP_WIDTH = 400;
// 屏幕显示高度
// #define DISP_HIGH 240
static int DISP_HIGH = 240;

// 屏幕显示位置X
static int DISP_X = 0;
// 屏幕显示位置Y
static int DISP_Y = 0;

// 显示通道
// #define DISP_CHANNEL 1
static int DISP_CHANNEL = 0;
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

void Ui_Display_Black()
{
    int fbfd = 0;
    char *fbp = 0;

    static int xres = 0;
    static int yres = 0;
    static int bits_per_pixel = 0;

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    long int screensize = 0;

    struct fb_bitfield red;
    struct fb_bitfield green;
    struct fb_bitfield blue;

    unsigned int *ptr = NULL;

    fbfd = open("/dev/fb0", O_RDWR);

    if (!fbfd)
    {
        printf("Error: cannot open framebuffer device.\n");
        exit(1);
    }

    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo))
    {
        printf("Error：reading fixed information.\n");
        exit(2);
    }

    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo))
    {
        printf("Error: reading variable information.\n");
        exit(3);
    }

    printf("R:%d,G:%d,B:%d \n", vinfo.red, vinfo.green, vinfo.blue);
    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    xres = vinfo.xres;
    yres = vinfo.yres;
    bits_per_pixel = vinfo.bits_per_pixel;
    //计算屏幕的总大小（字节）
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    printf("screensize=%d byte\n", screensize);

    //对象映射
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

    if ((int)fbp == -1)
    {
        printf("Error: failed to map framebuffer device to memory.\n");
        exit(4);
    }

    ptr = (unsigned int *)fbp;
    int i, j;

    for (i = 0; i < vinfo.xres; i++)
        for (j = 0; j < vinfo.yres; j++)
            *ptr++ = 0x80000000;

    // memset(fbp,0,screensize);
    //删除对象映射
    munmap(fbp, screensize);
    close(fbfd);
}

void *DecThread(void *Argv)
{
    VDEC_CHN_ATTR_S attr;
    memset(&attr, 0, sizeof(VDEC_CHN_ATTR_S));

    attr.deType = PAYLOAD_TYPE_H264;
    attr.u32FrameRate = DEC_RATE;
    attr.u32PicHight = DEC_WIDTH;
    attr.u32PicWidth = DEC_HIGH;
    // 使用默认值
    attr.BufSize = 0;
    attr.FormatType = FORMAT_YV12;

    int DecChannel;
    // 创建解码通道
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

    unsigned int FlenSize;
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
        if (!VDEC_Query(0, &Stat))
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
}

void DecH264VideoStream(void)
{
    pthread_t DecPid;

    // 4个图层，从优先级低使能，优先级0 < 1 < 2 < 3
    // 不允许不使能Channel 0，Layer 0，直接使用Channel 0，Layer 1

    VO_LAYER_INFO VoInfo;
    memset(&VoInfo, 0, sizeof(VO_LAYER_INFO));

    VoInfo.Rect.X = DISP_X;
    VoInfo.Rect.Y = DISP_Y;
    VoInfo.Rect.W = DISP_WIDTH;
    VoInfo.Rect.H = DISP_HIGH;

    // VO_EnableChn(DISP_CHANNEL, &VoInfo);等价于VO_EnableVideoLayer(DISP_CHANNEL, 0, &VoInfo);
    VO_EnableChn(DISP_CHANNEL, &VoInfo);

    pthread_create(&DecPid, NULL, DecThread, NULL);

    pthread_join(DecPid, NULL);

    VO_DisableVideoLayer(DISP_CHANNEL, DISP_LAYER);
}

void DecJpgPicuter(void)
{
    VO_LAYER_INFO VoInfo;
    memset(&VoInfo, 0, sizeof(VO_LAYER_INFO));

    VoInfo.Rect.X = 0;
    VoInfo.Rect.Y = 0;
    // 可以在解码部分缩放，也可以在显示部分缩放
    VoInfo.Rect.W = DEC_WIDTH;
    VoInfo.Rect.H = DEC_HIGH;

    unsigned int Channel = DISP_CHANNEL;

    // 使用显示通道0，图层0
    VO_EnableChn(Channel, &VoInfo);

    VDEC_CHN_ATTR_S attr;
    memset(&attr, 0, sizeof(VDEC_CHN_ATTR_S));

    attr.deType = PAYLOAD_TYPE_MJPEG;
    // 使用默认值
    attr.BufSize = 0;
    attr.FormatType = FORMAT_YV12;
    // 可以不按比例缩放解码
    attr.u32PicWidth = DISP_WIDTH;
    attr.u32PicHight = DISP_WIDTH;

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
    char *short_opt = "h?t:s:c:b:W:H:X:Y:";

    static struct option longopts[] =
        {{"help", no_argument, NULL, 'h'},
         {"type", required_argument, NULL, 't'},
         {"srcfile", required_argument, NULL, 's'},
         {"channel", required_argument, NULL, 'c'},
         {"black", required_argument, NULL, 'b'},
         {"width", required_argument, NULL, 'W'},
         {"high", required_argument, NULL, 'H'},
         {"xaxis", required_argument, NULL, 'X'},
         {"yaxis", required_argument, NULL, 'Y'},
         {NULL, 0, 0, 0}};

    int opt;
    int option_index = 0;

    int BlackFlag = 0;

    while ((opt = getopt_long(argc, argv, short_opt,
                              longopts, &option_index)) != -1)
    {
        switch (opt)
        {
        case '?':
        case 'h':
            return PrintUsage();

        case 't':
            DecType = (DecType_E)atoi(optarg);
            break;

        case 's':
            strcpy(SrcFile, optarg);
            break;

        case 'c':
            DISP_CHANNEL = atoi(optarg);
            break;

        case 'b':
            BlackFlag = atoi(optarg);
            break;
        
        case 'W':
            DISP_WIDTH = atoi(optarg);
            break;

        case 'H':
            DISP_HIGH = atoi(optarg);
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

    if (BlackFlag)
    {
        // 当UI画在最上层通道时，刷黑，使之透明
        Ui_Display_Black();
    }

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

    // 禁用显示
    VO_Disable();
    // 反初始化解码器
    VDEC_DeInit();

    return 0;
}
