#include "StdAfx.h"
#include "GoMaze.h"
#include "Resource.h"

//生成随数，并且【range_min<=u<range_max】,看清，一般range_max+1；
int RangedRand(int range_min, int range_max)
{
		int u = rand();
		 u = (double)u/ (RAND_MAX + 1) * (range_max - range_min)+ range_min;
		return u;
}


//设置地图
void CGoMaze::SetMaze(int nWidth,int nHeight,HWND hwnd,HINSTANCE hInst)
{
	bitmapstone = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_STONE));
	bitmapin = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_IN));
	bitmapout = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_OUT));
	bitmapman = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_MAN));
	this->hwnd = hwnd;
	m_wide = nWidth;
	m_height = nHeight;
	this->hInst = hInst;
	CreateMap();//随机生成地图
}

CGoMaze::CGoMaze(void)
{
	InitList(list);
}

CGoMaze::~CGoMaze(void)
{//销毁窗口
	for(int i =0;i<m_height;i++)
		for(int j =0;j<m_wide;j++)
		{
			if (maze[i][j] == 1)
			{
				DestroyWindow(hwndmaze[i][j]);
			}
		}
		DestroyWindow(hwndmaze[0][0]);
		DestroyWindow(hwndmaze[m_height-1][m_wide-1]);
		DestroyWindow(hwndman);
		DestroyList(list);
}

void CGoMaze::DrawMaze(void)
{
	int ID = 2000;
	for(int i =0;i<m_height;i++)
		for(int j =0;j<m_wide;j++)
		{
			if (maze[i][j] == 1)
			{
				hwndmaze[i][j] = CreateWindow(_T("static"),NULL,SS_BITMAP |WS_CHILD|WS_VISIBLE,j*33,i*33,33,33,hwnd,(HMENU)(ID++),hInst,NULL);
				SendMessage(hwndmaze[i][j],STM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)bitmapstone);
			}
			else
				maze[i][j] = 0;
		}
		curPos.x=0;curPos.y=0;
		endPos.x=m_height-1;endPos.y=m_wide-1;
		//小人的图标
		hwndman = CreateWindow(_T("man"),NULL,SS_BITMAP |WS_CHILD|WS_VISIBLE,0,0,33,33,hwnd,(HMENU)(ID++),hInst,NULL);
		SendMessage(hwndman,STM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)bitmapman);
		//出入口图标
		hwndmaze[0][0] = CreateWindow(_T("static"),NULL,SS_BITMAP |WS_CHILD|WS_VISIBLE,0,0,33,33,hwnd,(HMENU)(ID++),hInst,NULL);
		SendMessage(hwndmaze[0][0],STM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)bitmapin);
		hwndmaze[m_height-1][m_wide-1] = CreateWindow(_T("static"),NULL,SS_BITMAP |WS_CHILD|WS_VISIBLE,(m_wide-1)*33,(m_height-1)*33,33,33,hwnd,(HMENU)(ID++),hInst,NULL);
		SendMessage(hwndmaze[m_height-1][m_wide-1],STM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)bitmapout);
}

//inclu为TRUE时包括边,
//获取每一个点的信息，当inclu为false时计算指定点附近可以打通的墙的个数
//当inclu为false时计算指定点附近可以打通的墙的个数,包括地图边界.
int CGoMaze::SurroundNum(int x,int y,BOOL inclu)
{
	int i =1;
	int num = 0;
	POINT temp;
	int next_x=x+1,next_y=y+1,prev_x=x-1,prev_y=y-1;
	if(!inclu) ClearList(list);
	//下
	if (next_x>=m_height)
	{
		if(inclu)
		num++;
	}
	else
	{
		if (maze[next_x][y] == 1)
		{
			temp.x = next_x;
			temp.y = y;
			if(!inclu) ListInsert(list,i++,temp);
			num ++;
		}
	}
	//右
	if (next_y>=m_wide)
	{
		if(inclu)
		num++;
	}
	else
	{
		if (maze[x][next_y] == 1)
		{
			temp.x = x;
			temp.y = next_y;
			if(!inclu) ListInsert(list,i++,temp);
			num ++;
		}
	}
	//上
	if (prev_x<0)
	{
		if(inclu)
		num++;
	}
	else
	{
		if (maze[prev_x][y] == 1)
		{
			temp.x = prev_x;
			temp.y = y;
			if(!inclu) ListInsert(list,i++,temp);
			num++;
		}
	}
	//左
	if (prev_y<0)
	{
		if(inclu)
		num++;
	}
	else
	{
		if (maze[x][prev_y] == 1)
		{
			temp.x = x;
			temp.y = prev_y;
			if(!inclu) ListInsert(list,i++,temp);
			num++;
		}
	}
	return num;
}

