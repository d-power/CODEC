#ifndef __FY_COMM_AIO_H__
#define __FY_COMM_AIO_H__

#include "fy_common.h"
#include "fy_errno.h"
#include <linux/types.h>
#include <linux/wait.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define AI_SEP_POINT_NUM 640 //8k/25*2
#define POINT_NUM_PER_FRM 6

//max number of one device
#define MAX_AUDIO_CHN_NUM	16//16
#define TOTAL_CHN_NUM		20
/*max length of audio frame by bytes, one frame contain many sample point */
#define MAX_AUDIO_FRAME_LEN    AI_SEP_POINT_NUM*2//0x1000 
/*max length of audio stream by bytes */
#define MAX_AUDIO_STREAM_LEN   MAX_AUDIO_FRAME_LEN

#define MAX_AUDIO_FILE_PATH_LEN	256
#define MAX_AUDIO_FILE_NAME_LEN	256

#define MAX_VOLUME_CNT  12800

//driver macro
#define MAX_CACHE_BUF_NODE	3 

//user cache frame size default
#define AIO_FRM_SIZE MAX_AUDIO_FRAME_LEN//100//0x4000

//ai default record buffer number and init cache frame number
#define AI_CACHE_FRM_NUM 7
#define AI_DEFAULT_BUFFER_NUM 2

//ao default play buffer number and init cache frame number
#define AO_CACHE_FRM_NUMBER 5
#define AO_DEFAULT_BUFFER_NUM 2

#define USE_ENQUENE_BUFFER FY_TRUE

/*The VQE EQ Band num.*/
#define VQE_EQ_BAND_NUM  10

#define AIO_GET_INTVAL_TIME(sec,usec)	(sec*1000+usec/1000)

enum{
	I2S_DEV0 = 0,  //audio device i2s-dev0
	I2S_DEV1,	//audio device i2s-dev1
	I2S_DEV2,	//audio device i2s-dev2
	I2S_DEV3,	//audio device i2s-dev3
	I2S_DEV_TOTAL
};

enum{
	SELECT_TYPE_AI,
	SELECT_TYPE_AO
};

typedef enum fyAUDIO_SAMPLE_RATE_E 
{ 
    AUDIO_SAMPLE_RATE_8000   = 8000,    /* 8K samplerate*/
    AUDIO_SAMPLE_RATE_12000  = 12000,   /* 12K samplerate(not used)*/    
    AUDIO_SAMPLE_RATE_11025  = 11025,   /* 11.025K samplerate(not used)*/
    AUDIO_SAMPLE_RATE_16000  = 16000,   /* 16K samplerate*/
    AUDIO_SAMPLE_RATE_22050  = 22050,   /* 22.050K samplerate(not used)*/
    AUDIO_SAMPLE_RATE_24000  = 24000,   /* 24K samplerate(not used)*/
    AUDIO_SAMPLE_RATE_32000  = 32000,   /* 32K samplerate*/
    AUDIO_SAMPLE_RATE_44100  = 44100,   /* 44.1K samplerate(not used)*/
    AUDIO_SAMPLE_RATE_48000  = 48000,   /* 48K samplerate(not used)*/
    AUDIO_SAMPLE_RATE_BUTT,
} AUDIO_SAMPLE_RATE_E; 

typedef enum fyAUDIO_BUS_PARAM_E{
	AUDIO_BUS_8WIDTH   =  0x8,
	AUDIO_BUS_16WIDTH  =  0x10,
	AUDIO_BUS_32WIDTH  =  0x20,
	AUDIO_BUS_64WIDTH  =  0x40,
	AUDIO_BUS_128WIDTH =  0x80,
}AUDIO_BUS_PARAM_E;


typedef enum fyAUDIO_BIT_WIDTH_E
{
    AUDIO_BIT_WIDTH_8   = 8,   /* 8bit width */
    AUDIO_BIT_WIDTH_16  = 16,   /* 16bit width*/
    AUDIO_BIT_WIDTH_32  = 32,   /* 32bit width*/
    AUDIO_BIT_WIDTH_BUTT,
} AUDIO_BIT_WIDTH_E;

