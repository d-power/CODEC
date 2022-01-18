#ifndef __TDE_DRV_IOC_H__
#define __TDE_DRV_IOC_H__

#include "../fy_comm_tde.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/*Define attributes of tde bitblit*/
typedef struct fyTDS_BIT_BLIT_S
{
	TDE_HANDLE s32Handle;
	TDE2_SURFACE_S stBackGround;
	TDE2_RECT_S stBackGroundRect;
	TDE2_SURFACE_S stForeGround;
	TDE2_RECT_S stForeGroundRect;
	TDE2_SURFACE_S stDst;
	TDE2_RECT_S stDstRect;
	TDE2_OPT_S stOpt;
	int NULL_Flag;
}TDS_BIT_BLIT_S;

/*Define attributes of tde solid*/
typedef struct fyTDS_SOLID_S
{
	TDE_HANDLE s32Handle;
	TDE2_SURFACE_S stForeGround;
	TDE2_RECT_S stForeGroundRect;
	TDE2_SURFACE_S stDst;
	TDE2_RECT_S stDstRect;
	TDE2_FILLCOLOR_S stFillColor;
	TDE2_OPT_S stOpt;
	int NULL_Flag;
}TDS_SOLID_S;


/*Define attributes of tde quick copy*/
typedef struct fyTDS_QUICK_COPY_S
{
	TDE_HANDLE s32Handle;
	TDE2_SURFACE_S stSrc;
	TDE2_RECT_S stSrcRect;
	TDE2_SURFACE_S stDst;
	TDE2_RECT_S stDstRect;
}TDS_QUICK_COPY_S;

/*Define attributes of tde quick fill*/
typedef struct fyTDS_QUICK_FILL_S
{
	TDE_HANDLE s32Handle;
	TDE2_SURFACE_S stDst;
	TDE2_RECT_S stDstRect;
	FY_U32 u32FillData;
}TDS_QUICK_FILL_S;

/*Define attributes of tde mb blit*/
typedef struct fyTDS_MB_BLIT_S
{
	TDE_HANDLE s32Handle;
	TDE2_MB_S stMB;
	TDE2_RECT_S stMbRect;
	TDE2_SURFACE_S stDst;
	TDE2_RECT_S stDstRect;
	TDE2_MBOPT_S stMbOpt;
}TDS_MB_BLIT_S;

/*Define attributes of tde end job*/
typedef struct fyTDS_END_JOB_S
{
	TDE_HANDLE s32Handle;
	FY_BOOL bSync;
	FY_BOOL bBlock;
	FY_U32 u32TimeOut;
}TDS_END_JOB_S;

/*Define attributes of tde bmp rop*/
typedef struct fyTDS_BMP_ROP_S
{
	TDE_HANDLE s32Handle;
	TDE2_SURFACE_S stBackGround;
	TDE2_RECT_S stBackGroundRect;
	TDE2_SURFACE_S stForeGround;
	TDE2_RECT_S stForeGroundRect;
	TDE2_SURFACE_S stMask;
	TDE2_RECT_S stMaskRect;
	TDE2_SURFACE_S stDst;
	TDE2_RECT_S stDstRect;
	TDE2_ROP_CODE_E enRopCode_Color;
	TDE2_ROP_CODE_E enRopCode_Alpha;
}TDS_BMP_ROP_S;

/*Define attributes of tde bmp blend*/
typedef struct fyTDS_BMP_BLEND_S
{
	TDE_HANDLE s32Handle;
	TDE2_SURFACE_S stBackGround;
	TDE2_RECT_S stBackGroundRect;
	TDE2_SURFACE_S stForeGround;
	TDE2_RECT_S stForeGroundRect;
	TDE2_SURFACE_S stMask;
	TDE2_RECT_S stMaskRect;
	TDE2_SURFACE_S stDst;
	TDE2_RECT_S stDstRect;
	FY_U8 u8Alpha;
	TDE2_ALUCMD_E enBlendMode;
}TDS_BMP_BLEND_S;


