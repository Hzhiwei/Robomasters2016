#ifndef __DRV_GUI_H
#define __DRV_GUI_H

#include "stm32f4xx.h"
#include "string.h"
#include "bsp_oled.h"


/** 
  * @brief GUI State
  */ 
typedef enum
{
  GUI_FAILED = 0,
  GUI_SUCCESS = 1,
}GUI_State_EmTypedef;

/** 
  * @brief 图片控件数据结构 
  */  
typedef struct
{
  //控件左上角的位置
  uint16_t x;
  uint16_t y;
  //控件水平尺寸
  uint16_t hSize;
  //控件垂直尺寸
  uint16_t vSize;
  //要显示的图片的信息
  BmpInfoStructTypedDef* imageInfo;

}*WidgetImageHandle;


/** 
  * @brief 文本控件数据结构 
  */  
typedef struct
{
  //控件左上角的位置
  uint16_t x;
  uint16_t y;
  //控件水平尺寸
  uint16_t hSize;
  //控件垂直尺寸
  uint16_t vSize;
  //包含的文本信息
  uint8_t* pText;
  //是否显示边框
  ISorNOT isShowRim;
}*WidgetTextHandle;


/** 
  * @brief 进度条控件数据结构 
  */  
typedef struct
{
  //进度条左上角的位置
  uint16_t x;
  uint16_t y;
  //进度条水平尺寸
  uint16_t hSize;
  //进度条垂直尺寸
  uint16_t vSize;
  //控件最小值
  int32_t miniMum;
  //控件最大值
  int32_t maxiMum;
  //控件当前值
  float value;
  //数值显示状态
  ISorNOT isShowValue;
  //进度条形式
  uint8_t form;
}*WidgetProgBarHandle;


/** 
  * @brief check State
  */ 
typedef enum
{
  FALSE = 0,
  TURE  = 1,
}TorF_EmTypeDef;

/** 
  * @brief checkbox控件数据结构 
  */  
typedef struct
{
  //checkbox控件左上角的位置
  uint16_t x;
  uint16_t y;
  //checkbox控件水平尺寸
  uint16_t hSize;
  //checkbox控件垂直尺寸
  uint16_t vSize;
  //控件当前状态
  TorF_EmTypeDef checked;
  //控件文本
  uint8_t* pText;
  //text invChoose
  InvChoose textInv;
}*WidgetCheckBoxHandle;


/** 
  * @brief 控件类型枚举 
  */ 
typedef enum
{
  WIDGET_IMAGE  = 0,
  WIDGET_TEXT,
  WIDGET_PROGBAR,
  WIDGET_CHECKBOX,
}WidgetTypeEmTypeDef;

/** 
  * @brief 控件结构 
  */ 
typedef struct
{
  //控件类型
  WidgetTypeEmTypeDef type;
  //控件句柄
  uint32_t handle;
}WidgetTypeDef;

/**
  * @brief 菜单结构
  */ 
typedef struct _menu
{
  //父菜单
  struct _menu*  p_parMenu;
  WidgetTypeDef x;
}menuTypeDef;


void GUI_DrawHLine(uint16_t y,uint16_t x0,uint16_t x1,ISorNOT isShow,ISorNOT isRefresh);
void GUI_DrawVLine(uint16_t x,uint16_t y0,uint16_t y1,ISorNOT isShow,ISorNOT isRefresh);
void GUI_DrawLine(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,ISorNOT isShow,ISorNOT isRefresh);
void GUI_DrawRect(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,ISorNOT isShow,ISorNOT isRefresh);
void GUI_DrawFullRect(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,ISorNOT isShow,ISorNOT isRefresh);
void GUI_DrawBmp(int16_t x, int16_t y, BmpInfoStructTypedDef* bmpInfo, InvChoose InvSW);

WidgetImageHandle GUI_WidgetImage_Create(uint16_t x, uint16_t y, uint16_t hSize, uint16_t vSize);
GUI_State_EmTypedef GUI_WidgetImage_SetBmp(WidgetImageHandle handle, BmpInfoStructTypedDef* bmpInfo);
GUI_State_EmTypedef GUI_WidgetImage_SetHV(WidgetImageHandle handle, uint16_t hSize, uint16_t vSize);
GUI_State_EmTypedef GUI_WidgetImage_Show(WidgetImageHandle handle, ISorNOT isShow, ISorNOT isRefresh);

WidgetTextHandle GUI_WidgetText_Create(uint16_t x, uint16_t y, uint16_t hSize, uint16_t vSize);
GUI_State_EmTypedef GUI_WidgetText_SetText(WidgetTextHandle handle, void* fmt, ...);
GUI_State_EmTypedef GUI_WidgetText_AddText(WidgetTextHandle handle, void* fmt, ...);
GUI_State_EmTypedef GUI_WidgetText_SetRim(WidgetTextHandle handle,ISorNOT isShowRim);
GUI_State_EmTypedef GUI_WidgetText_Show(WidgetTextHandle handle, ISorNOT isShow, ISorNOT isRefresh);

WidgetProgBarHandle GUI_WidgetProgBar_Create(uint16_t x, uint16_t y, int32_t miniMum,int32_t maxiMum,\
      uint16_t hSize, float value, uint8_t form, ISorNOT isShowValue);
GUI_State_EmTypedef GUI_WidgetProgBar_SetVal(WidgetProgBarHandle handle, float value);
GUI_State_EmTypedef GUI_WidgetProgBar_Show(WidgetProgBarHandle handle, ISorNOT isShow, ISorNOT isRefresh);

WidgetCheckBoxHandle GUI_WidgetCheckBox_Create(uint16_t x, uint16_t y,\
      uint16_t hSize, TorF_EmTypeDef checked, void* pText);
GUI_State_EmTypedef GUI_WidgetCheckBox_SetVal(WidgetCheckBoxHandle handle,TorF_EmTypeDef checked);
GUI_State_EmTypedef GUI_WidgetCheckBox_SetTextInv(WidgetCheckBoxHandle handle,InvChoose textInv);
GUI_State_EmTypedef GUI_WidgetCheckBox_Show(WidgetCheckBoxHandle handle, ISorNOT isShow, ISorNOT isRefresh);

void GUI_WidgetDestroy(WidgetTypeDef* widget);
void GUI_WidgetFresh(WidgetTypeDef* widgets, uint16_t num, ISorNOT isRefresh);

#endif
