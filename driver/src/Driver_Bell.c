
#include "Driver_bell.h"
#include "Task_Bell.h"

#include "Handler.h"
#include "OSinclude.h"


/**
  * @brief  ·äÃùÆ÷³õÊ¼»¯
  * @param  void
  * @retval void
  */
void Bell_InitConfig(void)
{
    TIM2->CCR1 = 0;
    
    BellWarningNum = 0;
    BellWarningCount = 0;
}


/**
  * @brief  ·äÃùÆ÷°´Ö¸¶¨ÐòÁÐÑ­»·Ãù½Ð
  * @param  Ãù½ÐÐòÁÐ
  * @retval void
  */
void Bell_BarkWarning(uint8_t Serial, uint16_t Count)
{
    BellWarningNum = Serial;
    BellWarningCount = Count;
}


/**
  * @brief  ·äÃùÆ÷×´Ì¬¿ØÖÆ
  * @param  0Í£Ö¹		elseÃù½Ð
  * @retval void
  */
void Bell_Bark(uint8_t bark)
{
    if(bark == '0')
    {
        TIM2->CCR1 = 0;
    }
    else
    {
        TIM2->CCR1 = 150;
    }
}
