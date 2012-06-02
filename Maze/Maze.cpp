// Maze.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Maze.h"
#include "GoMaze.h"

#define MAX_LOADSTRING 100
#define WM_STARTDRAW (WM_USER + 1)

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
HWND mainhwnd;;
BOOL ISEnable = FALSE;		//是否设置成功
int nWide,nHeight;		//迷宫地图的宽和高
CGoMaze *gomaze = NULL;		//创建迷宫
BOOL IsMazeExist = FALSE;
BOOL AutoRun = FALSE;
WNDPROC oldProc;	//指向旧的过程函数

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	ManWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	SetUpWndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;
	
	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MAZE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAZE));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	//注册小人窗口
	WNDCLASS wc;
	GetClassInfo(hInstance,_T("static"),&wc);
	oldProc = wc.lpfnWndProc;
	wc.lpfnWndProc = ManWndProc;
	wc.lpszClassName = _T("man");
	RegisterClass(&wc);

	//注册主窗口
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDC_MAZE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MAZE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDC_MAZE));
	return		RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW&~WS_MAXIMIZEBOX&~WS_SIZEBOX ,
      300, 200, 400, 500, NULL, NULL, hInstance, NULL);

   mainhwnd = hWnd;
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   SetWindowText(hWnd,_T("迷宫生成寻路软件"));
   return TRUE;
}


//
void Thread(PVOID param)
{ 
	gomaze->FindPath();
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_KEYDOWN:
		if(AutoRun)
			if (IsMazeExist)
				if(IsWindow(gomaze->hwndman)&&wParam == VK_SPACE)
				{
					_beginthread(Thread,0,NULL);
					AutoRun = FALSE;
				}
				break;
	case  WM_SETFOCUS:
		if(!AutoRun)
		if(IsMazeExist)
		if (IsWindow(gomaze->hwndman))
		{
			SetFocus(gomaze->hwndman);
		}
		break;
	case WM_STARTDRAW:
		if(gomaze !=NULL) delete gomaze;
		gomaze = new CGoMaze;
		IsMazeExist = TRUE;
		gomaze->SetMaze(nWide,nHeight,hWnd,hInst);
		gomaze->DrawMaze();
		return 0;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_START:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_SETUP), hWnd, SetUpWndProc);
			break;
		case ID_AUTO:
			if(AutoRun)
				if (IsMazeExist)
					if(IsWindow(gomaze->hwndman))
				{
					_beginthread(Thread,0,NULL);
					AutoRun = FALSE;
				}
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//设置对话框的消息处理函数
INT_PTR CALLBACK	SetUpWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	HWND hwnduser,hwndauto;
	HWND hWide,hHeight;
	TCHAR widebuffer[3];
	TCHAR hHeightbuffer[3];
	//获取子窗口句柄
	hWide = GetDlgItem(hDlg,IDC_WIDE);
	hHeight = GetDlgItem(hDlg,IDC_HEIGHT);
	hwnduser = GetDlgItem(hDlg,IDC_USER);
	hwndauto = GetDlgItem(hDlg,IDC_AUTO);
	switch (message)
	{
	case WM_INITDIALOG:
		SendMessage(hWide,EM_SETLIMITTEXT,(WPARAM)2,0);
		SendMessage(hHeight,EM_SETLIMITTEXT,(WPARAM)2,0);
		SendMessage(hwnduser,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{//获取RADIO
			if (SendMessage(hwndauto,BM_GETCHECK,0,0) == BST_CHECKED)
			{
				AutoRun = TRUE;
			}
			if (SendMessage(hwnduser,BM_GETCHECK,0,0) == BST_CHECKED)
			{
				AutoRun = FALSE;
			}
			//获取数据
			GetWindowText(hWide,widebuffer,3);
			GetWindowText(hHeight,hHeightbuffer,3);
			nWide = _ttoi(widebuffer);
			nHeight = _ttoi(hHeightbuffer);
			if (nWide>50||nHeight>30)
			{
				MessageBox(hDlg,_T("宽不能大于50，高不能大于30！"),_T("警告"),MB_OK|MB_ICONWARNING);
				ISEnable = FALSE;
			}
			else
			{
				if (nWide > 5)
				{
					MoveWindow(mainhwnd,100,100,nWide*33+5,nHeight*33+47,TRUE);
				}
				else
				{
					MoveWindow(mainhwnd,300,200,nWide*33+5,nHeight*33+66,TRUE);
				}
				SendMessage(mainhwnd,WM_STARTDRAW,0,0);
				ISEnable = TRUE;
				EndDialog(hDlg, LOWORD(wParam));
			}
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDCANCEL)
		{
			ISEnable = FALSE;
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//小人的消息处理函数
LRESULT CALLBACK	ManWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	BOOL IsPlay = TRUE;
	switch (message)
	{
	case WM_KEYDOWN:
		if(!AutoRun)
		{
			switch(wParam)
			{
			case VK_UP:
				IsPlay = gomaze->MoveMan(UP);
				break;
			case VK_DOWN:
				IsPlay = gomaze->MoveMan(DOWN);
				break;
			case VK_LEFT:
				IsPlay = gomaze->MoveMan(LEFT);
				break;
			case  VK_RIGHT:
				IsPlay = gomaze->MoveMan(RIGHT);
				break;
			}
		}
		if (!IsPlay) MessageBeep(0);
		break;
	}
	return CallWindowProc(oldProc,hWnd,message,wParam,lParam);
}