
#include "sim_IIC.h"


//SCL:E7
//SDA:E8


#define IIC_DELAY IIC_Delay(15);


/**
  * @brief  模拟IIC初始化
  * @note   SCL:E7		SDA:E8
  * @param  void
  * @retval void
  */
void IIC_Init(void)
{
	;
}


/**
  * @brief  IIC向指定设备指定寄存器写入一个byte
  * @param  目标设备地址
  * @param  寄存器地址
  * @param  要写入的数据
  * @retval 0 成功		1 失败
  */
uint8_t IIC_SingleSend(uint8_t addr, uint8_t reg, uint8_t data)
{
	if(IIC_Start())
	{
		return 1;
	}
	IIC_SendByte(addr << 1);
	if(IIC_WaitACK())
	{
		IIC_Stop();
		return 1;
	}
	IIC_SendByte(reg);
	IIC_WaitACK();
	IIC_SendByte(data);
	IIC_WaitACK();
	IIC_Stop();
	return 0;
}


/**
  * @brief  IIC从指定设备指定寄存器读取一个byte
  * @param  目标设备地址
  * @param  寄存器地址
  * @param  读出数据存放地址
  * @retval 0 成功		1 失败
  */
uint8_t IIC_SingleRead(uint8_t addr, uint8_t reg, uint8_t *data)
{
	if(IIC_Start())
	{
		return 1;
	}
	IIC_SendByte(addr << 1);
	if(IIC_WaitACK())
	{
		IIC_Stop();
		return 1;
	}
	IIC_SendByte(reg);
	IIC_WaitACK();
	IIC_Start();
	IIC_SendByte(addr << 1 | 0x01);
	IIC_WaitACK();
	*data = IIC_ReadByte();
	IIC_NACK();
	IIC_Stop();
	return 0;
	
}


/**
  * @brief  IIC向设备的某一个地址写入指定长度的数据
  * @param  设备地址
  * @param  寄存器地址
  * @param  数据长度
  * @param  要传输的数据内存地址
  * @retval 0 成功		1 失败
  */
uint8_t IIC_SendBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
	uint8_t i;
	
	if(IIC_Start())	//启动失败
	{
		return 1;
	}
	IIC_SendByte(addr << 1);
	if(IIC_WaitACK())
	{
		IIC_Stop();
		return 1;
	}
	IIC_SendByte(reg);
	IIC_WaitACK();
	for(i = 0; i < len; i++)
	{
		IIC_SendByte(data[i]);
		if(IIC_WaitACK())
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_Stop();
	return 0;
}


/**
  * @brief  IIC从设备的某一个地址读取指定长度的数据
  * @param  设备地址
  * @param  寄存器地址
  * @param  数据长度
  * @param  存取读取的数据的地址
  * @retval 0 成功		1 失败
  */
uint8_t IIC_ReadBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
	if(IIC_Start())
	{
		return 1;
	}
	IIC_SendByte(addr << 1);
	if(IIC_WaitACK())
	{
		IIC_Stop();
		return 1;
	}
	IIC_SendByte(reg);
	IIC_WaitACK();
	IIC_Start();
	IIC_SendByte(addr << 1 | 0x01);
	IIC_WaitACK();
	while(len)
	{
		*data = IIC_ReadByte();
		if(len == 1)
		{
			IIC_NACK();
		}
		else
		{
			IIC_ACK();
		}
		data++;
		len--;
	}
	IIC_Stop();
	return 0;
}


//用于us级延时
void IIC_Delay(u32 time)
{
	while(time--);
}


/**
  * @brief  产生IIC起始信号
  * @param  void
  * @retval 0 启动成功		1 启动失败
  */
uint8_t IIC_Start(void)
{
	IIC_SDA_H;
	IIC_SCL_H;
	IIC_DELAY;
	if(!IIC_SDA_Read)
	{
		return 1;
	}
	IIC_SDA_L;
	IIC_DELAY;
	if(IIC_SDA_Read)
	{
		return 1;
	}
	IIC_DELAY;
	return 0;
}


/**
  * @brief  产生IIC停止信号
  * @param  void
  * @retval void 
  */
void IIC_Stop(void)
{
	IIC_SCL_L;
	IIC_DELAY;
	IIC_SDA_L;
	IIC_DELAY;
	IIC_SCL_H;
	IIC_DELAY;
	IIC_SDA_H;
	IIC_DELAY;
}


/**
  * @brief  产生IIC ACK信号
  * @param  void
  * @retval void 
  */
void IIC_ACK(void)
{
	IIC_SCL_L;
	IIC_DELAY;
	IIC_SDA_L;
	IIC_DELAY;
	IIC_SCL_H;
	IIC_DELAY;
	IIC_SCL_L;
	IIC_DELAY;
}


/**
  * @brief  产生IIC NACK信号
  * @param  void
  * @retval void 
  */
void IIC_NACK(void)
{
	IIC_SCL_L;
	IIC_DELAY;
	IIC_SDA_H;
	IIC_DELAY;
	IIC_SCL_H;
	IIC_DELAY;
	IIC_SCL_L;
	IIC_DELAY;
}


/**
  * @brief  等待IIC NACK信号
  * @param  void
  * @retval 0 接收ACK成功		1 接收ACK失败 
  */
uint8_t IIC_WaitACK(void)
{
	IIC_SCL_L;
	IIC_DELAY;
	IIC_SDA_H;
	IIC_DELAY;
	IIC_SCL_H;
	IIC_DELAY;
	if(IIC_SDA_Read)
	{
		IIC_SCL_L;
		return 1;
	}
	IIC_SCL_L;
	return 0;
}


/**
  * @brief  IIC发送一个byte
  * @param  要发送的byte
  * @retval void
  */
void IIC_SendByte(uint8_t byte)
{
	uint8_t i = 8;
	while(i--)
	{
		IIC_SCL_L;
		IIC_DELAY;
		if(byte & 0x80)
		{
			IIC_SDA_H;
		}
		else
		{
			IIC_SDA_L;
		}
		byte <<= 1;
		IIC_DELAY;
		IIC_SCL_H;
		IIC_DELAY;
	}
	IIC_SCL_L;
}


/**
  * @brief  IIC接收一个byte
  * @param  void
  * @retval 接收到的byte
  */
uint8_t IIC_ReadByte(void)
{
	uint8_t i = 8;
	uint8_t byte = 0;
	
	IIC_SDA_H;
	while(i--)
	{
		byte <<= 1;
		IIC_SCL_L;
		IIC_DELAY;
		IIC_SCL_H;
		IIC_DELAY;
		if(IIC_SDA_Read)
		{
			byte |= 0x01;
		}
	}
	IIC_SCL_L;
	return byte;
}







