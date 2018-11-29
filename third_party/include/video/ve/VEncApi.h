/******************************************************************************
Copyright(c) 2016-2018 Digital Power Inc.
File name: VEncApi.c
Author: LiuZhengzhong
Version: 1.0.0
Date: 2018/5/7
Description: Platform of DP X5 video encode C api define
History:
Bug report: liuzhengzhong@d-power.com.cn
******************************************************************************/

#ifndef __VENCAPI_H__
#define __VENCAPI_H__

#include "ViApi.h"

typedef enum _PAYLOAD_TYPE_E
{
    VENC_PAYLOAD_TYPE_H264 = 0,
    VENC_PAYLOAD_TYPE_JPEG,

} PAYLOAD_TYPE_E;

typedef struct _VENC_ATTR_S
{
    // 编码类型，默认PAYLOAD_TYPE_H264
    PAYLOAD_TYPE_E enType;
    // 源图像宽度，默认1280
    unsigned int u32SrcWidth;
    // 源图像高度，默认720
    unsigned int u32SrcHeight;
    // 目标图像宽度，默认640
    unsigned int u32DstWidth;
    // 目标图像高度，默认480
    unsigned int u32DstHeight;
    // 比特率，默认1 * 1024 * 1024
    unsigned int u32BitRate;
    // 帧率，默认30
    unsigned int u32FrameRate;
    // 是否开启动态码率，固定编码质量系数，0关闭，1开启
    unsigned int u32EnableQP;
    // I帧质量系数，范围0~51，值越小，质量越高
    unsigned int u32IQP;
    // P帧质量系数，范围0~51，值越小，质量越高
    unsigned int u32PQP;

} VENC_ATTR_S;

typedef enum _VENC_STATUS
{
    // 编码出错
    VENC_ERROR = -1,
    // 编码成功
    VENC_OK = 0,
    // 无法获取到输入帧
    VENC_NO_FRAME_BUFFER = 1,
    // 输出码流buffer已经溢出
    VENC_BITSTREAM_IS_FULL = 2

} VENC_STATUS;

typedef struct _VENC_STREAM_S
{
    // 时间戳
    long long s64Pts;
    // 是否I帧
    int s32Flag;
    // 码流第一部分长度
    unsigned int u32Size0;
    // 码流第二部分长度，当此字段不为0时，应用层需要把pData0，pData1拼接为完整的一帧
    unsigned int u32Size1;
    // 码流第一部分地址
    unsigned char *pData0;
    // 码流第二部分地址
    unsigned char *pData1;
    // 隐藏字段，内部使用
    void *HideField;

} VENC_STREAM_S;

typedef struct _VENC_STREAM_HEAD
{
    // 头缓冲区
    unsigned char *HeadBuffer; 
    // 头长度
    unsigned int HeadLengh;    

} VENC_STREAM_HEAD;

#define MAX_VID_ENCODE_CHANNEL 8

/******************************************************************************
Function: VENC_Init
Description: 初始化视频编码器
Param:
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VENC_Init(void);

/******************************************************************************
Function: VENC_DeInit
Description: 反初始化视频编码器
Param:
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VENC_DeInit(void);

/******************************************************************************
Function: VENC_CreateChn
Description: 创建编码通道
Param:
    Attr    in      编码器属性
Return: 成功返回大于等于0的通道号，失败返回-1
Others:
******************************************************************************/
int VENC_CreateChn(VENC_ATTR_S *Attr);

/******************************************************************************
Function: VENC_DestroyChn
Description: 销毁编码通道
Param:
    Channel     in      通道号
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VENC_DestroyChn(int Channel);

/******************************************************************************
Function: VENC_StartRecvPic
Description: 开始编码
Param:
    Channel     in      通道号
Return: VENC_STATUS中的某一个状态
Others: 此函数已被弃用
******************************************************************************/
VENC_STATUS VENC_StartRecvPic(int Channel);

/******************************************************************************
Function: VENC_StopRecvPic
Description: 停止编码
Param:
    Channel     in      通道号
Return:
Others: 此函数已被弃用
******************************************************************************/
VENC_STATUS VENC_StopRecvPic(int Channel);

/******************************************************************************
Function: VENC_SendFrame
Description: 向编码器送一帧数据
Param:
    Channel     in      通道号
    Frame       in      图像数据
Return: 成功返回1，失败返回0
Others: 在此函数内完成编码，后续即可调用VENC_GetStream获取编码后数据
******************************************************************************/
VENC_STATUS VENC_SendFrame(int Channel, ENC_FRAME_S *Frame);

/******************************************************************************
Function: VENC_GetStream
Description: 获取编码后的数据
Param:
    Channel     in      通道号
    Stream      out     数据指针
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VENC_GetStream(int Channel, VENC_STREAM_S *Stream);

/******************************************************************************
Function: VENC_ReleaseStream
Description: 释放编码后的数据
Param:
    Channel     in      通道号
    Stream      in      数据指针
Return: 成功返回1，失败返回0
Others: 此函数与VENC_GetStream成对出现
******************************************************************************/
int VENC_ReleaseStream(int Channel, VENC_STREAM_S *Stream);

/******************************************************************************
Function: VENC_RequestIDR
Description: 强制编码器下一帧为I帧
Param:
    Channel     in      通道号
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VENC_RequestIDR(int Channel);

/******************************************************************************
Function: VDEC_GetHeader
Description: 获取264头信息
Param:
    Channel     in      通道号
    Head        out     头信息
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VDEC_GetHeader(int Channel, VENC_STREAM_HEAD *Head);

#endif // !__VENCAPI_H__
