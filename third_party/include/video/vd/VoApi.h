/******************************************************************************
Copyright(c) 2016-2018 Digital Power Inc.
File name: voapi.h
Author: LiuZhengzhong
Version: 1.0.0
Date: 2018/1/30
Description: Platform of DP X5 display C api
History:
Bug report: liuzhengzhong@d-power.com.cn
******************************************************************************/


#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "VDecApi.h"


#define CHNANNEL_NUM            3       // 3个显示通道，固定
#define LAYER_NUM               4       // 每个通道4个图层，固定

#define ZORDER_MAX              CHNANNEL_NUM * LAYER_NUM - 1    // 11
#define ZORDER_MIN              0


typedef enum _VO_INTF_TYPE_E
{
    TYPE_UNAVAILABLE = 0,

} VO_INTF_TYPE_E;


typedef enum _VO_INTF_SYNC_E
{
    SYNC_UNAVAILABLE = 0,

} VO_INTF_SYNC_E;


typedef struct tagVO_SYNC_INFO_S
{

} VO_SYNC_INFO_S;


typedef struct _VO_PUB_ATTR_S
{
    unsigned int    u32BgColor;     // 背景色，从低位起，ARGB排序
    VO_INTF_TYPE_E  enIntfType;     // VO接口类型，空结构，无需赋值
    VO_INTF_SYNC_E  enIntfSync;     // VO接口格式，空结构，无需赋值
    VO_SYNC_INFO_S  stSyncInfo;     // VO接口时序配置，空结构，相关参数已在系统配置中提供

} VO_PUB_ATTR_S;


typedef struct _VO_LAYER_RECT
{
    int             X;              // 图层的X坐标
    int             Y;              // 图层的Y坐标
    unsigned int    W;              // 图层的宽度
    unsigned int    H;              // 图层的高度

} VO_LAYER_RECT;


typedef struct _VO_LAYER_INFO
{
    VO_LAYER_RECT       Rect;           // 位置信息
    unsigned int        LayerMode;      // 图层模式，0或1，0:带buffer的图层，1:无buffer，只用一个颜色值表示图像内容，通常使用带buffer的图层
    unsigned int        Color;          // 仅当图层模式为1时此成员有效
    unsigned char       AlphaValue;     // 全局Alpha值，默认0xFF

} VO_LAYER_INFO;


/******************************************************************************
Function: VO_Enable
Description: 打开显示设备
Param:
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VO_Enable(void);


/******************************************************************************
Function: VO_Disable
Description: 关闭显示设备
Param:
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VO_Disable(void);


/******************************************************************************
Function: VO_SetPubAttr
Description: 设置VO公共属性
Param:
    Attr    in      属性
Return: 成功返回1，失败返回0
Others: 暂时只支持设置背景色
******************************************************************************/
int VO_SetPubAttr(VO_PUB_ATTR_S *Attr);


/******************************************************************************
Function: VO_GetPubAttr
Description: 获取VO公共属性
Param:
    Attr    out      接收数据结构体指针
Return: 成功返回1，失败返回0
Others: 暂时只支持获取背景色信息
******************************************************************************/
int VO_GetPubAttr(VO_PUB_ATTR_S *Attr);


/******************************************************************************
Function: VO_EnableChn
Description: 使能视频通道
Param:
    Channel in      指定通道
    Info    in      图层参数结构
Return: 成功返回1，失败返回0
Others: Channel取值范围是0和1，两个视频通道供使用
        使能通道的同时，会使能该通道的第0个图层
******************************************************************************/
int VO_EnableChn(unsigned int Channel, VO_LAYER_INFO *Info);


/******************************************************************************
Function: VO_DisableChn
Description: 关闭视频通道
Param:
    Channel in      指定通道
Return: 成功返回1，失败返回0
Others: Channel取值范围是0和1，两个视频通道供使用
        关闭通道的同时，会关闭该通道的第0个图层
        如果有其他图层存在，并且0图层关闭，其他图层也无法显示
******************************************************************************/
int VO_DisableChn(unsigned int Channel);


/******************************************************************************
Function: VO_EnableVideoLayer
Description: 使能视频层
Param:
    Channel in      指定通道
    Layer   in      指定图层
    Info    in      图层参数结构
Return: 成功返回1，失败返回0
Others: Channel取值范围是0和1，两个视频通道供使用
        Layer取值范围是0到3，每个通道4层供使用
******************************************************************************/
int VO_EnableVideoLayer(unsigned int Channel, unsigned int Layer, VO_LAYER_INFO *Info);


/******************************************************************************
Function: VO_DisableVideoLayer
Description: 关闭视频层
Param:
    Channel in      指定通道
    Layer   in      指定图层
Return: 成功返回1，失败返回0
Others: Channel取值范围是0和1，两个视频通道供使用
        Layer取值范围是0到3，每个通道4层供使用
******************************************************************************/
int VO_DisableVideoLayer(unsigned int Channel, unsigned int Layer);


/******************************************************************************
Function: VO_SetVideoLayerAttr
Description: 设置视频层属性
Param:
    Channel in      指定通道
    Layer   in      指定图层
    Info    in      图层参数结构
Return: 成功返回1，失败返回0
Others: 空函数
******************************************************************************/
int VO_SetVideoLayerAttr(unsigned int Channel, unsigned int Layer, VO_LAYER_INFO *Info);


/******************************************************************************
Function: VO_GetVideoLayerAttr
Description: 获取视频层属性
Param:
    Channel in      指定通道
    Layer   in      指定图层
    Info    out     接收图层参数结构
Return: 成功返回1，失败返回0
Others: 空函数
******************************************************************************/
int VO_GetVideoLayerAttr(unsigned int Channel, unsigned int Layer, VO_LAYER_INFO *Info);


/******************************************************************************
Function: VO_SetZoomInWindow
Description: 设置缩放窗口
Param:
    Channel in      指定通道
    Layer   in      指定图层
    SrcInfo in      图像信息结构
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VO_SetZoomInWindow(unsigned int Channel, unsigned int Layer, VDEC_SRC_INFO *SrcInfo);


/******************************************************************************
Function: VO_ChnShow
Description: 显示指定窗口的图像
Param:
    Channel in      指定通道
    Layer   in      指定图层
    Frame   in      解码后的Frame结构
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int VO_ChnShow(unsigned int Channel, unsigned int Layer, VDEC_FRAME_S *Frame);



#endif // !__DISPLAY_H__