//随机生成地图
void CGoMaze::CreateMap()
{
	int n;//几条路可以走
	int i,num;//num表示指定点周围的墙和障碍物的个数
	POINT temp;
	//数据初始化,1表示障碍物，0表示通路
	curPos.x=0;curPos.y=0;
	endPos.x=m_height-1;endPos.y=m_wide-1;
	for(int i =0;i<m_height;i++)
		for(int j =0;j<m_wide;j++)
		{
			maze[i][j] = 1;
		}
		//起点和终点都设置为0；
		maze[0][0] = 0;
		maze[endPos.x][endPos.y] = 0;

//开始打墙
	while (!(curPos.x== endPos.x&&curPos.y==endPos.y))
	{//判断有几条路可以走
		n = SurroundNum(curPos.x,curPos.y,FALSE);
		while (n)//试探各个附近墙壁,有n个墙壁，
		{
			i = RangedRand(1,n+1);
			GetElem(list,i,nextPos);
			num = SurroundNum(nextPos.x,nextPos.y,TRUE);
			if (num>=3)
			{//能够打通
				s.push(curPos);
				curPos = nextPos;
				maze[nextPos.x][nextPos.y] = 0;
				break;
			}
			else{//不能打通，减少一条通路
				n--;
				ListDelete(list,i,temp);
				}
		}
	if (!n)//n == 0；当点到达死路的时候，向后退。
	{
		maze[curPos.x][curPos.y] = 2;
		if (!s.empty())
		{
			POINT & t = s.top();
			curPos = t;
			s.pop();
		}
		else
		{//设置右下角的障碍物
			curPos = endPos;
			while (1)
			{
					n = SurroundNum(curPos.x,curPos.y,FALSE);
					i = RangedRand(1,n+1);
					GetElem(list,i,nextPos);
					num = SurroundNum(nextPos.x,nextPos.y,TRUE);
					if (n>=2||num>=2)
					{//能够打通
						curPos = nextPos;
						maze[nextPos.x][nextPos.y] = 0;
						break;
					}
					else{
						curPos.x = 0;curPos.y = 0;
						return;
					}
			}
		}
	}
	}
}

//移动小人
BOOL CGoMaze::MoveMan(int dir)
{
	switch (dir)
	{
	case LEFT:
		nextPos.x = curPos.x;
		nextPos.y = curPos.y-1;
		break;
	case RIGHT:
		nextPos.x = curPos.x;
		nextPos.y = curPos.y+1;
		break;
	case UP:
		nextPos.x = curPos.x-1;
		nextPos.y = curPos.y;
		break;
	case DOWN:
		nextPos.x = curPos.x+1;
		nextPos.y = curPos.y;
		break;
	}
	//超过边界
	if (nextPos.x<0||nextPos.y<0||nextPos.x>=m_height||nextPos.y>=m_wide)
	{
		return FALSE;
	}
	//只有0才通过,若为1，2，3都不能通过
	if (!maze[nextPos.x][nextPos.y])
	{
		MoveWindow(hwndman,nextPos.y*33,nextPos.x*33,33,33,TRUE);
		curPos = nextPos;
		if(curPos.x==endPos.x&&curPos.y==endPos.y)
		{
			MessageBox(hwnd,_T("恭喜通关！"),_T("恭喜"),MB_OK|MB_ICONINFORMATION);
		}
		return TRUE;
	}
	return FALSE;
}


//在maze二维数组中寻路
void CGoMaze::FindPath()
{
	curPos.x=0;curPos.y=0;
	endPos.x=m_height-1;endPos.y=m_wide-1;
	BOOL IsDead = FALSE;	//是否为死路
	POINT temp;		//保存上一个点
	while (1)
	{
		for(int dir =RIGHT;dir<=LEFT;dir++)//循环试探4个方向
		{//dir为方向enum{RIGHT,DOWN,UP,LEFT};更改可以改变优先级
			temp = curPos;
			if (MoveMan(dir))//若向一个方向移动小人成功，则返回TRUE，并且小人已移动，否则小人未移动，返回FALSE
			{
				Sleep(100);
				IsDead = FALSE;
				maze[temp.x][temp.y] = 2;//2代表此点为走过的路
				s.push(temp);//把前一个点压入栈
				break;
			}
			else
			{
				IsDead = TRUE;
			}
		}
		//遇到死路，弹出堆栈
		if (IsDead)
		{
			if (s.empty())//堆栈为空，迷宫没有路
			{
				MessageBox(hwnd,_T("没有通路！"),_T("失败"),MB_OK|MB_ICONWARNING);
				return;
			}else{
				maze[curPos.x][curPos.y] = 3;//3代表此点为死路。
				POINT & t = s.top();
				curPos = t;
				s.pop();
				MoveWindow(hwndman,curPos.y*33,curPos.x*33,33,33,TRUE);//向后退
				Sleep(100);
			}
		}
		
		if(curPos.x==endPos.x&&curPos.y==endPos.y)
		{//到达终点,终止循环
			return;
		}
	}
}