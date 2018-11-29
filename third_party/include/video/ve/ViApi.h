/******************************************************************************
Copyright(c) 2016-2018 Digital Power Inc.
File name: ViApi.h
Author: LiuZhengzhong
Version: 1.0.0
Date: 2018/5/7
Description: Platform of DP X5 video in C api define
History:
Bug report: liuzhengzhong@d-power.com.cn
******************************************************************************/

#ifndef __VIAPI_H__
#define __VIAPI_H__

#include <stdbool.h>

typedef enum _VI_DATA_FMT_E
{
    VI_DATA_FMT_YUV420 = 0,
    VI_DATA_FMT_YUV422P,
    VI_DATA_FMT_NV12,
    VI_DATA_FMT_YUYV,
    VI_DATA_FMT_BUT,

} VI_DATA_FMT_E;

typedef struct _SIZE_S
{
    // 宽度
    unsigned int Width;
    // 高度
    unsigned int Height;

} SIZE_S;

typedef enum _Angle_E
{
    // 不旋转
    Angle_0 = 0,
    // 旋转90度
    Angle_90 = 90,
    // 旋转180度
    Angle_180 = 180,
    // 旋转270度
    Angle_270 = 270,
    // 垂直方向翻转
    Angle_VFLIP = 0xFF00,
    // 水平方向翻转
    Angle_HFLIP = 0xFF01,

} Angle_E;

typedef struct _VI_DEV_ATTR_S
{
    // 1s内帧数，默认30
    unsigned int u32TimePerFrame;
    // 图像输入大小，默认1280,720
    SIZE_S stSize;
    // 图像输入格式，默认VI_DATA_FMT_YUV420
    VI_DATA_FMT_E enDataFmt;
    // 旋转、翻转，默认0为不旋转
    Angle_E u32Angle;
    // Buffer数量，默认3
    unsigned int BuffersCnt;

} VI_DEV_ATTR_S;

typedef struct _ENC_FRAME_S
{
    // 内部使用
    void *HideField;
    // 内部使用，物理缓冲区地址
    unsigned char *PhyBufferY;
    unsigned char *PhyBufferC;
    // 虚拟缓冲区地址，用户可根据该字段获取YUV数据
    unsigned char *VirBufferY;
    unsigned char *VirBufferC;
    // 内部使用，缓冲区索引
    unsigned int Index;
    // 内部使用，帧标志
    unsigned int Flags;
    // 图像输入大小，该属性已被废弃，赋任何值都没有意义
    SIZE_S stSize;

} ENC_FRAME_S;

/******************************************************************************
Function: EnableDev
Description: 使能Video IN设备
Param:
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VI_EnableDev(void);

/******************************************************************************
Function: DisableDev
Description: 关闭Video IN设备
Param:
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VI_DisableDev(void);

/******************************************************************************
Function: VI_SetDevAttr
Description: 设置设备属性
Param:
    Attr    in      设备属性
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VI_SetDevAttr(VI_DEV_ATTR_S *Attr);

/******************************************************************************
Function: VI_GetDevAttr
Description: 获取设备属性
Param:
    Attr    out      设备属性
Return: 成功返回1，失败返回0
Others: 
******************************************************************************/
int VI_GetDevAttr(VI_DEV_ATTR_S *Attr);

/******************************************************************************
Function: VI_GetFrame
Description: 获取摄像头的图像数据
Param:
    Frame   out     图像参数结构
Return: 成功返回1，失败返回0
Others: 
******************************************************************************/
int VI_GetFrame(ENC_FRAME_S *Frame);

/******************************************************************************
Function: VI_ReleaseFrame
Description: 释放摄像头的图像数据
Param:
    Frame   in      图像参数结构
Return: 成功返回1，失败返回0
Others: 
******************************************************************************/
int VI_ReleaseFrame(ENC_FRAME_S *Frame);

/******************************************************************************
Function: VI_SetFrameDepth
Description: 设置帧深度
Param:
    Count   in      帧深度
Return: 成功返回1，失败返回0
Others: 此函数已被弃用，帧深度由VI_DEV_ATTR_S中的BuffersCnt指定
******************************************************************************/
int VI_SetFrameDepth(unsigned int Count);

/******************************************************************************
Function: VI_GetFrameDepth
Description: 获取帧深度
Param: void
Return: 返回当前帧深度值
Others: 此函数已被弃用
******************************************************************************/
int VI_GetFrameDepth(void);

#endif // !__VIAPI_H__
