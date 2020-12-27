#ifndef __TIM_H
#define __TIM_H
#include "stm32f10x.h"

typedef void (* TIM_CALLBACK)(u8 *flag);

void TIM6_Config(u16 psc, u16 arr);
extern u8 Enter_Interrupt_flag;
 
#endif
