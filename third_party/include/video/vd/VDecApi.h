/******************************************************************************
Copyright(c) 2016-2018 Digital Power Inc.
File name: vdecapi.h
Author: LiuZhengzhong
Version: 1.0.0
Date: 2018/1/30
Description: Platform of DP X5 video decode C api
History:
Bug report: liuzhengzhong@d-power.com.cn
******************************************************************************/


#ifndef __VDECAPI_H__
#define __VDECAPI_H__

typedef long long int64_t;

typedef enum _PAYLOAD_TYPE_E
{
    PAYLOAD_TYPE_H264   = 1,
    PAYLOAD_TYPE_MJPEG  = 2,

} PAYLOAD_TYPE_E;


typedef enum _PIXEL_FORMAT_E
{
    FORMAT_YV12 = 1,
    FORMAT_NV21 = 2,

} PIXEL_FORMAT_E;


typedef struct _VDEC_CHN_ATTR_S            // 解码器属性
{
    PAYLOAD_TYPE_E      deType;             // 解码类型
    PIXEL_FORMAT_E      FormatType;         // 解码输出格式，默认YV12
    unsigned int        u32PicWidth;        // 解码宽度
    unsigned int        u32PicHight;        // 解码高度
    unsigned int        u32FrameRate;       // 解码帧率
    unsigned int        BufSize;            // 码流缓冲区大小，范围0x10000-0x800000,若值为0，默认0x100000

} VDEC_CHN_ATTR_S;


typedef struct _VDEC_STREAM_S              // 码流属性
{
    unsigned char       *pu8Addr;           // 码流地址
    unsigned int        u32Len;             // 码流长度
    unsigned long long  u64PTS;             // 时间戳

} VDEC_STREAM_S, *PTR_VDEC_STREAM_S;


typedef struct _VDEC_CHN_STAT_S            // 通道状态
{
    PAYLOAD_TYPE_E  deType;                 // 解码类型
    unsigned int    u32LeftPics;            // 缓冲区内有效未解码数据大小，字节
    unsigned int    u32FrameNum;            // 缓冲区内有效未解码数据帧数
    unsigned int    u32BufSize;             // 码流buffer总大小，字节，若解码器未初始化，为0

} VDEC_CHN_STAT_S;


typedef enum _VDEC_FORMAT_E
{
    HWC_FORMAT_MINVALUE     = 0x50,
    HWC_FORMAT_RGBA_8888    = 0x51,
    HWC_FORMAT_RGB_565      = 0x52,
    HWC_FORMAT_BGRA_8888    = 0x53,
    HWC_FORMAT_YCbYCr_422_I = 0x54,
    HWC_FORMAT_CbYCrY_422_I = 0x55,
    HWC_FORMAT_MBYUV420     = 0x56,
    HWC_FORMAT_MBYUV422     = 0x57,
    HWC_FORMAT_YUV420PLANAR = 0x58,
    HWC_FORMAT_YUV411PLANAR = 0x59,
    HWC_FORMAT_YUV422PLANAR = 0x60,
    HWC_FORMAT_YUV444PLANAR = 0x61,
    HWC_FORMAT_YUV420UVC    = 0x62,
    HWC_FORMAT_YUV420VUC    = 0x63,
    HWC_FORMAT_YUV422UVC    = 0x64,
    HWC_FORMAT_YUV422VUC    = 0x65,
    HWC_FORMAT_YUV411UVC    = 0x66,
    HWC_FORMAT_YUV411VUC    = 0x67,
    HWC_FORMAT_DEFAULT      = 0x99,    // The actual color format is determined
    HWC_FORMAT_MAXVALUE     = 0x100

} VDEC_FORMAT_E;


typedef struct _VDEC_SRC_INFO
{
    unsigned int    W;
    unsigned int    H;
    unsigned int    Crop_X;
    unsigned int    Crop_Y;
    unsigned int    Crop_W;
    unsigned int    Crop_H;
    VDEC_FORMAT_E   Format;

} VDEC_SRC_INFO;


typedef struct _VDEC_FRAME_S               // 解码图像信息
{
    VDEC_SRC_INFO       SrcInfo;            // 图像信息
    PIXEL_FORMAT_E      enPixelFormat;      // 像素格式
    void                *u32PhyAddr[3];     // 物理地址
    void                *pVirAddr[3];       // 虚拟地址
    unsigned int        u32Stride;          // 图像行宽，单位为像素
    unsigned long long  u64pts;             // 时间戳
    unsigned int        u32ValidPic;        // 解码器内显示队列中待显示图像个数
    unsigned int        u32TotalBuf;        // 解码器内共有多少个图像Buf
    unsigned int        u32EmptyBuf;        // 未被解码器和显示占用的图像Buf个数
    void                *Pic;               // 内部使用，用户无需关心此字段

} VDEC_FRAME_S;


