#ifndef __STRINGDECODING_H
#define __STRINGDECODING_H


#include "stm32f4xx.h"



int32_t SDEC_Lenght(char *Str);
char SDEC_GetChar(char *Str, int32_t Location);
void SDEC_GetString(char *Str, char *Target, char Location, int32_t Lenght);
int32_t SDEC_SearchChar(char *Str, char Target);
//int32_t SDEC_SearchString(char *Str, char *Target);
int8_t SDEC_StringIsEqual(char *Str1, char *Str2);
uint8_t SDEC_ChildStringIsEqual(char *Str1, uint16_t Offset1, char *Str2, uint16_t Offset2, uint16_t Lenght);
int32_t SDEC_SearchMultiChar(char *Str, uint32_t *Loc, char Target);





#endif
