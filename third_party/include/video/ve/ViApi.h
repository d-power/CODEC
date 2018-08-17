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
    unsigned int    Width;                  // 宽度
    unsigned int    Height;                 // 高度

} SIZE_S;


typedef struct _VI_DEV_ATTR_S
{
    unsigned int        u32TimePerFrame;    // 1s内帧数
    SIZE_S              stSize;             // 图像输入大小
    VI_DATA_FMT_E       enDataFmt;          // 图像输入格式
    unsigned int        u32Angle;           // 旋转，取值0,90,180,270

} VI_DEV_ATTR_S;


typedef struct _ENC_FRAME_S
{
    void            *HideField;         // 内部使用
    unsigned char   *PhyBuffer;         // 物理缓冲区地址
    unsigned char   *VirBuffer;         // 虚拟缓冲区地址
    unsigned int    Index;              // 缓冲区索引
    unsigned int    Flags;              // 帧标志
    SIZE_S          stSize;             // 图像输入大小

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
Others: 帧深度即VI通道中缓存视频帧数量
******************************************************************************/
int VI_SetFrameDepth(unsigned int Count);


/******************************************************************************
Function: VI_GetFrameDepth
Description: 获取帧深度
Param: void
Return: 返回当前帧深度值
Others:
******************************************************************************/
int VI_GetFrameDepth(void);


#endif // !__VIAPI_H__
