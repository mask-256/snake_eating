#include "tim.h"

TIM_CALLBACK tim_callback=0;
u8 Enter_Interrupt_flag=0;

void TIM6_Config(u16 psc, u16 arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
	TIM_TimeBaseInitStruct.TIM_Period = arr;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
	TIM_ARRPreloadConfig(TIM6, ENABLE);
	TIM_GenerateEvent(TIM6, TIM_EventSource_Update);

	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM6, ENABLE);

}

void TIM6_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	
	Enter_Interrupt_flag = 1;
	GPIOA->ODR = ~(GPIOA->ODR>>8)<<8;
}

