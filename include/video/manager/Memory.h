/******************************************************************************
Copyright(c) 2016-2018 Digital Power Inc.
File name: Memory.h
Author: LiuZhengzhong
Version: 1.0.0
Date: 2019/7/18
Description: memory control and shared all library, only for X9 platform
History:
Bug report: liuzhengzhong@d-power.com.cn
******************************************************************************/

#ifndef __MANAGER_MEMORY_H__
#define __MANAGER_MEMORY_H__

#include <stdio.h>

typedef struct _BufferHandle_S
{
    // 虚拟地址
    void *VirAddr;
    // 等于虚拟地址
    void *VirAddrAlign;
    // ion申请出的句柄
    int Handle;
    // 文件标识符
    int Fd;
    // 物理地址
    unsigned int PhyAddr;
    // 等于物理地址
    unsigned int PhyAddrAlign;
    // 申请了多大的缓冲区
    int BufSize;
}BufferHandle_S;

typedef enum _BufferType_E
{
    MEM_NORMAL = 0,
    // write back
    MEM_WRITE_BACK,
    // write throught
    MEM_WRITE_THROUGHT,
    // uncached
    MEM_UNCACHED,
}BufferType_E;


/******************************************************************************
Function: AllocMemory
Description: 分配一块内存
Param:
    Len         in      申请内存的长度
    BufferType  in      申请的内存类型
Return: 成功返回内存结构指针，失败返回空
Others: MEM_NORMAL类型只封装了malloc，申请的是用户空间内存，会在
        BufferHandle_S->VirAddr中携带malloc后的地址。其他三种类型，
        申请的是物理连续内存，BufferHandle_S中的所有成员都有效。
******************************************************************************/
BufferHandle_S *AllocMemory(size_t Len, BufferType_E BufferType);

/******************************************************************************
Function: MemroyFree
Description: 释放一块内存
Param:
    Buffer      in      要释放的内存结构
    Final       in      是否关闭内存管理设备
Return: 成功返回1，失败返回0
Others: AllocMemory中，第一次使用时会打开/dev/ion设备，最后一次释放，Final传1，
        关闭/dev/ion设备
******************************************************************************/
int MemroyFree(BufferHandle_S *Buffer, int Final);


#endif