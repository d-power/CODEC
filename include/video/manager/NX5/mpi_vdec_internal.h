#ifndef __MPI_VDEC_INTERNAL_H__
#define __MPI_VDEC_INTERNAL_H__

#define MAX_FRAME_BUFFER_COUNT 16

#define DEBUG_LOG 0

#define CHAN_MUTEX 0

typedef struct _MPI_VDEC_CHN_S
{
    PAYLOAD_TYPE_E      enType;
    fyVDEC_CHN_ATTR_S  chnAttr;
    //VDEC_CHN_PARAM_S chnParam;
    //VDEC_CHN_STAT_S  chnStat;
    //VIDEO_DISPLAY_MODE_E enDisplayMode;
    FY_U32 vbCount;

    int  state; /* 0 - free, 1 - used*/
    int fd;
#if(CHAN_MUTEX)
    pthread_mutex_t *mutex;
#endif
}MPI_VDEC_CHN_S;


extern MPI_VDEC_CHN_S g_stMpiVdecChn[VDEC_MAX_CHN_NUM];

extern void MPI_VDEC_Chan_Init(VDEC_CHN VdChn);

#if(DEBUG_LOG)
#define JPEGD_ENTER() printf("Enter %s\n",__FUNCTION__)
#define JPEGD_EXIT()  printf("EXIT %s\n",__FUNCTION__)
#define MPI_LOG(s...) fprintf((FILE *)stderr,##s)
#else
#define JPEGD_ENTER()
#define JPEGD_EXIT()
#define   MPI_LOG(s...)
#endif


#if(CHAN_MUTEX)
#define MPI_DEC_CHN_LOCK(pChn)     {pthread_mutex_lock(((pChn)->mutex)); JPEGD_ENTER();}
#define MPI_DEC_CHN_UNLOCK(pChn)   {pthread_mutex_unlock(((pChn)->mutex));JPEGD_EXIT();}
#else
#define MPI_DEC_CHN_LOCK(pChn)     { JPEGD_ENTER();}
#define MPI_DEC_CHN_UNLOCK(pChn)   {JPEGD_EXIT();}
#endif


#endif// __MPI_VDEC_INTERNAL_H__
