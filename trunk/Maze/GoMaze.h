#pragma once
#include "StdAfx.h"
#include <stack>
#include "list.h"
using namespace std ;


enum{RIGHT,DOWN,UP,LEFT};
class CGoMaze
{
public:
	CGoMaze(void);
	void SetMaze(int nWidth,int nHeight,HWND hwnd,HINSTANCE hInst);
	~CGoMaze(void);
public:
	HWND hwndman;	//小人的句柄
private:
	int maze[60][60];		//迷宫数据
	HWND  hwndmaze[60][60];		//障碍物的句柄
	int m_wide,m_height;		//迷宫大小
	HWND hwnd;		//主窗口的句柄
	HBITMAP bitmapstone;		//障碍物的图标
	HBITMAP bitmapin,bitmapout;	//出入口的位图
	HBITMAP bitmapman;	//小人的位图
	HINSTANCE hInst;		//程序实例

	LinkList list;	//列表
	stack<POINT> s;		//堆栈

	POINT curPos,nextPos,endPos;		//移动方向
public:
	void DrawMaze(void);	//画迷宫
	int SurroundNum(int x,int y,BOOL inclu);
	void CreateMap();	//创建随机地图
	BOOL MoveMan(int dir);	//移动小人
	void FindPath();	//寻找路径
};
