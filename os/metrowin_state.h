#pragma once

#define slot___MODULE_THREAD_STATE 1
#define slot___THREAD_STATE 2

namespace metrowin
{
   class thread;
} // namespace metrowin

//#include "types.h"
//#include "template.h"

#ifndef __AFXTLS_H__
   #include "metrowin_thread_slots.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// ___DEBUG_STATE

#ifdef DEBUG

class ___DEBUG_STATE : public no_track_object
{
public:
   ___DEBUG_STATE();
   virtual ~___DEBUG_STATE();
};

EXTERN_PROCESS_LOCAL(___DEBUG_STATE, afxDebugState)

#endif //DEBUG

/////////////////////////////////////////////////////////////////////////////
// ___WIN_STATE


class ___WIN_STATE : public no_track_object
{
public:
   // printing abort
   bool m_bUserAbort;
};

EXTERN_PROCESS_LOCAL(___WIN_STATE, gen_WinState)


/////////////////////////////////////////////////////////////////////////////
// __MODULE_STATE : portion of state that is pushed/popped
// forward references required for __MODULE_THREAD_STATE definition
//class CHandleMap;
class hwnd_map;
class hmenu_map;
//class hdc_map;
//class hgdiobj_map;
class himagelist_map;
class frame_window;
class CToolTipCtrl;
namespace core
{
   
   class thread;
   class window;
}

#ifndef _PNH_DEFINED
typedef int (__cdecl * _PNH)( size_t );
#define _PNH_DEFINED
#endif

template<class TYPE> 
class CEmbeddedButActsLikePtr
{
public:
   inline TYPE* operator->() { return &m_data; }
   inline operator TYPE*() { return &m_data; }
   TYPE m_data;
};

// __MODULE_THREAD_STATE (local to thread *and* module)
class CLASS_DECL_metrowin __MODULE_THREAD_STATE : 
   public no_track_object
{
public:
   __MODULE_THREAD_STATE();
   virtual ~__MODULE_THREAD_STATE();

   // current thread pointer
   class ::metrowin::thread* m_pCurrentWinThread;



   // thread-local ca2 API new handler (separate from C-runtime)
   _PNH m_pfnNewHandler;


   // common controls thread state
   CToolTipCtrl* m_pToolTip;
   ::user::window * m_pLastHit;       // last ::user::window to own tooltip
   int_ptr m_nLastHit;         // last hittest code
//   TOOLINFO* m_pLastInfo;    // last TOOLINFO structure
   int_ptr m_nLastStatus;      // last flyby status message
   ::user::control_bar* m_pLastStatus; // last flyby status control bar
};

// forward references required for __MODULE_STATE definition
class application;


class ::user::window;



class CDllIsolationWrapperBase;
#ifndef ___NO_AFXCMN_SUPPORT
class CComCtlWrapper;
#endif
class CCommDlgWrapper;

// __MODULE_STATE (global data for a module)
class CLASS_DECL_metrowin __MODULE_STATE : public no_track_object
{
public:
//   __MODULE_STATE(bool bDLL, WNDPROC pfn_window_procedure, DWORD dwVersion,
  //    bool bSystem = FALSE);
   __MODULE_STATE(bool bDLL, DWORD dwVersion, bool bSystem = FALSE);
   ~__MODULE_STATE();

   ::application* m_pCurrentWinApp;
   HINSTANCE m_hCurrentInstanceHandle;
   HINSTANCE m_hCurrentResourceHandle;
   const char * m_lpszCurrentAppName;
   bool m_bDLL;    // TRUE if module is a DLL, FALSE if it is an EXE
   bool m_bSystem; // TRUE if module is a "system" module, FALSE if not
   bool m_bReserved[2]; // padding

   DWORD m_fRegisteredClasses; // flags for registered ::user::window classes

   //mutex       m_mutexRegClassList;

   hwnd_map       * m_pmapHWND;
   mutex             * m_pmutexHwnd;
//   hdc_map        * m_pmapHDC;
//   hgdiobj_map    * m_pmapHGDIOBJ;
   hmenu_map      * m_pmapHMENU;




   // number of locked OLE objects
   long m_nObjectCount;
   bool m_bUserCtrl;

   // __register_class and System.RegisterWndClass data

   string * m_pstrUnregisterList;

//   WNDPROC m_pfn_window_procedure;
   DWORD m_dwVersion;  // version that module linked against

   // variables related to a given process in a module
   //  (used to be __MODULE_PROCESS_STATE)
   void (*m_pfnFilterToolTipMessage)(MSG*, ::user::window *);


   // define thread local portions of module state
   thread_local<__MODULE_THREAD_STATE, slot___MODULE_THREAD_STATE> m_thread;

