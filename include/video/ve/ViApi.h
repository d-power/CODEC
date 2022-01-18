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

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum _VI_DATA_FMT_E
{
    // YU12
    VI_DATA_FMT_YUV420 = 0,
    // YU16
    VI_DATA_FMT_YUV422P,
    // NV12
    VI_DATA_FMT_NV12,
    // YUYV
    VI_DATA_FMT_YUYV,
    VI_DATA_FMT_BUT,

} VI_DATA_FMT_E;

typedef struct _RECT_S
{
    int Left;
    int Top;
    int Width;
    int Height;
}RECT_S;

typedef struct _SIZE_S
{
    // 宽度
    unsigned int Width;
    // 高度
    unsigned int Height;

} SIZE_S;

// X5的旋转属性部分摄像头可以支持
// X9的旋转属性全部可以支持
typedef enum _Angle_E
{
    // 不旋转
    Angle_0 = 0,
    // 旋转90度
    Angle_90 = 90,
    // 旋转180度，水平翻转+垂直翻转
    Angle_180 = 180,
    // 旋转270度，水平翻转+垂直翻转+旋转90
    Angle_270 = 270,
    // 垂直方向翻转
    Angle_VFLIP = 0xFF00,
    // 水平方向翻转
    Angle_HFLIP = 0xFF01,
    // X9，垂直翻转+旋转90度
    Angle_HFLIP_90 = 0xFF02,
    // X9，水平翻转+旋转90度
    Angle_VFLIP_90 = 0XFF03,
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
    // 是否裁剪图像图像 0为禁用 1为启用(注意调用前一定要将这个结构体清空,若需要裁剪图像在赋值1)
    int bCrop;
    // 裁剪区域
    RECT_S CropRect;
} ENC_FRAME_S;

// 单个水印的属性
typedef struct _WaterMarkInfo_S
{
    // 水印起始横坐标
    int X;
    // 水印起始横坐标
    int Y;
    // 水印显示内容
    char Buffer[64];
}WaterMarkInfo_S;

// 多个水印的属性
typedef struct _MWaterMarkInfo_S
{
    // 水印数量,最多5条
    int Count;
    WaterMarkInfo_S *pInfo;
}MWaterMarkInfo_S;

//字符和图片的映射表,用户自行映射好关系
typedef struct _PicMap_S
{
    //单个字符
    char val[4];
    //图片名字,只支持bmp
    char picname[32];
}PicMap_S;

/******************************************************************************
Function: EnableDev
Description: 使能Video IN设备
Param:
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VI_EnableDev(void);
int VI_EnableDev2(int Index);

/******************************************************************************
Function: DisableDev
Description: 关闭Video IN设备
Param:
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VI_DisableDev(void);
int VI_DisableDev2(int Index);

/******************************************************************************
Function: VI_SetDevAttr
Description: 设置设备属性
Param:
    Attr    in      设备属性
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VI_SetDevAttr(VI_DEV_ATTR_S *Attr);
int VI_SetDevAttr2(VI_DEV_ATTR_S *Attr, int Index);

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
int VI_GetFrame2(ENC_FRAME_S *Frame, int Index);

/******************************************************************************
Function: VI_ReleaseFrame
Description: 释放摄像头的图像数据
Param:
    Frame   in      图像参数结构
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VI_ReleaseFrame(ENC_FRAME_S *Frame);
int VI_ReleaseFrame2(ENC_FRAME_S *Frame, int Index);

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

/******************************************************************************
Function: VI_InitWaterMark
Description: 初始化水印
Param:
    PicPath   in      图片位置
    X         in      水印起始横坐标
    Y         in      水印起始纵坐标
    Count     in      pMap的长度
    pMap      in      图片和字符映射关系
Return: 成功返回1，失败返回0
Others: None
******************************************************************************/
int VI_InitWaterMark(char *PicPath, MWaterMarkInfo_S *pInfo, int Count, PicMap_S *pMap);

/******************************************************************************
Function: VI_ReleaseWaterMark
Description: 反初始化水印
Param: None
Return: None
Others: None
******************************************************************************/
void VI_ReleaseWaterMark();

/******************************************************************************
Function: VI_ReleaseWaterMark
Description: 反初始化水印
Param:
    InWidth     in      图像输入宽度
    InHigh      in      图像输入高度
    VirBufferY  in      虚拟缓冲区地址
    Count       in      pMap的长度
    pMap        in      图片和字符映射关系
Return: None
Others: None
******************************************************************************/
void VI_DoWaterMark(unsigned int InWidth, unsigned int InHigh, unsigned char *VirBufferY, int count, PicMap_S *pMap);

/******************************************************************************
Function: VI_AdjustCscParam(仅支持nvp6134解码芯片使用)
Description: 调整视频亮度、对比度、饱和度、色彩度
Param:
    bright     in      亮度
    contrast   in      对比度
    saturation in      饱和度
    hue        in      色彩度
Return: 成功返回1，失败返回-1
Others: None
******************************************************************************/
int VI_AdjustCscParam(int bright, int contrast, int saturation, int hue);

#ifdef __cplusplus
}
#endif

#endif // !__VIAPI_H__
