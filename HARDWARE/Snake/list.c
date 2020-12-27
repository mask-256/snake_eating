#include "list.h"

struct information
{
	int x;
	int y;
};

struct link
{
	struct information info;	//?�u��
	struct link *pnext;		//��?��
	struct link *prev;
};

//��
struct link *phead = NULL;
struct link *tail = NULL;
static int tail_hide_x=0, tail_hide_y=0;

//�ϰ���
struct link *obstacle = NULL;

u8 Snake_Eat_self(void);
static struct link *Add_Link(int x, int y);
//static u8 Snake_link_Add_Length(Snake_egg *snake);



//�W�[�@???
struct link *Add_Link(int x, int y)
{
	struct link *pnew = NULL;
	
	while(!pnew)
	{	
		pnew = (struct link*)malloc(sizeof(struct link));	//��?�@???��?�s
		
	}
	
	pnew->info.x = x;
	pnew->info.y = y;
	pnew->pnext = NULL;	//�̦Z�@???�a�}��??��
	pnew->prev = NULL;
	return pnew;	//��^??�a�}
	
}


//����
void Creat_List_2(int x, int y)
{
	struct link *pnew = Add_Link(x, y);	//�W�[�@???

	if (phead == NULL)
	{
		/*�p�G?��?�šA?��?���a�}���V�s�ت�??
		 �Ϊ�?�p�G�Ĥ@???�a�}?�šA??�s�ت�??�N�O�Ĥ@???*/
		phead = pnew;
		phead->prev = NULL;
		phead->pnext = NULL;
		tail = pnew;
	}
	else
	{
		//list����?����Vpnew�����a�}
		tail->pnext = pnew;
		pnew->prev = tail;
		pnew->pnext = NULL;
		//list���a�}���?�̦Z�@???�����a�}�A�H�K�U�����J
		tail = pnew;
	}
}

void Obstacle_Init(int obstacle_num, Cell_Info cell_info)
{
	struct link	*list = NULL;
	struct link *snake_list = NULL;
	struct link *Obstacle_pnew = NULL;
	u8 flag=0;		//ÿ���ؿ���ʼʱ, ���ɵ��ϰ��������غϵı�־
	
	int random=0;
	int position_x, position_y; 

	//��������ϰ���
	for(u8 i=0; i<obstacle_num; )
	{
		Obstacle_pnew = (struct link*)malloc(sizeof(struct link));
		random = rand();
		position_x = random%cell_info.cell_xdir_num;
		position_y = random%cell_info.cell_ydir_num;
		
		//�����ϰ����λ��(�ϰ�������ڵ�һ�л��ߵ�һ�У����ܳ������ǵ����)
		if(position_x == 0)
			position_x = 1;
		if(position_y == 0)
			position_y = 1;

		//�������ɵ��ϰ��������غ�
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

		//����һ���ϰ����λ��
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

//��ʾ���е��ϰ���
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
ÿ���ؿ��ߵ���ʼ״̬
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
* ���� ��Snake_move
* ���� : �����ߵ��ƶ��Լ��ж����Ƿ�ײǽ����ҧβ
* �β� ��x,y ��������ʼ����
		 height ͼƬ�ĸ߶�
		 length ͼƬ�ĳ���
		 *image �洢ͼƬ�ĵ�ַ
* ����ֵ����
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
��ʾ���нڵ��λ��(�������һ���ڵ�, ��ÿ���ߵ�λ��)
Ϊ�˽�����ڳԵ��ߵ�ʱ, ��������1, ���ӵ��Ǹ����ȵ���ʾλ��
����β����һ���µĽڵ�, ����β���ӳ���λ�û��Զ���ʾ����
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

//�������һ���ߵ�
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

//�����ߵĳ���
u8 Snake_link_Add_Length(Snake_egg snake)
{
	if((phead->info.x == snake.x)&&(phead->info.y == snake.y))
	{
		Creat_List_2(tail_hide_x, tail_hide_y);
		return 1;
	}
	return 0;
}

//�ж��Ƿ�ҧ���Լ�
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

//ÿ���ؿ�ɾ���ߵ���Ϣ������ʾ�������
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

