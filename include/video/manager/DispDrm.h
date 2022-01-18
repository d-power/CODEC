/******************************************************************************
Copyright(c) 2011-2019 Digital Power Inc.
File name: Drm.h
Author: LiuZhengzhong
Version: 1.0.0
Date: 2019/07/18
Description: 
History: 
Bug report: liuzhengzhong@d-power.com.cn
******************************************************************************/

#ifndef __MANAGER_DISP_DRM_H__
#define __MANAGER_DISP_DRM_H__

#include <stdint.h>
#include <video/manager/Memory.h>
#include <video/manager/xf86drmMode.h>

// struct ModeSet_S
// {
// 	struct ModeSet_S *Next;
// 	// 显卡的文件标识符
// 	int Fd;
// 	// 代表第几个connector
// 	uint32_t OutputId;
// 	// connector模式
// 	drmModeModeInfo Mode;
// 	// connector的id
// 	uint32_t ConnId;
// 	// encoder的id
// 	uint32_t EncId;
// 	// crtc的id
// 	uint32_t CrtcId;
// 	// crtc索引，drm内部的数组索引
// 	uint32_t CrtcIdx;
// };

typedef struct _CameraBuffer_S
{
    // 通过ion申请的内存结构
    BufferHandle_S *Buffers;
    // 表示几个Planes
    int PlaneNum;
    // Framebuffer的handle
    unsigned int FbHandle;
    // drm buffer object
    unsigned int Bo;
    // 申请buffer的大小
    size_t 	Size;
    // 宽
    unsigned int Width;
    // 高
    unsigned int Height;
} CameraBuffer_S, RotBuffer_S;


/******************************************************************************
Function: GetDrmFd
Description: 获取drm的文件标识符
Param:
Return: 成功返回Fd，失败返回0
Others: 通常是内部调用使用
******************************************************************************/
int GetDrmFd(void);

/******************************************************************************
Function: CreateBuffer
Description: 创建摄像头或旋转模块的内存，并关联到fb
Param:
    Width		in		宽
    Height		in		高
    Fourcc		in		格式
    BufferNum	in		分配几块内存
    CamBuffer	in		外面传递这个指针进来，函数内部对指针成员赋值
Return: 
Others: VI会用到CreateBuffer和DestoryBuffer
******************************************************************************/
void CreateBuffer(int Width, int Height, int Fourcc, int BufferNum, CameraBuffer_S *CamBuffer);

/******************************************************************************
Function: DestoryBuffer
Description: 销毁buffer
Param: 
    BufferNum	in		buffer指针结构的长度
    CamBuffer	in		buffer结构起始地址
Return: 
Others: 与CreateBuffer成对使用
******************************************************************************/
void DestoryBuffer(int BufferNum, CameraBuffer_S *CamBuffer);

/******************************************************************************
Function: GetFbByPhyAddr
Description: 通过物理地址，置换出一个Drm Framebuffer的句柄，建立内存与显示的绑定关系
Param: 
    Width       in      buffer宽
    Height      in      buffer高
    Buffer      in      传递进去的buffer结构地址
    Fourcc      in      数据格式
    PlaneNum    out     返回Plane数量
    Bo          out     Buffer Object, drm内部使用，释放时需要传回去
Return: 成功返回FbHandle句柄值，失败返回0
Others: 
******************************************************************************/
int GetFbByPhyAddr(int Width, int Height, BufferHandle_S *Buffer, int Fourcc, int *PlaneNum, int *Bo);

/******************************************************************************
Function: RemoveFb
Description: 移除FbHandle
Param: 
    FbHandle    in      FbHandle句柄值
Return: 
Others: 与GetFbByPhyAddr成对使用
******************************************************************************/
void RemoveFb(int FbHandle, int Bo);

/******************************************************************************
Function: InitDisplayEngine
Description: 初始化显示引擎
Param: 
Return: 成功返回1，失败返回0
Others:
******************************************************************************/
int InitDisplayEngine();

/******************************************************************************
Function: UnInitDisplayEngine
Description: 反初始化显示引擎
Param: 
Return: 成功返回1，失败返回0
Others: 内部只会关闭drm句柄
******************************************************************************/
int UnInitDisplayEngine();

/******************************************************************************
Function: GetReservePlane
Description: 获取一个保留plane
Param:
Return: 
Others: 成功返回1，失败返回0
******************************************************************************/
// int GetReservePlane();

/******************************************************************************
Function: ReleaseReservePlane
Description: 释放一个保留plane
Param:
    PlaneId		in		要释放的PlaneId
Return: 
Others: 成功返回1，失败返回0
******************************************************************************/
// void ReleaseReservePlane(uint32_t PlaneId);

/******************************************************************************
Function: DisablePlanesAboveCurrent
Description: 禁用指定plane上面的所有plane
Param:
    CurrentPlaneId	in		指定的plane ID
Return: 
Others: 
******************************************************************************/
// void DisablePlanesAboveCurrent(uint32_t CurrentPlaneId);

/******************************************************************************
Function: DrmShow
Description: 指定位置刷新显示
Param:
    PlaneId		in		Plane id
    Fb			in		绑定的Fb文件标识符，VI CreateBuffer内申请出的Framebuffer Fd
    CX			in		显示位置X值，C代表CRTC
    CY			in		显示位置Y值，C代表CRTC
    CW			in		显示宽度，C代表CRTC
    CH			in		显示高度，C代表CRTC
    X			in		源数据X位置，VI会传递出来
    Y			in		源数据Y位置，VI会传递出来
    W			in		源数据宽度，VI会传递出来
    H			in		源数据高度，VI会传递出来
Return: 成功返回1，失败返回0
Others: 
******************************************************************************/
int DrmShow(uint32_t PlaneId, int Fb, int CX, int CY, int CW, int CH, int X, int Y, int W, int H);

#endif
