// nepCL.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "nepCL.h"
#include "..\util\protocol.h"
#include "sock.h"
#include "act.h"
#include <shellapi.h>



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

#include <WinInet.h> // for getFileFromHttp
#include <Shlwapi.h> // for PathFileExists 
#include <hash_map>

#include <map>
#include <list>

#include <io.h>


#pragma comment(lib,"wininet.lib")
#pragma comment(lib,"Shlwapi.lib")
using namespace std;

#define LOG(P) wprintf(_T("%s\n"),(wstring(_T("")) + ##P).c_str());


typedef hash_map<wstring, wstring> type_sectionData;
typedef hash_map<wstring, type_sectionData> type_sectionsMap;





int isDirExists(const TCHAR* s) {
	_tfinddatai64_t c_file;
	intptr_t hFile;
	int result = 0;

	hFile = _tfindfirsti64(s, &c_file);
	if (c_file.attrib & _A_SUBDIR) result = 1;
	_findclose(hFile);

	return result;
}

/**
localPath는 디렉토리여야하며
serverPath는 파일명을 포함하는 디렉터리
파일명은 serverpath에서 얻어옴
**/
int GetFileFromNameServer(const TCHAR* localPath, TCHAR* serverPath)
{
	FILE *fp;
	TCHAR paths[256] = { 0, };
	TCHAR *path;
	TCHAR *serverfilename = NULL;

	tstring str;
	str = str + localPath + _T("downloadscript.tmp");
	_tfopen_s(&fp, str.c_str(), _T("w"));
	if (fp == NULL)
		return S_FALSE;

	_ftprintf(fp, _T("o internet.ddc.ac.kr\n"));
	_ftprintf(fp, _T("public\n"));
	_ftprintf(fp, _T("iptv\n"));



	_tcscpy_s(paths, 256, serverPath);
	for (int i = _tcslen(paths) - 1; i > 0; i--)
	{
		if (paths[i] == '\\')
		{
			paths[i] = 0;
			serverfilename = &paths[i + 1];
			break;
		}
	}
	if (serverfilename != NULL)
	{
		TCHAR *context = NULL;
		path = _tcstok_s(paths, _T("\\"), &context);
		while (path != NULL)
		{
			_ftprintf(fp, _T("cd %s\n"), path);
			path = _tcstok_s(NULL, _T("\\"), &context);
		}
	}
	else
	{
		serverfilename = serverPath;
	}
	_ftprintf(fp, _T("get %s\n"), serverfilename); //후에 랜덤으로 문자열 생성하여 임시파일로저장 
	_ftprintf(fp, _T("bye\n"));


	fclose(fp);
	ShellExecute(NULL, _T("open"), _T("ftp"), (tstring() + _T(" -s:") + str.c_str()).c_str(), NULL, SW_HIDE);
	return S_OK;
}


tstring GetLasetOrderIP()
{

	//서버 환경조사
	TCHAR tmpPath[MAX_PATH];
	GetTempPath(MAX_PATH, tmpPath);
	TCHAR IP[26];
	if (GetFileFromNameServer(tmpPath, _T("recoveryserver\\ip.txt")))
	{

		FILE * fp;
		tstring path = tstring(tstring() + tmpPath + _T("ip.txt"));
		_tfopen_s(&fp, path.c_str(), _T("r"));

		if (fp != NULL)
		{
			_fgetts(IP, 25, fp);
			if (IP[_tcslen(IP) - 1] == _T('\n'))
			{
				IP[_tcslen(IP) - 1] = 0;
			}
			fclose(fp);
			_tremove(path.c_str());
			return tstring(IP);
		}
	}

	return NULL;

}


bool loadSections(type_sectionsMap &sectionsMap, TCHAR *fileName)
{

	TCHAR sections[1024] = { 0x00 };
	TCHAR *section = 0;
	DWORD sectionCount = ::GetPrivateProfileSectionNames(sections, 1024, fileName);
	DWORD sectionIndex = 0;

	while (sectionIndex < sectionCount)
	{
		section = sections + sectionIndex;
		TCHAR keys[1024] = { 0x00 };
		TCHAR *key = 0;
		DWORD keyCount = ::GetPrivateProfileSection(section, keys, 1024, fileName);
		DWORD keyIndex = 0;

		while (keyIndex < keyCount)
		{
			key = keys + keyIndex;
			int keyLen = _tcslen(key);
			TCHAR value[512] = { 0x00 };
			for (int i = 0; i < keyLen; i++) {
				if (key[i] == _T('=')) {
					key[i] = 0;
					break;
				}
			}
			GetPrivateProfileString(section, key, _T(""), value, 512, fileName);
			sectionsMap[section][key] = value;
			keyIndex += keyLen + 1;
		}
		sectionIndex += _tcslen(section) + 1;
	}
	return S_OK;
}


