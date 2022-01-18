#ifndef __FY_COMM_VPSS_H__
#define __FY_COMM_VPSS_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "fy_type.h"
#include "fy_common.h"
#include "fy_errno.h"
#include "fy_comm_video.h"

typedef enum fyEN_VPU_ERR_CODE_E
{

    EN_ERR_VPU_GRP_HAS_BINDED      = 0x40,
    EN_ERR_VPU_GRP_NOT_BINDED      = 0x41,

    ERR_VPU_NOT_ENABLE             = 0x45,
    ERR_VPU_NOT_DISABLE            = 0x46,
    ERR_VPU_NOT_CONFIG             = 0x47,

    ERR_VPU_CHN_NOT_DISABLE        = 0x48,
    ERR_VPU_CHN_NOT_ENABLE         = 0x49,
    ERR_VPU_CHN_NOT_CONFIG         = 0x4a,
    ERR_VPU_CHN_GET_TIMEOUT        = 0x4b,

    ERR_VPU_GRP_INVALID_ID         = 0x55,
    ERR_VPU_GRP_NOT_CREATE         = 0x56,

    ERR_VPU_GRP_NOT_DESTROY        = 0x58,
    ERR_VPU_GRP_CHN_FULL           = 0x59,
    ERR_VPU_GRP_WAIT_FRAME_TIMEOUT = 0x5a,
    ERR_VPU_GRP_CHN_NOT_EMPTY      = 0x5b,
    ERR_VPU_DST_NOT_BIND           = 0x5c,
    ERR_VPU_GRP_INVALID_BASE_PTS   = 0x5d,
    ERR_VPU_GRP_NOT_START          = 0x5e,
    ERR_VPU_GRP_NOT_STOP           = 0x5f,
    ERR_VPU_GRP_INVALID_FRMRATE    = 0x60,
    ERR_VPU_GLOBLE_NOT_ENABLE      = 0x61,
    ERR_VPU_SET_SCALER_LEVEL_ERROR = 0x62,

    /* new added */
    ERR_VPU_BUTT

}EN_VPU_ERR_CODE_E;
	

#define FY_ERR_VPSS_NULL_PTR        FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define FY_ERR_VPSS_NOTREADY        FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define FY_ERR_VPSS_INVALID_DEVID   FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
#define FY_ERR_VPSS_INVALID_CHNID   FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
#define FY_ERR_VPSS_EXIST           FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
#define FY_ERR_VPSS_UNEXIST         FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
#define FY_ERR_VPSS_NOT_SUPPORT     FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define FY_ERR_VPSS_NOT_PERM        FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define FY_ERR_VPSS_NOMEM           FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define FY_ERR_VPSS_NOBUF           FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
#define FY_ERR_VPSS_ILLEGAL_PARAM   FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define FY_ERR_VPSS_BUSY            FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define FY_ERR_VPSS_BUF_EMPTY       FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)

#define FY_ERR_VPU_HAS_BINDED		FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_VPU_GRP_HAS_BINDED)
#define FY_ERR_VPU_NOT_BINDED		FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_VPU_GRP_NOT_BINDED)
#define FY_ERR_VPU_CHN_NOT_ENABLE	FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, ERR_VPU_CHN_NOT_ENABLE)
#define FY_ERR_VPU_CHN_GET_TIMEOUT	FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, ERR_VPU_CHN_GET_TIMEOUT)
#define FY_ERR_VPU_GRP_NOT_START	FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, ERR_VPU_GRP_NOT_START)
#define FY_ERR_VPU_GRP_NOT_STOP	    FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, ERR_VPU_GRP_NOT_STOP)
#define FY_ERR_VPU_GLOBLE_NOT_ENABLE    FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, ERR_VPU_GLOBLE_NOT_ENABLE)
#define FY_ERR_VPU_SET_SCALER_LEVEL_ERROR    FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, ERR_VPU_SET_SCALER_LEVEL_ERROR)
#define FY_ERR_VPU_GRP_NOT_START    FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, ERR_VPU_GRP_NOT_START)
#define FY_ERR_VPU_GRP_NOT_DESTORY  FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, ERR_VPU_GRP_NOT_DESTROY)
#define FY_ERR_VPU_WAIT_FRAME_TIMEOUT   FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, ERR_VPU_GRP_WAIT_FRAME_TIMEOUT)
#define FY_ERR_VPU_DISABLE_CHN_TIMEOUT  FY_DEF_ERR(FY_ID_VPSS, EN_ERR_LEVEL_ERROR, ERR_VPU_CHN_NOT_DISABLE)


