
#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <WinUser.h>

#include "ep_def.h"
#include "osutility.h"


#define EPW_WEATHER_CLASSNAME "weather_class"

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

HRESULT STDMETHODCALLTYPE NavigationCompletedHandlerInvoke(ICoreWebView2NavigationCompletedEventHandler* This, ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args) {
	BOOL isSuccess;
	args->lpVtbl->get_IsSuccess(args, &isSuccess);
	static BOOL bErrorDisplayed = FALSE;
	if (!isSuccess && !bErrorDisplayed) {
		//webviewWindow->lpVtbl->NavigateToString(webviewWindow, error_html);
		bErrorDisplayed = TRUE;  // 避免无限循环
	}
	return S_OK;
}


// Define the virtual table for NavigationCompletedHandler
static const ICoreWebView2NavigationCompletedEventHandlerVtbl NavigationCompletedHandler_Vtbl = {
	HandlerQueryInterface,
	HandlerAddRef,
	HandlerRelease,
	NavigationCompletedHandlerInvoke
};

NavigationCompletedHandler* CreateNavigationCompletedHandler() {
	NavigationCompletedHandler* handler = (NavigationCompletedHandler*)malloc(sizeof(NavigationCompletedHandler));
	handler->lpVtbl = &NavigationCompletedHandler_Vtbl;
	handler->refCount = 1;
	return handler;
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
			RECT bounds;
			GetClientRect(hWnd, &bounds);


			webviewController = controller;
			webviewController->lpVtbl->put_Bounds(webviewController, bounds);
			webviewController->lpVtbl->get_CoreWebView2(webviewController, &webviewWindow);
			webviewController->lpVtbl->AddRef(webviewController);
			// 在此处添加 NavigationCompleted 事件处理程序



		}

		ICoreWebView2Settings* Settings;
		RECT bounds;
		GetClientRect(hWnd, &bounds);
		if (webviewController != NULL)
		{
			webviewController->lpVtbl->put_Bounds(webviewController, bounds);
		}

		if (webviewWindow != NULL)
		{
			webviewWindow->lpVtbl->get_Settings(webviewWindow, &Settings);
			Settings->lpVtbl->put_IsScriptEnabled(Settings, TRUE);
			Settings->lpVtbl->put_AreDefaultScriptDialogsEnabled(Settings, TRUE);
			Settings->lpVtbl->put_IsWebMessageEnabled(Settings, TRUE);
			Settings->lpVtbl->put_AreDevToolsEnabled(Settings, FALSE);
			Settings->lpVtbl->put_AreDefaultContextMenusEnabled(Settings, TRUE);
			Settings->lpVtbl->put_IsStatusBarEnabled(Settings, TRUE);



			_epw_Weather_NavigateToProvider(This);



			// 在此处添加 NavigationCompleted 事件处理程序

			EventRegistrationToken tkOnNavigationCompleted;
			NavigationCompletedHandler* navCompletedHandler = CreateNavigationCompletedHandler();
			webviewWindow->lpVtbl->add_NavigationCompleted(webviewWindow, (ICoreWebView2NavigationCompletedEventHandler*)navCompletedHandler, &tkOnNavigationCompleted);
		}
		//ExecuteScriptExample(webviewWindow);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE _epw_Weather_NavigateToProvider(EPWeather* _this)
{
	// _ep_Weather_ReboundBrowser(_this, FALSE);
	HRESULT hr = S_OK;
	{
		if (webviewWindow)
		{

			hr = webviewWindow->lpVtbl->Navigate(webviewWindow, L"http://127.0.0.1:8019");

		}
		else
		{
			hr = E_FAIL;
		}
		if (FAILED(hr))
		{
		}
	}
	return hr;
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
		if (_this->hMainThread)
		{

			CloseHandle(_this->hMainThread);

		}
		if (_this->hInitializeEvent)
		{
			CloseHandle(_this->hInitializeEvent);
		}




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

	//else if (uMsg == WM_CLOSE || (uMsg == WM_KEYUP && wParam == VK_ESCAPE) || (uMsg == WM_ACTIVATEAPP && wParam == FALSE && GetAncestor(GetForegroundWindow(), GA_ROOT) != _this->hWnd))
	//{
	//	epw_Weather_Hide(_this);
	//	return 0;
	//}
	else if (uMsg == WM_SHOWWINDOW && webviewController)
	{
		RECT bounds;
		GetClientRect(hWnd, &bounds);
		webviewController->lpVtbl->put_Bounds(webviewController, bounds);
	}
	else	if (uMsg == WM_DPICHANGED)
	{
		printf("WM_DPICHANGED  \n");
		RECT* rc = lParam;
		SetWindowPos(hWnd, NULL, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top, 0);

		if (webviewController) {
			webviewController->lpVtbl->put_Bounds(webviewController, *rc);
		}
		return 0;
	}
	else if (uMsg == WM_SIZE && webviewController)
	{
		RECT bounds;
		GetClientRect(hWnd, &bounds);
		webviewController->lpVtbl->put_Bounds(webviewController, bounds);
		return 0;
	}


	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}


