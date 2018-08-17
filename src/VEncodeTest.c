/******************************************************************************
Copyright(c) 2016-2018 Digital Power Inc.
File name: VEncodeTest.c
Author: LiuZhengzhong
Version: 1.0.0
Date: 2018/5/8
Description: Platform of DP X5 video encode test
History:
Bug report: liuzhengzhong@d-power.com.cn
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdarg.h>
#include <getopt.h>

#include <video/ve/VEncApi.h>
#include <video/ve/ViApi.h>

#define SRC_WIDTH 640
#define SRC_HEIGHT 480

// 根据传入参数赋值的编码输出文件
static char DstFile[64] = {0};

static const char *Usage =
    "********************************************************************************\n"
    "OVERVIEW: Video Encode Function Test Application! \n"
    "Usage: ./VEncodeTest [options] <arg> \n"
    "\nOPTIONS:\n"
    "-h [-?|--help]         Print VideoEncode Usage Information And Exit \n"
    "-d [--dstfile]         Destination File To Encode. \n"
    "\nDemo: ./VEncodeTest -d H264.dat \n"
    "\n********************************************************************************\n";

int PrintUsage()
{
    printf("%s\n", Usage);
    return 0;
}

const char *EncStatus[] =
{
    "VENC_ERROR",
    "VENC_OK",
    "VENC_NO_FRAME_BUFFER",
    "VENC_BITSTREAM_IS_FULL"
};

static int Channel = -1;

void *EncodeThread(void *Param)
{
    VENC_STREAM_HEAD Head;

    // 获取H264头信息
    if(VDEC_GetHeader(Channel, &Head))
    {
        printf("X5_VDEC_GetHeader() Successful\n");
    }

    FILE *Fd;
    Fd = fopen(DstFile, "wb");
    // 向文件写入头信息
    fwrite(Head.HeadBuffer, 1, Head.HeadLengh, Fd);

    int Count;

    // 编码200帧
    for(Count = 0; Count < 200; Count++)
    {
        ENC_FRAME_S Frame;

        // 从video in获取一帧
        if(VI_GetFrame(&Frame))
        {
            printf("X5_VI_GetFrame() Successful\n");
        }

        Frame.stSize.Width = SRC_WIDTH;
        Frame.stSize.Height = SRC_HEIGHT;

        // 送给编码器
        if(VENC_SendFrame(Channel, &Frame))
        {
            printf("X5_VENC_SendFrame() Successful\n");
        }

        // 编码一帧
        int Status = VENC_StartRecvPic(Channel);
        printf("X5_VENC_StartRecvPic[%s]\n", EncStatus[Status + 1]);

        VENC_STREAM_S Stream;
        memset(&Stream, 0, sizeof(VENC_STREAM_S));

        // 获取编码后的数据
        if(VENC_GetStream(Channel, &Stream))
        {
            printf("X5_VENC_GetStream() Successful\n");
        }

        // 写入编码后的数据
        fwrite(Stream.pData0, 1, Stream.u32Size0, Fd);

        // 如果Stream.u32Size1为真，则拼接pData1，才可以形成完整的一帧
        if(Stream.u32Size1)
        {
            fwrite(Stream.pData1, 1, Stream.u32Size1, Fd);
        }

        // 释放编码过的数据
        if(VENC_ReleaseStream(Channel, &Stream))
        {
            printf("X5_VENC_ReleaseStream() Successful\n");
        }

        // 释放video in内的一帧
        if(VI_ReleaseFrame(&Frame))
        {
            printf("X5_VI_ReleaseFrame() Successful\n");
        }
    }

    fclose(Fd);
    Fd = NULL;
}

int main(int argc, char **argv)
{
    char *short_opt = "h?d:";

    static struct option longopts[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"dstfile", required_argument, NULL, 'd'},
        {NULL, 0, 0, 0}
    };

    int opt;
    int option_index = 0;

    while((opt = getopt_long(argc, argv, short_opt,
                             longopts, &option_index)) != -1)
    {
        switch(opt)
        {
            case '?':
            case 'h':
                return PrintUsage();

            case 'd':
                strcpy(DstFile, optarg);
                break;

            default:
                break;
        }
    }

    // 使能设备video in设备，即/dev/video0
    if(VI_EnableDev())
    {
        printf("X5_EnableDev() Successful\n");
    }

    // 初始化编码器
    if(VENC_Init())
    {
        printf("X5_VENC_Init() Successful\n");
    }

    VENC_ATTR_S EncAttr;
    memset(&EncAttr, 0, sizeof(VENC_ATTR_S));

    EncAttr.enType = PAYLOAD_TYPE_H264;
    EncAttr.u32BitRate = 1 * 1024 * 1024;
    EncAttr.u32FrameRate = 30;
    EncAttr.u32SrcWidth = SRC_WIDTH;
    EncAttr.u32SrcHeight = SRC_HEIGHT;
    EncAttr.u32DstWidth = SRC_WIDTH;
    EncAttr.u32DstHeight = SRC_HEIGHT;

    // 创建编码器
    Channel = VENC_CreateChn(&EncAttr);

    if(Channel >= 0)
    {
        printf("X5_VENC_CreateChn() Successful\n");
    }

    VI_DEV_ATTR_S Attr;
    memset(&Attr, 0, sizeof(VI_DEV_ATTR_S));

    Attr.enDataFmt = VI_DATA_FMT_YUV420;
    Attr.stSize.Width = SRC_WIDTH;
    Attr.stSize.Height = SRC_HEIGHT;
    Attr.u32Angle = 0;
    Attr.u32TimePerFrame = 30;

    // 设置video in属性
    if(VI_SetDevAttr(&Attr))
    {
        printf("X5_VI_SetDevAttr() Successful\n");
    }

    pthread_t Pid;
    // 创建线程开始编码
    pthread_create(&Pid, NULL, EncodeThread, NULL);

    // 等待线程结束再执行一些回收动作
    pthread_join(Pid, NULL);

    if(VENC_DestroyChn(Channel))
    {
        printf("X5_VENC_DestroyChn() Successful\n");
    }

    if(VENC_DeInit())
    {
        printf("X5_VENC_DeInit() Successful\n");
    }

    if(VI_DisableDev())
    {
        printf("X5_DisableDev() Successful\n");
    }

    return 0;
}
