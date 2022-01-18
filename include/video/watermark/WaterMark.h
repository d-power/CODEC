#ifndef _WATER_MARK_H_
#define _WATER_MARK_H_

#include <video/ve/ViApi.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define YUV420_SP
#define MAX_ICON_PIC        83
#define DISP_ICON_NUM       23
#define MAX_WATERMARK_NUM   5

typedef struct _BackGroudLayerInfo
{
    int width;
    int height;
    unsigned char *y;
#ifdef YUV420_SP
    unsigned char *c;
#else
    unsigned char *u;
    unsigned char *v;
#endif
} BackGroudLayerInfo;

// 单张图片的属性
typedef struct _SinglePicture
{
    int id;		            //picture id
    int width;              //pic width
    int height;             //pic height
    unsigned char *y;
#ifdef YUV420_SP
    unsigned char *c;
#else
    unsigned char *u;
    unsigned char *v;
#endif
    unsigned char *alph;
} SinglePicture;

// 多张图片的属性
typedef struct _WaterMarkInfo
{
    int picture_number;
    SinglePicture single_pic[MAX_ICON_PIC];
} WaterMarkInfo;

// 水印的起始坐标点
typedef struct _WaterMarkPositon
{
    int x;
    int y;
} WaterMarkPositon;

// 水印显示参数
typedef struct _ShowWaterMarkParam
{
    WaterMarkPositon pos;          //the position of the waterMark
    int number;
    int id_list[DISP_ICON_NUM];    //the index of the picture of the waterMark
} ShowWaterMarkParam;

typedef struct _AjustBrightnessParam
{
    int recycle_frame;			   // brightness recycle frame
    int cur_frame;				   // use this to decide if ajust brightness
    int number;					   // number as same as ShowWaterMarkParam.number
    int Brightness[DISP_ICON_NUM]; // the index of the picture of the waterMark
} AjustBrightnessParam;

// 单个水印的属性
typedef struct
{
    int positionX;
    int positionY;
    char *content;  //[DISP_ICON_NUM];
} SingleWaterMark;

// 多个水印的属性
typedef struct
{
    int waterMarkNum;
    SingleWaterMark singleWaterMark[MAX_WATERMARK_NUM];
} WaterMarkMultiple;

// 记录所有水印有关的属性
typedef struct _WATERMARK_CTRL
{
    AjustBrightnessParam ADBright;
    WaterMarkInfo wminfo;
    WaterMarkMultiple multi;
} WATERMARK_CTRL;

typedef struct
{
    unsigned char *y;
#ifdef YUV420_SP
    unsigned char *c;
#else
    unsigned char *u;
    unsigned char *v;
#endif
    int posx;
    int posy;
    int width;
    int height;
    char *display;
    int main_channel;
    float scale;
} WaterMarkInData;

void *initialwaterMark(char *path, int count, PicMap_S *pMap);

int releaseWaterMark(void *ctrl);

int waterMarkSetMultiple(void *ctrl, const char *waterMark);

int MultipleWaterMarkSet(void *ctrl, MWaterMarkInfo_S *pMInfo);

int watermark_blending(BackGroudLayerInfo *bg_info, WaterMarkInfo *wm_info, ShowWaterMarkParam *wm_Param);

int doWaterMarkMultiple(WaterMarkInData *data, void *ctrl, const char *time_str);

int WaterMarkMultipleShow(WaterMarkInData *data, void *ctrl, int count, PicMap_S *pMap);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
