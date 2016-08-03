#ifndef __DRIVER_ESP8266_H
#define __DRIVER_ESP8266_H


#include "OSinclude.h"
#include "stm32f4xx.h"


#define TargetAPSSID                    "TP-LINK_E98B7A"
#define ESP8266RXBufferLenght    128                     //ESP8266发送缓存长度
#define ESP8266TXBufferLenght    512                     //ESP8266接收缓存长度


#ifdef  __DRIVER_ESP8266_GLOBALS
#define __DRIVER_ESP8266_EXT
#else
#define __DRIVER_ESP8266_EXT extern
#endif




__DRIVER_ESP8266_EXT char ESP8266TXBuffer[ESP8266TXBufferLenght];         //ESP8266发送缓存
__DRIVER_ESP8266_EXT char ESP8266RXBuffer[ESP8266RXBufferLenght];         //ESP8266接收缓存

void ESP8266_InitConfig(void);
void ESP8266_SendPack(uint16_t Num);



#endif
