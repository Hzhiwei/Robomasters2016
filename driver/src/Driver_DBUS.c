

#define  __DBUS_GLOBALS

#include "Driver_DBUS.h"


#define ClearShackTick      50      //消抖时长
    
    

/**
  * @brief  DBUS初始化
  * @param  void
  * @retval void
  */
void DBUS_InitConfig(void)
{
	DBUSConnectStatus = Lost;
	DBUSFrameRate = 0;
	DBUSFrameCounter = 0;
	DBUS_ReceiveData.ch1 = 0;
	DBUS_ReceiveData.ch2 = 0;
	DBUS_ReceiveData.ch3 = 0;
	DBUS_ReceiveData.ch4 = 0;
    DBUS_ReceiveData.keyBoard.key_code = 0;
    DBUS_ReceiveData.keyBoard.jumpkey_code = 0;
}


/**
  * @brief  DBUS数据解码
  * @param  void
  * @retval void
  */
void DBUS_DataDecoding(void)
{
	LASTDBUS_ReceiveData = DBUS_ReceiveData;
	
	DBUS_ReceiveData.ch1 = (DBUSBuffer[0] | DBUSBuffer[1]<<8) & 0x07FF;
	DBUS_ReceiveData.ch1 -= 1024;
	DBUS_ReceiveData.ch2 = (DBUSBuffer[1]>>3 | DBUSBuffer[2]<<5 ) & 0x07FF;
	DBUS_ReceiveData.ch2 -= 1024;
	DBUS_ReceiveData.ch3 = (DBUSBuffer[2]>>6 | DBUSBuffer[3]<<2 | DBUSBuffer[4]<<10) & 0x07FF;
	DBUS_ReceiveData.ch3 -= 1024;
	DBUS_ReceiveData.ch4 = (DBUSBuffer[4]>>1 | DBUSBuffer[5]<<7) & 0x07FF;		
	DBUS_ReceiveData.ch4 -= 1024;
	
	DBUS_ReceiveData.switch_left = ( (DBUSBuffer[5] >> 4)& 0x000C ) >> 2;
	DBUS_ReceiveData.switch_right =  (DBUSBuffer[5] >> 4)& 0x0003 ;
	
	DBUS_ReceiveData.mouse.x = DBUSBuffer[6] | (DBUSBuffer[7] << 8);	//x axis
	DBUS_ReceiveData.mouse.y = DBUSBuffer[8] | (DBUSBuffer[9] << 8);
	DBUS_ReceiveData.mouse.z = DBUSBuffer[10]| (DBUSBuffer[11] << 8);
	
	DBUS_ReceiveData.mouse.press_left 	= DBUSBuffer[12];	// is pressed?
	DBUS_ReceiveData.mouse.press_right 	= DBUSBuffer[13];
	
	DBUS_ReceiveData.keyBoard.key_code 	= DBUSBuffer[14] | DBUSBuffer[15] << 8; //key borad code
}


/**
  * @brief  按键跳变检测
  * @param  当前系统时间
  * @retval void
  */
void DBUS_ButtonCheckJump(portTickType CurrentTick)
{
    uint8_t index;
    
    //上次对应按键被按下时的系统时间
    static portTickType LastJumpTick[15] = {0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0};
    
    for(index = 0; index < 15; index++)
    {
        if(CurrentTick - LastJumpTick[index] >= ClearShackTick)
        {
            if((DBUS_ReceiveData.keyBoard.key_code & (1 << index)) && (!(LASTDBUS_ReceiveData.keyBoard.key_code & (1 << index))))
            {
                DBUS_ReceiveData.keyBoard.jumpkey_code |= 1 << index;
                LastJumpTick[index] = CurrentTick;
            }
        }
        else
        {
            DBUS_ReceiveData.keyBoard.jumpkey_code &= ~(1 << index);
        }
    }
}


/**
  * @brief  判断一个键是否被按下
  * @param  要进行判断的按键，字符大写
  * @retval 1 按下        0 未按下
  */
uint8_t DBUS_CheckPush(uint16_t Key)
{
    if(DBUS_ReceiveData.keyBoard.key_code & Key)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/**
  * @brief  判断一个键是否发生按下跳变
  * @param  要进行判断的按键，字符大写
  * @retval 1 按下        0 未按下
  */
uint8_t DBUS_CheckJump(uint16_t Key)
{
    if(DBUS_ReceiveData.keyBoard.jumpkey_code & Key)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}




