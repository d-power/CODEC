/******************************************************************************
Copyright(c) 2011-2019 Digital Power Inc.
File name: BufferList.h
Author: LiuZhengzhong
Version: 1.0.0
Date: 2019/08/03
Description: 
History: 
Bug report: liuzhengzhong@d-power.com.cn
******************************************************************************/

#ifndef __BUFFER_LIST_H__
#define __BUFFER_LIST_H__

#include <video/manager/Memory.h>
// #include <lombo_decplugin.h>

typedef struct _BufferList_S
{
    // 链表的下一个节点
    struct _BufferList_S    *Next;
    // 通过memory库申请出的内存结构
    BufferHandle_S  *Buffer;
    // 编解码器操作buffer的状态
    int Status;
    // 编解码器对这个buffer使用了多少大小
    int Size;

    // 编码状态：
    // 是否结束一帧的标志
    int EndFlag;
    // 帧类型
    int Flag;
    // 时间戳
    int Pts;

    // 解码状态：
    // 解码器对buffer的使用状态
    // dec_buf_handle_t DecUseInfo;
    // 解码缓冲区与framebuffer关联的handle
    // int FbHandle;

} BufferList_S;

/******************************************************************************
Function: CreateBufferList
Description: 创建一个新的链表
Param:
Return: 成功返回一个新的链表指针
Others: 
******************************************************************************/
BufferList_S *CreateBufferList(void);

/******************************************************************************
Function: AddBufferList
Description: 往List的最后一个节点添加一个节点
Param:
    List    in      任意的一个链表节点
Return: 成功返回最后一个节点的指针，失败返回空
Others:
******************************************************************************/
BufferList_S *AddBufferList(BufferList_S *List);

/******************************************************************************
Function: DeleteBufferList
Description: 删除当前的List节点
Param:
    Head    in      链表头节点的指针
    List    in      要删除的节点
Return: 成功返回下一个节点的地址，返回空证明无Next节点
Others: 如果头节点有变化，函数内部会对Head重新赋值
******************************************************************************/
BufferList_S *DeleteBufferList(BufferList_S **Head, BufferList_S *List);

/******************************************************************************
Function: RangeBufferList
Description: 以List节点起始，迭代，查看链表中的信息
Param:
    List    in      链表头节点
Return:
Others: 
******************************************************************************/
void RangeBufferList(BufferList_S *List);


#endif
