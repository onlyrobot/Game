/********************************************************************************
*File name:SnakeGame1.0.c                                                       *
*Author:彭瑶                                                                    *
*Version:1.0                                                                    *
*Date:2018.04.17                                                                *
*Debug Environment:Visual Studio 2017                                           *
*Description:贪吃蛇游戏源代码（C语言）                                          *
*********************************************************************************/
#include<stdio.h>
#include<Windows.h>
#include<time.h>
#include<conio.h>

#define SIZE 19 //定义地图大小
#define MAX_LENGTH 90 //定义蛇的最大长度

typedef struct point //地图上的点的节点
{
	int r;
	int c;
} point;

void init(int * length, point * foodAt, int * dir, point body[], char map[][SIZE]); //初始化
int getDir(int dir); //获取蛇的行进方向
int move(point foodAt, int dir, int length, point body[]); //蛇的运动
void draw(int length, point foodAt, point body[], char map[][SIZE]); //画图
void food(point * foodAt, point body[], int length, char map[][SIZE]); //生成食物

int main()
{
	char map[SIZE][SIZE]; //定义地图
	point body[MAX_LENGTH], foodAt; //整个蛇身体和食物的所在点
	int length; //蛇的实际长度
	int dir; //行进方向
	int rate = 1; //行进速率
	int result; //保存蛇运动的结果：【死亡】、【得分】、【无】

	init(&length, &foodAt, &dir, body, map); //初始化

	while (1)
	{
		Sleep(500 / rate);
		dir = getDir(dir); //获取蛇的行进方向
		result = move(foodAt, dir, length, body); //蛇的运动
		if (result == 1) //如果吃到食物
		{
			length++;
			rate = length / 3;
			if (length == MAX_LENGTH)
			{
				printf("您已通关！");
				break;
			}
			food(&foodAt, body, length, map); //生成食物
		}
		draw(length, foodAt, body, map); //画图
		if (result == -1) //如果死亡
		{
			break;
		}
		
	}
	Sleep(500);
	printf("  失败，此次得分为%d                          ", (length - 3) * 100);
	system("pause");
}
void init(int * length, point * foodAt, int * dir, point body[], char map[][SIZE]) //初始化
{
	memset(map, '*', SIZE * SIZE); //初始化地图
	body[0].r = 3, body[0].c = 2; //初始化蛇的身体
	body[1].r = 2, body[1].c = 2;
	body[2].r = 1, body[2].c = 2;
	*length = 3; //初始长度为3
	*dir = 2; //初始方向向下
	food(foodAt, body, *length, map); //生成食物
	draw(*length, *foodAt, body, map); //画图
	printf("按下任意键开始，用ASDW控制方向\n");
	_getch();
	srand((unsigned)time(NULL)); //生成随机数种子，备用
}
int getDir(int dir) //获取蛇的行进方向，规定返回值0代表向上，1代表向右，2代表向下，3代表向左
{
	char key;
	int newDir = dir;
	if (_kbhit())
	{
		key = _getch();
		switch (key)
		{
		case 'A': case 'a': newDir = 3; break;
		case 'S': case 's': newDir = 2; break;
		case 'D': case 'd': newDir = 1; break;
		case 'W': case 'w': newDir = 0; break;
		}
	}
	if (newDir - dir == 2 || newDir - dir == -2) //蛇不能反向
	{
		newDir = dir;
	}
	return newDir;
}
int move(point foodAt, int dir, int length, point body[]) //蛇的运动，规定返回值-1代表死亡，0代表没有吃到食物，1代表吃到食物
{
	int i, flag = 0;
	point head = body[0];
	switch (dir)
	{
	case 0: head.r -= 1; break;
	case 1: head.c += 1; break;
	case 2: head.r += 1; break;
	case 3: head.c -= 1; break;
	}
	if (head.r < 0 || head.r >= SIZE || head.c < 0 || head.c >= SIZE) //出界了死亡
	{
		return -1;
	}
	for (i = 0; i < length; i++)
	{
		if (head.r == body[i].r && head.c == body[i].c) //咬到了自己死亡
		{
			return -1;
		}
	}
	if (head.r == foodAt.r && head.c == foodAt.c) //吃到了食物
	{
		length++;
		flag = 1; //标记一下，便与等下返回值为1
	}
	for (i = length - 1; i > 0; i--) //移动蛇的位置
	{
		body[i] = body[i - 1];
	}
	body[0] = head; //换一个头
	if (flag == 1)
	{
		return 1;
	}
	return 0;
}
void draw(int length, point foodAt, point body[], char map[][SIZE]) //画图
{
	static char bitmap[SIZE + 2][SIZE + 2]; //定义一个数组，用于把地图背景、边界、蛇、食物都画上去
	static char buffer[SIZE * SIZE * 4]; //定义一个足够大的字符缓存区，用于缓存要画的字符,
	//输出到屏幕时一次性输出避免闪烁
	int i, j, counter, bufferSize = SIZE * SIZE * 4;
	for (i = 0; i < SIZE; i++) //背景
	{
		for (j = 0; j < SIZE; j++)
		{
			bitmap[i + 1][j + 1] = map[i][j];
		}
	}
	//边框
	bitmap[0][0] = '0', bitmap[0][SIZE + 1] = '1';
	bitmap[SIZE + 1][0] = '2', bitmap[SIZE + 1][SIZE + 1] = '3';
	for (i = 0; i < SIZE; i++)
	{
		bitmap[0][i + 1] = '4', bitmap[SIZE + 1][i + 1] = '4';
		bitmap[i + 1][0] = '5', bitmap[i + 1][SIZE + 1] = '5';
	}
	bitmap[foodAt.r + 1][foodAt.c + 1] = 'f'; //食物
	bitmap[body[0].r + 1][body[0].c + 1] = 'h'; //蛇头
	for (i = 1; i < length; i++) //蛇身
	{
		bitmap[body[i].r + 1][body[i].c + 1] = 'b';
	}

	counter = 0;
	for (i = 0; i < SIZE + 2; i++)
	{
		for (j = 0; j < SIZE + 2; j++)
		{
			switch (bitmap[i][j])
			{
			case 'f': counter += sprintf_s(buffer + counter, bufferSize - counter, "★"); break;
			case 'b': counter += sprintf_s(buffer + counter, bufferSize - counter, "●"); break;
			case 'h': counter += sprintf_s(buffer + counter, bufferSize - counter, "○"); break;
			case '0': counter += sprintf_s(buffer + counter, bufferSize - counter, "┏"); break;
			case '1': counter += sprintf_s(buffer + counter, bufferSize - counter, "━┓"); break;
			case '2': counter += sprintf_s(buffer + counter, bufferSize - counter, "┗"); break;
			case '3': counter += sprintf_s(buffer + counter, bufferSize - counter, "━┛"); break;
			case '4': counter += sprintf_s(buffer + counter, bufferSize - counter, " ━"); break;
			case '5': counter += sprintf_s(buffer + counter, bufferSize - counter, "┃ "); break;
			default: counter += sprintf_s(buffer + counter, bufferSize - counter, "▓");
			}
		}
		counter += sprintf_s(buffer + counter, bufferSize - counter, "\n");
	}
	system("cls");
	printf("%s", buffer);
}
void food(point * foodAt, point body[], int length, char map[][SIZE]) //生成食物
{
	int i;
	while (1)
	{
		foodAt->r = rand() % SIZE, foodAt->c = rand() % SIZE; //随机生成食物位置
		for (i = 0; i < length; i++)
		{
			if (foodAt->r == body[i].r && foodAt->c == body[i].c) //如果该位置在蛇的身体上
			{
				goto retry;
			}
		}
		break;
	retry:;
	}
}