#define READ_BUF_SIZE    1024
int getFileFromHttp(LPCTSTR pszUrl, LPCTSTR	 pszFile)
{
	HINTERNET    hInet, hUrl;
	DWORD        dwReadSize = 0;

	// WinINet함수 초기화
	if ((hInet = InternetOpen(_T("BlueSwitch"),            // user agent in the HTTP protocol
		INTERNET_OPEN_TYPE_DIRECT,    // AccessType
		NULL,                        // ProxyName
		NULL,                        // ProxyBypass
		0)) != NULL)                // Options
	{
		// 입력된 HTTP주소를 열기
		if ((hUrl = InternetOpenUrl(hInet,        // 인터넷 세션의 핸들
			pszUrl,                        // URL
			NULL,                        // HTTP server에 보내는 해더
			0,                            // 해더 사이즈
			INTERNET_FLAG_RELOAD,         // Flag INTERNET_FLAG_RELOAD:: 강제 서버 새로고침
			0)) != NULL)                // Context
		{
			FILE    *fp;

			// 다운로드할 파일 만들기
			_tfopen_s(&fp, pszFile, _T("wb"));
			if (fp != NULL)
			{
				TCHAR    szBuff[READ_BUF_SIZE];
				DWORD    dwSize = 0;

				do {
					// 웹상의 파일 읽기
					InternetReadFile(hUrl, szBuff, READ_BUF_SIZE, &dwSize);

					// 웹상의 파일을 만들어진 파일에 써넣기
					fwrite(szBuff, dwSize, 1, fp);

					dwReadSize += dwSize;
				} while ((dwSize != 0));

				fclose(fp);
			}
			// 인터넷 핸들 닫기
			InternetCloseHandle(hUrl);
		}
		// 인터넷 핸들 닫기
		InternetCloseHandle(hInet);
	}
	return(dwReadSize);
}

BOOL CreateDirectoryContinue(LPCTSTR lpszPath)
{
	TCHAR szPathBuffer[MAX_PATH];

	size_t len = _tcslen(lpszPath);

	for (size_t i = 0; i < len; i++)
	{
		szPathBuffer[i] = *(lpszPath + i);
		if (szPathBuffer[i] == _T('\\') || szPathBuffer[i] == _T('/'))
		{
			szPathBuffer[i + 1] = NULL;
			if (!PathFileExists(szPathBuffer))
			{
				if (!::CreateDirectory(szPathBuffer, NULL))
				{
					if (GetLastError() != ERROR_ALREADY_EXISTS)
						return FALSE;
				}
			}
		}
	}
	return TRUE;

}


bool critical = false;
void downloadFile(wstring localFilePath, wstring localFile, wstring serverFilePath, wstring serverFile, wstring tmpDir)
{
	if (localFilePath.compare(_T("")) != 0)
		CreateDirectoryContinue((tmpDir + _T("\\") + localFilePath).c_str()); //디렉터리 구조를 tmp폴더에 만듬

	if (getFileFromHttp((serverFilePath + serverFile).c_str(), (tmpDir + _T("\\") + localFilePath + _T("\\") + localFile).c_str()) == 0)
	{
		LOG(_T("파일다운에러;;"));
	}
	else
		critical = true;
}


void ifDirEmptyDelete(const  TCHAR *path)
{
	if (PathIsDirectoryEmpty(path))
	{
		RemoveDirectory(path);
	}
}


void GetFilesFromDirectory(const TCHAR *param_path, std::list<wstring> &list_arg)
{
	TCHAR path[MAX_PATH];
	TCHAR file_path[MAX_PATH];
	WIN32_FIND_DATA file_data;

	_stprintf_s(path, MAX_PATH, _T("%s\\*.*"), param_path);

	HANDLE search_handle = FindFirstFile(path, &file_data);



	if (INVALID_HANDLE_VALUE != search_handle){
		// 파일 목록을 성공적으로 구성한 경우..
		if (_tcscmp(file_data.cFileName, _T(".")) == 0) //skip .
		{
			FindNextFile(search_handle, &file_data); //skip ..
			if (!FindNextFile(search_handle, &file_data))  //if there is no more file list
			{
				FindClose(search_handle); //close and return;
				return;
			}
		}
		do {

			_stprintf_s(file_path, MAX_PATH, _T("%s\\%s"), param_path, file_data.cFileName);

			list_arg.push_back(file_path);

			// if FindNextFile cant get file_data from search_handle return INVALID_HANDLE_VALUE(0)
		} while (FindNextFile(search_handle, &file_data));
		FindClose(search_handle);
	}
}

