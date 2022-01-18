#ifndef __FY_COMM_JPEGE_H__
#define __FY_COMM_JPEGE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "fy_errno.h"
#include "fy_common.h"
#include "fy_comm_video.h"
#include "fy_comm_vb.h"

/* invlalid channel ID */
#define FY_ERR_JPEGE_INVALID_CHNID              FY_DEF_ERR(FY_ID_JPEGED, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
#define FY_ERR_JPEGE_ILLEGAL_PARAM              FY_DEF_ERR(FY_ID_JPEGED, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define FY_ERR_JPEGE_NULL_PTR                   FY_DEF_ERR(FY_ID_JPEGED, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define FY_ERR_JPEGE_NOT_CONFIG                 FY_DEF_ERR(FY_ID_JPEGED, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
#define FY_ERR_JPEGE_NOT_SUPPORT                FY_DEF_ERR(FY_ID_JPEGED, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define FY_ERR_JPEGE_NOT_PERM                   FY_DEF_ERR(FY_ID_JPEGED, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define FY_ERR_JPEGE_NOMEM                      FY_DEF_ERR(FY_ID_JPEGED, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define FY_ERR_JPEGE_NOBUF                      FY_DEF_ERR(FY_ID_JPEGED, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
#define FY_ERR_JPEGE_BUF_EMPTY                  FY_DEF_ERR(FY_ID_JPEGED, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
#define FY_ERR_JPEGE_BUF_FULL                   FY_DEF_ERR(FY_ID_JPEGED, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
#define FY_ERR_JPEGE_SYS_NOTREADY               FY_DEF_ERR(FY_ID_JPEGED, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define FY_ERR_JPEGE_BUSY                       FY_DEF_ERR(FY_ID_JPEGED, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define FY_ERR_JPEGE_EXIST                      FY_DEF_ERR(FY_ID_JPEGED, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
#define FY_ERR_JPEGE_UNEXIST                    FY_DEF_ERR(FY_ID_JPEGED, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)


typedef struct fyJPEGE_ATTR_JPEGE_S
{
    FY_U32  u32MaxPicWidth;                         /*maximum width of a picture to be encoded, in pixel*/
    FY_U32  u32MaxPicHeight;                        /*maximum height of a picture to be encoded, in pixel*/
    FY_U32  u32BufSize;                             /*stream buffer size*/

    FY_U32  u32PicWidth;                            /*width of a picture to be encoded, in pixel*/
    FY_U32  u32PicHeight;                           /*height of a picture to be encoded, in pixel*/
}JPEGE_CHN_ATTR_S;

typedef struct fyJPEGE_RECV_PIC_PARAM_S
{
    FY_S32 s32RecvPicNum;                          /*Number of frames received and encoded by the encoding channel*/
}JPEGE_RECV_PIC_PARAM_S;


typedef struct fyJPEGE_STREAM_S
{                      				
    FY_U32   u32PhyAddr;                         /*the physics address of stream*/
    FY_U8   *pu8Addr;                            /*the virtual address of stream*/
    FY_U32   u32Len;                             /*the length of stream*/

    FY_U64   u64PTS;                             /*PTS*/
    FY_U32   u32Qfactor;                         /* image quality */
    FY_S32   s32MilliSec;    
} JPEGE_STREAM_S;

typedef struct fyJPFEG_CHN_STAT_S
{
    FY_U32 u32LeftPics;                             /*left picture number */
    FY_U32 u32LeftStreamBytes;                      /*left stream bytes*/
    FY_U32 u32LeftStreamFrames;                     /*left stream frames*/
    FY_U32 u32CurPacks;                     /*left stream frames*/
    FY_U32 u32LeftRecvPics;                         /*Number of frames to be received. This member is valid after FY_MPI_JPEGE_StartRecvPicEx is called.*/
    FY_U32 u32LeftEncPics;                          /*Number of frames to be encoded. This member is valid after FY_MPI_JPEGE_StartRecvPicEx is called.*/
}JPEGE_CHN_STAT_S;


typedef struct fyJPEGE_PARAM_S
{
    FY_U32 u32Qfactor;                             /*image quality :[1,99]*/

    FY_U8  u8YQt[64];                              /* y qt value */
    FY_U8  u8CbQt[64];                             /* cb qt value */
    FY_U8  u8CrQt[64];                             /* cr qt value */

    FY_U32 u32MCUPerECS;                           /*default value: 0, MCU number of one ECS*/
} JPEGE_PARAM_S;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __FY_COMM_JPEGE_H__ */


