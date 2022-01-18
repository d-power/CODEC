#ifndef __JPEGD_H__
#define __JPEGD_H__

extern FY_S32 JPEGD_Init(void);
extern void JPEGD_DeInit(void);

extern FY_S32 JPEGD_CreateChn(VDEC_CHN VdChn, const fyVDEC_CHN_ATTR_S *pstAttr);
extern FY_S32 JPEGD_DestroyChn(VDEC_CHN VdChn);


extern FY_S32 JPEGD_GetChnAttr(VDEC_CHN VdChn, fyVDEC_CHN_ATTR_S *pstAttr);

extern FY_S32 JPEGD_StartRecvStream(VDEC_CHN VdChn);
extern FY_S32 JPEGD_StopRecvStream(VDEC_CHN VdChn);

extern FY_S32 JPEGD_Query(VDEC_CHN VdChn, fyVDEC_CHN_STAT_S *pstStat);

extern FY_S32 JPEGD_GetFd(VDEC_CHN VdChn);
extern FY_S32 JPEGD_CloseFd(VDEC_CHN VdChn);

extern FY_S32 JPEGD_ResetChn(VDEC_CHN VdChn);

extern FY_S32 JPEGD_SetChnParam(VDEC_CHN VdChn, VDEC_CHN_PARAM_S *pstParam);
extern FY_S32 JPEGD_GetChnParam(VDEC_CHN VdChn, VDEC_CHN_PARAM_S *pstParam);

extern FY_S32 JPEGD_SetProtocolParam(VDEC_CHN VdChn, VDEC_PRTCL_PARAM_S *pstParam);
extern FY_S32 JPEGD_GetProtocolParam(VDEC_CHN VdChn, VDEC_PRTCL_PARAM_S *pstParam);


extern FY_S32 JPEGD_SendStream(VDEC_CHN VdChn, const fyVDEC_STREAM_S *pstStream, FY_S32 s32MilliSec);

extern FY_S32 JPEGD_GetImage(VDEC_CHN VdChn, VIDEO_FRAME_INFO_S *pstFrameInfo, FY_S32 s32MilliSec);
extern FY_S32 JPEGD_ReleaseImage(VDEC_CHN VdChn, VIDEO_FRAME_INFO_S *pstFrameInfo);

extern FY_S32 JPEGD_GetUserData(VDEC_CHN VdChn, VDEC_USERDATA_S *pstUserData, FY_S32 s32MilliSec);
extern FY_S32 JPEGD_ReleaseUserData(VDEC_CHN VdChn, VDEC_USERDATA_S *pstUserData);

extern FY_S32 JPEGD_SetRotate(VDEC_CHN VdChn, ROTATE_E enRotate);
extern FY_S32 JPEGD_GetRotate(VDEC_CHN VdChn, ROTATE_E *penRotate);

extern FY_S32 JPEGD_GetChnLuma(VDEC_CHN VdChn, VDEC_CHN_LUM_S *pstLuma);

extern FY_S32 JPEGD_SetUserPic(VDEC_CHN VdChn, VIDEO_FRAME_INFO_S *pstUsrPic);
extern FY_S32 JPEGD_EnableUserPic(VDEC_CHN VdChn, FY_BOOL bInstant);
extern FY_S32 JPEGD_DisableUserPic(VDEC_CHN VdChn);

extern FY_S32 JPEGD_SetDisplayMode(VDEC_CHN VdChn, VIDEO_DISPLAY_MODE_E enDisplayMode);
extern FY_S32 JPEGD_GetDisplayMode(VDEC_CHN VdChn, VIDEO_DISPLAY_MODE_E *penDisplayMode);

extern FY_S32 JPEGD_SetChnVBCnt(VDEC_CHN VdChn, FY_U32 u32BlkCnt);
extern FY_S32 JPEGD_GetChnVBCnt(VDEC_CHN VdChn, FY_U32 *pu32BlkCnt);
extern FY_S32 JPEGD_AttachVbPool(VDEC_CHN VdChn, VDEC_CHN_POOL_S *pstPool);
extern FY_S32 JPEGD_DetachVbPool(VDEC_CHN VdChn);

extern FY_S32 JPEGD_SetModParam(VDEC_MOD_PARAM_S *pstModParam);
extern FY_S32 JPEGD_GetModParam(VDEC_MOD_PARAM_S *pstModParam);
extern FY_S32 JPEGD_SetLogParam(FY_U32 log);


#endif//__JPEGD_H__