typedef enum fyAUDIO_CHN_NUM_E{
	AUDIO_CHN_NUM_1 = 0x1,
	AUDIO_CHN_NUM_2 = 0x2,
	AUDIO_CHN_NUM_4 = 0x4,
	AUDIO_CHN_NUM_8 = 0x8,
	AUDIO_CHN_NUM_16 = 0x10,
	AUDIO_CHN_NUM_20 = 0x14
}AUDIO_CHN_NUM_E;


typedef enum fyAIO_MODE_E
{
    AIO_MODE_I2S_MASTER  = 0,   /* AIO I2S master mode */
    AIO_MODE_I2S_SLAVE,         /* AIO I2S slave mode */
    AIO_MODE_PCM_SLAVE_STD,     /* AIO PCM slave standard mode (not used)*/
    AIO_MODE_PCM_SLAVE_NSTD,    /* AIO PCM slave non-standard mode (not used)*/
    AIO_MODE_PCM_MASTER_STD,    /* AIO PCM master standard mode (not used)*/
    AIO_MODE_PCM_MASTER_NSTD,   /* AIO PCM master non-standard mode (not used)*/
    AIO_MODE_BUTT    
} AIO_MODE_E;

typedef enum fyAIO_SOUND_MODE_E
{
    AUDIO_SOUND_MODE_MONO   =0,/*mono*/
    AUDIO_SOUND_MODE_STEREO =1,/*stereo*/
    AUDIO_SOUND_MODE_BUTT    
} AUDIO_SOUND_MODE_E;

/*
An example of the packing scheme for G726-32 codewords is as shown, and bit A3 is the least significant bit of the first codeword: 
RTP G726-32:
0                   1
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
|B B B B|A A A A|D D D D|C C C C| ...
|0 1 2 3|0 1 2 3|0 1 2 3|0 1 2 3|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

MEDIA G726-32:
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
|A A A A|B B B B|C C C C|D D D D| ...
|3 2 1 0|3 2 1 0|3 2 1 0|3 2 1 0|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
*/
typedef enum fyG726_BPS_E
{
    G726_16K = 0,       /* G726 16kbps, see RFC3551.txt  4.5.4 G726-16 */ 
    G726_24K,           /* G726 24kbps, see RFC3551.txt  4.5.4 G726-24 */
    G726_32K,           /* G726 32kbps, see RFC3551.txt  4.5.4 G726-32 */
    G726_40K,           /* G726 40kbps, see RFC3551.txt  4.5.4 G726-40 */
    MEDIA_G726_16K,     /* G726 16kbps for ASF ... */ 
    MEDIA_G726_24K,     /* G726 24kbps for ASF ... */
    MEDIA_G726_32K,     /* G726 32kbps for ASF ... */
    MEDIA_G726_40K,     /* G726 40kbps for ASF ... */
    G726_BUTT
} G726_BPS_E;

typedef enum fyADPCM_TYPE_E
{
    /* see DVI4 diiffers in three respects from the IMA ADPCM at RFC3551.txt 4.5.1 DVI4 */
    
    ADPCM_TYPE_DVI4 = 0,    /* 32kbps ADPCM(DVI4) for RTP */
    ADPCM_TYPE_IMA,         /* 32kbps ADPCM(IMA),NOTICE:point num must be 161/241/321/481 */
    ADPCM_TYPE_ORG_DVI4,
    ADPCM_TYPE_BUTT
} ADPCM_TYPE_E;

typedef enum fyAUDIO_TYPE_PARAM_E{
	I2S_CHN_CMD = 0,
	I2S_CLK_DIV
}AUDIO_TYPE_PARAM_E;

typedef enum fyAUDIO_RTX_MODE_E{
	I2S_RTX_IDLE = 0,
	I2S_RXF_MODE,
	I2S_TXF_MODE,
	I2S_RTX_MODE
}AUDIO_RTX_MODE_E;

typedef enum fyAUDIO_DATA_MODE_E{
	I2S_MIX_DATA_FORMAT = 0,
	I2S_CHN_DATA_FORMAT
}AUDIO_DATA_MODE_E;

typedef enum fyAUDIO_WORK_MODE{
	DEV_SUSPEND = 0,
	DEV_RESUME
}AUDIO_WORK_E;

