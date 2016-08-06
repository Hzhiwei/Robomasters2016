

#include "StringDecoding.h"


/**
  * @brief  字符串长度计算
  * @brief  字符串首地址
  * @retval 字符串长度
  */
int32_t SDEC_Lenght(char *Str)
{
    int32_t Lenght = 0;
    
    for(;;)
    {
        if(*Str)
        {
            Str++;
            Lenght++;
        }
        else
        {
            break;
        }
    }
    return Lenght;
}


/**
  * @brief  返回字符串指定位置的字符
  * @brief  字符串首地址
  * @param  位置
  * @retval 字符
  */
char SDEC_GetChar(char *Str, int32_t Location)
{
    return *(Str + Location);
}


/**
  * @brief  返回字符串指定位置指定长度字符串
  * @brief  字符串首地址
  * @brief  目标字符串存储首地址
  * @param  位置
  * @retval void
  */
void SDEC_GetString(char *Str, char *Target, char Location, int32_t Lenght)
{
    int32_t index;
    
    for(Str += Location, index = 0; index < Lenght; index++, Str++, Target++)
    {
        *Target = *Str;
    }
    *Target = 0;
}



/**
  * @brief  返回指定字符第一次出现的位置
  * @param  待搜寻的字符串
  * @param  目标字符
  * @retval 位置 (负数表示不存在)
  */
int32_t SDEC_SearchChar(char *Str, char Target)
{
    int32_t Location;
    
    for(Location = 0; *Str != 0; Str++, Location++)
    {
        if(*Str == Target)
        {
            return Location;
        }
    }
    
    return -1;
}



/**
  * @brief  返回指定字符串第一次出现的位置
  * @param  待搜寻的字符串
  * @param  目标字符
  * @retval 位置 (负数表示不存在)
  */
int32_t SDEC_SearchString(char *Str, char *Target)
{
//    int32_t index = 0, Location;
//    
//    for(Location = 0;*Str != 0;Str++, Location++)
//    {
//        if(*Str == *Target)
//        {
//            return index;
//        }
//    }
    
    return -1;
}


/**
  * @brief  判断字符串是否相等
  * @param  字符串1
  * @param  字符串2
  * @retval 1 相等     0 不相等
  */
int8_t SDEC_StringIsEqual(char *Str1, char *Str2)
{
    while((*Str1 != 0) && (*Str2 != 0))
    {
        if(*Str1 != *Str2)
        {
            return 0;
        }
        Str1++; 
        Str2++;
    }
    
    if((*Str1 == 0) && (*Str2 == 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/**
  * @brief  比较两个字符串指定位置指定长度子字符串是否相等
  * @param  字符串1地址
  * @param  字符串1偏移
  * @param  字符串2地址
  * @param  字符串2偏移
  * @param  待比较部分长度
  * @retval 0 不相等           1 相等
  */
uint8_t SDEC_ChildStringIsEqual(char *Str1, uint16_t Offset1, char *Str2, uint16_t Offset2, uint16_t Lenght)
{
    uint16_t index;
    
    Str1 += Offset1;
    Str2 += Offset2;
    
    for(index = 0; index < Lenght; index++, Str1++, Str2++)
    {
        if(*Str1 != *Str2)
        {
            return 0;
        }
    }
    return 1;
}



/**
  * @brief  指定字符歌词出现的位置
  * @param  字符串首地址
  * @param  用于存储位置数组的地址
  * @param  目标字符
  * @retval 出现次数
  */
int32_t SDEC_SearchMultiChar(char *Str, uint32_t *Loc, char Target)
{
    int32_t Num = 0, Location = 0;
    
    while(*Str)
    {
        if(*Str == Target)
        {
            Loc[Num] = Location;
            Num++;
        }
        Location++;
        Str++;
    }
    return Num;
}