/*Define attributes of tde pattern fill*/
typedef struct fyTDE_PATTERN_FILL_S
{
	TDE_HANDLE s32Handle;
	TDE2_SURFACE_S pstBackGround;
	TDE2_RECT_S pstBackGroundRect;
	TDE2_SURFACE_S pstForeGround;
	TDE2_RECT_S pstForeGroundRect;
	TDE2_SURFACE_S pstDst;
	TDE2_RECT_S pstDstRect;
	TDE2_PATTERN_FILL_OPT_S pstOpt;
	int NULL_Flag;
}TDE_PATTERN_FILL_S;


/******************************************************************************
    TDE driver ioctl definition list
 *****************************************************************************/

#define IOC_TYPE_TDE   't'
#define IOC_TDE_BEGIN_JOB                   _IOR(IOC_TYPE_TDE,1,int)
#define IOC_TDE_BIT_BLIT                    _IOW(IOC_TYPE_TDE,2,TDS_BIT_BLIT_S)
#define IOC_TDE_SOLID_DRAW                  _IOW(IOC_TYPE_TDE,3,TDS_SOLID_S)
#define IOC_TDE_QUICK_COPY                  _IOW(IOC_TYPE_TDE,4,TDS_QUICK_COPY_S)
#define IOC_TDE_QUICK_RESIZE                _IOW(IOC_TYPE_TDE,5,TDS_QUICK_COPY_S)
#define IOC_TDE_QUICK_FILL                  _IOW(IOC_TYPE_TDE,6,TDS_QUICK_FILL_S)
#define IOC_TDE_QUICK_DEFLICKER             _IOW(IOC_TYPE_TDE,7,TDS_QUICK_COPY_S)
#define IOC_TDE_MB_BLIT                     _IOW(IOC_TYPE_TDE,8,TDS_MB_BLIT_S)
#define IOC_TDE_END_JOB                     _IOW(IOC_TYPE_TDE,9,TDS_END_JOB_S)
#define IOC_TDE_WAIT_FOR_DONE               _IOW(IOC_TYPE_TDE,10,FY_S32)
#define IOC_TDE_CANCEL_JOB                  _IOW(IOC_TYPE_TDE,11,FY_S32)
#define IOC_TDE_BMP_MASK_ROP                _IOW(IOC_TYPE_TDE,12,TDS_BMP_ROP_S)
#define IOC_TDE_BMP_MASK_BLEND              _IOW(IOC_TYPE_TDE,13,TDS_BMP_BLEND_S)
#define IOC_TDE_WAIT_ALL_DONE               _IO(IOC_TYPE_TDE,14)
#define IOC_TDE_RESET                       _IO(IOC_TYPE_TDE,15)
#define IOC_TDE_SET_DEFLICKER_LEVEL         _IOW(IOC_TYPE_TDE,17,TDE_DEFLICKER_LEVEL_E)
#define IOC_TDE_GET_DEFLICKER_LEVEL         _IOR(IOC_TYPE_TDE,18,TDE_DEFLICKER_LEVEL_E)
#define IOC_TDE_SET_ALPHA_THRESHOLD_VALUE   _IOW(IOC_TYPE_TDE,19,FY_U8)
#define IOC_TDE_GET_ALPHA_THRESHOLD_VALUE   _IOR(IOC_TYPE_TDE,20,FY_U8)
#define IOC_TDE_SET_ALPHA_THRESHOLD_STATE   _IOW(IOC_TYPE_TDE,21,FY_BOOL)
#define IOC_TDE_GET_ALPHA_THRESHOLD_STATE   _IOW(IOC_TYPE_TDE,22,FY_BOOL)
#define IOC_TDE_PATTERN_FILL                _IOW(IOC_TYPE_TDE,23,TDE_PATTERN_FILL_S)
#define IOC_TDE_ENABLE_REGION_DEFLICKER     _IOW(IOC_TYPE_TDE,24,FY_BOOL)
#define IOC_TDE_DO_COMPRESS                 _IOW(IOC_TYPE_TDE,25,TDS_QUICK_COPY_S)
#define IOC_TDE_DE_COMPRESS                 _IOW(IOC_TYPE_TDE,26,TDS_QUICK_COPY_S)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __TDE_DRV_IOC_H__ */


