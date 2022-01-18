#ifndef __FY_VPU_IOC__
#define __FY_VPU_IOC__

#include "../fy_comm_vpss.h"
#include "../fy_comm_vb.h"
// #include "../fy_comm_coolview.h"
#include "drv_ioc_extension.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define VPU_IOC_MAGIC 'P'

/**@}*/

/**
*@name VPU IOctrl cmd
*@{
*/

#define VPU_IOC_MAGIC 'P'

#define IOC_VPSS_CREATE_GRP         _IOW(VPU_IOC_MAGIC,1,VPU_GRP_ATTR_S)
#define IOC_VPSS_DESTORY_GRP        _IO(VPU_IOC_MAGIC,2)
#define IOC_VPSS_SET_GRP_ATTR       _IOW(VPU_IOC_MAGIC,3,VPU_GRP_ATTR_S)
#define IOC_VPSS_GET_GRP_ATTR       _IOR(VPU_IOC_MAGIC,4,VPU_GRP_ATTR_S)
#define IOC_VPSS_START_GRP          _IO(VPU_IOC_MAGIC,5)
#define IOC_VPSS_STOP_GRP           _IO(VPU_IOC_MAGIC,6)
#define IOC_VPSS_SET_CHN_ATTR       _IOW(VPU_IOC_MAGIC,7,VPU_CHN_ATTR_S)
#define IOC_VPSS_GET_CHN_ATTR       _IOR(VPU_IOC_MAGIC,8,VPU_CHN_ATTR_S)
#define IOC_VPSS_SET_CHN_MODE       _IOW(VPU_IOC_MAGIC,9,VPU_CHN_MODE_S)
#define IOC_VPSS_GET_CHN_MODE       _IOR(VPU_IOC_MAGIC,10,VPU_CHN_MODE_S)
#define IOC_VPSS_ENABLE_CHN         _IO(VPU_IOC_MAGIC,11)
#define IOC_VPSS_DISABLE_CHN        _IO(VPU_IOC_MAGIC,12)
#define IOC_VPSS_SET_MASK_FUNC      _IOW(VPU_IOC_MAGIC,13,VPSS_MASK_INFO_S)
#define IOC_VPSS_GET_MASK_FUNC      _IOR(VPU_IOC_MAGIC,14,VPSS_MASK_INFO_S)
#define IOC_VPSS_GET_CPY            _IOWR(VPU_IOC_MAGIC,20,VPSS_PIC_DATA_S)
#define IOC_VPSS_GET_BGM            _IOWR(VPU_IOC_MAGIC,21,VPSS_PIC_DATA_S)
#define IOC_VPSS_GET_YCMEAN         _IOWR(VPU_IOC_MAGIC,22,VPSS_PIC_DATA_S)
#define IOC_VPSS_SEND_FRAME         _IOW(VPU_IOC_MAGIC,23,VIDEO_FRAME_INFO_S_VPU)
#define IOC_VPSS_GET_CHN_FRAME      _IOWR(VPU_IOC_MAGIC,24,VIDEO_FRAME_INFO_S_VPU)
#define IOC_VPSS_RELEASE_CHN_FRAME  _IOW(VPU_IOC_MAGIC,25,VIDEO_FRAME_INFO_S)
#define IOC_VPSS_SET_LOGO_FUNC      _IOW(VPU_IOC_MAGIC,26,VPSS_LOGO_INFO)
#define IOC_VPSS_GET_LOGO_FUNC      _IOWR(VPU_IOC_MAGIC,27,VPSS_LOGO_INFO)

#define IOC_VPSS_SET_DEPTH          _IOW(VPU_IOC_MAGIC,33,FY_U32)
#define IOC_VPSS_GET_DEPTH          _IOR(VPU_IOC_MAGIC,34,FY_U32)
#define IOC_VPSS_SET_GRP_FR         _IOW(VPU_IOC_MAGIC,35,VPSS_FRAME_RATE_S)
#define IOC_VPSS_GET_GRP_FR         _IOR(VPU_IOC_MAGIC,36,VPSS_FRAME_RATE_S)

