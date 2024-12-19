#ifndef _H_AS_SERVICE_P_H_
#define _H_AS_SERVICE_P_H_
#include "ep_main.h"
#include "ep_weather_host_h.h"


#include <windowsx.h>

#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")
#include <netlistmgr.h>
#include "WebView2.h"

#include <ShellScalingApi.h>

#include <winnt.h>
#include <initguid.h>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>



#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")


ICoreWebView2* webviewWindow;
ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler* webEnvHandler;
ICoreWebView2CreateCoreWebView2ControllerCompletedHandler* webCompletedHandler;

typedef struct {
	ICoreWebView2NavigationCompletedEventHandlerVtbl* lpVtbl;
	ULONG refCount;
} NavigationCompletedHandler;


typedef interface EPWeather
{
	CONST_VTBL ITestVtbl * lpVtbl;
	unsigned int cbCount;

	HANDLE hMainThread;
	HRESULT hrLastError;

    HANDLE hInitializeEvent;
    HWND hWnd;

}EPWeather;


ULONG   STDMETHODCALLTYPE def_addRef(EPWeather* _this);
ULONG   STDMETHODCALLTYPE def_release(EPWeather* _this);
HRESULT STDMETHODCALLTYPE def_qry(EPWeather* _this, REFIID riid, void** ppv);


HRESULT STDMETHODCALLTYPE def_show(EPWeather* _this);
HRESULT STDMETHODCALLTYPE def_hide(EPWeather* _this);
HRESULT STDMETHODCALLTYPE def_main(EPWeather* _this);


static const ITestVtbl vtable = {
	.QueryInterface = def_qry,
	.AddRef = def_addRef,
	.Release = def_release,

	.Show = def_show,
	.Hide = def_hide,
	.vwin = def_main,
};


#endif
