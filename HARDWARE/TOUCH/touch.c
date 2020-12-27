#include "touch.h" 
#include "lcd.h"
#include "delay.h"
#include "stdlib.h"
#include "math.h"
#include "24cxx.h"	    
#include "tim.h"


_m_tp_dev tp_dev=
{
	TP_Init,
	TP_Scan,
	TP_Adjust,
	0,
	0, 
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	  	 		
};

//Ĭ��Ϊtouchtype=0������.
u8 CMD_RDX=0XD0;
u8 CMD_RDY=0X90;
 	 			    					   
//SPIд����
//������ICд��1byte����    
//num:Ҫд�������
void TP_Write_Byte(u8 num)    
{  
	u8 count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TDIN=1;  
		else TDIN=0;   
		num<<=1;    
		TCLK=0; 	 
		TCLK=1;		//��������Ч	        
	}		 			    
} 		 
//SPI������ 
//�Ӵ�����IC��ȡadcֵ
//CMD:ָ��
//����ֵ:����������	   
u16 TP_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	TCLK=0;		//������ʱ�� 	 
	TDIN=0; 	//����������
	TCS=0; 		//ѡ�д�����IC
	TP_Write_Byte(CMD);//����������
	delay_us(6);//ADS7846��ת��ʱ���Ϊ6us
	TCLK=0; 	     	    
	delay_us(1);    	   
	TCLK=1;		//��1��ʱ�ӣ����BUSY	    	    
	TCLK=0; 	     	    
	for(count=0;count<16;count++)//����16λ����,ֻ�и�12λ��Ч 
	{ 				  
		Num<<=1; 	 
		TCLK=0;	//�½�����Ч  	    	   
		TCLK=1;
		if(DOUT)Num++; 		 
	}  	
	Num>>=4;   	//ֻ�и�12λ��Ч.
	TCS=1;		//�ͷ�Ƭѡ	 
	return(Num);   
}
//��ȡһ������ֵ(x����y)
//������ȡREAD_TIMES������,����Щ������������,
//Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ 
//xy:ָ�CMD_RDX/CMD_RDY��
//����ֵ:����������
#define READ_TIMES 5 	//��ȡ����
#define LOST_VAL 1	  	//����ֵ
u16 TP_Read_XOY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=TP_Read_AD(xy);		 		    
	for(i=0;i<READ_TIMES-1; i++)//����
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
//��ȡx,y����
//��Сֵ��������100.
//x,y:��ȡ��������ֵ
//����ֵ:0,ʧ��;1,�ɹ���
u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=TP_Read_XOY(CMD_RDX);
	ytemp=TP_Read_XOY(CMD_RDY);	  												   
	//if(xtemp<100||ytemp<100)return 0;//����ʧ��
	*x=xtemp;
	*y=ytemp;
	return 1;//�����ɹ�
}
//����2�ζ�ȡ������IC,�������ε�ƫ��ܳ���
//ERR_RANGE,��������,����Ϊ������ȷ,�����������.	   
//�ú����ܴ�����׼ȷ��
//x,y:��ȡ��������ֵ
//����ֵ:0,ʧ��;1,�ɹ���
#define ERR_RANGE 50 //��Χ 
u8 TP_Read_XY2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=TP_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//ǰ�����β�����+-50��
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
}  
//////////////////////////////////////////////////////////////////////////////////		  
//��LCD�����йصĺ���  
//��һ��������
//����У׼�õ�
//x,y:����
//color:��ɫ
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color)
{
	POINT_COLOR=color;
	LCD_DrawLine(x-12,y,x+13,y);//����
	LCD_DrawLine(x,y-12,x,y+13);//����
	LCD_DrawPoint(x+1,y+1);
	LCD_DrawPoint(x-1,y+1);
	LCD_DrawPoint(x+1,y-1);
	LCD_DrawPoint(x-1,y-1);
	LCD_Draw_Circle(x,y,6);//������Ȧ
}	  
//��һ�����(2*2�ĵ�)		   
//x,y:����
//color:��ɫ
void TP_Draw_Big_Point(u16 x,u16 y,u16 color)
{	    
	POINT_COLOR=color;
	LCD_DrawPoint(x,y);//���ĵ� 
	LCD_DrawPoint(x+1,y);
	LCD_DrawPoint(x,y+1);
	LCD_DrawPoint(x+1,y+1);	 	  	
}						  
//////////////////////////////////////////////////////////////////////////////////		  
//��������ɨ��
//tp:0,��Ļ����;1,��������(У׼�����ⳡ����)
//����ֵ:��ǰ����״̬.
//0,�����޴���;1,�����д���
u8 TP_Scan(u8 tp)
{			   
	if(PEN==0)//�а�������
	{
		if(tp)TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]);//��ȡ��������
		else if(TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]))//��ȡ��Ļ����
		{
	 		tp_dev.x[0]=tp_dev.xfac*tp_dev.x[0]+tp_dev.xoff;//�����ת��Ϊ��Ļ����
			tp_dev.y[0]=tp_dev.yfac*tp_dev.y[0]+tp_dev.yoff;  
	 	} 
		if((tp_dev.sta&TP_PRES_DOWN)==0)//֮ǰû�б�����
		{		 
			tp_dev.sta=TP_PRES_DOWN|TP_CATH_PRES;//��������  
			tp_dev.x[4]=tp_dev.x[0];//��¼��һ�ΰ���ʱ������
			tp_dev.y[4]=tp_dev.y[0];  	   			 
		}			   
	}else
	{
		if(tp_dev.sta&TP_PRES_DOWN)//֮ǰ�Ǳ����µ�
		{
			tp_dev.sta&=~(1<<7);//��ǰ����ɿ�	
		}else//֮ǰ��û�б�����
		{
			tp_dev.x[4]=0;
			tp_dev.y[4]=0;
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
		}	    
	}
	return tp_dev.sta&TP_PRES_DOWN;//���ص�ǰ�Ĵ���״̬
}	  
//////////////////////////////////////////////////////////////////////////	 
//������EEPROM����ĵ�ַ�����ַ,ռ��13���ֽ�(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
#define SAVE_ADDR_BASE 40
//����У׼����										    
void TP_Save_Adjdata(void)
{  
	s32 temp;			 
	//����У�����!		   							  
	temp=tp_dev.xfac*100000000;//����xУ������      
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE,temp,4);   
	temp=tp_dev.yfac*100000000;//����yУ������    
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+4,temp,4);
	//����xƫ����
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+8,tp_dev.xoff,2);		    
	//����yƫ����
	AT24CXX_WriteLenByte(SAVE_ADDR_BASE+10,tp_dev.yoff,2);	
	//���津������
	AT24CXX_WriteOneByte(SAVE_ADDR_BASE+12,tp_dev.touchtype);	
	temp=0X0A;//���У׼����
	AT24CXX_WriteOneByte(SAVE_ADDR_BASE+13,temp); 
}
//�õ�������EEPROM�����У׼ֵ
//����ֵ��1���ɹ���ȡ����
//        0����ȡʧ�ܣ�Ҫ����У׼
u8 TP_Get_Adjdata(void)
{		  
	s32 tempfac;			  
	u8 temp;
	temp=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+13);//��ȡ�����,���Ƿ�У׼���� 		 
	if(temp==0X0A)//�������Ѿ�У׼����			   
	{    												 
		tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE,4);		   
		tp_dev.xfac=(float)tempfac/100000000;//�õ�xУ׼����
		tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+4,4);			          
		tp_dev.yfac=(float)tempfac/100000000;//�õ�yУ׼����
	    //�õ�xƫ����
		tp_dev.xoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+8,2);			   	  
 	    //�õ�yƫ����
		tp_dev.yoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+10,2);				 	  
 		tp_dev.touchtype=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+12);//��ȡ�������ͱ��
		if(tp_dev.touchtype)//X,Y��������Ļ�෴
		{
			CMD_RDX=0X90;
			CMD_RDY=0XD0;	 
		}else				   //X,Y��������Ļ��ͬ
		{
			CMD_RDX=0XD0;
			CMD_RDY=0X90;	 
		}		 
		return 1;	 
	}
	return 0;
}	 
//��ʾ�ַ���
const u8* TP_REMIND_MSG_TBL="Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.";
 					  
