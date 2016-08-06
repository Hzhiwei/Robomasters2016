#define __DRIVER_ESP8266_GLOBALS


#include "OSinclude.h"
#include "Task_ESP8266.h"
#include "Driver_ESP8266.h"
#include "StringDecoding.h"
#include <String.h>


/**
  * @brief  ESP8266初始化
  * @param  void
  * @retval 1 初始化成功         0 初始化失败
  */
void ESP8266_InitConfig(void)
{
    LatestRespond = 0;
    
    ESP8266APPortLinkFlag = 0;
    
    xTaskCreate(Task_SetupConnection,
                "Task_SetupConnection",
                256,
                NULL,
                6,
                NULL);
                
}


/**
  * @brief  向ESP8266发送一包数据(即ESP8266ControlTXBuffer[ESP8266ControlTXBufferLenght])
  * @param  数据量
  * @retval void
  */
void ESP8266_SendPack(uint16_t Num)
{
    if(DMA1_Stream3->NDTR)
    {
        return;
    }
    DMA_Cmd(DMA1_Stream3, DISABLE);                                     //关闭 DMA 传输
    while (DMA_GetCmdStatus(DMA1_Stream3) != DISABLE){}                 //确保 DMA 可以被设置
    DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3 | DMA_FLAG_HTIF3);       //清空标志位
    DMA1_Stream3->M0AR = (uint32_t)ESP8266TXBuffer;              //设置数据
    DMA_SetCurrDataCounter(DMA1_Stream3, Num);                          //数据传输量
    DMA_Cmd(DMA1_Stream3, ENABLE);                                      //开启 DMA 传输
}


/**
  * @brief  等待响应数据
  * @param  超时时间
  * @param  检测时间
  * @retval responseOK  收到数据        responseNO  直到超时还未收到数据
  */
responseType ESP8266_WaitFordata(uint32_t Tick, uint32_t Crack)
{
    portTickType StartTick = xTaskGetTickCount();
    
    LatestRespond = 0;
    
    while(xTaskGetTickCount() - StartTick < Tick)
    {
        if(LatestRespond)
        {
            return responseOK;
        }
        else
        {
            vTaskDelay(Crack);
        }
    }
    return responseNO;
    
}



/**
  * @brief  模块复位
  * @param  void
  * @retval 0 复位失败       1 复位成功
  */
