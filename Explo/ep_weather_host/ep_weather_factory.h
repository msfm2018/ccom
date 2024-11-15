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



//CoCreateInstance ʱ��ϵͳ���Զ�������Ӧ�Ĺ����������� factory_qy �� factory_Instance����
//
//�����������£�
//
//���� CoCreateInstance���ͻ������󴴽�һ��ָ�� CLSID �Ķ���
//ϵͳ�����๤����ϵͳ���� CLSID ����ע����е��๤����IClassFactory����
//���� QueryInterface��ϵͳ��ͨ���๤���� QueryInterface �������� factory_qy������ȡ�ӿ�ָ�롣��ͨ�����ڹ�����ʼ���ڼ���еġ�
//���� CreateInstance��Ȼ��ϵͳ�����๤���� CreateInstance �������� factory_Instance�����Դ���������Ķ���ʵ����
//��ˣ�factory_qy �� factory_Instance ����ϵͳ�Զ����õģ��ͻ�������ֱ�ӽ�����Щ���á