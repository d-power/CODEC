/******************************************************************************
Copyright(c) 2016-2018 Digital Power Inc.
File name: AudioCodecTest.c
Author: LiuZhengzhong
Version: 1.0.4
Date: 2018/3/14
Description: ffmpeg decode and encode C api wrapper test, support mp3 and aac
History:
Bug report: liuzhengzhong@d-power.com.cn
******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/audio/FFMPEG/AudioCodec.h"

#define PCM_FILE        "/mnt/extsd/Dahua/X5_CODEC/test/TestFile/Test_8000Hz_16bit_mono.wav"
#define ENC_MP3_FILE    "/mnt/extsd/Dahua/X5_CODEC/test/TestFile/Test_ENC_MP3.mp3"
#define DEC_MP3_FILE    "/mnt/extsd/Dahua/X5_CODEC/test/TestFile/Test_DEC_MP3.raw"
#define ENC_AAC_FILE    "/mnt/extsd/Dahua/X5_CODEC/test/TestFile/Test_ENC_AAC.aac"

static char Mp3EncOutBuf[4096];
static char Mp3DecOutBuf[4096];


int EncMp3(void)
{
    void *EncHandle;
    EncHandle = AudioCodecEncInit(CODEC_TYPE_MP3, 8000);

    if(!EncHandle)
    {
        printf("AudioCodecEncInit Failed!\n");
        return 0;
    }

    int i;
    FILE *PcmFile;
    FILE *OutFile;
    PcmFile = fopen(PCM_FILE, "rb");

    if(PcmFile == NULL)
    {
        printf("Can Not Open PCM File!\n");
        return 0;
    }

    OutFile = fopen(ENC_MP3_FILE, "wb");

    if(OutFile == NULL)
    {
        printf("Can Not Open MP3 File!\n");
        return 0;
    }

    unsigned char *Buf;
    fseek(PcmFile, 44L, SEEK_SET);      // 去掉44字节的wav头
    int BufSize = AudioCodecGetBufferSize(EncHandle);
    Buf = (unsigned char*)malloc(BufSize);

    if(Buf == NULL)
    {
        printf("Out Of Memory!\n");
        return 0;
    }

    int Result;

    for(i = 0; i < 10000; i++)
    {
        if(fread(Buf, 1, BufSize, PcmFile) != BufSize)
        {
            break;
        }

        Result = AudioCodecEncStream(EncHandle, Buf, BufSize, Mp3EncOutBuf, 4096);
        printf("Result=%d\n", Result);

        if(Result > 0)
        {
            fwrite(Mp3EncOutBuf, 1, Result, OutFile);
        }
    }

    printf("%d\n", i);
    fclose(PcmFile);
    PcmFile = NULL;
    fclose(OutFile);
    OutFile = NULL;
    free(Buf);
    Buf = NULL;
}


int DecMp3(void)
{
    void *DecHandle;
    DecHandle = AudioCodecDecInit(CODEC_TYPE_MP3);

    if(!DecHandle)
    {
        printf("AudioCodecDecInit Failed!\n");
        return 0;
    }

    int i;
    FILE *Mp3File;
    FILE *OutFile;
    Mp3File = fopen(ENC_MP3_FILE, "rb");
//    Mp3File = fopen("/mnt/extsd/Dahua/X5_CODEC/test/lock.mp3", "rb");

    if(Mp3File == NULL)
    {
        printf("Can Not Open ENC MP3 File!\n");
        return 0;
    }

    OutFile = fopen(DEC_MP3_FILE, "wb");

    if(OutFile == NULL)
    {
        printf("Can Not Open DEC MP3 File!\n");
        return 0;
    }

    unsigned char *Buf;
    int BufSize = 580;
    int OffSet = 0;
    Buf = (unsigned char*)malloc(BufSize);

    if(Buf == NULL)
    {
        printf("Out Of Memory!\n");
        return 0;
    }

    int Result;

    for(i = 0; i < 10000; i++)
    {
        fseek(Mp3File, OffSet, SEEK_CUR);

        if(fread(Buf, 1, BufSize, Mp3File) != BufSize)
            break;

        Result = AudioCodecDecStream(DecHandle, Buf, BufSize, Mp3DecOutBuf, 4096);

        if(Result > 0)
        {
            fwrite(Mp3DecOutBuf, 1, Result, OutFile);
            OffSet = Result / 2 - BufSize;
        }
    }

    fclose(Mp3File);
    Mp3File = NULL;
    fclose(OutFile);
    OutFile = NULL;
    free(Buf);
    Buf = NULL;
}


static void Mp3DecCallback(unsigned char  *Data0,
                           unsigned char  *Data1,
                           int            DecLen,
                           int            Channel,
                           int            SampleRate,
                           int            BitRate,
                           void           *UserData)
{
    printf("Data0[%x]\nData1[%x]\nDecLen[%d]\nChannel[%d]\nSampleRate[%d]\nBitRate[%d]\n\n",
           Data0, Data1, DecLen, Channel, SampleRate, BitRate);
    FILE *Fd = (FILE*)UserData;
    fwrite(Data0, 1, DecLen, Fd);
//    if (Data1)
//        fwrite(Data1, 1, DecLen, DecFileOut);
}


void DecMp3File(void)
{
    void *DecHandle;
    DecHandle = AudioCodecDecInit(CODEC_TYPE_MP3);

    if(!DecHandle)
    {
        printf("AudioCodecDecInit Failed!\n");
        return;
    }

    int Ret;
    char *Mp3File;
    FILE *DecFileOut;
    Mp3File = "/mnt/extsd/Dahua/X5_CODEC/test/TestFile/lock.mp3";
    DecFileOut = fopen("/mnt/extsd/Dahua/X5_CODEC/test/TestFile/lock.raw", "wb");

    if(DecFileOut == NULL)
    {
        printf("Can Not Open Out File!\n");
        return;
    }

    Ret = AudioCodecDecFile(DecHandle, Mp3File, &Mp3DecCallback, (void*)DecFileOut);
    fclose(DecFileOut);
    DecFileOut = NULL;
    AudioCodecDecUninit(DecHandle);
}


static void Mp3EncCallback(unsigned char  *Data0,
                           unsigned char  *Data1,
                           int            EncLen,
                           int            *End,
                           void           *UserData)
{
//    printf("EncLen=%d\n", EncLen);
    FILE *Fd = (FILE*)UserData;

    if(fread(Data0, 1, EncLen, Fd) != EncLen)
        *End = 1;
}


void EncMp3File(void)
{
    void *EncHandle;
    EncHandle = AudioCodecEncInit(CODEC_TYPE_MP3, 8000);

    if(!EncHandle)
    {
        printf("AudioCodecEncInit Failed!\n");
        return;
    }

    int Ret;
    char *EncFileOut;
    FILE *PcmFile;
    EncFileOut = "/mnt/extsd/Dahua/X5_CODEC/test/TestFile/EncMp3File.mp3";
    PcmFile = fopen("/mnt/extsd/Dahua/X5_CODEC/test/TestFile/Test_8000Hz_16bit_mono.wav", "rb");

    if(PcmFile == NULL)
    {
        printf("Can Not Open Out File!\n");
        return;
    }

    fseek(PcmFile, 44L, SEEK_SET);
    Ret = AudioCodecEncFile(EncHandle, EncFileOut, &Mp3EncCallback, (void*)PcmFile);
    fclose(PcmFile);
    PcmFile = NULL;
    AudioCodecEncUninit(EncHandle);
}


static void AacEncCallback(unsigned char  *Data0,
                           unsigned char  *Data1,
                           int            EncLen,
                           int            *End,
                           void           *UserData)
{
    FILE *Fd = (FILE*)UserData;

    if(fread(Data0, 1, EncLen, Fd) != EncLen)
        *End = 1;
}


void EncAacFile(void)
{
    void *EncHandle;
    EncHandle = AudioCodecEncInit(CODEC_TYPE_AAC, 8000);

    if(!EncHandle)
    {
        printf("AudioCodecEncInit Failed!\n");
        return;
    }

    int Ret;
    char *EncFileOut;
    FILE *PcmFile;
    EncFileOut = "/mnt/extsd/Dahua/X5_CODEC/test/TestFile/EncAacFile.aac";
    PcmFile = fopen("/mnt/extsd/Dahua/X5_CODEC/test/TestFile/Test_8000Hz_16bit_mono.wav", "rb");

    if(PcmFile == NULL)
    {
        printf("Can Not Open Out File!\n");
        return;
    }

    fseek(PcmFile, 44L, SEEK_SET);
    Ret = AudioCodecEncFile(EncHandle, EncFileOut, &AacEncCallback, (void*)PcmFile);
    fclose(PcmFile);
    PcmFile = NULL;
    AudioCodecEncUninit(EncHandle);
}


static void AacDecCallback(unsigned char  *Data0,
                           unsigned char  *Data1,
                           int            DecLen,
                           int            Channel,
                           int            SampleRate,
                           int            BitRate,
                           void           *UserData)
{
    printf("Data0[%x]\nData1[%x]\nDecLen[%d]\nChannel[%d]\nSampleRate[%d]\nBitRate[%d]\n\n",
           Data0, Data1, DecLen, Channel, SampleRate, BitRate);
    FILE *Fd = (FILE*)UserData;
    fwrite(Data0, 1, DecLen, Fd);
}


void DecAacFile(void)
{
    void *DecHandle;
    DecHandle = AudioCodecDecInit(CODEC_TYPE_AAC);

    if(!DecHandle)
    {
        printf("AudioCodecDecInit Failed!\n");
        return;
    }

    int Ret;
    char *AacFile;
    FILE *DecFileOut;
    AacFile = "/mnt/extsd/Dahua/X5_CODEC/test/TestFile/EncAacFile.aac";
    DecFileOut = fopen("/mnt/extsd/Dahua/X5_CODEC/test/TestFile/DecAacFile.raw", "wb");

    if(DecFileOut == NULL)
    {
        printf("Can Not Open Out File!\n");
        return;
    }

    Ret = AudioCodecDecFile(DecHandle, AacFile, &AacDecCallback, (void*)DecFileOut);
    fclose(DecFileOut);
    DecFileOut = NULL;
    AudioCodecDecUninit(DecHandle);
}


int main()
{
    AudioCodecInit();
//    EncMp3();
//    DecMp3();
    EncMp3File();
    DecMp3File();
    printf("\n\n");
    EncAacFile();
    DecAacFile();
}
