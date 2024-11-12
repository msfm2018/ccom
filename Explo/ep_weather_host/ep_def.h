#ifndef _H_AS_SERVICE_P_H_
#define _H_AS_SERVICE_P_H_
#include "ep_main.h"
#include "ep_weather_host_h.h"


#include <ShlObj.h>

#include <windowsx.h>
#include <ShlObj.h>
#include <Shobjidl.h>
#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")
#include <netlistmgr.h>
#include <Iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
#include "WebView2.h"
#pragma comment(lib, "uxtheme.lib")
#include <ShellScalingApi.h>
#include <shlwapi.h>
#include <winnt.h>
#include <initguid.h>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include <Shlobj_core.h>



#define EP_WEATHER_NUM_SIGNALS 4

#define EP_WEATHER_TIMER_REQUEST_REPAINT 1
#define EP_WEATHER_TIMER_REQUEST_REPAINT_DELAY 1000
#define EP_WEATHER_TIMER_REQUEST_REFRESH 10
#define EP_WEATHER_TIMER_REQUEST_REFRESH_DELAY 2000
#define EP_WEATHER_TIMER_SCHEDULE_REFRESH 11
#define EP_WEATHER_TIMER_RESIZE_WINDOW 15
#define EP_WEATHER_TIMER_RESIZE_WINDOW_DELAY 150
#define EP_WEATHER_TIMER_EXECUTEDATASCRIPT 20
#define EP_WEATHER_TIMER_EXECUTEDATASCRIPT_DELAY 500


#define EP_WEATHER_KEEP_VALUE -1

#define EP_WEATHER_NUM_PROVIDERS 2
#define EP_WEATHER_PROVIDER_TEST 0
#define EP_WEATHER_PROVIDER_GOOGLE 1

#define EP_WEATHER_NUM_TUNITS 2
#define EP_WEATHER_TUNIT_CELSIUS 0
#define EP_WEATHER_TUNIT_FAHRENHEIT 1

#define EP_WEATHER_VIEW_ICONONLY 1
#define EP_WEATHER_VIEW_ICONTEMP 3
#define EP_WEATHER_VIEW_ICONTEXT 0
#define EP_WEATHER_VIEW_TEMPONLY 4
#define EP_WEATHER_VIEW_TEXTONLY 5

#define EP_WEATHER_UPDATE_NORMAL 1200
#define EP_WEATHER_UPDATE_REDUCED 3600

#define EP_WEATHER_WM_FETCH_DATA (WM_USER + 10)
#define EP_WEATHER_WM_SET_BROWSER_THEME (WM_USER + 11)
#define EP_WEATHER_WM_REBOUND_BROWSER (WM_USER + 12)
#define EP_WEATHER_WM_SETDEVMODE (WM_USER + 13)
#define EP_WEATHER_WM_SETZOOMFACTOR (WM_USER + 14)

#define EP_WEATHER_HEIGHT_ERROR 280
#define EP_WEATHER_HEIGHT 353
#define EP_WEATHER_WIDTH 673

#define EP_WEATHER_ICONPACK_MICROSOFT 0
#define EP_WEATHER_ICONPACK_GOOGLE 1

ICoreWebView2* webviewWindow;
ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler* webEnvHandler;
ICoreWebView2CreateCoreWebView2ControllerCompletedHandler* webCompletedHandler;

typedef interface EPWeather
{
	CONST_VTBL ITestVtbl * lpVtbl;
	unsigned int cbCount;


	HANDLE hMainThread;
	HRESULT hrLastError;
	ITaskbarList* pTaskList;



  

    /**/HANDLE hInitializeEvent;//
    HWND hWnd;

    INT64 bBrowserBusy; // interlocked
    HWND hNotifyWnd; // interlocked
    LONG64 dwTemperatureUnit; // interlocked
    LONG64 dwUpdateSchedule; // interlocked
    WCHAR wszTerm[MAX_PATH];
    WCHAR wszLanguage[MAX_PATH];
    LONG64 cbx; // interlocked
    LONG64 cby; // interlocked
    LONG64 dwProvider; // interlocked
    LONG64 bIsNavigatingToError; // interlocked
    LONG64 g_darkModeEnabled; // interlocked
    LONG64 dwGeolocationMode;
    LONG64 dwWindowCornerPreference;
    LONG64 dwDevMode;
    LONG64 dwTextDir;
    LONG64 dwIconPack;
    LONG64 dwZoomFactor;

    /**/HANDLE hMutexData;// // protects the following:
    DWORD cbTemperature;
    /*//*/LPCWSTR wszTemperature;//
    DWORD cbUnit;
    /*//*/LPCWSTR wszUnit;//
    DWORD cbCondition;
    /*//*/LPCWSTR wszCondition;//
    DWORD cbImage;
    /*//*/char* pImage;//
    DWORD cbLocation;
    /*//*/LPCWSTR wszLocation;//
    LONG64 dwTextScaleFactor; // interlocked
    /**/HMODULE hUxtheme;//
    /**/HMODULE hShlwapi;//
    /**/HKEY hKCUAccessibility;//
    /**/HKEY hKLMAccessibility;//
    DWORD cntResizeWindow;

    RECT rcBorderThickness; // local variables:

    ICoreWebView2Controller* pCoreWebView2Controller;
    ICoreWebView2* pCoreWebView2;
   // GenericObjectWithThis* pCoreWebView2NavigationStartingEventHandler;
    EventRegistrationToken tkOnNavigationStarting;
  //  GenericObjectWithThis* pCoreWebView2NavigationCompletedEventHandler;
    EventRegistrationToken tkOnNavigationCompleted;
  //  GenericObjectWithThis* pCoreWebView2PermissionRequestedEventHandler;
    EventRegistrationToken tkOnPermissionRequested;
    RECT rc;
    LONG64 dpiXInitial;
    LONG64 dpiYInitial;
    FARPROC SHRegGetValueFromHKCUHKLMFunc;
    LONG64 cbGenericObject;

    /**/HANDLE hSignalExitMainThread;//
    /**/HANDLE hSignalKillSwitch;//
    /**/HANDLE hSignalOnAccessibilitySettingsChangedFromHKCU;//
    /**/HANDLE hSignalOnAccessibilitySettingsChangedFromHKLM;//
}EPWeather;


ULONG   STDMETHODCALLTYPE def_addRef(EPWeather* _this);
ULONG   STDMETHODCALLTYPE def_release(EPWeather* _this);
HRESULT STDMETHODCALLTYPE def_qry(EPWeather* _this, REFIID riid, void** ppv);


HRESULT STDMETHODCALLTYPE def_show(EPWeather* _this);
HRESULT STDMETHODCALLTYPE def_hide(EPWeather* _this);
HRESULT STDMETHODCALLTYPE def_win(EPWeather* _this);


static const ITestVtbl vtable = {
	.QueryInterface = def_qry,
	.AddRef = def_addRef,
	.Release = def_release,

	.Show = def_show,
	.Hide = def_hide,
	.vwin = def_win,
};


#endif
