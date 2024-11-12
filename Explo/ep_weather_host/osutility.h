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

    // ��ȡ RtlGetVersion ������ַ
    PNTGETVERSION RtlGetVersion = (PNTGETVERSION)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "RtlGetVersion");

    if (RtlGetVersion) {
        // ���� RtlGetVersion ��ȡ����ϵͳ�汾
        if (RtlGetVersion(&osvi) == 0) {
            // ��� Windows 11 �İ汾��Ϣ
            if (osvi.dwMajorVersion == 10 && osvi.dwMinorVersion == 0 && osvi.dwBuildNumber >= 22000) {
                return 1; // �� Windows 11
            }
        }
    }

    return 0; // ���� Windows 11
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

    DWORD dwTextScaleFactor = 0;  // ���ڴ洢��ȡ������ֵ
    DWORD dwSize = sizeof(dwTextScaleFactor);  // ��������С

    // ���� RegGetValue ��ȡע���
    LONG result = RegGetValue(
        HKEY_CURRENT_USER,  // ����
        TEXT("SOFTWARE\\Microsoft\\Accessibility"),  // �Ӽ�·��
        TEXT("TextScaleFactor"),  // ֵ����
        RRF_RT_REG_DWORD,  // Ԥ�����ͣ�DWORD
        NULL,  // ������ͣ�����Ϊ NULL
        &dwTextScaleFactor,  // ָ��洢ֵ�ı���
        &dwSize  // ֵ�Ĵ�С�����ֽ�Ϊ��λ��
    );

	//// �����
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