//FH add 

#define VPSS_MAX_MASK_AREA             (8)
#define VPSS_MAX_LOGO_AREA             (8)
#define GAIN_NODES                    (12)
#define VPSS_MAX_GOSD_COLOR_IDX        (256)
#define SLICE_HEAD_SIZE   64


//FH add end

typedef   FY_S32 VPSS_GRP;  /* [0, 128] */
typedef   FY_S32 VPSS_CHN;  /* [0, 4] */

/*Define 4 video frame*/
typedef enum fyVPSS_FRAME_WORK_E
{
    VPSS_FRAME_WORK_LEFT     =  0,
    VPSS_FRAME_WORK_RIGHT    =  1,
    VPSS_FRAME_WORK_BOTTOM   =  2,
    VPSS_FRAME_WORK_TOP      =  3,
    VPSS_FRAME_WORK_BUTT
}VPSS_FRAME_WORK_E;

/*Define de-interlace mode*/    
typedef enum  fyVPSS_DIE_MODE_E
{
    VPSS_DIE_MODE_NODIE     = 0,
    VPSS_DIE_MODE_AUTO      = 1,
    VPSS_DIE_MODE_DIE       = 2,
    VPSS_DIE_MODE_BUTT
}VPSS_DIE_MODE_E;

/*Define attributes of vpss channel*/
typedef struct fyVPSS_CHN_ATTR_S
{
    FY_BOOL bSpEn;    /*Sharpen enable*/   //Not support
    FY_BOOL bUVInvert;  /* UV Invert enable*/  //Not support
    FY_BOOL bBorderEn; /*Frame enable*/ //Not support
    BORDER_S  stBorder;  //Not support

    //fh add
    CROP_INFO_S cropInfo;
}VPSS_CHN_ATTR_S;

typedef struct fyVPSS_GRP_PARAM_S
{
    FY_U32 u32Contrast;          /*strength of dymanic contrast improve*/
    FY_U32 u32DieStrength;       /*strength of de-interlace,not used now*/
    FY_U32 u32IeStrength;        /*strength of image enhance*/
    FY_U32 u32SfStrength;        /*strength of y space filter*/
    FY_U32 u32TfStrength;        /*strength of y time filter*/
    FY_U32 u32CfStrength;        /*strength of c space filter*/
    FY_U32 u32CTfStrength;       /*strength of c time filter*/
    FY_U32 u32CvbsStrength;      /*strength of cvbs*/
    FY_U32 u32DeMotionBlurring;  /*strength of de motion blurring,not used now*/
}VPSS_GRP_PARAM_S;

/*Define detailed NR params for grp image process*/
typedef struct fyVPSS_NR_ADVANCED_PARAM_S
{
    FY_U32 u32Mdz;
    FY_U32 u32HTfRelaStrength;   /*relation strength of hard NR chn */

    /*only for weak nr chn*/
    FY_U32 u32Edz;
    FY_U32 u32WTfRelaStrength;   /*relation strength of weak NR chn */
}VPSS_NR_ADVANCED_PARAM_S;


/* Define image feild select mode */
typedef enum fyVPSS_CAPSEL_E
{
    VPSS_CAPSEL_BOTH = 0,             /* top and bottom field */
    VPSS_CAPSEL_TOP,                  /* top field */
    VPSS_CAPSEL_BOTTOM,               /* bottom field */
    
    VPSS_CAPSEL_BUTT
} VPSS_CAPSEL_E;

/*Define coordinate mode*/
typedef enum fyVPSS_CROP_COORDINATE_E   
{
    VPSS_CROP_RATIO_COOR = 0,   /*Ratio coordinate*/
    VPSS_CROP_ABS_COOR          /*Absolute coordinate*/
}VPSS_CROP_COORDINATE_E;

