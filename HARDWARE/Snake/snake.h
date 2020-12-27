#ifndef __SNAKE_H
#define __SNAKE_H
#include "stm32f10x.h"
#include "tim.h"
#include "lcd.h"

#define WALL_COLOUR RED		
#define CELL_COLOUR BLUE	

#define WALL_START_X 	0
#define WALL_START_Y	0
#define WALL_END_X		239
#define WALL_END_Y		159

//#define CELL_SIZE		7		
//#define CELL_LINE_WIGTH	1
//#define CELL_START_X    5
//#define CELL_START_Y	5

typedef struct
{
	u16 x;
	u16 y;
}Snake_egg;

typedef struct
{
	u8 cell_size;			//单元格尺寸为奇数,便于填充
	u8 cell_line_wigth;     //网格线的宽度

	u16 cell_start_x;		//网格开始的坐标
	u16 cell_start_y;		

	u16 cell_xdir_num;		//网格在x轴方向的数量
	u16 cell_ydir_num;		//网格在y轴方向的数量
}Cell_Info;

#define CELL_INFO_DEFAULT   		\
{									\
	.cell_size  		= 7,		\
	.cell_line_wigth	= 1,		\
	.cell_start_x		= 5, 		\
	.cell_start_y		= 5,		\
	.cell_xdir_num		= 0,		\
	.cell_ydir_num		= 0,		\
}

typedef struct 
{
	u16 Button_Up_Start_X;
	u16 Button_Up_Start_Y;
	u16 Button_Up_End_X;
	u16 Button_Up_End_Y;
	u16 Button_Down_Start_X;
	u16 Button_Down_Start_Y;
	u16 Button_Down_End_X;
	u16 Button_Down_End_Y;
	u16 Button_Left_Start_X;
	u16 Button_Left_Start_Y;
	u16 Button_Left_End_X;
	u16 Button_Left_End_Y;
	u16 Button_Right_Start_X;
	u16 Button_Right_Start_Y;
	u16 Button_Right_End_X;
	u16 Button_Right_End_Y;
}BUTTON;

#define BUTTON_SIZE_DEFAULT				\
{										\
	.Button_Up_Start_X			= 95,	\
	.Button_Up_Start_Y			= 180,	\
	.Button_Up_End_X			= 145,	\
	.Button_Up_End_Y			= 230,	\
	.Button_Down_Start_X		= 95,	\
	.Button_Down_Start_Y		= 250,	\
	.Button_Down_End_X			= 145,	\
	.Button_Down_End_Y			= 300,	\
	.Button_Left_Start_X		= 25,	\
	.Button_Left_Start_Y		= 215,	\
	.Button_Left_End_X			= 75,	\
	.Button_Left_End_Y			= 265,	\
	.Button_Right_Start_X		= 165,	\
	.Button_Right_Start_Y		= 215,	\
	.Button_Right_End_X			= 215,	\
	.Button_Right_End_Y			= 265,	\
}



extern const unsigned char gImage_button[5000];

void Background_Display(u16 x1, u16 y1, u16 x2, u16 y2);
void Cell_fill(u16 x, u16 y, u16 color);

void Start_game(void);
void Generate_Random_num(Snake_egg *snake_egg);
#endif



