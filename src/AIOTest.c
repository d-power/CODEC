/******************************************************************************
Copyright(c) 2016-2018 Digital Power Inc.
File name: AudioTest.c
Author: WangErchi
Version: 1.0.4
Date: 2018/3/13
Description: Platform of DP X5 video decode Test
History:
Bug report: wangerchi@d-power.com.cn
******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdarg.h>
#include <getopt.h>

#include <audio/AudioIn/AiApi.h>
#include <audio/AudioOut/AoApi.h>

static char RecordFile[64];
static char PlayFile[64];

static AIO_ATTR_S Attr;

static const char *kUsage =
    "********************************************************************************\n"
    "OVERVIEW: Audio Input/Output Function Test Application \n"
    "Usage: ./AIOTest [options] <arg> \n"
    "\nOPTIONS:\n"
    "-h     [-?|--help]     print AudioTest usage information and exit\n"
    "-R     [--RecFile]     enable record audio stream,define the output pcm stream file recorded\n"
    "-P     [--PlayFile]    enable play audio stream,define the pcm stream file player\n"
    "-B     [--Bits]        define the audio stream bitwidth, default is 16\n"
    "-C     [--Chn]         define the audio channel, mono or stereo, default is 1(mono, 2 for stereo)\n"
    "-M     [--Mode]        define the audio mode, master or slave, default is 0(master, 1 for slave)\n"
    "-S     [--Rate]        define the audio stream samplerate, default is 8000\n"
    "-n     [--CacheNum]    define the number of cache, default is 4\n"
    "-s     [--CacheSize]   define the size of cache, default is 20ms\n"
    "\n********************************************************************************\n";

int PrintUsage()
{
    printf("%s\n", kUsage);
    return 0;
}

static void *PlayBackThread(void *arg)
{
    printf("Start PlayBackThread!\n");

    FILE *Fd = fopen(PlayFile, "rb");

    if (Fd == NULL)
    {
        printf("Open RecordFile ERROR\n");
        return NULL;
    }

    // 首先使能AO设备
    if (AO_Enable() != 0)
    {
        printf("Error of AO_Enable !\n");
        return NULL;
    }

    // 全局变量Attr
    int Channel = AO_EnableChn(&Attr);

    if (Channel < 0)
    {
        printf("Error of AO_EnableChn !\n");
        return NULL;
    }

    // 获取当前通道的属性
    AIO_ATTR_S GetAttr;
    memset(&GetAttr, 0, sizeof(AIO_ATTR_S));

    if (AO_GetPubAttr(Channel, &GetAttr) < 0)
    {
        printf("Error of AO_GetPubAttr !\n");
        return NULL;
    }

    int i;
    AUDIO_FRAME_S Frame;
    memset(&Frame, 0, sizeof(AUDIO_FRAME_S));

    int SamplesPerBlock = GetAttr.enSampleRate *
                          GetAttr.Cache.u32BlockTime / 1000;

    int BlockSize = SamplesPerBlock *
                    GetAttr.enSoundMode *
                    (GetAttr.enBitWidth / 8);

    char *Buffer = (char *)malloc(BlockSize);

    if (Buffer == NULL)
    {
        printf("Out of Memory!\n");
        return NULL;
    }

    Frame.pVirAddr[0] = (void *)Buffer;
    Frame.u32Len = BlockSize;

    for (i = 0; i < 500; i++)
    {
        if (fread(Buffer, 1, BlockSize, Fd) != BlockSize)
        {
            fseek(Fd, 0, SEEK_SET);
            continue;
        }

        if (AO_SendFrame(Channel, &Frame,
                         BlockSize * GetAttr.Cache.u32BlockTime) != 0)
        {
            continue;
        }
    }

    AO_DisableChn(Channel);
    AO_Disable();

    free(Buffer);
    Buffer = NULL;

    return NULL;
}

static void *RecordThread(void *arg)
{
    printf("Start RecordThread!\n");

    FILE *Fd = fopen(RecordFile, "wb");

    if (Fd == NULL)
    {
        printf("Open RecordFile ERROR\n");
        return NULL;
    }

    // 首先使能Audio In设备
    if (AI_Enable() != 0)
    {
        printf("Error of AI_Enable !\n");
        return NULL;
    }

    // 打开回声消除
    AI_EnableAec();

    // 全局变量Attr
    // 创建一个Audio In通道，创建通道后，即会开始采集数据
    int Channel = AI_EnableChn(&Attr);

    if (Channel < 0)
    {
        printf("Error of AI_EnableChn !\n");
        return NULL;
    }

    // 获取当前通道的属性
    // AIO_ATTR_S GetAttr;
    // memset(&GetAttr, 0, sizeof(AIO_ATTR_S));

    // AI_GetPubAttr(Channel, &GetAttr);

    int i;
    // 超时时间
    int timeout = Attr.Cache.u32Blocks * Attr.Cache.u32BlockTime;
    AUDIO_FRAME_S Frame;
    memset(&Frame, 0, sizeof(AUDIO_FRAME_S));

    for (i = 0; i < 500; i++)
    {
        // 获取一帧
        if (AI_GetFrame(Channel, &Frame, timeout) == SUCCESS)
        {
            if (Frame.pVirAddr[0] != NULL)
            {
                fwrite(Frame.pVirAddr[0], 1, Frame.u32Len, Fd);
            }
        }
    }

    AI_DisableAec();
    AI_DisableChn(Channel);
    AI_Disable();

    fclose(Fd);
    Fd = NULL;

    return NULL;
}

int main(int argc, char **argv)
{
    char *short_opt = "h?R:P:B:C:M:S:n:s:";

    static struct option longopts[] = {{"help", no_argument, NULL, 'h'},
                                       {"RecFile", required_argument, NULL, 'R'},
                                       {"PlayFile", required_argument, NULL, 'P'},
                                       {"Bits", required_argument, NULL, 'B'},
                                       {"Chn", required_argument, NULL, 'C'},
                                       {"Mode", required_argument, NULL, 'M'},
                                       {"Rate", required_argument, NULL, 'S'},
                                       {"CacheNum", required_argument, NULL, 'n'},
                                       {"CacheSize", required_argument, NULL, 's'},
                                       {NULL, 0, 0, 0}};

    int opt;
    int option_index = 0;

    bool RecordFlag = false;
    bool PlayFlag = false;

    if (argc <= 1)
    {
        return PrintUsage();
    }

    memset(&Attr, 0, sizeof(AIO_ATTR_S));
    // 底层默认4个缓冲块
    Attr.Cache.u32Blocks = 4;
    // 底层默认每个块20ms的数据
    Attr.Cache.u32BlockTime = 20;
    // 我们需要使用默认块大小做同步读数据时的超时时间

    while ((opt = getopt_long(argc, argv, short_opt,
                              longopts, &option_index)) != -1)
    {
        switch (opt)
        {
        case '?':
        case 'h':
            return PrintUsage();

        case 'R':
            strcpy(RecordFile, optarg);
            RecordFlag = true;
            break;

        case 'B':
            // 位宽
            Attr.enBitWidth = (AUDIO_BIT_WIDTH_E)atoi(optarg);
            break;

        case 'C':
            // 单声道/双声道
            Attr.enSoundMode = (AIO_SOUND_MODE_E)atoi(optarg);
            break;

        case 'M':
            // 同步/异步
            Attr.enWorkMode = (AIO_MODE_E)atoi(optarg);
            break;

        case 'S':
            // 采样率
            Attr.enSampleRate = (AUDIO_SAMPLE_RATE_E)atoi(optarg);
            break;

        case 'P':
            strcpy(PlayFile, optarg);
            PlayFlag = true;
            break;

        case 'n':
            Attr.Cache.u32Blocks = (int)atoi(optarg);
            break;

        case 's':
            Attr.Cache.u32BlockTime = (int)atoi(optarg);
            break;

        default:
            return PrintUsage();
        }
    }

    pthread_t PlayBackThreadID;
    pthread_t RecordThreadID;

    // 播放文件 pcm流
    //char * play_file = "/mnt/play_file.raw";
    if (RecordFlag)
    {
        pthread_create(&RecordThreadID, NULL, RecordThread, NULL);
    }

    // 录音文件 为pcm流
    if (PlayFlag)
    {
        pthread_create(&PlayBackThreadID, NULL, PlayBackThread, NULL);
    }

    if (RecordFlag)
    {
        pthread_join(RecordThreadID, NULL);
    }

    if (PlayFlag)
    {
        pthread_join(PlayBackThreadID, NULL);
    }

    printf("exist\n");
    return 0;
}