typedef enum fyAUDIO_TIME_STATE{
	STAMP_IDLE_STAT = 0,    //idle, can be use
	STAMP_READY_STAT,		//ready to use
	STAMP_INVALID_STAT    //invalid state
}AUDIO_TIME_STATE;

typedef struct fyAUDIO_MIX_INFO{
	FY_U32 frame_size;
	FY_U32 usr_addr;
}AUDIO_MIX_INFO;

typedef struct fyAUDIO_FRAME_INFO{
	FY_U8 chn_id;
	FY_U32 frame_size;
	FY_U32 usr_addr;
}AUDIO_FRAME_INFO;

typedef struct _fyFile_info{
	FY_BOOL isNeedSave;
	FY_CHAR fileName[64];
}fyFile_info;

typedef struct fyAUDIO_FRAME_E{
	FY_U32 frame_cnt;
	FY_U32 frame_size;
	fyFile_info fileInfo;
}AUDIO_FRAME_E;

typedef struct _MEM_CACHE_Info
{
	FY_U32 u32Size;
	FY_U32 u32PhyAddr;
	FY_VOID* pVirAddr;
}MEM_CACHE_Info;

typedef struct fyAIO_ATTR_S{
	AUDIO_SAMPLE_RATE_E fs_rate;  //8K:8000  16K:16000 32K:32000 48K:48000
	AUDIO_CHN_NUM_E chn_num;   // 2,4,8,16,20 channel
	AUDIO_BIT_WIDTH_E chn_width; // 8/16/32 bits
	AIO_MODE_E is_slave;   //0: Master  1: Slave
	AUDIO_TYPE_PARAM_E set_param; // 0: mix channnel bits 1: sck, only for master
	AUDIO_RTX_MODE_E i2s_rxt_mode;  //1:rx 2:tx 
	AUDIO_DATA_MODE_E i2s_data_format;//0:mix  1:channel
	FY_U32 u32DmaSize;
}AIO_ATTR_S;

typedef struct fyAUDIO_PARAM_S{
	FY_U32 param_size;
	union{
		FY_U32 fs_rate;  //8K:8000  16K:16000 32K:32000 48K:48000
		FY_U32 dma_link_size; //config dma link buffer size
		FY_U32 volume_level;   //set volume cnt
		FY_U8 chn_num;   // 2,4,8,16,20 channel
		FY_U8 chn_width; // 8/16/32 bits
		FY_U8 is_slave;   //0: Master  1: Slave
		FY_U8 set_param; // 0: mix channnel bits 1: sck, only for master
		FY_U8 cur_chn; // channel to playback
		FY_U8 i2s_start; // 0: stop  1: start
		FY_U8 i2s_rxt_mode;  //1:rx 2:tx 
		FY_U8 i2s_data_format; //0:mix  1:channel
	}param;
}AUDIO_PARAM_S;


enum{
	AO_BUF_IDLE = 0,    //idle, can be use
	AO_BUF_FULL,		//buffer full, ready to use
	AO_BUF_BUSY,       //user are copying the data from buf
	AO_BUF_INVALID,    //invalid state
};
	

typedef struct _AO_PLAY_PARAM{
	FY_U16 param_size;
	FY_U8 node;
	FY_U8 state;
}fyAO_PLAY_PARAM;


typedef struct fyAI_CHN_PARAM_S
{
    FY_U32 u32UsrFrmDepth;
} AI_CHN_PARAM_S;

typedef struct fyAUDIO_FRAME_S
{ 
    AUDIO_BIT_WIDTH_E   enBitwidth;     /*audio frame bitwidth*/
    AUDIO_SOUND_MODE_E  enSoundmode;    /*audio frame momo or stereo mode*/
	FY_U32  u32PhyAddr; 
    FY_VOID *pVirAddr;
    FY_U64  u64TimeStamp;                /*audio frame timestamp*/
    FY_U32  u32Seq;                      /*audio frame seq*/
    FY_U32  u32Len;                      /*data lenth per channel in frame*/
} AUDIO_FRAME_S; 

typedef struct fyAEC_FRAME_S
{
    AUDIO_FRAME_S   stRefFrame;    /* AEC reference audio frame */
    FY_BOOL         bValid;        /* whether frame is valid */
	FY_BOOL         bSysBind;        /* whether is sysbind */
} AEC_FRAME_S;

