// ConsoleApplication5.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <windows.h>
#include <iostream>
#include <initguid.h>

#include "ep_weather_host_h.h" // 包含生成的头文件




DEFINE_GUID(CLSID_EPWeather,
	0xa6ea9c2d, 0x4982, 0x4827, 0x92, 0x4, 0xa, 0xc5, 0x32, 0x95, 0x9f, 0x61);

const IID IID_IEPWeather = { 0xCDBF3734, 0xF847, 0x4F1B, { 0xB9, 0x53, 0xA6, 0x05, 0x43, 0x4D, 0xC1, 0xE7 } };

int main()
{
	// 1. 初始化 COM 库
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		std::cerr << "无法初始化 COM 库" << std::endl;
		return -1;
	}

	// 2. 获取 IEPWeather 接口的实例
	ITest* pWeather = nullptr;

	hr = CoCreateInstance(CLSID_EPWeather, // 使用你的 CLSID
		NULL,
		CLSCTX_INPROC_SERVER, // 服务器类型
		IID_IEPWeather, // 接口 ID
		(void**)&pWeather);

	if (FAILED(hr)) {
		std::cerr << "无法创建 IEPWeather 实例" << std::endl;
		CoUninitialize();
		return -1;
	}


	// 3. 调用 Show 方法
	//hr = pWeather->Show();
	hr = pWeather->vwin();
	if (SUCCEEDED(hr)) {
		std::cout << "Show 方法调用成功！" << std::endl;
	}
	else {
		std::cerr << "Show 方法调用失败" << std::endl;
	}

	// 4. 释放接口并反初始化 COM 库
	if (pWeather) {
		pWeather->Release();
	}

	CoUninitialize();

	return 0;
}
