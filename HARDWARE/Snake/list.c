#include "list.h"

struct information
{
	int x;
	int y;
};

struct link
{
	struct information info;	//?誹办
	struct link *pnext;		//?办
	struct link *prev;
};

//蛇
struct link *phead = NULL;
struct link *tail = NULL;
static int tail_hide_x=0, tail_hide_y=0;

//障碍物
struct link *obstacle = NULL;

u8 Snake_Eat_self(void);
static struct link *Add_Link(int x, int y);
//static u8 Snake_link_Add_Length(Snake_egg *snake);



//糤???
struct link *Add_Link(int x, int y)
{
	struct link *pnew = NULL;
	
	while(!pnew)
	{	
		pnew = (struct link*)malloc(sizeof(struct link));	//ビ?????
		
	}
	
	pnew->info.x = x;
	pnew->info.y = y;
	pnew->pnext = NULL;	//程???ゲ??
	pnew->prev = NULL;
	return pnew;	//??
	
}


//Ю础
void Creat_List_2(int x, int y)
{
	struct link *pnew = Add_Link(x, y);	//糤???

	if (phead == NULL)
	{
		/*狦???р?穝??
		 ┪?狦材??????穝??碞琌材???*/
		phead = pnew;
		phead->prev = NULL;
		phead->pnext = NULL;
		tail = pnew;
	}
	else
	{
		//list?办pnew
		tail->pnext = pnew;
		pnew->prev = tail;
		pnew->pnext = NULL;
		//listэ?程???獽Ω础
		tail = pnew;
	}
}

void Obstacle_Init(int obstacle_num, Cell_Info cell_info)
{
	struct link	*list = NULL;
	struct link *snake_list = NULL;
	struct link *Obstacle_pnew = NULL;
	u8 flag=0;		//每个关卡开始时, 生成的障碍物与蛇重合的标志
	
	int random=0;
	int position_x, position_y; 

	//随机产生障碍物
	for(u8 i=0; i<obstacle_num; )
	{
		Obstacle_pnew = (struct link*)malloc(sizeof(struct link));
		random = rand();
		position_x = random%cell_info.cell_xdir_num;
		position_y = random%cell_info.cell_ydir_num;
		
		//限制障碍物的位置(障碍物出现在第一行或者第一列，可能出现死角的情况)
		if(position_x == 0)
			position_x = 1;
		if(position_y == 0)
			position_y = 1;

		//避免生成的障碍物与蛇重合
		if(phead != NULL)
		{
			snake_list = phead;
			while(snake_list != NULL)
			{
				if((snake_list->info.x==position_x)&&(snake_list->info.y==position_y))
				{
					flag = 1;
					break;
				}
				snake_list = snake_list->pnext;
			}
			if(flag == 1)
			{
				flag = 0;
				continue;
			}
		}

		//设置一个障碍物的位置
		if(obstacle == NULL)
		{
			obstacle = Obstacle_pnew;
			list = Obstacle_pnew;
			Obstacle_pnew->info.x = position_x;
			Obstacle_pnew->info.y = position_y;
		}
		else 
		{
			Obstacle_pnew->info.x = position_x;
			Obstacle_pnew->info.y = position_y;
			list->pnext = Obstacle_pnew;
			Obstacle_pnew->prev = list;
			Obstacle_pnew->pnext = NULL;
			list = list->pnext;
			
		}

		i ++;
	}
}

void Obstacle_List_Delete(void)
{	
	struct link *list = obstacle;
	if(list != NULL)
	{
		while(list->pnext != NULL)
		{
			list = obstacle;
			Cell_fill(list->info.x, list->info.y, WHITE);
			obstacle = list->pnext;
			free(list);
		}
	}
}

//显示所有的障碍物
void Obstacle_Display(void)
{
	if(obstacle != NULL)
	{
		struct link* list = obstacle;
		while(list != NULL)
		{
			Cell_fill(list->info.x, list->info.y, BLACK);
			list = list->pnext;
		}
	}
}

u8 Hit_Obstacle(void)
{
	struct link *list = obstacle;

	while(list != NULL)
	{
		if((list->info.x==phead->info.x) && (list->info.y==phead->info.y))
			return 0;
		list = list->pnext;
	}
	return 1;
}

/*
每个关卡蛇的起始状态
*/
void Snake_Init(SnakeHead_Info *snakehead_info)
{

	Creat_List_2(1,2);
	Creat_List_2(2,2);
//	Creat_List_2(3,2);

	snakehead_info->head_posi_x = phead->info.x;
	snakehead_info->head_posi_y = phead->info.y;
	snakehead_info->dir			= DIR_NULL;
}