   //Fusion: declare pointer to array of pointers to isolation aware dll wrappers (ex: comctl32).
   CDllIsolationWrapperBase** m_pDllIsolationWrappers;
   //Defaults to TRUE. When FALSE - ca2 API will not activate context in __MAINTAIN_STATE2 (used by __MANAGE_STATE).
   bool   m_bSetAmbientActCtx;
   //Handle of the module context.
   HANDLE   m_hActCtx;
   void CreateActivationContext();
};

CLASS_DECL_metrowin __MODULE_STATE* __set_module_state(__MODULE_STATE* pNewState);
CLASS_DECL_metrowin __MODULE_STATE* __get_module_state();
CLASS_DECL_metrowin bool __is_module_dll();
CLASS_DECL_metrowin bool __init_current_state_app();
CLASS_DECL_metrowin __MODULE_STATE* __get_static_module_state();
CLASS_DECL_metrowin HINSTANCE __get_instance_handle_helper();

CLASS_DECL_metrowin __MODULE_THREAD_STATE* __get_module_thread_state();

#define ___CMDTARGET_GETSTATE() (m_pModuleState)

/////////////////////////////////////////////////////////////////////////////
// macros & classes to manage pushing/popping the module state

struct CLASS_DECL_metrowin __MAINTAIN_STATE
{
   explicit __MAINTAIN_STATE(__MODULE_STATE* pModuleState) throw();
   ~__MAINTAIN_STATE();

protected:
   __MODULE_STATE* m_pPrevModuleState;
};

class ___THREAD_STATE;
struct CLASS_DECL_metrowin __MAINTAIN_STATE2
{
   explicit __MAINTAIN_STATE2(__MODULE_STATE* pModuleState);
   ~__MAINTAIN_STATE2();

protected:
   __MODULE_STATE* m_pPrevModuleState;
   ___THREAD_STATE* m_pThreadState;

   ulong_ptr m_ulActCtxCookie;
   bool m_bValidActCtxCookie;
};
#define __MANAGE_STATE(p) _gen::InitManaged(); __MAINTAIN_STATE2 _ctlState(p);

/////////////////////////////////////////////////////////////////////////////
// Thread global state

// forward references required for ___THREAD_STATE definition
class view;
class CToolTipCtrl;
class push_routing_frame;
class CPushRoutingView;

#define ___TEMP_CLASS_NAME_SIZE 96
class CLASS_DECL_metrowin ___THREAD_STATE : 
   public no_track_object,
   public ::thread_state
{
public:
   ___THREAD_STATE();
   virtual ~___THREAD_STATE();

   // override for m_pModuleState in ___APP_STATE
   __MODULE_STATE* m_pModuleState;
   __MODULE_STATE* m_pPrevModuleState;

   class ::metrowin::thread* m_pCurrentWinThread;

   // primitive::memory safety pool for temp maps
   void * m_pSafetyPoolBuffer;    // current buffer

   // thread local exception context
   __EXCEPTION_CONTEXT m_exceptionContext;

   // ::user::window create, gray dialog hook, and other hook data
   ::user::interaction * m_pWndInit;
   ::user::window * m_pAlternateWndInit;      // special case commdlg hooking
   DWORD m_dwPropStyle;
   DWORD m_dwPropExStyle;
   oswindow m_hWndInit;
   HHOOK m_hHookOldCbtFilter;
   HHOOK m_hHookOldMsgFilter;

   // message pump for run
   MSG m_msgCur;                   // current message
   point m_ptCursorLast;          // last mouse position
   UINT m_nMsgLast;                // last mouse message

#ifdef DEBUG
      int m_nDisablePumpCount; // Diagnostic trap to detect illegal re-entrancy
#endif

   // other ::user::window modal data
   MSG m_lastSentMsg;              // see ::user::window::message_handler
   oswindow m_hTrackingWindow;         // see ::user::window::TrackPopupMenu
   HMENU m_hTrackingMenu;
   char m_szTempClassName[___TEMP_CLASS_NAME_SIZE];    // see System.RegisterWndClass
   oswindow m_hLockoutNotifyWindow;    // see ::user::window::OnCommand
   bool m_bInMsgFilter;

   // other framework modal data
   CPushRoutingView* m_pPushRoutingView;

   // ca2 API/DB thread-local data
   bool m_bWaitForDataSource;

   // OLE control thread-local data
   ::user::window * m_pWndPark;       // "parking space" ::user::window
   long m_nCtrlRef;        // reference count on parking ::user::window
   bool m_bNeedTerm;       // TRUE if OleUninitialize needs to be called
};

EXTERN_THREAD_LOCAL(___THREAD_STATE, gen_ThreadState, slot___THREAD_STATE)

CLASS_DECL_metrowin ___THREAD_STATE* __get_thread_state();
