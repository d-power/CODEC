#ifndef __FYVO_H__
#define __FYVO_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "../../mpi_vo.h"
#include "../../mpi_vb.h"
#include "drv_ioc_extension.h"

typedef struct fhVOU_EXPORT_FUNC_S
{
    FY_S32 (*pfnVouChnFrameSmartGet)(VB_UID_E enMod, VO_LAYER voLayer, VO_CHN voChn, VIDEO_FRAME_INFO_S * pstFrame, FY_S32 s32MilliSec);
    FY_S32 (*pfnVouChnFrameGet)(VB_UID_E enMod, VO_LAYER voLayer, VO_CHN voChn, VIDEO_FRAME_INFO_S * pstFrame);
    FY_S32 (*pfnVouChnFramePut)(VB_UID_E enMod, VO_LAYER voLayer, VO_CHN voChn, VIDEO_FRAME_INFO_S * pstFrame);
    FY_S32 (*pfnVouGuiOnOff)(VO_DEV voDev, GRAPHIC_LAYER voLayer, FY_S32 bEnable);
    FY_S32 (*pfnVouGuiShowHide)(VO_DEV voDev, GRAPHIC_LAYER voLayer, FY_S32 show);
    FY_S32 (*pfnVouGuiSetPos)(VO_DEV voDev, GRAPHIC_LAYER voLayer, const POINT_S* pstPos);
    FY_S32 (*pfnVouGuiSetAlpha)(VO_DEV voDev, GRAPHIC_LAYER voLayer, const ALPHA_S *pstAlpha);
    FY_S32 (*pfnVouGuiUpdate)(VO_DEV voDev, GRAPHIC_LAYER voLayer, const VIDEO_FRAME_INFO_S *pstFrame, FY_S32 s32MilliSec);
    FY_S32 (*pfnVouGuiSetSize)(VO_DEV voDev, GRAPHIC_LAYER voLayer, const SIZE_S* pstSize);
} VOU_EXPORT_FUNC_S;

/*************************** VO Driver IO Command Definition ****************************/

#define IOC_TYPE_VOU                   'O'

#define VOIO_BIND_DEV_LAYER             _IOW(IOC_TYPE_VOU,     0,   FY_S32)
#define VOIO_ENABLE                     _IO(IOC_TYPE_VOU,      2)
#define VOIO_DISABLE                    _IO(IOC_TYPE_VOU,      3)
#define VOIO_SET_PUB_ATTR               _IOW(IOC_TYPE_VOU,     4,   fyVO_PUB_ATTR_S)
#define VOIO_GET_PUB_ATTR               _IOR(IOC_TYPE_VOU,     5,   fyVO_PUB_ATTR_S)

#define VOIO_SET_DEV_FRAME_RATE         _IOW(IOC_TYPE_VOU,     6,   FY_S32)
#define VOIO_GET_DEV_FRAME_RATE         _IOR(IOC_TYPE_VOU,     7,   FY_S32)

#define VOIO_SET_VTTH                   _IOW(IOC_TYPE_VOU,     8,   FY_S32)
#define VOIO_GET_VTTH                   _IOR(IOC_TYPE_VOU,     9,   FY_S32)

#define VOIO_ENALBE_RECV_FR_MATCH	    _IO(IOC_TYPE_VOU,      10)
#define VOIO_DISALBE_RECV_FR_MATCH	    _IO(IOC_TYPE_VOU,      11)

#define VOIO_SET_WBC_SOURCE	            _IOW(IOC_TYPE_VOU,     12,  VO_WBC_SOURCE_S)
#define VOIO_GET_WBC_SOURCE	            _IOR(IOC_TYPE_VOU,     13,  VO_WBC_SOURCE_S)
#define VOIO_ENABLE_WBC	                _IO(IOC_TYPE_VOU,      14)
#define VOIO_DISABLE_WBC	            _IO(IOC_TYPE_VOU,      15)
#define VOIO_SET_WBC_ATTR	            _IOW(IOC_TYPE_VOU,     16,  VO_WBC_ATTR_S)
#define VOIO_GET_WBC_ATTR	            _IOR(IOC_TYPE_VOU,     17,  VO_WBC_ATTR_S)
#define VOIO_SET_WBC_MODE	            _IOW(IOC_TYPE_VOU,     18,  VO_WBC_MODE_E)
#define VOIO_GET_WBC_MODE	            _IOR(IOC_TYPE_VOU,     19,  VO_WBC_MODE_E)
#define VOIO_SET_WBC_DEPTH	            _IOW(IOC_TYPE_VOU,     20,  FY_S32)
#define VOIO_GET_WBC_DEPTH	            _IOR(IOC_TYPE_VOU,     21,  FY_S32)
#define VOIO_GET_WBC_FRAME	            _IOWR(IOC_TYPE_VOU,    22,  VOIO_VIDEO_FRAME_INFO_S)
#define VOIO_RELEASE_WBC_FRAME	        _IOW(IOC_TYPE_VOU,     23,  VIDEO_FRAME_INFO_S)