/*Define attributes of CLIP function*/
typedef struct fyVPSS_CROP_INFO_S
{
    FY_BOOL bEnable;        /*CROP enable*/
    VPSS_CROP_COORDINATE_E  enCropCoordinate;   /*Coordinate mode of the crop start point*/ 
    RECT_S  stCropRect;     /*CROP rectangular*/   
}VPSS_CROP_INFO_S;

typedef struct{
	FY_U32 y2rGain;
	FY_U32 cb2rGain;
	FY_U32 cr2rGain;

	FY_U32 y2gGain;
	FY_U32 cb2gGain;
	FY_U32 cr2gGain;

	FY_U32 y2bGain;
	FY_U32 cb2bGain;
	FY_U32 cr2bGain;

	FY_U32 compensateR;
	FY_U32 compensateG;
	FY_U32 compensateB;
} AI_YCbCr2RGB_S;

//data format ensure
typedef struct{
	FY_U32 rOffset;
	FY_U32 gOffset;
	FY_U32 bOffset;
}AI_RGBCLAMP_S;


typedef enum
{
    DS_NONE          = 1,
    DS_ONE_FOURTH    = 4,
    DS_ONE_EIGHTH    = 8,
    DS_ONE_SIXTEENTH = 16
}DS_MODE_E;

typedef struct{
	FY_U32 bYcMeanEn;
	DS_MODE_E ycmeanMode;
	FY_U32 bBgmYEn;
	FY_U32 bCpyEn;
} MAIN_CHN_INFO_S;


typedef struct{
	FY_U32 AiOutSel;
	FY_U32 RGBOutMode;
	AI_YCbCr2RGB_S yCbCr2RgbCfg;
	AI_RGBCLAMP_S rgbClampCfg;
} AI_CHN_INFO_S;

/*
1D compress rate:50%(128b),63%(160b),75%(192b),(x/(128*2)==x/(256)), only support yuv422
TILE compress rate:50%(192b),59%(224b), 67%(256b, (x/(64*4*1.5)==x/(384)) only support yuv420
slice compress rate:40%~70%, one step :5% only support yuv422
*/
typedef enum
{
    COMPRESS_RATIO_NONE   = 100, /**No compress*/
    COMPRESS_RATIO_75_PER = 75,  /**compress rate=75%*/
    COMPRESS_RATIO_70_PER = 70,
    COMPRESS_RATIO_67_PER = 67,
    COMPRESS_RATIO_65_PER = 65,
    COMPRESS_RATIO_63_PER = 63,
    COMPRESS_RATIO_60_PER = 60,
    COMPRESS_RATIO_59_PER = 59,
    COMPRESS_RATIO_55_PER = 55,
    COMPRESS_RATIO_50_PER = 50,
    COMPRESS_RATIO_45_PER = 45,
    COMPRESS_RATIO_40_PER = 40,
    COMPRESS_RATIO_35_PER = 35,
    COMPRESS_RATIO_30_PER = 30
}COMPRESS_RATIO_E;

typedef struct fyVPSS_GRP_ATTR_S
{
    /*statistic attributes*/
    FY_U32  u32MaxW;  /*MAX width of the group*/
    FY_U32  u32MaxH;  /*MAX height of the group*/
    fyPIXEL_FORMAT_E enPixFmt; /*Pixel format*/
    
    FY_BOOL bIeEn;    /*Image enhance enable*/
    FY_BOOL bDciEn;   /*Dynamic contrast Improve enable*/
    FY_BOOL bNrEn;    /*Noise reduce enable*/ 
    FY_BOOL bHistEn;  /*Hist enable*/
    FY_BOOL bEsEn;      /*Edge smooth enable*/
    VPSS_DIE_MODE_E enDieMode; /*De-interlace enable*/

    //VI_OPS viOps;
    FY_BOOL bApcEn;
    FY_BOOL bPurpleEn;
    FY_BOOL bYGammaEn;
    FY_BOOL bChromaEn;
    FY_BOOL bLcEn;
}VPSS_GRP_ATTR_S;

/*Define vpss frame control info*/
typedef struct fyVPSS_FRAME_RATE_S
{
    FY_S32  s32SrcFrmRate;        /* Input frame rate of a  group*/
    FY_S32  s32DstFrmRate;        /* Output frame rate of a channel group */
} VPSS_FRAME_RATE_S;