typedef struct fyAUDIO_FRAME_INFO_S
{
    AUDIO_FRAME_S *pstFrame;/*frame ptr*/
    FY_U32         u32Id;   /*frame id*/
} AUDIO_FRAME_INFO_S;

typedef struct fyAUDIO_STREAM_S 
{ 
    FY_U8 *pStream;         /* the virtual address of stream */ 
    FY_U32 u32PhyAddr;      /* the physics address of stream */
    FY_U32 u32Len;          /* stream lenth, by bytes */
    FY_U64 u64TimeStamp;    /* frame time stamp*/
    FY_U32 u32Seq;          /* frame seq,if stream is not a valid frame,u32Seq is 0*/
} AUDIO_STREAM_S;


typedef struct fyAO_CHN_STATE_S
{
    FY_U32                  u32ChnTotalNum;    /* total number of channel buffer */
    FY_U32                  u32ChnFreeNum;     /* free number of channel buffer */
    FY_U32                  u32ChnBusyNum;     /* busy number of channel buffer */
} AO_CHN_STATE_S;


typedef enum fyAUDIO_TRACK_MODE_E
{
    AUDIO_TRACK_NORMAL      = 0,
    AUDIO_TRACK_BOTH_LEFT   = 1,
    AUDIO_TRACK_BOTH_RIGHT  = 2,
    AUDIO_TRACK_EXCHANGE    = 3,
    AUDIO_TRACK_MIX         = 4,
    AUDIO_TRACK_LEFT_MUTE   = 5,
    AUDIO_TRACK_RIGHT_MUTE  = 6,
    AUDIO_TRACK_BOTH_MUTE   = 7,

    AUDIO_TRACK_BUTT
} AUDIO_TRACK_MODE_E;

typedef enum fyAUDIO_FADE_RATE_E
{
    AUDIO_FADE_RATE_1	= 0,
    AUDIO_FADE_RATE_2	= 1,
    AUDIO_FADE_RATE_4	= 2,
    AUDIO_FADE_RATE_8   = 3,
    AUDIO_FADE_RATE_16  = 4,
    AUDIO_FADE_RATE_32  = 5,
    AUDIO_FADE_RATE_64  = 6,
    AUDIO_FADE_RATE_128 = 7,
    
    AUDIO_FADE_RATE_BUTT
} AUDIO_FADE_RATE_E; 

typedef struct fyAUDIO_FADE_S
{
    FY_BOOL         bFade; 
    AUDIO_FADE_RATE_E enFadeInRate; 
    AUDIO_FADE_RATE_E enFadeOutRate;
} AUDIO_FADE_S;

/**Defines the configure parameters of AGC.*/
typedef struct fyAUDIO_AGC_CONFIG_S
{
    FY_BOOL bUsrMode;          /* mode 0: auto,mode 1: manual.*/

    FY_S8 s8TargetLevel;       /* target voltage level, range: [-40, -1]dB */
    FY_S8 s8NoiseFloor;        /* noise floor, range: [-65, -20]dB */
    FY_S8 s8MaxGain;           /* max gain, range: [0, 30]dB */
    FY_S8 s8AdjustSpeed;       /* adjustable speed, range: [0, 10]dB/s */

    FY_S8 s8ImproveSNR;        /* switch for improving SNR, range: [0:close, 1:upper limit 3dB, 2:upper limit 6dB] */
    FY_S8 s8UseHighPassFilt;   /* switch for using fygh pass filt, range: [0:close, 1:80Hz, 2:120Hz, 3:150:Hz, 4:300Hz: 5:500Hz] */
    FY_S8 s8OutputMode;        /* output mode, mute when lower than noise floor, range: [0:close, 1:open] */
    FY_S16 s16NoiseSupSwitch;  /* switch for noise suppression, range: [0:close, 1:open] */
    

    FY_S32 s32Reserved;
} AUDIO_AGC_CONFIG_S;

