#include "ShowError.h"
#include "resource.h"

HWND g_hMainWnd = NULL;
HWND g_hAboutWnd = NULL;

BOOL bAboutWndCreated = FALSE;


/************************************************************************/
/* ������ڵ�
/************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/* ��ʵ������ */
	HANDLE hMutex = CreateMutex(NULL, FALSE, SHOW_ERROR_MUTEX);
	if(ERROR_ALREADY_EXISTS == GetLastError())
	{
		return FALSE;
	}
	
	return CreateMainWindow(hInstance);
}


/************************************************************************/
/* ����������
/************************************************************************/
int CreateMainWindow(HINSTANCE hInstance)
{
	// 1. ��ƴ�����
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	/* ���ô���ͼ��Ϊ��Դ�ļ��е�ͼ�� */
	/* wndClass.hIcon = LoadIcon(NULL, IDI_ERROR); */
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = ShowErrorMainWndMsgProc;
	wndClass.lpszClassName = MainWndClassName;
	wndClass.lpszMenuName = MainWndAppName;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	// 2. ע�ᴰ����
	RegisterClass(&wndClass);

	/* ��ȡ��ǰ��Ļ�Ŀ�Ⱥ͸߶� */
	int nWidth = GetSystemMetrics(SM_CXSCREEN);
	int nHeight = GetSystemMetrics(SM_CYSCREEN);

	// 3. �������� - ��Ļ������ʾ������󻯴��ڰ�ť��������ı䴰�ڴ�С
	g_hMainWnd = CreateWindow(MainWndClassName, MainWndWindowName, WS_OVERLAPPEDWINDOW & (~WS_THICKFRAME) & (~WS_MAXIMIZEBOX), (nWidth - MAINWND_WINDOW_WIDTH) / 2, (nHeight - MAINWND_WINDOW_HEIGHT) / 2, MAINWND_WINDOW_WIDTH, MAINWND_WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	// 4. ��ʾ����
	ShowWindow(g_hMainWnd, SW_SHOWNORMAL);

	// 5. ˢ�´���
	UpdateWindow(g_hMainWnd);

	// 6. ��Ϣѭ��
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
/* ���� About ����
/************************************************************************/
int CreateAboutWindow(HINSTANCE hInstance)
{
	// 1. ��ƴ�����
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	/* ���ô���ͼ��Ϊ��Դ�ļ��е�ͼ�� */
	/* ��õ�ǰģ���� */
	/* wndClass.hIcon = LoadIcon(hInstance, IDI_ERROR); */
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = ShowErrorAboutWndMsgProc;
	wndClass.lpszClassName = AboutWndClassName;
	wndClass.lpszMenuName = AboutWndAppName;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	// 2. ע�ᴰ����
	RegisterClass(&wndClass);

	/* ��ȡ��ǰ��Ļ�Ŀ�Ⱥ͸߶� */
	int nWidth = GetSystemMetrics(SM_CXSCREEN);
	int nHeight = GetSystemMetrics(SM_CYSCREEN);

	// 3. �������� - ��Ļ������ʾ������С����󻯴��ڰ�ť��������ı䴰�ڴ�С
	g_hAboutWnd = CreateWindow(AboutWndClassName, AboutWndWindowName, WS_OVERLAPPEDWINDOW & (~WS_THICKFRAME) & (~WS_MAXIMIZEBOX) & (~WS_MINIMIZEBOX), (nWidth - ABOUTWND_WINDOW_WIDTH) / 2, (nHeight - ABOUTWND_WINDOW_HEIGHT) / 2, ABOUTWND_WINDOW_WIDTH, ABOUTWND_WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	// 4. ��ʾ����
	ShowWindow(g_hAboutWnd, SW_SHOWNORMAL);

	// 5. ˢ�´���
	UpdateWindow(g_hAboutWnd);

	// 6. ��Ϣѭ��
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
/* �����ڹ��̴�����
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
			/* Ϊ���������չ��� WS_EX_LAYERED���Ӷ�Ϊʵ�ִ���͸����׼�� */
			LONG lExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
			lExStyle = lExStyle | WS_EX_LAYERED;
			SetWindowLong(hWnd, GWL_EXSTYLE, lExStyle);

			/* ʵ�ִ���͸�� - ���ô���͸����Ϊ 220 */
			SetLayeredWindowAttributes(hWnd, 0, WND_TRANSPARENCY, LWA_ALPHA);


			/* ������̬�ı��� */
			hStaticErrorCode = CreateWindow(TEXT("STATIC"), TEXT("Error Code:"),
				WS_CHILD | WS_VISIBLE | SS_RIGHT,
				20, 20, 110, 18, hWnd, (HMENU)ID_STATIC_ERROR_MESSAGE, 
				((LPCREATESTRUCT)lParam)->hInstance, NULL);
			hStaticErrorMsg = CreateWindow(TEXT("STATIC"), TEXT("Error Message:"),
				WS_CHILD | WS_VISIBLE | SS_RIGHT,
				20, 60, 110, 18, hWnd, (HMENU)ID_STATIC_ERROR_CODE, 
				((LPCREATESTRUCT)lParam)->hInstance, NULL);

			/* �����ı������ES_AUTOHSCROLL �� ES_AUTOVSCROLL ��ʾ��������ʱ�Զ����ҹ����Լ����¹��� */
			hTxtErrorCode = CreateWindow(TEXT("EDIT"), NULL,
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
				140, 18, 270, 21, hWnd, (HMENU)ID_TXT_ERROR_CODE, 
				((LPCREATESTRUCT)lParam)->hInstance, NULL);
			hTxtErrorMsg = CreateWindow(TEXT("EDIT"), NULL,
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
				140, 58, 270, 21, hWnd, (HMENU)ID_TXT_ERROR_MESSAGE, 
				((LPCREATESTRUCT)lParam)->hInstance, NULL);

			/* ������ť */
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
			
			/* �����ı���Ϊֻ��ģʽ */
			SendMessage(hTxtErrorMsg, EM_SETREADONLY, TRUE, 0);

			/* �����ı�����������ַ���Ϊ 11 λ */
			SendMessage(hTxtErrorCode, EM_LIMITTEXT, 11, 0);

			/* �������� */
			//SendMessage(hTxtErrorCode, WM_SETFONT, reinterpret_cast<WPARAM>(::GetStockObject(DEFAULT_GUI_FONT)), true);
			//SendMessage(hTxtErrorMsg, WM_SETFONT, reinterpret_cast<WPARAM>(::GetStockObject(DEFAULT_GUI_FONT)), true);

			/* �������뽹�� */
			SetFocus(hTxtErrorCode);

			break;
		}
	case WM_CTLCOLORSTATIC:
		{
			/* ʹ�þ�̬�ı���ʹ��͸������ɫ */
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

							/* ������ʾֵ */
							SendMessage(hTxtErrorMsg, WM_SETTEXT, strMessage.length(), (LPARAM)strMessage.c_str());

							/* ����ȫ��ѡ�� */
							SendMessage(hTxtErrorMsg, EM_SETSEL, 0, -1);

							bSucceed = TRUE;
						}
						catch(...)
						{
							MessageBox(hWnd, TEXT("   Your Input Is Invalid��Please Check !   "), TEXT("Error"), MB_OK | MB_ICONERROR);

							bSucceed = FALSE;
						}
						
					}
					else
					{
						MessageBox(hWnd, TEXT("   Your Input Is Invalid��Please Check !   "), TEXT("Error"), MB_OK | MB_ICONERROR);

						bSucceed = FALSE;
					}

					if(bSucceed == FALSE)
					{
						/* �������뽹�� */
						SetFocus(hTxtErrorCode);
					}
					else
					{
						/* �������뽹�� */
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
/* About ���ڵĴ��ڴ������
/************************************************************************/
LRESULT CALLBACK ShowErrorAboutWndMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		{
			/* Ϊ���������չ��� WS_EX_LAYERED���Ӷ�Ϊʵ�ִ���͸����׼�� */
			LONG lExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
			lExStyle = lExStyle | WS_EX_LAYERED;
			SetWindowLong(hWnd, GWL_EXSTYLE, lExStyle);

			/* ʵ�ִ���͸�� - ���ô���͸����Ϊ 220 */
			SetLayeredWindowAttributes(hWnd, 0, WND_TRANSPARENCY, LWA_ALPHA);

			break;
		}
	case WM_CTLCOLORSTATIC:
		{
			/* ʹ�þ�̬�ı���ʹ��͸������ɫ */
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
/* ת��������Ϊ������Ϣ������ָ���Ĵ�����ת��Ϊ������Ϣ
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
/* �ж�һ���ַ����Ƿ�Ϊ����
/************************************************************************/
bool ValidateStringIsLong(string strSrc)
{
	if(strSrc.length() <= 0)
	{
		return false;
	}

	/* ȥ���ַ����е����пո� */
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
