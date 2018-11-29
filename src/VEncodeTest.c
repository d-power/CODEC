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
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdarg.h>
#include <getopt.h>

#include <video/ve/VEncApi.h>
#include <video/ve/ViApi.h>

static int SRC_WIDTH = 1280;
static int SRC_HEIGHT = 720;

static int DST_WIDTH = 640;
static int DST_HEIGHT = 360;

// 根据传入参数赋值的编码输出文件
static char DstFile[64] = {0};

static int VDecType = 0;

static int MaxCount = 10;

static const char *Usage =
    "********************************************************************************\n"
    "OVERVIEW: Video Encode Function Test Application! \n"
    "Usage: ./VEncodeTest [options] <arg> \n"
    "\nOPTIONS:\n"
    "-? [--help]            Print VideoEncode Usage Information And Exit \n"
    "-t [--t]               Encode Type, 0 is h264 and 1 is jpg, default is 0. \n"
    "-d [--df]              Destination File To Encode. \n"
    "-w [--sw]              Camera Source Width, default is 1280. \n"
    "-h [--sh]              Camera Source High, default is 720. \n"
    "-W [--dw]              Encode Width, default is 640. \n"
    "-H [--dh]              Encode High, default is 360. \n"
    "\nDemo: ./VEncodeTest -d H264.dat -w 1280 -h 720 -W 640 -H 360 \n"
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

    FILE *Fd = NULL;
    Fd = fopen(DstFile, "wb");

    if (Fd == NULL)
    {
        printf("Can Not Open File[%s]\n", DstFile);
        return NULL;
    }

    // 向文件写入头信息
    fwrite(Head.HeadBuffer, 1, Head.HeadLengh, Fd);

    int Count;

    // 编码200帧
    for(Count = 0; Count < 200; Count++)
    {
        ENC_FRAME_S Frame;

        // 从video in获取一帧
        if(!VI_GetFrame(&Frame))
        {
            printf("VI_GetFrame() Failed!\n");
            continue;
        }

        // 送给编码器
        if(VENC_SendFrame(Channel, &Frame) < 0)
        {
            printf("VENC_SendFrame() Failed!\n");
            continue;
        }

        VENC_STREAM_S Stream;
        memset(&Stream, 0, sizeof(VENC_STREAM_S));

        // 获取编码后的数据
        if(!VENC_GetStream(Channel, &Stream))
        {
            printf("VENC_GetStream() Failed!\n");
            continue;
        }

        // 每隔5帧强制一个I帧数据
        // if (Count % 5 == 0)
        // {
        //     VENC_RequestIDR(Channel);
        // }

        if (Stream.s32Flag)
            printf("Get I Frame!\n");

        // 写入编码后的数据
        fwrite(Stream.pData0, 1, Stream.u32Size0, Fd);

        // 如果Stream.u32Size1为真，则拼接pData1，才可以形成完整的一帧
        if(Stream.u32Size1)
        {
            fwrite(Stream.pData1, 1, Stream.u32Size1, Fd);
        }

        // 释放编码过的数据
        if(!VENC_ReleaseStream(Channel, &Stream))
        {
            printf("VENC_ReleaseStream() Failed!\n");
        }

        // 释放video in内的一帧
        if(!VI_ReleaseFrame(&Frame))
        {
            printf("VI_ReleaseFrame() Failed!\n");
        }
    }

    fclose(Fd);
    Fd = NULL;

    return NULL;
}

void *EncodeJPGThread(void *Param)
{
    FILE *Fd = NULL;
    Fd = fopen(DstFile, "wb");

    if (Fd == NULL)
    {
        printf("Can Not Open File[%s]\n", DstFile);
        return NULL;
    }

    int Count;

    // 多取几帧再存图片，防止摄像头刚启动时可能出现的问题
    for(Count = 0; Count <= MaxCount; Count++)
    {
        ENC_FRAME_S Frame;

        // 从video in获取一帧
        if(!VI_GetFrame(&Frame))
        {
            printf("VI_GetFrame() Failed!\n");
            return NULL;
        }

        // 送给编码器
        if(VENC_SendFrame(Channel, &Frame) < 0)
        {
            printf("VENC_SendFrame() Failed!\n");
            return NULL;
        }

        VENC_STREAM_S Stream;
        memset(&Stream, 0, sizeof(VENC_STREAM_S));

        // 获取编码后的数据
        if(!VENC_GetStream(Channel, &Stream))
        {
            printf("VENC_GetStream() Failed!\n");
            return NULL;
        }

        if (Count == MaxCount)
        {
            // 写入编码后的数据
            fwrite(Stream.pData0, 1, Stream.u32Size0, Fd);

            // 如果Stream.u32Size1为真，则拼接pData1，才可以形成完整的一帧
            if(Stream.u32Size1)
            {
                fwrite(Stream.pData1, 1, Stream.u32Size1, Fd);
            }

            fclose(Fd);
        }

        // 释放编码过的数据
        if(!VENC_ReleaseStream(Channel, &Stream))
        {
            printf("VENC_ReleaseStream() Failed!\n");
            return NULL;
        }

        // 释放video in内的一帧
        if(!VI_ReleaseFrame(&Frame))
        {
            printf("VI_ReleaseFrame() Failed!\n");
            return NULL;
        }
    }

    return NULL;
}

