#ifndef __COMMONDATASTRUCTURE_H
#define __COMMONDATASTRUCTURE_H

/******************************************************
此文件为常用数据结构
******************************************************/

#include "stm32f4xx.h"


//格式转换联合体
typedef union
{
    uint8_t U[4];
    float F;
    int I;
}FormatTrans;



#endif
