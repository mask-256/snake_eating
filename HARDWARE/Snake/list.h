#ifndef __LIST_H
#define __LIST_H
#include <stdlib.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "snake.h"

typedef enum
{
	DIR_NULL,
	LEFT,
	UP,
	DOWN,
	RIGHT,
}HEAD_DIR;

typedef struct
{
	HEAD_DIR dir;
	u16 head_posi_x;
	u16 head_posi_y;
}SnakeHead_Info;


void Obstacle_Init(int obstacle_num, Cell_Info cell_info);
void Obstacle_Display(void);
void Obstacle_List_Delete(void);

void Snake_Init(SnakeHead_Info *snakehead_info);
u8 Snake_List_Move(u8 key_num, SnakeHead_Info *snakehead_info, Cell_Info cell_info);
void Snake_List_LCD_Display(void);
void Snake_Link_Generate(Snake_egg *snake_egg);
u8 Snake_link_Add_Length(Snake_egg snake);
void Snake_List_delete(void);

#endif
