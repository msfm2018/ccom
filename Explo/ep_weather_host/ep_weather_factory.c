#include "ep_weather_factory.h"
#include "ep_def.h"

ULONG STDMETHODCALLTYPE factory_AddRef(IClassFactory* _this) { return 1; }

ULONG STDMETHODCALLTYPE factory_Release(IClassFactory* _this) { return 1; }

HRESULT STDMETHODCALLTYPE factory_qy(IClassFactory* _this, REFIID riid, void** ppv)
{
	if (!IsEqualIID(riid, &IID_IUnknown) && !IsEqualIID(riid, &IID_IClassFactory))
	{
		*ppv = 0;
		return(E_NOINTERFACE);
	}
	*ppv = _this;
	_this->lpVtbl->AddRef(_this);
	return(NOERROR);
}

HRESULT STDMETHODCALLTYPE factory_LockServer(IClassFactory* this, BOOL flock)
{
	if (flock) InterlockedIncrement(&epw_LockCount);
	else
	{
		LONG dwOutstandingLocks = InterlockedDecrement(&epw_LockCount);
		LONG dwOutstandingObjects = InterlockedAdd(&epw_OutstandingObjects, 0);
		if (!dwOutstandingObjects && !dwOutstandingLocks)
		{
		}
	}
	return(NOERROR);
}

HRESULT STDMETHODCALLTYPE factory_Instance(IClassFactory* _this, IUnknown* punkOuter, REFIID vTableGuid, void** ppv)
{
	HRESULT hr = E_NOINTERFACE;
	EPWeather* thisobj = NULL;
	*ppv = 0;

	if (punkOuter)
	{
		hr = CLASS_E_NOAGGREGATION;
	}
	else
	{
		BOOL bOk = FALSE;
		if (IsEqualIID(vTableGuid, &IID_IEPWeather))
		{
			if (!(thisobj = calloc(1, sizeof(EPWeather))))
			{
				hr = E_OUTOFMEMORY;
			}
			else
			{
				thisobj->lpVtbl = &vtable;
				bOk = TRUE;
			}
		}
		if (bOk)
		{
			thisobj->cbCount = 1;
			hr = thisobj->lpVtbl->QueryInterface(thisobj, vTableGuid, ppv);
			thisobj->lpVtbl->Release(thisobj);
			if (SUCCEEDED(hr)) InterlockedIncrement(&epw_OutstandingObjects);
		}
		else
		{
			return hr;
		}
	}

	return(hr);
}