/******************************************************************************
Copyright(c) 2016-2018 Digital Power Inc.
File name: WaterMarkTest.c
Author: liaosenlin
Version: 1.0.0
Date: 2018/08/23
Description: WaterMarkTest
History:
Bug report: liaosenlin@d-power.com.cn
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
#include <time.h>
#include <sys/time.h>

#include <dirent.h>
#include <sys/stat.h>

#include <video/ve/ViApi.h>
#include <video/vd/VoApi.h>
#include <video/ve/VEncApi.h>

static unsigned int InWidth = 1280;
static unsigned int InHigh = 720;
static unsigned int InBufferCount = 3;
static unsigned int DisplayWidth = 480;
static unsigned int DisplayHigh = 272;
static unsigned int DisplayX = 0;
static unsigned int DisplayY = 0;

static int SRC_WIDTH = 1280;
static int SRC_HEIGHT = 720;

static int DST_WIDTH = 640;
static int DST_HEIGHT = 360;

static char H264DstFile[] = {"/mnt/1.264"};
static char JpgDstFile[] = {"/mnt/1.jpg"};

static int Channel = -1;

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

static void getTimeString(char *str, int len)
{
    time_t timep;
    struct tm *p;
    int year, month, day, hour, min, sec;
    
    time(&timep);
    
    p = localtime(&timep);
    
    year = p->tm_year + 1900;
    month = p->tm_mon + 1;
    day = p->tm_mday;
    hour = p->tm_hour;
    min = p->tm_min;
    sec = p->tm_sec;

    snprintf(str, len, "%d-%02d-%02d %02d:%02d:%02d 新 港", year, month, day, hour, min, sec);
}

PicMap_S Map[] = 
{
    {"5", "5.bmp"},
    {"6", "6.bmp"},
    {"7", "7.bmp"},
    {"8", "8.bmp"},
    {"9", "9.bmp"},
    {" ", "10.bmp"},
    {"-", "11.bmp"},
    {":", "12.bmp"},
    {"0", "你.bmp"},
    {"1", "1.bmp"},
    {"2", "2.bmp"},
    {"3", "3.bmp"},
    {"4", "4.bmp"},
    {"新", "wm_540p_80.bmp"},
    {"港", "wm_540p_81.bmp"}
};

void Preview()
{
    int DisplayChannel = 1;
    int DisplayLayer = 0;

    // 使能显示
    if (!VO_Enable())
    {
        printf("VO_Enable() Failed!\n");
        return ;
    }

    FlushFrameBuffer(0x80000000);


    VO_LAYER_INFO VoInfo;
    memset(&VoInfo, 0, sizeof(VO_LAYER_INFO));

    VoInfo.Rect.X = DisplayX;
    VoInfo.Rect.Y = DisplayY;
    VoInfo.Rect.W = DisplayWidth;
    VoInfo.Rect.H = DisplayHigh;

    VO_EnableChn(DisplayChannel, &VoInfo);

    // 使能设备video in设备，即/dev/video0
    if (!VI_EnableDev())
    {
        printf("VI_EnableDev() Failed!\n");
        return;
    }

    VI_DEV_ATTR_S Attr;
    memset(&Attr, 0, sizeof(VI_DEV_ATTR_S));

    Attr.enDataFmt = VI_DATA_FMT_NV12;//VI_DATA_FMT_YUV420;
    Attr.stSize.Width = InWidth;
    Attr.stSize.Height = InHigh;
    // Attr.u32Angle = Angle_VFLIP;
    Attr.u32TimePerFrame = 30;
    Attr.BuffersCnt = InBufferCount;

    // 设置video in属性
    if (!VI_SetDevAttr(&Attr))
    {
        printf("VI_SetDevAttr() Failed!\n");
        return;
    }

    int Count;
    int ArrCount = sizeof(Map)/ sizeof(Map[0]);

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
    VO_Frame.SrcInfo.Format = HWC_FORMAT_YUV420UVC; //HWC_FORMAT_YUV420PLANAR;

    if (!VO_SetZoomInWindow(DisplayChannel, DisplayLayer, &VO_Frame.SrcInfo))
    {
        printf("VO_SetZoomInWindow Error!\n");
    }

    MWaterMarkInfo_S *pMInfo;
    pMInfo = (MWaterMarkInfo_S *)malloc(sizeof(MWaterMarkInfo_S));
    
    memset(pMInfo, 0, sizeof(*pMInfo));

    pMInfo->Count = 5; 
    pMInfo->pInfo = (WaterMarkInfo_S *)malloc(sizeof(WaterMarkInfo_S) * pMInfo->Count);
    
    memset(pMInfo->pInfo, 0, sizeof(WaterMarkInfo_S) * pMInfo->Count);

    for(Count = 0; Count < pMInfo->Count; ++Count)
    {
        pMInfo->pInfo[Count].X = 0;
        pMInfo->pInfo[Count].Y = Count * 80;
    }

    VI_InitWaterMark("/mnt/watermark", pMInfo, ArrCount, Map);

    // 预览1000帧
    for(Count = 0; Count < 1000; Count++)
    {
        // 从video in获取一帧
        if (!VI_GetFrame(&VI_Frame))
        {
            printf("VI_GetFrame() Failed!\n");
            return;
        }

        getTimeString(pMInfo->pInfo[0].Buffer, 128);
        getTimeString(pMInfo->pInfo[1].Buffer, 128);
        getTimeString(pMInfo->pInfo[2].Buffer, 128);
        getTimeString(pMInfo->pInfo[3].Buffer, 128);
        getTimeString(pMInfo->pInfo[4].Buffer, 128);
       
        VI_DoWaterMark(InWidth, InHigh, VI_Frame.VirBufferY, ArrCount, Map);

        VO_Frame.u32PhyAddr[0] = (void *)VI_Frame.PhyBufferY;
        VO_Frame.u32PhyAddr[1] = (void *)VI_Frame.PhyBufferC;
        VO_Frame.u32PhyAddr[2] = (void *)(VI_Frame.PhyBufferC + InWidth * InHigh / 4);

        if(!VO_ChnShow(DisplayChannel, DisplayLayer, &VO_Frame))
        {
            printf("VO_ChnShow Error!\n");
            continue;
        }

        if(!VI_ReleaseFrame(&VI_Frame))
        {
            printf("VI_ReleaseFrame() Failed!\n");
            return;
        }
    }

    VI_ReleaseWaterMark();

    VO_DisableVideoLayer(DisplayChannel, DisplayLayer);

    if(!VI_DisableDev())
    {
        printf("DisableDev() Failed!\n");
    }

    // 禁用显示
    VO_Disable();

    return;
}

void *EncodeJPGThread(void *Param)
{
    FILE *Fd = NULL;
    Fd = fopen(JpgDstFile, "wb");

    if (Fd == NULL)
    {
        printf("Can Not Open File[%s]\n", JpgDstFile);
        return NULL;
    }

    int Count;
    int ArrCount = sizeof(Map)/ sizeof(Map[0]);

    MWaterMarkInfo_S *pMInfo;
    pMInfo = (MWaterMarkInfo_S *)malloc(sizeof(MWaterMarkInfo_S));
    
    memset(pMInfo, 0, sizeof(*pMInfo));

    pMInfo->Count = 5; 
    pMInfo->pInfo = (WaterMarkInfo_S *)malloc(sizeof(WaterMarkInfo_S) * pMInfo->Count);
    
    memset(pMInfo->pInfo, 0, sizeof(WaterMarkInfo_S) * pMInfo->Count);

    for(Count = 0; Count < pMInfo->Count; ++Count)
    {
        pMInfo->pInfo[Count].X = 0;
        pMInfo->pInfo[Count].Y = Count * 80;
    }

    VI_InitWaterMark("/mnt/watermark", pMInfo, ArrCount, Map);

    // 多取几帧再存图片，防止摄像头刚启动时可能出现的问题
    for(Count = 0; Count <= 10; Count++)
    {
        ENC_FRAME_S Frame;

        // 从video in获取一帧
        if(!VI_GetFrame(&Frame))
        {
            printf("VI_GetFrame() Failed!\n");
            return NULL;
        }

        getTimeString(pMInfo->pInfo[0].Buffer, 128);
        getTimeString(pMInfo->pInfo[1].Buffer, 128);
        getTimeString(pMInfo->pInfo[2].Buffer, 128);
        getTimeString(pMInfo->pInfo[3].Buffer, 128);
        getTimeString(pMInfo->pInfo[4].Buffer, 128);

        VI_DoWaterMark(InWidth, InHigh, Frame.VirBufferY, ArrCount, Map);

        // 送给编码器
        if(VENC_SendFrame(Channel, &Frame) < 0)
        {
            printf("VENC_SendFrame() Failed!\n");
            VI_ReleaseWaterMark();
            return NULL;
        }

        VENC_STREAM_S Stream;
        memset(&Stream, 0, sizeof(VENC_STREAM_S));

        // 获取编码后的数据
        if(!VENC_GetStream(Channel, &Stream))
        {
            printf("VENC_GetStream() Failed!\n");
            VI_ReleaseWaterMark();
            return NULL;
        }

        if (Count == 10)
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
            VI_ReleaseWaterMark();
            return NULL;
        }

        // 释放video in内的一帧
        if(!VI_ReleaseFrame(&Frame))
        {
            printf("VI_ReleaseFrame() Failed!\n");
            VI_ReleaseWaterMark();
            return NULL;
        }
    }

    VI_ReleaseWaterMark();
    
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

    return NULL;
}

void *EncodeThread(void *Param)
{
    VENC_STREAM_HEAD Head;
    
    int Count;
    int ArrCount = sizeof(Map)/ sizeof(Map[0]);

    // 获取H264头信息
    if(VDEC_GetHeader(Channel, &Head))
    {
        printf("X5_VDEC_GetHeader() Successful\n");
    }

    FILE *Fd = NULL;
    Fd = fopen(H264DstFile, "wb");

    if (Fd == NULL)
    {
        printf("Can Not Open File[%s]\n", H264DstFile);
        return NULL;
    }

    // 向文件写入头信息
    fwrite(Head.HeadBuffer, 1, Head.HeadLengh, Fd);

    MWaterMarkInfo_S *pMInfo;
    pMInfo = (MWaterMarkInfo_S *)malloc(sizeof(MWaterMarkInfo_S));
    
    memset(pMInfo, 0, sizeof(*pMInfo));

    pMInfo->Count = 5; 
    pMInfo->pInfo = (WaterMarkInfo_S *)malloc(sizeof(WaterMarkInfo_S) * pMInfo->Count);
    
    memset(pMInfo->pInfo, 0, sizeof(WaterMarkInfo_S) * pMInfo->Count);

    for(Count = 0; Count < pMInfo->Count; ++Count)
    {
        pMInfo->pInfo[Count].X = 0;
        pMInfo->pInfo[Count].Y = Count * 80;
    }

    VI_InitWaterMark("/mnt/watermark", pMInfo, ArrCount, Map);

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

        getTimeString(pMInfo->pInfo[0].Buffer, 128);
        getTimeString(pMInfo->pInfo[1].Buffer, 128);
        getTimeString(pMInfo->pInfo[2].Buffer, 128);
        getTimeString(pMInfo->pInfo[3].Buffer, 128);
        getTimeString(pMInfo->pInfo[4].Buffer, 128);
        
        VI_DoWaterMark(InWidth, InHigh, Frame.VirBufferY, ArrCount, Map);

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

    VI_ReleaseWaterMark();

    fclose(Fd);
    Fd = NULL;

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

    return NULL;
}

void EnCodeWithWaterMark(int VDecType)
{
    FlushFrameBuffer(0x80000000);
    
    if(!VI_EnableDev())
    {
        printf("EnableDev() Failed!\n");
        return ;
    }

    // 初始化编码器
    if(!VENC_Init())
    {
        printf("VENC_Init() Failed!\n");
        return ;
    }

    VENC_ATTR_S EncAttr;
    memset(&EncAttr, 0, sizeof(VENC_ATTR_S));

    EncAttr.enType = (VDecType == 0) ? VENC_PAYLOAD_TYPE_H264 : VENC_PAYLOAD_TYPE_JPEG;//VENC_PAYLOAD_TYPE_H264;//  : VENC_PAYLOAD_TYPE_JPEG;
    EncAttr.u32SrcWidth = SRC_WIDTH;
    EncAttr.u32SrcHeight = SRC_HEIGHT;
    EncAttr.u32DstWidth = DST_WIDTH;//SRC_WIDTH;
    EncAttr.u32DstHeight = DST_HEIGHT;
    EncAttr.u32BitRate = 256 * 1024 *1024;
    EncAttr.fmtType = VENC_PIXEL_FMT_YUV420SP;
//    EncAttr.u32EnableQP = 1;
//    EncAttr.u32IQP = 30;
//    EncAttr.u32PQP = 45;d

    // 创建编码器
    Channel = VENC_CreateChn(&EncAttr);

    if(Channel < 0)
    {
        printf("VENC_CreateChn() Failed!\n");
        return ;
    }

    VI_DEV_ATTR_S Attr;
    memset(&Attr, 0, sizeof(VI_DEV_ATTR_S));

    Attr.stSize.Width = SRC_WIDTH;
    Attr.stSize.Height = SRC_HEIGHT;
    Attr.enDataFmt = VI_DATA_FMT_NV12;
    
    // 设置video in属性
    if(!VI_SetDevAttr(&Attr))
    {
        printf("VI_SetDevAttr() Failed!\n");
        return ;
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
}

int main(int argc, char **argv)
{
    printf("Start WaterMark Test!\n");
    
    Preview();
    
    //EnCodeWithWaterMark(0);
    //EnCodeWithWaterMark(1);

    return 0;
}
