#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
//#include "lcd.h"
//#include "button.h"
#include "touch.h"

//#include "touch_scanf.h"
#include "list.h"
#include "tim.h"
#include "key.h"



#if 1

#include "stdlib.h"
#include "key.h"


#define MAX_SCORE 5

SnakeHead_Info snakehead_info;
Snake_egg snake_egg;
extern Cell_Info cell_info;

static u8 program_step=0;
static u8 score_count=0;

void NVIC_Config(void);
void BSP_init(void);
void Prepare_Start_Game(void);

int main(void)
{
	LED_Init();
	BUTTON button = BUTTON_SIZE_DEFAULT;

	u8 tim_trigger=0;
	u8 key_num=0;
	u8 level = 1;

	BSP_init();
	Prepare_Start_Game();

	while(1)
	{
		switch(program_step)
		{
			case 0:		//背景初始化
				program_step = 1;
				score_count = 0;
				if(level >= 5)
				{
					LCD_ShowString(50, 100, 100, 50, 16, "Congratulations");
					while(1);
				}
				LCD_Display_level(level);
				LCD_Fill(0, 0, 240, 155, WHITE);
				delay_ms(200);
				Background_Display(5, 5, 240-level*25, 150);
				Snake_Init(&snakehead_info);
				Obstacle_Init(level+1, cell_info);
				Obstacle_Display();
				
				break;
			case 1:
				program_step = 2;

				LCD_Display_Score(score_count);
				Snake_Link_Generate(&snake_egg);
				Cell_fill(snake_egg.x, snake_egg.y, RED);
				break;
			case 2:
//				tim_trigger = Enter_Interrupt_flag;
				key_num = touch_scanf(button, snakehead_info);
				if(key_num != 0)	
				{
//					program_step = 3;
					if(Snake_List_Move(key_num, &snakehead_info, cell_info) == 0)
					{
						LCD_ShowString(80, 100, 100, 50, 16, "GAME OVER!");
						while(1);
					}
					else 
					{
						if(Snake_link_Add_Length(snake_egg) == 1)
						{
							score_count ++;
							if(score_count >= (MAX_SCORE+level*5))
							{
								Snake_List_delete();
								Obstacle_List_Delete();
								TIM_Cmd(TIM6, DISABLE);
								program_step = 0;
								level ++;
								continue;
							}

//							Snake_Link_Generate(&snake_egg);
							program_step = 1;
						}
					}
				}
				break;
			default:
				
				break;
		}
		Snake_List_LCD_Display();
	}
}


void BSP_init(void)
{
	NVIC_Config();
	delay_init();	    	 //延时函数初始化	  
 	LCD_Init();
	KEY_Init();
	TIM6_Config(71, 9999);
	TIM_Cmd(TIM6, ENABLE);
	tp_dev.init();
}
#endif
void Prepare_Start_Game(void)
{
	u8 str1[] = "Please press";
	u8 str2[] = "KEY1";
	u8 str3[] = "to start the game";
	u32 random_num=0;

	LCD_ShowString(80, 80, 240, 15, 16, str1);
	POINT_COLOR = RED;
	LCD_ShowString(90, 110, 100, 24, 24, str2);
	POINT_COLOR = BLACK;
	LCD_ShowString(60, 150, 240, 15, 16, str3);

	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) == 1);
	TIM_Cmd(TIM6, DISABLE);
	random_num = TIM_GetCounter(TIM6);
	srand(random_num);
	LCD_Fill(60, 80, 196, 260, WHITE);
	
	Display_UP(95, 180, 50, 50, gImage_button);
	Display_DOWN(95, 250, 50, 50, gImage_button);
	Display_LEFT(25, 215, 50, 50, gImage_button);
	Display_RIGHT(165, 215, 50, 50, gImage_button);

	TIM6_Config(7199, 3999);
	TIM_Cmd(TIM6, ENABLE);
}


void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
}



