#ifndef  __VB_DRV_IOC_H__
#define  __VB_DRV_IOC_H__

/******************************************************************************
  Include other header files
 *****************************************************************************/

#include "../fy_defines.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  /* __cplusplus */

/******************************************************************************
  Macros, Enums, Structures definition list
 *****************************************************************************/
 typedef struct fyVB_IOC_ARG_S
{
    unsigned int PoolId;
    unsigned int BlkSize;
    unsigned int BlkCnt;
    unsigned int BlkHandle;
    unsigned int IsCommPool;
    char MmzName[MAX_MMZ_NAME_LEN+1];
    unsigned int Reserve;
    unsigned int PhyAddr;	
}VB_IOC_ARG_S;

typedef struct fyVB_IOC_EXT_ARG_S
{
    unsigned int PoolId;
    unsigned int BlkSize;
    unsigned int BlkCnt;
    unsigned int BlkHandle;
    unsigned int IsCommPool;
    char MmzName[MAX_MMZ_NAME_LEN+1];
	char MmbName[MMAX_MMB_NAME_LEN+1];
    unsigned int Reserve;
    unsigned int PhyAddr;
}VB_IOC_EXT_ARG_S;


typedef struct fyVB_POOL_INFO_S {
    unsigned int PoolId;
    unsigned int PoolSize;
    unsigned int BlkSize;
    unsigned int PoolPhyAddr;
    unsigned int UserMode;
    void *PoolVirAddr;
} VB_POOL_INFO_S;

typedef struct fyVB_MOD_CONF_S
{
    VB_UID_E UserID;
    VB_CONF_S VbConfig;
} VB_MOD_CONF_S;

typedef enum fyIOC_VB_E
{
    IOC_VB_CREATE_POOL = 1,    
    IOC_VB_DESTROY_POOL,    
    IOC_VB_GET_BLOCK,    
    IOC_VB_RELEASE_BLOCK,    
    IOC_VB_HANDLE2_PHYSADDR,
    
    IOC_VB_HANDLE2_POLLID,
    IOC_VB_INQUIRE_USERCNT,
    IOC_VB_INIT,
    IOC_VB_EXIT,
    IOC_VB_GET_CONF = 10,
    
    IOC_VB_SET_CONF,
    IOC_VB_MMAP_POOL,
    IOC_VB_MUNMAP_POOL,
    IOC_VB_INIT_MODPOOL,
    IOC_VB_EXIT_MODPOOL,
    IOC_VB_GET_MODPOOL_CONF,
    IOC_VB_SET_MODPOOL_CONF = 17,

    IOC_VB_CREATE_POOLEX = 20,
    IOC_VB_DESTROY_POOLEX,
    IOC_VB_GET_BLOCKEX,
    IOC_VB_RELEASE_BLOCKEX,
    IOC_VB_MMAP_POOLEX,

    IOC_VB_MUNMAP_POOLEX,
    IOC_VB_HANDLE2_PHYSADDREX,
    IOC_VB_CREATE_VIRTUALVBPOOL,
    IOC_VB_ADDBLOCK,
} IOC_VB_E;


/******************************************************************************
  VB driver ioctl definition list
 *****************************************************************************/
#define VB_IOC_MAGIC_CODE   'B'

#define VB_CREATE_POOL_CTRL    _IOWR(VB_IOC_MAGIC_CODE, IOC_VB_CREATE_POOL, VB_IOC_ARG_S)
#define VB_DESTROY_POOL_CTRL    _IOWR(VB_IOC_MAGIC_CODE, IOC_VB_DESTROY_POOL, unsigned int)
#define VB_GET_BLOCK_CTRL    _IOWR(VB_IOC_MAGIC_CODE, IOC_VB_GET_BLOCK, VB_IOC_ARG_S)
#define VB_RELEASE_BLOCK_CTRL    _IOWR(VB_IOC_MAGIC_CODE, IOC_VB_RELEASE_BLOCK, unsigned int)
#define VB_HANDLE2_PHYSADDR_CTRL    _IOWR(VB_IOC_MAGIC_CODE, IOC_VB_HANDLE2_PHYSADDR, unsigned int)