//��ʾУ׼���(��������)
void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac)
{	  
	POINT_COLOR=RED;
	LCD_ShowString(40,160,lcddev.width,lcddev.height,16,"x1:");
 	LCD_ShowString(40+80,160,lcddev.width,lcddev.height,16,"y1:");
 	LCD_ShowString(40,180,lcddev.width,lcddev.height,16,"x2:");
 	LCD_ShowString(40+80,180,lcddev.width,lcddev.height,16,"y2:");
	LCD_ShowString(40,200,lcddev.width,lcddev.height,16,"x3:");
 	LCD_ShowString(40+80,200,lcddev.width,lcddev.height,16,"y3:");
	LCD_ShowString(40,220,lcddev.width,lcddev.height,16,"x4:");
 	LCD_ShowString(40+80,220,lcddev.width,lcddev.height,16,"y4:");  
 	LCD_ShowString(40,240,lcddev.width,lcddev.height,16,"fac is:");     
	LCD_ShowNum(40+24,160,x0,4,16);		//��ʾ��ֵ
	LCD_ShowNum(40+24+80,160,y0,4,16);	//��ʾ��ֵ
	LCD_ShowNum(40+24,180,x1,4,16);		//��ʾ��ֵ
	LCD_ShowNum(40+24+80,180,y1,4,16);	//��ʾ��ֵ
	LCD_ShowNum(40+24,200,x2,4,16);		//��ʾ��ֵ
	LCD_ShowNum(40+24+80,200,y2,4,16);	//��ʾ��ֵ
	LCD_ShowNum(40+24,220,x3,4,16);		//��ʾ��ֵ
	LCD_ShowNum(40+24+80,220,y3,4,16);	//��ʾ��ֵ
 	LCD_ShowNum(40+56,lcddev.width,fac,3,16); 	//��ʾ��ֵ,����ֵ������95~105��Χ֮��.

}
		 
