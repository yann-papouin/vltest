

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Sun Jun 19 17:12:15 2011
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

#ifndef __GenVieweridl_h__
#define __GenVieweridl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DGenViewer_FWD_DEFINED__
#define ___DGenViewer_FWD_DEFINED__
typedef interface _DGenViewer _DGenViewer;
#endif 	/* ___DGenViewer_FWD_DEFINED__ */


#ifndef ___DGenViewerEvents_FWD_DEFINED__
#define ___DGenViewerEvents_FWD_DEFINED__
typedef interface _DGenViewerEvents _DGenViewerEvents;
#endif 	/* ___DGenViewerEvents_FWD_DEFINED__ */


#ifndef __GenViewer_FWD_DEFINED__
#define __GenViewer_FWD_DEFINED__

#ifdef __cplusplus
typedef class GenViewer GenViewer;
#else
typedef struct GenViewer GenViewer;
#endif /* __cplusplus */

#endif 	/* __GenViewer_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __GenViewerLib_LIBRARY_DEFINED__
#define __GenViewerLib_LIBRARY_DEFINED__

/* library GenViewerLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_GenViewerLib;

#ifndef ___DGenViewer_DISPINTERFACE_DEFINED__
#define ___DGenViewer_DISPINTERFACE_DEFINED__

/* dispinterface _DGenViewer */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DGenViewer;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("26B6621A-6C6B-499E-AE28-48B19E1E6826")
    _DGenViewer : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DGenViewerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DGenViewer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DGenViewer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DGenViewer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DGenViewer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DGenViewer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DGenViewer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DGenViewer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DGenViewerVtbl;

    interface _DGenViewer
    {
        CONST_VTBL struct _DGenViewerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DGenViewer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DGenViewer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DGenViewer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DGenViewer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DGenViewer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DGenViewer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DGenViewer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DGenViewer_DISPINTERFACE_DEFINED__ */


#ifndef ___DGenViewerEvents_DISPINTERFACE_DEFINED__
#define ___DGenViewerEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DGenViewerEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DGenViewerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("2F253F55-D831-43E5-A192-E2D474474D67")
    _DGenViewerEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DGenViewerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DGenViewerEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DGenViewerEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DGenViewerEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DGenViewerEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DGenViewerEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DGenViewerEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DGenViewerEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DGenViewerEventsVtbl;

    interface _DGenViewerEvents
    {
        CONST_VTBL struct _DGenViewerEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DGenViewerEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DGenViewerEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DGenViewerEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DGenViewerEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DGenViewerEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DGenViewerEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DGenViewerEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DGenViewerEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_GenViewer;

#ifdef __cplusplus

class DECLSPEC_UUID("AF0099DC-3AC8-4E61-B004-1D6CA5407879")
GenViewer;
#endif
#endif /* __GenViewerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


