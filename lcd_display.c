#include "lcd_display.h"

BUTTON button=
{
	95, 180, 145, 230,	//up
	95, 250, 145, 300,	//down
	25, 215, 75,  265,	//left
	165,215, 215, 265,  //right
};
Cell_Info cell_info=
{
	7, 1, 5, 5, 0, 0,
};

/*******************************************************
* 函数 ：Snake_Background
* 功能 : 画墙以及单元格
* 形参 ：x1,y1 蛇可以活动的起始位置
	 x2,y2 蛇可以活动的结束位置
* 返回值：无
*******************************************************/
void Snake_Background(u16 x1, u16 y1, u16 x2, u16 y2)
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
* 函数 ：Display_UP
* 功能 : 显示按键(上下左右使用一张图片)
* 形参 ：x,y 按键的起始坐标
		 height 图片的高度
		 length 图片的长度
		 *image 存储图片的地址
* 返回值：无
*******************************************************/
void Display_UP(u16 x, u16 y, u16 height, u16 length, const u8 *image)
{
	u32 i,j;

	for(j=0; j<height; j++)		//up
	{
		LCD_SetCursor(x, y+j);
		LCD_WriteRAM_Prepare();
		for(i=0; i<length; i++)
			LCD_WR_DATA((image[length*j*2+i*2]<<8)|image[j*length*2+i*2+1]);
	}
}

void Display_DOWN(u16 x, u16 y, u16 height, u16 length, const u8 *image)
{
	u32 i,j;

	for(j=0; j<height; j++)		//down
	{
		LCD_SetCursor(x, y+j);
		LCD_WriteRAM_Prepare();
		for(i=0; i<length; i++)
			LCD_WR_DATA((image[length*(height-1-j)*2+i*2]<<8)|(image[(height-1-j)*length*2+i*2+1]));
	}
}

void Display_LEFT(u16 x, u16 y, u16 height, u16 length, const u8 *image)
{
	u32 i,j;

	for(i=length; i>0; i--)		//left
	{
		LCD_SetCursor(x, y+(length-i));
		LCD_WriteRAM_Prepare();
		for(j=0; j<height; j++)
			LCD_WR_DATA((image[j*length*2+2*(i-1)]<<8)|image[j*length*2+2*(i-1)+1]);
	}
}

void Display_RIGHT(u16 x, u16 y, u16 height, u16 length, const u8 *image)
{
	u32 i,j;

	for(i=0; i<length; i++)		//right
	{
		LCD_SetCursor(x, y+i);
		LCD_WriteRAM_Prepare();
		for(j=height; j>0; j--)
			LCD_WR_DATA((image[(j-1)*length*2+2*i]<<8)|image[(j-1)*length*2+2*i+1]);
	}

}

/*******************************************************
* 函数 ：Display_UP
* 功能 : 显示按键(上下左右使用一张图片)
* 形参 ：x,y 单元格的位置(第x行单元格第y列)
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