/**Defines the configure parameters of AEC.*/
typedef struct fyAI_AEC_CONFIG_S
{
    FY_BOOL bUsrMode;                             /* mode 0: auto mode 1: mannual.*/
    FY_S8 s8CngMode;                              /* cozy noisy mode: 0 close, 1 open, recommend 1*/ 
    FY_S8 s8NearAllPassEnergy;                    /* the far-end energy threshold for judging whether unvarnished transmission: 0 -59dBm0, 1 -49dBm0, 2 -39dBm0, recommend 1 */
    FY_S8 s8NearCleanSupEnergy;                   /* the energy threshold for compelling reset of near-end signal: 0 12dB, 1 15dB, 2 18dB, recommend 2 */  
                                                                
    FY_S16 s16DTHnlSortQTh;                       /* the threshold of judging single or double talk, recommend 16384, [0, 32767] */
 
    FY_S16 s16EchoBandLow;                       /* voice processing band1, low frequency parameter, [1, 63) for 8k, [1, 127) for 16k, recommend 10 */
    FY_S16 s16EchoBandHigh;                      /* voice processing band1, fygh frequency parameter, (s16EchoBandLow, 63] for 8k, (s16EchoBandLow, 127] for 16k, recommend 41 */
                                                   /* s16EchoBandHigh must be greater than s16EchoBandLow */
    FY_S16 s16EchoBandLow2;                      /* voice processing band2, low frequency parameter, [1, 63) for 8k, [1, 127) for 16k, recommend 47 */
    FY_S16 s16EchoBandHigh2;                     /* voice processing band2, fygh frequency parameter, (s16EchoBandLow2, 63] for 8k, (s16EchoBandLow2, 127] for 16k, recommend 72 */
                                                   /* s16EchoBandHigh2 must be greater than s16EchoBandLow2 */

    FY_S16 s16ERLBand[6];                        /* ERL protect area, [1, 63] for 8k, [1, 127] for 16k, frequency band calculated by s16ERLBand * 62.5 */
                                                  /* besides, s16ERLBand[n+1] should be greater than s16ERLBand[n] */
    FY_S16 s16ERL[7];                            /* ERL protect value of ERL protect area, the smaller its value, the more strength its protect ability,[0, 18]*/
    FY_S16 s16VioceProtectFreqL;                 /* protect area of near-end low frequency, [1, 63) for 8k, [1, 127) for 16k, recommend 3 */
    FY_S16 s16VioceProtectFreqL1;                /* protect area of near-end low frequency1, (s16VioceProtectFreqL, 63] for 8k, (s16VioceProtectFreqL, 127] for 16k, recommend 6 */
    FY_S32 s32Reserved;                          /* s16VioceProtectFreqL1 must be greater than s16VioceProtectFreqL */
} AI_AEC_CONFIG_S;

/**Defines the configure parameters of ANR.*/
typedef struct fyAUDIO_ANR_CONFIG_S
{
    FY_BOOL bUsrMode;   /* mode 0: auto,mode 1: manual.*/

    FY_S16 s16NrIntensity;       /* noise reduce intensity, range: [0, 25] */
    FY_S16 s16NoiseDbThr;        /* noise threshold, range: [30, 60] */
    FY_S8  s8SpProSwitch;        /* switch for music probe, range: [0:close, 1:open] */
   
    FY_S32 s32Reserved;
} AUDIO_ANR_CONFIG_S;

/**Defines the configure parameters of HPF.*/
typedef enum fyAUDIO_HPF_FREQ_E
{
    AUDIO_HPF_FREQ_80   = 80,    /* 80Hz */
    AUDIO_HPF_FREQ_120  = 120,   /* 120Hz */
    AUDIO_HPF_FREQ_150  = 150,   /* 150Hz */
    AUDIO_HPF_FREQ_BUTT,
} AUDIO_HPF_FREQ_E;

typedef struct fyAUDIO_HPF_CONFIG_S
{
    FY_BOOL bUsrMode;   /* mode 0: auto mode 1: mannual.*/
    AUDIO_HPF_FREQ_E enHpfFreq; /*freq to be processed*/
} AUDIO_HPF_CONFIG_S;

typedef struct fyAI_RNR_CONFIG_S
{
    FY_BOOL bUsrMode;                /* mode 0: auto,mode 1: mannual.*/

    FY_S32  s32NrMode;               /*mode 0: floor noise; 1:ambient noise */

    FY_S32 s32MaxNrLevel;           /*max NR level range:[2,20]dB*/

    FY_S32  s32NoiseThresh;         /*noise threshold, range:[-80, -20]*/                     
} AI_RNR_CONFIG_S;

