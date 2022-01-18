#ifndef __JPEG_DRV_IO_H__
#define __JPEG_DRV_IO_H__

#include "../fy_comm_vdec.h"
#include "../fy_comm_jpege.h"

typedef struct
{
  FY_U32 u32PhyAddr;
  FY_U32 u32VirtAddr;
  FY_U32 u32BufSize;
  FY_U32 rsv;
}JPEGE_STRBUF_INFO_S;

typedef struct {
	FY_U32 Luma_Addr;//y data, phyaddr
	FY_U32 Chroma_Addr;//uv data, phyaddr
	FY_U32 JPG_Addr;//jpg data, phyaddr
	FY_U32 JPG_BufSize;
	FY_U16 Img_W;
	FY_U16 Img_H;
	FY_U16 ROI_X;
	FY_U16 ROI_Y;
	FY_U16 ROI_W;
	FY_U16 ROI_H;
	FY_U16 Stride;//Input Image Address Stride.
	FY_U8 Format;
	FY_U8 EncEn;
	FY_U8 DcmpEn;//TT_ecdc enable, encoder only.
	/*ByteMode: TT_ecdc compression ratio of a tile64x4, encoder only.
	00: 192byte 50%; 01: 224byte 60%; 02: 256byte 66%.
	In TT_ecdc disable, must be 00.*/
	FY_U8 EcsEn;
	FY_U16 NRST;//NRST = NMCU between two reset markers - 1
	FY_U32 Qfactor;//1--99
	FY_U8 ByteMode;
	/*ScanMode: 00: Block store; 10: CbCr; 11: CrCb.
	In decoder & TT_ecdc & block store, must be 00.*/
	FY_U8 ScanMode;
	/*RotateCfg: 00: 0; 01: 90; 10: 180; 11: 270.
	In decoder & TT_ecdc & block store, must be 00.*/
	FY_U8 RotateCfg;
	FY_U8 LumaQTbl[8][8];
	FY_U8 ChromaQTbl[8][8];
	FY_U32 timeout;//ms
	FY_U32 s32EncodeSize;//driver return encsize
}JPEGE_DMODE_PARA_S;

#define JPEGE_IOC_MAGIC      'J'
#define IOC_JPEGE_CREATE_CHAN                    _IOW(JPEGE_IOC_MAGIC,  0,  JPEGE_CHN_ATTR_S)
#define IOC_JPEGE_DESTROY_CHAN                   _IOW(JPEGE_IOC_MAGIC,  1,  FY_U32)
#define IOC_JPEGE_RESET_CHAN                     _IO(JPEGE_IOC_MAGIC,   2)
#define IOC_JPEGE_START_RECIEVE_PIC              _IOW(JPEGE_IOC_MAGIC,  3,  FY_U32)
#define IOC_JPEGE_START_RECIEVE_PIC_EX           _IOW(JPEGE_IOC_MAGIC,  4,  JPEGE_RECV_PIC_PARAM_S)
#define IOC_JPEGE_STOP_RECIEVE_PIC               _IOW(JPEGE_IOC_MAGIC,  5,  FY_U32)
#define IOC_JPEGE_QUERY                          _IOR(JPEGE_IOC_MAGIC,  6,  JPEGE_CHN_STAT_S)
#define IOC_JPEGE_SET_CHAN_ATTR                  _IOW(JPEGE_IOC_MAGIC,  7,  JPEGE_CHN_ATTR_S)
#define IOC_JPEGE_GET_CHAN_ATTR                  _IOWR(JPEGE_IOC_MAGIC, 8,  JPEGE_CHN_ATTR_S)
#define IOC_JPEGE_GET_STREAM                     _IOWR(JPEGE_IOC_MAGIC, 9,  JPEGE_STREAM_S)
#define IOC_JPEGE_RELEASE_STREAM                 _IOW(JPEGE_IOC_MAGIC,  10, JPEGE_STREAM_S)
#define IOC_JPEGE_SEND_FRAME                     _IOW(JPEGE_IOC_MAGIC,  11, VIDEO_FRAME_INFO_S)
#define IOC_JPEGE_SET_CHAN_PRIORITY              _IOWR(JPEGE_IOC_MAGIC, 12, FY_U32)
#define IOC_JPEGE_GET_CHAN_PRIORITY              _IOWR(JPEGE_IOC_MAGIC, 13, FY_U32)
#define IOC_JPEGE_SET_MAX_STREAM_CNT             _IOWR(JPEGE_IOC_MAGIC, 14, FY_U32)
#define IOC_JPEGE_GET_MAX_STREAM_CNT             _IOWR(JPEGE_IOC_MAGIC, 15, FY_U32)
#define IOC_JPEGE_GET_STREAM_BUF_INFO            _IOWR(JPEGE_IOC_MAGIC, 16, JPEGE_STRBUF_INFO_S)
#define IOC_JPEGE_SET_CHAN_PARAM                 _IOWR(JPEGE_IOC_MAGIC, 17, JPEGE_PARAM_S)
#define IOC_JPEGE_GET_CHAN_PARAM                 _IOWR(JPEGE_IOC_MAGIC, 18, JPEGE_PARAM_S)
#define IOC_JPEGE_SET_ADV_PARAM                  _IOWR(JPEGE_IOC_MAGIC, 19, FY_U32)
#define IOC_JPEGE_GET_ADV_PARAM                  _IOWR(JPEGE_IOC_MAGIC, 20, FY_U32)

