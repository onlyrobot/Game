/********************************************************************************
*File name:Wuziqi1.0.c                                                          *
*Author:彭瑶                                                                    *
*Version:1.0                                                                    *
*Date:2018.04.13                                                                *
*Debug Environment:Visual Studio 2017                                           *
*Description:人机五子棋对战程序源代码（C语言），实现AI计算权值部分有多种方式，修*
*改时只需把相应的updateWeight方法取消注释，其他保留注释。                       *
*********************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<conio.h>
#include<time.h>
#define SIZE 15 //定义棋盘大小
#define SPACE 0 //定义棋盘没有子为0
#define AI 1 //AI下的子为1
#define PLAYER 2 //玩家下的子为2

typedef struct coordinate //保存座标的结构体
{
	int x;
	int y;
} coordinate;

void init(char board[SIZE][SIZE]); //游戏初始化
void drawBoard(char board[SIZE][SIZE]); //绘制棋盘
coordinate playerRound(char board[SIZE][SIZE]); //玩家回合
																   //根据上一个着棋点来判断游戏结果，返回值0代表游戏没结束，返回值1代表某一方获胜，返回值2代表和局
int judge(coordinate lastCoordinate, char board[SIZE][SIZE]);
//AI回合,根据玩家的着子位置来决定下哪里
coordinate AIRound(coordinate playerCoordinate, char board[SIZE][SIZE]);
//更新权值
void updateWeight(coordinate lastCoordinate, int weight[SIZE][SIZE],
	char board[SIZE][SIZE]);

int main()
{
	char board[SIZE][SIZE]; //定义棋盘
	coordinate lastCoordinate; //上一个着棋点
	int result; //用于保存游戏判断结果

	init(board); //游戏初始化
	while (1)
	{
		lastCoordinate = playerRound(board); //玩家回合
		{
			drawBoard(board); //绘制棋盘
			result = judge(lastCoordinate, board);//判断游戏结果
			if (result == 1)
			{
				printf("玩家胜\n");
				break;
			}
			else if (result == 2)
			{
				printf("和局\n");
				break;
			}
		}
		lastCoordinate = AIRound(lastCoordinate, board); //AI回合
		{
			drawBoard(board); //绘制棋盘
			result = judge(lastCoordinate, board);//判断游戏结果
			if (result == 1)
			{
				printf("AI胜\n");
				break;
			}
			else if (result == 2)
			{
				printf("和局\n");
				break;
			}
		}
	}
	system("pause");
}
void init(char board[SIZE][SIZE]) //游戏初始化
{
	srand((unsigned int)time(NULL)); //设置随机数种子备用
	memset(board, '\0', SIZE * SIZE); //棋盘初始化
	drawBoard(board); //绘制棋盘
}
void drawBoard(char board[SIZE][SIZE]) //绘制棋盘
{
	//定义一个足够大的字符数组来缓存输出图形（为了防止闪烁）
	static char buffer[SIZE * SIZE * 4];
	memset(buffer, '\0', SIZE * SIZE * 4);
	int i, j, counter, length = SIZE * SIZE * 4;
	counter = 0;
	system("cls"); //先清屏
	counter += sprintf_s(buffer + counter, length - counter, "x\\y");
	for (i = 0; i < SIZE; i++)
	{
		counter += sprintf_s(buffer + counter, length - counter, "%2d", i % 10);
	}
	counter += sprintf_s(buffer + counter, length - counter, "\n");
	for (i = 0; i < SIZE; i++)
	{
		counter += sprintf_s(buffer + counter, length - counter, "%2d ", i);
		for (j = 0; j < SIZE; j++)
		{
			if (board[i][j] != SPACE)
			{
				if (board[i][j] == PLAYER) //如果是玩家的棋子
				{
					counter += sprintf_s(buffer + counter, length - counter, "●");
				}
				else //否则是AI的棋子
				{
					counter += sprintf_s(buffer + counter, length - counter, "○");
				}
			}
			else
			{
				if (i == 0)
				{
					if (j == 0)
					{
						counter += sprintf_s(buffer + counter, length - counter, "┏ ");
					}
					else if (j == SIZE - 1)
					{
						counter += sprintf_s(buffer + counter, length - counter, "┓");
					}
					else
					{
						counter += sprintf_s(buffer + counter, length - counter, "┯ ");
					}
				}
				else if (i == SIZE - 1)
				{
					if (j == 0)
					{
						counter += sprintf_s(buffer + counter, length - counter, "┗ ");
					}
					else if (j == SIZE - 1)
					{
						counter += sprintf_s(buffer + counter, length - counter, "┛");
					}
					else
					{
						counter += sprintf_s(buffer + counter, length - counter, "┷ ");
					}
				}
				else
				{
					if (j == 0)
					{
						counter += sprintf_s(buffer + counter, length - counter, "┠ ");
					}
					else if (j == SIZE - 1)
					{
						counter += sprintf_s(buffer + counter, length - counter, "┨");
					}
					else
					{
						counter += sprintf_s(buffer + counter, length - counter, "┼ ");
					}
				}
			}
		}
		counter += sprintf_s(buffer + counter, length - counter, "\n");
	}
	puts(buffer);
}
coordinate playerRound(char board[SIZE][SIZE]) //玩家回合
{
	coordinate coordinate; //保存玩家下棋座标
	int x, y;
	while (1)
	{
		scanf_s("%d %d", &x, &y);
		if (x < 0 || x >= SIZE || y < 0 || y >= SIZE || board[x][y] != SPACE)
		{
			printf("输入不合法\n");
			while (getchar() != '\n');
		}
		else
		{
			coordinate.x = x; //玩家输入的座标转化为
			coordinate.y = y; //程序中棋盘的座标
			break;
		}
	}
	board[coordinate.x][coordinate.y] = PLAYER;
	return coordinate;
}
//根据上一个着棋点来判断游戏结果，返回值0代表游戏没结束，返回值1代表某一方获胜，返回值2代表和局
int judge(coordinate lastCoordinate, char board[SIZE][SIZE])
{
	int i, j, k, l, n, m, counter, x = lastCoordinate.x, y = lastCoordinate.y;
	char flag = board[x][y]; //上一个着棋点是哪一方的
							 //计算出扫描的矩形范围，i为上边界，j为下边界，k为左边界，l为右边界
	i = 0 < x - 4 ? x - 4 : 0, j = SIZE - 1 > x + 4 ? x + 4 : SIZE - 1;
	k = 0 < y - 4 ? y - 4 : 0, l = SIZE - 1 > y + 4 ? y + 4 : SIZE - 1;
	//横向判断是否有五子连线
	counter = 0;
	for (n = k; n <= l; n++)
	{
		if (board[x][n] == flag)
		{
			counter++;
		}
		else
		{
			counter = 0;
		}
		if (counter >= 5)
		{
			return 1;
		}
	}
	//纵向判断是否有五子连线
	counter = 0;
	for (m = i; m <= j; m++)
	{
		if (board[m][y] == flag)
		{
			counter++;
		}
		else
		{
			counter = 0;
		}
		if (counter >= 5)
		{
			return 1;
		}
	}
	//捺向判断是否有五子连线
	counter = 0;
	for (m = i; m <= j; m++)
	{
		for (n = k; n <= l; n++)
		{
			if (x - m == y - n)
			{
				if (board[m][n] == flag)
				{
					counter++;
				}
				else
				{
					counter = 0;
				}
			}
			if (counter >= 5)
			{
				return 1;
			}
		}
	}
	//撇向判断是否有五子连线
	counter = 0;
	for (m = i; m <= j; m++)
	{
		for (n = k; n <= l; n++)
		{
			if (x - m == n - y)
			{
				if (board[m][n] == flag)
				{
					counter++;
				}
				else
				{
					counter = 0;
				}
			}
			if (counter >= 5)
			{
				return 1;
			}
		}
	}
	//判断是否和局
	for (m = 0; m < SIZE - 1; m++)
	{
		for (n = 0; n < SIZE - 1; n++)
		{
			if (board[m][n] == SPACE) //如果棋盘还有空位，那么游戏没有结束
			{
				return 0;
			}
		}
	}
	return 2; //如果没有空位了，那么和棋
}
//AI回合
coordinate AIRound(coordinate LastCoordinate, char board[SIZE][SIZE])
{
	int i, j, maxWeight = 0, counter = 0;
	static int weight[SIZE][SIZE] = { 0 }; //局部静态数组，用于保存权值
	coordinate coordinate; //保存AI下棋座标
	//AI每次下棋前都会先根据玩家上一步下棋位置来更新权值，再选择权值最大的点来下棋，下完棋再次更新权值
	updateWeight(LastCoordinate, weight, board); //先更新权值
	for (i = 0; i < SIZE; i++) //求得最大权值，以及最大权值的点的个数
	{
		for (j = 0; j < SIZE; j++)
		{
			if (weight[i][j] > maxWeight)
			{
				maxWeight = weight[i][j];
				counter = 1;
			}
			else if (weight[i][j] == maxWeight)
			{
				counter++;
			}
		}
	}
	//从最大权值点中随机选取一个最为下棋点
	int temp = rand() % counter + 1;
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			if (weight[i][j] == maxWeight)
			{
				temp--;
				if (temp == 0)
				{
					coordinate.x = i;
					coordinate.y = j;
					goto outer;
				}
			}
		}
	}
outer:
	board[coordinate.x][coordinate.y] = AI;
	updateWeight(coordinate, weight, board); //再次更新权值
	return coordinate;
}
//以下为多个updateWeight方法（约定最小权值为-1，表示改点已有棋子）
//这个最简单，AI也最愚蠢
/*void updateWeight(coordinate lastCoordinate, int weight[SIZE][SIZE],
char board[SIZE][SIZE])
{
int x = lastCoordinate.x, y = lastCoordinate.y, i, j, k, l, m, n;
weight[x][y] = -1;
//计算出权值被影响到的区域，i为上边界，j为下边界，k为左边界，l为右边界
i = 0 < x - 4 ? x - 4 : 0, j = SIZE - 1 > x + 4 ? x + 4 : SIZE - 1;
k = 0 < y - 4 ? y - 4 : 0, l = SIZE - 1 > y + 4 ? y + 4 : SIZE - 1;
for (m = i; m <= j; m++)
{
for (n = k; n <= l; n++)
{
weight[m][n]++; //最简单的权值计算方法，每被影响到一次，权值加一
}
}
}*/
void updateWeight(coordinate lastCoordinate, int weight[SIZE][SIZE],
	char board[SIZE][SIZE])
{
	int x = lastCoordinate.x, y = lastCoordinate.y, i, j, k, l, m, n, o, p, counter;
	weight[x][y] = -1;
	//计算出权值被影响到的区域，i为上边界，j为下边界，k为左边界，l为右边界
	i = 0 < x - 4 ? x - 4 : 0, j = SIZE - 1 > x + 4 ? x + 4 : SIZE - 1;
	k = 0 < y - 4 ? y - 4 : 0, l = SIZE - 1 > y + 4 ? y + 4 : SIZE - 1;
	for (m = i; m <= j; m++)
	{
		for (n = k; n <= l; n++) //对权值被影响到的点进行遍历
		{
			//如果该点没有着棋，那么权值加上一个与着棋点之间距离成反比的数
			if (board[m][n] == SPACE)
			{
				if (m == x || n == y || abs(m - x) == abs(n - y)) //保证在同一条直线上
				{
					weight[m][n] += (int)(6 / sqrt((m - x) * (m - x) + (n - y) * (n - y)));
				}
			}
			//如果该点已经着了相对的棋，那么这个点与上一个着棋点之间的点权值减去一个与两点距离成反比，
			//与相对点数量成正比的数
			else if (board[m][n] != board[x][y])

			{
				counter = 1;
				for (o = x > m ? m : x; o < x || o < m; o++)
				{
					for (p = y > n ? n : y; p < y || p < n; p++) //遍历两点之间的点
					{
						if (board[o][p] == SPACE)
						{
							weight[o][p] -= (int)(5 * counter / sqrt((o - x) * (o - x) + (p - y) * (p - y)));
						}
						else if (board[o][p] != board[x][y])
						{
							counter++;
						}
						else
						{
							goto outer1;
						}
						if (weight[o][p] < 0)
						{
							weight[o][p] = 0;
						}
					}
				}
			outer1:;
			}
			//如果该点已经着了相同的棋子，那么这个点与上一个着棋点之间的点权值加上一个与距离成反比，
			//与相同点数量成正比的数
			else				
			{
				counter = 1;
				for (o = x > m ? m : x; o < x || o < m; o++)
				{
					for (p = y > n ? n : y; p < y || p < n; p++) //遍历两点之间的点
					{
						if (board[o][p] == SPACE)
						{
							weight[o][p] += (int)(6 * counter / sqrt((o - x) * (o - x) + (p - y) * (p - y)));
						}
						else if (board[o][p] == board[x][y])
						{
							counter++;
						}
						else
						{
							goto outer2;
						}
					}
				}
			outer2:;
			}
		}
	}
}