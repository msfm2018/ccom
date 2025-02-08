// ConsoleApplication5.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <windows.h>
#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>  // 添加此头文件
#include <initguid.h>
#include <cstdlib>  
#include "ep_weather_host_h.h" // 包含生成的头文件

using namespace std;


DEFINE_GUID(CLSID_EPWeather,
	0xa6ea9c2d, 0x4982, 0x4827, 0x92, 0x4, 0xa, 0xc5, 0x32, 0x95, 0x9f, 0x61);

const IID IID_IEPWeather = { 0xCDBF3734, 0xF847, 0x4F1B, { 0xB9, 0x53, 0xA6, 0x05, 0x43, 0x4D, 0xC1, 0xE7 } };
ITest* pWeather = nullptr;

const char* LOG_FILE = "c:\\11\\dll_weather.log";

// 辅助函数：格式化 HRESULT 错误信息
std::string GetErrorMessage(HRESULT hr)
{
	char* msgBuffer = nullptr;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&msgBuffer, 0, NULL);
	std::string errorMsg = msgBuffer ? msgBuffer : "Unknown error";
	LocalFree(msgBuffer);
	return errorMsg;
}

// 记录日志到文件
void LogError(const std::string& message, HRESULT hr)
{
	std::ofstream logFile(LOG_FILE, std::ios::app); // 追加模式
	if (logFile.is_open()) {
		std::ostringstream oss;
		oss << message << " - HRESULT: 0x" << std::hex << hr << " (" << GetErrorMessage(hr) << ")";
		logFile << oss.str() << std::endl;
		logFile.close();
	}
}
extern "C" __declspec(dllexport) void dll_weather()
{
	// 1. 初始化 COM 库
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		LogError("CoInitialize failed", hr);
		return;
	}

	// 2. 获取 IEPWeather 接口的实例

	hr = CoCreateInstance(CLSID_EPWeather, NULL, CLSCTX_INPROC_SERVER, IID_IEPWeather, (void**)&pWeather);
	if (FAILED(hr)) {
		LogError("CoCreateInstance failed", hr);
		CoUninitialize();
		return;
	}

	// 3. 调用 Show 方法
	hr = pWeather->vwin();
	if (FAILED(hr)) {
		LogError("vwin() failed", hr);
	}

	//// 4. 释放 COM 对象
	//if (pWeather) {
	//	pWeather->Release();
	//}

	//// 5. 释放 COM 库
	//CoUninitialize();
}

extern "C" __declspec(dllexport) void dll_weather1()
{
	// 1. 初始化 COM 库
	HRESULT hr = CoInitialize(NULL);


	// 2. 获取 IEPWeather 接口的实例


	hr = CoCreateInstance(CLSID_EPWeather, // 使用你的 CLSID
		NULL,
		CLSCTX_INPROC_SERVER, // 服务器类型
		IID_IEPWeather, // 接口 ID
		(void**)&pWeather);




	// 3. 调用 Show 方法

	hr = pWeather->vwin();




}

extern "C"  __declspec(dllexport) void weather_show() {
	pWeather->Show();
}

extern "C"  __declspec(dllexport) void weather_hide() {
	pWeather->Hide();
}
extern "C"  __declspec(dllexport) void dll_unweather() {
	// 4. 释放接口并反初始化 COM 库
	if (pWeather) {
		pWeather->Release();
	}

	CoUninitialize();
}

BOOL IsUserAdmin()
{
	BOOL isAdmin = FALSE;
	HANDLE hToken;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
	{
		TOKEN_ELEVATION elevation;
		DWORD dwSize;
		if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize))
			isAdmin = elevation.TokenIsElevated;
		CloseHandle(hToken);
	}
	return isAdmin;
}

extern "C" __declspec(dllexport) BOOL RegisterCOM()
{
	WCHAR wszArgs[MAX_PATH];
	WCHAR wszOwnPath[MAX_PATH];

	// 获取自身 EXE 的完整路径
	ZeroMemory(wszOwnPath, ARRAYSIZE(wszOwnPath));
	GetModuleFileNameW(NULL, wszOwnPath, ARRAYSIZE(wszOwnPath));

	// 提取 DLL 目录
	WCHAR* lastSlash = wcsrchr(wszOwnPath, L'\\'); // 使用 wcsrchr 处理 WCHAR 字符串
	if (lastSlash)
		*lastSlash = L'\0'; // 截取目录路径

	// 生成 DLL 路径
	swprintf_s(wszArgs, MAX_PATH, L"/s \%s\\weather\\com\\ep_com_host.dll", wszOwnPath);



	WCHAR wszApp[MAX_PATH * 2];
	GetSystemDirectoryW(wszApp, MAX_PATH * 2);
	wcscat_s(wszApp, MAX_PATH * 2, L"\\regsvr32.exe");

	SHELLEXECUTEINFOW sei;
	ZeroMemory(&sei, sizeof(SHELLEXECUTEINFOW));
	sei.cbSize = sizeof(sei);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.hwnd = NULL;
	sei.hInstApp = NULL;
	sei.lpVerb = L"runas";
	sei.lpFile = wszApp;
	sei.lpParameters = wszArgs;
	sei.hwnd = NULL;
	sei.nShow = SW_NORMAL;
	if (ShellExecuteExW(&sei) && sei.hProcess)
	{
		WaitForSingleObject(sei.hProcess, INFINITE);
		DWORD dwExitCode = 0;
		if (GetExitCodeProcess(sei.hProcess, &dwExitCode) && !dwExitCode)
		{
		}
		else
		{
		}
		CloseHandle(sei.hProcess);
	}
	return TRUE;
}



extern "C" __declspec(dllexport) BOOL UnregisterCOM()
{
	WCHAR wszArgs[MAX_PATH];
	WCHAR wszOwnPath[MAX_PATH];

	// 获取自身 EXE 的完整路径
	ZeroMemory(wszOwnPath, ARRAYSIZE(wszOwnPath));
	GetModuleFileNameW(NULL, wszOwnPath, ARRAYSIZE(wszOwnPath));

	// 提取 DLL 目录
	WCHAR* lastSlash = wcsrchr(wszOwnPath, L'\\'); // 使用 wcsrchr 处理 WCHAR 字符串
	if (lastSlash)
		*lastSlash = L'\0'; // 截取目录路径

	// 生成 DLL 路径
	swprintf_s(wszArgs, MAX_PATH, L"/s /u \%s\\weather\\com\\ep_com_host.dll", wszOwnPath);



	WCHAR wszApp[MAX_PATH * 2];
	GetSystemDirectoryW(wszApp, MAX_PATH * 2);
	wcscat_s(wszApp, MAX_PATH * 2, L"\\regsvr32.exe");

	SHELLEXECUTEINFOW sei;
	ZeroMemory(&sei, sizeof(SHELLEXECUTEINFOW));
	sei.cbSize = sizeof(sei);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.hwnd = NULL;
	sei.hInstApp = NULL;
	sei.lpVerb = L"runas";
	sei.lpFile = wszApp;
	sei.lpParameters = wszArgs;
	sei.hwnd = NULL;
	sei.nShow = SW_NORMAL;
	if (ShellExecuteExW(&sei) && sei.hProcess)
	{
		WaitForSingleObject(sei.hProcess, INFINITE);
		DWORD dwExitCode = 0;
		if (GetExitCodeProcess(sei.hProcess, &dwExitCode) && !dwExitCode)
		{
		}
		else
		{
		}
		CloseHandle(sei.hProcess);
	}
	return TRUE;
}