#define VOIO_ENABLE_VIDEO_LAYER         _IO(IOC_TYPE_VOU,      24)
#define VOIO_DISABLE_VIDEO_LAYER        _IO(IOC_TYPE_VOU,      25)
#define VOIO_SET_VIDEO_LAYER_ATTR       _IOW(IOC_TYPE_VOU,     26,  VO_VIDEO_LAYER_ATTR_S)
#define VOIO_GET_VIDEO_LAYER_ATTR       _IOR(IOC_TYPE_VOU,     27,  VO_VIDEO_LAYER_ATTR_S)
#define VOIO_SET_COMPRESS_ATTR          _IOW(IOC_TYPE_VOU,     28,  VO_COMPRESS_ATTR_S)
#define VOIO_GET_COMPRESS_ATTR          _IOR(IOC_TYPE_VOU,     29,  VO_COMPRESS_ATTR_S)
#define VOIO_BIND_VIDEO_LAYER           _IO(IOC_TYPE_VOU,      30)
#define VOIO_UNBIND_VIDEO_LAYER         _IO(IOC_TYPE_VOU,      31)
#define VOIO_SET_PRIORITY	            _IOW(IOC_TYPE_VOU,     32,  FY_S32)
#define VOIO_GET_PRIORITY	            _IOR(IOC_TYPE_VOU,     33,  FY_S32)
#define VOIO_SET_CSC	                _IOW(IOC_TYPE_VOU,     34,  VO_CSC_S)
#define VOIO_GET_CSC	                _IOR(IOC_TYPE_VOU,     35,  VO_CSC_S)
#define VOIO_SET_DISP_BUFLEN	        _IOW(IOC_TYPE_VOU,     36,  FY_S32)
#define VOIO_GET_DISP_BUFLEN	        _IOR(IOC_TYPE_VOU,     37,  FY_S32)
#define VOIO_SET_PALY_TOLERATION	    _IOW(IOC_TYPE_VOU,     38,  FY_S32)
#define VOIO_GET_PALY_TOLERATION	    _IOR(IOC_TYPE_VOU,     39,  FY_S32)
#define VOIO_GET_SCREEN_FRAME	        _IOWR(IOC_TYPE_VOU,    40,  VOIO_VIDEO_FRAME_INFO_S)
#define VOIO_RELEASE_SCREEN_FRAME	    _IOW(IOC_TYPE_VOU,     41,  VIDEO_FRAME_INFO_S)
#define VOIO_SET_PART_MODE	            _IOW(IOC_TYPE_VOU,     42,  VO_PART_MODE_E)
#define VOIO_GET_PART_MODE	            _IOR(IOC_TYPE_VOU,     43,  VO_PART_MODE_E)
#define VOIO_SET_ATTR_BEGIN	            _IO(IOC_TYPE_VOU,      44)
#define VOIO_SET_ATTR_END	            _IO(IOC_TYPE_VOU,      45)