#define IOC_JPEGE_BIND_CHAN                      _IOW(JPEGE_IOC_MAGIC,  20, FY_U32)
#define IOC_JPEGE_UNBIND_CHAN                    _IO(JPEGE_IOC_MAGIC,   21)

#define IOC_JPEGE_SET_ROTATE                     _IOW(JPEGE_IOC_MAGIC,  23, ROTATE_E)
#define IOC_JPEGE_GET_ROTATE                     _IOR(JPEGE_IOC_MAGIC,  24, ROTATE_E)
#define IOC_JPEGE_DIRECT_MODE                     _IOR(JPEGE_IOC_MAGIC,  25, JPEGE_DMODE_PARA_S)


#define IOC_TYPE_JPEGD 'j'

typedef struct iocsendstream_t
{
    fyVDEC_STREAM_S stream;
    FY_S32 s32MilliSec;
}JPEGD_IOC_SENDSTREAM_S;

typedef struct iocgetimage_t
{
    VIDEO_FRAME_INFO_S fameInfo;
    FY_S32 s32MilliSec;
}JPEGD_IOC_GETIMAGE_S;


typedef struct iocgetUsrData_t
{
    VDEC_USERDATA_S usrData;
    FY_S32 s32MilliSec;
}JPEGD_IOC_GETUSERDATA_S;

