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
	HWND hwndman;	//С�˵ľ��
private:
	int maze[60][60];		//�Թ�����
	HWND  hwndmaze[60][60];		//�ϰ���ľ��
	int m_wide,m_height;		//�Թ���С
	HWND hwnd;		//�����ڵľ��
	HBITMAP bitmapstone;		//�ϰ����ͼ��
	HBITMAP bitmapin,bitmapout;	//����ڵ�λͼ
	HBITMAP bitmapman;	//С�˵�λͼ
	HINSTANCE hInst;		//����ʵ��

	LinkList list;	//�б�
	stack<POINT> s;		//��ջ

	POINT curPos,nextPos,endPos;		//�ƶ�����
public:
	void DrawMaze(void);	//���Թ�
	int SurroundNum(int x,int y,BOOL inclu);
	void CreateMap();	//���������ͼ
	BOOL MoveMan(int dir);	//�ƶ�С��
	void FindPath();	//Ѱ��·��
};