#define VOIO_ENABLE_CHN	                _IO(IOC_TYPE_VOU,      48)
#define VOIO_DISABLE_CHN	            _IO(IOC_TYPE_VOU,      49)
#define VOIO_SET_CHN_ATTR	            _IOW(IOC_TYPE_VOU,     50,  VO_CHN_ATTR_S)
#define VOIO_GET_CHN_ATTR	            _IOR(IOC_TYPE_VOU,     51,  VO_CHN_ATTR_S)
#define VOIO_SET_CHN_PARAM	            _IOW(IOC_TYPE_VOU,     52,  VO_CHN_PARAM_S)
#define VOIO_GET_CHN_PARAM	            _IOR(IOC_TYPE_VOU,     53,  VO_CHN_PARAM_S)
#define VOIO_SET_CHN_DISP_POS	        _IOW(IOC_TYPE_VOU,     54,  POINT_S)
#define VOIO_GET_CHN_DISP_POS	        _IOR(IOC_TYPE_VOU,     55,  POINT_S)
#define VOIO_SET_CHN_FIELD	            _IOW(IOC_TYPE_VOU,     56,  VO_DISPLAY_FIELD_E)
#define VOIO_GET_CHN_FIELD	            _IOR(IOC_TYPE_VOU,     57,  VO_DISPLAY_FIELD_E)
#define VOIO_SEND_FRAME	                _IOW(IOC_TYPE_VOU,     58,  VOIO_VIDEO_FRAME_INFO_S)
#define VOIO_GET_CHN_FRAME	            _IOWR(IOC_TYPE_VOU,    59,  VOIO_VIDEO_FRAME_INFO_S)
#define VOIO_RELEASE_CHN_FRAME	        _IOW(IOC_TYPE_VOU,     60,  VIDEO_FRAME_INFO_S)
#define VOIO_GET_CHN_FRAME_RATE	        _IOR(IOC_TYPE_VOU,     61,  FY_S32)
#define VOIO_SET_CHN_FRAME_RATE	        _IOW(IOC_TYPE_VOU,     62,  FY_S32)
#define VOIO_PAUSE_CHN	                _IO(IOC_TYPE_VOU,      63)
#define VOIO_RESUME_CHN	                _IO(IOC_TYPE_VOU,      64)
#define VOIO_STEP_CHN	                _IO(IOC_TYPE_VOU,      65)
#define VOIO_REFRESH_CHN            	_IO(IOC_TYPE_VOU,      66)
#define VOIO_SET_ZOOMIN_WINDOW	        _IOW(IOC_TYPE_VOU,     67,  VO_ZOOM_ATTR_S)
#define VOIO_GET_ZOOMIN_WINDOW	        _IOR(IOC_TYPE_VOU,     68,  VO_ZOOM_ATTR_S)
#define VOIO_SHOW_CHN	                _IO(IOC_TYPE_VOU,      69)
#define VOIO_HIDE_CHN	                _IO(IOC_TYPE_VOU,      70)
#define VOIO_GET_CHN_PTS	            _IOR(IOC_TYPE_VOU,     71,  FY_U64)
#define VOIO_QUERY_CHN_STAT	            _IOR(IOC_TYPE_VOU,     72,  VO_QUERY_STATUS_S)
#define VOIO_CLEAR_CHN_BUFFER	        _IOW(IOC_TYPE_VOU,     73,  FY_BOOL)
#define VOIO_SET_CHN_BORDER	            _IOW(IOC_TYPE_VOU,     74,  VO_BORDER_S)
#define VOIO_GET_CHN_BORDER	            _IOR(IOC_TYPE_VOU,     75,  VO_BORDER_S)


#define VOIO_SET_CHN_RECEIVE_THRESHOLD	_IOW(IOC_TYPE_VOU,     80,  FY_S32)
#define VOIO_GET_CHN_RECEIVE_THRESHOLD	_IOR(IOC_TYPE_VOU,     81,  FY_S32)
#define VOIO_GET_CHN_RG_LUMA	        _IOWR(IOC_TYPE_VOU,    82,  VOIO_REGION_INFO_S)
#define VOIO_SET_CASCADE_ATTR	        _IOW(IOC_TYPE_VOU,     83,  VO_CAS_ATTR_S)
#define VOIO_GET_CASCADE_ATTR	        _IOR(IOC_TYPE_VOU,     84,  VO_CAS_ATTR_S)
#define VOIO_ENALBE_CASCADE_DEV	        _IO(IOC_TYPE_VOU,      85)
#define VOIO_DISALBE_CASCADE_DEV	    _IO(IOC_TYPE_VOU,      86)
#define VOIO_SET_CASCADE_PATTERN	    _IOW(IOC_TYPE_VOU,     87,  FY_S32)
#define VOIO_GET_CASCADE_PATTERN	    _IOR(IOC_TYPE_VOU,     88,  FY_S32)
#define VOIO_CASCADE_POS_BIND_CHN	    _IOW(IOC_TYPE_VOU,     89,  FY_S32)
#define VOIO_CASCADE_POS_UNBIND_CHN	    _IOW(IOC_TYPE_VOU,     90,  FY_S32)
#define VOIO_ENABLE_CASCADE	            _IO(IOC_TYPE_VOU,      91)
#define VOIO_DISABLE_CASCADE	        _IO(IOC_TYPE_VOU,      92)
#define VOIO_BIND_GRAPHIC_LAYER	        _IOW(IOC_TYPE_VOU,     93,  FY_S32)
#define VOIO_UNBIND_GRAPHIC_LAYER	    _IOW(IOC_TYPE_VOU,     94,  FY_S32)
#define VOIO_SET_GRAPHIC_LAYER_CSC	    _IOW(IOC_TYPE_VOU,     95,  VO_CSC_S)
#define VOIO_GET_GRAPHIC_LAYER_CSC	    _IOR(IOC_TYPE_VOU,     96,  VO_CSC_S)
#define VOIO_SET_VGA_PARAM	            _IOW(IOC_TYPE_VOU,     97,  VO_VGA_PARAM_S)
#define VOIO_GET_VGA_PARAM	            _IOR(IOC_TYPE_VOU,     98,  VO_VGA_PARAM_S)
#define VOIO_SET_HDMI_PARAM	            _IOW(IOC_TYPE_VOU,     99,  VO_HDMI_PARAM_S)
#define VOIO_GET_HDMI_PARAM	            _IOR(IOC_TYPE_VOU,     100, VO_HDMI_PARAM_S)
#define VOIO_QUERY_DEV_INTF_STATUS	    _IOWR(IOC_TYPE_VOU,    101, VOIO_DEVINTF_STATUS_S)

