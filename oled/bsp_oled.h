#ifndef __BSP_OLED_H
#define __BSP_OLED_H

#include "stm32f4xx.h"


/** 
  * @brief  Bmp structures definition
  */
typedef __packed struct __BmpInfo
{
  uint16_t width;
  uint16_t height;
  const uint8_t* pBmp;
}BmpInfoStructTypedDef;


/** 
  * @brief  翻转开关
  */
typedef enum
{
	INV_OFF = 0,
	INV_ON  = 1,
}InvChoose;


/** 
  * @brief  是 或 否
  */
typedef enum
{
	NOT = 0,
	IS  = 1,
}ISorNOT;



void OLED_SetPositionInPage(uint8_t x, uint8_t y);
void OLED_Init(void);


void OLED_DrawPixel(uint8_t x, uint8_t y, ISorNOT isShow, ISorNOT isRefresh);
void OLED_RefreshGram(void);
//void OLED_DrawBmp(uint16_t x, uint16_t y, BmpInfoStructTypedDef* bmpInfo, InvChoose InvSW);
void OLED_DrawSomeBits(int16_t x, int16_t y, uint8_t wrtData, uint8_t effBits, \
        InvChoose InvSW, ISorNOT isRefresh);
void OLED_DrawBmp(int16_t x, int16_t y, uint16_t showW, uint16_t showH, \
        BmpInfoStructTypedDef* bmpInfo, InvChoose InvSW);
void OLED_Print6x8Str(int16_t x,int16_t y,uint16_t showW,uint16_t showH,
        uint8_t* pWrtData,InvChoose InvSW,ISorNOT isRefresh);
uint8_t* OLED_TextPrint(void* fmt, ...);
void OLED_Clear(void);


#endif


///**
//  * @brief  在指定起点绘制图片(同时更新到显存)
//  * @param  x:左上角x
//  * @param  y:左上角y
//  * @param  bmp:图片信息
//  * @param  None
//*/
//void OLED_DrawBmp(uint16_t x, uint16_t y, BmpInfoStructTypedDef* bmpInfo, InvChoose InvSW)
//{
//  uint8_t firstBitExc = y%8;                    //第一行位偏移(0-7)
//  uint8_t endBitExc = (y+bmpInfo->height)%8+1;  //最后一行位偏移(0-7)
//  uint16_t firstPage = y/8;                     //真实的起始页地址
//  uint16_t endPage = (y+bmpInfo->height)/8;     //真实的结束页地址
//  uint16_t drawEndPage;                         //画的结束页
//  uint16_t xBeginPos = x;                       //起始x位置(0-127)
//  uint16_t xEndPos = x+bmpInfo->width-1;        //结束x位置(0-127)
//  
//  //限制范围
//  drawEndPage = endPage>=OLED_PAGE?(OLED_PAGE-1):endPage;
//  xEndPos = xEndPos>=OLED_WIDTH?(OLED_WIDTH-1):xEndPos;
//  
//  uint16_t pageP=firstPage,pageCnt=0;
//  uint32_t xP,xCnt;
//  uint8_t highBitsGet=0xff<<(8-firstBitExc);    //一般位置取高位
//  uint8_t lowBitsGet=0xff>>(firstBitExc);       //一般位置取低位
//  
//  int16_t tmp;
//  //第一行和最后一行是同一行的特殊处理
////  if(firstPage == endPage)
////  {
////    OLED_SetPositionInPage(xBeginPos,pageP);
////  }
////  else if(firstPage<endPage)
////  {
//    //第一行特殊处理
//    OLED_SetPositionInPage(xBeginPos,pageP);
//    for(xP=xBeginPos,xCnt=pageCnt*bmpInfo->width; xP<=xEndPos; xP++,xCnt++)
//    {
//      OLED_Gram[xP][pageP] = \
//          ((OLED_Gram[xP][pageP])&(0xff>>(8-firstBitExc))) | \
//          ((*(bmpInfo->pBmp+xCnt)&lowBitsGet)<<(firstBitExc));
//      OLED_WriteData(OLED_Gram[xP][pageP]);
//    }
//    pageP++;
//    pageCnt++;
//    //循环处理中间的
//    tmp = (endPage>drawEndPage)?drawEndPage:(drawEndPage-1);
//    for(; pageP<=tmp; pageP++,pageCnt++)
//    {
//      OLED_SetPositionInPage(xBeginPos,pageP);
//      for(xP=xBeginPos,xCnt=pageCnt*bmpInfo->width; xP<=xEndPos; xP++,xCnt++)
//      {
//        OLED_Gram[xP][pageP] = \
//          ((*(bmpInfo->pBmp+xCnt-bmpInfo->width)&highBitsGet)>>(8-firstBitExc)) | \
//          ((*(bmpInfo->pBmp+xCnt)&lowBitsGet)<<(firstBitExc));
//        OLED_WriteData(OLED_Gram[xP][pageP]);
//      }
//    }
//    //最后一行特殊处理（更加特殊一些）
//    if(tmp==drawEndPage-1)
//    {
//      OLED_SetPositionInPage(xBeginPos, pageP);
//      for(xP=xBeginPos,xCnt=pageCnt*bmpInfo->width; xP<=xEndPos; xP++,xCnt++)
//      {
//        OLED_Gram[xP][pageP] = \
//            (((*(bmpInfo->pBmp+xCnt-bmpInfo->width)&highBitsGet)>>(8-firstBitExc))&(0xff>>(8-endBitExc))) | \
//            ((OLED_Gram[xP][pageP])&(0xff<<endBitExc));
//        OLED_WriteData(OLED_Gram[xP][pageP]);
//      }
//    }
////  }
//}