#define VB_HANDLE2_POLLID_CTRL    _IOWR(VB_IOC_MAGIC_CODE, IOC_VB_HANDLE2_POLLID, unsigned int)
#define VB_INQUIRE_USERCNT_CTRL    _IOWR(VB_IOC_MAGIC_CODE, IOC_VB_INQUIRE_USERCNT, unsigned int)
#define VB_INIT_CTRL    _IO(VB_IOC_MAGIC_CODE, IOC_VB_INIT)
#define VB_EXIT_CTRL    _IO(VB_IOC_MAGIC_CODE, IOC_VB_EXIT)
#define VB_GET_CONF_CTRL    _IOR(VB_IOC_MAGIC_CODE, IOC_VB_GET_CONF, VB_CONF_S)

#define VB_SET_CONF_CTRL    _IOW(VB_IOC_MAGIC_CODE, IOC_VB_SET_CONF, VB_CONF_S)
#define VB_MMAP_POOL_CTRL    _IOWR(VB_IOC_MAGIC_CODE, IOC_VB_MMAP_POOL, VB_POOL_INFO_S)
#define VB_MUNMAP_POOL_CTRL    _IOWR(VB_IOC_MAGIC_CODE, IOC_VB_MUNMAP_POOL, unsigned int)
#define VB_INIT_MODPOOL_CTRL    _IOW(VB_IOC_MAGIC_CODE, IOC_VB_INIT_MODPOOL, unsigned int)
#define VB_EXIT_MODPOOL_CTRL    _IOW(VB_IOC_MAGIC_CODE, IOC_VB_EXIT_MODPOOL, unsigned int)
#define VB_GET_MODPOOL_CONF_CTRL    _IOWR(VB_IOC_MAGIC_CODE, IOC_VB_GET_MODPOOL_CONF, VB_MOD_CONF_S)
#define VB_SET_MODPOOL_CONF_CTRL    _IOW(VB_IOC_MAGIC_CODE, IOC_VB_SET_MODPOOL_CONF, VB_MOD_CONF_S)

#define VB_CREATE_POOLEX_CTRL    _IOW(VB_IOC_MAGIC_CODE, IOC_VB_CREATE_POOLEX, VB_IOC_EXT_ARG_S)
#define VB_DESTROY_POOLEX_CTRL    _IOW(VB_IOC_MAGIC_CODE, IOC_VB_DESTROY_POOLEX, unsigned int)
#define VB_GET_BLOCKEX_CTRL    _IOW(VB_IOC_MAGIC_CODE, IOC_VB_GET_BLOCKEX, VB_IOC_ARG_S)
#define VB_RELEASE_BLOCKEX_CTRL    _IOW(VB_IOC_MAGIC_CODE, IOC_VB_RELEASE_BLOCKEX, unsigned int)
#define VB_MMAP_POOLEX_CTRL    _IOW(VB_IOC_MAGIC_CODE, IOC_VB_MMAP_POOLEX, VB_POOL_INFO_S)

#define VB_MUNMAP_POOLEX_CTRL    _IOW(VB_IOC_MAGIC_CODE, IOC_VB_MUNMAP_POOLEX, unsigned int)
#define VB_HANDLE2_PHYSADDREX_CTRL    _IOW(VB_IOC_MAGIC_CODE, IOC_VB_HANDLE2_PHYSADDREX, unsigned int)
#define VB_CREATE_VIRTUALVBPOOL_CTRL    _IOWR(VB_IOC_MAGIC_CODE, IOC_VB_CREATE_VIRTUALVBPOOL, VB_IOC_ARG_S)
#define VB_ADDBLOCK_CTRL    _IOWR(VB_IOC_MAGIC_CODE, IOC_VB_ADDBLOCK, VB_IOC_ARG_S)



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  /* __cplusplus */

#endif /*__VB_DRV_IOC_H__*/
