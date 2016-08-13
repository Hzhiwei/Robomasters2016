/**
  ******************************************************************************
  * @file    drv_gui.c
  * @author  EC_Dog
  * @version V1.0
  * @date    2016/08/01
  * @brief   
  *   全部基于画点函数绘制
  ******************************************************************************
  * @attention
  *   有很多函数是不安全的。。。
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "drv_gui.h"
#include "bsp_oled.h"
#include "stdlib.h"
#include "stdarg.h"
/* Defines -------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/


/**
  * @brief  绘制水平线条
  * @param  y:y位置
  * @param  x0:x起始位置
  * @param  x1:x结束位置
  * @param  isShow: not.黑；is.亮
  * @param  isRefresh: not.不更新到oled;is.更新
  * @retval None
*/
void GUI_DrawHLine(uint16_t y,uint16_t x0,uint16_t x1,ISorNOT isShow,ISorNOT isRefresh)
{ 
  for(uint16_t x=x0;x<=x1;x++)
    OLED_DrawPixel(x,y,isShow,isRefresh);
}


/**
  * @brief  绘制垂直线条
  * @param  x:x位置
  * @param  y0:y起始位置
  * @param  y1:y结束位置
  * @param  isShow: not.黑；is.亮
  * @param  isRefresh: not.不更新到oled;is.更新
  * @retval None
*/
void GUI_DrawVLine(uint16_t x,uint16_t y0,uint16_t y1,ISorNOT isShow,ISorNOT isRefresh)
{
  for(uint16_t y= y0;y<=y1;y++)
    OLED_DrawPixel(x,y,isShow,isRefresh);
}


/**
  * @brief  绘制线条
  * @param  x0:x起始位置（小）
  * @param  y0:y起始位置
  * @param  x1:x结束位置（大）
  * @param  y1:y结束位置
  * @param  isShow: not.黑；is.亮
  * @param  isRefresh: not.不更新到oled;is.更新
  * @retval None
*/
void GUI_DrawLine(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,ISorNOT isShow,ISorNOT isRefresh)
{
  float k = 0;
  float b = 0;
  float fX0 = x0;
  float fX1 = x1;
  float fY0 = y0;
  float fY1 = y1;
  uint16_t x = 0;
  uint16_t y = 0;
  
  k = (fY1-fY0)/(fX1-fX0);
  b = fY1-k*fX1;
  
  for(x=x0;x<=x1;x++)
  {
    y=k*x+b;
    OLED_DrawPixel(x,y,isShow,isRefresh);
  }
}


/**
  * @brief  绘制矩形
  * @param  x0:x左上角
  * @param  y0:x左上角
  * @param  x1:y右下角
  * @param  y1:y右下角
  * @param  isShow: not.黑；is.亮
  * @param  isRefresh: not.不更新到oled;is.更新
  * @retval None
*/
void GUI_DrawRect(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,ISorNOT isShow,ISorNOT isRefresh)
{
  GUI_DrawHLine(y0,x0,x1,isShow,isRefresh);
  GUI_DrawHLine(y1,x0,x1,isShow,isRefresh);
  GUI_DrawVLine(x0,y0,y1,isShow,isRefresh);
  GUI_DrawVLine(x1,y0,y1,isShow,isRefresh);
}


/**
  * @brief  绘制实心矩形()
  * @param  x0:x左上角
  * @param  y0:x左上角
  * @param  x1:y右下角
  * @param  y1:y右下角
  * @param  isShow: not.黑；is.亮
  * @param  isRefresh: not.不更新到oled;is.更新
  * @retval None
*/
void GUI_DrawFullRect(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,ISorNOT isShow,ISorNOT isRefresh)
{
  for(int16_t y=y0;y<=y1;y++)
  {
     GUI_DrawHLine(y,x0,x1,isShow,isRefresh);
  }
}


