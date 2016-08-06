
#include "OSinclude.h"
#include "Task_ESP8266.h"
#include "Driver_ESP8266.h"

/**
  * @brief  与指定AP建立连接
  * @param  指定AP的SSID
  * @retval void
  */
void Task_SetupConnection(void *Parameters)
{
    ESP8266_ExitTransparentMode();
    
//    while(!ESP8266_SetMode(1))
//    {
//        vTaskDelay(500);
//    }
//    
//    while(!ESP8266_JoinAP(TargetAPSSID, TargetAPPassword, 20000))
//    {
//        vTaskDelay(3000);
//    }
    
    while(!ESP8266_ConnectServerPort(1, "192.168.1.125", "6000"))
    {
        vTaskDelay(1000);
    }
    
    while(!ESP8266_StartTransparentMode())
    {
        vTaskDelay(1000);
    }
    
    ESP8266APPortLinkFlag = 1;
    
    vTaskDelete(NULL);
}


