/*Define vpss channel's work mode*/
typedef enum fyVPSS_CHN_MODE_E   
{
    VPSS_CHN_MODE_AUTO = 0, /*Auto mode*/
    VPSS_CHN_MODE_USER  /*User mode*/
}VPSS_CHN_MODE_E;

typedef struct
{
	FY_U32 bUseGloble;
	FY_S32 glob_idx;
	POINT_S pic_pos;
}VPSS_CHN_GLOBLE_CFG;


/*Define attributes of vpss channel's work mode*/
typedef struct fyVPSS_CHN_MODE_S
{
    VPSS_CHN_MODE_E  enChnMode;   /*Vpss channel's work mode*/
    FY_U32 u32Width;              /*Width of target image*/
    FY_U32 u32Height;             /*Height of target image*/
    FY_BOOL bDouble;              /*Field-frame transfer, only valid for VPSS_CHN2*/
    VPSS_FRAME_RATE_S stFrameRate;
    ASPECT_RATIO_S stAspectRatio;
    fyPIXEL_FORMAT_E  enPixelFormat;/*Pixel format of target image*/
    COMPRESS_MODE_E enCompressMode;   /*Compression mode of the output*/
    
    COMPRESS_RATIO_E CompRate; //FY add
    VPSS_CHN_GLOBLE_CFG GlobCfg;
    union
    {
        MAIN_CHN_INFO_S mainCfg;
        AI_CHN_INFO_S aiCfg;
    };
}VPSS_CHN_MODE_S;


/*Define detailed params for channel image process*/
typedef struct fyVPSS_CHN_PARAM_S
{
    FY_U32 u32SpStrength;
    FY_U32 u32SfStrength;
    FY_U32 u32TfStrength;
    FY_U32 u32CfStrength;
    FY_U32 u32DeMotionBlurring;  
} VPSS_CHN_PARAM_S;

/*Define vpss prescale info*/
typedef struct fyVPSS_PRESCALE_INFO_S
{
    FY_BOOL bPreScale;       /*prescale enable*/
    SIZE_S  stDestSize;      /*destination size*/
}VPSS_PRESCALE_INFO_S;

/*Define vpss filter info*/
typedef struct fyVPSS_SIZER_INFO_S
{
    FY_BOOL bSizer;
    SIZE_S  stSize;
}VPSS_SIZER_INFO_S;


/*Define attributes of vpss extend channel*/
typedef struct fyVPSS_EXT_CHN_ATTR_S
{
    VPSS_CHN        s32BindChn;             /*channel bind to*/
    FY_U32          u32Width;               /*Width of target image*/
    FY_U32          u32Height;              /*Height of target image*/
    FY_S32          s32SrcFrameRate;        /*Frame rate of source*/
    FY_S32          s32DstFrameRate;        /*Frame rate of extend chn input&output*/
    fyPIXEL_FORMAT_E  enPixelFormat;          /*Pixel format of target image*/
}VPSS_EXT_CHN_ATTR_S;

typedef struct fyVPSS_REGION_INFO_S
{
    RECT_S *pstRegion;    /*region attribute*/
    FY_U32 u32RegionNum;       /*count of the region*/
}VPSS_REGION_INFO_S;

typedef enum fyVPSS_PRESCALE_MODE_E
{
    VPSS_PRESCALE_MODE_DEFAULT = 0,  /*use vpss to prescale*/
    VPSS_PRESCALE_MODE_OTHER,        /*use vgs to prescale*/
    VPSS_PRESCALE_MODE_BUTT
}VPSS_PRESCALE_MODE_E;

typedef struct fyVPSS_MOD_PARAM_S
{
    FY_U32  u32VpssVbSource;
    FY_U32  u32MaxWidth;
    FY_U32  u32MaxHeight;
    FY_U32  u32BgmNum;
    FY_U32  u32CpyNum;
    FY_U32  u32YMeanNum;
} VPSS_MOD_PARAM_S;

typedef struct fyVPSS_LOGO_COLOR
{
	FY_U32  logo_idx;
	FY_U32  idx_offset;//
	FY_U32  color[VPSS_MAX_GOSD_COLOR_IDX/2];
}VPSS_LOGO_COLOR_INFO;