/**
  * @brief  在指定起点绘制图片，以点为单位一列一列打印
  * @param  x:左上角x
  * @param  y:左上角y
  * @param  bmp:图片信息
  * @param  InvSW:翻转开关
  * @retval None
*/
void GUI_DrawBmp(int16_t x, int16_t y, BmpInfoStructTypedDef* bmpInfo, InvChoose InvSW)
{
  
  int16_t begX=x<0?0:x;       //开始
  int16_t begY=y<0?0:y;
  int16_t begXCnt=x<0?0-x:0;
  int16_t begYCnt=y<0?0-y:0;
  
  int16_t posX=begX, posY=begY;
  uint16_t xCnt, yCnt;
  uint8_t bitShow;   //显示
  for(xCnt=begXCnt; xCnt<bmpInfo->width; xCnt++,posX++)
  {
    for(yCnt=begYCnt,posY=begY; yCnt<bmpInfo->height; yCnt++,posY++)
    {
      if(InvSW==INV_OFF)
      {
        bitShow = (*(bmpInfo->pBmp+xCnt+(yCnt/8)*bmpInfo->width)>>(yCnt%8))&0x01;
      }
      else
      {
        bitShow = !((*(bmpInfo->pBmp+xCnt+(yCnt/8)*bmpInfo->width)>>(yCnt%8))&0x01);
      }
      OLED_DrawPixel(posX,posY,(ISorNOT)bitShow,NOT);
    }
  }
}


/***************************************************************
  * 图片控件 begin
****************************************************************/
/**
  * @brief  创建图片显示控件
  * @param  x:左上角x
  * @param  y:左上角y
  * @param  hSize:水平宽度
  * @param  vSize:垂直高度
  * @retval WidgetImageHandle:控件句柄
*/
WidgetImageHandle GUI_WidgetImage_Create(uint16_t x, uint16_t y, uint16_t hSize, uint16_t vSize)
{
  WidgetImageHandle handle;
  
  //控件初始化
  handle = malloc(sizeof(*handle));
  handle->x = x;
  handle->y = y;
  handle->hSize = hSize;
  handle->vSize = vSize;
  
  return handle;
}


/**
  * @brief  设置控件对应图片
  * @param  pBmpInfo:指向图片信息的指针
  * @param  handle:控件句柄
  * @retval GUI_State_EmTypedef
*/
GUI_State_EmTypedef GUI_WidgetImage_SetBmp(WidgetImageHandle handle, BmpInfoStructTypedDef* bmpInfo)
{
  if(handle != NULL)
  {
    handle->imageInfo = bmpInfo;
    return GUI_SUCCESS;
  }
  else
  {
    return GUI_FAILED;
  }
}


/**
  * @brief  设置图片控件大小
  * @param  handle:控件句柄
  * @param  hSize:水平宽度
  * @param  vSize:垂直高度
  * @retval GUI_State_EmTypedef
*/
GUI_State_EmTypedef GUI_WidgetImage_SetHV(WidgetImageHandle handle, uint16_t hSize, uint16_t vSize)
{
  handle->hSize = hSize;
  handle->vSize = vSize;
  
  if(handle != NULL)
  {
    handle->hSize = hSize;
    handle->vSize = vSize;
    return GUI_SUCCESS;
  }
  else
  {
    return GUI_FAILED;
  }
}


/**
  * @brief  图片控件显示
  * @param  handle:控件句柄
  * @param  isShow:是否显示
  * @param  isRefresh: not.不直接显示到oled;is.更新
  * @retval GUI_State_EmTypedef
*/
GUI_State_EmTypedef GUI_WidgetImage_Show(WidgetImageHandle handle, ISorNOT isShow, ISorNOT isRefresh)
{
  if(handle!=NULL && handle->imageInfo!=NULL)
  {
    if(isShow == IS)
    {
      OLED_DrawBmp(handle->x,handle->y,handle->hSize,handle->vSize,handle->imageInfo, INV_OFF);
    }
    else
    {
      GUI_DrawFullRect(handle->x,handle->y,handle->x+handle->hSize-1,handle->y+handle->vSize-1,isShow,isRefresh);
    }
    return GUI_SUCCESS;
  }
  else
  {
    return GUI_FAILED;
  }
}
/***************************************************************
  * 图片控件 end
****************************************************************/


/***************************************************************
  * 文本控件 begin(Test OK)
****************************************************************/
/**
  * @brief  创建文本控件
  * @param  x:左上角x
  * @param  y:左上角y
  * @param  hSize:水平宽度
  * @param  vSize:垂直高度
  * @retval WidgetTextHandle:控件句柄
*/
WidgetTextHandle GUI_WidgetText_Create(uint16_t x, uint16_t y, uint16_t hSize, uint16_t vSize)
{
  WidgetTextHandle handle;
  handle = malloc(sizeof(*handle));
  handle->x = x,
  handle->y = y,
  
  handle->hSize = hSize;
  handle->vSize = vSize;
  return handle;
}


