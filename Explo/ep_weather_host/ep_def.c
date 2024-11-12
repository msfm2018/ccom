
#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <WinUser.h>

#include "ep_def.h"
#include "osutility.h"
#include "bar_html.h"


#define EPW_WEATHER_CLASSNAME "Sample Window Class"

extern  HMODULE epw_hModule;



HWND hWnd = NULL;
HWND hWnd1 = NULL;
ICoreWebView2Controller* webviewController = NULL;
BOOL bEnvCreated = FALSE;

ULONG HandlerRefCount = 0;


ULONG HandlerAddRef(IUnknown* This)
{
	return ++HandlerRefCount;
}

ULONG HandlerRelease(IUnknown* This)
{
	--HandlerRefCount;
	if (HandlerRefCount == 0)
	{
		if (webCompletedHandler)
		{
			free(webCompletedHandler->lpVtbl);
			free(webCompletedHandler);
		}
		if (webEnvHandler)
		{
			free(webEnvHandler->lpVtbl);
			free(webEnvHandler);
		}
	}
	return HandlerRefCount;
}

HRESULT HandlerQueryInterface(IUnknown* This, IID* riid, void** ppvObject)
{
	*ppvObject = This;
	HandlerAddRef(This);
	return S_OK;
}

HRESULT HandlerInvoke(IUnknown* This, HRESULT errorCode, void* arg)
{
	if (!bEnvCreated)
	{
		bEnvCreated = TRUE;

		webCompletedHandler = malloc(sizeof(ICoreWebView2CreateCoreWebView2ControllerCompletedHandler));

		webCompletedHandler->lpVtbl = malloc(sizeof(ICoreWebView2CreateCoreWebView2ControllerCompletedHandlerVtbl));

		webCompletedHandler->lpVtbl->AddRef = HandlerAddRef;
		webCompletedHandler->lpVtbl->Release = HandlerRelease;
		webCompletedHandler->lpVtbl->QueryInterface = HandlerQueryInterface;
		webCompletedHandler->lpVtbl->Invoke = HandlerInvoke;

		ICoreWebView2Environment* env = arg;
		env->lpVtbl->CreateCoreWebView2Controller(env, hWnd, webCompletedHandler);
	}
	else
	{
		ICoreWebView2Controller* controller = arg;

		if (controller != NULL) {
			webviewController = controller;
			webviewController->lpVtbl->get_CoreWebView2(webviewController, &webviewWindow);
			webviewController->lpVtbl->AddRef(webviewController);
		}

		ICoreWebView2Settings* Settings;
		webviewWindow->lpVtbl->get_Settings(webviewWindow, &Settings);
		Settings->lpVtbl->put_IsScriptEnabled(Settings, TRUE);
		Settings->lpVtbl->put_AreDefaultScriptDialogsEnabled(Settings, TRUE);
		Settings->lpVtbl->put_IsWebMessageEnabled(Settings, TRUE);
		Settings->lpVtbl->put_AreDevToolsEnabled(Settings, FALSE);
		Settings->lpVtbl->put_AreDefaultContextMenusEnabled(Settings, TRUE);
		Settings->lpVtbl->put_IsStatusBarEnabled(Settings, TRUE);

		RECT bounds;
		GetClientRect(hWnd, &bounds);
		webviewController->lpVtbl->put_Bounds(webviewController, bounds);

		_epw_Weather_NavigateToProvider(This);
	}
	//刷新一下 windows
	SetTimer(hWnd, EP_WEATHER_TIMER_REQUEST_REPAINT, EP_WEATHER_TIMER_REQUEST_REPAINT_DELAY, NULL);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE _epw_Weather_NavigateToProvider(EPWeather* _this)
{
	// _ep_Weather_ReboundBrowser(_this, FALSE);
	HRESULT hr = S_OK;

	{
		if (webviewWindow)
		{
			// webviewWindow->lpVtbl->Navigate(webviewWindow, wszScriptData);}
			// 使用 NavigateToString 加载 HTML 内容
			hr = webviewWindow->lpVtbl->NavigateToString(webviewWindow, hstr1);
		}
		else
		{
			hr = E_FAIL;
		}
		if (FAILED(hr))
		{
			InterlockedExchange64(&_this->bBrowserBusy, FALSE);
		}


	}
	return hr;
}


static void epw_Weather_SetTextScaleFactorFromRegistry(EPWeather* _this, HKEY hKey, BOOL bRefresh)
{
	DWORD dwTextScaleFactor = 100, dwSize = sizeof(DWORD);
	if (_this->SHRegGetValueFromHKCUHKLMFunc && _this->SHRegGetValueFromHKCUHKLMFunc(L"SOFTWARE\\Microsoft\\Accessibility", L"TextScaleFactor", SRRF_RT_REG_DWORD, NULL, &dwTextScaleFactor, (LPDWORD)(&dwSize)) != ERROR_SUCCESS)
	{
		dwTextScaleFactor = 100;
	}
	if (InterlockedExchange64(&_this->dwTextScaleFactor, dwTextScaleFactor) == dwTextScaleFactor)
	{
		bRefresh = FALSE;
	}
	if (hKey == HKEY_CURRENT_USER)
	{
		if (RegCreateKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Accessibility", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WOW64_64KEY | KEY_WRITE, NULL, &_this->hKCUAccessibility, NULL) == ERROR_SUCCESS)
		{
			RegNotifyChangeKeyValue(_this->hKCUAccessibility, FALSE, REG_NOTIFY_CHANGE_LAST_SET, _this->hSignalOnAccessibilitySettingsChangedFromHKCU, TRUE);
		}
	}
	else if (hKey == HKEY_LOCAL_MACHINE)
	{
		if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Accessibility", REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WOW64_64KEY | KEY_WRITE, &_this->hKLMAccessibility))
		{
			RegNotifyChangeKeyValue(_this->hKLMAccessibility, FALSE, REG_NOTIFY_CHANGE_LAST_SET, _this->hSignalOnAccessibilitySettingsChangedFromHKLM, TRUE);
		}
	}
	if (bRefresh)
	{
		_ep_Weather_StartResize(_this);
	}
}

HRESULT STDMETHODCALLTYPE _ep_Weather_StartResize(EPWeather* _this)
{
	_this->cntResizeWindow = 0;
	SetTimer(_this->hWnd, EP_WEATHER_TIMER_RESIZE_WINDOW, EP_WEATHER_TIMER_RESIZE_WINDOW_DELAY, NULL);
	return S_OK;
}


static DWORD epw_Weather_ReleaseBecauseClientDiedThread(EPWeather* _this)
{
	Sleep(5000);
	while (_this->lpVtbl->Release(_this));
	return 0;
}

ULONG STDMETHODCALLTYPE def_addRef(EPWeather* _this)
{
	ULONG value = InterlockedIncrement64(&(_this->cbCount));
	printf("[General] AddRef: %d\n", value);
	return value;
}

ULONG STDMETHODCALLTYPE def_release(EPWeather* _this)
{
	ULONG value = InterlockedDecrement64(&(_this->cbCount));
	printf("[General] Release: %d\n", value);

	if (value == 0)
	{
		free(_this);
		LONG dwOutstandingObjects = InterlockedDecrement(&epw_OutstandingObjects);
		LONG dwOutstandingLocks = InterlockedAdd(&epw_LockCount, 0);
		if (!dwOutstandingObjects && !dwOutstandingLocks)
		{
		}
		printf("[General] Outstanding objects: %d, outstanding locks: %d\n", dwOutstandingObjects, dwOutstandingLocks);

		return(0);
	}
	return value;
}

HRESULT STDMETHODCALLTYPE def_qry(EPWeather* _this, REFIID riid, void** ppv)
{
	if (!IsEqualIID(riid, &IID_IEPWeather) && !IsEqualIID(riid, &IID_IUnknown))
	{
		*ppv = 0;
		return(E_NOINTERFACE);
	}
	*ppv = _this;
	_this->lpVtbl->AddRef(_this);
	return(NOERROR);
}

HRESULT STDMETHODCALLTYPE def_show(EPWeather* _this)
{
	MessageBox(0, L"aaaa", L"show", 0);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE def_hide(EPWeather* _this)
{
	MessageBox(0, L"aaaa", L"hide", 0);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE epw_Weather_Hide(EPWeather* _this)
{
	SetLastError(0);
	LONG_PTR dwExStyle = GetWindowLongPtrW(_this->hWnd, GWL_EXSTYLE);
	if (!GetLastError())
	{
		SetWindowLongPtrW(_this->hWnd, GWL_EXSTYLE, ~WS_EX_TOOLWINDOW & dwExStyle);
	}
	ShowWindow(_this->hWnd, SW_HIDE);
	return S_OK;
}


LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	EPWeather* _this = NULL;
	if (uMsg == WM_CREATE)
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)(lParam);
		_this = (int*)(pCreate->lpCreateParams);
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)_this);
	}
	else
	{
		//get this 参数
		LONG_PTR ptr = GetWindowLongPtrW(hWnd, GWLP_USERDATA);
		_this = (EPWeather*)(ptr);
	}
	if (!_this)
	{
		printf("this is null\n");
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}
	if (uMsg == WM_TIMER && wParam == EP_WEATHER_TIMER_REQUEST_REPAINT)
	{
		HWND hNotifyWnd = InterlockedAdd64(&_this->hNotifyWnd, 0);
		printf("[Timer Repaint] Request posted to window %x.\n", hNotifyWnd);
		if (hNotifyWnd)
		{
			InvalidateRect(hNotifyWnd, NULL, TRUE);
			//Sleep(100);
			//InvalidateRect(hNotifyWnd, NULL, TRUE);
		}
		KillTimer(_this->hWnd, EP_WEATHER_TIMER_REQUEST_REPAINT);
		return 0;
	}
	
	else if (uMsg == WM_TIMER && wParam == EP_WEATHER_TIMER_REQUEST_REFRESH)
	{
		KillTimer(_this->hWnd, EP_WEATHER_TIMER_REQUEST_REFRESH);
		return SendMessageW(_this->hWnd, EP_WEATHER_WM_FETCH_DATA, 0, 0);
	}
	else if (uMsg == WM_CLOSE || (uMsg == WM_KEYUP && wParam == VK_ESCAPE) || (uMsg == WM_ACTIVATEAPP && wParam == FALSE && GetAncestor(GetForegroundWindow(), GA_ROOT) != _this->hWnd))
	{
		epw_Weather_Hide(_this);
		return 0;
	}
	
	else

		if (uMsg == WM_DPICHANGED)
		{
			printf("WM_DPICHANGED  \n");
			RECT* rc = lParam;
			SetWindowPos(hWnd, NULL, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top, 0);
			return 0;
		}


	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}