typedef struct fyAUDIO_EQ_CONFIG_S
{
    FY_S8  s8GaindB[VQE_EQ_BAND_NUM];   /*EQ band, include 100,200,250,350,500,800,1.2k,2.5k,4k,8k in turn, range:[-100, 20]*/
    FY_S32 s32Reserved;
} AUDIO_EQ_CONFIG_S;


/**Defines the configure parameters of UPVQE work state.*/
typedef enum fyVQE_WORKSTATE_E
{
    VQE_WORKSTATE_COMMON  = 0,   /* common environment, Applicable to the family of voice calls. */
    VQE_WORKSTATE_MUSIC   = 1,   /* music environment , Applicable to the family of music environment. */
    VQE_WORKSTATE_NOISY   = 2,   /* noisy environment , Applicable to the noisy voice calls.  */
} VQE_WORKSTATE_E;

/* HDR Set CODEC GAIN Function Handle type */
typedef FY_S32 (*pFuncGainCallBack)(FY_S32 s32SetGain);

typedef struct fyAI_HDR_CONFIG_S
{
    FY_BOOL bUsrMode;               /* mode 0: auto mode 1: mannual.*/

    FY_S32 s32MinGaindB;            /* the minimum of MIC(AI) CODEC gain, [0, 120]*/
    FY_S32 s32MaxGaindB;            /* the maximum of MIC(AI) CODEC gain, [0, 120]*/

    FY_S32 s32MicGaindB;            /* the current gain of MIC(AI) CODEC,[s32MinGaindB, s32MaxGaindB]*/
    FY_S32 s32MicGainStepdB;        /* the step size of gain adjustment, [1, 3], recommemd 2 */
    pFuncGainCallBack pcallback;    /* the callback function pointer of CODEC gain adjustment */
} AI_HDR_CONFIG_S;



/**Defines the configure parameters of VQE.*/
typedef struct fyAI_VQE_CONFIG_S
{
    FY_S32              bHpfOpen;
	FY_S32              bAecOpen;     
    FY_S32              bAnrOpen;
    FY_S32              bRnrOpen;
    FY_S32              bAgcOpen;
    FY_S32              bEqOpen;
    FY_S32              bHdrOpen;
	
    FY_S32              s32WorkSampleRate;  /* Sample Rate: 8KHz/16KHz/48KHz. default: 8KHz*/
    FY_S32              s32FrameSample; /* VQE frame length: 80-4096 */
    VQE_WORKSTATE_E     enWorkstate;

                                       
    AUDIO_HPF_CONFIG_S  stHpfCfg;
 	AI_AEC_CONFIG_S     stAecCfg;
    AUDIO_ANR_CONFIG_S  stAnrCfg;
    AI_RNR_CONFIG_S     stRnrCfg;
    AUDIO_AGC_CONFIG_S  stAgcCfg;  
    AUDIO_EQ_CONFIG_S   stEqCfg;
    AI_HDR_CONFIG_S     stHdrCfg;	
} AI_VQE_CONFIG_S;

typedef struct fyAO_VQE_CONFIG_S
{
    FY_S32              bHpfOpen;
    FY_S32              bAnrOpen;      
    FY_S32              bAgcOpen;
    FY_S32              bEqOpen;
    
    FY_S32              s32WorkSampleRate;  /* Sample Rate: 8KHz/16KHz/48KHz. default: 8KHz*/
    FY_S32              s32FrameSample;     /* VQE frame length: 80-4096 */
    VQE_WORKSTATE_E     enWorkstate;

    AUDIO_HPF_CONFIG_S stHpfCfg;
    AUDIO_ANR_CONFIG_S stAnrCfg;
    AUDIO_AGC_CONFIG_S stAgcCfg;
    AUDIO_EQ_CONFIG_S  stEqCfg;
} AO_VQE_CONFIG_S;