/**
  * @brief  为文本框添加文本
  * @param  handle:控件句柄
  * @param  fmt:文本  \n：可以换行。。。
  * @retval GUI_State_EmTypedef
*/
GUI_State_EmTypedef GUI_WidgetText_AddText(WidgetTextHandle handle, void* fmt, ...)
{
  if(handle != NULL)
  {
    uint8_t oledBuf[256];
    int16_t newLen;   //新添加的数据长度
    int16_t oldLen;   //原始数据长度
    uint8_t* tmpPText;
    va_list ap;
    
    if(handle->pText != NULL)
    {
      oldLen = strlen((char*)handle->pText);
    }
    
    va_start(ap,fmt);
    newLen = vsnprintf((char*)oledBuf, 256, (char*)fmt, ap);
    va_end(ap);
    
    if(newLen<0)
    {
      return GUI_FAILED;
    }
    else
    {
      tmpPText = handle->pText;
      handle->pText = malloc(newLen+oldLen+1);
      if(handle->pText!=NULL)
      {
        strcpy((char*)(handle->pText), (char*)tmpPText);
        free(tmpPText);
        strcpy((char*)(handle->pText+oldLen), (char*)oledBuf);
        return GUI_SUCCESS;
      }
      else//创建失败
      {
        handle->pText = tmpPText;
        return GUI_FAILED;
      }
    }
  }
  else
  {
    return GUI_FAILED;
  }
}


/**
  * @brief  设置控件对应文本
  * @param  handle:控件句柄
  * @param  fmt:文本  \n：可以换行。。。
  * @retval GUI_State_EmTypedef
*/
GUI_State_EmTypedef GUI_WidgetText_SetText(WidgetTextHandle handle, void* fmt, ...)
{
  if(handle != NULL)
  {
    uint8_t oledBuf[256];
    int16_t newLen;
    va_list ap;
    
    if(handle->pText != NULL)
    {
      free(handle->pText);
      handle->pText = NULL;
    }
    
    va_start(ap,fmt);
    newLen = vsnprintf((char*)oledBuf, 256, (char*)fmt, ap);
    va_end(ap);
    
    if(newLen<0)
    {
      return GUI_FAILED;
    }
    else
    {
      handle->pText = malloc(newLen+1);
      if(handle->pText!=NULL)
      {
        strcpy((char*)(handle->pText), (char*)oledBuf);
//        for(uint16_t i=0;i<newLen;i++)
//        {
//          *(handle->pText+i) = oledBuf[i];
//        }
        return GUI_SUCCESS;
      }
      else
      {
        return GUI_FAILED;
      }
    }
  }
  else
  {
    return GUI_FAILED;
  }
}

/**
  * @brief  isShowRim设置
  * @param  handle:控件句柄
  * @retval GUI_State_EmTypedef
*/
GUI_State_EmTypedef GUI_WidgetText_SetRim(WidgetTextHandle handle,ISorNOT isShowRim)
{
  if(handle!=NULL)
  {
    handle->isShowRim = isShowRim;
    return GUI_SUCCESS;
  }
  else
  {
    return GUI_FAILED;
  }
}


/**
  * @brief  文本控件显示
  * @param  handle:控件句柄
  * @param  isShow:是否显示文本
  * @param  isRefresh: not.不直接显示到oled;is.更新
  * @retval GUI_State_EmTypedef
*/
GUI_State_EmTypedef GUI_WidgetText_Show(WidgetTextHandle handle, ISorNOT isShow, ISorNOT isRefresh)
{
  if(handle!=NULL)
  {
    uint16_t x1 = handle->x+handle->hSize-1;
    uint16_t y1 = handle->y+handle->vSize-1;
    
    //最外边的框
    if(handle->isShowRim == IS)
      GUI_DrawRect(handle->x,handle->y,x1,y1,IS,isRefresh);
    else
      GUI_DrawRect(handle->x,handle->y,x1,y1,NOT,isRefresh);
    //内层的框
    GUI_DrawRect(handle->x+1,handle->y+1,x1-1,y1-1,NOT,isRefresh);
    
    if(handle->pText!=NULL && handle->hSize>=2 && handle->vSize>=2)
    {
      if(isShow == IS)
        OLED_Print6x8Str(handle->x+2,handle->y+2,handle->hSize-4,handle->vSize-4,
              handle->pText,INV_OFF,isRefresh);
      else
        GUI_DrawFullRect(handle->x+2,handle->y+2,x1-2,y1-2,NOT,isRefresh);
    }
    return GUI_SUCCESS;
  }
  else
  {
    return GUI_FAILED;
  }
}
/***************************************************************
  * 文本控件 end
****************************************************************/