uint8_t ESP8266_Reset(void)
{
    strcpy(ESP8266TXBuffer, "AT+RST\n");
    ESP8266_SendPack(7);
    
    //未收到响应
    if(ESP8266_WaitFordata(3000, 50) == responseNO)
    {
        return 0;
    }
    
    //收到响应，确认是否是OK
    if(SDEC_StringIsEqual(ESP8266RXBuffer, "OK") == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


/**
  * @brief  设置为指定模式
  * @param  1 Station模式     2 AP模式     3 AP+Station模式
  * @retval 0 设置失败      1 设置成功
  */
uint8_t  ESP8266_SetMode(uint8_t mode)
{
    if(mode == 1)
    {
        strcpy(ESP8266TXBuffer, "AT+CWMODE=1\r\n");
    }
    else if(mode == 2)
    {
        strcpy(ESP8266TXBuffer, "AT+CWMODE=2\r\n");
    }
    else
    {
        strcpy(ESP8266TXBuffer, "AT+CWMODE=3\r\n");
    }
    
    ESP8266_SendPack(11);
    
    //未收到响应
    if(ESP8266_WaitFordata(1000, 50) == responseNO)
    {
        return 0;
    }
    
    //收到响应，确认是否成功
    if(mode == 1)
    {
        if(*(ESP8266RXBuffer + 10) != '1')
        {
            return 0;
        }
    }
    else if(mode == 2)
    {
        if(*(ESP8266RXBuffer + 10) != '2')
        {
            return 0;
        }
    }
    else
    {
        if(*(ESP8266RXBuffer + 10) != '3')
        {
            return 0;
        }
    }
    
    return 1;
}


/**
  * @brief  搜寻可用AP列表
  * @param  存储APP列表的数组首地址
  * @param  存储各WiFi名在列表里的偏移的数组首地址（第一个数据为AP数量）
  * @retval 搜索到的AP数量
  */
uint16_t ESP8266_SearchAP(char *List, uint16_t *Offset)
{
    strcpy(ESP8266TXBuffer, "AT+CWLAP\r\n");
    ESP8266_SendPack(10);
    
    //未收到响应
    if(ESP8266_WaitFordata(10000, 50) == responseNO)
    {
        return 0;
    }
    
    //未收到响应
    if(ESP8266_WaitFordata(20000, 50) == responseNO)
    {
        return 0;
    }
    
#warning decoding is not finished!!!
    
    return 1;
}


/**
  * @brief  加入指定AP
  * @param  指定AP的SSID
  * @param  密码
  * @param  超时时间
  * @retval 0 加入失败      1 加入成功
  */
uint8_t ESP8266_JoinAP(char *SSID, char *Password, uint16_t Tick)
{
    char Str[128] = "AT+CWJAP=\"";
    uint16_t Lenght = 0;
    
    strcat(Str, SSID);
    Lenght += SDEC_Lenght(SSID);
    strcat(Str, "\",\"");
    strcat(Str, Password);
    Lenght += SDEC_Lenght(Password);
    strcat(Str, "\"\r\n");
    Lenght += 16;
    
    strcpy(ESP8266TXBuffer, Str);
    ESP8266_SendPack(Lenght);
    
    //未收到响应
    if(ESP8266_WaitFordata(5000, 100) == responseNO)
    {
        return 0;
    }
    
    if(SDEC_ChildStringIsEqual(ESP8266RXBuffer, 45, "ERROR", 0, 5))
    {
        return 0;
    }
    
    //未收到响应
    if(ESP8266_WaitFordata(20000, 100) == responseNO)
    {
        return 0;
    }
    
    //未收到响应
    if(ESP8266_WaitFordata(20000, 100) == responseNO)
    {
        return 0;
    }
    
    //未收到响应
    if(ESP8266_WaitFordata(20000, 100) == responseNO)
    {
        return 0;
    }
    
    if(SDEC_ChildStringIsEqual(ESP8266RXBuffer, 2, "OK", 0, 2))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/**
  * @brief  连接指定IP
  * @param  协议  1 “TCP”     0“UDP”
  * @param  IP地址
  * @param  端口
  * @retval 0 失败      1 成功
  */
uint8_t ESP8266_ConnectServerPort(uint8_t  protocol, char *IP, char *port)
{
    char Str[128] = "AT+CIPSTART=\"";
    uint16_t Lenght = 0;

//发送字符串连接
    if(protocol)
    {
        strcat(Str, "TCP");
    }
    else
    {
        strcat(Str, "UDP");
    }
    
    strcat(Str, "\",\"");
    
    strcat(Str, IP);
    Lenght += SDEC_Lenght(IP);
    
    strcat(Str, "\",");
    strcat(Str, port);
    
    Lenght += SDEC_Lenght(port);
    strcat(Str, "\r\n");
    
    Lenght += 25;
    
//发送
    strcpy(ESP8266TXBuffer, Str);
    ESP8266_SendPack(Lenght);
    
    //未收到响应
    if(ESP8266_WaitFordata(20000, 50) == responseNO)
    {
        return 0;
    }
    
    //判断连接结果
    if((SDEC_ChildStringIsEqual(ESP8266RXBuffer, 0, "CONNECT", 0, 7)) || (SDEC_ChildStringIsEqual(ESP8266RXBuffer, 41, "ALREADY CONNECTED", 0, 17)))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/**
  * @brief  进入串口透传模式,并启动透传
  * @param  void
  * @retval 0 进入失败      1 进入成功
  */
uint8_t ESP8266_StartTransparentMode(void)
{
    strcpy(ESP8266TXBuffer, "AT+CIPMODE=1\r\n");
    ESP8266_SendPack(14);
    
    //响应1
    if(ESP8266_WaitFordata(2000, 50) == responseNO)
    {
        return 0;
    }
    
    //响应判断
    if(!SDEC_ChildStringIsEqual(ESP8266RXBuffer, 17, "OK", 0, 2))
    {
        return 0;
    }
    
    strcpy(ESP8266TXBuffer, "AT+CIPSEND\r\n");
    ESP8266_SendPack(12);
    
    //响应1
    if(ESP8266_WaitFordata(2000, 50) == responseNO)
    {
        return 0;
    }
    
    //错误响应
    if(!SDEC_ChildStringIsEqual(ESP8266RXBuffer, 15, "OK", 0, 2))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


/**
  * @brief  透传模式下发送数据
  * @param  待发送数据
  * @param  数据起始偏移
  * @param  数据量（-1为发送至'\0'）
  * @retval void
  */
void ESP8266_SendTransparentData(uint8_t *data, uint16_t Offset, int8_t Lenght)
{
    char *BufferPoint = ESP8266TXBuffer;
    data += Offset;
    uint16_t index;
    
    //复制数据至发送缓存
    if(Lenght < 0)
    {
        Lenght = 0;
        while(*data)
        {
            *BufferPoint = *data;
            Lenght++;
            data++;
            BufferPoint++;
        }
    }
    else
    {
        for(index = 0; index < Lenght; index++)
        {
            *BufferPoint = *data;
            data++;
            BufferPoint++;
        }
    }
    
    ESP8266_SendPack(Lenght);
}


/**
  * @brief  退出透传模式
  * @param  void
  * @retval void
  */
void ESP8266_ExitTransparentMode(void)
{
    strcpy(ESP8266TXBuffer, "+++\r\n");
    ESP8266_SendPack(5);
}