#define VOIO_SET_CHN_CM	                _IOR(IOC_TYPE_VOU,     102, VO_CM_S)
#define VOIO_GET_CHN_CM	                _IOW(IOC_TYPE_VOU,     103, VO_CM_S)

#define VOIO_GFX_ENABLE                 _IO(IOC_TYPE_VOU,      104)
#define VOIO_GFX_DISABLE	            _IO(IOC_TYPE_VOU,      105)
#define VOIO_GFX_SET_POS                _IOW(IOC_TYPE_VOU,     106, POINT_S)
#define VOIO_GFX_SET_ALPHA	            _IOW(IOC_TYPE_VOU,     107, ALPHA_S)
#define VOIO_GFX_SHOW                   _IO(IOC_TYPE_VOU,      108)
#define VOIO_GFX_HIDE	                _IO(IOC_TYPE_VOU,      109)
#define VOIO_GFX_UPDATE                 _IOW(IOC_TYPE_VOU,     110, VOIO_VIDEO_FRAME_INFO_S)
#define VOIO_GFX_SET_SIZE               _IOW(IOC_TYPE_VOU,     111, SIZE_S)
#define VOIO_SET_HDMI_AUDIO             _IOW(IOC_TYPE_VOU,     112, VO_HDMI_AUDIO_S)
#define VOIO_GET_HDMI_AUDIO             _IOR(IOC_TYPE_VOU,     113, VO_HDMI_AUDIO_S)

#define VOIO_GET_GFX_FRAME	            _IOWR(IOC_TYPE_VOU,    114,  VOIO_VIDEO_FRAME_INFO_S)
#define VOIO_PUT_GFX_FRAME	            _IOW(IOC_TYPE_VOU,     115,  VIDEO_FRAME_INFO_S)

#define VOIO_RESET_HW_ONCE	            _IO(IOC_TYPE_VOU,      116)
#define VOIO_RESET_HW	                _IO(IOC_TYPE_VOU,      117)

#define VOIO_FILL_CHN	                _IOW(IOC_TYPE_VOU,     118,  FY_U32)

#define VOIO_GET_AUTO_BUFFER            _IOWR(IOC_TYPE_VOU,    119,  VOIO_VIDEO_FRAME_INFO_S)
#define VOIO_PUT_AUTO_BUFFER            _IOW(IOC_TYPE_VOU,     120,  VIDEO_FRAME_INFO_S)

#define VOIO_TEST_MODE                  _IOWR(IOC_TYPE_VOU,    121,  VO_TEST_PARAM_S)

#define VOIO_ADV_IOC                    FY_DRV_ADV_IOC(IOC_TYPE_VOU)


/****************************************************************************************/


typedef struct fyVOCMD_VIDEO_FRAME_INFO_S
{
    VIDEO_FRAME_INFO_S stFrame;
    FY_S32 s32MilliSec;
} VOIO_VIDEO_FRAME_INFO_S;


typedef struct fyVOCMD_REGION_INFO_S
{
	FY_S32 s32MilliSec;
    FY_U32 u32LumaData[VPSS_LUMA_STAT_NUM];
    FY_U32 u32RegionNum;       						/*count of the region*/
	RECT_S stRegion[VPSS_LUMA_STAT_NUM];         	/*region attribute*/
} VOIO_REGION_INFO_S;

typedef struct fyVOCMD_DEVINTF_STATUS_S
{
    VO_INTF_TYPE_E      enDevInter;
    VO_DEVINTF_STATUS_S stStatus;
} VOIO_DEVINTF_STATUS_S;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __FYVO_H__ */