/***************************************************************
  * 进度条控件 begin
****************************************************************/
/**
  * @brief  创建进度条控件
  * @param  x:左上角x
  * @param  y:左上角y
  * @param  hSize:水平宽度
  * @param  vSize:垂直高度(不开放)
  * @param  value:值设置
  * @param  form:0.形式0；1.形式1
  * @param  isShowValue:
  * @retval WidgetProgBarHandle:控件句柄
*/
WidgetProgBarHandle GUI_WidgetProgBar_Create(uint16_t x, uint16_t y, int32_t miniMum,int32_t maxiMum,\
      uint16_t hSize, float value, uint8_t form, ISorNOT isShowValue)
{
  WidgetProgBarHandle handle;
  handle = malloc(sizeof(*handle));
  
  handle->x = x,
  handle->y = y,
  handle->miniMum = miniMum;
  handle->maxiMum = miniMum<=maxiMum?maxiMum:miniMum;
  handle->value = miniMum;
  handle->hSize = hSize;
  handle->vSize = 14;
  GUI_WidgetProgBar_SetVal(handle, value);
  handle->form = form;
  handle->isShowValue = isShowValue;
  
  return handle;
}


/**
  * @brief  value值设置
  * @param  handle:控件句柄
  * @retval GUI_State_EmTypedef
*/
GUI_State_EmTypedef GUI_WidgetProgBar_SetVal(WidgetProgBarHandle handle, float value)
{
  if(handle!=NULL)
  {
    if(value<handle->miniMum)
      handle->value = handle->miniMum;
    else if(value>handle->maxiMum)
      handle->value = handle->maxiMum;
    else
      handle->value = value;
    
    return GUI_SUCCESS;
  }
  else
  {
    return GUI_FAILED;
  }
}


/**
  * @brief  进度条控件显示
  * @param  handle:控件句柄
  * @param  isShow:是否显示控件
  * @param  isRefresh: not.不直接显示到oled;is.更新
  * @retval GUI_State_EmTypedef
*/
GUI_State_EmTypedef GUI_WidgetProgBar_Show(WidgetProgBarHandle handle, ISorNOT isShow, ISorNOT isRefresh)
{
  if(handle != NULL)
  {
    uint16_t x1 = handle->x + handle->hSize -1;
    uint16_t y1 = handle->y + handle->vSize -1;
    float percent = (handle->value-handle->miniMum)/(float)(handle->maxiMum-handle->miniMum);
    
    //清空该片区域
    GUI_DrawFullRect(handle->x,handle->y,x1,y1,NOT,isRefresh);
    if(isShow == IS)
    {
      if(handle->form == 0)
      {
        uint16_t width = handle->hSize - 2;
        uint16_t showX = (uint16_t)((float)(width-2)*percent);
        GUI_DrawFullRect(handle->x+1,handle->y+2,x1-1,handle->y+3,IS,isRefresh);
        GUI_DrawFullRect(handle->x+1+showX,handle->y+1,handle->x+2+showX,handle->y+4,IS,isRefresh);
      }
      else if(handle->form == 1)
      {
        uint16_t width = handle->hSize - 2;
        uint16_t showX = (uint16_t)((float)(width-2)*percent);
        GUI_DrawRect(handle->x+1,handle->y+1,x1-1,handle->y+4,IS,isRefresh);
        GUI_DrawFullRect(handle->x+1,handle->y+2,handle->x+1+showX,handle->y+3,IS,isRefresh);
      }
      
      if(handle->isShowValue == IS)
      {
        uint8_t* pText = OLED_TextPrint("*Val:%3.2f",handle->value);
        OLED_Print6x8Str(handle->x+1,handle->y+6,handle->hSize-2,8,pText,INV_OFF,isRefresh);
        free(pText);
      }
    }
    
    return GUI_SUCCESS;
  }
  else
  {
    return GUI_FAILED;
  }
}
/***************************************************************
  * 进度条控件 end
****************************************************************/


/***************************************************************
  * checkBox控件 begin
****************************************************************/
/**
  * @brief  创建checkbox控件
  * @param  x:左上角x
  * @param  y:左上角y
  * @param  hSize:水平宽度
  * @param  vSize:垂直高度(不开放)
  * @param  checked:是否选中
  * @param  pText:控件文本指针
  * @retval WidgetCheckBoxHandle:控件句柄
*/
WidgetCheckBoxHandle GUI_WidgetCheckBox_Create(uint16_t x, uint16_t y,\
      uint16_t hSize, TorF_EmTypeDef checked, void* pText)
{
  WidgetCheckBoxHandle handle;
  handle = malloc(sizeof(*handle));
  
  handle->x = x,
  handle->y = y,

  handle->hSize = hSize;
  handle->vSize = 12;
  handle->checked = checked;
  handle->pText = pText;
  
  return handle;
}

