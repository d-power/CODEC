#ifndef __SYS_BIND_H__
#define __SYS_BIND_H__


#include "fy_type.h"
#include "fy_common.h"
#include "fy_comm_video.h"
#include "fy_comm_aio.h"
#include "fy_comm_sys.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


#define BIND_SRC_MAXNUM 8


typedef enum _MPP_DATA_TYPE_E
{
	MPP_DATA_VIU_FRAME,     /* VIDEO_FRAME_INFO_S   */
	MPP_DATA_VOU_FRAME,     /* VIDEO_FRAME_INFO_S */
	MPP_DATA_VDEC_FRAME,    /* VIDEO_FRAME_INFO_S   */
	MPP_DATA_VIDEO_FRAME,   /* VIDEO_FRAME_INFO_S   */
	MPP_DATA_VOU_WBC_FRAME,
	MPP_DATA_AUDIO_FRAME,   /* */
	MPP_DATA_BUTT
} MPP_DATA_TYPE_E;


typedef struct _VIDEO_FORMAT_S
{
	FY_U32          u32Width;
	FY_U32          u32Height;
	VIDEO_FIELD_E   u32Field;
	fyPIXEL_FORMAT_E  enPixelFormat;// bpp, planar,seimiplanar

	VIDEO_FORMAT_E  enVideoFormat;
	COMPRESS_MODE_E	enCompressMode;//no compress or compress mode
	void* prv;
}VIDEO_FORMAT_S;

typedef struct _AUDIO_FORMAT_S
{
	AUDIO_SAMPLE_RATE_E enSamplerate;   /* sample rate */
	AUDIO_BIT_WIDTH_E   enBitwidth;     /* bitwidth */
	AIO_MODE_E          enWorkmode;     /* master or slave mode */
	AUDIO_SOUND_MODE_E  enSoundmode;    /* momo or steror */
	void* prv;
}AUDIO_FORMAT_S;




typedef struct _MPP_BIND_DEST_S
{
	FY_U32    u32Num;
	MPP_CHN_S astMppChn[BIND_SRC_MAXNUM];
}MPP_BIND_DEST_S;


typedef int (*receiver_get_data_cb)(FY_S32 s32DevId, FY_S32 s32ChnId, FY_BOOL bBlock, MPP_DATA_TYPE_E enDataType, void* data);
typedef void (*receiver_reset_cb)(FY_S32 s32DevId, FY_S32 s32ChnId, void *data);
typedef void (*sender_bind_notify)(FY_S32 s32DevId, FY_S32 s32ChnId, MPP_BIND_DEST_S* dest);
// reciver_data[0], receivers_count, reciver_data[4 + 12*receivers_count] // modId,modDev, modChan
typedef void (*sender_get_data_format)(FY_S32 s32DevId, FY_S32 s32ChnId, MPP_DATA_TYPE_E* enDataType, void* fmt);
typedef int (*receive_check_data_format)(FY_S32 s32DevId, FY_S32 s32ChnId, MPP_DATA_TYPE_E enDataType, void* fmt);


typedef struct MPP_BIND_SENDER_S
{
	MOD_ID_E    enModId;
	unsigned int u32MaxDevCnt;
	unsigned int u32MaxChnCnt;
	sender_bind_notify notify;
	sender_get_data_format get_format;
}MPP_BIND_SENDER;

typedef struct MPP_BIND_RECEIVER_S
{
	MOD_ID_E    enModId;
	unsigned int u32MaxDevCnt;
	unsigned int u32MaxChnCnt;
	receiver_get_data_cb data_cb;
	receiver_reset_cb	   reset_cb;
	receive_check_data_format check_format;
}MPP_BIND_RECEIVER;

int SYS_BIND_Init(void);
int SYS_BIND_Exit(void);
int SYS_BIND_RegisterSender(MPP_BIND_SENDER *sender);
int SYS_BIND_RegisterReceiver(MPP_BIND_RECEIVER *receiver);
int SYS_BIND_UnRegisterSender(MOD_ID_E  enModId);
int SYS_BIND_UnRegisterReceiver(MOD_ID_E  enModId);

int SYS_Bind(MPP_CHN_S *pstSrcChn, MPP_CHN_S *pstDestChn);
int SYS_UnBind(MPP_CHN_S *pstSrcChn, MPP_CHN_S *pstDestChn);
int SYS_GetBindbyDest(MPP_CHN_S *pstDestChn, MPP_CHN_S *pstSrcChn);
int SYS_Iner_GetBindbySrc(MPP_CHN_S *pstSrcChn,MPP_BIND_DEST_S *dest);
int SYS_GetBindbySrc(MPP_CHN_S *pstSrcChn, MPP_BIND_DEST_S *dest);
int SYS_BIND_SendData(MOD_ID_E enModId, FY_S32 s32DevId, FY_S32 s32ChnId, FY_BOOL bBlock, MPP_DATA_TYPE_E enDataType, void* data);

const char* SYS_GetModName(MOD_ID_E enModId);

#define MPI_SYS_BIND_Init	SYS_BIND_Init
#define MPI_SYS_BIND_Exit	SYS_BIND_Exit
#define MPI_SYS_BIND_RegisterSender	SYS_BIND_RegisterSender
#define MPI_SYS_BIND_RegisterReceiver	SYS_BIND_RegisterReceiver
#define MPI_SYS_BIND_UnRegisterSender	SYS_BIND_UnRegisterSender
#define MPI_SYS_BIND_UnRegisterReceiver	SYS_BIND_UnRegisterReceiver

#define MPI_SYS_Bind 	SYS_Bind
#define MPI_SYS_UnBind	SYS_UnBind
#define MPI_SYS_GetBindbyDest	SYS_GetBindbyDest
#define MPI_SYS_Iner_GetBindbySrc	SYS_Iner_GetBindbySrc

#define MPI_SYS_BIND_SendData	SYS_BIND_SendData



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif//__SYS_BIND_H__

