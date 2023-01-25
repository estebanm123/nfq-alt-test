/* Header file automatically generated from Class.idl */
/*
 * File built with Microsoft(R) MIDLRT Compiler Engine Version 10.00.0229 
 */

#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

/* verify that the <rpcsal.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCSAL_H_VERSION__
#define __REQUIRED_RPCSAL_H_VERSION__ 100
#endif

#include <rpc.h>
#include <rpcndr.h>

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include <windows.h>
#include <ole2.h>
#endif /*COM_NO_WINDOWS_H*/
#ifndef __Class_h_h__
#define __Class_h_h__
#ifndef __Class_h_p_h__
#define __Class_h_p_h__


#pragma once

// Ensure that the setting of the /ns_prefix command line switch is consistent for all headers.
// If you get an error from the compiler indicating "warning C4005: 'CHECK_NS_PREFIX_STATE': macro redefinition", this
// indicates that you have included two different headers with different settings for the /ns_prefix MIDL command line switch
#if !defined(DISABLE_NS_PREFIX_CHECKS)
#define CHECK_NS_PREFIX_STATE "always"
#endif // !defined(DISABLE_NS_PREFIX_CHECKS)


#pragma push_macro("MIDL_CONST_ID")
#undef MIDL_CONST_ID
#define MIDL_CONST_ID const __declspec(selectany)


// Header files for imported files
#include "winrtbase.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.AI.MachineLearning.MachineLearningContract\3.0.0.0\Windows.AI.MachineLearning.MachineLearningContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.AI.MachineLearning.Preview.MachineLearningPreviewContract\2.0.0.0\Windows.AI.MachineLearning.Preview.MachineLearningPreviewContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.ApplicationModel.Calls.Background.CallsBackgroundContract\2.0.0.0\Windows.ApplicationModel.Calls.Background.CallsBackgroundContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.ApplicationModel.Calls.CallsPhoneContract\5.0.0.0\Windows.ApplicationModel.Calls.CallsPhoneContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.ApplicationModel.Calls.CallsVoipContract\4.0.0.0\Windows.ApplicationModel.Calls.CallsVoipContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.ApplicationModel.CommunicationBlocking.CommunicationBlockingContract\2.0.0.0\Windows.ApplicationModel.CommunicationBlocking.CommunicationBlockingContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.ApplicationModel.SocialInfo.SocialInfoContract\2.0.0.0\Windows.ApplicationModel.SocialInfo.SocialInfoContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.ApplicationModel.StartupTaskContract\3.0.0.0\Windows.ApplicationModel.StartupTaskContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.Devices.Custom.CustomDeviceContract\1.0.0.0\Windows.Devices.Custom.CustomDeviceContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.Devices.DevicesLowLevelContract\3.0.0.0\Windows.Devices.DevicesLowLevelContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.Devices.Printers.PrintersContract\1.0.0.0\Windows.Devices.Printers.PrintersContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.Devices.SmartCards.SmartCardBackgroundTriggerContract\3.0.0.0\Windows.Devices.SmartCards.SmartCardBackgroundTriggerContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.Devices.SmartCards.SmartCardEmulatorContract\6.0.0.0\Windows.Devices.SmartCards.SmartCardEmulatorContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.Foundation.FoundationContract\4.0.0.0\Windows.Foundation.FoundationContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.Foundation.UniversalApiContract\10.0.0.0\Windows.Foundation.UniversalApiContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.Gaming.XboxLive.StorageApiContract\1.0.0.0\Windows.Gaming.XboxLive.StorageApiContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.Graphics.Printing3D.Printing3DContract\4.0.0.0\Windows.Graphics.Printing3D.Printing3DContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.Networking.Connectivity.WwanContract\2.0.0.0\Windows.Networking.Connectivity.WwanContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.Networking.Sockets.ControlChannelTriggerContract\3.0.0.0\Windows.Networking.Sockets.ControlChannelTriggerContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.Security.Isolation.IsolatedWindowsEnvironmentContract\2.0.0.0\Windows.Security.Isolation.Isolatedwindowsenvironmentcontract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.Services.Maps.GuidanceContract\3.0.0.0\Windows.Services.Maps.GuidanceContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.Services.Maps.LocalSearchContract\4.0.0.0\Windows.Services.Maps.LocalSearchContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.Services.Store.StoreContract\4.0.0.0\Windows.Services.Store.StoreContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.Services.TargetedContent.TargetedContentContract\1.0.0.0\Windows.Services.TargetedContent.TargetedContentContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.Storage.Provider.CloudFilesContract\4.0.0.0\Windows.Storage.Provider.CloudFilesContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.System.Profile.ProfileHardwareTokenContract\1.0.0.0\Windows.System.Profile.ProfileHardwareTokenContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.System.Profile.ProfileSharedModeContract\2.0.0.0\Windows.System.Profile.ProfileSharedModeContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.System.Profile.SystemManufacturers.SystemManufacturersContract\3.0.0.0\Windows.System.Profile.SystemManufacturers.SystemManufacturersContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.System.SystemManagementContract\7.0.0.0\Windows.System.SystemManagementContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.UI.ViewManagement.ViewManagementViewScalingContract\1.0.0.0\Windows.UI.ViewManagement.ViewManagementViewScalingContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0\Windows.UI.Xaml.Core.Direct.XamlDirectContract\2.0.0.0\Windows.UI.Xaml.Core.Direct.XamlDirectContract.h"

