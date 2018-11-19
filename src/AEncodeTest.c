/******************************************************************************
Copyright(c) 2016-2018 Digital Power Inc.
File name: AEncodeTest.c
Author: LiuZhengzhong
Version: 1.0.4
Date: 2018/3/14
Description: Platform of DP X5 audio encode test
History:
Bug report: liuzhengzhong@d-power.com.cn
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <getopt.h>
#include <pthread.h>

#include <audio/codec/AEncApi.h>

#define ENC_BUFFER_BYTES 2048
// 每次编码多少个byte
#define ENC_ONCE_BYTE 160

// 解码类型枚举
typedef enum _EncType_E
{
    EncG711A = 1,
    EncG711U,
    EncMP3,
    EncAAC,
    EncBUT,

} EncType_E;

// 根据传入参数赋值的解码类型
EncType_E EncType;
// 根据传入参数赋值的解码源文件
static char SrcFile[64] = {0};
// 根据传入参数赋值的解码后写入的目标文件
static char DstFile[64] = {0};

static const char *Usage =
    "********************************************************************************\n"
    "OVERVIEW: Audio Encode Function Test Application! \n"
    "Usage: ./AEncodeTest [options] <arg> \n"
    "\nOPTIONS:\n"
    "-h [-?|--help]     Print AEncodeTest Usage Information And Exit \n"
    "-t [--type]        Encode Type : 1.G711A 2.G711U 3.MP3 4.AAC \n"
    "-s [--srcfile]     Source File To Encode. We provide pcm file with 44 byte wav head info.\n"
    "-d [--dstfile]     Encode To Destination File \n"
    "\nDemo: ./AEncodeTest -t 1 -s pcm.wav -d 711A.enc \n"
    "\n********************************************************************************\n";

int PrintUsage()
{
    printf("%s\n", Usage);
    return 0;
}

void *G711EncThread(void *Argv)
{
    EncType_E Type = (EncType_E)Argv;

    int EncChannel;
    int i;

    FILE *ReadFd, *WriteFd;

    AUDIO_CHN_ATTR_S Attr;
    memset(&Attr, 0, sizeof(AUDIO_CHN_ATTR_S));

    if (Type == EncG711A)
        Attr.enType = PT_G711A;
    else if (Type == EncG711U)
        Attr.enType = PT_G711U;

    // 给编码通道分配多少缓冲区，字节
    Attr.u32BufSize = ENC_BUFFER_BYTES;

    // 创建一个编码通道
    EncChannel = AENC_CreateChn(&Attr);

    if (EncChannel < 0)
    {
        printf("Create Channel Failed!\n");
        return NULL;
    }

    ADUIO_STREAM_S StreamIn;
    memset(&StreamIn, 0, sizeof(ADUIO_STREAM_S));

    ADUIO_STREAM_S StreamOut;
    memset(&StreamOut, 0, sizeof(ADUIO_STREAM_S));

    // 打开原始码流文件
    ReadFd = fopen(SrcFile, "rb");

    if (ReadFd == NULL)
    {
        printf("Open SrcFile[%s] Failed!\n", SrcFile);
        return 0;
    }

    // 打开编码后保存文件
    WriteFd = fopen(DstFile, "wb");

    if (WriteFd == NULL)
    {
        printf("Open DstFile[%s] Failed!\n", DstFile);
        return 0;
    }

    // 去掉44字节的wav头
    fseek(ReadFd, 44L, SEEK_SET);

    unsigned char *Buf;
    // 分配原始码流读取缓冲区
    Buf = (unsigned char *)malloc(ENC_ONCE_BYTE);

    if (Buf == NULL)
    {
        printf("Out Of Memory!\n");
        return NULL;
    }

    for (i = 0; i < 10000; i++)
    {
        // 读取原始码流
        if (fread(Buf, 1, ENC_ONCE_BYTE, ReadFd) != ENC_ONCE_BYTE)
            break;

        // 原始码流地址
        StreamIn.Stream.pStream = Buf;
        // 原始码流长度，G711可以传入任意2的倍数长度（PCM为16bit）
        StreamIn.Stream.u32Len = ENC_ONCE_BYTE;
        // 帧序号和时间戳用户传进什么就会返回什么
        StreamIn.u32Seq = 0;
        StreamIn.u64TimeStamp = 0;

        // 编码
        AENC_SendStream(EncChannel, &StreamIn, &StreamOut);
        // 编码后数据写入文件
        fwrite(StreamOut.Stream.pStream, 1, StreamOut.Stream.u32Len, WriteFd);
    }

    fclose(WriteFd);
    WriteFd = NULL;

    fclose(ReadFd);
    ReadFd = NULL;

    free(Buf);
    Buf = NULL;

    // 销毁编码通道
    AENC_DestroyChn(EncChannel);

    return NULL;
}

void G711Enc(EncType_E type)
{
    pthread_t EncPid;

    pthread_create(&EncPid, NULL, G711EncThread, (void *)type);

    pthread_join(EncPid, NULL);
}

void Mp3EncCallback(unsigned char *Data0,
                    unsigned char *Data1,
                    int EncLen,
                    int *End,
                    void *UserData)
{
    FILE *Fd = (FILE *)UserData;

    // 按照回调函数传递上来的长度，把数据写入Data0
    // 若写入结束，把*End置为1
    // 然后X5_AENC_SendStream也会返回
    // 否则编码器会一直处于循环状态
    // 回调函数阻塞也会导致编码器阻塞
    if (fread(Data0, 1, EncLen, Fd) != EncLen)
        *End = 1;
}

// 文件式编码
void FileEnc(EncType_E Type)
{
    int EncChannel;

    AUDIO_CHN_ATTR_S Attr;
    memset(&Attr, 0, sizeof(AUDIO_CHN_ATTR_S));

    if (Type == EncMP3)
        Attr.enType = PT_MP3;
    else if (Type == EncAAC)
        Attr.enType = PT_AAC;

    // MP3不用分配缓冲区，更底层的编解码器会自动分配
    Attr.u32BufSize = 0;
    // 原始PCM流为8k采样率
    Attr.u32SampleRate = 8000;

    EncChannel = AENC_CreateChn(&Attr);

    if (EncChannel < 0)
    {
        printf("Create Channel Failed!\n");
        return;
    }

    FILE *ReadFd;

    // 打开原始码流文件
    ReadFd = fopen(SrcFile, "rb");

    if (ReadFd == NULL)
    {
        printf("Open SrcFile[%s] Failed!\n", SrcFile);
        return;
    }

    // 去掉44字节的wav头
    fseek(ReadFd, 44L, SEEK_SET);

    ADUIO_STREAM_S StreamIn;
    memset(&StreamIn, 0, sizeof(ADUIO_STREAM_S));

    // 编码后文件
    StreamIn.File.FileName = DstFile;
    // 传递编码回调函数指针
    StreamIn.File.EncCallback = &Mp3EncCallback;
    // 传递用户自定义数据，即回调的UserData
    StreamIn.File.UserData = (void *)ReadFd;

    // 编码，MP3、AAC文件编码不需要传StreamOut
    AENC_SendStream(EncChannel, &StreamIn, NULL);

    fclose(ReadFd);
    ReadFd = NULL;

    AENC_DestroyChn(EncChannel);

    return;
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
            EncType = (EncType_E)atoi(optarg);
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

    AENC_Init();

    // 解析传入解码类型
    switch (EncType)
    {
    case EncG711A:
    case EncG711U:
        G711Enc(EncType);
        break;

    case EncMP3:
        FileEnc(EncMP3);
        break;

    case EncAAC:
        FileEnc(EncAAC);
        break;

    default:
        printf("Unknow Encode Type! Please Specify Right Encode Type!\n");
        break;
    }

    AENC_DeInit();

    return 0;
}