//У׼�������(0,1)��(2,3)��(0,2)��(1,3),(1,2)��(0,3)���������ľ���
const u8 TP_ADJDIS_TBL[3][4]={{0,1,2,3},{0,2,1,3},{1,2,0,3}};//У׼��������
//������У׼����
//�õ��ĸ�У׼����
void TP_Adjust(void)
{								 
	u16 pos_temp[4][2];//���껺��ֵ
	u8  cnt=0;	
	u16 d1,d2;
	u32 tem1,tem2;
	float fac; 	
	u16 outtime=0; 	  
	LCD_Clear(WHITE);	//���� 	    
	POINT_COLOR=BLUE;	//��ɫ
	LCD_ShowString(40,40,160,100,16,(u8*)TP_REMIND_MSG_TBL);//��ʾ��ʾ��Ϣ
	TP_Drow_Touch_Point(20,20,RED);//����1 
	tp_dev.sta=0;//���������ź� 
	tp_dev.xfac=0;//xfac��������Ƿ�У׼��,����У׼֮ǰ�������!�������	 
	while(1)//�������10����û�а���,���Զ��˳�
	{
READJ:
		tp_dev.scan(1);//ɨ����������
		if((tp_dev.sta&0xc0)==TP_CATH_PRES)//����������һ��(��ʱ�����ɿ���.)
		{	
			outtime=0;		
			tp_dev.sta&=~(1<<6);	//��ǰ����Ѿ����������. 
			pos_temp[cnt][0]=tp_dev.x[0];
			pos_temp[cnt][1]=tp_dev.y[0];
			cnt++;	  
			switch(cnt)
			{			   
				case 1:						 
					TP_Drow_Touch_Point(20,20,WHITE);				//�����1 
					TP_Drow_Touch_Point(lcddev.width-20,20,RED);	//����2
					break;
				case 2:
 					TP_Drow_Touch_Point(lcddev.width-20,20,WHITE);	//�����2
					TP_Drow_Touch_Point(20,lcddev.height-20,RED);	//����3
					break;
				case 3:
 					TP_Drow_Touch_Point(20,lcddev.height-20,WHITE);			//�����3
 					TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,RED);	//����4
					break;                                                                                                                                                                                                                                                          
				case 4:	 //ȫ���ĸ����Ѿ��õ�
					for(cnt=0;cnt<3;cnt++)//���������ľ����Ƿ�������Χ�ڣ�
					{ 
						tem1=abs(pos_temp[TP_ADJDIS_TBL[cnt][0]][0]-pos_temp[TP_ADJDIS_TBL[cnt][1]][0]);//x1-x2/x1-x3/x2-x3
						tem2=abs(pos_temp[TP_ADJDIS_TBL[cnt][0]][1]-pos_temp[TP_ADJDIS_TBL[cnt][1]][1]);//y1-y2/y1-y3/y2-y3
						tem1*=tem1;
						tem2*=tem2;
						d1=sqrt(tem1+tem2);//�õ�����֮��ľ��� 
						tem1=abs(pos_temp[TP_ADJDIS_TBL[cnt][2]][0]-pos_temp[TP_ADJDIS_TBL[cnt][3]][0]);//x3-x4/x2-x4/x1-x4
						tem2=abs(pos_temp[TP_ADJDIS_TBL[cnt][2]][1]-pos_temp[TP_ADJDIS_TBL[cnt][3]][1]);//y3-y4/y2-y4/y1-y4
						tem1*=tem1;
						tem2*=tem2;
						d2=sqrt(tem1+tem2);//�õ�����֮��ľ���
						fac=(float)d1/d2;
						if(fac<0.95||fac>1.05||d1==0||d2==0)//���ϸ�
						{
							cnt=0;
							TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	//�����4
							TP_Drow_Touch_Point(20,20,RED);									//����1
							TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//��ʾ����   
							goto READJ;	//���ϸ�����У׼
						}
					}  
					//��ȷ��
					//������
					tp_dev.xfac=(float)(lcddev.width-40)/(pos_temp[1][0]-pos_temp[0][0]);//�õ�xfac		 
					tp_dev.xoff=(lcddev.width-tp_dev.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//�õ�xoff
 					tp_dev.yfac=(float)(lcddev.height-40)/(pos_temp[2][1]-pos_temp[0][1]);//�õ�yfac
					tp_dev.yoff=(lcddev.height-tp_dev.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//�õ�yoff  
					if(abs(tp_dev.xfac)>2||abs(tp_dev.yfac)>2)//������Ԥ����෴��.
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	//�����4
   	 					TP_Drow_Touch_Point(20,20,RED);								//����1
						LCD_ShowString(40,26,lcddev.width,lcddev.height,16,"TP Need readjust!");
						tp_dev.touchtype=!tp_dev.touchtype;//�޸Ĵ�������.
						if(tp_dev.touchtype)//X,Y��������Ļ�෴
						{
							CMD_RDX=0X90;
							CMD_RDY=0XD0;	 
						}else				   //X,Y��������Ļ��ͬ
						{
							CMD_RDX=0XD0;
							CMD_RDY=0X90;	 
						}			    
						continue;
					}		
					POINT_COLOR=BLUE;
					LCD_Clear(WHITE);//����
					LCD_ShowString(35,110,lcddev.width,lcddev.height,16,"Touch Screen Adjust OK!");//У�����
					delay_ms(1000);
					TP_Save_Adjdata();  
 					LCD_Clear(WHITE);//����   
					return;//У�����				 
			}
		}
		delay_ms(10);
		outtime++;
		if(outtime>1000)
		{
			TP_Get_Adjdata();
			break;
	 	} 
 	}
}	  
//��������ʼ��  		    
//����ֵ:0,û�н���У׼
//       1,���й�У׼
u8 TP_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO 

	//ע��,ʱ��ʹ��֮��,��GPIO�Ĳ�������Ч
	//��������֮ǰ,����ʹ��ʱ��.����ʵ���������������

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_0|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//GPIOC->ODR|=0X200f;    //PC0~3 13 ȫ������  
	TP_Read_XY(&tp_dev.x[0],&tp_dev.y[0]);//��һ�ζ�ȡ��ʼ��	 
	AT24CXX_Init();//��ʼ��24CXX
	if(TP_Get_Adjdata())
		return 0;//�Ѿ�У׼
	else			   //δУ׼?
	{ 										    
		LCD_Clear(WHITE);//����
		TP_Adjust();  //��ĻУ׼ 
		TP_Save_Adjdata();	 
	}			
	TP_Get_Adjdata();	
	
	return 1; 									 
}