int main(int argc, char **argv)
{
    char *short_opt = "?:t:d:w:h:W:H:";

    static struct option longopts[] =
    {
        {"help", no_argument, NULL, '?'},
        {"t", required_argument, NULL, 't'},
        {"df", required_argument, NULL, 'd'},
        {"sw", required_argument, NULL, 'w'},
        {"sh", required_argument, NULL, 'h'},
        {"dw", required_argument, NULL, 'W'},
        {"dh", required_argument, NULL, 'H'},
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
                return PrintUsage();

            case 't':
                VDecType = atoi(optarg);
                break;

            case 'd':
                strcpy(DstFile, optarg);
                break;

            case 'w':
                SRC_WIDTH = atoi(optarg);
                break;

            case 'h':
                SRC_HEIGHT = atoi(optarg);
                break;

            case 'W':
                DST_WIDTH = atoi(optarg);
                break;

            case 'H':
                DST_HEIGHT = atoi(optarg);
                break;

            default:
                break;
        }
    }

    // 使能设备video in设备，即/dev/video0
    if(!VI_EnableDev())
    {
        printf("EnableDev() Failed!\n");
        return 0;
    }

    // 初始化编码器
    if(!VENC_Init())
    {
        printf("VENC_Init() Failed!\n");
        return 0;
    }

    VENC_ATTR_S EncAttr;
    memset(&EncAttr, 0, sizeof(VENC_ATTR_S));

    EncAttr.enType = (VDecType == 0) ? VENC_PAYLOAD_TYPE_H264 : VENC_PAYLOAD_TYPE_JPEG;
    EncAttr.u32SrcWidth = SRC_WIDTH;
    EncAttr.u32SrcHeight = SRC_HEIGHT;
    EncAttr.u32DstWidth = DST_WIDTH;
    EncAttr.u32DstHeight = DST_HEIGHT;
    EncAttr.u32BitRate = 256 * 1024 *1024;
//    EncAttr.u32EnableQP = 1;
//    EncAttr.u32IQP = 30;
//    EncAttr.u32PQP = 45;

    // 创建编码器
    Channel = VENC_CreateChn(&EncAttr);

    if(Channel < 0)
    {
        printf("VENC_CreateChn() Failed!\n");
        return 0;
    }

    VI_DEV_ATTR_S Attr;
    memset(&Attr, 0, sizeof(VI_DEV_ATTR_S));

    Attr.stSize.Width = SRC_WIDTH;
    Attr.stSize.Height = SRC_HEIGHT;

    // 设置video in属性
    if(!VI_SetDevAttr(&Attr))
    {
        printf("VI_SetDevAttr() Failed!\n");
        return 0;
    }

    pthread_t Pid = 0;

    if (VDecType == 0)
        // 创建线程开始h264编码
        pthread_create(&Pid, NULL, EncodeThread, NULL);
    else
        // 创建线程开始jpg编码
        pthread_create(&Pid, NULL, EncodeJPGThread, NULL);

    if (Pid)
        // 等待线程结束再执行一些回收动作
        pthread_join(Pid, NULL);

    if(!VENC_DestroyChn(Channel))
    {
        printf("VENC_DestroyChn() Failed!\n");
    }

    if(!VENC_DeInit())
    {
        printf("VENC_DeInit() Failed!\n");
    }

    if(!VI_DisableDev())
    {
        printf("VI_DisableDev() Failed!\n");
    }

    return 0;
}
