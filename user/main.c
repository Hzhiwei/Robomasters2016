
#define __HANDLER_GLOBALS

#include "main.h"
#include "OSinclude.h"
#include "Handler.h"

#include "Task_SysInitConfig.h"

/**
  * @brief  Ö÷º¯Êý
  * @param  void
  * @retval void
  */
int main(void)
{
    xTaskCreate(Task_SysInitConfig,
                "Test",
                1600,
                NULL,
                3,
                NULL);
	
    vTaskStartScheduler();
    
    while(1);
}