typedef struct fyVPU_LOGO_CFG
{
	fyPIXEL_FORMAT_E logoPiexlFmt;
	unsigned char *logo_addr;  /* phyaddress */
	POINT_S         logo_startpos;
	POINT_S         logo_cutpos; 
	FY_U32 stride; 
	FY_U32 rgbmode;/*0:stdio RGB,1:computer RGB*/
	FY_U32 bGlobalAlphaEn;
	FY_U32 globalAlpha;
	FY_U32 bMagicColorEn;/**< magic color enable */
	FY_U32 magicColor;/**< format is ARGB0565,magic color is transparent */
	SIZE_S logo_size;
}VPSS_LOGO_CFG;

typedef struct
{
	FY_U32 logo_idx;
	FY_U32 bLogoEn; 
	VPSS_LOGO_CFG logo_cfg;
}VPSS_LOGO_INFO;

typedef struct
{
	FY_U32  masaic_mode;
	FY_U32  color;
	FY_U32  masaic_size;
}MASK_CFG;

typedef struct
{
	FY_U32 mask_enable[VPSS_MAX_MASK_AREA];
	RECT_S         area_value[VPSS_MAX_MASK_AREA];
	MASK_CFG  masaic;
}VPSS_MASK_INFO_S;

/*NR3D,APC,PURPLE,YGAMMA,CHROMA,LOCALCHROMA*/

typedef struct fyVPSS_YCNR_INFO_S
{
	FY_U32 bRefEcdcEn;
	COMPRESS_RATIO_E CompRate;

	FY_U32 BypassNr2d;
	FY_U32 YSel0;
	FY_U32 YSel1;
	FY_U32 UpSel;
	FY_U32 yMode[3];
	FY_U32 uMode[3];
	FY_U32 vMode[3];
	FY_U32 ylScaler;
	FY_U32 ydScaler;
	FY_U32 ulScaler;
	FY_U32 ulTh;
	FY_U32 vlTh;
	FY_U32 H;
	FY_U32 V;
	FY_U32 D;
	FY_U32 curTh2;
	FY_U32 curTh3;
	FY_U32 preTh2;
	FY_U32 preTh3;
	FY_U32 curDifTh2;
	FY_U32 curDifTh3;
	FY_U32 preDifTh2;
	FY_U32 preDifTh3;
	FY_U32 ldLTh;
	FY_U32 ldDTh;
	FY_U32 roundScaler;
	FY_U32 Rate_iir;
	FY_U32 th_iir;
	FY_U32 pScaler_dif;
	FY_U32 Th1;
	FY_U32 Th2;
	FY_U32 Offset;
	FY_U32 difWeight;
	FY_U32 lut_difRem[17];
	FY_U32 difMax;
	FY_U32 StrengthD;
	FY_U32 yth_shrink[3];
	FY_U32 uth_shrink[3];
	FY_U32 vth_shrink[3];
	FY_U32 rate_y;
	FY_U32 th_y;
	FY_U32 IntCoefDWeight;
	FY_U32 ExtCoefDWeight;
	FY_U32 X_y_sw[2];
	FY_U32 Y_y_sw[2];
	FY_U32 slope_y_sw[2];
	FY_U32 X_y_tw[4];
	FY_U32 Y_y_tw[4];
	FY_U32 slope_y_tw[4];
	FY_U32 sw_shift;
	FY_U32 tw_shift;
	FY_U32 uv_offset;
	FY_U32 VLScaler;
	FY_U32 X_uv_sw[2];
	FY_U32 Y_uv_sw[2];
	FY_U32 slope_uv_sw[2];
	FY_U32 X_uv_tw[4];
	FY_U32 Y_uv_tw[4];
	FY_U32 slope_uv_tw[4];

}VPSS_YCNR_INFO_S;

typedef struct fyVPSS_APC_INFO_S
{
	FY_U32 pgain;   //apc de pgain 0~255
	FY_U32 ngain;   //apc de ngain 0~255
	FY_U32 edgeLv;  //apc es pgain 0~255
	FY_U32 detailLv;//apc es ngain 0~255
} VPSS_APC_INFO_S;


