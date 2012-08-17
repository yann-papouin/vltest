

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Thu Jun 28 14:12:51 2012
 */
/* Compiler settings for .\GenViewer.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __GenViewer_ocxidl_h__
#define __GenViewer_ocxidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DMyViewer_FWD_DEFINED__
#define ___DMyViewer_FWD_DEFINED__
typedef interface _DMyViewer _DMyViewer;
#endif 	/* ___DMyViewer_FWD_DEFINED__ */


#ifndef ___DMyViewerEvents_FWD_DEFINED__
#define ___DMyViewerEvents_FWD_DEFINED__
typedef interface _DMyViewerEvents _DMyViewerEvents;
#endif 	/* ___DMyViewerEvents_FWD_DEFINED__ */


#ifndef __MyViewer_FWD_DEFINED__
#define __MyViewer_FWD_DEFINED__

#ifdef __cplusplus
typedef class MyViewer MyViewer;
#else
typedef struct MyViewer MyViewer;
#endif /* __cplusplus */

#endif 	/* __MyViewer_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __MyViewerLib_LIBRARY_DEFINED__
#define __MyViewerLib_LIBRARY_DEFINED__

/* library MyViewerLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_MyViewerLib;

#ifndef ___DMyViewer_DISPINTERFACE_DEFINED__
#define ___DMyViewer_DISPINTERFACE_DEFINED__

/* dispinterface _DMyViewer */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DMyViewer;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("26B6621A-6C6B-499E-AE28-48B19E1E6826")
    _DMyViewer : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DMyViewerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DMyViewer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DMyViewer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DMyViewer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DMyViewer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DMyViewer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DMyViewer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DMyViewer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DMyViewerVtbl;

    interface _DMyViewer
    {
        CONST_VTBL struct _DMyViewerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DMyViewer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DMyViewer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DMyViewer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DMyViewer_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DMyViewer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DMyViewer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DMyViewer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DMyViewer_DISPINTERFACE_DEFINED__ */


#ifndef ___DMyViewerEvents_DISPINTERFACE_DEFINED__
#define ___DMyViewerEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DMyViewerEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DMyViewerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("2F253F55-D831-43E5-A192-E2D474474D67")
    _DMyViewerEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DMyViewerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DMyViewerEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DMyViewerEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DMyViewerEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DMyViewerEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DMyViewerEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DMyViewerEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DMyViewerEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DMyViewerEventsVtbl;

    interface _DMyViewerEvents
    {
        CONST_VTBL struct _DMyViewerEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DMyViewerEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DMyViewerEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DMyViewerEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DMyViewerEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DMyViewerEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DMyViewerEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DMyViewerEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DMyViewerEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_MyViewer;

#ifdef __cplusplus

class DECLSPEC_UUID("AF0099DC-3AC8-4E61-B004-1D6CA5407879")
MyViewer;
#endif
#endif /* __MyViewerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


