#include "touch_scanf.h"

u16 i=0,j=0;

void touch_scanf(BUTTON button)
{
	static char key_free_flag=0;

	tp_dev.scan(0);	
	if(key_free_flag == 0)
	{
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
			key_free_flag = 1;
			if((tp_dev.x[0]>button.Button_Up_Start_X)&&(tp_dev.y[0]>button.Button_Up_Start_Y)&& \
				(tp_dev.x[0]<button.Button_Up_End_X)&&(tp_dev.y[0]<button.Button_Up_End_Y))
			{	
				j = j-1;
			}
			else if((tp_dev.x[0]>button.Button_Down_Start_X)&&(tp_dev.y[0]>button.Button_Down_Start_Y)&& \
				(tp_dev.x[0]<button.Button_Down_End_X)&&(tp_dev.y[0]<button.Button_Down_End_Y))
			{	
				j=j+1;
			}
			else if((tp_dev.x[0]>button.Button_Left_Start_X)&&(tp_dev.y[0]>button.Button_Left_Start_Y)&& \
				(tp_dev.x[0]<button.Button_Left_End_X)&&(tp_dev.y[0]<button.Button_Left_End_Y))
			{	
				i=i-1;
			}
			else if((tp_dev.x[0]>button.Button_Right_Start_X)&&(tp_dev.y[0]>button.Button_Right_Start_Y)&& \
				(tp_dev.x[0]<button.Button_Right_End_X)&&(tp_dev.y[0]<button.Button_Right_End_Y))
			{	
				i=i+1;
			}
			
		}
	}
	else if((tp_dev.sta&TP_PRES_DOWN) == 0)
	{
		key_free_flag = 0;
	}
}

