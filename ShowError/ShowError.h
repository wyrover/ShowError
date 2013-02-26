#ifndef __SHOW_ERROR_H__
#define __SHOW_ERROR_H__


#include <Windows.h>
#include <string>
#include <strsafe.h>
#include <algorithm>
#include <functional>

using std::string;
using std::wstring;


#ifdef UNICODE
#define tstring wstring
#else
#define tstring string
#endif


#define MainWndClassName		TEXT("App.ShowError.MainWnd.ClassName")
#define MainWndMenuName			TEXT("App.ShowError.MainWnd.MenuName")
#define MainWndAppName			TEXT("App.ShowError.MainWnd.AppName")
#define MainWndWindowName		TEXT(" ShowError - Zachary")

#define AboutWndClassName		TEXT("App.ShowError.AboutWnd.ClassName")
#define AboutWndMenuName		TEXT("App.ShowError.AboutWnd.MenuName")
#define AboutWndAppName			TEXT("App.ShowError.AboutWnd.AppName")
#define AboutWndWindowName		TEXT(" About - Zachary")


#define SHOW_ERROR_MUTEX	TEXT("Global\\SHOW_ERROR_MNSAFE_MUTEX")


#define ID_STATIC_ERROR_CODE		1
#define ID_STATIC_ERROR_MESSAGE		2

#define ID_TXT_ERROR_CODE			3
#define ID_TXT_ERROR_MESSAGE		4

#define ID_BUTTON_TRANSFORM			5
#define ID_BUTTON_ABOUT				6
#define ID_BUTTON_EXIT				7


#define MAINWND_WINDOW_WIDTH		450
#define MAINWND_WINDOW_HEIGHT		180

#define ABOUTWND_WINDOW_WIDTH		300
#define ABOUTWND_WINDOW_HEIGHT		150

#define WND_TRANSPARENCY			230

int CreateMainWindow(HINSTANCE hInstance);

int CreateAboutWindow(HINSTANCE hInstance);

bool ValidateStringIsLong(string strSrc);

tstring TransformErrorCode2Message(DWORD dwError);

LRESULT CALLBACK ShowErrorMainWndMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK ShowErrorAboutWndMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


#endif