#define IOC_JPEGD_BIND_CHAN             _IOW(IOC_TYPE_JPEGD,10,VDEC_CHN)
#define IOC_JPEGD_UNBIND_CHAN           _IO(IOC_TYPE_JPEGD,1)
#define IOC_JPEGD_CREATE_CHAN           _IOW(IOC_TYPE_JPEGD,0,fyVDEC_CHN_ATTR_S)
#define IOC_JPEGD_DESTROY_CHAN          _IO(IOC_TYPE_JPEGD,2)
#define IOC_JPEGD_GET_CHAN_ATTR         _IOR(IOC_TYPE_JPEGD,3,fyVDEC_CHN_ATTR_S)
#define IOC_JPEGD_START_RECV_STREAM     _IO(IOC_TYPE_JPEGD,7)
#define IOC_JPEGD_STOP_RECV_STREAM      _IO(IOC_TYPE_JPEGD,8)
#define IOC_JPEGD_QUERY                 _IOR(IOC_TYPE_JPEGD,6,fyVDEC_CHN_STAT_S)
#define IOC_JPEGD_RESET_CHAN            _IO(IOC_TYPE_JPEGD,9)
#define IOC_JPEGD_SET_CHAN_PARAM        _IOW(IOC_TYPE_JPEGD,13,VDEC_CHN_PARAM_S)
#define IOC_JPEGD_GET_CHAN_PARAM        _IOR(IOC_TYPE_JPEGD,14,VDEC_CHN_PARAM_S)
#define IOC_JPEGD_SET_PROTOCOL_PARAM    _IOW(IOC_TYPE_JPEGD,15,VDEC_PRTCL_PARAM_S)
#define IOC_JPEGD_GET_PROTOCOL_PARAM    _IOR(IOC_TYPE_JPEGD,16,VDEC_PRTCL_PARAM_S)
#define IOC_JPEGD_SEND_STREAM           _IOW(IOC_TYPE_JPEGD,4,JPEGD_IOC_SENDSTREAM_S)
#define IOC_JPEGD_GET_IMAGE             _IOWR(IOC_TYPE_JPEGD,20,JPEGD_IOC_GETIMAGE_S)
#define IOC_JPEGD_RELEASE_IMAGE         _IOW(IOC_TYPE_JPEGD,21, VIDEO_FRAME_INFO_S)
#define IOC_JPEGD_GET_USER_DATA         _IOWR(IOC_TYPE_JPEGD,17,JPEGD_IOC_GETUSERDATA_S)
#define IOC_JPEGD_RELEASE_USER_DATA     _IOW(IOC_TYPE_JPEGD,19,VDEC_USERDATA_S)
#define IOC_JPEGD_SET_ROTATE            _IOW(IOC_TYPE_JPEGD,29,ROTATE_E)
#define IOC_JPEGD_GET_ROTATE            _IOR(IOC_TYPE_JPEGD,30,ROTATE_E)
#define IOC_JPEGD_GET_CHAN_LUMA         _IOR(IOC_TYPE_JPEGD,31,VDEC_CHN_LUM_S)
#define IOC_JPEGD_SET_USER_PIC          _IOW(IOC_TYPE_JPEGD,26,VIDEO_FRAME_INFO_S)
#define IOC_JPEGD_ENABLE_USER_PIC       _IOW(IOC_TYPE_JPEGD,27, FY_BOOL)
#define IOC_JPEGD_DISABLE_USER_PIC      _IO(IOC_TYPE_JPEGD,28)
#define IOC_JPEGD_SET_DISPLAY_MODE      _IOW(IOC_TYPE_JPEGD,32,VIDEO_DISPLAY_MODE_E)
#define IOC_JPEGD_GET_DISPLAY_MODE      _IOR(IOC_TYPE_JPEGD,33,VIDEO_DISPLAY_MODE_E)
#define IOC_JPEGD_SET_CHAN_VBCNT        _IOW(IOC_TYPE_JPEGD,34,FY_U32)
#define IOC_JPEGD_GET_CHAN_VBCNT        _IOR(IOC_TYPE_JPEGD,35,FY_U32)
#define IOC_JPEGD_ATTACH_VBPOOL         _IOW(IOC_TYPE_JPEGD,36,VDEC_CHN_POOL_S)
#define IOC_JPEGD_DETACH_VBPOOL         _IO(IOC_TYPE_JPEGD,37)
#define IOC_JPEGD_SET_MOD_PARAM         _IOW(IOC_TYPE_JPEGD,38,VDEC_MOD_PARAM_S)
#define IOC_JPEGD_GET_MOD_PARAM         _IOR(IOC_TYPE_JPEGD,39,VDEC_MOD_PARAM_S)
#define IOC_JPEGD_SET_LOG_PARAM         _IOW(IOC_TYPE_JPEGD,40,FY_U32)

#endif //__JPEG_DRV_IO_H__