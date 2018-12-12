/********************************************************************************
*File name:MineSweeper1.0.c                                                     *
*Author:彭瑶                                                                    *
*Version:1.0                                                                    *
*Date:2018.04.20                                                                *
*Debug Environment:Visual Studio 2017                                           *
*Description:扫雷小游戏，只有最简单功能的第一版本                               *
*********************************************************************************/
#include<stdio.h>
#include<time.h>
#include<conio.h>
#include<Windows.h>
#define SIZE 15

typedef struct point //用于记录地图上的某个点
{
	int r; //行
	int c; //列
} point;
HANDLE stdOutput; //声明标准输出句柄

void init(char map[][SIZE], point * cursor, int * mineNum); //初始化
void draw(char map[][SIZE], point cursor); //画图
int process(int command, point * cursor, int * remain, char map[][SIZE]); //处理输入的指令，
//返回值-1代表游戏失败，1代表游戏胜利
int getMineNum(point center, char map[][SIZE]);

int main()
{
	char map[SIZE][SIZE]; //定义地图，用-1表示安全区，-2表示雷区，-3表示被标记的安全区，
	//-4表示被标记的雷区, 0-8均表示已经扫过的安全区，数字代表周围的雷数，9代表显示出来的雷
	point cursor; //保存光标的位置（游戏光标，非系统光标）
	int command, result, remain; //remain代表剩余需要扫除的区域
	init(map, &cursor, &remain); //初始化地图，光标位置和雷的数量
	while (1)
	{
		if (_kbhit()) //当有指令输入时
		{
			command = _getch(); //获得指令
			result = process(command, &cursor, &remain, map); //处理指令，返回游戏结果
			draw(map, cursor); //画图
			if (result == 1)
			{
				printf("你赢了");
				break;
			}
			else if (result == -1)
			{
				printf("你输了");
				break;
			}
		}
	}
	system("pause");
}

void init(char map[][SIZE], point * cursor, int * remain) //初始化
{
	int difficulty = 3, i, j; //难度用来控制生成的雷区和安全区比例
	stdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cci; //隐藏系统光标
	cci.bVisible = 0;
	cci.dwSize = 1;
	SetConsoleCursorInfo(stdOutput, &cci);

	srand((unsigned)(time(NULL))); //设置随机数种子
	*remain = 0;
	cursor->r = SIZE / 2, cursor->c = SIZE / 2; //把光标设为地图中间
	for (i = 0; i < SIZE; i++) //为地图初始化
	{
		for (j = 0; j < SIZE; j++)
		{
			if (rand() % 10 < difficulty) //将该点设为雷区
			{
				map[i][j] = -2; 
			}
			else //将该点设为安全区
			{
				map[i][j] = -1;
				(*remain)++;
			}
		}
	}
	draw(map, *cursor);
}
void draw(char map[][SIZE], point cursor)
{
	int i, j, k;
	COORD coord = { 0, 0 }; //座标
	SetConsoleCursorPosition(stdOutput, coord); //设定光标座标
	printf("  按下空格标记（★），按下A确定\n        ASDW控制光标移动\n");
	for (i = 0; i < SIZE; i++)
	{
		if (i == 0) //输出第一行边框
		{
			printf("┏");
			for (k = 0; k < SIZE; k++)
			{
				printf(" ━");
			}
			printf("━┓\n");
		}
		for (j = 0; j < SIZE; j++) //输出地图内容
		{
			if (j == 0)
			{
				printf("┃ ");
			}
			if (i == cursor.r && j == cursor.c) //如果当前游戏光标在这个区域
			{
				SetConsoleTextAttribute(stdOutput, FOREGROUND_BLUE); //把下个字符的颜色变为蓝色
			} 
			switch (map[i][j])
			{
			case -1: case -2: printf("■"); break; //没扫过的地方用■表示
			case -3: case -4: printf("★"); break; //标记过的地方用★表示
			case 9: 
				SetConsoleTextAttribute(stdOutput, FOREGROUND_RED); //把下个字符的颜色变为红色
				printf("●"); 
				SetConsoleTextAttribute(stdOutput,
					FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); //设置回去原来的颜色
				break; //显示出来的雷用红色的●表示
			default: printf("%02d", map[i][j]); //已经扫过的地方用数字表示周围雷的数量
			}
			if (j == SIZE - 1)
			{
				printf("┃");
			}
			if (i == cursor.r && j == cursor.c)
			{
				SetConsoleTextAttribute(stdOutput, 
					FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); //设置回去原来的颜色
			}
		}
		printf("\n");
		if (i == SIZE - 1) //输出最后一行边框
		{
			printf("┗");
			for (k = 0; k < SIZE; k++)
			{
				printf(" ━");
			}
			printf("━┛\n");
		}
	}
}
int process(int command, point * cursor, int * remain, char map[][SIZE]) //处理输入的指令，返回值-1代表游戏结束
//1代表游戏胜利
{
	int i, j;
	switch (command)
	{
	case 'j': case 'J': 
		if (map[cursor->r][cursor->c] == -1) //如果是安全区，则计算其周围的雷的数量，以及判断是否游戏胜利
		{
			map[cursor->r][cursor->c] = getMineNum(*cursor, map);
			(*remain)--; 
			if (*remain == 0) //如果没有需要扫除的区域，则胜利
			{
				return 1;
			}
		}
		else if (map[cursor->r][cursor->c] == -2) //如果是雷，游戏结束，显示所有未扫过的区域的雷
		{
			for (i = 0; i < SIZE; i++)
			{
				for (j = 0; j < SIZE; j++)
				{
					if (map[i][j] == -2 || map[i][j] == -4) //如果是雷，则显示出来
					{
						map[i][j] = 9;
					}
				}
			}
			return -1;
		} break;
	case ' ': 
		if (map[cursor->r][cursor->c] == -1 || map[cursor->r][cursor->c] == -2) //如果未被标记
		{
			map[cursor->r][cursor->c] -= 2; //将该点标记
		} 
		else if (map[cursor->r][cursor->c] == -3 || map[cursor->r][cursor->c] == -4) //如果已经被标记
		{
			map[cursor->r][cursor->c] += 2; //取消该点的标记
		} break;
	case 'A': case 'a': cursor->c -= cursor->c > 0 ? 1 : 0; break; //光标左移
	case 'S': case 's': cursor->r += cursor->r < SIZE -1 ? 1 : 0; break; //光标下移
	case 'D': case 'd': cursor->c += cursor->c < SIZE - 1 ? 1 : 0; break; //光标右移
	case 'W': case 'w': cursor->r -= cursor->r > 0 ? 1 : 0; break; //光标上移
	}
	return 0;
}
int getMineNum(point center, char map[][SIZE]) //计算center点周围的雷数量
{
	int i, j, k, l, m, n, x = center.r, y = center.c, counter = 0;
	//计算出center点周围的范围，i为上边界，j为下边界，k为左边界，l为右边界
	i = 0 < x - 1 ? x - 1 : 0, j = SIZE - 1 > x + 1 ? x + 1 : SIZE - 1;
	k = 0 < y - 1 ? y - 1 : 0, l = SIZE - 1 > y + 1 ? y + 1 : SIZE - 1;
	for (m = i; m <= j; m++)
	{
		for (n = k; n <= l; n++)
		{
			if (map[m][n] == -2 || map[m][n] == -4)
			{
				counter++;
			}
		}
	}
	return counter;
}