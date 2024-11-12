

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for ep_weather_host.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ep_weather_host_h_h__
#define __ep_weather_host_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

#ifndef __ITest_FWD_DEFINED__
#define __ITest_FWD_DEFINED__
typedef interface ITest ITest;

#endif 	/* __ITest_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ITest_INTERFACE_DEFINED__
#define __ITest_INTERFACE_DEFINED__

/* interface ITest */
/* [helpstring][oleautomation][uuid][object] */ 


EXTERN_C const IID IID_ITest;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDBF3734-F847-4F1B-B953-A605434DC1E7")
    ITest : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Show( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Hide( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE vwin( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ITestVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITest * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITest * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITest * This);
        
        DECLSPEC_XFGVIRT(ITest, Show)
        HRESULT ( STDMETHODCALLTYPE *Show )( 
            ITest * This);
        
        DECLSPEC_XFGVIRT(ITest, Hide)
        HRESULT ( STDMETHODCALLTYPE *Hide )( 
            ITest * This);
        
        DECLSPEC_XFGVIRT(ITest, vwin)
        HRESULT ( STDMETHODCALLTYPE *vwin )( 
            ITest * This);
        
        END_INTERFACE
    } ITestVtbl;

    interface ITest
    {
        CONST_VTBL struct ITestVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITest_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITest_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITest_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITest_Show(This)	\
    ( (This)->lpVtbl -> Show(This) ) 

#define ITest_Hide(This)	\
    ( (This)->lpVtbl -> Hide(This) ) 

#define ITest_vwin(This)	\
    ( (This)->lpVtbl -> vwin(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITest_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