//#define IOC_VPSS_SET_MODE_PARAM     _IOW(VPU_IOC_MAGIC,43,VPSS_MOD_PARAM_S)
//#define IOC_VPSS_GET_MODE_PARAM     _IOR(VPU_IOC_MAGIC,44,VPSS_MOD_PARAM_S)
#define IOC_VPSS_SET_YCNR_PARAM     _IOW(VPU_IOC_MAGIC,45,VPSS_YCNR_INFO_S)
#define IOC_VPSS_GET_YCNR_PARAM     _IOR(VPU_IOC_MAGIC,46,VPSS_YCNR_INFO_S)
#define IOC_VPSS_SET_APC_PARAM      _IOW(VPU_IOC_MAGIC,47,VPSS_APC_INFO_S)
#define IOC_VPSS_GET_APC_PARAM      _IOR(VPU_IOC_MAGIC,48,VPSS_APC_INFO_S)
#define IOC_VPSS_SET_PURPLE_PARAM   _IOW(VPU_IOC_MAGIC,49,VPSS_PURPLEFRI_INFO_S)
#define IOC_VPSS_GET_PURPLE_PARAM   _IOR(VPU_IOC_MAGIC,50,VPSS_PURPLEFRI_INFO_S)
#define IOC_VPSS_SET_CHROMA_PARAM   _IOW(VPU_IOC_MAGIC,51,VPSS_CHROMA_INFO_S)
#define IOC_VPSS_SET_LC_PARAM       _IOW(VPU_IOC_MAGIC,53,VPSS_LC_INFO_S)
#define IOC_VPSS_GET_LC_PARAM       _IOR(VPU_IOC_MAGIC,54,VPSS_LC_INFO_S)
#define IOC_VPSS_GET_HIST_PARAM     _IOWR(VPU_IOC_MAGIC,55,VPSS_HIST_STAT_S)
#define IOC_VPSS_SET_GAMMA_PARAM    _IOW(VPU_IOC_MAGIC,56,VPSS_GAMMA_S)
#define IOC_VPSS_SET_IE_PARAM                 _IOW(VPU_IOC_MAGIC,59,VPSS_IE_PARAM_S)
#define IOC_VPSS_GET_IE_PARAM                 _IOR(VPU_IOC_MAGIC,60,VPSS_IE_PARAM_S)
#define IOC_VPSS_GET_SPLITE_INFO              _IOWR(VPU_IOC_MAGIC,61,VPU_SPLITE_INFO_S)
#define IOC_VPSS_GET_PKG_INFO                 _IOWR(VPU_IOC_MAGIC,62,VPU_PKG_INFO_S)
#define IOC_VPSS_SET_GLOBLE_DISP_INIT_CFG     _IOW(VPU_IOC_MAGIC,63,VPSS_GLOBLE_DISPLAY_INIT_CFG)
#define IOC_VPSS_GET_GLOBLE_DISP_INIT_CFG     _IOWR(VPU_IOC_MAGIC,64,VPSS_GLOBLE_DISPLAY_INIT_CFG)
#define IOC_VPSS_SET_SCALER_COEFF             _IOW(VPU_IOC_MAGIC,65,FY_U32)


#define IOC_VPSS_BIND_GROUP_CHN   _IOW(VPU_IOC_MAGIC,190,VPU_CHN_ID)

#define IOC_VPSS_ADV_IOC                 FY_DRV_ADV_IOC(VPU_IOC_MAGIC)

#define IO_CMD_SET_MOD_PARAM        1
#define IO_CMD_GET_MOD_PARAM        2
#define IO_CMD_RELEASE_PIC_DATA     3
#define IO_CMD_SET_COLOR_OSD        4
#define IO_CMD_GET_COLOR_OSD        5
#define IO_CMD_GET_GRP_FRAME        6
#define IO_CMD_RELEASE_GRP_FRAME    7
#define IO_CMD_SET_PIC_EFFECT       8
#define IO_CMD_GET_PIC_EFFECT       9
#define IO_CMD_GET_SCALER_LEVEL     10
#define IO_CMD_SET_SCALER_LEVEL     11

