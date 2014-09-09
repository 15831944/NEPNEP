// nepCL.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
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




// 전역 변수:
HINSTANCE hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.
HWND hWnd;
HICON ICON; 

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void ADDIcon();
void DeleteIcon();




//소캣의 싱글톤을 여따가 정의합니다
socketClass socketClass::singleton;


//통신으로 하는 일들을 적습니다
//전역은 몸에 안좋지만 방법잉벗습니다.
const cmdStruct cmd_list[] = {

		{ 0x0000, NULL },                       // 내용 없음
		{ 0x0001, NULL },                       // 내용 없음
		{ 0x0002, NULL },                       // 내용 없음
		{ 0x0003, NULL },                       // 내용 없음
		{ 0x0004, NULL },	                    // 내용 없음

		{ 0x0100, NULL },                       // 내용 없음
		{ 0x0101, cmd_req_login },		// 로그인
		{ 0x0102, cmd_req_logout },		// 로그아웃
		{ 0x0103, cmd_req_update },		// 업데이트

		{ 0x0200, NULL },
		{ 0x0201, NULL },		// 모두에게 전송
		{ 0x0201, NULL }			// 특정 대상에게 전송

};

const cmdStruct acmd_list[] = {

		{ 0x0000, NULL },                       // 내용 없음
		{ 0x0001, NULL },                       // 내용 없음
		{ 0x0002, NULL },                       // 내용 없음
		{ 0x0003, NULL },                       // 내용 없음
		{ 0x0004, NULL },	                    // 내용 없음

		{ 0x0100, NULL },                       // 내용 없음
		{ 0x0101, NULL },
		{ 0x0102, NULL },

		{ 0x0200, NULL },
		{ 0x0201, NULL },		// 모두에게 전송
		{ 0x0201, NULL }		// 특정 대상에게 전송

};



int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
 	// TODO: 여기에 코드를 입력합니다.

	HANDLE hMutex;
	//중복 실행 방지
	hMutex = CreateMutex(NULL, FALSE, TEXT("NEPNEP"));
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		return 0;
	}
	hInst = hInstance;
	MSG msg;
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_NEPCL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NEPCL));

	// 기본 메시지 루프입니다.
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
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
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
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
//  함수: init()
//
//  목적:  앱을 초기화합니다.
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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
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
		// 메뉴 선택을 구문 분석합니다.
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
		// TODO: 여기에 그리기 코드를 추가합니다.
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

// 정보 대화 상자의 메시지 처리기입니다.
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
