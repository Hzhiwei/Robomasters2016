
#include "Task_SysInitConfig.h"
#include "OSinclude.h"

#include "Handler.h"

#include "mpu9250dmp.h"

#include "BSP_CAN.h"
#include "BSP_TIM.h"
#include "BSP_DMA.h"
#include "BSP_SPI.h"
#include "BSP_GPIO.h"
#include "BSP_NVIC.h"
#include "BSP_UART.h"

#include "Driver_Bell.h"
#include "Driver_DBUS.h"
#include "Driver_Judge.h"
#include "Driver_Vision.h"
#include "Driver_Chassis.h"
#include "Driver_mpu9250.h"
#include "Driver_Control.h"
#include "Driver_Steering.h"
#include "Driver_PokeMotor.h"
#include "Driver_FricMotor.h"
#include "Driver_CloudMotor.h"
#include "Driver_StatusMachine.h"
#include "Driver_SuperGyroscope.h"

#include "Task_Bell.h"
#include "Task_Monitor.h"
#include "Task_CANSend.h"
#include "Task_Control.h"

#include "bsp_oled.h"
#include "drv_gui.h"

#include "Driver_ESP8266.h"
/**
  * @brief  系统初始化任务
  * @param  unused
  * @retval void
  */
  
  int kkk = 0, dddd = 3;;
  
  
void Task_SysInitConfig(void *Parameters)
{
    BSP_GPIO_InitConfig();
    BSP_CAN_InitConfig();
    BSP_TIM_InitConfig();
    BSP_UART_InitConfig();
    BSP_DMA_InitConfig();
    BSP_NVIC_InitConfig();
    
    Steering_InitConfig();
    Poke_InitConfig();
    Bell_InitConfig();
    DBUS_InitConfig();
    CloudMotor_InitConfig();
    ControlPID_InitConfig();
    StatusMachine_InitConfig();
    SuperGyo_InitConfig();
    Vision_InitConfig();
    MPU9250_InitConfig();
    FricMotor_InitConfig();
    Chassis_InitConfig();
    
    //延时保证陀螺仪正常初始化
    vTaskDelay(1000);
    MPU9250DMP_InitConfig(0);
    
#if USEESP8266orOLED == 1
    ESP8266_InitConfig();
#elif USEESP8266orOLED == 0
    OLED_Init();
    Oled_Handler = GUI_WidgetText_Create(0, 0, 78, 64);
    GUI_WidgetText_SetRim(Oled_Handler, IS);
    GUI_WidgetText_Show(Oled_Handler, IS, IS);
#if INFANTRY == 7
    OLED_Print6x8Str(80, 00, 42, 8, (uint8_t *)"Target:", INV_OFF, IS);
    OLED_Print6x8Str(100, 10, 30, 8, (uint8_t *)"RED ", INV_OFF, IS);
    OLED_Print6x8Str(80, 20, 42, 8, (uint8_t *)"PC    :", INV_OFF, IS);
    OLED_Print6x8Str(100, 30, 30, 8, (uint8_t *)" -  ", INV_OFF, IS);
#endif
#endif
    
    
    //等待底盘陀螺仪初始化完成
    vTaskDelay(1000); 
    
    //生成CAN发送队列
	Queue_CANSend = xQueueCreate(64, sizeof(CanSend_Type));	


    
    //////////////////////任务建立
    //监视器任务
    xTaskCreate(Task_Monitor,
                "Task_Monitor",
                512,
                NULL,
                3,
                NULL);
    
    //CAN发送任务
    xTaskCreate(Task_CANSend,
                "Task_CANSend",
                200,
                NULL,
                4,
                NULL);
    
    //控制任务
    xTaskCreate(Task_Control,
                "Task_Control",
                512,
                NULL,
                4,
                NULL);
                
    //蜂鸣器鸣叫任务
//	xTaskCreate(Task_BellBark,
//				"Task_BellCycle",
//				64,
//				NULL,
//				2,
//				NULL);
                
    DMA_Cmd(DMA1_Stream0, DISABLE);
    while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream0, DBUSLength + DBUSBackLength);
    DMA_Cmd(DMA1_Stream0, ENABLE);
    
    DMA_Cmd(DMA1_Stream2, DISABLE);
    while(DMA_GetCmdStatus(DMA1_Stream2) != DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream2, JudgeBufferLength);
    DMA_Cmd(DMA1_Stream2, ENABLE);
    
    Bell_BarkWarning(1, 1);

    vTaskDelay(500);
    
    vTaskDelete(NULL);
}
