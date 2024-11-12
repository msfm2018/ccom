#include <windows.h>
#include <stdio.h>
#include <winternl.h>

#include <ShellScalingApi.h>
#pragma comment(lib, "Shcore.lib")

typedef LONG(NTAPI* PNTGETVERSION)(PRTL_OSVERSIONINFOW);

#define EP_WEATHER_HEIGHT 353
#define EP_WEATHER_WIDTH 673

DWORD dwWeatherZoomFactor = 0;

int IsWindows11() {
    RTL_OSVERSIONINFOW osvi;
    ZeroMemory(&osvi, sizeof(RTL_OSVERSIONINFOW));
    osvi.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);

    // 获取 RtlGetVersion 函数地址
    PNTGETVERSION RtlGetVersion = (PNTGETVERSION)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "RtlGetVersion");

    if (RtlGetVersion) {
        // 调用 RtlGetVersion 获取操作系统版本
        if (RtlGetVersion(&osvi) == 0) {
            // 检查 Windows 11 的版本信息
            if (osvi.dwMajorVersion == 10 && osvi.dwMinorVersion == 0 && osvi.dwBuildNumber >= 22000) {
                return 1; // 是 Windows 11
            }
        }
    }

    return 0; // 不是 Windows 11
}

RECT dpirc(){
    HMONITOR hMonitor = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
    UINT dpiX, dpiY;
    HRESULT hr = GetDpiForMonitor(hMonitor, MDT_DEFAULT, &dpiX, &dpiY);
    MONITORINFO mi;
    ZeroMemory(&mi, sizeof(MONITORINFO));
    mi.cbSize = sizeof(MONITORINFO);
    if (GetMonitorInfoW(hMonitor, &mi))
    {

    }

    DWORD dwTextScaleFactor = 0;  // 用于存储读取到的数值
    DWORD dwSize = sizeof(dwTextScaleFactor);  // 缓冲区大小

    // 调用 RegGetValue 读取注册表
    LONG result = RegGetValue(
        HKEY_CURRENT_USER,  // 根键
        TEXT("SOFTWARE\\Microsoft\\Accessibility"),  // 子键路径
        TEXT("TextScaleFactor"),  // 值名称
        RRF_RT_REG_DWORD,  // 预期类型：DWORD
        NULL,  // 输出类型，可以为 NULL
        &dwTextScaleFactor,  // 指向存储值的变量
        &dwSize  // 值的大小（以字节为单位）
    );

	//// 检查结果
    if (result != ERROR_SUCCESS) {
        dwTextScaleFactor = 100;
    };



    dwWeatherZoomFactor = 110.3;
  static  RECT rcWeatherFlyoutWindow;




  rcWeatherFlyoutWindow.left =  mi.rcWork.left;
  rcWeatherFlyoutWindow.top =  mi.rcWork.top;



    rcWeatherFlyoutWindow.right = rcWeatherFlyoutWindow.left + MulDiv(MulDiv(MulDiv(EP_WEATHER_WIDTH, dpiX, 96), dwTextScaleFactor, 100), dwWeatherZoomFactor, 100);
    rcWeatherFlyoutWindow.bottom = rcWeatherFlyoutWindow.top + MulDiv(MulDiv(MulDiv(EP_WEATHER_HEIGHT, dpiX, 96), dwTextScaleFactor, 100), dwWeatherZoomFactor, 100);

  

    return rcWeatherFlyoutWindow;

}