typedef enum _VDEC_STATUS
{
    VDEC_RESULT_OK          = 0,        //
    VDEC_FRAME_DECODED      = 1,        // 解码成功，输出了一帧图像
    VDEC_CONTINUE           = 2,        // 解码成功，但没有图像输出，需要继续解码
    VDEC_KEYFRAME_DECODED   = 3,        // 解码成功，输出了一帧关键帧
    VDEC_NO_FRAME_BUFFER    = 4,        // 当前无法获取到图像Buffer
    VDEC_NO_BITSTREAM       = 5,        // 当前无法获取到码流数据
    VDEC_RESOLUTION_CHANGE  = 6,        // 视频分辨率发生变化，无法继续
    VDEC_UNSUPPORTED        = -1,       // 不能支持的格式或申请内存失败，无法继续解码
    VDEC_CHANNEL_ERROR      = -2,       // 解码通道错误

} VDEC_STATUS;


#define MAX_VID_DECODE_CHANNEL     8


/******************************************************************************
Function: VDEC_Init
Description: 初始化视频解码器
Param:
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VDEC_Init(void);


/******************************************************************************
Function: VDEC_DeInit
Description: 反初始化视频解码器
Param:
Return: 成功返回1，失败返回0
Others: 执行此函数会销毁所有视频通道
******************************************************************************/
int VDEC_DeInit(void);


/******************************************************************************
Function: VDEC_CreateChn
Description: 创建解码通道
Param:
    Attr    in      解码器属性
Return: 成功返回大于等于0的通道号，失败返回-1
Others:
******************************************************************************/
int VDEC_CreateChn(VDEC_CHN_ATTR_S *Attr);


/******************************************************************************
Function: VDEC_DestroyChn
Description: 销毁解码通道
Param:
    Channel   in      通道号
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VDEC_DestroyChn(int Channel);


/******************************************************************************
Function: VDEC_GetChnAttr
Description: 获取通道属性
Param:
    Channel     in      指定通道号
    Attr        out     接收数据结构体指针
Return: 成功返回1，失败返回0
Others: Attr的内容为强拷贝
******************************************************************************/
int VDEC_GetChnAttr(int Channel, VDEC_CHN_ATTR_S *Attr);


/******************************************************************************
Function: VDEC_StartRecvStream
Description: 开始解码
Param:
    Channel in      指定通道号
    DropB   in      当码流的时间戳比当前时间大时，是否丢弃B帧，取值0或1
    OnlyI   in      只解码I帧，取值0或1
    Time    in      当前时间，微秒，可以填0
Return:
Others: 解码一帧数据
******************************************************************************/
VDEC_STATUS VDEC_StartRecvStream(int Channel, unsigned int DropB, unsigned int OnlyI, int64_t Time);


/******************************************************************************
Function: VDEC_StopRecvStream
Description: 停止解码
Param:
    Channel in      指定通道号
Return:
Others:
******************************************************************************/
VDEC_STATUS VDEC_StopRecvStream(int Channel);


/******************************************************************************
Function: VDEC_SendStream
Description: 向解码器送一帧数据
Param:
    Channel     in      指定通道号
    Stream      in      码流信息
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VDEC_SendStream(int Channel, VDEC_STREAM_S *Stream);


/******************************************************************************
Function: VDEC_Query
Description: 查询通道状态
Param:
    Channel     in      指定通道号
    Stat        out     接收数据结构体指针
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VDEC_Query(int Channel, VDEC_CHN_STAT_S *Stat);


/******************************************************************************
Function: VDEC_ResetChn
Description: 复位通道
Param:
    Channel     in      指定通道号
Return: 成功返回1，失败返回0
Others: 解码器被重置，但初始化信息被保留，码流Buffer被清空，图像数据被清空
******************************************************************************/
int VDEC_ResetChn(int Channel);


/******************************************************************************
Function: VDEC_GetImage
Description: 获取解码数据
Param:
    Channel     in      指定通道号
    Frame       out     接收数据结构体指针
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VDEC_GetImage(int Channel, VDEC_FRAME_S *Frame);


/******************************************************************************
Function: VDEC_ReleaseImage
Description: 释放解码数据
Param:
    Channel     in      指定通道号
    Frame       in      X5_VDEC_GetImage所得结构体指针
Return: 成功返回1，失败返回0
Others: X5_VDEC_GetImage与X5_VDEC_ReleaseImage必须串行，成对出现
        不允许出现调用两次X5_VDEC_GetImage再X5_VDEC_ReleaseImage两次的情况
******************************************************************************/
int VDEC_ReleaseImage(int Channel, VDEC_FRAME_S *Frame);


#endif // !__VDECAPI_H__