DWORD WINAPI bar_Weather_MainThread(EPWeather* _this) {

	HRESULT hr = S_OK;
	BOOL bShouldReleaseBecauseClientDied = FALSE;

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	_this->hrLastError = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	hr = CoInitialize(NULL);
	_this->hrLastError = CoCreateInstance(&CLSID_TaskbarList, NULL, CLSCTX_INPROC, &IID_ITaskbarList, (LPVOID*)&_this->pTaskList);


	WNDCLASSW wc = {};
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = epw_hModule;
	wc.lpszClassName = _T(EPW_WEATHER_CLASSNAME);
	// wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);// IsWindows11 ? (HBRUSH)GetStockObject(GRAY_BRUSH) : NULL;
	wc.hbrBackground = IsWindows11 ? (HBRUSH)GetStockObject(BLACK_BRUSH) : NULL;
	wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
	RegisterClassW(&wc);

	DWORD dwStyle = WS_POPUP;// | WS_SIZEBOX;
	DWORD dwExStyle = 0;
	HMONITOR hMonitor = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
	UINT dpiX, dpiY;
	GetDpiForMonitor(hMonitor, MDT_DEFAULT, &dpiX, &dpiY);
	RECT rc = dpirc();
	AdjustWindowRectExForDpi(&rc, dwStyle, FALSE, dwExStyle, dpiX);

	RECT workArea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);

	int workAreaWidth = workArea.right - workArea.left - rc.right + rc.left + 20;
	int workAreaHeight = workArea.bottom - workArea.top - rc.bottom + rc.top + 20;

	hWnd = CreateWindowExW(dwExStyle, _T(EPW_WEATHER_CLASSNAME), L"", WS_OVERLAPPED | dwStyle, workAreaWidth, workAreaHeight, rc.right - rc.left - 20, rc.bottom - rc.top - 20, NULL, NULL, epw_hModule, _this); // 1030, 630

	_this->hWnd = hWnd;
	hWnd1 = hWnd;
	ShowWindow(hWnd, 1);

	//做一个圆角窗口
	DWM_WINDOW_CORNER_PREFERENCE cornerPreference = DWMWCP_ROUND;
	DwmSetWindowAttribute(hWnd, DWMWA_WINDOW_CORNER_PREFERENCE, &cornerPreference, sizeof(cornerPreference));

	webEnvHandler = malloc(sizeof(ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler));


	webEnvHandler->lpVtbl = malloc(sizeof(ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandlerVtbl));


	webEnvHandler->lpVtbl->AddRef = HandlerAddRef;
	webEnvHandler->lpVtbl->Release = HandlerRelease;
	webEnvHandler->lpVtbl->QueryInterface = HandlerQueryInterface;
	webEnvHandler->lpVtbl->Invoke = HandlerInvoke;

	UpdateWindow(hWnd);

	CreateCoreWebView2EnvironmentWithOptions(NULL, NULL, NULL, webEnvHandler);

	MSG msg = { };


	BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			break;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}




}
HRESULT STDMETHODCALLTYPE def_win(EPWeather* _this)
{
	_this->hInitializeEvent = CreateEventW(NULL, FALSE, FALSE, NULL);

	_this->hMainThread = CreateThread(NULL, 0, bar_Weather_MainThread, _this, 0, NULL);

	WaitForSingleObject(_this->hInitializeEvent, INFINITE);


	return S_OK;
}