/*Defines the configure parameters of AI saving file.*/
typedef struct fyAUDIO_SAVE_FILE_INFO_S
{
    FY_BOOL     bCfg;
    FY_CHAR  	aFilePath[MAX_AUDIO_FILE_PATH_LEN];
	FY_CHAR  	aFileName[MAX_AUDIO_FILE_NAME_LEN];
    FY_U32 		u32FileSize;  /*in KB*/
} AUDIO_SAVE_FILE_INFO_S;

/*Defines whether the file is saving or not .*/
typedef struct fyAUDIO_FILE_STATUS_S
{
    FY_BOOL     bSaving;
} AUDIO_FILE_STATUS_S;

typedef enum fyEN_AIO_ERR_CODE_E
{
    AIO_ERR_VQE_ERR        = 65 , /*vqe error*/

} EN_AIO_ERR_CODE_E;

/* obsolete */
typedef enum fyAUDIO_CLKDIR_E
{
    AUDIO_CLKDIR_RISE      = 0,
    AUDIO_CLKDIR_FALL       = 1,

    AUDIO_CLKDIR_BUTT
} AUDIO_CLKDIR_E;

/* obsolete */
typedef struct fyAUDIO_FRAME_COMBINE_S
{
    AUDIO_FRAME_S stFrm;                /* audio frame */
    AEC_FRAME_S   stRefFrm;             /* AEC reference audio frame */
    FY_BOOL       bEnableVqe;         /* whether is enable vqe */
} AUDIO_FRAME_COMBINE_S;

/* obsolete */
typedef struct fyAUDIO_RESAMPLE_ATTR_S
{
    FY_U32                  u32InPointNum;      /* input point number of frame */
    AUDIO_SAMPLE_RATE_E     enInSampleRate;     /* input sample rate */
    AUDIO_SAMPLE_RATE_E     enOutSampleRate;    /* output sample rate */
} AUDIO_RESAMPLE_ATTR_S;

typedef struct fyAIO_RESMP_INFO_S
{
    FY_BOOL                 bReSmpEnable;      /* resample enable or disable */
    AUDIO_RESAMPLE_ATTR_S   stResmpAttr;
} AIO_RESMP_INFO_S;

/* obsolete */
typedef enum fyAUDIO_AEC_MODE_E
{
    AUDIO_AEC_MODE_CLOSE	 = 0,
    AUDIO_AEC_MODE_OPEN	     = 1,

    AUDIO_AEC_MODE_BUTT
} AUDIO_AEC_MODE_E;

typedef struct audio_buf_ctrl{
	FY_U32 link_buf;
	FY_U8 link_buf_stat;
}AUDIO_BUFF_CTRL;

typedef struct audio_mem_s {
	FY_VOID *vir_addr;
	FY_U32 phy_addr;
}AUDIO_MEM_S;


typedef struct _fyAUDIO_MACRO{
	FY_S32 id;
	FY_U32 main_clk;
	FY_U32 fs_rate;
	FY_U8 chn_num;
	FY_U8 chn_width;
	FY_U8 is_slave;
	FY_U8 is_start;
	FY_U32 bus_width;
	FY_U8 clk_div;
	FY_U8 rtx_mode;
	FY_U8 data_format;

	FY_U32 i2s_node_ptr;
	FY_U32 cur_buf_size;
	FY_U32 usr_buf_size;
	FY_U32 is_need_flush;
	FY_U32 chn_buf_ptr[TOTAL_CHN_NUM];
	FY_U32 chn_buf_src[TOTAL_CHN_NUM];
	FY_U32 mix_buf_ptr;
	FY_U32 mix_buf_src;
	FY_U32 chn_cpy_byte;
	FY_U32 bus_cpy_byte;

	//AUDIO_MEM_S record_buf;
	//AUDIO_MEM_S play_buf;
	//AUDIO_BUFF_CTRL record_buf_ctl[MAX_CACHE_BUF_NODE];
	//AUDIO_BUFF_CTRL playback_buf_ctl[MAX_CACHE_BUF_NODE];
}fyAUDIO_MACRO_E;

typedef struct _fyAudio_ChnData_Format{
	FY_U8 cur_chn;
	unsigned long buf;
}fyAudio_ChnData_Format;

typedef struct _fyAUDIO_Equene_Param
{
	FY_S32 sDevId;
	FY_U32 intVal;
}fyAUDIO_Equene_Param;