void moveAllFile(const TCHAR *path)
{
	list<wstring> list;
	GetFilesFromDirectory(path, list);

	TCHAR filename[120] = { 0, };
	TCHAR filepath[120] = { 0, };

	for (auto iter = list.begin(); iter != list.end(); iter++)
	{
		const TCHAR *str = iter->c_str();
		int len = _tcslen(str);
		for (int i = len - 1; i > 0; i--)
			if (str[i] == _T('\\'))
			{
			_tcscpy_s(filename, 120, str + i + 1);
			_tcsncpy_s(filepath, 120, str, len - i);
			break;
			}

		MoveFile(str, filename);
	}

}


#define ERR_PATCH_CONNECT -1
#define ERR_PATCH_CREATESCRIPT -2
int update()
{

	tstring ips=GetLasetOrderIP();
	//TCHAR ip[30] = _T("218.180.88.59");
	const TCHAR *ip = ips.c_str();





	DeleteFile(_T("down.ini"));

	wstring patchFileLocalPath;
	wstring patchFIleServerPath;
	wstring patchFilePath;
	wstring patchUrl = _T("http://") + wstring(ip) + _T("/patch/");

	wstring serverIniUrl = patchUrl + _T("down.ini");
	if (getFileFromHttp(serverIniUrl.c_str(), _T(".\\down.ini")) == 0)
		return ERR_PATCH_CONNECT;



	type_sectionsMap localSectionMap;
	loadSections(localSectionMap, _T(".\\local.ini"));


	type_sectionsMap serverSectionMap;
	loadSections(serverSectionMap, _T(".\\down.ini"));

	const wstring tempPathName = _T("patchtmp");
	CreateDirectory(tempPathName.c_str(), NULL);

	for (auto serverSection = serverSectionMap.begin();
		serverSection != serverSectionMap.end(); serverSection++)
	{

		LOG(_T("PATCHING MODULE " + serverSection->first.c_str()));
		type_sectionData serverKeys = serverSection->second;
		auto serverPath = serverKeys.find(_T("path"));
		auto serverVer = serverKeys.find(_T("ver"));

		auto localSection = localSectionMap.find(serverSection->first);
		if (localSection == localSectionMap.end()){
			//새로운 파일이다!
			// 서버에 있는 쌩 주소로 다운로드해야지!
			if (serverPath != serverKeys.end())
				downloadFile(serverPath->second.c_str(), serverSection->first,
				patchUrl, serverPath->second + serverSection->first, tempPathName);
			else
				downloadFile(_T(""), serverSection->first,
				patchUrl, serverSection->first, tempPathName);
			continue;
		}


		type_sectionData localKeys = localSection->second;

		auto localPath = localKeys.find(_T("path"));


		auto localVer = localKeys.find(_T("ver"));



		//서버와 로컬 버전이 다르다는 뜻
		if (localVer->second.compare(serverVer->second) != 0)
		{
			//다운로드
			//경로가 변경될 수가 있기때문에 우선 딜리트
			if (localPath != localKeys.end())
			{

				DeleteFile((localPath->second + localSection->first).c_str());
				ifDirEmptyDelete(localPath->second.c_str());

			}
			else
			{
				DeleteFile((localSection->first).c_str());
			}

			if (serverPath != serverKeys.end())
			{
				downloadFile(serverPath->second.c_str(), serverSection->first,
					patchUrl, serverPath->second + serverSection->first, tempPathName);
			}
			else
			{
				downloadFile(_T(""), serverSection->first,
					patchUrl, serverSection->first, tempPathName);
			}

		}

		//확인을 하면 로컬맵에서 제외
		localSectionMap.erase(localSection);

	}
	//이제 로컬맵에 남아있는건 안쓰는 파일뿐w
	//전부 지운다
	for (auto localSection = localSectionMap.begin();
		localSection != localSectionMap.end(); localSection++)
	{
		auto key = localSection->second;
		auto path = key.find(_T("path"));
		if (path != key.end())
		{
			DeleteFile((path->second + localSection->first).c_str());
			ifDirEmptyDelete(path->second.c_str());
		}
		else
		{
			DeleteFile(localSection->first.c_str());
		}
	}



	if (critical)
	{
		DeleteFile(_T("local.ini"));
		_trename(_T("down.ini"), _T("local.ini"));

		moveAllFile((tempPathName).c_str());
	}
	else
	{
		DeleteFile(_T("down.ini"));
	}
	_trmdir(tempPathName.c_str());
	system("pause");
	return 0;
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
