#include "StdAfx.h"
#include "GoMaze.h"
#include "Resource.h"

//�������������ҡ�range_min<=u<range_max��,���壬һ��range_max+1��
int RangedRand(int range_min, int range_max)
{
		int u = rand();
		 u = (double)u/ (RAND_MAX + 1) * (range_max - range_min)+ range_min;
		return u;
}


//���õ�ͼ
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
	CreateMap();//������ɵ�ͼ
}

CGoMaze::CGoMaze(void)
{
	InitList(list);
}

CGoMaze::~CGoMaze(void)
{//���ٴ���
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
		//С�˵�ͼ��
		hwndman = CreateWindow(_T("man"),NULL,SS_BITMAP |WS_CHILD|WS_VISIBLE,0,0,33,33,hwnd,(HMENU)(ID++),hInst,NULL);
		SendMessage(hwndman,STM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)bitmapman);
		//�����ͼ��
		hwndmaze[0][0] = CreateWindow(_T("static"),NULL,SS_BITMAP |WS_CHILD|WS_VISIBLE,0,0,33,33,hwnd,(HMENU)(ID++),hInst,NULL);
		SendMessage(hwndmaze[0][0],STM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)bitmapin);
		hwndmaze[m_height-1][m_wide-1] = CreateWindow(_T("static"),NULL,SS_BITMAP |WS_CHILD|WS_VISIBLE,(m_wide-1)*33,(m_height-1)*33,33,33,hwnd,(HMENU)(ID++),hInst,NULL);
		SendMessage(hwndmaze[m_height-1][m_wide-1],STM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)bitmapout);
}

//incluΪTRUEʱ������,
//��ȡÿһ�������Ϣ����incluΪfalseʱ����ָ���㸽�����Դ�ͨ��ǽ�ĸ���
//��incluΪfalseʱ����ָ���㸽�����Դ�ͨ��ǽ�ĸ���,������ͼ�߽�.
int CGoMaze::SurroundNum(int x,int y,BOOL inclu)
{
	int i =1;
	int num = 0;
	POINT temp;
	int next_x=x+1,next_y=y+1,prev_x=x-1,prev_y=y-1;
	if(!inclu) ClearList(list);
	//��
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
	//��
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
	//��
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
	//��
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

//������ɵ�ͼ
void CGoMaze::CreateMap()
{
	int n;//����·������
	int i,num;//num��ʾָ������Χ��ǽ���ϰ���ĸ���
	POINT temp;
	//���ݳ�ʼ��,1��ʾ�ϰ��0��ʾͨ·
	curPos.x=0;curPos.y=0;
	endPos.x=m_height-1;endPos.y=m_wide-1;
	for(int i =0;i<m_height;i++)
		for(int j =0;j<m_wide;j++)
		{
			maze[i][j] = 1;
		}
		//�����յ㶼����Ϊ0��
		maze[0][0] = 0;
		maze[endPos.x][endPos.y] = 0;

//��ʼ��ǽ
	while (!(curPos.x== endPos.x&&curPos.y==endPos.y))
	{//�ж��м���·������
		n = SurroundNum(curPos.x,curPos.y,FALSE);
		while (n)//��̽��������ǽ��,��n��ǽ�ڣ�
		{
			i = RangedRand(1,n+1);
			GetElem(list,i,nextPos);
			num = SurroundNum(nextPos.x,nextPos.y,TRUE);
			if (num>=3)
			{//�ܹ���ͨ
				s.push(curPos);
				curPos = nextPos;
				maze[nextPos.x][nextPos.y] = 0;
				break;
			}
			else{//���ܴ�ͨ������һ��ͨ·
				n--;
				ListDelete(list,i,temp);
				}
		}
	if (!n)//n == 0�����㵽����·��ʱ������ˡ�
	{
		maze[curPos.x][curPos.y] = 2;
		if (!s.empty())
		{
			POINT & t = s.top();
			curPos = t;
			s.pop();
		}
		else
		{//�������½ǵ��ϰ���
			curPos = endPos;
			while (1)
			{
					n = SurroundNum(curPos.x,curPos.y,FALSE);
					i = RangedRand(1,n+1);
					GetElem(list,i,nextPos);
					num = SurroundNum(nextPos.x,nextPos.y,TRUE);
					if (n>=2||num>=2)
					{//�ܹ���ͨ
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

//�ƶ�С��
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
	//�����߽�
	if (nextPos.x<0||nextPos.y<0||nextPos.x>=m_height||nextPos.y>=m_wide)
	{
		return FALSE;
	}
	//ֻ��0��ͨ��,��Ϊ1��2��3������ͨ��
	if (!maze[nextPos.x][nextPos.y])
	{
		MoveWindow(hwndman,nextPos.y*33,nextPos.x*33,33,33,TRUE);
		curPos = nextPos;
		if(curPos.x==endPos.x&&curPos.y==endPos.y)
		{
			MessageBox(hwnd,_T("��ϲͨ�أ�"),_T("��ϲ"),MB_OK|MB_ICONINFORMATION);
		}
		return TRUE;
	}
	return FALSE;
}


//��maze��ά������Ѱ·
void CGoMaze::FindPath()
{
	curPos.x=0;curPos.y=0;
	endPos.x=m_height-1;endPos.y=m_wide-1;
	BOOL IsDead = FALSE;	//�Ƿ�Ϊ��·
	POINT temp;		//������һ����
	while (1)
	{
		for(int dir =RIGHT;dir<=LEFT;dir++)//ѭ����̽4������
		{//dirΪ����enum{RIGHT,DOWN,UP,LEFT};���Ŀ��Ըı����ȼ�
			temp = curPos;
			if (MoveMan(dir))//����һ�������ƶ�С�˳ɹ����򷵻�TRUE������С�����ƶ�������С��δ�ƶ�������FALSE
			{
				Sleep(100);
				IsDead = FALSE;
				maze[temp.x][temp.y] = 2;//2����˵�Ϊ�߹���·
				s.push(temp);//��ǰһ����ѹ��ջ
				break;
			}
			else
			{
				IsDead = TRUE;
			}
		}
		//������·��������ջ
		if (IsDead)
		{
			if (s.empty())//��ջΪ�գ��Թ�û��·
			{
				MessageBox(hwnd,_T("û��ͨ·��"),_T("ʧ��"),MB_OK|MB_ICONWARNING);
				return;
			}else{
				maze[curPos.x][curPos.y] = 3;//3����˵�Ϊ��·��
				POINT & t = s.top();
				curPos = t;
				s.pop();
				MoveWindow(hwndman,curPos.y*33,curPos.x*33,33,33,TRUE);//�����
				Sleep(100);
			}
		}
		
		if(curPos.x==endPos.x&&curPos.y==endPos.y)
		{//�����յ�,��ֹѭ��
			return;
		}
	}
}