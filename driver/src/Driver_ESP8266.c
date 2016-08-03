#define __DRIVER_ESP8266_GLOBALS


#include "OSinclude.h"
#include "Driver_ESP8266.h"
#include "StringDecoding.h"


/**
  * @brief  ESP8266初始化
  * @param  void
  * @retval 1 初始化成功         0 初始化失败
  */
void ESP8266_InitConfig(void)
{
    
}


/**
  * @brief  向ESP8266发送一包数据(即ESP8266ControlTXBuffer[ESP8266ControlTXBufferLenght])
  * @param  数据量
  * @retval void
  */
void ESP8266_SendPack(uint16_t Num)
{
    DMA_Cmd(DMA1_Stream3, DISABLE);                                     //关闭 DMA 传输
    while (DMA_GetCmdStatus(DMA1_Stream3) != DISABLE){}                 //确保 DMA 可以被设置
    DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3 | DMA_FLAG_HTIF3);       //清空标志位
    DMA1_Stream3->M0AR = (uint32_t)ESP8266TXBuffer;              //设置数据
    DMA_SetCurrDataCounter(DMA1_Stream3, Num);                          //数据传输量
    DMA_Cmd(DMA1_Stream3, ENABLE);                                      //开启 DMA 传输
}


/**
  * @brief  ESP8266解码

 */



