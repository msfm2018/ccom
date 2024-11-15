#ifndef _H_AS_H_
#define _H_AS_H_
#include <initguid.h>
#include <Windows.h>
#include <tchar.h>
#include <Shlwapi.h>

#pragma comment(lib, "Version.lib")
#pragma comment(lib, "Shlwapi.lib")


extern HMODULE epw_hModule;
extern DWORD epw_OutstandingObjects;
extern DWORD epw_LockCount;


//代表了这个类 用于注册表
#define CLSID_Test_Name "my test demo"
#define CLSID_Test_TEXT "{A6EA9C2D-4982-4827-9204-0AC532959F61}"
DEFINE_GUID(CLSID_EPWeather,
	0xa6ea9c2d, 0x4982, 0x4827, 0x92, 0x4, 0xa, 0xc5, 0x32, 0x95, 0x9f, 0x61);



// 代表了这个类的一个接口  iid生成接口方法
#if defined(__cplusplus) && !defined(CINTERFACE)
#else
DEFINE_GUID(IID_IEPWeather,
	0xcdbf3734, 0xf847, 0x4f1b, 0xb9, 0x53, 0xa6, 0x5, 0x43, 0x4d, 0xc1, 0xe7);
#endif




#endif
