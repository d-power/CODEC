/******************************************************************************
Copyright(c) 2011-2019 Digital Power Inc.
File name: Rot.h
Author: LiuZhengzhong
Version: 1.0.0
Date: 2019/07/20
Description: rotate function by drm framework
History: 
Bug report: liuzhengzhong@d-power.com.cn
******************************************************************************/

#ifndef __MANAGER_ROT_H__
#define __MANAGER_ROT_H__

// 中文说明：该模块负责旋转摄像头或者显示的图像，依赖于DRM框架

typedef enum
{
	DRM_TRANSFORM_COPY          = 0, /* original image copy */
	DRM_TRANSFORM_FLIP_H        = 1, /* (1 << 0) */
	DRM_TRANSFORM_FLIP_V        = 2, /* (1 << 1) */
	DRM_TRANSFORM_ROT_90        = 4, /* (1 << 2) */
	DRM_TRANSFORM_ROT_180       = 3, /* (FLIP_H | FLIP_V) */
	DRM_TRANSFORM_ROT_270       = 7, /* ((FLIP_H | FLIP_V) + ROT_90) */
	DRM_TRANSFORM_FLIP_H_ROT_90 = 5, /* (FLIP_H + ROT_90) */
	DRM_TRANSFORM_FLIP_V_ROT_90 = 6, /* (FLIP_V + ROT_90) */
} Transform_E;

struct RotDrmVer
{
    unsigned int Major;
    unsigned int Minor;
};

struct RotDrmExecArg
{
	unsigned int Handle;
	Transform_E Transform;
	unsigned int InFb;
    unsigned int OutFb;
};

struct RotTimeout
{
	unsigned int Handle;
	unsigned long long Time;
};

int RotGetVersion(struct RotDrmVer *Ver);
int RotOpen(unsigned int *Handle);
int RotExec(struct RotDrmExecArg *ExecArg);
int RotClose(unsigned int *Handle);
int RotSetTimeout(struct RotTimeout *Timeout);

#endif