/**
  * @brief  checked值设置
  * @param  handle:控件句柄
  * @retval GUI_State_EmTypedef
*/
GUI_State_EmTypedef GUI_WidgetCheckBox_SetVal(WidgetCheckBoxHandle handle,TorF_EmTypeDef checked)
{
  if(handle!=NULL)
  {
    handle->checked = checked;
    return GUI_SUCCESS;
  }
  else
  {
    return GUI_FAILED;
  }
}

/**
  * @brief  textInv值设置
  * @param  handle:控件句柄
  * @retval GUI_State_EmTypedef
*/
GUI_State_EmTypedef GUI_WidgetCheckBox_SetTextInv(WidgetCheckBoxHandle handle,InvChoose textInv)
{
  if(handle!=NULL)
  {
    handle->textInv = textInv;
    return GUI_SUCCESS;
  }
  else
  {
    return GUI_FAILED;
  }
}

/**
  * @brief  checkbox控件显示
  * @param  handle:控件句柄
  * @param  isShow:是否显示
  * @param  isRefresh: not.不直接显示到oled;is.更新
  * @retval GUI_State_EmTypedef
*/
GUI_State_EmTypedef GUI_WidgetCheckBox_Show(WidgetCheckBoxHandle handle, ISorNOT isShow, ISorNOT isRefresh)
{
  if(handle != NULL)
  {
    uint16_t x1 = handle->x+handle->hSize-1;
    uint16_t y1 = handle->y+handle->vSize-1;
    GUI_DrawFullRect(handle->x,handle->y,x1,y1,NOT,isRefresh);
    if(isShow == IS)
    {
      GUI_DrawRect(handle->x+1,handle->y+1,handle->x+handle->vSize-2,y1-1,IS,isRefresh);
      if(handle->checked == IS)
      {
        GUI_DrawFullRect(handle->x+3,handle->y+3,handle->x+handle->vSize-4,y1-3,IS,isRefresh);
      }
      if(handle->textInv == INV_ON)
        GUI_DrawHLine(y1-9,handle->x+handle->vSize+1,x1,IS,isRefresh);
      
      OLED_Print6x8Str(handle->x+handle->vSize+1,y1-8,handle->hSize-3-(handle->vSize-2),8,\
              handle->pText,handle->textInv,isRefresh);
    }
    return GUI_SUCCESS;
  }
  else
  {
    return GUI_FAILED;
  }
}
/***************************************************************
  * checkBox控件 end
****************************************************************/


/***************************************************************
  * MessageBox控件 begin
****************************************************************/
//一个特殊的TextBox............................
/***************************************************************
  * MessageBox控件 end
****************************************************************/

/**
  * @brief  控件销毁
  * @param  widget:控件句柄
  * @retval None
*/
void GUI_WidgetDestroy(WidgetTypeDef* widget)
{
  free((void*)widget->handle);
  widget->handle = 0;
}

/**
  * @brief  控件更新显示
  * @param  widgets:控件句柄
  * @param  num:控件数量
  * @param  isRefresh: not.不直接显示到oled;is.更新
  * @retval GUI_State_EmTypedef
*/
void GUI_WidgetFresh(WidgetTypeDef* widgets, uint16_t num, ISorNOT isRefresh)
{
  WidgetTypeDef* pTmpWidget;
  for(uint16_t i=0;i<num;i++)
  {
    pTmpWidget = widgets+i;
    switch(pTmpWidget->type)
    {
      case WIDGET_IMAGE:
        GUI_WidgetImage_Show((WidgetImageHandle)pTmpWidget->handle, IS, isRefresh);
        break;
      case WIDGET_TEXT:
        GUI_WidgetText_Show((WidgetTextHandle)pTmpWidget->handle, IS, isRefresh);
        break;
      case WIDGET_PROGBAR:
        GUI_WidgetProgBar_Show((WidgetProgBarHandle)pTmpWidget->handle, IS, isRefresh);
        break;
      case WIDGET_CHECKBOX:
        GUI_WidgetCheckBox_Show((WidgetCheckBoxHandle)pTmpWidget->handle, IS, isRefresh);
        break;
    }
  }
}
