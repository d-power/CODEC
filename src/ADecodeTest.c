/******************************************************************************
Copyright(c) 2016-2018 Digital Power Inc.
File name: ADecodeTest.c
Author: LiuZhengzhong
Version: 1.0.4
Date: 2018/3/14
Description: Platform of DP X5 audio decode test
History:
Bug report: liuzhengzhong@d-power.com.cn
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <getopt.h>
#include <pthread.h>

#include <audio/codec/ADecApi.h>

// 给解码器分配2048byte缓冲区
#define DEC_BUFFER_BYTES 2048
// G.711每次解码多少个byte，2的倍数即可
#define DEC_ONCE_BYTE 160

// 解码类型枚举
typedef enum _DecType_E
{
    DecG711A = 1,
    DecG711U,
    DecMP3,
    DecAAC,
    DecBUT,

} DecType_E;

// 根据传入参数赋值的解码类型
DecType_E DecType;
// 根据传入参数赋值的解码源文件
static char SrcFile[64] = {0};
// 根据传入参数赋值的解码后写入的目标文件
static char DstFile[64] = {0};

static const char *Usage =
    "********************************************************************************\n"
    "OVERVIEW: Audio Decode Function Test Application! \n"
    "Usage: ./ADecodeTest [options] <arg> \n"
    "\nOPTIONS:\n"
    "-h [-?|--help]     Print ADecodeTest Usage Information And Exit \n"
    "-t [--type]        Decode Type : 1.G711A 2.G711U 3.MP3 4.AAC \n"
    "-s [--srcfile]     Source File To Decode \n"
    "-d [--dstfile]     Decode To Destination File \n"
    "\nDemo: ./ADecodeTest -t 1 -s 711A.enc -d 711A.dec \n"
    "\n********************************************************************************\n";

int PrintUsage()
{
    printf("%s\n", Usage);
    return 0;
}

void *G711DecThread(void *Argv)
{
    DecType_E Type = (DecType_E)Argv;
    int DecChannel;
    int i;
    FILE *EncFd, *DecFd;

    AUDIO_CHN_ATTR_S Attr;
    memset(&Attr, 0, sizeof(AUDIO_CHN_ATTR_S));

    switch (Type)
    {
    case DecG711A:
        Attr.enType = PT_G711A;
        break;
    case DecG711U:
        Attr.enType = PT_G711U;
        break;
    }

    // 给解码器分配多少缓冲区，字节
    Attr.u32BufSize = DEC_BUFFER_BYTES;

    // 创建一个解码通道
    DecChannel = ADEC_CreateChn(&Attr);

    ADUIO_STREAM_S StreamIn;
    memset(&StreamIn, 0, sizeof(ADUIO_STREAM_S));

    ADUIO_STREAM_S StreamOut;
    memset(&StreamOut, 0, sizeof(ADUIO_STREAM_S));

    // 打开源文件
    EncFd = fopen(SrcFile, "rb");

    if (EncFd == NULL)
    {
        printf("%s Open Failed!\n", SrcFile);
        return 0;
    }

    // 打开解码后数据写入文件
    DecFd = fopen(DstFile, "wb");

    if (DecFd == NULL)
    {
        printf("%s Open Failed!\n", DstFile);
        return 0;
    }

    // 分配原始码流读取缓冲区
    unsigned char *Buf;
    Buf = (unsigned char *)malloc(DEC_ONCE_BYTE);

    if (Buf == NULL)
    {
        printf("Out Of Memory!\n");
        return NULL;
    }

    for (i = 0; i < 10000; i++)
    {
        // 读取原始码流
        if (fread(Buf, 1, DEC_ONCE_BYTE, EncFd) != DEC_ONCE_BYTE)
            break;

        StreamIn.Stream.pStream = Buf;
        // G711可以传入任意长度，PCMA和PCMU均为8bit
        StreamIn.Stream.u32Len = DEC_ONCE_BYTE;
        // 帧序号和时间戳用户传进什么就会返回什么
        StreamIn.u32Seq = 0;
        StreamIn.u64TimeStamp = 0;

        // 解码
        ADEC_SendStream(DecChannel, &StreamIn, &StreamOut);
        // 解码后数据写入文件
        fwrite(StreamOut.Stream.pStream, 1, StreamOut.Stream.u32Len, DecFd);
    }

    fclose(EncFd);
    EncFd = NULL;

    fclose(DecFd);
    DecFd = NULL;

    free(Buf);
    Buf = NULL;

    ADEC_DestroyChn(DecChannel); // 销毁解码通道
}

void G711Dec(DecType_E type)
{
    pthread_t DecPid;
    pthread_create(&DecPid, NULL, G711DecThread, (void *)type);
    pthread_join(DecPid, NULL);
}

void Mp3DecCallback(unsigned char *Data0,
                    unsigned char *Data1,
                    int DecLen,
                    int Channel,
                    int SampleRate,
                    int BitRate,
                    void *UserData)
{
    FILE *Fd = (FILE *)UserData;
    // 按照回调函数传递上来的长度，把Data0数据写入文件
    // 若原始编码文件是单声道，则Data1为空
    // 若原始编码文件是双声道，则Data0和Data1分别保存两个声道的数据
    // 两个声道的数据长度都为DecLen
    // X5_ADEC_SendStream解码完整个文件后才会返回
    // 若回调函数中阻塞，X5_ADEC_SendStream也会阻塞
    fwrite(Data0, 1, DecLen, Fd);
}

void FileDec(DecType_E Type)
{
    int DecChannel;

    AUDIO_CHN_ATTR_S Attr;
    memset(&Attr, 0, sizeof(AUDIO_CHN_ATTR_S));

    switch (Type)
    {
    case DecMP3:
        Attr.enType = PT_MP3;
        break;
    case DecAAC:
        Attr.enType = PT_AAC;
        break;
    }

    // 不用分配缓冲区，更底层的编解码器会自动分配
    Attr.u32BufSize = 0;

    // 创建一个解码通道
    DecChannel = ADEC_CreateChn(&Attr);

    ADUIO_STREAM_S StreamIn;
    memset(&StreamIn, 0, sizeof(ADUIO_STREAM_S));

    FILE *DecFd;

    // 打开解码后数据写入文件
    DecFd = fopen(DstFile, "wb");

    if (DecFd == NULL)
    {
        printf("%s Open Failed!\n", DstFile);
        return;
    }

    StreamIn.File.FileName = SrcFile;
    StreamIn.File.DecCallback = &Mp3DecCallback;
    StreamIn.File.UserData = (void *)DecFd;

    ADEC_SendStream(DecChannel, &StreamIn, NULL);

    fclose(DecFd);
    DecFd = NULL;

    ADEC_DestroyChn(DecChannel);
}

int main(int argc, char **argv)
{
    char *short_opt = "h?t:s:d:";

    static struct option longopts[] =
        {{"help", no_argument, NULL, 'h'},
         {"type", required_argument, NULL, 't'},
         {"srcfile", required_argument, NULL, 's'},
         {"dstfile", required_argument, NULL, 'd'},
         {NULL, 0, 0, 0}};

    int opt;
    int option_index = 0;

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

        case 'd':
            strcpy(DstFile, optarg);
            break;

        default:
            return PrintUsage();
        }
    }

    // 初始化音频解码器
    ADEC_Init();

    // 解析传入解码类型
    switch (DecType)
    {
    case DecG711A:
    case DecG711U:
        G711Dec(DecType);
        break;

    case DecMP3:
        FileDec(DecMP3);
        break;

    case DecAAC:
        FileDec(DecAAC);
        break;

    default:
        printf("Unknow Decode Type! Please Specify Right Decode Type!\n");
        break;
    }

    ADEC_DeInit();

    return 0;
}
