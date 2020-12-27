#include "snake.h"


//static BUTTON button=
//{
//	95, 180, 145, 230,	//up
//	95, 250, 145, 300,	//down
//	25, 215, 75,  265,	//left
//	165,215, 215, 265,  //right
//};
//static Cell_Info cell_info=
//{
//	7, 1, 5, 5, 0, 0,
//};
Cell_Info cell_info = CELL_INFO_DEFAULT;


void Start_game(void)
{
	u8 str[] = "please press KEY1 to start the game";
	u32 random_num=0;

	TIM6_Config(71, 999);
	LCD_ShowString(0, 100, 240, 15, 12, str);
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) == 1);
	TIM_Cmd(TIM6, DISABLE);
	random_num = TIM_GetCounter(TIM6);
	LCD_ShowxNum(100, 150, random_num, 8, 12, 0);
	srand(random_num);
	LCD_Fill(0, 100, 240, 115, WHITE);
}
 
void Generate_Random_num(Snake_egg *snake_egg)
{
	static u16 base_num=0;
	u32 random_num = 0;
	
	//产生随机数
	base_num = TIM_GetCounter(TIM6);
	random_num = rand();
	
	if(base_num < 7)
		base_num=7;
	if(random_num > 0xffffff)
		random_num = random_num/2;

	//蛇蛋的坐标
	snake_egg->x = (random_num+base_num) / (base_num /7) % cell_info.cell_xdir_num;
	snake_egg->y = (random_num+base_num) / (base_num /3 ) % cell_info.cell_ydir_num;

}

/*******************************************************
* 函数 ：Snake_Background
* 功能 : 画墙以及单元格
* 形参 ：x1,y1 蛇可以活动的起始位置
		x2,y2 蛇可以活动的结束位置
* 返回值：无
*******************************************************/
void Background_Display(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 cell_x=0;
	u16 cell_y=0;
	u16 actual_x2=0;
	u16 actual_y2=0;

	if((x1<1)&&(y1<1))
		return ;

	LCD_Fill(0, 0, x1-1, WALL_END_Y, WALL_COLOUR);
	LCD_Fill(x1, 0, x2, y1-1, WALL_COLOUR);		
	LCD_Fill(x1, y2+1, x2, WALL_END_Y, WALL_COLOUR);
	LCD_Fill(x2+1, 0, WALL_END_X, WALL_END_Y, WALL_COLOUR);

	POINT_COLOR = CELL_COLOUR;
	
	for(cell_x=x1+cell_info.cell_size; cell_x<x2; cell_x=cell_x+(cell_info.cell_size+cell_info.cell_line_wigth))
	{
		LCD_DrawLine(cell_x, y1, cell_x, y2);
	}
	if(cell_x > x2)
	{
		cell_x = cell_x-(cell_info.cell_size+cell_info.cell_line_wigth);
		LCD_Fill(cell_x, y1, x2, y2, WALL_COLOUR);
	}
	
	actual_x2 = cell_x-1;
	for(cell_y=y1+cell_info.cell_size; cell_y<y2; cell_y=cell_y+(cell_info.cell_size+cell_info.cell_line_wigth))
	{
		LCD_DrawLine(x1, cell_y, actual_x2, cell_y);
	}
	if(cell_y > y2)
	{
		cell_y = cell_y-(cell_info.cell_size+cell_info.cell_line_wigth);
		LCD_Fill(x1, cell_y, actual_x2, y2, WALL_COLOUR);
	}
	actual_y2 = cell_y-1;

	cell_info.cell_xdir_num = (actual_x2-x1-cell_info.cell_size)/(cell_info.cell_line_wigth+cell_info.cell_size)+1;
	cell_info.cell_ydir_num = (actual_y2-y1-cell_info.cell_size)/(cell_info.cell_line_wigth+cell_info.cell_size)+1;
}


/*******************************************************
* 函数 ：Cell_fill
* 功能 : 单元格填充
* 形参 ：x,y 单元格的位置(第x行单元格第y列)
		 colour  颜色值
* 返回值：无
*******************************************************/
void Cell_fill(u16 x, u16 y, u16 color)
{
	u16 Pixel_x,Pixel_y;
	u16 Pixel_offset;

	Pixel_offset = (cell_info.cell_size-1)/2;
	Pixel_x = cell_info.cell_start_x+(cell_info.cell_line_wigth+cell_info.cell_size)*x+Pixel_offset;
	Pixel_y = cell_info.cell_start_y+(cell_info.cell_line_wigth+cell_info.cell_size)*y+Pixel_offset;
	
	LCD_Fill(Pixel_x-Pixel_offset, Pixel_y-Pixel_offset, Pixel_x+Pixel_offset, Pixel_y+Pixel_offset, color);
}