typedef struct fyVPSS_PURPLEFRI_INFO_S
{
	FY_U32 bSatCorrEn;
	FY_U32 bMedCorrEn;
	FY_U32 ShiftBits;
	FY_U32 WinYSkipMode;
	FY_U32 RollSin;
	FY_U32 RollCos;
	FY_U32 TanTheta;
	FY_U32 TanHfTheta;
	FY_U32 TanOneThirdTheta;
	FY_U32 TanQuarterTheta;
	FY_U32 TanOneFifthTheta;
	FY_U32 TanOneSixthTheta;
	FY_U32 YTh;
	FY_U32 ExpoTh;
	FY_U32 YGapth;
	FY_U32 ULth;
	FY_U32 UHth;
	FY_U32 SatThL;
	FY_U32 SatThH;

} VPSS_PURPLEFRI_INFO_S;

typedef struct fyVPSS_CHROMA_INFO_S{
    FY_U32 satParaR1;
} VPSS_CHROMA_INFO_S;



typedef struct{

	FY_U32 EllipseCoefD;
	FY_U32 EllipseCoefE;
	FY_U32 EllipseCoefF;
	FY_U32 FlagA;
	FY_U32 FlagB;
	FY_U32 FlagC;
	FY_U32 TanB;
	FY_U32 TanC;

}LC_CFG;

typedef struct fyVPSS_LC_INFO_S
{
	LC_CFG lc_cfg[3];
} VPSS_LC_INFO_S;

typedef struct{
	int milliSec;
	FY_U32 histBin[33][2];
}VPSS_HIST_STAT_S;


typedef struct{
	FY_U32 IeCtr;
	FY_U32 IeBrt;
	FY_U32 IeHuSin;
	FY_U32 IeHuCos;
	FY_U32 IeSat;
}VPSS_IE_PARAM_S;


typedef struct fyVPSS_CONTRAST_CFG_S
{
    FY_U32 bYcEn;
    FY_U32 bGainMappingEn;
    FY_U32 crt;
    FY_U32 mid;
    FY_U32 crtMap[GAIN_NODES];
} VPSS_CONTRAST_CFG_S;

typedef struct fyVPSS_BRIGHTNESS_CFG_S
{
    FY_U32 bYcEn;
    FY_U32 bGainMappingEn;
    FY_U32 brt;
    FY_U32 brtMap[GAIN_NODES];
} VPSS_BRIGHTNESS_CFG_S;

typedef struct fyVPSS_SAT_CFG_S
{
    FY_U32 bGainMappingEn;
    FY_U32 sat;
    FY_U32 blueSurp;
    FY_U32 redSurp;
    FY_U32 satMap[GAIN_NODES];
} VPSS_SAT_CFG_S;

typedef struct fyVPSS_GAMMA_S
{
	FY_U32 YGamma[80];
} VPSS_GAMMA_S;


typedef struct
{
	FY_S32 glob_idx;
	FY_BOOL bEnable;
	fyPIXEL_FORMAT_E glb_pixfmt;
	FY_U32 frame_rate;
	SIZE_S tal_size;
}VPSS_GLOBLE_DISPLAY_INIT_CFG;


//JUST For test get ycmean
typedef struct fyVPSS_PIC_DATA_S
{
    FY_S32      milliSec;
    FY_U32      u32PoolId;
    FY_U32      u32Width;
    FY_U32      u32Height;
    FY_U32      u32PhyAddr;
    FY_VOID     *pVirAddr;
    FY_U64      u64pts;
    FY_U32      u32TimeRef;
}VPSS_PIC_DATA_S;

typedef struct fyVPU_GRP_EFFECT_S
{
    FY_U32 u32Contrast;          /*strength of dymanic contrast improve*/
    FY_U32 u32Brightness;       /*strength of de-interlace,not used now*/
    FY_U32 u32IeStrength;        /*strength of image enhance*/
    FY_U32 u32Saturation;
}VPU_GRP_EFFECT_S;

typedef enum
{
	DIRECTION_H=0,
	DIRECTION_V,
}PIC_DIRECTION;

typedef struct
{
	PIC_DIRECTION scaler_dir;
	unsigned int level;
}VPU_SCALER_COEFF;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* __FY_COMM_VPSS_H__ */


