

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_MyViewerLib,0xEE38FA44,0xA470,0x42BB,0x88,0x38,0xE4,0xF5,0xBC,0x04,0x4C,0x58);


MIDL_DEFINE_GUID(IID, DIID__DMyViewer,0x26B6621A,0x6C6B,0x499E,0xAE,0x28,0x48,0xB1,0x9E,0x1E,0x68,0x26);


MIDL_DEFINE_GUID(IID, DIID__DMyViewerEvents,0x2F253F55,0xD831,0x43E5,0xA1,0x92,0xE2,0xD4,0x74,0x47,0x4D,0x67);


MIDL_DEFINE_GUID(CLSID, CLSID_MyViewer,0xAF0099DC,0x3AC8,0x4E61,0xB0,0x04,0x1D,0x6C,0xA5,0x40,0x78,0x79);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



