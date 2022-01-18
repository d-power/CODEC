#ifndef  __DRV_IOC_EXTENSION_H__
#define  __DRV_IOC_EXTENSION_H__

/******************************************************************************
  Include other header files
 *****************************************************************************/



#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  /* __cplusplus */

/******************************************************************************
  Macros, Enums, Structures definition list
 *****************************************************************************/
#define FY_DRV_ADV_IOC_E    0xFE

typedef struct fyDRV_IOC_EXTENSION_S
{
    unsigned short ext_cmd;//extended driver ioc cmd
    unsigned short ext_param_len;//parameter bytes  of the specific cmd  
    void *p_param;//pointer to parameter structure of the specific cmd, driver can check parameter size by(ext_param_len==sizeof(param structure))
} FY_DRV_IOC_EXTENSION_S;




#define FY_DRV_ADV_IOC(MODULE_IOC_MAGIC)    _IOWR(MODULE_IOC_MAGIC, FY_DRV_ADV_IOC_E, FY_DRV_IOC_EXTENSION_S) 

//example, driver module include this header file in its drv_ioc.h, then define its ADV_IOC
//#define IOC_VI_ADV_IOC  FY_DRV_ADV_IOC(VIU_IOC_MAGIC)
//#define IOC_VPSS_ADV_IOC  FY_DRV_ADV_IOC(VPU_IOC_MAGIC)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  /* __cplusplus */

#endif 