#if defined(__cplusplus) && !defined(CINTERFACE)
#if defined(__MIDL_USE_C_ENUM)
#define MIDL_ENUM enum
#else
#define MIDL_ENUM enum class
#endif
/* Forward Declarations */
#ifndef ____x_ABI_CPeek__Win32_CIClass_FWD_DEFINED__
#define ____x_ABI_CPeek__Win32_CIClass_FWD_DEFINED__
namespace ABI {
    namespace Peek_Win32 {
        interface IClass;
    } /* Peek_Win32 */
} /* ABI */
#define __x_ABI_CPeek__Win32_CIClass ABI::Peek_Win32::IClass

#endif // ____x_ABI_CPeek__Win32_CIClass_FWD_DEFINED__


namespace ABI {
    namespace Peek_Win32 {
        class Class;
    } /* Peek_Win32 */
} /* ABI */



/*
 *
 * Interface Peek_Win32.IClass
 *
 * Interface is a part of the implementation of type Peek_Win32.Class
 *
 *
 * The IID for this interface was automatically generated by MIDLRT.
 *
 * Interface IID generation seed: Peek_Win32.IClass:HRESULT get_MaPropriete(Int32*);HRESULT put_MaPropriete(Int32);
 *
 *
 */
#if !defined(____x_ABI_CPeek__Win32_CIClass_INTERFACE_DEFINED__)
#define ____x_ABI_CPeek__Win32_CIClass_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_Peek_Win32_IClass[] = L"Peek_Win32.IClass";
namespace ABI {
    namespace Peek_Win32 {
        /* [uuid("4be13c0d-c829-551f-a252-e75e2f01038d"), version, object, exclusiveto] */
        MIDL_INTERFACE("4be13c0d-c829-551f-a252-e75e2f01038d")
        IClass : public IInspectable
        {
        public:
            /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_MaPropriete(
                /* [out, retval] */int * value
                ) = 0;
            /* [propput] */virtual HRESULT STDMETHODCALLTYPE put_MaPropriete(
                /* [in] */int value
                ) = 0;
            
        };

        extern MIDL_CONST_ID IID & IID_IClass=_uuidof(IClass);
        
    } /* Peek_Win32 */
} /* ABI */

EXTERN_C const IID IID___x_ABI_CPeek__Win32_CIClass;
#endif /* !defined(____x_ABI_CPeek__Win32_CIClass_INTERFACE_DEFINED__) */


/*
 *
 * Class Peek_Win32.Class
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    Peek_Win32.IClass ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_Peek_Win32_Class_DEFINED
#define RUNTIMECLASS_Peek_Win32_Class_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_Peek_Win32_Class[] = L"Peek_Win32.Class";
#endif


#else // !defined(__cplusplus)
/* Forward Declarations */
#ifndef ____x_ABI_CPeek__Win32_CIClass_FWD_DEFINED__
#define ____x_ABI_CPeek__Win32_CIClass_FWD_DEFINED__
typedef interface __x_ABI_CPeek__Win32_CIClass __x_ABI_CPeek__Win32_CIClass;

#endif // ____x_ABI_CPeek__Win32_CIClass_FWD_DEFINED__



/*
 *
 * Interface Peek_Win32.IClass
 *
 * Interface is a part of the implementation of type Peek_Win32.Class
 *
 *
 * The IID for this interface was automatically generated by MIDLRT.
 *
 * Interface IID generation seed: Peek_Win32.IClass:HRESULT get_MaPropriete(Int32*);HRESULT put_MaPropriete(Int32);
 *
 *
 */
#if !defined(____x_ABI_CPeek__Win32_CIClass_INTERFACE_DEFINED__)
#define ____x_ABI_CPeek__Win32_CIClass_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_Peek_Win32_IClass[] = L"Peek_Win32.IClass";
/* [uuid("4be13c0d-c829-551f-a252-e75e2f01038d"), version, object, exclusiveto] */
typedef struct __x_ABI_CPeek__Win32_CIClassVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CPeek__Win32_CIClass * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CPeek__Win32_CIClass * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CPeek__Win32_CIClass * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CPeek__Win32_CIClass * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CPeek__Win32_CIClass * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CPeek__Win32_CIClass * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
/* [propget] */HRESULT ( STDMETHODCALLTYPE *get_MaPropriete )(
        __x_ABI_CPeek__Win32_CIClass * This,
        /* [out, retval] */int * value
        );
    /* [propput] */HRESULT ( STDMETHODCALLTYPE *put_MaPropriete )(
        __x_ABI_CPeek__Win32_CIClass * This,
        /* [in] */int value
        );
    END_INTERFACE
    
} __x_ABI_CPeek__Win32_CIClassVtbl;

interface __x_ABI_CPeek__Win32_CIClass
{
    CONST_VTBL struct __x_ABI_CPeek__Win32_CIClassVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CPeek__Win32_CIClass_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CPeek__Win32_CIClass_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CPeek__Win32_CIClass_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CPeek__Win32_CIClass_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CPeek__Win32_CIClass_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CPeek__Win32_CIClass_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CPeek__Win32_CIClass_get_MaPropriete(This,value) \
    ( (This)->lpVtbl->get_MaPropriete(This,value) )

#define __x_ABI_CPeek__Win32_CIClass_put_MaPropriete(This,value) \
    ( (This)->lpVtbl->put_MaPropriete(This,value) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CPeek__Win32_CIClass;
#endif /* !defined(____x_ABI_CPeek__Win32_CIClass_INTERFACE_DEFINED__) */


/*
 *
 * Class Peek_Win32.Class
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    Peek_Win32.IClass ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_Peek_Win32_Class_DEFINED
#define RUNTIMECLASS_Peek_Win32_Class_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_Peek_Win32_Class[] = L"Peek_Win32.Class";
#endif


#endif // defined(__cplusplus)
#pragma pop_macro("MIDL_CONST_ID")
#endif // __Class_h_p_h__

#endif // __Class_h_h__