DWORD WINAPI bar_Weather_MainThread(EPWeather* _this) {

	BOOL bShouldReleaseBecauseClientDied = FALSE;

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);



	WNDCLASSW wc = {};
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = epw_hModule;
	wc.lpszClassName = _T(EPW_WEATHER_CLASSNAME);

	//wc.hbrBackground = IsWindows11 ? (HBRUSH)GetStockObject(BLACK_BRUSH) : NULL;
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

	//int workAreaWidth = workArea.right - workArea.left - rc.right + rc.left + 20;
	////int workAreaHeight = workArea.bottom - workArea.top - rc.bottom + rc.top + 20;
	//int workAreaHeight = workArea.top;
	//int screenWidth = GetSystemMetrics(SM_CXSCREEN);  // 获取屏幕宽度
	////hWnd = CreateWindowExW(dwExStyle, _T(EPW_WEATHER_CLASSNAME), L"", WS_OVERLAPPED | dwStyle, workAreaWidth, workAreaHeight, rc.right - rc.left - 20, rc.bottom - rc.top - 20, NULL, NULL, epw_hModule, _this); // 1030, 630
	////hWnd = CreateWindowExW(dwExStyle, _T(EPW_WEATHER_CLASSNAME), L"weather", WS_OVERLAPPED | dwStyle, workAreaWidth, workAreaHeight,
	//hWnd = CreateWindowExW(dwExStyle, _T(EPW_WEATHER_CLASSNAME), L"weather", WS_OVERLAPPED | dwStyle, screenWidth * 0.7, workAreaHeight,
	//	//rc.right - rc.left - 20, rc.bottom - rc.top - 20, NULL, NULL, epw_hModule, _this); // 1030, 630
	//	screenWidth * 0.3, workArea.bottom, NULL, NULL, epw_hModule, _this); // 1030, 630









	int workAreaWidth = workArea.right - workArea.left - rc.right + rc.left + 20;
	//int workAreaHeight = workArea.bottom - workArea.top - rc.bottom + rc.top + 20;
	int workAreaHeight = workArea.top;
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);  // 获取屏幕宽度
	//hWnd = CreateWindowExW(dwExStyle, _T(EPW_WEATHER_CLASSNAME), L"", WS_OVERLAPPED | dwStyle, workAreaWidth, workAreaHeight, rc.right - rc.left - 20, rc.bottom - rc.top - 20, NULL, NULL, epw_hModule, _this); // 1030, 630
	//hWnd = CreateWindowExW(dwExStyle, _T(EPW_WEATHER_CLASSNAME), L"weather", WS_OVERLAPPED | dwStyle, workAreaWidth, workAreaHeight,
	hWnd = CreateWindowExW(dwExStyle, _T(EPW_WEATHER_CLASSNAME), L"weather", WS_OVERLAPPED | dwStyle, screenWidth * 0.3, workAreaHeight,
		//rc.right - rc.left - 20, rc.bottom - rc.top - 20, NULL, NULL, epw_hModule, _this); // 1030, 630
		screenWidth * 0.7, workArea.bottom, NULL, NULL, epw_hModule, _this); // 1030, 630

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
	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
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
HRESULT STDMETHODCALLTYPE def_show(EPWeather* _this)
{
	SetLastError(0);
	LONG_PTR dwExStyle = GetWindowLongPtrW(_this->hWnd, GWL_EXSTYLE);
	if (!GetLastError())
	{
		SetWindowLongPtrW(_this->hWnd, GWL_EXSTYLE, ~WS_EX_TOOLWINDOW & dwExStyle);
	}
	ShowWindow(_this->hWnd, SW_SHOW);
	SetWindowPos(_this->hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE def_main(EPWeather* _this)
{
	_this->hInitializeEvent = CreateEventW(NULL, FALSE, FALSE, NULL);

	_this->hMainThread = CreateThread(NULL, 0, bar_Weather_MainThread, _this, 0, NULL);

	//WaitForSingleObject(_this->hInitializeEvent, INFINITE);


	return S_OK;
}

HRESULT STDMETHODCALLTYPE def_hide(EPWeather* _this)
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

