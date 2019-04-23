/******************************************************************************
Copyright(c) 2016-2018 Digital Power Inc.
File name: H2642JPEGTest.c
Author: LiuZhengzhong
Version: 1.0.0
Date: 2019/4/22
Description: Platform of DP X5/X1H h264 decode to JPEG encode
History:
Bug report: liuzhengzhong@d-power.com.cn
******************************************************************************/

#include <getopt.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <video/vd/VDecApi.h>
#include <video/ve/VEncApi.h>
#include <video/ve/ViApi.h>

#define DEC_BUF_SIZE 128 * 1024

// 解码源宽度
static int DEC_W = 640;
// 解码源高度
static int DEC_H = 480;

// 屏幕显示宽度
static int ENC_W = 320;
// 屏幕显示高度
static int ENC_H = 240;

// 根据传入参数赋值的解码源文件
static char SrcFile[128] = {0};

static int EncChannel = 0;

static const char *Usage =
    "********************************************************************************\n"
    "OVERVIEW: h264 decode to JPEG encode Test Application! \n"
    "Usage: ./H2642JPEGTest [options] <arg> \n"
    "\nOPTIONS:\n"
    "-? [--help]            Print H2642JPEGTest Usage Information And Exit \n"
    "-s [--df]              Source File To Decode. \n"
    "-w [--dw]              Source Decode Stream Width, default is 1280. \n"
    "-h [--dh]              Source Decode Stream High, default is 720. \n"
    "-W [--ew]              Encode Width, default is 320. \n"
    "-H [--eh]              Encode High, default is 240. \n"
    "\nDemo: ./H2642JPEGTest   \n"
    "\n********************************************************************************\n";

int PrintUsage()
{
    printf("%s\n", Usage);
    return 0;
}

void EncodeJPG(int Count, ENC_FRAME_S *Frame)
{
    char DstFile[64] = {0};
    sprintf(DstFile, "%d.jpg", Count);

    FILE *Fd = NULL;
    Fd = fopen(DstFile, "wb");

    if (Fd == NULL)
    {
        printf("Can Not Open File[%s]\n", DstFile);
        return;
    }

    // 送给编码器
    if(VENC_SendFrame(EncChannel, Frame) < 0)
    {
        printf("VENC_SendFrame() Failed!\n");
        return;
    }

    VENC_STREAM_S Stream;
    memset(&Stream, 0, sizeof(VENC_STREAM_S));

    // 获取编码后的数据
    if(!VENC_GetStream(EncChannel, &Stream))
    {
        printf("VENC_GetStream() Failed!\n");
        return;
    }

    // 写入编码后的数据
    fwrite(Stream.pData0, 1, Stream.u32Size0, Fd);

    // 如果Stream.u32Size1为真，则拼接pData1，才可以形成完整的一帧
    if(Stream.u32Size1)
    {
        fwrite(Stream.pData1, 1, Stream.u32Size1, Fd);
    }

    fclose(Fd);
    
    // 释放编码过的数据
    if(!VENC_ReleaseStream(EncChannel, &Stream))
    {
        printf("VENC_ReleaseStream() Failed!\n");
        return;
    }

    return;
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

    ENC_FRAME_S EncFrame;
    memset(&EncFrame, 0, sizeof(ENC_FRAME_S));

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

                EncFrame.PhyBufferY = (unsigned char*)Frame.u32PhyAddr[0];
                EncFrame.PhyBufferC = (unsigned char*)Frame.u32PhyAddr[2];

                EncodeJPG(i, &EncFrame);

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

int main(int argc, char **argv)
{
    // char *short_opt = "?:t:s:w:h:r:W:H:X:Y:";

    // static struct option longopts[] =
    //     {{"help", no_argument, NULL, '?'},
    //      {"type", required_argument, NULL, 't'},
    //      {"sf", required_argument, NULL, 's'},
    //      {"sw", required_argument, NULL, 'w'},
    //      {"sh", required_argument, NULL, 'h'},
    //      {"dw", required_argument, NULL, 'W'},
    //      {"dh", required_argument, NULL, 'H'},
    //      {NULL, 0, 0, 0}};

    // int opt;
    // int option_index = 0;

    // while ((opt = getopt_long(argc, argv, short_opt,
    //                           longopts, &option_index)) != -1)
    // {
    //     switch (opt)
    //     {
    //     case '?':
    //         return PrintUsage();

    //     case 's':
    //         strcpy(SrcFile, optarg);
    //         break;

    //     case 'w':
    //         DEC_W = atoi(optarg);
    //         break;

    //     case 'h':
    //         DEC_H = atoi(optarg);
    //         break;

    //     case 'W':
    //         ENC_W = atoi(optarg);
    //         break;

    //     case 'H':
    //         ENC_H = atoi(optarg);
    //         break;

    //     default:
    //         return PrintUsage();
    //     }
    // }

    strcpy(SrcFile, "/mnt/CODEC/third_party/TestFile/Test_640x480.h264");

    // 初始化解码器
    VDEC_Init();

    // 初始化编码器
    if(!VENC_Init())
    {
        printf("VENC_Init() Failed!\n");
        return 0;
    }

    VENC_ATTR_S EncAttr;
    memset(&EncAttr, 0, sizeof(VENC_ATTR_S));

    EncAttr.fmtType = VENC_PIXEL_FMT_YVU420P;
    EncAttr.enType = VENC_PAYLOAD_TYPE_JPEG;
    EncAttr.u32SrcWidth = DEC_W;
    EncAttr.u32SrcHeight = DEC_H;
    EncAttr.u32DstWidth = ENC_W;
    EncAttr.u32DstHeight = ENC_H;
    EncAttr.u32BitRate = 128 * 1024 *1024;

    // 创建编码器
    EncChannel = VENC_CreateChn(&EncAttr);

    if(EncChannel < 0)
    {
        printf("VENC_CreateChn() Failed!\n");
        return 0;
    }

    pthread_t Pid;

    pthread_create(&Pid, NULL, DecThread, NULL);

    pthread_join(Pid, NULL);

    // 反初始化解码器
    VDEC_DeInit();

    return 0;
}

