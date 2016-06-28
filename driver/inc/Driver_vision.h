
#ifndef __DRIVER_VISION_H
#define __DRIVER_VISION_H


#include "stm32f4xx.h"


#define PCDATALENGTH            26  //֡��
#define ENEMYDATABUFFERLENGHT   30 //��������֡
#define FITDATANUMMIN           8   //��С�������������


#ifdef  __DRIVER_VISION_GLOBALS
#define __DRIVER_VISION_EXT 
#else
#define __DRIVER_VISION_EXT extern
#endif


//�Ӿ����ݽṹ
typedef struct
{
    float X;
    float Y;
    float Z;
    float TimeStamp;
    int Time;
    char ID;
}Enemy_Struct;

//��ʽת��������
typedef union
{
    uint8_t u8[4];
    float F;
    int I;
}FormatTrans;

//�����ݽṹ
typedef struct
{
    float X;
    float Y;
    float Z;
}Point_Struct;

//�����ݽṹ(����������ʵ�ʽǶȣ�
typedef struct
{
    float H;
    float V;
}AngleF_Struct;

//�����ݽṹ(�������ڱ������Ƕȣ�
typedef struct
{
    int16_t H;
    int16_t V;
}AngleI_Struct;

//�ٶȲ����ٶ�
typedef struct
{
    float X;
    float Y;
    float Z;
}CompensateSpeed_struct;


//����ԭʼ���ݻ���
__DRIVER_VISION_EXT uint8_t PCDataBuffer[PCDATALENGTH];
//����ԭʼ���ݻ���ָ��
__DRIVER_VISION_EXT uint8_t PCDataBufferPoint;
//�����������ݻ���
__DRIVER_VISION_EXT Enemy_Struct EnemyDataBuffer[ENEMYDATABUFFERLENGHT];
//�����������ݻ���ָ��
__DRIVER_VISION_EXT uint8_t EnemyDataBufferPoint;


//֡��ͳ��
__DRIVER_VISION_EXT int16_t PCFrameCounter;
//֡��ͳ��
__DRIVER_VISION_EXT int16_t PCFrameRate;


void Vision_InitConfig(void);
AngleI_Struct RecToPolar(float X, float Y, float Z, uint8_t mode);
uint8_t ForcastCore(uint16_t SampleTime, uint16_t ForcastTime, Point_Struct *ForcastPoint);
CompensateSpeed_struct Get_EnemyCompensate(void);
uint8_t ForcastOnce(uint16_t SampleTime, uint16_t ForcastTime, AngleI_Struct *ForcastAngle, uint8_t TimeMode);


#endif