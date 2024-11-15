#ifndef _H_AS_FACTORY_H_
#define _H_AS_FACTORY_H_
#include "ep_main.h"
ULONG STDMETHODCALLTYPE factory_AddRef(IClassFactory* _this);
ULONG STDMETHODCALLTYPE factory_Release(IClassFactory* _this);
HRESULT STDMETHODCALLTYPE factory_qy(IClassFactory* _this);

HRESULT STDMETHODCALLTYPE factory_Instance(IClassFactory* _this);
HRESULT STDMETHODCALLTYPE factory_LockServer(IClassFactory* _this, BOOL flock);

static const IClassFactoryVtbl tVtbl = {
	factory_qy,
	factory_AddRef,
	factory_Release,
	factory_Instance,
	factory_LockServer
};

static IClassFactory icf =
{
	&tVtbl
};
static IClassFactory* objFactory = &icf;

#endif



//CoCreateInstance 时，系统会自动调用相应的工厂函数（如 factory_qy 和 factory_Instance）。
//
//具体流程如下：
//
//调用 CoCreateInstance：客户端请求创建一个指定 CLSID 的对象。
//系统查找类工厂：系统根据 CLSID 查找注册表中的类工厂（IClassFactory）。
//调用 QueryInterface：系统会通过类工厂的 QueryInterface 方法（即 factory_qy）来获取接口指针。这通常是在工厂初始化期间进行的。
//调用 CreateInstance：然后，系统调用类工厂的 CreateInstance 方法（即 factory_Instance），以创建所请求的对象实例。
//因此，factory_qy 和 factory_Instance 是由系统自动调用的，客户端无需直接进行这些调用。