/**
	following functions do not support yet
*/
//#define IOC_VPSS_QUERY_VBPOOL_SIZE  _IOWR(VPU_IOC_MAGIC,15,VPSS_VBPOOL_INFO_S)
//#define IOC_VPSS_DESTORY_VBPOOL	  _IOWR(VPU_IOC_MAGIC,17,FY_U32)
//#define IOC_VPSS_ATTACH_VBPOOL	  _IOWR(VPU_IOC_MAGIC,18,VPU_VBPOOL_ATTACH_S)
//#define IOC_VPSS_DETACH_VBPOOL	  _IOWR(VPU_IOC_MAGIC,19,VPU_VBPOOL_ATTACH_S)
//#define IOC_VPSS_RESET_GRP          _IOWR(VPU_IOC_MAGIC,28,VPSS_GRP)
//#define IOC_VPSS_SET_GRP_PARAM      _IOWR(VPU_IOC_MAGIC,29,VPU_GRP_PARAM_S)
//#define IOC_VPSS_GET_GRP_PARAM      _IOWR(VPU_IOC_MAGIC,30,VPU_GRP_PARAM_S)
//#define IOC_VPSS_SET_CHN_PARAM      _IOWR(VPU_IOC_MAGIC,31,VPU_CHN_EC_ADV_PARAM_S)
//#define IOC_VPSS_GET_CHN_PARAM      _IOWR(VPU_IOC_MAGIC,32,VPU_CHN_EC_ADV_PARAM_S)
//#define IOC_VPSS_SET_GRP_DELAY      _IOWR(VPU_IOC_MAGIC,37,VPSS_GRP)
//#define IOC_VPSS_GET_GRP_DELAY      _IOWR(VPU_IOC_MAGIC,38,VPSS_GRP)
//#define IOC_VPSS_SET_CHN_OVERLAY    _IOWR(VPU_IOC_MAGIC,39,TARET_CHN)
//#define IOC_VPSS_GET_CHN_OVERLAY    _IOWR(VPU_IOC_MAGIC,40,TARET_CHN)
//#define IOC_VPSS_ENABLE_USER_FR_CTRL _IOWR(VPU_IOC_MAGIC,41,TARET_CHN)
//#define IOC_VPSS_DISABLE_USER_FR_CTRL _IOWR(VPU_IOC_MAGIC,42,TARET_CHN)


typedef struct
{
	FY_U32 chan_id;
}VPU_CHN_ID;

typedef struct
{
    FY_U32  u32MaxW;  /*MAX width of the group*/
    FY_U32  u32MaxH;  /*MAX height of the group*/
    fyPIXEL_FORMAT_E enPixFmt; /*Pixel format*/
    VPSS_DIE_MODE_E enDieMode; /*De-interlace enable*/

    FY_BOOL bNrEn;
    FY_BOOL bApcEn;
    FY_BOOL bPurpleEn;
    FY_BOOL bYGammaEn;
    FY_BOOL bChromaEn;
    FY_BOOL bLcEn;
    FY_BOOL bHistEn;
    FY_BOOL bIeEn;
}VPU_GRP_ATTR_S;

typedef struct
{
    CROP_INFO_S cropInfo;
}VPU_CHN_ATTR_S;

typedef struct
{
    VPSS_CHN_MODE_E  enChnMode;   /*Vpss channel's work mode*/
    FY_U32 u32Width;              /*Width of target image*/
    FY_U32 u32Height;             /*Height of target image*/
    fyPIXEL_FORMAT_E  enPixelFormat;/*Pixel format of target image*/
    COMPRESS_MODE_E enCompressMode;   /*Compression mode of the output*/

    COMPRESS_RATIO_E CompRate;
	VPSS_CHN_GLOBLE_CFG glob_cfg;
    FY_U32  s32SrcFrmRate;        /* Input frame rate of a  group*/
    FY_U32  s32DstFrmRate;        /* Output frame rate of a channel group */
    union
    {
        MAIN_CHN_INFO_S mainCfg;
        AI_CHN_INFO_S aiCfg;
    };
}VPU_CHN_MODE_S;

typedef struct{
	// VO_EC_S voEc;
} VPU_CHN_EC_ADV_PARAM_S;

typedef struct{
	// VO_EC_S voEc;
}VPU_MOD_PARAM_S;

typedef struct fyVIDEO_FRAME_INFO_S_VPU
{
    VIDEO_FRAME_INFO_S st_VFrame;
    FY_S32 s32MilliSec;
} VIDEO_FRAME_INFO_S_VPU;

typedef struct fhVPSS_REGION_INFO_S_T
{
    RECT_S stRegion[VPSS_LUMA_STAT_NUM];
    FY_U32 RegionNum;
    FY_U32 LumaData[VPSS_LUMA_STAT_NUM];
    FY_U32 timeout;
} VPSS_REGION_INFO_S_T;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif //__FY_VPU_IOC__
