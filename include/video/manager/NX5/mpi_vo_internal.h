#ifndef __MPI_VO_INTERNAL_H__
#define __MPI_VO_INTERNAL_H__


#include "include/fy_errno.h"
#include "include/drv/vo_drv_ioc.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/syscall.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* Begin of #ifdef __cplusplus */


typedef struct fyVO_WBC_FD_S
{
    FY_S32                  s32Fd;
} VO_WBC_FD_S;

typedef struct fyVO_GFX_FD_S
{
    FY_S32                  s32Fd;
} VO_GFX_FD_S;


typedef struct fyVO_CHN_FD_S
{
    FY_S32                  s32Fd;
} VO_CHN_FD_S;

typedef struct fyVO_LAYER_FD_S
{
    FY_S32                  s32Fd;
    VO_CHN_FD_S             stChn[VO_MAX_CHN_NUM];
} VO_LAYER_FD_S;

typedef struct fyVO_Device_FD_S
{
    FY_S32                  s32Fd;
    VO_LAYER_FD_S           stLayer[VO_MAX_LAYER_NUM];
} VO_DEVICE_FD_S;

#define VO_WBC_BIND_MAGIC_NUM       0x08000000
#define VO_GFX_BIND_MAGIC_NUM       0x80000000

#define VO_DFT_CAS_DEV      (0)
#define VO_DFT_DEV          (0)
#define VO_DFT_LAYER        (0)

typedef enum fyMPI_LOG_LEVEL_E{
     MPI_LOG_VERBOSE = 0,
     MPI_LOG_INFO,
     MPI_LOG_DEBUG,
     MPI_LOG_WARN,
     MPI_LOG_ERROR,
     MPI_LOG_LEVEL_MAX
} MPI_LOG_LEVEL_E;

#define MPI_LOG_Print(level, msg, ...) \
   if(level > MPI_LOG_INFO) { \
	   printf(msg, ##__VA_ARGS__); \
   } \


#define MPI_LOG_CHECK_COND_GOTO(cond, tag, level, msg, ...) \
   if(cond) { \
	   MPI_LOG_Print(level,  "[Func]:%s [Line]:%d: " msg "\n",  __FUNCTION__, __LINE__, ##__VA_ARGS__); \
       goto tag; \
   } \


#define MPI_LOG_CHECK_COND_GOTO_RET(cond, tag, err, level, msg, ...) \
   if(cond) { \
      MPI_LOG_Print(level,  "[Func]:%s [Line]:%d: " msg "\n",  __FUNCTION__, __LINE__, ##__VA_ARGS__); \
      s32Ret = err; \
      goto tag; \
   } \


#define MPI_LOG_CHECK_COND_RETURN(cond, err, level, msg, ...)  \
   if(cond) { \
	   MPI_LOG_Print(level,  "[Func]:%s [Line]:%d: " msg "\n",  __FUNCTION__, __LINE__, ##__VA_ARGS__); \
       return err; \
   } \


#define MPI_LOG_CHECK_COND(cond, level, msg, ...)  \
	  if(cond) { \
		  MPI_LOG_Print(level,  "[Func]:%s [Line]:%d: " msg "\n",  __FUNCTION__, __LINE__, ##__VA_ARGS__); \
	  } \

#define MPI_LOG(level, msg, ...) \
   	MPI_LOG_Print(level,  "[Func]:%s [Line]:%d: " msg "\n",  __FUNCTION__, __LINE__, ##__VA_ARGS__); \



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif

