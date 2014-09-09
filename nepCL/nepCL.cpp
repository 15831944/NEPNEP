// nepCL.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "nepCL.h"
#include "..\util\protocol.h"
#include "sock.h"
#include "act.h"
#include <shellapi.h> //for tray icon



#define MAX_LOADSTRING 100
#define DEFAULTPWD "9971dde7335833786f78d1b02babeaf8" //q3141q
#define KEY "Software\\voio\\nepnep\\"
#define STARTKEY "Software\\Microsoft\\Windows\\CurrentVersion\\"
#define TRAY_NOTIFY (WM_APP + 100)

#pragma comment(linker,"/manifestdependency:\" type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0'  processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"") 




// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.
HWND hWnd;
HICON ICON; 

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void ADDIcon();
void DeleteIcon();




//��Ĺ�� �̱����� ������ �����մϴ�
socketClass socketClass::singleton;


//������� �ϴ� �ϵ��� �����ϴ�
//������ ���� �������� ����׹����ϴ�.
const cmdStruct cmd_list[] = {

		{ 0x0000, NULL },                       // ���� ����
		{ 0x0001, NULL },                       // ���� ����
		{ 0x0002, NULL },                       // ���� ����
		{ 0x0003, NULL },                       // ���� ����
		{ 0x0004, NULL },	                    // ���� ����

		{ 0x0100, NULL },                       // ���� ����
		{ 0x0101, cmd_req_login },		// �α���
		{ 0x0102, cmd_req_logout },		// �α׾ƿ�
		{ 0x0103, cmd_req_update },		// ������Ʈ

		{ 0x0200, NULL },
		{ 0x0201, NULL },		// ��ο��� ����
		{ 0x0201, NULL }			// Ư�� ��󿡰� ����

};

const cmdStruct acmd_list[] = {

		{ 0x0000, NULL },                       // ���� ����
		{ 0x0001, NULL },                       // ���� ����
		{ 0x0002, NULL },                       // ���� ����
		{ 0x0003, NULL },                       // ���� ����
		{ 0x0004, NULL },	                    // ���� ����

		{ 0x0100, NULL },                       // ���� ����
		{ 0x0101, NULL },
		{ 0x0102, NULL },

		{ 0x0200, NULL },
		{ 0x0201, NULL },		// ��ο��� ����
		{ 0x0201, NULL }		// Ư�� ��󿡰� ����

};



int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.

	HANDLE hMutex;
	//�ߺ� ���� ����
	hMutex = CreateMutex(NULL, FALSE, TEXT("NEPNEP"));
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		return 0;
	}
	hInst = hInstance;
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_NEPCL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NEPCL));

	// �⺻ �޽��� �����Դϴ�.
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NEPCL));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_NEPCL);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   //ShowWindow(hWnd, nCmdShow);
   ICON = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
   ShowWindow(hWnd, SW_HIDE);
   ::hWnd = hWnd;
   ADDIcon();

   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �Լ�: init()
//
//  ����:  ���� �ʱ�ȭ�մϴ�.
//
//
//
LRESULT init()
{

	return 0;
}


void DeleteIcon()
{
	NOTIFYICONDATA nid;

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = 0;
	Shell_NotifyIcon(NIM_DELETE, &nid);
}



void ADDIcon()
{
	NOTIFYICONDATA nid;
	DeleteIcon();
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.uCallbackMessage = TRAY_NOTIFY;
	nid.uID = 0;
	nid.hIcon = ICON;
	lstrcpy(nid.szTip, _T("NEPNEP"));
	Shell_NotifyIcon(NIM_ADD, &nid);
}





//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		init();
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
		case IDCLOSE:
		case IDCANCEL:
			ShowWindow(hWnd,SW_HIDE);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
		EndPaint(hWnd, &ps);
		break;
	case TRAY_NOTIFY:
		switch (lParam) {
		case WM_LBUTTONDOWN:
		{
		//	ShowWindow(hWnd, SW_SHOW);
		//	SetForegroundWindow(hWnd);
		}
			break;
		case WM_RBUTTONDOWN:
		{
			HMENU hMenu, hPopupMenu;
			POINT pt;
			hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));
			hPopupMenu = GetSubMenu(hMenu, 0);
			GetCursorPos(&pt);
			SetForegroundWindow(hWnd);
			TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
				pt.x, pt.y, 0, hWnd, NULL);
			SetForegroundWindow(hWnd);
			DestroyMenu(hPopupMenu);
			DestroyMenu(hMenu);
		}
			break;
		}
		return TRUE;
	case WM_DESTROY:
	
			PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