/*******************************************************
* 函数 ：Snake_move
* 功能 : 控制蛇的移动以及判断蛇是否撞墙或者咬尾
* 形参 ：x,y 按键的起始坐标
		 height 图片的高度
		 length 图片的长度
		 *image 存储图片的地址
* 返回值：无
*******************************************************/
u8 Snake_List_Move(u8 key_num, SnakeHead_Info *snakehead_info, Cell_Info cell_info)
{
	struct link *list = tail;
	
	tail_hide_x = list->info.x;
	tail_hide_y = list->info.y;
	while(list->prev != NULL)
	{
		list->info.x = list->prev->info.x;
		list->info.y = list->prev->info.y;
		list = list->prev;
	}

	switch(key_num)
	{
		case 1:
			snakehead_info->dir = UP;
			if(snakehead_info->head_posi_y <= 0)
				return 0;
			snakehead_info->head_posi_y = list->info.y - 1;
			break;
		case 2:
			snakehead_info->dir = DOWN;
			if(snakehead_info->head_posi_y >= cell_info.cell_ydir_num)
				return 0;
			snakehead_info->head_posi_y  = list->info.y + 1;
			break;
		case 3:
			snakehead_info->dir = LEFT;
			if(snakehead_info->head_posi_x <= 0)
				return 0;
			snakehead_info->head_posi_x  = list->info.x - 1;
			break;
		case 4:
			snakehead_info->dir = RIGHT;
			if(snakehead_info->head_posi_x >= cell_info.cell_xdir_num)
				return 0;
			snakehead_info->head_posi_x  = list->info.x + 1;
			break;
		default:
			break;
	}

	list->info.x = snakehead_info->head_posi_x;
	list->info.y = snakehead_info->head_posi_y;

	if(Snake_Eat_self() == 0)
		return 0;
	
	if(Hit_Obstacle() == 0)
		return 0;
	return 1;
}


/*
显示所有节点的位置(除了最后一个节点, 即每节蛇的位置)
为了解决蛇在吃到蛇蛋时, 长度增加1, 增加的那个长度的显示位置
在蛇尾创建一个新的节点, 在蛇尾增加长度位置会自动显示出来
*/
void Snake_List_LCD_Display(void)
{
	static int head_x=0, head_y=0;
	struct link *list = phead;
	
	if(phead != NULL)
	{
		if((head_x != phead->info.x)||(head_y != phead->info.y))
		{
			head_x = phead->info.x;
			head_y = phead->info.y;

			while(list != NULL)
			{
				if(list->pnext == NULL)
				{
					if(list == tail)
					{		
						Cell_fill(list->info.x, list->info.y, WHITE);
						break;
					}	
				}
				if(list == phead)
					Cell_fill(list->info.x, list->info.y, RED);
				list = list->pnext;

			}
		}
	}
}

//随机产生一个蛇蛋
void Snake_Link_Generate(Snake_egg *snake_egg)
{
	struct link *list1 = phead;
	struct link *list2 = obstacle;
	Generate_Random_num(snake_egg);
	while((list1 != NULL)||(list2 != NULL))
	{
		if(list1 != NULL)
		{
			if((snake_egg->x == list1->info.x)&&(snake_egg->y == list1->info.y))
			{
				list1 = phead;
				list2 = obstacle;
				Generate_Random_num(snake_egg);
			}
			list1 = list1->pnext;
		}
		if(list2 != NULL)
		{
			if((snake_egg->x == list2->info.x)&&(snake_egg->y == list2->info.y))
			{
				list1 = phead;
				list2 = obstacle;
				Generate_Random_num(snake_egg);
			}
			list2 = list2->pnext;
		}
	}
}

//增加蛇的长度
u8 Snake_link_Add_Length(Snake_egg snake)
{
	if((phead->info.x == snake.x)&&(phead->info.y == snake.y))
	{
		Creat_List_2(tail_hide_x, tail_hide_y);
		return 1;
	}
	return 0;
}

//判断是否咬到自己
u8 Snake_Eat_self(void)
{
	struct link *list = phead->pnext;

	for(; list->pnext!=NULL; list=list->pnext)
	{
		if((list->info.x==phead->info.x)&&(list->info.y==phead->info.y))
			return 0;
	}
	return 1;
}

//每个关卡删除蛇的信息并在显示屏上清除
void Snake_List_delete(void)
{
	struct link *list = phead;
	
	if(list != NULL)
	{
		while(list->pnext != NULL)
		{
			list = phead;
			Cell_fill(list->info.x, list->info.y, WHITE);
			phead = list->pnext;
			free(list);
		}
	}
}