/*******************************************************
* ���� ��Display_UP
* ���� : ��ʾ����(��������ʹ��һ��ͼƬ)
* �β� ��x,y ��������ʼ����
		 height ͼƬ�ĸ߶�
		 length ͼƬ�ĳ���
		 *image �洢ͼƬ�ĵ�ַ
* ����ֵ����
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

u8 touch_scanf(BUTTON button, SnakeHead_Info snakehead)
{
	volatile static u8 key_flag=0;
	static u8 key_num = 0;
//	static u8 key_num_L = 0;

	if(Enter_Interrupt_flag == 1)
	{
		Enter_Interrupt_flag = 0;
		return key_num;
	}

	tp_dev.scan(0);	
		
	switch(key_flag)
	{
		case 0:
			if(tp_dev.sta&TP_PRES_DOWN)			//������������
			{	
				key_flag = 1;
				if((tp_dev.x[0]>button.Button_Up_Start_X)&&(tp_dev.y[0]>button.Button_Up_Start_Y)&& \
					(tp_dev.x[0]<button.Button_Up_End_X)&&(tp_dev.y[0]<button.Button_Up_End_Y))
				{	
					if((snakehead.dir!=DOWN) && (snakehead.dir!=UP))
					{
						key_num = 1;
//						if(key_num == key_num_L)
//							key_flag = 0;
//						else 
//							key_num_L = key_num;
					}
					else 
						key_flag = 0;
				}
				else if((tp_dev.x[0]>button.Button_Down_Start_X)&&(tp_dev.y[0]>button.Button_Down_Start_Y)&& \
					(tp_dev.x[0]<button.Button_Down_End_X)&&(tp_dev.y[0]<button.Button_Down_End_Y))
				{	
					if((snakehead.dir!=UP) && (snakehead.dir!=DOWN))	
					{
						key_num = 2;
//						if(key_num == key_num_L)
//							key_flag = 0;
//						else 
//							key_num_L = key_num;
					}
					else
						key_flag = 0;
				}
				else if((tp_dev.x[0]>button.Button_Left_Start_X)&&(tp_dev.y[0]>button.Button_Left_Start_Y)&& \
					(tp_dev.x[0]<button.Button_Left_End_X)&&(tp_dev.y[0]<button.Button_Left_End_Y))
				{	
					if((snakehead.dir!=RIGHT) && (snakehead.dir!=LEFT))
					{
						key_num = 3;
//						if(key_num == key_num_L)
//							key_flag = 0;
//						else 
//							key_num_L = key_num;					
					}
					else 
						key_flag = 0;
				}
				else if((tp_dev.x[0]>button.Button_Right_Start_X)&&(tp_dev.y[0]>button.Button_Right_Start_Y)&& \
					(tp_dev.x[0]<button.Button_Right_End_X)&&(tp_dev.y[0]<button.Button_Right_End_Y))
				{	
					if((snakehead.dir!=LEFT) && (snakehead.dir!=RIGHT))
					{
						key_num = 4;
//						if(key_num == key_num_L)
//							key_flag = 0;
//						else 
//							key_num_L = key_num;					
					}
					else 
						key_flag = 0;
				}
				else 
				{
					key_flag = 0;
				}
				
			}
			break;
		case  1:
			if(!(tp_dev.sta&TP_PRES_DOWN))
			{
				key_flag = 2;
			}	
			break;
		case 2:
			key_flag = 0;
			TIM_Cmd(TIM6, DISABLE);
			TIM_SetCounter(TIM6, 0);
			TIM_Cmd(TIM6, ENABLE);
			return key_num;
			break;
		default:
			break;
	}
	
	return 0;
}





