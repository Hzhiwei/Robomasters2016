#ifndef __DRIVER_ESP8266_H
#define __DRIVER_ESP8266_H


#include "OSinclude.h"
#include "stm32f4xx.h"


#define TargetAPSSID                "TP-LINK_E98B7A"
#define TargetAPPassword            "kaixinjiuhao"


#define ESP8266RXBufferLenght       512                     //ESP8266发送缓存长度
#define ESP8266TXBufferLenght       512                     //ESP8266接收缓存长度
#define ESP8266APListLenght         512                     //可连接AP列表
#define ESP8266APListOffsetLenght   64                      //AP列表各SSID偏移



#ifdef  __DRIVER_ESP8266_GLOBALS
#define __DRIVER_ESP8266_EXT
#else
#define __DRIVER_ESP8266_EXT extern
#endif


//收到数据响应枚举
typedef enum
{
    responseNO,
    responseOK
}responseType;


__DRIVER_ESP8266_EXT uint8_t ESP8266APPortLinkFlag;                             //透传连接成功标志  0 连接未成功         1 连接成功
__DRIVER_ESP8266_EXT char ESP8266TXBuffer[ESP8266TXBufferLenght];               //ESP8266发送缓存
__DRIVER_ESP8266_EXT char ESP8266RXBuffer[ESP8266RXBufferLenght];               //ESP8266接收缓存
__DRIVER_ESP8266_EXT uint8_t LatestRespond;                                     //ESP8266，获得最新数据置1
__DRIVER_ESP8266_EXT char ESP8266APList[ESP8266APListLenght];                   //ESP8266可连接AP列表
__DRIVER_ESP8266_EXT uint16_t ESP8266APListOffset[ESP8266APListOffsetLenght];   //可连接AP列表中各SSID偏移位置，第一个字节为可连接AP数量，最后一个字节为列表终止位置

void ESP8266_InitConfig(void);
responseType ESP8266_WaitFordata(uint32_t Tick, uint32_t Crack);
uint8_t ESP8266_Reset(void);
uint8_t  ESP8266_SetMode(uint8_t mode);
void ESP8266_SendPack(uint16_t Num);
uint16_t ESP8266_SearchAP(char *List, uint16_t *Offset);
uint8_t ESP8266_JoinAP(char *SSID, char *Password, uint16_t Tick);
uint8_t ESP8266_ConnectServerPort(uint8_t  protocol, char *IP, char *port);
uint8_t ESP8266_StartTransparentMode(void);
void ESP8266_SendTransparentData(uint8_t *data, uint16_t Offset, int8_t Lenght);
void ESP8266_ExitTransparentMode(void);



#endif
