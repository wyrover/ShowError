#include "ShowError.h"
#include "resource.h"

HWND g_hMainWnd = NULL;
HWND g_hAboutWnd = NULL;

BOOL bAboutWndCreated = FALSE;


/************************************************************************/
/* 程序入口点
/************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/* 单实例程序 */
	HANDLE hMutex = CreateMutex(NULL, FALSE, SHOW_ERROR_MUTEX);
	if(ERROR_ALREADY_EXISTS == GetLastError())
	{
		return FALSE;
	}
	
	return CreateMainWindow(hInstance);
}


/************************************************************************/
/* 创建主窗口
/************************************************************************/
int CreateMainWindow(HINSTANCE hInstance)
{
	// 1. 设计窗口类
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	/* 设置窗口图标为资源文件中的图标 */
	/* wndClass.hIcon = LoadIcon(NULL, IDI_ERROR); */
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = ShowErrorMainWndMsgProc;
	wndClass.lpszClassName = MainWndClassName;
	wndClass.lpszMenuName = MainWndAppName;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	// 2. 注册窗口类
	RegisterClass(&wndClass);

	/* 获取当前屏幕的宽度和高度 */
	int nWidth = GetSystemMetrics(SM_CXSCREEN);
	int nHeight = GetSystemMetrics(SM_CYSCREEN);

	// 3. 创建窗口 - 屏幕居中显示，无最大化窗口按钮，不允许改变窗口大小
	g_hMainWnd = CreateWindow(MainWndClassName, MainWndWindowName, WS_OVERLAPPEDWINDOW & (~WS_THICKFRAME) & (~WS_MAXIMIZEBOX), (nWidth - MAINWND_WINDOW_WIDTH) / 2, (nHeight - MAINWND_WINDOW_HEIGHT) / 2, MAINWND_WINDOW_WIDTH, MAINWND_WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	// 4. 显示窗口
	ShowWindow(g_hMainWnd, SW_SHOWNORMAL);

	// 5. 刷新窗口
	UpdateWindow(g_hMainWnd);

	// 6. 消息循环
	MSG msg;
	BOOL bReturn;
	while((bReturn = GetMessage(&msg, g_hMainWnd, 0, 0)) != 0)
	{
		if(bReturn == -1)
		{
			break;;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}


/************************************************************************/
/* 创建 About 窗口
/************************************************************************/
int CreateAboutWindow(HINSTANCE hInstance)
{
	// 1. 设计窗口类
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	/* 设置窗口图标为资源文件中的图标 */
	/* 获得当前模块句柄 */
	/* wndClass.hIcon = LoadIcon(hInstance, IDI_ERROR); */
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = ShowErrorAboutWndMsgProc;
	wndClass.lpszClassName = AboutWndClassName;
	wndClass.lpszMenuName = AboutWndAppName;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	// 2. 注册窗口类
	RegisterClass(&wndClass);

	/* 获取当前屏幕的宽度和高度 */
	int nWidth = GetSystemMetrics(SM_CXSCREEN);
	int nHeight = GetSystemMetrics(SM_CYSCREEN);

	// 3. 创建窗口 - 屏幕居中显示，无最小化最大化窗口按钮，不允许改变窗口大小
	g_hAboutWnd = CreateWindow(AboutWndClassName, AboutWndWindowName, WS_OVERLAPPEDWINDOW & (~WS_THICKFRAME) & (~WS_MAXIMIZEBOX) & (~WS_MINIMIZEBOX), (nWidth - ABOUTWND_WINDOW_WIDTH) / 2, (nHeight - ABOUTWND_WINDOW_HEIGHT) / 2, ABOUTWND_WINDOW_WIDTH, ABOUTWND_WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	// 4. 显示窗口
	ShowWindow(g_hAboutWnd, SW_SHOWNORMAL);

	// 5. 刷新窗口
	UpdateWindow(g_hAboutWnd);

	// 6. 消息循环
	MSG msg;
	BOOL bReturn;
	while((bReturn = GetMessage(&msg, g_hAboutWnd, 0, 0)) != 0)
	{
		if(bReturn == -1)
		{
			break;;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}


/************************************************************************/
/* 主窗口过程处理函数
/************************************************************************/
LRESULT CALLBACK ShowErrorMainWndMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hStaticErrorMsg;
	static HWND hStaticErrorCode;	

	static HWND hTxtErrorMsg;
	static HWND hTxtErrorCode;

	static HWND hBtnExit;
	static HWND hBtnAbout;
	static HWND hBtnTransform;

	switch(uMsg)
	{
	case WM_CREATE:
		{
			/* 为窗口添加扩展风格 WS_EX_LAYERED，从而为实现窗口透明做准备 */
			LONG lExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
			lExStyle = lExStyle | WS_EX_LAYERED;
			SetWindowLong(hWnd, GWL_EXSTYLE, lExStyle);

			/* 实现窗口透明 - 设置窗口透明度为 220 */
			SetLayeredWindowAttributes(hWnd, 0, WND_TRANSPARENCY, LWA_ALPHA);


			/* 创建静态文本框 */
			hStaticErrorCode = CreateWindow(TEXT("STATIC"), TEXT("Error Code:"),
				WS_CHILD | WS_VISIBLE | SS_RIGHT,
				20, 20, 110, 18, hWnd, (HMENU)ID_STATIC_ERROR_MESSAGE, 
				((LPCREATESTRUCT)lParam)->hInstance, NULL);
			hStaticErrorMsg = CreateWindow(TEXT("STATIC"), TEXT("Error Message:"),
				WS_CHILD | WS_VISIBLE | SS_RIGHT,
				20, 60, 110, 18, hWnd, (HMENU)ID_STATIC_ERROR_CODE, 
				((LPCREATESTRUCT)lParam)->hInstance, NULL);

			/* 创建文本输入框，ES_AUTOHSCROLL 和 ES_AUTOVSCROLL 表示输入数据时自动左右滚动以及上下滚动 */
			hTxtErrorCode = CreateWindow(TEXT("EDIT"), NULL,
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
				140, 18, 270, 21, hWnd, (HMENU)ID_TXT_ERROR_CODE, 
				((LPCREATESTRUCT)lParam)->hInstance, NULL);
			hTxtErrorMsg = CreateWindow(TEXT("EDIT"), NULL,
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
				140, 58, 270, 21, hWnd, (HMENU)ID_TXT_ERROR_MESSAGE, 
				((LPCREATESTRUCT)lParam)->hInstance, NULL);

			/* 创建按钮 */
			hBtnTransform = CreateWindow(TEXT("button"), TEXT("Transform"), 
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
				50, 100, 100, 25, hWnd, (HMENU)ID_BUTTON_TRANSFORM, 
				((LPCREATESTRUCT)lParam)->hInstance, NULL);
			hBtnAbout = CreateWindow(TEXT("button"), TEXT("About"), 
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
				170, 100, 100, 25, hWnd, (HMENU)ID_BUTTON_ABOUT, 
				((LPCREATESTRUCT)lParam)->hInstance, NULL);
			hBtnExit = CreateWindow(TEXT("button"), TEXT("Exit"), 
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
				290, 100, 100, 25, hWnd, (HMENU)ID_BUTTON_EXIT, 
				((LPCREATESTRUCT)lParam)->hInstance, NULL);
			
			/* 设置文本框为只读模式 */
			SendMessage(hTxtErrorMsg, EM_SETREADONLY, TRUE, 0);

			/* 设置文本框最大输入字符数为 11 位 */
			SendMessage(hTxtErrorCode, EM_LIMITTEXT, 11, 0);

			/* 设置字体 */
			//SendMessage(hTxtErrorCode, WM_SETFONT, reinterpret_cast<WPARAM>(::GetStockObject(DEFAULT_GUI_FONT)), true);
			//SendMessage(hTxtErrorMsg, WM_SETFONT, reinterpret_cast<WPARAM>(::GetStockObject(DEFAULT_GUI_FONT)), true);

			/* 设置输入焦点 */
			SetFocus(hTxtErrorCode);

			break;
		}
	case WM_CTLCOLORSTATIC:
		{
			/* 使得静态文本框使用透明背景色 */
			return (DWORD)GetStockObject(NULL_BRUSH);
		}
	case WM_COMMAND:
		{
			switch (wParam)
			{
			case ID_BUTTON_EXIT:
				{
					DestroyWindow(hWnd);

					break;
				}
			case ID_BUTTON_ABOUT:
				{
					//CreateAboutWindow(GetModuleHandle(NULL));

					break;
				}
			case ID_BUTTON_TRANSFORM:
				{
					BOOL bSucceed = FALSE;
					CHAR pszErrorCode[20] = { 0 };

					SendMessageA(hTxtErrorCode, WM_GETTEXT, 20, (LPARAM)pszErrorCode);
					if (ValidateStringIsLong((string)pszErrorCode) == true)
					{
						try
						{
							tstring strMessage = TEXT("");
							strMessage = TransformErrorCode2Message(atol(pszErrorCode));

							/* 设置显示值 */
							SendMessage(hTxtErrorMsg, WM_SETTEXT, strMessage.length(), (LPARAM)strMessage.c_str());

							/* 设置全部选中 */
							SendMessage(hTxtErrorMsg, EM_SETSEL, 0, -1);

							bSucceed = TRUE;
						}
						catch(...)
						{
							MessageBox(hWnd, TEXT("   Your Input Is Invalid，Please Check !   "), TEXT("Error"), MB_OK | MB_ICONERROR);

							bSucceed = FALSE;
						}
						
					}
					else
					{
						MessageBox(hWnd, TEXT("   Your Input Is Invalid，Please Check !   "), TEXT("Error"), MB_OK | MB_ICONERROR);

						bSucceed = FALSE;
					}

					if(bSucceed == FALSE)
					{
						/* 设置输入焦点 */
						SetFocus(hTxtErrorCode);
					}
					else
					{
						/* 设置输入焦点 */
						SetFocus(hTxtErrorMsg);
					}

					break;
				}
			}

			break;
		}
	case WM_CLOSE:
		{
			DestroyWindow(hWnd);

			break;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);

			break;
		}
	default:
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

	return 0;
}


/************************************************************************/
/* About 窗口的窗口处理过程
/************************************************************************/
LRESULT CALLBACK ShowErrorAboutWndMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		{
			/* 为窗口添加扩展风格 WS_EX_LAYERED，从而为实现窗口透明做准备 */
			LONG lExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
			lExStyle = lExStyle | WS_EX_LAYERED;
			SetWindowLong(hWnd, GWL_EXSTYLE, lExStyle);

			/* 实现窗口透明 - 设置窗口透明度为 220 */
			SetLayeredWindowAttributes(hWnd, 0, WND_TRANSPARENCY, LWA_ALPHA);

			break;
		}
	case WM_CTLCOLORSTATIC:
		{
			/* 使得静态文本框使用透明背景色 */
			return (DWORD)GetStockObject(NULL_BRUSH);
		}
	case WM_CLOSE:
		{
			DestroyWindow(hWnd);

			break;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);

			break;
		}
	default:
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

	return 0;
}


/************************************************************************/
/* 转换错误码为错误消息，即将指定的错误码转换为错误消息
/************************************************************************/
tstring TransformErrorCode2Message(DWORD dwError)
{
	tstring strResult;
	LPTSTR lpszBufErrorMsg;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpszBufErrorMsg, 0, NULL);

	if(lpszBufErrorMsg != NULL)
	{
		strResult = (LPTSTR)lpszBufErrorMsg;

		LocalFree(lpszBufErrorMsg);
	}
	else
	{
		strResult = TEXT("Invalid Error Code");
	}

	return strResult;
}


/************************************************************************/
/* 判断一个字符串是否为整数
/************************************************************************/
bool ValidateStringIsLong(string strSrc)
{
	if(strSrc.length() <= 0)
	{
		return false;
	}

	/* 去掉字符串中的所有空格 */
	strSrc.resize(remove(strSrc.begin(), strSrc.end(), ' ') - strSrc.begin());

	if(strSrc.length() <= 0)
	{
		return false;
	}
	for(int i = 0; i < strSrc.length(); i++)
	{
		if (strSrc[i] < 48 || strSrc[i] > 57)
		{
			return false;	
		}
	}

	return true;
}
