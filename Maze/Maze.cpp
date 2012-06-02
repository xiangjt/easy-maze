// Maze.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Maze.h"
#include "GoMaze.h"

#define MAX_LOADSTRING 100
#define WM_STARTDRAW (WM_USER + 1)

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
HWND mainhwnd;;
BOOL ISEnable = FALSE;		//�Ƿ����óɹ�
int nWide,nHeight;		//�Թ���ͼ�Ŀ�͸�
CGoMaze *gomaze = NULL;		//�����Թ�
BOOL IsMazeExist = FALSE;
BOOL AutoRun = FALSE;
WNDPROC oldProc;	//ָ��ɵĹ��̺���

// �˴���ģ���а����ĺ�����ǰ������:
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

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;
	
	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MAZE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAZE));

	// ����Ϣѭ��:
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	//ע��С�˴���
	WNDCLASS wc;
	GetClassInfo(hInstance,_T("static"),&wc);
	oldProc = wc.lpfnWndProc;
	wc.lpfnWndProc = ManWndProc;
	wc.lpszClassName = _T("man");
	RegisterClass(&wc);

	//ע��������
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW&~WS_MAXIMIZEBOX&~WS_SIZEBOX ,
      300, 200, 400, 500, NULL, NULL, hInstance, NULL);

   mainhwnd = hWnd;
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   SetWindowText(hWnd,_T("�Թ�����Ѱ·���"));
   return TRUE;
}


//
void Thread(PVOID param)
{ 
	gomaze->FindPath();
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
		// �����˵�ѡ��:
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
		// TODO: �ڴ���������ͼ����...
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

// �����ڡ������Ϣ�������
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

//���öԻ������Ϣ������
INT_PTR CALLBACK	SetUpWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	HWND hwnduser,hwndauto;
	HWND hWide,hHeight;
	TCHAR widebuffer[3];
	TCHAR hHeightbuffer[3];
	//��ȡ�Ӵ��ھ��
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
		{//��ȡRADIO
			if (SendMessage(hwndauto,BM_GETCHECK,0,0) == BST_CHECKED)
			{
				AutoRun = TRUE;
			}
			if (SendMessage(hwnduser,BM_GETCHECK,0,0) == BST_CHECKED)
			{
				AutoRun = FALSE;
			}
			//��ȡ����
			GetWindowText(hWide,widebuffer,3);
			GetWindowText(hHeight,hHeightbuffer,3);
			nWide = _ttoi(widebuffer);
			nHeight = _ttoi(hHeightbuffer);
			if (nWide>50||nHeight>30)
			{
				MessageBox(hDlg,_T("���ܴ���50���߲��ܴ���30��"),_T("����"),MB_OK|MB_ICONWARNING);
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

//С�˵���Ϣ������
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