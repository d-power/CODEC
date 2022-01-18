#ifndef __SYS_INTERNAL_H__
#define __SYS_INTERNAL_H__


//reciver list of one channel
struct sender_2_recv
{
	struct sender* parrent;
//	struct sender_2_recv* pre;
	struct sender_2_recv* next;
	int recv_modID;
	int recv_modDev;
	int recv_modDevChan;
};

// one chan sender
struct sender
{
	struct sender_2_recv* recv_list; //struct sender_2_recv
	int bindCnt; // init: 0, max BIND_SRC_MAXNUM , bindCnt++
};


struct receiver
{
	int hasSource; // init: 0
	int send_modId;
	int send_modDev;
	int send_modDevChan;
	long long send_data_times; /* 0 */
};


struct binder // 48 bypte
{
	MOD_ID_E enModId;

	char name[8];
	unsigned int u32MaxDevCnt;
	unsigned int u32MaxChnCnt;
	receiver_get_data_cb  receive_func;
	receiver_reset_cb     receive_reset_func;
	sender_bind_notify send_bind_notify;
	sender_get_data_format get_format;
	receive_check_data_format check_format;

	unsigned int total_chan_count;

	struct sender* senders;
	struct receiver* receivers;
};



#ifdef __KERNEL__
	extern spinlock_t g_stSysBindLock;
	extern struct semaphore g_stSysDevSem;

	#define SYS_BIND_LOCK_INIT()						spin_lock_init(&g_stSysBindLock);
	#define SYS_BIND_LOCK_DESTROY()						0
	#define SYS_BIND_LOCK()								spin_lock_irqsave(&g_stSysBindLock,spin_flags)
	#define SYS_BIND_UNLOCK()							spin_unlock_irqrestore(&g_stSysBindLock,spin_flags)

	#define SYS_DEV_LOCK_INIT()							sema_init(&g_stSysDevSem,1)
	#define SYS_DEV_LOCK_DESTROY()						0
	#define SYS_DEV_LOCK()								down_interruptible(&g_stSysDevSem)
	#define SYS_DEV_UNLOCK()							up(&g_stSysDevSem)


	#define FATAL_ERROR(fmt, file,func,line,cont)		panic(fmt,file,func,line,cont)
//	#define ERROR_LOG(fmt...)							printk(KERN_WARNING "@KERNEL" fmt)
//	#define INFO_LOG(fmt...)							printk(KERN_INFO "@KERNEL" fmt)


	#define MALLOC(sz)									vmalloc(sz)//kmalloc(sz, GFP_KERNEL)
	#define FREE(p)										vfree(p)//kfree(p)
#else
	extern pthread_mutex_t g_stSysBindLock;
	extern pthread_mutex_t g_stSysDevSem;
	#define SYS_BIND_LOCK_INIT()						pthread_mutex_init((pthread_mutex_t *)&g_stSysBindLock, 0)
	#define SYS_BIND_LOCK_DESTROY()						pthread_mutex_destroy((pthread_mutex_t *)&g_stSysBindLock);
	#define SYS_BIND_LOCK()								pthread_mutex_lock((pthread_mutex_t *)&g_stSysBindLock)
	#define SYS_BIND_UNLOCK()							pthread_mutex_unlock((pthread_mutex_t *)&g_stSysBindLock)

	#define SYS_DEV_LOCK_INIT()							pthread_mutex_init((pthread_mutex_t *)&g_stSysDevSem, 0)
	#define SYS_DEV_LOCK_DESTROY()						pthread_mutex_destroy((pthread_mutex_t *)&g_stSysDevSem);
	#define SYS_DEV_LOCK()								pthread_mutex_lock((pthread_mutex_t *)&g_stSysDevSem)
	#define SYS_DEV_UNLOCK()							pthread_mutex_unlock((pthread_mutex_t *)&g_stSysDevSem)


	#define FATAL_ERROR(fmt, file,func,line,cont)		{printf(fmt,file,func,line,cont);exit(-1);}
	#define ERROR_LOG(fmt...)							fprintf((FILE *)stderr,"@LIB" fmt)
	#define INFO_LOG(fmt...)							printf("@LIB" fmt)


	#define MALLOC(sz)									malloc(sz)
	#define FREE(p)										free(p)
#endif



extern struct binder* s_pastSenderTbl[FY_ID_BUTT];
extern struct binder* s_pastReceiverTbl[FY_ID_BUTT];

extern  MPP_BIND_DEST_S stSendBindSrc_1;
extern  MPP_BIND_DEST_S stSendBindSrc_2;

int SysGetIdxByDevChn(struct binder *b, int s32DevId, int s32ChnId);
int SysGetDevChnByIdx(struct binder *b, unsigned int chan_idx, int *s32DevId, int* s32ChnId);
int SysGetBinderbySrc(MPP_CHN_S *pstSrcChn, MPP_BIND_DEST_S *dest);

#endif //__SYS_INTERNAL_H__