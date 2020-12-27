#ifndef __BACKGROUND_H
#define __BACKGROUND_H
#include "stm32f10x.h"
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
	u8 cell_size;			//单元格尺寸为奇数,便于填充
	u8 cell_line_wigth;     //网格线的宽度

	u16 cell_start_x;		//网格开始的坐标
	u16 cell_start_y;		

	u16 cell_xdir_num;		//网格在x轴方向的数量
	u16 cell_ydir_num;		//网格在y轴方向的数量
}Cell_Info;

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
extern BUTTON button;
extern Cell_Info cell_info;

extern const unsigned char gImage_button[5000];

void Snake_Background(u16 x1, u16 y1, u16 x2, u16 y2);
void Display_UP(u16 x, u16 y, u16 height, u16 length, const u8 *image);
void Display_DOWN(u16 x, u16 y, u16 height, u16 length, const u8 *image);
void Display_LEFT(u16 x, u16 y, u16 height, u16 length, const u8 *image);
void Display_RIGHT(u16 x, u16 y, u16 height, u16 length, const u8 *image);
void Cell_fill(u16 x, u16 y, u16 color);

#endif