typedef struct _fyAIO_CHN_status
{
	FY_BOOL bSet;
	FY_U8 uChnNum;	
	FY_U8 chn_width;	
	FY_U32 fs_rate;
}fyAIO_CHN_status;

typedef struct _TIME_PARAM{
	FY_U32 state;
	FY_U32 phy_addr;
	FY_U64 pts;
}TIME_PARAM;

typedef struct _AIO_TIME_STAMP
{
	FY_U32 chn_id;
	TIME_PARAM pts_info_ptr;
}AIO_TIME_STAMP;

/* invlalid device ID */
#define FY_ERR_AI_INVALID_DEVID     FY_DEF_ERR(FY_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
/* invlalid channel ID */
#define FY_ERR_AI_INVALID_CHNID     FY_DEF_ERR(FY_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define FY_ERR_AI_ILLEGAL_PARAM     FY_DEF_ERR(FY_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
/* using a NULL point */
#define FY_ERR_AI_NULL_PTR          FY_DEF_ERR(FY_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define FY_ERR_AI_NOT_CONFIG        FY_DEF_ERR(FY_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define FY_ERR_AI_NOT_SUPPORT       FY_DEF_ERR(FY_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change stati attribute */
#define FY_ERR_AI_NOT_PERM          FY_DEF_ERR(FY_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
/* the devide is not enabled  */
#define FY_ERR_AI_NOT_ENABLED       FY_DEF_ERR(FY_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
/* failure caused by malloc memory */
#define FY_ERR_AI_NOMEM             FY_DEF_ERR(FY_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
/* failure caused by malloc buffer */
#define FY_ERR_AI_NOBUF             FY_DEF_ERR(FY_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
/* no data in buffer */
#define FY_ERR_AI_BUF_EMPTY         FY_DEF_ERR(FY_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
/* no buffer for new data */
#define FY_ERR_AI_BUF_FULL          FY_DEF_ERR(FY_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
/* system is not ready,had not initialed or loaded*/
#define FY_ERR_AI_SYS_NOTREADY      FY_DEF_ERR(FY_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)

#define FY_ERR_AI_BUSY              FY_DEF_ERR(FY_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
/* vqe  err */ 
#define FY_ERR_AI_VQE_ERR       FY_DEF_ERR(FY_ID_AI, EN_ERR_LEVEL_ERROR, AIO_ERR_VQE_ERR)

/* invlalid device ID */
#define FY_ERR_AO_INVALID_DEVID     FY_DEF_ERR(FY_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
/* invlalid channel ID */
#define FY_ERR_AO_INVALID_CHNID     FY_DEF_ERR(FY_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define FY_ERR_AO_ILLEGAL_PARAM     FY_DEF_ERR(FY_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
/* using a NULL point */
#define FY_ERR_AO_NULL_PTR          FY_DEF_ERR(FY_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define FY_ERR_AO_NOT_CONFIG        FY_DEF_ERR(FY_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define FY_ERR_AO_NOT_SUPPORT       FY_DEF_ERR(FY_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change stati attribute */
#define FY_ERR_AO_NOT_PERM          FY_DEF_ERR(FY_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
/* the devide is not enabled  */
#define FY_ERR_AO_NOT_ENABLED       FY_DEF_ERR(FY_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
/* failure caused by malloc memory */
#define FY_ERR_AO_NOMEM             FY_DEF_ERR(FY_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
/* failure caused by malloc buffer */
#define FY_ERR_AO_NOBUF             FY_DEF_ERR(FY_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
/* no data in buffer */
#define FY_ERR_AO_BUF_EMPTY         FY_DEF_ERR(FY_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
/* no buffer for new data */
#define FY_ERR_AO_BUF_FULL          FY_DEF_ERR(FY_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
/* system is not ready,had not initialed or loaded*/
#define FY_ERR_AO_SYS_NOTREADY      FY_DEF_ERR(FY_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)

#define FY_ERR_AO_BUSY              FY_DEF_ERR(FY_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)

/* vqe  err */ 
#define FY_ERR_AO_VQE_ERR       FY_DEF_ERR(FY_ID_AO, EN_ERR_LEVEL_ERROR, AIO_ERR_VQE_ERR)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __FY_COMM_AIO_H__ */

