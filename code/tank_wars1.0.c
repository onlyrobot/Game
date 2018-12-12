/********************************************************************************
*File name:TankWars1.0.c                                                        *
*Author:彭瑶                                                                    *
*Version:1.0                                                                    *
*Date:2018.06.02                                                                *
*Debug Environment:Visual Studio 2017                                           *
*Description:坦克大战简单版（C语言代码）                                        *
* *
*                                       *
*                                                                               *
*********************************************************************************/

#include<stdio.h>
#include<conio.h>
#include<time.h>
#include<Windows.h>

#define SIZE 16 //定义地图大小，一个坦克占3*3个格子

typedef struct area //地图上的点，该结构体包含点的行r，列c，代表的东西thing，和方向dir
{
	char thing; //这个点代表的东西，用'\0'表示空，用't'代表坦克，用'b'代表炮弹
	//由于坦克要用3*3个点来表示，这里用坦克中心的点来代表坦克
	char dir; //这个点上的东西的方向，用't'表示上，'r'表示右，'b'表示下，'l'表示左
	//当该点是坦克时，方向表示炮口指向的方向，当为子弹时，表示子弹的运动方向
} area;
typedef struct point //地图上区域的座标，'r'表示行，'c'表示列
{
	int r;
	int c;
} point;



int main()
{
	area map[SIZE][SIZE]; //地图，由结构体point组成的二维数组
	point myTank; //我的坦克所在区域的座标
	char dir; //玩家输入的运动方向
	int i, result; //result保存游戏的结果，1表示失败0表示没有失败


	//初始化
	init(map, &myTank);

	while (1)
	{
		for (i = 0; i < 3; i++)
		{
			Sleep(30);
			dir = getDir();
			if (dir != '\0')
			{
				result = move(dir, map, &myTank); //玩家控制的坦克的移动
			}
			draw(map, &myTank); //控制台画出坦克
			if (result == 1)
			{
				printf("游戏失败");
				break;
			}
		}
		result = environmentMove(map, &myTank); //环境移动，包含AI坦克的移动和子弹的移动
		draw(map, &myTank); //控制台画出坦克
		if (result == 1)
		{
			printf("游戏失败");
			break;
		}
	}

}

char getDir() //获取玩家输入的行进方向
{
	char key, dir = '\0';
	if (_kbhit())
	{
		key = _getch();
		switch (key)
		{
		case 'A': case 'a': dir = 'l'; break;
		case 'S': case 's': dir = 'b'; break;
		case 'D': case 'd': dir = 'r'; break;
		case 'W': case 'w': dir = 't'; break;
		}
	}
	return dir;
}
char move(char dir, area map[SIZE][SIZE] , point *myTank) //玩家控制的坦克移动
{
	if (dir == map[myTank->r][myTank->c].dir) //如果玩家输入的运动方向和坦克当前朝向相同
	{
		switch (dir)
		{
		case 't': 
			if (myTank->r > 1) //如果向上移动不会超出地图边界
			{
				//下面实现坦克的移动
				memcpy(&map[myTank->r - 1][myTank->c], &map[myTank->r][myTank->c], sizeof(area));
				memset(&map[myTank->r][myTank->c], '\0', sizeof(area));
				myTank->r--;
			}
			break;
		case 'r': 
			if (myTank->c < SIZE - 2) //如果向右移动不会超出地图边界
			{
				//下面实现坦克的移动
				memcpy(&map[myTank->r][myTank->c + 1], &map[myTank->r][myTank->c], sizeof(area));
				memset(&map[myTank->r][myTank->c], '\0', sizeof(area));
				myTank->c++;
			}
			break;
		case 'b':
			if (myTank->r < SIZE - 2) //如果向下移动不会超出地图边界
			{
				//下面实现坦克的移动
				memcpy(&map[myTank->r + 1][myTank->c], &map[myTank->r][myTank->c], sizeof(area));
				memset(&map[myTank->r][myTank->c], '\0', sizeof(area));
				myTank->r++;
			}
			break;
		case 'l':
			if (myTank->c > 1) //如果向左移动不会超出地图边界
			{
				//下面实现坦克的移动
				memcpy(&map[myTank->r][myTank->c - 1], &map[myTank->r][myTank->c], sizeof(area));
				memset(&map[myTank->r][myTank->c], '\0', sizeof(area));
				myTank->c--;
			}
			break;
		}

		//判断坦克移动后是否与炮弹相撞而游戏失败

	}
	else //否则改变坦克的朝向
	{
		map[myTank->r][myTank->c].dir = dir;
	}
	return 0;
}
void draw(area map[SIZE][SIZE], point *myTank)
{

}