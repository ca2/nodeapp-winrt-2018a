#include "StdAfx.h"

#define COMPILE_MULTIMON_STUBS
#include <multimon.h>

#include "sal.h"


AFX_STATIC void CLASS_DECL_VMSWIN _AfxPreInitDialog(
   ::user::interaction * pWnd, LPRECT lpRectOld, DWORD* pdwStyleOld);
AFX_STATIC void CLASS_DECL_VMSWIN _AfxPostInitDialog(
   ::user::interaction * pWnd, const RECT& rectOld, DWORD dwStyleOld);
LRESULT CALLBACK
_AfxActivationWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

AFX_STATIC_DATA const char _afxOldWndProc[] = "AfxOldWndProc423";

const char _afxWndControlBar[] = AFX_WNDCONTROLBAR;
const char _afxWndMDIFrame[] = AFX_WNDMDIFRAME;
const char _afxWndFrameOrView[] = AFX_WNDFRAMEORVIEW;
const char _afxWndOleControl[] = AFX_WNDOLECONTROL;

   struct AFX_CTLCOLOR
   {
      HWND hWnd;
      HDC hDC;
      UINT nCtlType;
   };

const char _afxWnd[] = AFX_WND;

namespace win
{

   void window::mouse_hover_add(::user::interaction* pinterface)
   {
      m_guieptraMouseHover.add_unique(pinterface);
   }

   void window::mouse_hover_remove(::user::interaction* pinterface)
   {
      m_guieptraMouseHover.remove(pinterface); 
   }

   window::window()
   {
      m_pcallback = NULL;
      m_pguie = this;
      set_handle(NULL);
      m_pguieOwner = NULL;
      m_pguie->m_nFlags = 0;
      m_pfnSuper = NULL;
      m_nModalResult = 0;
      m_bMouseHover = false;
      m_pfont = NULL;
      m_pguieCapture = NULL;
   }

   void window::construct(HWND hWnd)
   {
      m_pcallback = NULL;
      m_pguie = this;
      set_handle(hWnd);
      m_pguie->m_nFlags = 0;
      m_pfnSuper = NULL;
      m_nModalResult = 0;
      m_bMouseHover = false;
      m_pfont = NULL;
      m_pguieCapture = NULL;
   }

   window::window(::ca::application * papp) :
      ca(papp),
      ::user::interaction(papp)
   {
      m_pcallback = NULL;
      m_pguie = this;
      set_handle(NULL);
      m_pguieOwner = NULL;
      m_pguie->m_nFlags = 0;
      m_pfnSuper = NULL;
      m_nModalResult = 0;
      m_bMouseHover = false;
      m_pfont = NULL;
      m_pguieCapture = NULL;
   }

   ::ca::window * window::from_os_data(void * pdata)
   {
      return dynamic_cast <::ca::window *>(from_handle((HWND) pdata));   
   }

   void * window::get_os_data() const
   {
      return get_handle();
   }



   // Change a window's style

   AFX_STATIC BOOL CLASS_DECL_VMSWIN _AfxModifyStyle(HWND hWnd, int nStyleOffset,
      DWORD dwRemove, DWORD dwAdd, UINT nFlags)
   {
      ASSERT(hWnd != NULL);
      DWORD dwStyle = ::GetWindowLong(hWnd, nStyleOffset);
      DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
      if (dwStyle == dwNewStyle)
         return FALSE;

      ::SetWindowLong(hWnd, nStyleOffset, dwNewStyle);
      if (nFlags != 0)
      {
         ::SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
            SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
      }
      return TRUE;
   }

   BOOL PASCAL
   window::ModifyStyle(HWND hWnd, DWORD dwRemove, DWORD dwAdd, UINT nFlags)
   {
      return _AfxModifyStyle(hWnd, GWL_STYLE, dwRemove, dwAdd, nFlags);
   }

   BOOL PASCAL
   window::ModifyStyleEx(HWND hWnd, DWORD dwRemove, DWORD dwAdd, UINT nFlags)
   {
      return _AfxModifyStyle(hWnd, GWL_EXSTYLE, dwRemove, dwAdd, nFlags);
   }



   const MSG* PASCAL window::GetCurrentMessage()
   {
      // fill in time and position when asked for
      _AFX_THREAD_STATE* pThreadState = _afxThreadState.get_data();
      pThreadState->m_lastSentMsg.time = ::GetMessageTime();
      pThreadState->m_lastSentMsg.pt = point(::GetMessagePos());
      return &pThreadState->m_lastSentMsg;
   }

   LRESULT window::Default()
   {
      // call DefWindowProc with the last message
      _AFX_THREAD_STATE* pThreadState = _afxThreadState.get_data();
      return DefWindowProc(pThreadState->m_lastSentMsg.message,
         pThreadState->m_lastSentMsg.wParam, pThreadState->m_lastSentMsg.lParam);
   }


   window * PASCAL window::from_handle(HWND hWnd)
   {
      hwnd_map* pMap = afxMapHWND(TRUE); //create ::collection::map if not exist
      ASSERT(pMap != NULL);
      window * pWnd =  pMap->from_handle(hWnd);
      if(pWnd != NULL && WIN_WINDOW(pWnd)->get_handle() != hWnd)
         return NULL;
      return pWnd;
   }

   window * PASCAL window::FromHandlePermanent(HWND hWnd)
   {
      hwnd_map* pMap = afxMapHWND();
      window * pWnd = NULL;
      if (pMap != NULL)
      {
         // only look in the permanent ::collection::map - does no allocations
         pWnd = pMap->lookup_permanent(hWnd);
         if(pWnd != NULL && WIN_WINDOW(pWnd)->get_handle() != hWnd)
            return NULL;
      }
      return pWnd;
   }

   BOOL window::Attach(HWND hWndNew)
   {
      ASSERT(get_handle() == NULL);     // only attach once, detach on destroy
      ASSERT(FromHandlePermanent(hWndNew) == NULL);
         // must not already be in permanent ::collection::map

      if (hWndNew == NULL)
         return FALSE;

      hwnd_map * pMap = afxMapHWND(TRUE); // create ::collection::map if not exist
      ASSERT(pMap != NULL);

      pMap->set_permanent(set_handle(hWndNew), this);
      if(m_pguie == NULL)
      {
         m_pguie = this;
      }

      return TRUE;
   }

   HWND window::Detach()
   {
      HWND hWnd = get_handle();
      if (hWnd != NULL)
      {
         hwnd_map * pMap = afxMapHWND(); // don't create if not exist
         if (pMap != NULL)
            pMap->remove_handle(get_handle());
      set_handle(NULL);
      }

      return hWnd;
   }

   void window::pre_subclass_window()
   {
      // no default processing
   }


   /////////////////////////////////////////////////////////////////////////////
   // window creation

   BOOL window::CreateEx(DWORD dwExStyle, const char * lpszClassName,
         const char * lpszWindowName, DWORD dwStyle,
         const RECT& rect, ::user::interaction* pParentWnd, id id,
         LPVOID lpParam /* = NULL */)
   {
      return CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle,
         rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
         pParentWnd->get_safe_handle(), id, lpParam);
   }

   BOOL window::CreateEx(DWORD dwExStyle, const char * lpszClassName,
      const char * lpszWindowName, DWORD dwStyle,
      int x, int y, int nWidth, int nHeight,
      HWND hWndParent, id id, LPVOID lpParam)
   {
      UNREFERENCED_PARAMETER(id);
      ASSERT(lpszClassName == NULL || AfxIsValidString(lpszClassName) || 
         AfxIsValidAtom(lpszClassName));
      ENSURE_ARG(lpszWindowName == NULL || AfxIsValidString(lpszWindowName));
      
      // allow modification of several common create parameters
      CREATESTRUCT cs;
      cs.dwExStyle = dwExStyle;
      cs.lpszClass = lpszClassName;
      cs.lpszName = lpszWindowName;
      cs.style = dwStyle;
      cs.x = x;
      cs.y = y;
      cs.cx = nWidth;
      cs.cy = nHeight;
      cs.hwndParent = hWndParent;
   //   cs.hMenu = hWndParent == NULL ? NULL : nIDorHMenu;
      cs.hMenu = NULL;
      cs.hInstance = System.m_hInstance;
      cs.lpCreateParams = lpParam;

      if(m_pguie != NULL && m_pguie != this)
      {
         if(!m_pguie->PreCreateWindow(cs))
         {
            PostNcDestroy();
            return FALSE;
         }
      }
      else
      {
         if (!PreCreateWindow(cs))
         {
            PostNcDestroy();
            return FALSE;
         }
      }

      if(cs.hwndParent == NULL)
      {
         cs.style &= ~WS_CHILD;
      }

      AfxHookWindowCreate(this);
      HWND hWnd = ::CreateWindowEx(cs.dwExStyle, cs.lpszClass,
            cs.lpszName, cs.style, cs.x, cs.y, cs.cx, cs.cy,
            cs.hwndParent, cs.hMenu, cs.hInstance, cs.lpCreateParams);

   #ifdef _DEBUG
      if (hWnd == NULL)
      {
         DWORD dwLastError = GetLastError();
         string strLastError = FormatMessageFromSystem(dwLastError);
         string strMessage;
         strMessage.Format("%s\n\nSystem Error Code: %d", strLastError, dwLastError);

         TRACE(::radix::trace::category_AppMsg, 0, "Warning: Window creation failed: GetLastError returned:\n");
         TRACE(::radix::trace::category_AppMsg, 0, "%s\n", strMessage);
         try
         {
            if(dwLastError == 0x0000057e)
            {
               System.simple_message_box(NULL, "Cannot create a top-level child window.");
            }
            else
            {
               System.simple_message_box(NULL, strMessage);
            }
         }
         catch(...)
         {
         }
      }
   #endif

      if (!AfxUnhookWindowCreate())
         PostNcDestroy();        // cleanup if CreateWindowEx fails too soon

      if (hWnd == NULL)
         return FALSE;
      WNDCLASS wndcls;
      if(lpszClassName != NULL &&
         GetClassInfo(System.m_hInstance, lpszClassName, &wndcls) &&
         wndcls.hIcon != NULL)
      {
         m_pguie->set_icon(new ::visual::icon(wndcls.hIcon), false);
         m_pguie->set_icon(new ::visual::icon(wndcls.hIcon), true);
      }
//      HWND hwndHandle = get_handle();
      if(hWnd != get_handle())
      {
         ASSERT(FALSE); // should have been set in send msg hook
      }
      return TRUE;
   }

   // for child windows
   BOOL window::PreCreateWindow(CREATESTRUCT& cs)
   {
      if (cs.lpszClass == NULL)
      {
         // make sure the default window class is registered
         VERIFY(AfxEndDeferRegisterClass(AFX_WND_REG, &cs.lpszClass));

         // no WNDCLASS provided - use child window default
         ASSERT(cs.style & WS_CHILD);
      }
      return TRUE;
   }

   BOOL window::create(const char * lpszClassName,
      const char * lpszWindowName, DWORD dwStyle,
      const RECT& rect,
      ::user::interaction* pParentWnd, id id,
      ::ca::create_context* pContext)
   {
      // can't use for desktop or pop-up windows (use CreateEx instead)
      ASSERT(pParentWnd != NULL);
      ASSERT((dwStyle & WS_POPUP) == 0);

      return CreateEx(0, lpszClassName, lpszWindowName,
         dwStyle | WS_CHILD,
         rect.left, rect.top,
         rect.right - rect.left, rect.bottom - rect.top,
         pParentWnd->_get_handle(), id, (LPVOID)pContext);
   }

   bool window::create_message_window(const char * pszName, ::ca::window_callback * pcallback)
   {
      m_pcallback = pcallback;
      if(IsWindow())
      {
         SetWindowText(pszName);
      }
      else
      {
         string strName = "ca2::fontopus::message_wnd::winservice_1";
         if(!create(NULL, pszName, 0, rect(0, 0, 0, 0), System.window_from_os_data(HWND_MESSAGE), NULL))
         {
            return false;
         }
      }
      return true;
   }


   window::~window()
   {

      if(m_papp != NULL && m_papp->m_psystem != NULL && Sys(m_papp).m_pwindowmap != NULL)
      {
         Sys(m_papp).m_pwindowmap->m_map.remove_key((INT_PTR) get_handle());
      }

      single_lock sl(m_pthread == NULL ? NULL : &m_pthread->m_mutex, TRUE);
      if(m_pfont != NULL)
      {
         delete m_pfont;
      }
      sl.unlock();
      if (get_handle() != NULL)
      {
         TRACE(::radix::trace::category_AppMsg, 0, "Warning: calling DestroyWindow in window::~window; "
            "OnDestroy or PostNcDestroy in derived class will not be called.\n");
         m_pcallback = NULL;
         DestroyWindow();
      }
   }

   void window::install_message_handling(::gen::message::dispatch * pinterface)
   {
      //m_pbuffer->InstallMessageHandling(pinterface);
      IGUI_WIN_MSG_LINK(WM_DESTROY           , pinterface, this, &window::_001OnDestroy);
      IGUI_WIN_MSG_LINK(WM_NCDESTROY         , pinterface, this, &window::_001OnNcDestroy);
      IGUI_WIN_MSG_LINK(WM_PAINT             , pinterface, this, &window::_001OnPaint);
      IGUI_WIN_MSG_LINK(WM_PRINT             , pinterface, this, &window::_001OnPrint);
      if(m_pguie != NULL && m_pguie != this)
      {
         m_pguie->install_message_handling(pinterface);
      }
      IGUI_WIN_MSG_LINK(WM_CAPTURECHANGED    , pinterface, this, &window::_001OnCaptureChanged);
      IGUI_WIN_MSG_LINK(WM_CREATE            , pinterface, this, &window::_001OnCreate);
      IGUI_WIN_MSG_LINK(WM_SETCURSOR         , pinterface, this, &window::_001OnSetCursor);
      IGUI_WIN_MSG_LINK(WM_ERASEBKGND        , pinterface, this, &window::_001OnEraseBkgnd);
      IGUI_WIN_MSG_LINK(WM_MOVE              , pinterface, this, &window::_001OnMove);
      IGUI_WIN_MSG_LINK(WM_SIZE              , pinterface, this, &window::_001OnSize);
      IGUI_WIN_MSG_LINK(WM_SHOWWINDOW        , pinterface, this, &window::_001OnShowWindow);
      IGUI_WIN_MSG_LINK(ca2m_PRODEVIAN_SYNCH , pinterface, this, &window::_001OnProdevianSynch);
   }

   void window::_001OnMove(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      if(!m_bRectOk && !(GetExStyle() & WS_EX_LAYERED))
      {
         class rect rectWindow;
         ::GetWindowRect(get_handle(), rectWindow);
         m_pguie->m_rectParentClient = rectWindow;
         m_rectParentClient = rectWindow;
      }
   }

   void window::_001OnSize(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);


      if(!m_bRectOk && !(GetExStyle() & WS_EX_LAYERED))
      {
         class rect rectWindow;
         ::GetWindowRect(get_handle(), rectWindow);
         m_pguie->m_rectParentClient = rectWindow;
         m_rectParentClient = rectWindow;
      }

/*      if(m_spdibMultAlphaWork.is_null())
      {
         m_spdibMultAlphaWork.create(get_app());
      }

      if(m_spdib.is_null())
      {
         m_spdib.create(get_app());
      }

      if(m_spdib.is_set() && m_rectParentClient.area() > 0)
      {
         m_spdib->create(m_rectParentClient.size());
      }*/

      m_pguie->layout();



   }

   void window::_001OnShowWindow(gen::signal_object * pobj)
   {
      SCAST_PTR(::gen::message::show_window, pshowwindow, pobj);
      m_bVisible = pshowwindow->m_bShow != FALSE;
      if(m_pguie != NULL && m_pguie != this)
         m_pguie->m_bVisible = m_bVisible;
   }

   void window::_001OnDestroy(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      Default();
      ::win::window_draw * pdraw = dynamic_cast < ::win::window_draw * > (System.get_twf());
      if(pdraw != NULL)
      {
         pdraw->m_eventFree.wait();
         pdraw->m_wndpaOut.remove(this);
         pdraw->m_wndpaOut.remove(m_pguie);
      }
   }

   void window::_001OnCaptureChanged(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      m_pguieCapture = NULL;
   }

   // WM_NCDESTROY is the absolute LAST message sent.
   void window::_001OnNcDestroy(gen::signal_object * pobj)
   {
      single_lock sl(m_pthread == NULL ? NULL : &m_pthread->m_mutex, TRUE);
      pobj->m_bRet = true;
      // cleanup main and active windows
      ::ca::thread* pThread = System.GetThread();
      if (pThread != NULL)
      {
         if (pThread->GetMainWnd() == this)
         {
            if (!afxContextIsDLL)
            {
               // shut down current thread if possible
               if (pThread != &System)
                  AfxPostQuitMessage(0);
            }
            pThread->SetMainWnd(NULL);
         }
         if (pThread->get_active_ui() == this)
            pThread->set_active_ui(NULL);
      }

      // cleanup tooltip support
      if(m_pguie != NULL)
      {
         if (m_pguie->m_nFlags & WF_TOOLTIPS)
         {
         }
      }

      // call default, unsubclass, and detach from the ::collection::map
      WNDPROC pfnWndProc = WNDPROC(GetWindowLongPtr(get_handle(), GWLP_WNDPROC));
      Default();
      if (WNDPROC(GetWindowLongPtr(get_handle(), GWLP_WNDPROC)) == pfnWndProc)
      {
         WNDPROC pfnSuper = *GetSuperWndProcAddr();
         if (pfnSuper != NULL)
            SetWindowLongPtr(get_handle(), GWLP_WNDPROC, reinterpret_cast<INT_PTR>(pfnSuper));
      }
      Detach();
      ASSERT(get_handle() == NULL);
      m_pfnDispatchWindowProc = &window::_start_user_message_handler;
      // call special post-cleanup routine
      PostNcDestroy();
      if(m_pguie != NULL && m_pguie != this)
      {
         m_pguie->PostNcDestroy();
      }
   }

   void window::PostNcDestroy()
   {
      set_handle(NULL);
      // default to nothing
   }

   void window::on_final_release()
   {
      if (get_handle() != NULL)
         DestroyWindow();    // will call PostNcDestroy
      else
         PostNcDestroy();
   }

   #ifdef _DEBUG
   void window::assert_valid() const
   {
      if (get_handle() == NULL)
         return;     // null (unattached) windows are valid

      // check for special wnd??? values
      ASSERT(HWND_TOP == NULL);       // same as desktop
      if (get_handle() == HWND_BOTTOM)
      {
      }
      else if (get_handle() == HWND_TOPMOST)
      {
      }
      else if (get_handle() == HWND_NOTOPMOST)
      {
      }
      else
      {
         // should be a normal window
         ASSERT(::IsWindow(get_handle()));

         // should also be in the permanent or temporary handle ::collection::map
         hwnd_map * pMap = afxMapHWND();
         if(pMap == NULL) // inside thread not having windows
            return; // let go
         ASSERT(pMap != NULL);

//         ::radix::object* p=NULL;
         /*if(pMap)
         {
            ASSERT( (p = pMap->lookup_permanent(get_handle())) != NULL ||
                  (p = pMap->lookup_temporary(get_handle())) != NULL);
         }*/
         
         //ASSERT(dynamic_cast < ::ca::window * > (p) == this);   // must be us

         // Note: if either of the above asserts fire and you are
         // writing a multithreaded application, it is likely that
         // you have passed a C++ object from one thread to another
         // and have used that object in a way that was not intended.
         // (only simple inline wrapper functions should be used)
         //
         // In general, window objects should be passed by HWND from
         // one thread to another.  The receiving thread can wrap
         // the HWND with a window object by using ::win::window::from_handle.
         //
         // It is dangerous to pass C++ objects from one thread to
         // another, unless the objects are designed to be used in
         // such a manner.
      }
   }


   void window::dump(dump_context & dumpcontext) const
   {
      ::radix::object::dump(dumpcontext);

      dumpcontext << "\nm_hWnd = " << (void *)get_handle();

      if (get_handle() == NULL || get_handle() == HWND_BOTTOM ||
         get_handle() == HWND_TOPMOST || get_handle() == HWND_NOTOPMOST)
      {
         // not a normal window - nothing more to dump
         return;
      }

      if (!::IsWindow(get_handle()))
      {
         // not a valid window
         dumpcontext << " (illegal HWND)";
         return; // don't do anything more
      }

      ::ca::window * pWnd = ::win::window::FromHandlePermanent(get_handle());
      if (pWnd != this)
         dumpcontext << " (Detached or temporary window)";
      else
         dumpcontext << " (permanent window)";

      // dump out window specific statistics
      char szBuf [64];
      if (!const_cast < window * > (this)->SendMessage(WM_QUERYAFXWNDPROC, 0, 0) && pWnd == this)
         ((::ca::window *) this)->GetWindowText(szBuf, _countof(szBuf));
      else
         ::DefWindowProc(get_handle(), WM_GETTEXT, _countof(szBuf), (LPARAM)&szBuf[0]);
      dumpcontext << "\ncaption = \"" << szBuf << "\"";

      ::GetClassName(get_handle(), szBuf, _countof(szBuf));
      dumpcontext << "\nclass name = \"" << szBuf << "\"";

      rect rect;
      ((::ca::window *) this)->GetWindowRect(&rect);
      dumpcontext << "\nrect = " << rect;
      dumpcontext << "\nparent ::ca::window * = " << (void *)((::ca::window *) this)->GetParent();

      dumpcontext << "\nstyle = " << (void *)(DWORD_PTR)::GetWindowLong(get_handle(), GWL_STYLE);
      if (::GetWindowLong(get_handle(), GWL_STYLE) & WS_CHILD)
         dumpcontext << "\nid = " << _AfxGetDlgCtrlID(get_handle());

      dumpcontext << "\n";
   }
   #endif

   BOOL window::DestroyWindow()
   {
      single_lock sl(m_pthread == NULL ? NULL : &m_pthread->m_mutex, TRUE);
      ::ca::window * pWnd;
      hwnd_map * pMap;
      HWND hWndOrig;
      BOOL bResult;

      if ((get_handle() == NULL) )
         return FALSE;

      bResult = FALSE;
      pMap = NULL;
      pWnd = NULL;
      hWndOrig = NULL;
      if (get_handle() != NULL)
      {
         pMap = afxMapHWND();
         if(pMap != NULL)
         {
            pWnd = dynamic_cast < ::ca::window * > (pMap->lookup_permanent(get_handle()));
      #ifdef _DEBUG
            hWndOrig = get_handle();
      #endif
         }
      }
      sl.unlock();
      if (get_handle() != NULL)
         bResult = ::DestroyWindow(get_handle());
      sl.lock();
      if (hWndOrig != NULL)
      {
         // Note that 'this' may have been deleted at this point,
         //  (but only if pWnd != NULL)
         if (pWnd != NULL)
         {
            // Should have been detached by OnNcDestroy
   #ifdef _DEBUG
            ::ca::window * pWndPermanent = dynamic_cast < ::ca::window * > (pMap->lookup_permanent(hWndOrig));;
            ASSERT(pWndPermanent == NULL);
            // It is important to call base class, including ca2 core
            // base classes implementation of install_message_handling
            // inside derived class install_message_handling
   #endif
         }
         else
         {
   #ifdef _DEBUG
            ASSERT(get_handle() == hWndOrig);
   #endif
         // Detach after DestroyWindow called just in case
            Detach();
         }
      }

      return bResult;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Default window implementation


   LRESULT window::DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
   {
      if (m_pfnSuper != NULL)
         return ::CallWindowProc(m_pfnSuper, get_handle(), nMsg, wParam, lParam);

      WNDPROC pfnWndProc;
      if ((pfnWndProc = *GetSuperWndProcAddr()) == NULL)
         return ::DefWindowProc(get_handle(), nMsg, wParam, lParam);
      else
         return ::CallWindowProc(pfnWndProc, get_handle(), nMsg, wParam, lParam);
   }

   WNDPROC* window::GetSuperWndProcAddr()
   {
      // Note: it is no longer necessary to override GetSuperWndProcAddr
      //  for each control class with a different WNDCLASS.
      //  This implementation now uses instance data, such that the previous
      //  WNDPROC can be anything.

      return &m_pfnSuper;
   }

   void window::pre_translate_message(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      // no default processing
   }


   void window::GetWindowText(string & rString)
   {
      /*ASSERT(::IsWindow(get_handle()));

         int nLen = ::GetWindowTextLength(get_handle());
         ::GetWindowText(get_handle(), rString.GetBufferSetLength(nLen), nLen+1);
         rString.ReleaseBuffer();*/
      rString = m_strWindowText;

   }

   int window::GetDlgItemText(int nID, string & rString) const
   {
      ASSERT(::IsWindow(get_handle()));
      rString = "";    // is_empty without deallocating

         HWND hWnd = ::GetDlgItem(get_handle(), nID);
         if (hWnd != NULL)
         {
            int nLen = ::GetWindowTextLength(hWnd);
            ::GetWindowText(hWnd, rString.GetBufferSetLength(nLen), nLen+1);
            rString.ReleaseBuffer();
         }

      return (int)rString.get_length();
   }

   BOOL window::GetWindowPlacement(WINDOWPLACEMENT* lpwndpl) 
   {
      ASSERT(::IsWindow(get_handle()));
      lpwndpl->length = sizeof(WINDOWPLACEMENT);
      return ::GetWindowPlacement(get_handle(), lpwndpl);
   }

   BOOL window::SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl)
   {
      ASSERT(::IsWindow(get_handle()));
      ((WINDOWPLACEMENT*)lpwndpl)->length = sizeof(WINDOWPLACEMENT);
      return ::SetWindowPlacement(get_handle(), lpwndpl);
   }

   /////////////////////////////////////////////////////////////////////////////
   // window will delegate owner draw messages to self drawing controls

   // Drawing: for all 4 control types
   void window::OnDrawItem(int /*nIDCtl*/, LPDRAWITEMSTRUCT lpDrawItemStruct)
   {

      // reflect notification to child window control
      if (ReflectLastMsg(lpDrawItemStruct->hwndItem))
         return;     // eat it

      // not handled - do default
      Default();
   }

   // Drawing: for all 4 control types
   int window::OnCompareItem(int /*nIDCtl*/, LPCOMPAREITEMSTRUCT lpCompareItemStruct)
   {
      // reflect notification to child window control
      LRESULT lResult;
      if (ReflectLastMsg(lpCompareItemStruct->hwndItem, &lResult))
         return (int)lResult;        // eat it

      // not handled - do default
      return (int)Default();
   }

   void window::OnDeleteItem(int /*nIDCtl*/, LPDELETEITEMSTRUCT lpDeleteItemStruct)
   {
      // reflect notification to child window control
      if (ReflectLastMsg(lpDeleteItemStruct->hwndItem))
         return;     // eat it
      // not handled - do default
      Default();
   }

   BOOL window::_EnableToolTips(BOOL bEnable, UINT nFlag)
   {
      UNREFERENCED_PARAMETER(bEnable);
      UNREFERENCED_PARAMETER(nFlag);
      return FALSE;
   }


   // Measure item implementation relies on unique control/menu IDs
   void window::OnMeasureItem(int /*nIDCtl*/, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
   {
      if (lpMeasureItemStruct->CtlType == ODT_MENU)
      {
         ASSERT(lpMeasureItemStruct->CtlID == 0);
      }
      else
      {
         ::user::interaction * pChild = GetDescendantWindow(lpMeasureItemStruct->CtlID);
         if (pChild != NULL && pChild->SendChildNotifyLastMsg())
            return;     // eaten by child
      }
      // not handled - do default
      Default();
   }
   
   BOOL window::GetWindowInfo(PWINDOWINFO pwi) const
   {
      ASSERT(::IsWindow((HWND)get_os_data())); 
      return ::GetWindowInfo((HWND)get_os_data(), pwi); 
   }

    ::ca::window * window::GetAncestor(UINT gaFlags) const
   { ASSERT(::IsWindow((HWND)get_os_data())); return  ::win::window::from_handle(::GetAncestor((HWND)get_os_data(), gaFlags)); }



    BOOL window::GetScrollBarInfo(LONG idObject, PSCROLLBARINFO psbi) const
   {
      ASSERT(::IsWindow((HWND)get_os_data())); 
      ASSERT(psbi != NULL);
      return ::GetScrollBarInfo((HWND)get_os_data(), idObject, psbi);
   }

    BOOL window::GetTitleBarInfo(PTITLEBARINFO pti) const
   {
      ASSERT(::IsWindow((HWND)get_os_data())); 
      ASSERT(pti != NULL);
      return ::GetTitleBarInfo((HWND)get_os_data(), pti);
   }

    BOOL window::AnimateWindow(DWORD dwTime, DWORD dwFlags) 
   {
      ASSERT(::IsWindow((HWND)get_os_data())); 
      return ::AnimateWindow((HWND)get_os_data(), dwTime, dwFlags);
   }

    BOOL window::FlashWindowEx(DWORD dwFlags, UINT  uCount, DWORD dwTimeout)
   {
      ASSERT(::IsWindow((HWND)get_os_data())); 
      FLASHWINFO fwi;
      fwi.cbSize = sizeof(fwi);
      fwi.hwnd = (HWND)get_os_data();
      fwi.dwFlags = dwFlags;
      fwi.uCount = uCount;
      fwi.dwTimeout = dwTimeout;

      return ::FlashWindowEx(&fwi);
   }



    BOOL window::SetLayeredWindowAttributes(COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
   {
      ASSERT(::IsWindow((HWND)get_os_data())); 
      return ::SetLayeredWindowAttributes((HWND)get_os_data(), crKey, bAlpha, dwFlags);
   }

    BOOL window::UpdateLayeredWindow(::ca::graphics * pDCDst, POINT *pptDst, SIZE *psize, 
      ::ca::graphics * pDCSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags)
   {
      ASSERT(::IsWindow((HWND)get_os_data())); 
      return ::UpdateLayeredWindow((HWND)get_os_data(), WIN_HDC(pDCDst), pptDst, psize,
         WIN_HDC(pDCSrc), pptSrc, crKey, pblend, dwFlags);
   }


    BOOL window::GetLayeredWindowAttributes(COLORREF *pcrKey, BYTE *pbAlpha, DWORD *pdwFlags) const
   {
      ASSERT(::IsWindow((HWND)get_os_data())); 
      return ::GetLayeredWindowAttributes((HWND)get_os_data(), pcrKey, pbAlpha, pdwFlags);
   }

    BOOL window::PrintWindow(::ca::graphics * pgraphics, UINT nFlags) const
   {
      ASSERT(::IsWindow((HWND)get_os_data())); 
      return ::PrintWindow((HWND)get_os_data(), (HDC)(dynamic_cast<::win::graphics * >(pgraphics))->get_os_data(), nFlags);
   }


   LRESULT window::OnNTCtlColor(WPARAM wParam, LPARAM lParam)
   {
      // fill in special struct for compatiblity with 16-bit WM_CTLCOLOR
      AFX_CTLCOLOR ctl;
      ctl.hDC = (HDC)wParam;
      ctl.hWnd = (HWND)lParam;
      _AFX_THREAD_STATE* pThreadState = _afxThreadState.get_data();
      ctl.nCtlType = pThreadState->m_lastSentMsg.message - WM_CTLCOLORMSGBOX;
      //ASSERT(ctl.nCtlType >= CTLCOLOR_MSGBOX);
      ASSERT(ctl.nCtlType <= CTLCOLOR_STATIC);

      // Note: We call the virtual message_handler for this window directly,
      //  instead of calling AfxCallWindowProc, so that Default()
      //  will still work (it will call the Default window proc with
      //  the original Win32 WM_CTLCOLOR message).
      /*
      return message_handler(WM_CTLCOLOR, 0, (LPARAM)&ctl);*/
      return 0;
   }

   /////////////////////////////////////////////////////////////////////////////
   // window extensions for help support

   void window::WinHelp(DWORD_PTR dwData, UINT nCmd)
   {
      UNREFERENCED_PARAMETER(dwData);
      UNREFERENCED_PARAMETER(nCmd);
      throw not_implemented_exception();

/*      application* pApp = &System;
      ASSERT_VALID(pApp);
      ASSERT(pApp->m_pszHelpFilePath != NULL);
      ASSERT(pApp->m_eHelpType == afxWinHelp);

      wait_cursor wait(get_app());

      PrepareForHelp();

      // need to use top level parent (for the case where get_handle() is in DLL)
      ::user::interaction * pWnd = EnsureTopLevelParent();

      TRACE(::radix::trace::category_AppMsg, 0, "WinHelp: pszHelpFile = '%s', dwData: $%lx, fuCommand: %d.\n", pApp->m_pszHelpFilePath, dwData, nCmd);

      // finally, run the Windows Help engine
   /* trans   if (!::WinHelp(WIN_WINDOW(pWnd)->get_handle(), pApp->m_pszHelpFilePath, nCmd, dwData))
      {
         // linux System.simple_message_box(AFX_IDP_FAILED_TO_LAUNCH_HELP);
         System.simple_message_box("Failed to launch help");
      }*/
   }

   //void window::HtmlHelp(DWORD_PTR dwData, UINT nCmd)
   //{
     // throw not_implemented_exception();
      /*
      application* pApp = &System;
      ASSERT_VALID(pApp);
      ASSERT(pApp->m_pszHelpFilePath != NULL);
      // to call HtmlHelp the m_fUseHtmlHelp must be set in
      // the application's constructor
      ASSERT(pApp->m_eHelpType == afxHTMLHelp);

      wait_cursor wait(get_app());

      PrepareForHelp();

      // need to use top level parent (for the case where get_handle() is in DLL)
      ::user::interaction * pWnd = EnsureTopLevelParent();

      TRACE(::radix::trace::category_AppMsg, 0, "HtmlHelp: pszHelpFile = '%s', dwData: $%lx, fuCommand: %d.\n", pApp->m_pszHelpFilePath, dwData, nCmd);

      // run the HTML Help engine
   /* trans   if (!AfxHtmlHelp(WIN_WINDOW(pWnd)->get_handle(), pApp->m_pszHelpFilePath, nCmd, dwData))
      {
         // linux System.simple_message_box(AFX_IDP_FAILED_TO_LAUNCH_HELP);
         System.simple_message_box("Failed to launch help");
      }*/
   //}

   void window::PrepareForHelp()
   {
      if (IsFrameWnd())
      {
         // frame_window windows should be allowed to exit help mode first
         frame_window* pFrameWnd = dynamic_cast < frame_window * >(this);
         pFrameWnd->ExitHelpMode();
      }

      // cancel any tracking modes
      SendMessage(WM_CANCELMODE);
      SendMessageToDescendants(WM_CANCELMODE, 0, 0, TRUE, TRUE);

      // need to use top level parent (for the case where get_handle() is in DLL)
      ::user::interaction * pWnd = EnsureTopLevelParent();
      WIN_WINDOW(pWnd)->SendMessage(WM_CANCELMODE);
      WIN_WINDOW(pWnd)->SendMessageToDescendants(WM_CANCELMODE, 0, 0, TRUE, TRUE);

      // attempt to cancel capture
      HWND hWndCapture = ::GetCapture();
      if (hWndCapture != NULL)
         ::SendMessage(hWndCapture, WM_CANCELMODE, 0, 0);
   }

   // for dll builds we just delay load it
   #ifndef _ApplicationFrameworkDLL
   _AFX_HTMLHELP_STATE::~_AFX_HTMLHELP_STATE()
   {
      if (m_hInstHtmlHelp)
         FreeLibrary(m_hInstHtmlHelp);
   }
   #endif

   /*HWND WINAPI AfxHtmlHelp(HWND hWnd, const char * szHelpFilePath, UINT nCmd, DWORD_PTR dwData)
   {
   // for dll builds we just delay load it
   #ifndef _ApplicationFrameworkDLL
   #ifdef _UNICODE
      #define _HTMLHELP_ENTRY "HtmlHelpW"
   #else
      #define _HTMLHELP_ENTRY "HtmlHelpA"
   #endif

      AfxLockGlobals(CRIT_DYNDLLLOAD);
      // check if the HtmlHelp library was loaded
      _AFX_HTMLHELP_STATE* pState = _afxHtmlHelpState.get_data();
      if (!pState->m_pfnHtmlHelp)
      {
         // load the library
         ASSERT(!pState->m_hInstHtmlHelp);
         pState->m_hInstHtmlHelp = LoadLibraryA("hhctrl.ocx");
         if (!pState->m_hInstHtmlHelp)
            return NULL;
         pState->m_pfnHtmlHelp = (HTMLHELPPROC *) GetProcAddress(pState->m_hInstHtmlHelp, _HTMLHELP_ENTRY);
         if (!pState->m_pfnHtmlHelp)
         {
            FreeLibrary(pState->m_hInstHtmlHelp);
            pState->m_hInstHtmlHelp = NULL;
            return NULL;
         }
      }
      AfxUnlockGlobals(CRIT_DYNDLLLOAD);
      // now call it
      return (*(pState->m_pfnHtmlHelp))(hWnd, szHelpFilePath, nCmd, dwData);
   #else
      return ::HtmlHelp(hWnd, szHelpFilePath, nCmd, dwData);
   #endif
   }*/

   void window::WinHelpInternal(DWORD_PTR dwData, UINT nCmd)
   {
      UNREFERENCED_PARAMETER(dwData);
      UNREFERENCED_PARAMETER(nCmd);
      throw not_implemented_exception();
      /*
      application* pApp = &System;
      ASSERT_VALID(pApp);
      if (pApp->m_eHelpType == afxHTMLHelp)
      {
         // translate from WinHelp commands and data to to HtmlHelp
         ASSERT((nCmd == HELP_CONTEXT) || (nCmd == HELP_CONTENTS) || (nCmd == HELP_FINDER));
         if (nCmd == HELP_CONTEXT)
            nCmd = HH_HELP_CONTEXT;
         else if (nCmd == HELP_CONTENTS)
            nCmd = HH_DISPLAY_TOC;
         else if (nCmd == HELP_FINDER)
            nCmd = HH_HELP_FINDER;
         HtmlHelp(dwData, nCmd);
      }
      else
         WinHelp(dwData, nCmd);*/
   }



   bool window::_001OnCmdMsg(BaseCmdMsg * pcmdmsg)
   {
      if(command_target_interface::_001OnCmdMsg(pcmdmsg))
         return TRUE;

//      BOOL b;

      //if(_iguimessageDispatchCommandMessage(pcommand, b))
        // return b;

      command_target * pcmdtarget = dynamic_cast < command_target * > (this);
      return pcmdtarget->command_target::_001OnCmdMsg(pcmdmsg);
   }


   bool window::BaseOnControlEvent(::user::control_event * pevent)
   {
      UNREFERENCED_PARAMETER(pevent);
      return false;
   }

   void window::_002OnDraw(::ca::graphics * pdc)
   {

      ::CallWindowProc(*GetSuperWndProcAddr(), get_handle(), WM_PRINT, (WPARAM)((dynamic_cast<::win::graphics * >(pdc))->get_os_data()), (LPARAM)(PRF_CHILDREN | PRF_CLIENT));

   }

   /////////////////////////////////////////////////////////////////////////////
   // main message_handler implementation

   void window::message_handler(gen::signal_object * pobj)
   {
      SCAST_PTR(::gen::message::base, pbase, pobj);
      if(m_pcallback != NULL)
      {
         m_pcallback->message_window_message_handler(pobj);
         if(pobj->m_bRet)
            return;
      }
      if(pbase->m_uiMessage == WM_TIMER)
      {
         System.GetThread()->step_timer();
      }
      else if(pbase->m_uiMessage == WM_LBUTTONDOWN)
      {
         g_pwndLastLButtonDown = this;
      }
      else if(pbase->m_uiMessage == CA2M_BERGEDGE)
      {
         if(pbase->m_wparam == BERGEDGE_GETAPP)
         {
            ::ca::application ** ppapp= (::ca::application **) pbase->m_lparam;
            *ppapp = get_app();
            pbase->m_bRet = true;
            return;
         }
      }
      pbase->set_lresult(0);

      if(pbase->m_uiMessage == WM_MOUSELEAVE)
      {
         m_bMouseHover = false;
         for(int i = 0; i < m_guieptraMouseHover.get_size(); i++)
         {
            if(m_guieptraMouseHover[i] == this 
            || m_guieptraMouseHover[i]->m_pimpl == this 
            || m_guieptraMouseHover[i]->m_pguie == this)
               continue;
            m_guieptraMouseHover[i]->SendMessage(WM_MOUSELEAVE);
         }
         m_guieptraMouseHover.remove_all();
      }

      if(pbase->m_uiMessage == WM_LBUTTONDOWN ||
         pbase->m_uiMessage == WM_LBUTTONUP ||
         pbase->m_uiMessage == WM_MBUTTONDOWN ||
         pbase->m_uiMessage == WM_MBUTTONUP ||
         pbase->m_uiMessage == WM_RBUTTONDOWN ||
         pbase->m_uiMessage == WM_RBUTTONUP ||
         pbase->m_uiMessage == WM_MOUSEMOVE)
      {
         // user presence status activity reporting
         if(pbase->m_uiMessage == WM_LBUTTONDOWN 
         || pbase->m_uiMessage == WM_RBUTTONDOWN 
         || pbase->m_uiMessage == WM_MBUTTONDOWN)
         {
            if(Application.m_puser != NULL)
            {
               if(&ApplicationUser != NULL)
               {
                  if(ApplicationUser.m_ppresence != NULL)
                  {
                     try
                     {
                        ApplicationUser.m_ppresence->report_activity();
                     }
                     catch(...)
                     {
                     }
                  }
               }
            }
         }
         ::gen::message::mouse * pmouse = (::gen::message::mouse *) pbase;

         Application.m_ptCursor = pmouse->m_pt;
         if(m_psession != NULL)
         {
            Session.m_ptCursor = pmouse->m_pt;
         }
         if(m_pguie != NULL && m_pguie != this && m_pguie->m_psession != NULL && m_pguie->m_psession != m_psession)
         {
            Sess(m_pguie->m_psession).m_ptCursor = pmouse->m_pt;
         }

         ::plane::session * psession = NULL;
         if(m_papp->is_system())
         {
            psession = System.query_session(0);
            if(psession != NULL && psession->m_bSessionSynchronizedCursor)
            {
               psession->m_ptCursor = pmouse->m_pt;
            }
         }

         if(m_bTranslateMouseMessageCursor && !pmouse->m_bTranslated)
         {
            pmouse->m_bTranslated = true;
            rect64 rectWindow;
            if(m_bOSNativeMouseMessagePosition)
            {
               class rect rectWindow32;
               ::GetWindowRect(get_handle(), &rectWindow32);
               ::copy(rectWindow, rectWindow32);
            }
            else
            {
               m_pguie->GetWindowRect(rectWindow);   
            }
            if(System.get_monitor_count() > 0)
            {
               rect rcMonitor;
               System.get_monitor_rect(0, &rcMonitor);
               if(rectWindow.left >= rcMonitor.left)
                  pmouse->m_pt.x += (LONG) rectWindow.left;
               if(rectWindow.top >= rcMonitor.top)
                  pmouse->m_pt.y += (LONG) rectWindow.top;
            }
            else
            {
               if(rectWindow.left >= 0)
                  pmouse->m_pt.x += (LONG) rectWindow.left;
               if(rectWindow.top >= 0)
                  pmouse->m_pt.y += (LONG) rectWindow.top;
            }
         }

         if(pbase->m_uiMessage == WM_MOUSEMOVE)
         {
            // We are at the message_handler procedure.
            // mouse messages originated from message_handler and that are mouse move events should end up with the correct cursor.
            // So the procedure starts by setting to the default cursor,
            // what forces, at the end of message processing, setting the bergedge cursor to the default cursor, if no other
            // handler has set it to another one.
            pmouse->m_ecursor = visual::cursor_default;
         }
   restart_mouse_hover_check:
         for(int i = 0; i < m_guieptraMouseHover.get_size(); i++)
         {
            if(!m_guieptraMouseHover[i]->_001IsPointInside(pmouse->m_pt))
            {
               ::user::interaction * pui = m_guieptraMouseHover[i];
               pui->SendMessage(WM_MOUSELEAVE);
               m_guieptraMouseHover.remove(pui);
               goto restart_mouse_hover_check;
            }
         }
         if(!m_bMouseHover)
         {
            string strApp = System.m_strAppName;
            if(strApp == "netshareserver")
            {
               m_bMouseHover = true;
               TRACKMOUSEEVENT tme = { sizeof(tme) };
               tme.dwFlags = TME_LEAVE;
               tme.hwndTrack = get_handle();
               //System.TrackMouseEvent(&tme);
            }
            else
            {
               m_bMouseHover = true;
               TRACKMOUSEEVENT tme = { sizeof(tme) };
               tme.dwFlags = TME_LEAVE;
               tme.hwndTrack = get_handle();
               TrackMouseEvent(&tme);
            }
         }
         if(m_pguieCapture != NULL)
         {
            if(m_pguieCapture->m_pimpl != NULL)
            {
               //m_pguieCapture->m_pimpl->SendMessage(pbase);
               try
               {
                  (m_pguieCapture->m_pimpl->*m_pguieCapture->m_pimpl->m_pfnDispatchWindowProc)(dynamic_cast < gen::signal_object * > (pmouse));
                  if(pmouse->get_lresult() != 0)
                     return;
               }
               catch(...)
               {
               }
               return;
            }
            else
            {
               //m_pguieCapture->SendMessage(pbase);
               try
               {
                  (m_pguieCapture->*m_pguieCapture->m_pfnDispatchWindowProc)(dynamic_cast < gen::signal_object * > (pmouse));
                  if(pmouse->get_lresult() != 0)
                     return;
               }
               catch(...)
               {
               }
               return;
            }
         }
         user::HWNDArray hwnda;
         user::LPWndArray wnda;
         wnda = System.frames();
         wnda.get_wnda(hwnda);
         user::WndUtil::SortByZOrder(hwnda);
         for(int i = 0; i < hwnda.get_size(); i++)
         {
            ::user::interaction * pguie = wnda.find_first(hwnda[i]);
            if(pguie != NULL && pguie->m_pguie != NULL)
            {
               pguie->m_pguie->_000OnMouse(pmouse);
               if(pmouse->m_bRet)
                  return;
            }
         }
         pbase->set_lresult(DefWindowProc(pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam));
         return;
      }
      else if(pbase->m_uiMessage == WM_KEYDOWN ||
         pbase->m_uiMessage == WM_KEYUP ||
         pbase->m_uiMessage == WM_CHAR)
      {

         if(pbase->m_uiMessage == WM_KEYDOWN)
         {
            try
            {
               Application.set_key_pressed(pbase->m_wparam, true);
            }
            catch(...)
            {
            }
         }
         else if(pbase->m_uiMessage == WM_KEYUP)
         {
            try
            {
               Application.set_key_pressed(pbase->m_wparam, false);
            }
            catch(...)
            {
            }
         }

         ::gen::message::key * pkey = (::gen::message::key *) pbase;
         ::user::interaction * puiFocus = dynamic_cast < ::user::interaction * > (Application.get_keyboard_focus());
         if(puiFocus != NULL 
         && puiFocus->IsWindow()
         && puiFocus->GetTopLevelParent() != NULL)
         {
            puiFocus->SendMessage(pkey);
            if(pbase->m_bRet)
               return;
         }
         /*
         else if(!pkey->m_bRet)
         {
            if(m_pguie != this && m_pguie != NULL)
            {
               m_pguie->_000OnKey(pkey);
               if(pbase->m_bRet)
                  return;
            }
            else
            {
               _000OnKey(pkey);
               if(pbase->m_bRet)
                  return;
            }
         }
         pbase->set_lresult(DefWindowProc(pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam));*/
         return;
      }
      if(pbase->m_uiMessage == ::gen::message_event)
      {
         if(m_pguie != this && m_pguie != NULL)
         {
            m_pguie->BaseOnControlEvent((::user::control_event *) pbase->m_lparam);
         }
         else
         {
            BaseOnControlEvent((::user::control_event *) pbase->m_lparam);
         }
         return;
      }
      (this->*m_pfnDispatchWindowProc)(pobj);
      if(pobj->m_bRet)
         return;
/*
      if(m_pguie != NULL && m_pguie != this)
      {
         m_pguie->_user_message_handler(pobj);
         if(pobj->m_bRet)
            return;
      }
*/
      pbase->set_lresult(DefWindowProc(pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam));
   }

   /*
   BOOL window::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
   {
      LRESULT lResult = 0;
      union MessageMapFunctions mmf;
      mmf.pfn = 0;
      CInternalGlobalLock winMsgLock;
      // special case for commands
      if (message == WM_COMMAND)
      {
         if (OnCommand(wParam, lParam))
         {
            lResult = 1;
            goto LReturnTrue;
         }
         return FALSE;
      }

      // special case for notifies
      if (message == WM_NOTIFY)
      {
         NMHDR* pNMHDR = (NMHDR*)lParam;
         if (pNMHDR->hwndFrom != NULL && OnNotify(wParam, lParam, &lResult))
            goto LReturnTrue;
         return FALSE;
      }

      // special case for activation
      if (message == WM_ACTIVATE)
         _AfxHandleActivate(this, wParam, ::win::window::from_handle((HWND)lParam));

      // special case for set cursor HTERROR
      if (message == WM_SETCURSOR &&
         _AfxHandleSetCursor(this, (short)LOWORD(lParam), HIWORD(lParam)))
      {
         lResult = 1;
         goto LReturnTrue;
      }

      // special case for windows that contain windowless ActiveX controls
      BOOL bHandled;

      bHandled = FALSE;

      const AFX_MSGMAP* pMessageMap; pMessageMap = GetMessageMap();
      UINT iHash; iHash = (LOWORD((DWORD_PTR)pMessageMap) ^ message) & (iHashMax-1);
      winMsgLock.lock(CRIT_WINMSGCACHE);
      AFX_MSG_CACHE* pMsgCache; pMsgCache = &_afxMsgCache[iHash];
      const AFX_MSGMAP_ENTRY* lpEntry;
      if (message == pMsgCache->nMsg && pMessageMap == pMsgCache->pMessageMap)
      {
         // cache hit
         lpEntry = pMsgCache->lpEntry;
         winMsgLock.unlock();
         if (lpEntry == NULL)
            return FALSE;

         // cache hit, and it needs to be handled
         if (message < 0xC000)
            goto LDispatch;
         else
            goto LDispatchRegistered;
      }
      else
      {
         // not in cache, look for it
         pMsgCache->nMsg = message;
         pMsgCache->pMessageMap = pMessageMap;

         for (/* pMessageMap already init'ed */ /*; pMessageMap->pfnGetBaseMap != NULL;
            pMessageMap = (*pMessageMap->pfnGetBaseMap)())
         {
            // Note: catch not so common but fatal mistake!!
            //       // BEGIN_MESSAGE_MAP(CMyWnd, CMyWnd)
            ASSERT(pMessageMap != (*pMessageMap->pfnGetBaseMap)());
            if (message < 0xC000)
            {
               // constant window message
               if ((lpEntry = AfxFindMessageEntry(pMessageMap->lpEntries,
                  message, 0, 0)) != NULL)
               {
                  pMsgCache->lpEntry = lpEntry;
                  winMsgLock.unlock();
                  goto LDispatch;
               }
            }
            else
            {
               // registered windows message
               lpEntry = pMessageMap->lpEntries;
               while ((lpEntry = AfxFindMessageEntry(lpEntry, 0xC000, 0, 0)) != NULL)
               {
                  UINT* pnID = (UINT*)(lpEntry->nSig);
                  ASSERT(*pnID >= 0xC000 || *pnID == 0);
                     // must be successfully registered
                  if (*pnID == message)
                  {
                     pMsgCache->lpEntry = lpEntry;
                     winMsgLock.unlock();
                     goto LDispatchRegistered;
                  }
                  lpEntry++;      // keep looking past this one
               }
            }
         }

         pMsgCache->lpEntry = NULL;
         winMsgLock.unlock();
         return FALSE;
      }

   LDispatch:
      ASSERT(message < 0xC000);

      mmf.pfn = lpEntry->pfn;

      switch (lpEntry->nSig)
      {
      default:
         ASSERT(FALSE);
         break;
      case AfxSig_l_p:
         {
            point point(lParam);      
            lResult = (this->*mmf.pfn_l_p)(point);
            break;
         }      
      case AfxSig_b_D_v:
         lResult = (this->*mmf.pfn_b_D)(::win::graphics::from_handle(reinterpret_cast<HDC>(wParam)));
         break;

      case AfxSig_b_b_v:
         lResult = (this->*mmf.pfn_b_b)(static_cast<BOOL>(wParam));
         break;

      case AfxSig_b_u_v:
         lResult = (this->*mmf.pfn_b_u)(static_cast<UINT>(wParam));
         break;

      case AfxSig_b_h_v:
         lResult = (this->*mmf.pfn_b_h)(reinterpret_cast<HANDLE>(wParam));
         break;

      case AfxSig_i_u_v:
         lResult = (this->*mmf.pfn_i_u)(static_cast<UINT>(wParam));
         break;

      case AfxSig_C_v_v:
         lResult = reinterpret_cast<LRESULT>((this->*mmf.pfn_C_v)());
         break;

      case AfxSig_v_u_W:
         (this->*mmf.pfn_v_u_W)(static_cast<UINT>(wParam), 
            ::win::window::from_handle(reinterpret_cast<HWND>(lParam)));
         break;

      case AfxSig_u_u_v:
         lResult = (this->*mmf.pfn_u_u)(static_cast<UINT>(wParam));
         break;

      case AfxSig_b_v_v:
         lResult = (this->*mmf.pfn_b_v)();
         break;

      case AfxSig_b_W_uu:
         lResult = (this->*mmf.pfn_b_W_u_u)(::win::window::from_handle(reinterpret_cast<HWND>(wParam)),
            LOWORD(lParam), HIWORD(lParam));
         break;

      case AfxSig_b_W_COPYDATASTRUCT:
         lResult = (this->*mmf.pfn_b_W_COPYDATASTRUCT)(
            ::win::window::from_handle(reinterpret_cast<HWND>(wParam)),
            reinterpret_cast<COPYDATASTRUCT*>(lParam));
         break;

      case AfxSig_b_v_HELPINFO:
         lResult = (this->*mmf.pfn_b_HELPINFO)(reinterpret_cast<LPHELPINFO>(lParam));
         break;

      case AfxSig_CTLCOLOR:
         {
            // special case for OnCtlColor to avoid too many temporary objects
            ASSERT(message == WM_CTLCOLOR);
            AFX_CTLCOLOR* pCtl = reinterpret_cast<AFX_CTLCOLOR*>(lParam);
            ::ca::graphics_sp dcTemp; 
            dcTemp.set_handle1(pCtl->hDC);
            window wndTemp; 
            wndTemp.set_handle(pCtl->hWnd);
            UINT nCtlType = pCtl->nCtlType;
            // if not coming from a permanent window, use stack temporary
            ::ca::window * pWnd = ::win::window::FromHandlePermanent(wndTemp.get_handle());
            if (pWnd == NULL)
            {
               pWnd = &wndTemp;
            }
            HBRUSH hbr = (this->*mmf.pfn_B_D_W_u)(&dcTemp, pWnd, nCtlType);
            // fast detach of temporary objects
            dcTemp.set_handle1(NULL);
            wndTemp.set_handle(NULL);
            lResult = reinterpret_cast<LRESULT>(hbr);
         }
         break;

      case AfxSig_CTLCOLOR_REFLECT:
         {
            // special case for CtlColor to avoid too many temporary objects
            ASSERT(message == WM_REFLECT_BASE+WM_CTLCOLOR);
            AFX_CTLCOLOR* pCtl = reinterpret_cast<AFX_CTLCOLOR*>(lParam);
            ::ca::graphics_sp dcTemp; 
            dcTemp.set_handle1(pCtl->hDC);
            UINT nCtlType = pCtl->nCtlType;
            HBRUSH hbr = (this->*mmf.pfn_B_D_u)(&dcTemp, nCtlType);
            // fast detach of temporary objects
            dcTemp.set_handle1(NULL);
            lResult = reinterpret_cast<LRESULT>(hbr);
         }
         break;

      case AfxSig_i_u_W_u:
         lResult = (this->*mmf.pfn_i_u_W_u)(LOWORD(wParam),
            ::win::window::from_handle(reinterpret_cast<HWND>(lParam)), HIWORD(wParam));
         break;

      case AfxSig_i_uu_v:
         lResult = (this->*mmf.pfn_i_u_u)(LOWORD(wParam), HIWORD(wParam));
         break;

      case AfxSig_i_W_uu:
         lResult = (this->*mmf.pfn_i_W_u_u)(::win::window::from_handle(reinterpret_cast<HWND>(wParam)),
            LOWORD(lParam), HIWORD(lParam));
         break;

      case AfxSig_i_v_s:
         lResult = (this->*mmf.pfn_i_s)(reinterpret_cast<LPTSTR>(lParam));
         break;

      case AfxSig_l_w_l:
         lResult = (this->*mmf.pfn_l_w_l)(wParam, lParam);
         break;

      
         
      case AfxSig_v_b_h:
          (this->*mmf.pfn_v_b_h)(static_cast<BOOL>(wParam), 
            reinterpret_cast<HANDLE>(lParam));
         break;

      case AfxSig_v_h_v:
          (this->*mmf.pfn_v_h)(reinterpret_cast<HANDLE>(wParam));
         break;

      case AfxSig_v_h_h:
          (this->*mmf.pfn_v_h_h)(reinterpret_cast<HANDLE>(wParam), 
            reinterpret_cast<HANDLE>(lParam));
         break;

      case AfxSig_v_v_v:
         (this->*mmf.pfn_v_v)();
         break;

      case AfxSig_v_u_v:
         (this->*mmf.pfn_v_u)(static_cast<UINT>(wParam));
         break;

      case AfxSig_v_u_u:
         (this->*mmf.pfn_v_u_u)(static_cast<UINT>(wParam), static_cast<UINT>(lParam));
         break;

      case AfxSig_v_uu_v:
         (this->*mmf.pfn_v_u_u)(LOWORD(wParam), HIWORD(wParam));
         break;

      case AfxSig_v_v_ii:
         (this->*mmf.pfn_v_i_i)(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
         break;

      case AfxSig_v_u_uu:
         (this->*mmf.pfn_v_u_u_u)(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
         break;

      case AfxSig_v_u_ii:
         (this->*mmf.pfn_v_u_i_i)(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
         break;

      case AfxSig_v_w_l:
         (this->*mmf.pfn_v_w_l)(wParam, lParam);
         break;

      case AfxSig_MDIACTIVATE:
         (this->*mmf.pfn_v_b_W_W)(get_handle() == reinterpret_cast<HWND>(lParam),
            ::win::window::from_handle(reinterpret_cast<HWND>(lParam)),
            ::win::window::from_handle(reinterpret_cast<HWND>(wParam)));
         break;

      case AfxSig_v_D_v:
         (this->*mmf.pfn_v_D)(::win::graphics::from_handle(reinterpret_cast<HDC>(wParam)));
         break;


      case AfxSig_v_W_v:
         (this->*mmf.pfn_v_W)(::win::window::from_handle(reinterpret_cast<HWND>(wParam)));
         break;

      case AfxSig_v_v_W:
         (this->*mmf.pfn_v_W)(::win::window::from_handle(reinterpret_cast<HWND>(lParam)));
         break;

      case AfxSig_v_W_uu:
         (this->*mmf.pfn_v_W_u_u)(::win::window::from_handle(reinterpret_cast<HWND>(wParam)), LOWORD(lParam),
            HIWORD(lParam));
         break;

      case AfxSig_v_W_p:
         {
            point point(lParam);
            (this->*mmf.pfn_v_W_p)(::win::window::from_handle(reinterpret_cast<HWND>(wParam)), point);
         }
         break;

      case AfxSig_v_W_h:
         (this->*mmf.pfn_v_W_h)(::win::window::from_handle(reinterpret_cast<HWND>(wParam)),
               reinterpret_cast<HANDLE>(lParam));
         break;

      case AfxSig_ACTIVATE:
         (this->*mmf.pfn_v_u_W_b)(LOWORD(wParam),
            ::win::window::from_handle(reinterpret_cast<HWND>(lParam)), HIWORD(wParam));
         break;

      case AfxSig_SCROLL:
      case AfxSig_SCROLL_REFLECT:
         {
            // special case for WM_VSCROLL and WM_HSCROLL
            ASSERT(message == WM_VSCROLL || message == WM_HSCROLL ||
               message == WM_VSCROLL+WM_REFLECT_BASE || message == WM_HSCROLL+WM_REFLECT_BASE);
            int nScrollCode = (short)LOWORD(wParam);
            int nPos = (short)HIWORD(wParam);
            if (lpEntry->nSig == AfxSig_SCROLL)
               (this->*mmf.pfn_v_u_u_W)(nScrollCode, nPos,
                  ::win::window::from_handle(reinterpret_cast<HWND>(lParam)));
            else
               (this->*mmf.pfn_v_u_u)(nScrollCode, nPos);
         }
         break;

      case AfxSig_v_v_s:
         (this->*mmf.pfn_v_s)(reinterpret_cast<LPTSTR>(lParam));
         break;

      case AfxSig_v_u_cs:
         (this->*mmf.pfn_v_u_cs)(static_cast<UINT>(wParam), reinterpret_cast<const char *>(lParam));
         break;

      case AfxSig_OWNERDRAW:
         (this->*mmf.pfn_v_i_s)(static_cast<int>(wParam), reinterpret_cast<LPTSTR>(lParam));
         lResult = TRUE;
         break;

      case AfxSig_i_i_s:
         lResult = (this->*mmf.pfn_i_i_s)(static_cast<int>(wParam), reinterpret_cast<LPTSTR>(lParam));
         break;

      case AfxSig_u_v_p:
         {
            point point(lParam);
            lResult = (this->*mmf.pfn_u_p)(point);
         }
         break;

      case AfxSig_u_v_v:
         lResult = (this->*mmf.pfn_u_v)();
         break;

      case AfxSig_v_b_NCCALCSIZEPARAMS:
         (this->*mmf.pfn_v_b_NCCALCSIZEPARAMS)(static_cast<BOOL>(wParam), 
            reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam));
         break;

      case AfxSig_v_v_WINDOWPOS:
         (this->*mmf.pfn_v_v_WINDOWPOS)(reinterpret_cast<WINDOWPOS*>(lParam));
         break;

      case AfxSig_v_uu_M:
         (this->*mmf.pfn_v_u_u_M)(LOWORD(wParam), HIWORD(wParam), reinterpret_cast<HMENU>(lParam));
         break;

      case AfxSig_v_u_p:
         {
            point point(lParam);
            (this->*mmf.pfn_v_u_p)(static_cast<UINT>(wParam), point);
         }
         break;

      case AfxSig_SIZING:
         (this->*mmf.pfn_v_u_pr)(static_cast<UINT>(wParam), reinterpret_cast<LPRECT>(lParam));
         lResult = TRUE;
         break;

      case AfxSig_MOUSEWHEEL:
         lResult = (this->*mmf.pfn_b_u_s_p)(LOWORD(wParam), (short)HIWORD(wParam),
            point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
         if (!lResult)
            return FALSE;
         break;
      case AfxSig_l:
         lResult = (this->*mmf.pfn_l_v)();
         if (lResult != 0)
            return FALSE;
         break;
      }
      goto LReturnTrue;

   LDispatchRegistered:    // for registered windows messages
      ASSERT(message >= 0xC000);
      ASSERT(sizeof(mmf) == sizeof(mmf.pfn));
      mmf.pfn = lpEntry->pfn;
      lResult = (this->*mmf.pfn_l_w_l)(wParam, lParam);

   LReturnTrue:
      if (pResult != NULL)
         *pResult = lResult;
      return TRUE;
   }
   */


   /////////////////////////////////////////////////////////////////////////////
   // window command handling

   BOOL window::OnCommand(WPARAM wParam, LPARAM lParam)
      // return TRUE if command invocation was attempted
   {
      UNREFERENCED_PARAMETER(wParam);
      UNREFERENCED_PARAMETER(lParam);
   /*   UINT nID = LOWORD(wParam);
      HWND hWndCtrl = (HWND)lParam;
      int nCode = HIWORD(wParam);

      // default routing for command messages (through closure table)

      if (hWndCtrl == NULL)
      {
         // zero IDs for normal commands are not allowed
         if (nID == 0)
            return FALSE;

         // make sure command has not become disabled before routing
         CTestCmdUI state;
         state.m_id = nID;
         _001OnCommand(nID, CN_UPDATE_COMMAND_UI, &state, NULL);
         if (!state.m_bEnabled)
         {
            TRACE(::radix::trace::category_AppMsg, 0, "Warning: not executing disabled command %d\n", nID);
            return TRUE;
         }

         // menu or accelerator
         nCode = CN_COMMAND;
      }
      else
      {
         // control notification
         ASSERT(nID == 0 || ::IsWindow(hWndCtrl));

         if (_afxThreadState->m_hLockoutNotifyWindow == get_handle())
            return TRUE;        // locked out - ignore control notification

         // reflect notification to child window control
         if (ReflectLastMsg(hWndCtrl))
            return TRUE;    // eaten by child

         // zero IDs for normal commands are not allowed
         if (nID == 0)
            return FALSE;
      }

   #ifdef _DEBUG
      if (nCode < 0 && nCode != (int)0x8000)
         TRACE(::radix::trace::category_AppMsg, 0, "Implementation Warning: control notification = $%X.\n",
            nCode);
   #endif

      return _001OnCommand(nID, nCode, NULL, NULL);*/
      return FALSE;
   }

   BOOL window::OnNotify(WPARAM, LPARAM lParam, LRESULT* pResult)
   {
      ASSERT(pResult != NULL);
      NMHDR* pNMHDR = (NMHDR*)lParam;
      HWND hWndCtrl = pNMHDR->hwndFrom;

      // get the child ID from the window itself
//      UINT_PTR nID = _AfxGetDlgCtrlID(hWndCtrl);
//      int nCode = pNMHDR->code;

      ASSERT(hWndCtrl != NULL);
      ASSERT(::IsWindow(hWndCtrl));

      if (_afxThreadState->m_hLockoutNotifyWindow == get_handle())
         return TRUE;        // locked out - ignore control notification

      // reflect notification to child window control
      if (ReflectLastMsg(hWndCtrl, pResult))
         return TRUE;        // eaten by child

      AFX_NOTIFY notify;
      notify.pResult = pResult;
      notify.pNMHDR = pNMHDR;
   //xxx   return _001OnCommand((UINT)nID, MAKELONG(nCode, WM_NOTIFY), &notify, NULL);
      return FALSE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // window extensions

   frame_window* window::GetParentFrame()
   {
      if (get_handle() == NULL) // no Window attached
      {
         return NULL;
      }

      ASSERT_VALID(this);

      ::user::interaction * pParentWnd = GetParent();  // start with one parent up
      while (pParentWnd != NULL)
      {
         if (pParentWnd->IsFrameWnd())
         {
            return dynamic_cast < frame_window * > (pParentWnd);
         }
         pParentWnd = pParentWnd->GetParent();
      }
      return NULL;
   }

   /* trans HWND CLASS_DECL_VMSWIN AfxGetParentOwner(::user::interaction * hWnd)
   {
      // check for permanent-owned window first
      ::ca::window * pWnd = ::win::window::FromHandlePermanent(hWnd);
      if (pWnd != NULL)
         return WIN_WINDOW(pWnd)->GetOwner();

      // otherwise, return parent in the Windows sense
      return (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD) ?
         ::GetParent(hWnd) : ::GetWindow(hWnd, GW_OWNER);
   }*/


   ::user::interaction * window::GetTopLevelParent()
   {
      if (get_handle() == NULL) // no Window attached
         return NULL;

      ASSERT_VALID(this);

      ::user::interaction * hWndParent = this;
      ::user::interaction * hWndT;
      while ((hWndT = AfxGetParentOwner(hWndParent)) != NULL)
         hWndParent = hWndT;

      return hWndParent;
   }

   ::user::interaction * window::GetTopLevelOwner()
   {
      if (get_handle() == NULL) // no Window attached
         return NULL;

      ASSERT_VALID(this);

      HWND hWndOwner = get_handle();
      HWND hWndT;
      while ((hWndT = ::GetWindow(hWndOwner, GW_OWNER)) != NULL)
         hWndOwner = hWndT;

      return ::win::window::from_handle(hWndOwner);
   }

   ::user::interaction * window::GetParentOwner()
   {
      if (get_handle() == NULL) // no Window attached
         return NULL;

      ASSERT_VALID(this);

      HWND hWndParent = get_handle();
      HWND hWndT;
      while ((::GetWindowLong(hWndParent, GWL_STYLE) & WS_CHILD) &&
         (hWndT = ::GetParent(hWndParent)) != NULL)
      {
         hWndParent = hWndT;
      }

      return ::win::window::from_handle(hWndParent);
   }

   BOOL window::IsTopParentActive()
   {
      ASSERT(get_handle() != NULL);
      ASSERT_VALID(this);

      ::user::interaction *pWndTopLevel=EnsureTopLevelParent();
      
      return window::GetForegroundWindow() == pWndTopLevel->GetLastActivePopup();
   }

   void window::ActivateTopParent()
   {
      // special activate logic for floating toolbars and palettes
      ::ca::window * pActiveWnd = GetForegroundWindow();
      if (pActiveWnd == NULL || !(WIN_WINDOW(pActiveWnd)->get_handle() == get_handle() || ::IsChild(WIN_WINDOW(pActiveWnd)->get_handle(), get_handle())))
      {
         // clicking on floating frame when it does not have
         // focus itself -- activate the toplevel frame instead.
         EnsureTopLevelParent()->SetForegroundWindow();
      }
   }

   frame_window* window::GetTopLevelFrame()
   {
      if (get_handle() == NULL) // no Window attached
         return NULL;

      ASSERT_VALID(this);

      frame_window* pFrameWnd = NULL;
      if(m_pguie != this)
         pFrameWnd = dynamic_cast < frame_window * > (m_pguie);
      else
         pFrameWnd = dynamic_cast < frame_window * > (this);
      if (pFrameWnd == NULL || !pFrameWnd->IsFrameWnd())
         pFrameWnd = GetParentFrame();

      if (pFrameWnd != NULL)
      {
         frame_window* pTemp;
         while ((pTemp = pFrameWnd->GetParentFrame()) != NULL)
            pFrameWnd = pTemp;
      }
      return pFrameWnd;
   }

   ::ca::window * PASCAL window::GetSafeOwner(::ca::window * pParent, HWND* pWndTop)
   {
      HWND hWnd = GetSafeOwner_((HWND) pParent->get_os_data(), pWndTop);
      return ::win::window::from_handle(hWnd);
   }

   int window::MessageBox(const char * lpszText, const char * lpszCaption, UINT nType)
   {
      if (lpszCaption == NULL)
         lpszCaption = AfxGetAppName();
      int nResult = ::MessageBox(get_handle(), lpszText, lpszCaption, nType);
      return nResult;
   }

   ::user::interaction * PASCAL window::GetDescendantWindow(::user::interaction * hWnd, id id)
   {
      single_lock sl(&hWnd->m_pthread->m_mutex, TRUE);
      // GetDlgItem recursive (return first found)
      // breadth-first for 1 level, then depth-first for next level

      // use GetDlgItem since it is a fast USER function
      ::user::interaction * pWndChild;
      if ((pWndChild = hWnd->GetDlgItem(id)) != NULL)
      {
         if (pWndChild->GetTopWindow() != NULL)
         {
            // children with the same ID as their parent have priority
            pWndChild = GetDescendantWindow(pWndChild, id);
            if (pWndChild != NULL)
               return pWndChild;
         }
         if (pWndChild != NULL)
            return pWndChild;
      }

      for(int i = 0; i < hWnd->m_uiptraChild.get_count(); i++)
      {
         if(hWnd->m_uiptraChild[i]->GetDlgCtrlId() == id)
         {
            if(hWnd->m_uiptraChild[i]->GetDescendantWindow(id))
               return hWnd->m_uiptraChild[i]->GetDescendantWindow(id);
            else
               return hWnd->m_uiptraChild[i];
         }
      }

      if(pWndChild == NULL)
         return NULL;

      // walk each child
      for (pWndChild = pWndChild->GetTopWindow(); pWndChild != NULL;
         pWndChild = pWndChild->GetNextWindow(GW_HWNDNEXT))
      {
         pWndChild = GetDescendantWindow(pWndChild, id);
         if (pWndChild != NULL)
            return pWndChild;
      }
      return NULL;    // not found
   }

   void PASCAL window::SendMessageToDescendants(HWND hWnd, UINT message,
      WPARAM wParam, LPARAM lParam, BOOL bDeep, BOOL bOnlyPerm)
   {
      // walk through HWNDs to avoid creating temporary window objects
      // unless we need to call this function recursively
      for (HWND hWndChild = ::GetTopWindow(hWnd); hWndChild != NULL;
         hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
      {
         // if bOnlyPerm is TRUE, don't send to non-permanent windows
         if (bOnlyPerm)
         {
            ::ca::window * pWnd = ::win::window::FromHandlePermanent(hWndChild);
            if (pWnd != NULL)
            {
               // call window proc directly since it is a C++ window
               AfxCallWndProc(dynamic_cast < ::user::interaction * > (pWnd), WIN_WINDOW(pWnd)->get_handle(), message, wParam, lParam);
            }
         }
         else
         {
            try
            {
               // send message with Windows SendMessage API
               ::SendMessage(hWndChild, message, wParam, lParam);
            }
            catch(...)
            {
            }
         }
         if (bDeep && ::GetTopWindow(hWndChild) != NULL)
         {
            // send to child windows after parent
            try
            {
               SendMessageToDescendants(hWndChild, message, wParam, lParam,
                  bDeep, bOnlyPerm);
            }
            catch(...)
            {
            }
         }
      }

   }

   /////////////////////////////////////////////////////////////////////////////
   // Scroll bar helpers
   //  hook for window functions
   //    only works for derived class (eg: ::view) that override 'GetScrollBarCtrl'
   // if the window doesn't have a _visible_ windows scrollbar - then
   //   look for a sibling with the appropriate ID

   CScrollBar* window::GetScrollBarCtrl(int) const
   {
      return NULL;        // no special scrollers supported
   }

   int window::SetScrollPos(int nBar, int nPos, BOOL bRedraw)
   {
         return ::SetScrollPos(get_handle(), nBar, nPos, bRedraw);
   }

   int window::GetScrollPos(int nBar) const
   {
         return ::GetScrollPos(get_handle(), nBar);
   }

   void window::SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw)
   {
         ::SetScrollRange(get_handle(), nBar, nMinPos, nMaxPos, bRedraw);
   }

   void window::GetScrollRange(int nBar, LPINT lpMinPos, LPINT lpMaxPos) const
   {
         ::GetScrollRange(get_handle(), nBar, lpMinPos, lpMaxPos);
   }

   // Turn on/off non-control scrollbars
   //   for WS_?SCROLL scrollbars - show/hide them
   //   for control scrollbar - enable/disable them
   void window::EnableScrollBarCtrl(int nBar, BOOL bEnable)
   {
         // WS_?SCROLL scrollbar - show or hide
         ShowScrollBar(nBar, bEnable);
   }

   BOOL window::SetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, BOOL bRedraw)
   {
      ASSERT(lpScrollInfo != NULL);

      HWND hWnd = get_handle();
      lpScrollInfo->cbSize = sizeof(*lpScrollInfo);
      ::SetScrollInfo(hWnd, nBar, lpScrollInfo, bRedraw);
      return TRUE;
   }

   BOOL window::GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, UINT nMask)
   {
      UNREFERENCED_PARAMETER(nMask);
      ASSERT(lpScrollInfo != NULL);

      HWND hWnd = get_handle();
      return ::GetScrollInfo(hWnd, nBar, lpScrollInfo);
   }

   int window::GetScrollLimit(int nBar)
   {
      int nMin, nMax;
      GetScrollRange(nBar, &nMin, &nMax);
      SCROLLINFO info;
      if (GetScrollInfo(nBar, &info, SIF_PAGE))
      {
         nMax -= __max(info.nPage-1,0);
      }
      return nMax;
   }

   void window::ScrollWindow(int xAmount, int yAmount,
      LPCRECT lpRect, LPCRECT lpClipRect)
   {
      ASSERT(::IsWindow(get_handle()));

      if (IsWindowVisible() || lpRect != NULL || lpClipRect != NULL)
      {
         // When visible, let Windows do the scrolling
         ::ScrollWindow(get_handle(), xAmount, yAmount, lpRect, lpClipRect);
      }
      else
      {
         // Windows does not perform any scrolling if the window is
         // not visible.  This leaves child windows unscrolled.
         // To account for this oversight, the child windows are moved
         // directly instead.
         HWND hWndChild = ::GetWindow(get_handle(), GW_CHILD);
         if (hWndChild != NULL)
         {
            for (; hWndChild != NULL;
               hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
            {
               rect rect;
               ::GetWindowRect(hWndChild, &rect);
               ScreenToClient(&rect);
               ::SetWindowPos(hWndChild, NULL,
                  rect.left+xAmount, rect.top+yAmount, 0, 0,
                  SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
            }
         }
      }

   }

   /////////////////////////////////////////////////////////////////////////////
   // minimal layout support

   /*
   void window::RepositionBars(const char * pszPrefix, const char * pszIdLeftOver,
      UINT nFlags, LPRECT lpRectParam, LPCRECT lpRectClient, BOOL bStretch)
   {
      ASSERT(nFlags == 0 || (nFlags & ~reposNoPosLeftOver) == reposQuery || 
            (nFlags & ~reposNoPosLeftOver) == reposExtra);

      // walk kids in order, control bars get the resize notification
      //   which allow them to shrink the client area
      // remaining size goes to the 'nIDLeftOver' pane
      // NOTE: nIDFirst->nIDLast are usually 0->0xffff

      AFX_SIZEPARENTPARAMS layout;
      ::user::interaction * hWndLeftOver = NULL;

      layout.bStretch = bStretch;
      layout.sizeTotal.cx = layout.sizeTotal.cy = 0;
      if (lpRectClient != NULL)
         layout.rect = *lpRectClient;    // starting rect comes from parameter
      else
      {
         if(m_pguie != this)
            m_pguie->GetClientRect(&layout.rect);    // starting rect comes from client rect
         else
            GetClientRect(&layout.rect);    // starting rect comes from client rect
      }

      if ((nFlags & ~reposNoPosLeftOver) != reposQuery)
         layout.hDWP = ::BeginDeferWindowPos(8); // reasonable guess
      else
         layout.hDWP = NULL; // not actually doing layout

      if(m_pguie != this && m_pguie != NULL)
      {
         for (::user::interaction * hWndChild = m_pguie->GetTopWindow(); hWndChild != NULL;
            hWndChild = hWndChild->GetNextWindow(GW_HWNDNEXT))
         {
            string strIdc = hWndChild->GetDlgCtrlId();
            ::user::interaction * pWnd = hWndChild;
            if (strIdc == pszIdLeftOver)
               hWndLeftOver = hWndChild;
            else if (gen::str::begins(strIdc, pszPrefix) && pWnd != NULL)
               hWndChild->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&layout);
         }
         for (int i = 0; i < m_pguie->m_uiptra.get_count();   i++)
         {
            ::user::interaction * hWndChild = m_pguie->m_uiptra[i];
            string strIdc = hWndChild->GetDlgCtrlId();
            ::user::interaction * pWnd = hWndChild;
            if (strIdc == pszIdLeftOver)
               hWndLeftOver = hWndChild;
            else if (gen::str::begins(strIdc, pszPrefix) && pWnd != NULL)
               hWndChild->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&layout);
         }
      }
      else
      {
         for (::user::interaction * hWndChild = GetTopWindow(); hWndChild != NULL;
            hWndChild = hWndChild->GetNextWindow(GW_HWNDNEXT))
         {
            string strIdc = hWndChild->GetDlgCtrlId();
            ::user::interaction * pWnd = hWndChild;
            if (strIdc == pszIdLeftOver)
               hWndLeftOver = hWndChild;
            else if (gen::str::begins(strIdc, pszPrefix) && pWnd != NULL)
               hWndChild->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&layout);
         }
         for (int i = 0; i < m_uiptra.get_count();   i++)
         {
            ::user::interaction * hWndChild = m_uiptra[i];
            string strIdc = hWndChild->GetDlgCtrlId();
            ::user::interaction * pWnd = hWndChild;
            if (strIdc == pszIdLeftOver)
               hWndLeftOver = hWndChild;
            else if (gen::str::begins(strIdc, pszPrefix) && pWnd != NULL)
               hWndChild->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&layout);
         }
      }

      // if just getting the available rectangle, return it now...
      if ((nFlags & ~reposNoPosLeftOver) == reposQuery)
      {
         ASSERT(lpRectParam != NULL);
         if (bStretch)
            ::copy(lpRectParam, &layout.rect);
         else
         {
            lpRectParam->left = lpRectParam->top = 0;
            lpRectParam->right = layout.sizeTotal.cx;
            lpRectParam->bottom = layout.sizeTotal.cy;
         }
         return;
      }

      // the rest is the client size of the left-over pane
      if (pszIdLeftOver != NULL && hWndLeftOver != NULL)
      {
         ::user::interaction * pLeftOver = hWndLeftOver;
         // allow extra space as specified by lpRectBorder
         if ((nFlags & ~reposNoPosLeftOver) == reposExtra)
         {
            ASSERT(lpRectParam != NULL);
            layout.rect.left += lpRectParam->left;
            layout.rect.top += lpRectParam->top;
            layout.rect.right -= lpRectParam->right;
            layout.rect.bottom -= lpRectParam->bottom;
         }
         // reposition the window
         if ((nFlags & reposNoPosLeftOver) != reposNoPosLeftOver)
         {
            pLeftOver->CalcWindowRect(&layout.rect);
            AfxRepositionWindow(&layout, pLeftOver, &layout.rect);
         }
      }

      // move and resize all the windows at once!
      if (layout.hDWP == NULL || !::EndDeferWindowPos(layout.hDWP))
         TRACE(::radix::trace::category_AppMsg, 0, "Warning: DeferWindowPos failed - low system resources.\n");
   }

   */

   void window::RepositionBars(UINT nIDFirst, UINT nIDLast, id nIdLeftOver,
         UINT nFlags, LPRECT lpRectParam, LPCRECT lpRectClient, BOOL bStretch)
   {
      UNREFERENCED_PARAMETER(nIDFirst);
      UNREFERENCED_PARAMETER(nIDLast);
      
      ASSERT(nFlags == 0 || (nFlags & ~reposNoPosLeftOver) == reposQuery || 
            (nFlags & ~reposNoPosLeftOver) == reposExtra);

      // walk kids in order, control bars get the resize notification
      //   which allow them to shrink the client area
      // remaining size goes to the 'nIDLeftOver' pane
      // NOTE: nIDFirst->nIDLast are usually 0->0xffff

      AFX_SIZEPARENTPARAMS layout;
      ::user::interaction * hWndLeftOver = NULL;

      layout.bStretch = bStretch;
      layout.sizeTotal.cx = layout.sizeTotal.cy = 0;
      if (lpRectClient != NULL)
         layout.rect = *lpRectClient;    // starting rect comes from parameter
      else
      {
         if(m_pguie != this)
            m_pguie->GetClientRect(&layout.rect);    // starting rect comes from client rect
         else
            GetClientRect(&layout.rect);    // starting rect comes from client rect
      }

      if ((nFlags & ~reposNoPosLeftOver) != reposQuery)
         layout.hDWP = ::BeginDeferWindowPos(8); // reasonable guess
      else
         layout.hDWP = NULL; // not actually doing layout

      if(m_pguie != this && m_pguie != NULL)
      {
         for (::user::interaction * hWndChild = m_pguie->GetTopWindow(); hWndChild != NULL;
            hWndChild = hWndChild->GetNextWindow(GW_HWNDNEXT))
         {
            id id = hWndChild->GetDlgCtrlId();
            ::user::interaction * pWnd = hWndChild;
            if (id == (int) nIdLeftOver)
               hWndLeftOver = hWndChild;
            else if (pWnd != NULL)
               hWndChild->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&layout);
         }
         for (::user::interaction * hWndChild = m_pguie->get_top_child(); hWndChild != NULL;
            hWndChild = hWndChild->under_sibling())
         {
            id id = hWndChild->GetDlgCtrlId();
            ::user::interaction * pWnd = hWndChild;
            if (id == nIdLeftOver)
               hWndLeftOver = hWndChild;
            else if (pWnd != NULL)
               hWndChild->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&layout);
         }
      }
      else
      {
         for (::user::interaction * hWndChild = GetTopWindow(); hWndChild != NULL;
            hWndChild = hWndChild->GetNextWindow(GW_HWNDNEXT))
         {
            id id = hWndChild->GetDlgCtrlId();
            ::user::interaction * pWnd = hWndChild;
            if (id == nIdLeftOver)
               hWndLeftOver = hWndChild;
            else if (pWnd != NULL)
               hWndChild->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&layout);
         }
         for (::user::interaction * hWndChild = m_pguie->get_top_child(); hWndChild != NULL;
            hWndChild = hWndChild->under_sibling())
         {
            id id = hWndChild->GetDlgCtrlId();
            ::user::interaction * pWnd = hWndChild;
            if (id == nIdLeftOver)
               hWndLeftOver = hWndChild;
            else if (pWnd != NULL)
               hWndChild->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&layout);
         }
      }

      // if just getting the available rectangle, return it now...
      if ((nFlags & ~reposNoPosLeftOver) == reposQuery)
      {
         ASSERT(lpRectParam != NULL);
         if (bStretch)
            ::CopyRect(lpRectParam, &layout.rect);
         else
         {
            lpRectParam->left = lpRectParam->top = 0;
            lpRectParam->right = layout.sizeTotal.cx;
            lpRectParam->bottom = layout.sizeTotal.cy;
         }
         return;
      }

      // the rest is the client size of the left-over pane
      if(nIdLeftOver != NULL && hWndLeftOver != NULL)
      {
         ::user::interaction * pLeftOver = hWndLeftOver;
         // allow extra space as specified by lpRectBorder
         if ((nFlags & ~reposNoPosLeftOver) == reposExtra)
         {
            ASSERT(lpRectParam != NULL);
            layout.rect.left += lpRectParam->left;
            layout.rect.top += lpRectParam->top;
            layout.rect.right -= lpRectParam->right;
            layout.rect.bottom -= lpRectParam->bottom;
         }
         // reposition the window
         if ((nFlags & reposNoPosLeftOver) != reposNoPosLeftOver)
         {
            pLeftOver->CalcWindowRect(&layout.rect);
            AfxRepositionWindow(&layout, pLeftOver, &layout.rect);
         }
      }

      // move and resize all the windows at once!
      if (layout.hDWP == NULL || !::EndDeferWindowPos(layout.hDWP))
         TRACE(::radix::trace::category_AppMsg, 0, "Warning: DeferWindowPos failed - low system resources.\n");
   }



   void window::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
   {
      DWORD dwExStyle = GetExStyle();
      if (nAdjustType == 0)
         dwExStyle &= ~WS_EX_CLIENTEDGE;
      ::AdjustWindowRectEx(lpClientRect, GetStyle(), FALSE, dwExStyle);
   }

   /////////////////////////////////////////////////////////////////////////////
   // Special keyboard/system command processing

   BOOL window::HandleFloatingSysCommand(UINT nID, LPARAM lParam)
   {
      ::user::interaction* pParent = GetTopLevelParent();
      switch (nID & 0xfff0)
      {
      case SC_PREVWINDOW:
      case SC_NEXTWINDOW:
         if (LOWORD(lParam) == VK_F6 && pParent != NULL)
         {
            pParent->SetFocus();
            return TRUE;
         }
         break;

      case SC_CLOSE:
      case SC_KEYMENU:
         // Check lParam.  If it is 0L, then the ::fontopus::user may have done
         // an Alt+Tab, so just ignore it.  This breaks the ability to
         // just press the Alt-key and have the first menu selected,
         // but this is minor compared to what happens in the Alt+Tab
         // case.
         if ((nID & 0xfff0) == SC_CLOSE || lParam != 0L)
         {
            if (pParent != NULL)
            {
               // Sending the above WM_SYSCOMMAND may destroy the cast,
               // so we have to be careful about restoring activation
               // and focus after sending it.
               HWND hWndSave = get_handle();
               HWND hWndFocus = ::GetFocus();
               pParent->SetActiveWindow();
               pParent->SendMessage(WM_SYSCOMMAND, nID, lParam);

               // be very careful here...
               if (::IsWindow(hWndSave))
                  ::SetActiveWindow(hWndSave);
               if (::IsWindow(hWndFocus))
                  ::SetFocus(hWndFocus);
            }
         }
         return TRUE;
      }
      return FALSE;
   }

   void window::WalkPreTranslateTree(HWND hWndStop, gen::signal_object * pobj)
   {
      ASSERT(hWndStop == NULL || ::IsWindow(hWndStop));
      ASSERT(pobj != NULL);

      SCAST_PTR(::gen::message::base, pbase, pobj);
      // walk from the target window up to the hWndStop window checking
      //  if any window wants to translate this message

      for (HWND hWnd = pbase->m_hwnd; hWnd != NULL; hWnd = ::GetParent(hWnd))
      {
         ::ca::window * pWnd = ::win::window::FromHandlePermanent(hWnd);
         if (pWnd != NULL)
         {
            // target window is a C++ window
            WIN_WINDOW(pWnd)->pre_translate_message(pobj);
            if(pobj->m_bRet)
               return; // trapped by target window (eg: accelerators)
         }

         // got to hWndStop window without interest
         if (hWnd == hWndStop)
            break;
      }
      // no special processing
   }

   BOOL window::SendChildNotifyLastMsg(LRESULT* pResult)
   {
      _AFX_THREAD_STATE* pThreadState = _afxThreadState.get_data();
      return OnChildNotify(pThreadState->m_lastSentMsg.message,
         pThreadState->m_lastSentMsg.wParam, pThreadState->m_lastSentMsg.lParam, pResult);
   }

   BOOL PASCAL window::ReflectLastMsg(HWND hWndChild, LRESULT* pResult)
   {
      // get the ::collection::map, and if no ::collection::map, then this message does not need reflection
      hwnd_map * pMap = afxMapHWND();
      if (pMap == NULL)
         return FALSE;

      // check if in permanent ::collection::map, if it is reflect it (could be OLE control)
      ::ca::window * pWnd = dynamic_cast < ::ca::window * > (pMap->lookup_permanent(hWndChild));
      ASSERT(pWnd == NULL || WIN_WINDOW(pWnd)->get_handle() == hWndChild);
      if (pWnd == NULL)
      {
         return FALSE;
      }

      // only OLE controls and permanent windows will get reflected msgs
      ASSERT(pWnd != NULL);
      return WIN_WINDOW(pWnd)->SendChildNotifyLastMsg(pResult);
   }

   BOOL window::OnChildNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
   {

      return ReflectChildNotify(uMsg, wParam, lParam, pResult);
   }

   BOOL window::ReflectChildNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
   {
      UNREFERENCED_PARAMETER(wParam);
      // Note: reflected messages are send directly to window::OnWndMsg
      //  and window::_001OnCommand for speed and because these messages are not
      //  routed by normal _001OnCommand routing (they are only dispatched)

      switch (uMsg)
      {
      // normal messages (just wParam, lParam through OnWndMsg)
      case WM_HSCROLL:
      case WM_VSCROLL:
      case WM_PARENTNOTIFY:
      case WM_DRAWITEM:
      case WM_MEASUREITEM:
      case WM_DELETEITEM:
      case WM_VKEYTOITEM:
      case WM_CHARTOITEM:
      case WM_COMPAREITEM:
         // reflect the message through the message ::collection::map as WM_REFLECT_BASE+uMsg
         //return window::OnWndMsg(WM_REFLECT_BASE+uMsg, wParam, lParam, pResult);
         return FALSE;

      // special case for WM_COMMAND
      case WM_COMMAND:
         {
            // reflect the message through the message ::collection::map as OCM_COMMAND
   /* xxx         int nCode = HIWORD(wParam);
            if (window::_001OnCommand(0, MAKELONG(nCode, WM_REFLECT_BASE+WM_COMMAND), NULL, NULL))
            {
               if (pResult != NULL)
                  *pResult = 1;
               return TRUE;
            } */
         }
         break;

      // special case for WM_NOTIFY
      case WM_NOTIFY:
         {
            // reflect the message through the message ::collection::map as OCM_NOTIFY
            NMHDR* pNMHDR = (NMHDR*)lParam;
//            int nCode = pNMHDR->code;
            AFX_NOTIFY notify;
            notify.pResult = pResult;
            notify.pNMHDR = pNMHDR;
   // xxxx         return window::_001OnCommand(0, MAKELONG(nCode, WM_REFLECT_BASE+WM_NOTIFY), &notify, NULL);
         }

      // other special cases (WM_CTLCOLOR family)
      default:
         if (uMsg >= WM_CTLCOLORMSGBOX && uMsg <= WM_CTLCOLORSTATIC)
         {
            // fill in special struct for compatiblity with 16-bit WM_CTLCOLOR
            /*AFX_CTLCOLOR ctl;
            ctl.hDC = (HDC)wParam;
            ctl.nCtlType = uMsg - WM_CTLCOLORMSGBOX;
            //ASSERT(ctl.nCtlType >= CTLCOLOR_MSGBOX);
            ASSERT(ctl.nCtlType <= CTLCOLOR_STATIC);

            // reflect the message through the message ::collection::map as OCM_CTLCOLOR
            BOOL bResult = window::OnWndMsg(WM_REFLECT_BASE+WM_CTLCOLOR, 0, (LPARAM)&ctl, pResult);
            if ((HBRUSH)*pResult == NULL)
               bResult = FALSE;
            return bResult;*/
            return FALSE;
         }
         break;
      }

      return FALSE;   // let the parent handle it
   }

   void window::OnParentNotify(UINT message, LPARAM lParam)
   {
      if ((LOWORD(message) == WM_CREATE || LOWORD(message) == WM_DESTROY))
      {
         if (ReflectLastMsg((HWND)lParam))
            return;     // eat it
      }
      // not handled - do default
      Default();
   }

   void window::OnSetFocus(::ca::window *)
   { 
      BOOL bHandled;

      bHandled = FALSE;
      if( !bHandled )
      {
        Default();
      }
   }

   LRESULT window::OnActivateTopLevel(WPARAM wParam, LPARAM)
   {
      if (LOWORD(wParam) == WA_INACTIVE)
      {
//         AFX_MODULE_THREAD_STATE* pModuleThreadState = AfxGetModuleThreadState();
      }

      return 0;
   }

   void window::OnSysColorChange()
   {
      throw not_implemented_exception();

/*      application* pApp = &System;
      if (pApp != NULL && pApp->GetMainWnd() == this)
      {
         // recolor global brushes used by control bars
         afxData.UpdateSysColors();
      }

      // forward this message to all other child windows
      if (!(GetStyle() & WS_CHILD))
         SendMessageToDescendants(WM_SYSCOLORCHANGE, 0, 0L, TRUE, TRUE);

      Default();*/
   }

   BOOL _afxGotScrollLines;

   void window::OnSettingChange(UINT uFlags, const char * lpszSection)
   {
      UNUSED_ALWAYS(uFlags);
      UNUSED_ALWAYS(lpszSection);

      // force refresh of settings that we cache
      _afxGotScrollLines = FALSE;


      window::OnDisplayChange(0, 0);    // to update system metrics, etc.
   }

   void window::OnDevModeChange(__in LPTSTR lpDeviceName)
   {
      UNREFERENCED_PARAMETER(lpDeviceName);
      throw not_implemented_exception();
      /*application* pApp = &System;
      if (pApp != NULL && pApp->GetMainWnd() == this)
         pApp->DevModeChange(lpDeviceName);

      // forward this message to all other child windows
      if (!(GetStyle() & WS_CHILD))
      {
         const MSG* pMsg = GetCurrentMessage();
         SendMessageToDescendants(pMsg->message, pMsg->wParam, pMsg->lParam,
            TRUE, TRUE);
      }*/
   }

   BOOL window::OnHelpInfo(HELPINFO* /*pHelpInfo*/)
   {
      if (!(GetStyle() & WS_CHILD))
      {
         ::user::interaction* pMainWnd = System.GetMainWnd();
         if (pMainWnd != NULL &&
            GetKeyState(VK_SHIFT) >= 0 &&
            GetKeyState(VK_CONTROL) >= 0 &&
            GetKeyState(VK_MENU) >= 0)
         {
//            pMainWnd->SendMessage(WM_COMMAND, ID_HELP);
            return TRUE;
         }
      }
      return Default() != 0;
   }

   LRESULT window::OnDisplayChange(WPARAM, LPARAM)
   {
      // update metrics if this window is the main window
      if (System.GetMainWnd() == this)
      {
         // update any system metrics cache
         afxData.UpdateSysMetrics();
      }

      // forward this message to all other child windows
      if (!(GetStyle() & WS_CHILD))
      {
         const MSG* pMsg = GetCurrentMessage();
         SendMessageToDescendants(pMsg->message, pMsg->wParam, pMsg->lParam,
            TRUE, TRUE);
      }

      return Default();
   }

   LRESULT window::OnDragList(WPARAM, LPARAM lParam)
   {
      LPDRAGLISTINFO lpInfo = (LPDRAGLISTINFO)lParam;
      ASSERT(lpInfo != NULL);

      LRESULT lResult;
      if (ReflectLastMsg(lpInfo->hWnd, &lResult))
         return (int)lResult;    // eat it

      // not handled - do default
      return (int)Default();
   }

   void window::_001OnCreate(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      Default();
   }

   void window::OnHScroll(UINT, UINT, CScrollBar* pScrollBar)
   {
      UNREFERENCED_PARAMETER(pScrollBar);
      Default();
   }

   void window::OnVScroll(UINT, UINT, CScrollBar* pScrollBar)
   {
      UNREFERENCED_PARAMETER(pScrollBar);
      Default();
   }

   BOOL CALLBACK window::GetAppsEnumWindowsProc(      
       HWND hwnd,
       LPARAM lParam)
   {
      user::HWNDArray * phwnda = (user::HWNDArray *) lParam;
      phwnda->add(hwnd);
      return TRUE;
   }

   void window::get_app_wnda(user::HWNDArray & wnda)
   {
      EnumWindows(GetAppsEnumWindowsProc, (LPARAM) &wnda);
   }

   void window::_001OnDeferPaintLayeredWindowBackground(::ca::graphics * pdc)
   {
      _001DeferPaintLayeredWindowBackground(pdc);
   }


   class print_window :
      virtual ::radix::object
   {
   public:



      manual_reset_event m_event;
      HWND m_hwnd;
      HDC m_hdc;

      print_window(::ca::application * papp, HWND hwnd, HDC hdc, DWORD dwTimeout) :
         ca(papp)
      {
         m_event.ResetEvent();
         m_hwnd = hwnd;
         m_hdc = hdc;
         AfxBeginThread(papp, &print_window::s_print_window, (LPVOID) this, THREAD_PRIORITY_ABOVE_NORMAL);
         if(m_event.wait(millis(dwTimeout)).timeout())
         {
            TRACE("print_window::time_out");
         }
      }


      static UINT AFX_CDECL s_print_window(LPVOID pvoid)
      {
         print_window * pprintwindow = (print_window *) pvoid;
         try
         {
            HANDLE hevent = (HANDLE) pprintwindow->m_event.get_os_data();
            ::PrintWindow(pprintwindow->m_hwnd, pprintwindow->m_hdc, 0);
            ::SetEvent(hevent);
         }
         catch(...)
         {
         }
         return 0;
      }
   };

/*   void window::_001DeferPaintLayeredWindowBackground(::ca::graphics * pdc)
   {

      rect rectClient;

      GetClientRect(rectClient);


      pdc->FillSolidRect(rectClient, 0x00000000);

      return;
      rect rectUpdate;
      GetDesktopWindow()->GetWindowRect(rectUpdate);
      pdc->SetViewportOrg(point(0, 0));
      rect rectPaint;
      rectPaint = rectUpdate;
      ScreenToClient(rectPaint);
      user::HWNDArray wndaApp;

      user::LPWndArray wndpa;

      

      ::ca::region_sp rgnWindow(get_app());
      ::ca::region_sp rgnIntersect(get_app());


      rgnWindow->CreateRectRgn(0, 0, 0, 0);
      rgnIntersect->CreateRectRgn(0, 0, 0, 0);
      

//      int iCount = wndaApp.get_count();
     
      if(GetWindowLong(GWL_EXSTYLE) & WS_EX_LAYERED)
      {
         wndpa = System.frames();
         rect rect5;
         rect rect9;
         ::ca::region_sp rgnUpdate(get_app());
         rgnUpdate->CreateRectRgnIndirect(rectUpdate);
         HWND hwndOrder = ::GetWindow(get_handle(), GW_HWNDNEXT);
         for(;;)
         {
//            char szText[1024];
            //::GetWindowTextA(hwndOrder, szText, sizeof(szText));
            if(hwndOrder == NULL ||
               !::IsWindow(hwndOrder))
               break;
            if(!::IsWindowVisible(hwndOrder) ||
            ::IsIconic(hwndOrder) ||
            hwndOrder == get_handle()
            || wndaApp.contains(hwndOrder))
            {
               if(hwndOrder == get_handle())
               {
                  // add as bookmark - doesn't paint it
                  wndaApp.add(hwndOrder);
               }
            }
   /*         else if(::GetWindowLong(wndaApp[j], GWL_EXSTYLE) & WS_EX_LAYERED)
            {
               j++;
            }*/
            /*else
            {
               rect rectWindow;
               ::GetWindowRect(hwndOrder, rectWindow);
               rgnWindow->SetRectRgn(rectWindow);
               rgnIntersect->SetRectRgn(rect(0, 0, 0, 0));
               rgnIntersect->CombineRgn(rgnUpdate, rgnWindow, RGN_AND);
               rect rectIntersectBox;
               rgnIntersect->GetRgnBox(rectIntersectBox);
               if(rectIntersectBox.is_empty())
               {
               }
               else
               {
                  rgnUpdate->CombineRgn(rgnUpdate, rgnWindow, RGN_DIFF);
                  rect rectDiffBox;
                  rgnUpdate->GetRgnBox(rectDiffBox);
                  wndaApp.add(hwndOrder);
                  if(rectDiffBox.is_empty())
                  {
                     break;
                  }
               }
            }
            hwndOrder = ::GetWindow(hwndOrder, GW_HWNDNEXT);
         }
         for(int j = wndaApp.get_upper_bound(); j >= 0; j--)
         {
            HWND hWnd = wndaApp[j];
            if(hWnd == get_handle())
               break;
            if(!::IsWindowVisible(hWnd) || ::IsIconic(hWnd))
               continue;
            ::GetWindowRect(hWnd, rect5);*/
            //rect9.intersect(rect5, rectUpdate);
            //if(rect9.width() >0 && rect9.height() > 0)
      /*      {
               ::ca::window * pwnd = dynamic_cast < ::ca::window * > (window::FromHandlePermanent(hWnd));
               if(pwnd == NULL)
               {
                  for(int l = 0; l < wndpa.get_count(); l++)
                  {
                     if(wndpa[l]->get_safe_handle() == hWnd)
                     {
                        pwnd = dynamic_cast < ::ca::window * > (wndpa[l]->m_pimpl);
                        break;
                     }
                  }
               }
               if(pwnd != NULL)
               {
                  pwnd->_001Print(pdc);
               }
               else if(::GetWindowLong(wndaApp[j], GWL_EXSTYLE) & WS_EX_LAYERED)
               {
                  HDC hDCMem = CreateCompatibleDC(NULL);
                  HBITMAP hBmp = NULL;
                  {
                     HDC hDC = ::GetDC(hWnd);
                     hBmp = CreateCompatibleBitmap(hDC, rect5.width(), rect5.height());
                     ::ReleaseDC(hWnd, hDC);
                  }
                  HGDIOBJ hOld = SelectObject(hDCMem, hBmp);
                  print_window printwindow(get_app(), hWnd, hDCMem, 284);
                  ::BitBlt(
                     (HDC) pdc->get_os_data(), 
                     rect5.left,
                     rect5.top,
                     rect5.width(), rect5.height(),
                     hDCMem,
                     0, 0,
                     SRCCOPY);
                  ::SelectObject(hDCMem, hOld);
                  ::DeleteObject(hDCMem);
                  ::DeleteObject(hBmp);
               }
               else
               {
                  pdc->SetViewportOrg(point(0, 0));
                  HDC hdcWindow = ::GetDCEx(wndaApp[j], NULL, DCX_WINDOW);
                  if(hdcWindow == NULL)
                     hdcWindow = ::GetDCEx(wndaApp[j], NULL, DCX_WINDOW | DCX_CACHE);
                  if(hdcWindow != NULL)
                  {
                     ::BitBlt(
                        (HDC) pdc->get_os_data(),
                        rect5.left,
                        rect5.top,
                        rect5.width(), rect5.height(),
                        hdcWindow,
                        rect5.left - rect5.left,
                        rect5.top - rect5.top,
                        SRCCOPY);
                     ::ReleaseDC(wndaApp[j], hdcWindow);
                  }
                  else
                  {
                     TRACE0("WARNING: failed to draw a background. this surface probably will be black.");
                  }
               }
            }
         }
      }
   }*/

   void window::_001OnProdevianSynch(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      System.get_event(m_pthread)->SetEvent();
      System.get_event(System.get_twf())->wait(millis(8400));
   }

   void window::_001OnPaint(gen::signal_object * pobj)
   {

      //lock lock(m_pguie, 1984);
      
      SCAST_PTR(::gen::message::base, pbase, pobj);

      PAINTSTRUCT paint;
      memset(&paint, 0, sizeof(paint));
      HDC hdc = ::BeginPaint(get_handle(), &paint);
      ::SelectClipRgn(hdc, NULL);

      try
      {

         ::ca::dib_sp dib(get_app());

         rect rectWindow;
         GetWindowRect(rectWindow);

         if(!dib->create(rectWindow.bottom_right()))
            return;

         ::ca::graphics * pdc = dib->get_graphics();

         if((dynamic_cast<::win::graphics * >(pdc))->get_os_data() == NULL 
         || (dynamic_cast<::win::graphics * >(pdc))->get_handle2() == NULL)
            return;

         rect rectPaint;
         rect rectUpdate;
         rectPaint = paint.rcPaint;
         if(rectPaint.is_null() || (GetExStyle() & WS_EX_LAYERED))
         {
            rectUpdate = rectWindow;
            rectPaint = rectWindow;
            ScreenToClient(rectPaint);
         }
         else
         {
            rectUpdate = rectPaint;
            ClientToScreen(rectUpdate);
         }
         (dynamic_cast<::win::graphics * >(pdc))->SelectClipRgn(NULL);
         if(m_pguie != NULL && m_pguie != this)
         {
            m_pguie->_001OnDeferPaintLayeredWindowBackground(pdc);
         }
         else
         {
            _001OnDeferPaintLayeredWindowBackground(pdc);
         }
         (dynamic_cast<::win::graphics * >(pdc))->SelectClipRgn(NULL);
         (dynamic_cast<::win::graphics * >(pdc))->SetViewportOrg(point(0, 0));
         _000OnDraw(pdc);
         (dynamic_cast<::win::graphics * >(pdc))->SetViewportOrg(point(0, 0));
         //(dynamic_cast<::win::graphics * >(pdc))->FillSolidRect(rectUpdate.left, rectUpdate.top, 100, 100, 255);
         (dynamic_cast<::win::graphics * >(pdc))->SelectClipRgn(NULL);
         (dynamic_cast<::win::graphics * >(pdc))->SetViewportOrg(point(0, 0));
         BitBlt(hdc, rectPaint.left, rectPaint.top, 
            rectPaint.width(), rectPaint.height(),
            (HDC) pdc->get_os_data(), rectUpdate.left, rectUpdate.top,
            SRCCOPY);

      }
      catch(...)
      {
      }

      ::EndPaint(get_handle(), &paint);
      pobj->m_bRet = true;
      pbase->set_lresult(0);
   }


   void window::_001OnPrint(gen::signal_object * pobj)
   {
      SCAST_PTR(::gen::message::base, pbase, pobj);

      if(pbase->m_wparam == NULL)
         return;

      ::ca::graphics_sp graphics(get_app());
      WIN_DC(graphics.m_p)->Attach((HDC) pbase->m_wparam);
      rect rectx;
      ::ca::bitmap * pbitmap = &graphics->GetCurrentBitmap();
      ::GetCurrentObject((HDC) pbase->m_wparam, OBJ_BITMAP);
//      DWORD dw = ::GetLastError();
      class size size = pbitmap->get_size();
      rectx.left = 0;
      rectx.top = 0;
      rectx.right = size.cx;
      rectx.bottom = size.cy;
      try
      {
         rect rectWindow;
         GetWindowRect(rectWindow);

         ::ca::dib_sp dib(get_app());
         if(!dib->create(rectWindow.bottom_right()))
            return;

         ::ca::graphics * pdc = dib->get_graphics();

         if(pdc->get_os_data() == NULL)
            return;

         rect rectPaint;
         rect rectUpdate;
         rectUpdate = rectWindow;
         rectPaint = rectWindow;
         rectPaint.offset(-rectPaint.top_left());
         (dynamic_cast<::win::graphics * >(pdc))->SelectClipRgn(NULL);
         if(m_pguie != NULL && m_pguie != this)
         {
            m_pguie->_001OnDeferPaintLayeredWindowBackground(pdc);
         }
         else
         {
            _001OnDeferPaintLayeredWindowBackground(pdc);
         }
         (dynamic_cast<::win::graphics * >(pdc))->SelectClipRgn(NULL);
         (dynamic_cast<::win::graphics * >(pdc))->SetViewportOrg(point(0, 0));
         _000OnDraw(pdc);
         (dynamic_cast<::win::graphics * >(pdc))->SetViewportOrg(point(0, 0));
         //(dynamic_cast<::win::graphics * >(pdc))->FillSolidRect(rectUpdate.left, rectUpdate.top, 100, 100, 255);
         (dynamic_cast<::win::graphics * >(pdc))->SelectClipRgn(NULL);
         (dynamic_cast<::win::graphics * >(pdc))->SetViewportOrg(point(0, 0));

         graphics->SelectClipRgn( NULL);
         graphics->BitBlt(rectPaint.left, rectPaint.top, 
            rectPaint.width(), rectPaint.height(),
            pdc, rectUpdate.left, rectUpdate.top,
            SRCCOPY);

         graphics->TextOut(0, 0, "Te Amo TBS", 11);
      }
      catch(...)
      {
      }
      graphics->FillSolidRect(rectx, RGB(255, 255, 255));
      WIN_DC(graphics.m_p)->Detach();
      pobj->m_bRet = true;
      pbase->set_lresult(0);
   }


   void window::OnEnterIdle(UINT /*nWhy*/, ::ca::window * /*pWho*/)
   {
      // In some OLE inplace active scenarios, OLE will post a
      // message instead of sending it.  This causes so many WM_ENTERIDLE
      // messages to be sent that tasks running in the background stop
      // running.  By dispatching the pending WM_ENTERIDLE messages
      // when the first one is received, we trick Windows into thinking
      // that only one was really sent and dispatched.
      {
         MSG msg;
         while (PeekMessage(&msg, NULL, WM_ENTERIDLE, WM_ENTERIDLE, PM_REMOVE))
            DispatchMessage(&msg);
      }

      Default();
   }

   HBRUSH window::OnCtlColor(::ca::graphics *, ::ca::window * pWnd, UINT)
   {
      ASSERT(pWnd != NULL && WIN_WINDOW(pWnd)->get_handle() != NULL);
      LRESULT lResult;
      if (WIN_WINDOW(pWnd)->SendChildNotifyLastMsg(&lResult))
         return (HBRUSH)lResult;     // eat it
      return (HBRUSH)Default();
   }

   // implementation of OnCtlColor for default gray backgrounds
   //   (works for any window containing controls)
   //  return value of FALSE means caller must call DefWindowProc's default
   //  TRUE means that 'hbrGray' will be used and the appropriate text
   //    ('clrText') and background colors are set.
   BOOL PASCAL window::GrayCtlColor(HDC hDC, HWND hWnd, UINT nCtlColor,
      HBRUSH hbrGray, COLORREF clrText)
   {
      if (hDC == NULL)
      {
         // sometimes Win32 passes a NULL hDC in the WM_CTLCOLOR message.
//         TRACE(::radix::trace::category_AppMsg, 0, "Warning: hDC is NULL in window::GrayCtlColor; WM_CTLCOLOR not processed.\n");
         return FALSE;
      }

      if (hbrGray == NULL ||
         nCtlColor == CTLCOLOR_EDIT || nCtlColor == CTLCOLOR_MSGBOX ||
         nCtlColor == CTLCOLOR_SCROLLBAR)
      {
         return FALSE;
      }

      if (nCtlColor == CTLCOLOR_LISTBOX)
      {
         // only handle requests to draw the space between edit and drop button
         //  in a drop-down combo (not a drop-down list)
         if (!_AfxIsComboBoxControl(hWnd, (UINT)CBS_DROPDOWN))
            return FALSE;
      }

      // set background color and return handle to brush
      LOGBRUSH logbrush;
      VERIFY(::GetObject(hbrGray, sizeof(LOGBRUSH), (LPVOID)&logbrush));
      ::SetBkColor(hDC, logbrush.lbColor);
      if (clrText == (COLORREF)-1)
         clrText = ::GetSysColor(COLOR_WINDOWTEXT);  // normal text
      ::SetTextColor(hDC, clrText);
      return TRUE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // 'dialog data' support

   /*BOOL window::UpdateData(BOOL bSaveAndValidate)
   {
      ASSERT(::IsWindow(get_handle())); // calling UpdateData before DoModal?

      CDataExchange dx(this, bSaveAndValidate);

      // prevent control notifications from being dispatched during UpdateData
      _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
      HWND hWndOldLockout = pThreadState->m_hLockoutNotifyWindow;
      ASSERT(hWndOldLockout != get_handle());   // must not recurse
      pThreadState->m_hLockoutNotifyWindow = get_handle();

      BOOL bOK = FALSE;       // assume failure
      try
      {
         DoDataExchange(&dx);
         bOK = TRUE;         // it worked
      }
      catch(user_exception * pe)
      {
         // validation failed - ::fontopus::user already alerted, fall through
         ASSERT(!bOK);                                 
         // Note: DELETE_EXCEPTION_(e) not required
      }
      catch(base_exception * pe)
      {
         // validation failed due to OOM or other resource failure
         //e->ReportError(MB_ICONEXCLAMATION, AFX_IDP_INTERNAL_FAILURE);
         pe->ReportError(MB_ICONEXCLAMATION, "falha interna");
         ASSERT(!bOK);
         pe->Delete();
      }
      

      pThreadState->m_hLockoutNotifyWindow = hWndOldLockout;
      return bOK;
   }*/


   /////////////////////////////////////////////////////////////////////////////
   // Centering dialog support (works for any non-child window)

   void window::CenterWindow(::user::interaction * pAlternateOwner)
   {
      ASSERT(::IsWindow(get_handle()));

      // determine owner window to center against
      DWORD dwStyle = GetStyle();
      ::user::interaction * hWndCenter = pAlternateOwner;
      if (pAlternateOwner == NULL)
      {
         if (dwStyle & WS_CHILD)
            hWndCenter = GetParent();
         else
            hWndCenter = GetWindow(GW_OWNER);
         if (hWndCenter != NULL)
         {
            // let parent determine alternate center window
            ::user::interaction * hWndTemp =
               (::user::interaction * )hWndCenter->SendMessage(WM_QUERYCENTERWND, 0, 0);
            if (hWndTemp != NULL)
               hWndCenter = hWndTemp;
         }
      }

      // get coordinates of the window relative to its parent
      rect rcDlg;
      GetWindowRect(&rcDlg);
      rect rcArea;
      rect rcCenter;
      ::user::interaction * hWndParent;
      if (!(dwStyle & WS_CHILD))
      {
         // don't center against invisible or minimized windows
         if (hWndCenter != NULL)
         {
            DWORD dwAlternateStyle = hWndCenter->GetWindowLong(GWL_STYLE);
            if (!(dwAlternateStyle & WS_VISIBLE) || (dwAlternateStyle & WS_MINIMIZE))
               hWndCenter = NULL;
         }

          MONITORINFO mi;
         mi.cbSize = sizeof(mi);

         // center within appropriate monitor coordinates
         if (hWndCenter == NULL)
         {
            HWND hwDefault = System.GetMainWnd()->_get_handle();

            GetMonitorInfo(
               MonitorFromWindow(hwDefault, MONITOR_DEFAULTTOPRIMARY), &mi);
            rcCenter = mi.rcWork;
            rcArea = mi.rcWork;
         }
         else
         {
            hWndCenter->GetWindowRect(&rcCenter);
            GetMonitorInfo(
               MonitorFromWindow(hWndCenter->_get_handle(), MONITOR_DEFAULTTONEAREST), &mi);
            rcArea = mi.rcWork;
         }
      }
      else
      {
         // center within parent client coordinates
         hWndParent = GetParent();
         ASSERT(hWndParent->IsWindow());

         hWndParent->GetClientRect(&rcArea);
         ASSERT(hWndCenter->IsWindow());
         hWndCenter->GetClientRect(&rcCenter);
         ::MapWindowPoints(hWndCenter->_get_handle(), hWndParent->_get_handle(), (POINT*)&rcCenter, 2);
      }

      // find dialog's upper left based on rcCenter
      int xLeft = (rcCenter.left + rcCenter.right) / 2 - rcDlg.width() / 2;
      int yTop = (rcCenter.top + rcCenter.bottom) / 2 - rcDlg.height() / 2;

      // if the dialog is outside the screen, move it inside
      if (xLeft < rcArea.left)
         xLeft = rcArea.left;
      else if (xLeft + rcDlg.width() > rcArea.right)
         xLeft = rcArea.right - rcDlg.width();

      if (yTop < rcArea.top)
         yTop = rcArea.top;
      else if (yTop + rcDlg.height() > rcArea.bottom)
         yTop = rcArea.bottom - rcDlg.height();

      // ::collection::map screen coordinates to child coordinates
      SetWindowPos(NULL, xLeft, yTop, -1, -1,
         SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
   }

   BOOL window::CheckAutoCenter()
   {
      return TRUE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Dialog initialization support

   BOOL window::ExecuteDlgInit(const char * lpszResourceName)
   {
      // find resource handle
      LPVOID lpResource = NULL;
      HGLOBAL hResource = NULL;
      if (lpszResourceName != NULL)
      {
//         HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_DLGINIT);
  //       HRSRC hDlgInit = ::FindResource(hInst, lpszResourceName, RT_DLGINIT);
    /*     if (hDlgInit != NULL)
         {
            // load it
            hResource = LoadResource(hInst, hDlgInit);
            if (hResource == NULL)
               return FALSE;
            // lock it
            lpResource = LockResource(hResource);
            ASSERT(lpResource != NULL);
         }*/
      }

      // execute it
      BOOL bResult = ExecuteDlgInit(lpResource);

      // cleanup
      if (lpResource != NULL && hResource != NULL)
      {
         UnlockResource(hResource);
         FreeResource(hResource);
      }
      return bResult;
   }

   BOOL window::ExecuteDlgInit(LPVOID lpResource)
   {
      BOOL bSuccess = TRUE;
      if (lpResource != NULL)
      {
         UNALIGNED WORD* lpnRes = (WORD*)lpResource;
         while (bSuccess && *lpnRes != 0)   
         {
            WORD nIDC = *lpnRes++;
            WORD nMsg = *lpnRes++;
            DWORD dwLen = *((UNALIGNED DWORD*&)lpnRes)++;

            // In Win32 the WM_ messages have changed.  They have
            // to be translated from the 32-bit values to 16-bit
            // values here.

            #define WIN16_LB_ADDSTRING  0x0401
            #define WIN16_CB_ADDSTRING  0x0403
            #define AFX_CB_ADDSTRING   0x1234

            // unfortunately, WIN16_CB_ADDSTRING == CBEM_INSERTITEM
            if (nMsg == AFX_CB_ADDSTRING)
               nMsg = CBEM_INSERTITEM;
            else if (nMsg == WIN16_LB_ADDSTRING)
               nMsg = LB_ADDSTRING;
            else if (nMsg == WIN16_CB_ADDSTRING)
               nMsg = CB_ADDSTRING;

            // check for invalid/unknown message types
            ASSERT(nMsg == LB_ADDSTRING || nMsg == CB_ADDSTRING ||
               nMsg == CBEM_INSERTITEM);

   #ifdef _DEBUG
            // For AddStrings, the count must exactly delimit the
            // string, including the NULL termination.  This check
            // will not catch all mal-formed ADDSTRINGs, but will
            // catch some.
            if (nMsg == LB_ADDSTRING || nMsg == CB_ADDSTRING || nMsg == CBEM_INSERTITEM)
               ASSERT(*((LPBYTE)lpnRes + (UINT)dwLen - 1) == 0);
   #endif

            if (nMsg == CBEM_INSERTITEM)
            {
               COMBOBOXEXITEM item = {0};
               item.mask = CBEIF_TEXT;
               item.iItem = -1;
               string strText(reinterpret_cast<LPTSTR>(lpnRes));            
               item.pszText = const_cast<LPTSTR>(strText.GetString());
               if (::SendDlgItemMessage(get_handle(), nIDC, nMsg, 0, (LPARAM) &item) == -1)
                  bSuccess = FALSE;
            }
            {
               // List/Combobox returns -1 for error
               if (::SendDlgItemMessageA(get_handle(), nIDC, nMsg, 0, (LPARAM) lpnRes) == -1)
                  bSuccess = FALSE;
            }


            // skip past data
            lpnRes = (WORD*)((LPBYTE)lpnRes + (UINT)dwLen);
         }
      }

      // send update message to all controls after all other siblings loaded
      if (bSuccess)
         SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, FALSE, FALSE);

      return bSuccess;
   }

   void window::UpdateDialogControls(command_target* pTarget, BOOL bDisableIfNoHndler)
   {
      UNREFERENCED_PARAMETER(pTarget);
      UNREFERENCED_PARAMETER(bDisableIfNoHndler);
      cmd_ui state(get_app());
      window wndTemp;       // very temporary window just for CmdUI update

      // walk all the kids - assume the IDs are for buttons
   /* xxx   for (HWND hWndChild = ::GetTopWindow(get_handle()); hWndChild != NULL;
            hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
      {
         // send to buttons
         wndTemp.set_handle(hWndChild); // quick and dirty attach
         state.m_nID = _AfxGetDlgCtrlID(hWndChild);
         state.m_pOther = &wndTemp;

         // check for reflect handlers in the child window
         ::ca::window * pWnd = ::win::window::FromHandlePermanent(hWndChild);
         if (pWnd != NULL)
         {
            // call it directly to disable any routing
            if (WIN_WINDOW(pWnd)->window::_001OnCommand(0, MAKELONG(0xffff,
               WM_COMMAND+WM_REFLECT_BASE), &state, NULL))
               continue;
         }

         // check for handlers in the parent window
         if (window::_001OnCommand((UINT)state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
            continue;

         // determine whether to disable when no handler exists
         BOOL bDisableTemp = bDisableIfNoHndler;
         if (bDisableTemp)
         {
            if ((wndTemp.SendMessage(WM_GETDLGCODE) & DLGC_BUTTON) == 0)
            {
               // non-button controls don't get automagically disabled
               bDisableTemp = FALSE;
            }
            else
            {
               // only certain button controls get automagically disabled
               UINT nStyle = (UINT)(wndTemp.GetStyle() & 0x0F);
               if (nStyle == (UINT)BS_AUTOCHECKBOX ||
                  nStyle == (UINT)BS_AUTO3STATE ||
                  nStyle == (UINT)BS_GROUPBOX ||
                  nStyle == (UINT)BS_AUTORADIOBUTTON)
               {
                  bDisableTemp = FALSE;
               }
            }
         }
         // check for handlers in the target (owner)
         state.DoUpdate(pTarget, bDisableTemp);
      }
      wndTemp.set_handle(NULL);      // quick and dirty detach */
   }


   id window::RunModalLoop(DWORD dwFlags, ::ca::live_object * pliveobject)
   {
      // for tracking the idle time state
      BOOL bIdle = TRUE;
      LONG lIdleCount = 0;
      BOOL bShowIdle = (dwFlags & MLF_SHOWONIDLE) && !(GetStyle() & WS_VISIBLE);
      HWND hWndParent = ::GetParent(get_handle());
      m_iModal = m_iModalCount;
      int iLevel = m_iModal;
      oprop(string("RunModalLoop.thread(") + gen::str::itoa(iLevel) + ")") = System.GetThread();
      m_iModalCount++;

      m_iaModalThread.add(::GetCurrentThreadId());
      ::radix::application * pappThis1 = dynamic_cast < ::radix::application * > (m_pthread->m_p);
      ::radix::application * pappThis2 = dynamic_cast < ::radix::application * > (m_pthread);
      // acquire and dispatch messages until the modal state is done
      MSG msg;
      for (;;)
      {
         ASSERT(ContinueModal(iLevel));

         // phase1: check to see if we can do idle work
         while (bIdle && !::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
         {
            ASSERT(ContinueModal(iLevel));

            // show the dialog when the message queue goes idle
            if (bShowIdle)
            {
               ShowWindow(SW_SHOWNORMAL);
               UpdateWindow();
               bShowIdle = FALSE;
            }

            // call on_idle while in bIdle state
            if (!(dwFlags & MLF_NOIDLEMSG) && hWndParent != NULL && lIdleCount == 0)
            {
               // send WM_ENTERIDLE to the parent
               ::SendMessage(hWndParent, WM_ENTERIDLE, MSGF_DIALOGBOX, (LPARAM)get_handle());
            }
            if ((dwFlags & MLF_NOKICKIDLE) ||
               !AfxCallWndProc(this, get_handle(), WM_KICKIDLE, MSGF_DIALOGBOX, lIdleCount++))
            {
               // stop idle processing next time
               bIdle = FALSE;
            }

            m_pthread->m_p->m_dwAlive = m_pthread->m_dwAlive = ::GetTickCount();
            if(pappThis1 != NULL)
            {
               pappThis1->m_dwAlive = m_pthread->m_dwAlive;
            }
            if(pappThis2 != NULL)
            {
               pappThis2->m_dwAlive = m_pthread->m_dwAlive;
            }
            if(pliveobject != NULL)
            {
               pliveobject->keep_alive();
            }
         }


         // phase2: pump messages while available
         do
         {
            if (!ContinueModal(iLevel))
               goto ExitModal;

            // pump message, but quit on WM_QUIT
            if (!m_pthread->pump_message())
            {
               AfxPostQuitMessage(0);
               return -1;
            }

            // show the window when certain special messages rec'd
            if (bShowIdle &&
               (msg.message == 0x118 || msg.message == WM_SYSKEYDOWN))
            {
               ShowWindow(SW_SHOWNORMAL);
               UpdateWindow();
               bShowIdle = FALSE;
            }

            if (!ContinueModal(iLevel))
               goto ExitModal;

            // reset "no idle" state after pumping "normal" message
            if (AfxIsIdleMessage(&msg))
            {
               bIdle = TRUE;
               lIdleCount = 0;
            }

            m_pthread->m_p->m_dwAlive = m_pthread->m_dwAlive = ::GetTickCount();
            if(pappThis1 != NULL)
            {
               pappThis1->m_dwAlive = m_pthread->m_dwAlive;
            }
            if(pappThis2 != NULL)
            {
               pappThis2->m_dwAlive = m_pthread->m_dwAlive;
            }
            if(pliveobject != NULL)
            {
               pliveobject->keep_alive();
            }

/*            if(pliveobject != NULL)
            {
               pliveobject->keep();
            }*/

         } 
         while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE) != FALSE);

      
         if(m_pguie->m_pthread != NULL)
         {
            m_pguie->m_pthread->step_timer();
         }
         if (!ContinueModal(iLevel))
            goto ExitModal;


      }

ExitModal:
      m_iaModalThread.remove_first(::GetCurrentThreadId());
      m_iModal = m_iModalCount;
      return m_nModalResult;
   }

   bool window::ContinueModal(int iLevel)
   {
      return iLevel < m_iModalCount;
   }

   void window::EndModalLoop(id nResult)
   {
      ASSERT(::IsWindow(get_handle()));

      // this result will be returned from window::RunModalLoop
      m_nModalResult = nResult;

      // make sure a message goes through to exit the modal loop
      if(m_iModalCount > 0)
      {
         m_iModalCount--;
         for(index i = 0; i < m_iaModalThread.get_count(); i++)
         {
            ::PostThreadMessage(m_iaModalThread[i], WM_NULL, 0, 0);
         }
         PostMessage(WM_NULL);
         System.GetThread()->PostThreadMessage(WM_NULL, 0, 0);
      }
   }

   void window::EndAllModalLoops(id nResult)
   {
      ASSERT(::IsWindow(get_handle()));

      // this result will be returned from window::RunModalLoop
      m_idModalResult = nResult;

      // make sure a message goes through to exit the modal loop
      if(m_iModalCount > 0)
      {
         int iLevel = m_iModalCount - 1;
         m_iModalCount = 0;
         PostMessage(WM_NULL);
         System.GetThread()->PostThreadMessage(WM_NULL, 0, 0);
         for(int i = iLevel; i >= 0; i--)
         {
            ::ca::thread * pthread = oprop(string("RunModalLoop.thread(") + gen::str::itoa(i) + ")").ca2 < ::ca::thread > ();
            try
            {
               pthread->PostThreadMessage(WM_NULL, 0, 0);
            }
            catch(...)
            {
            }
         }
      }
   }


   /////////////////////////////////////////////////////////////////////////////
   // frame_window (here for library granularity)

   BOOL window::IsFrameWnd()
   {
      return FALSE;
   }
   /////////////////////////////////////////////////////////////////////////////
   // Extra window support for dynamic subclassing of controls

   BOOL window::SubclassWindow(HWND hWnd)
   {
      if (!Attach(hWnd))
         return FALSE;

      // allow any other subclassing to occur
      pre_subclass_window();

      m_pfnSuper = (WNDPROC)::GetWindowLongPtr(hWnd, GWLP_WNDPROC);

      // now hook into the AFX WndProc
      WNDPROC* lplpfn = GetSuperWndProcAddr();
      WNDPROC oldWndProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC,
         (INT_PTR)AfxGetAfxWndProc());
      ASSERT(oldWndProc != AfxGetAfxWndProc());

      if (*lplpfn == NULL)
         *lplpfn = oldWndProc;   // the first control of that type created
   #ifdef _DEBUG
      else if (*lplpfn != oldWndProc)
      {
         TRACE(::radix::trace::category_AppMsg, 0, "Error: Trying to use SubclassWindow with incorrect window\n");
         TRACE(::radix::trace::category_AppMsg, 0, "\tderived class.\n");
         TRACE(::radix::trace::category_AppMsg, 0, "\thWnd = $%08X (nIDC=$%08X) is not a %hs.\n", (UINT)(UINT_PTR)hWnd,
            _AfxGetDlgCtrlID(hWnd), typeid(*this).name());
         ASSERT(FALSE);
         // undo the subclassing if continuing after assert
        ::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (INT_PTR)oldWndProc);
      }
   #endif
      ::gen::message::size size(get_app());
      _001OnSize(&size);
      return TRUE;
   }

   BOOL window::SubclassDlgItem(UINT nID, ::ca::window * pParent)
   {
      ASSERT(pParent != NULL);
      ASSERT(::IsWindow(WIN_WINDOW(pParent)->get_handle()));

      // check for normal dialog control first
      HWND hWndControl = ::GetDlgItem(WIN_WINDOW(pParent)->get_handle(), nID);
      if (hWndControl != NULL)
         return SubclassWindow(hWndControl);


      return FALSE;   // control not found
   }

   HWND window::UnsubclassWindow()
   {
      ASSERT(::IsWindow(get_handle()));

      // set WNDPROC back to original value
      WNDPROC* lplpfn = GetSuperWndProcAddr();
      SetWindowLongPtr(get_handle(), GWLP_WNDPROC, (INT_PTR)*lplpfn);
      *lplpfn = NULL;

      // and Detach the HWND from the window object
      return Detach();
   }


/*   view_update_hint::view_update_hint(::ca::application * papp) :
      ca(papp)
   {
   }
*/



   BOOL window::IsChild(::user::interaction * pWnd)
   {
      ASSERT(::IsWindow(get_handle())); 
      if(WIN_WINDOW(pWnd)->_get_handle() == NULL)
      {
         return ::user::interaction::IsChild(pWnd);
      }
      else
      {
         return ::IsChild(get_handle(), WIN_WINDOW(pWnd)->_get_handle());
      }
   }

   BOOL window::IsWindow()
   {
      return ::IsWindow(get_handle());
   }

   HWND window::_get_handle()
   {
      return get_handle();
   }

   bool window::SetWindowPos(int z, int x, int y, int cx, int cy, UINT nFlags)
   {
      /*bool b;
      bool * pb = &b;
      if(m_papp->s_ptwf != NULL)
         pb = &m_papp->s_ptwf->m_bProDevianMode;
      keeper < bool > keepOnDemandDraw(pb, false, *pb, true);
      */
      ASSERT(::IsWindow(get_handle())); 
   /*   return ::SetWindowPos(get_handle(), pWndInsertAfter->get_handle(),
         x, y, cx, cy, nFlags) != FALSE; */
      rect64 rectWindowOld = m_rectParentClient;
      if(nFlags & SWP_NOMOVE)
      {
         if(nFlags & SWP_NOSIZE)
         {
         }
         else
         {
            m_rectParentClient.right   = m_rectParentClient.left + cx;
            m_rectParentClient.bottom  = m_rectParentClient.top + cy;
         }
      }
      else
      {
         if(nFlags & SWP_NOSIZE)
         {
            m_rectParentClient.offset(x - m_rectParentClient.left, y - m_rectParentClient.top);
         }
         else
         {
            m_rectParentClient.left    = x;
            m_rectParentClient.top     = y;
            m_rectParentClient.right   = m_rectParentClient.left + cx;
            m_rectParentClient.bottom  = m_rectParentClient.top + cy;
         }
      }
      if(m_pguie != this
      && m_pguie != NULL)
      {
         m_pguie->m_rectParentClient = m_rectParentClient;
      }

      if(GetExStyle() & WS_EX_LAYERED)
      {
         if(rectWindowOld.top_left() != m_rectParentClient.top_left())
         {
            SendMessage(WM_MOVE, 0, 0);
         }
         if(rectWindowOld.size() != m_rectParentClient.size())
         {
            SendMessage(WM_SIZE, 0, 0);
         }

      }
      if(GetExStyle() & WS_EX_LAYERED)
      {
         nFlags |= SWP_NOCOPYBITS;
         nFlags |= SWP_NOREDRAW;
         nFlags |= SWP_NOMOVE;
         nFlags |= SWP_NOSIZE;
         nFlags |= SWP_NOZORDER;
         //nFlags |= SWP_FRAMECHANGED;
         if(nFlags & SWP_SHOWWINDOW)
         {
            ::SetWindowPos(get_handle(), (HWND) z, x, y, cx, cy, nFlags);
            ShowWindow(SW_SHOW);
         }
         else
         {
            ::SetWindowPos(get_handle(), (HWND) z, x, y, cx, cy, nFlags);
         }
         /*if(m_pguie != NULL)
         {
            m_pguie->oprop("pending_layout") = true;
            m_pguie->oprop("pending_zorder") = z;
         }*/
         /*if(&System != NULL && System.get_twf() != NULL)
         {
            System.get_twf()->synch_redraw();
         }*/
      }
      else
      {
         if(z == -3)
         {
            ::SetWindowPos(get_handle(), (HWND) 0, x, y, cx, cy, nFlags);
         }
         else
         {
            ::SetWindowPos(get_handle(), (HWND) z, x, y, cx, cy, nFlags);
         }
      }
      return true;

   }

   void window::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
   {
      ASSERT(::IsWindow(get_handle())); 
      SetWindowPos(NULL, x, y, nWidth, nHeight, bRepaint ? SWP_SHOWWINDOW : 0);
   }

   void window::ClientToScreen(LPRECT lprect)
   {

      class rect64 rectWindow;
      m_pguie->GetWindowRect(rectWindow);

      lprect->left   += (LONG) rectWindow.left;
      lprect->right  += (LONG) rectWindow.left;
      lprect->top    += (LONG) rectWindow.top;
      lprect->bottom += (LONG) rectWindow.top;

   }

   void window::ClientToScreen(LPPOINT lppoint)
   {
      class rect64 rectWindow;
      m_pguie->GetWindowRect(rectWindow);

      lppoint->x     += (LONG) rectWindow.left;
      lppoint->y     += (LONG) rectWindow.top;
   }


   void window::ClientToScreen(__rect64 * lprect)
   {
      class rect rectWindow;
      m_pguie->GetWindowRect(rectWindow);

      lprect->left   += rectWindow.left;
      lprect->right  += rectWindow.left;
      lprect->top    += rectWindow.top;
      lprect->bottom += rectWindow.top;

   }

   void window::ClientToScreen(__point64 * lppoint)
   {
      class rect64 rectWindow;
      m_pguie->GetWindowRect(rectWindow);

      lppoint->x     += rectWindow.left;
      lppoint->y     += rectWindow.top;
   }


   void window::ScreenToClient(LPRECT lprect)
   {
      class rect64 rectWindow;
      m_pguie->GetWindowRect(rectWindow);

      lprect->left   -= (LONG) rectWindow.left;
      lprect->right  -= (LONG) rectWindow.left;
      lprect->top    -= (LONG) rectWindow.top;
      lprect->bottom -= (LONG) rectWindow.top;

   }

   void window::ScreenToClient(LPPOINT lppoint)
   {
      class rect64 rectWindow;
      m_pguie->GetWindowRect(rectWindow);

      lppoint->x     -= (LONG) rectWindow.left;
      lppoint->y     -= (LONG) rectWindow.top;
   }


   void window::ScreenToClient(__rect64 * lprect)
   {
      class rect64 rectWindow;
      m_pguie->GetWindowRect(rectWindow);

      lprect->left   -= rectWindow.left;
      lprect->right  -= rectWindow.left;
      lprect->top    -= rectWindow.top;
      lprect->bottom -= rectWindow.top;

   }

   void window::ScreenToClient(__point64 * lppoint)
   {
      class rect64 rectWindow;
      m_pguie->GetWindowRect(rectWindow);

      lppoint->x     -= rectWindow.left;
      lppoint->y     -= rectWindow.top;
   }


   void window::GetWindowRect(__rect64 * lprect)
   {
      ASSERT(::IsWindow(get_handle()));
      // if it is temporary window - probably not ca2 wrapped window
      if(m_pguie == NULL || m_pguie == this)
      {
         rect rect32;
         ::GetWindowRect(get_handle(), rect32);
         ::copy(lprect, rect32);
      }
      else
      {
         interaction::GetWindowRect(lprect);
      }
   }

   void window::GetClientRect(__rect64 * lprect)
   {
      ASSERT(::IsWindow(get_handle())); 
      // if it is temporary window - probably not ca2 wrapped window
      if(m_pguie == NULL || m_pguie == this)
      {
         rect rect32;
         ::GetClientRect(get_handle(), rect32);
         ::copy(lprect, rect32);
      }
      else
      {
         interaction::GetClientRect(lprect);
      }
   }

   id window::SetDlgCtrlId(id id)
   {
      m_id = id;
      return m_id;
   }

   id window::GetDlgCtrlId()
   {
      return m_id;
   }

/*   guie_message_wnd::guie_message_wnd(::ca::application * papp) :
      ca(papp)
   {
      m_pguieForward = NULL;
   }

   LRESULT guie_message_wnd::message_handler(gen::signal_object * pobj)
   {
      if(m_pguieForward != NULL)
      {
         return m_pguieForward->message_handler(uiMessage, wparam, lparam);
      }
      else
      {
         return 0;
      }
   }*/

   void window::_001WindowMaximize()
   {
      ::user::interaction::_001WindowMaximize();
   }

   void window::_001WindowRestore()
   {
      m_eappearance = appearance_normal;
      if(m_pguie != NULL)
         m_pguie->m_eappearance = appearance_normal;
      ::ShowWindow(get_handle(), SW_RESTORE);
   }

   BOOL window::ShowWindow(int nCmdShow)
   {
      if(!::IsWindow(get_handle()))
         return false;
      if(GetExStyle() & WS_EX_LAYERED)
      {
         if(nCmdShow == SW_HIDE)
         {
            ModifyStyle(get_handle(), WS_VISIBLE, 0, 0);
         }
         else
         {
            ModifyStyle(get_handle(), 0, WS_VISIBLE, 0);
         }
         if(nCmdShow == SW_MAXIMIZE)
         {
            _001WindowMaximize();
         }
         else if(nCmdShow == SW_RESTORE)
         {
            _001WindowRestore();
         }
         else
         {
            if(nCmdShow == SW_MINIMIZE)
            {
               m_pguie->m_eappearance = appearance_iconic;
               m_eappearance = appearance_iconic;
            }
            ::ShowWindow(get_handle(), nCmdShow);
         }
         m_bVisible = ::IsWindowVisible(get_handle()) != FALSE;
         if(m_pguie!= NULL && m_pguie != this)
            m_pguie->m_bVisible = m_bVisible;
         if(!m_bVisible || IsIconic())
         {
            ::UpdateLayeredWindow(get_handle(), NULL, NULL, NULL, NULL, NULL, 0, NULL, 0);
         }
         return m_bVisible;
      }
      else
      {
         ::ShowWindow(get_handle(), nCmdShow);
         m_bVisible = ::IsWindowVisible(get_handle()) != FALSE;
         if(m_pguie!= NULL && m_pguie != this)
            m_pguie->m_bVisible = m_bVisible;
         return m_bVisible;
      }
   }


   bool window::IsIconic()
   {
      ASSERT(::IsWindow(get_handle())); 
      if(GetExStyle() & WS_EX_LAYERED)
      {
         return m_pguie->m_eappearance == appearance_iconic;
      }
      else
      {
         return ::IsIconic(get_handle()) != FALSE;
      }
   }

   bool window::IsZoomed()
   {
      ASSERT(::IsWindow(get_handle())); 
      return m_pguie->m_eappearance == appearance_zoomed;
   }


   ::user::interaction * window::GetParent()
   {
      if(!::IsWindow(get_handle()))
         return NULL;
      if(get_handle() == NULL)
         return NULL;
      return ::win::window::from_handle(::GetParent(get_handle()));
   }

   LONG window::GetWindowLong(int nIndex)
   {
      return ::GetWindowLong(get_handle(), nIndex);
   }

   LONG window::SetWindowLong(int nIndex, LONG lValue)
   {
      return ::SetWindowLong(get_handle(), nIndex, lValue);
   }


   ::user::interaction * window::release_capture()
   {
      HWND hwndCapture = ::GetCapture();
      if(hwndCapture == NULL)
         return NULL;
      if(hwndCapture == get_handle())
      {
         ::user::interaction * puieCapture = get_capture();
         if(::ReleaseCapture())
         {
            m_pguieCapture = NULL;
            return puieCapture;
         }
         else
         {
            return NULL;
         }
      }
      else
      {
         return window::GetCapture()->release_capture();
      }
   }

   ::user::interaction * window::get_capture()
   {
      HWND hwndCapture = ::GetCapture();
      if(hwndCapture == NULL)
         return NULL;
      if(hwndCapture == get_handle())
      {
         if(m_pguieCapture != NULL)
         {
            return m_pguieCapture;
         }
         else
         {
            if(m_pguie != NULL)
            {
               if(m_pguie->get_wnd() != NULL && WIN_WINDOW(m_pguie->get_wnd())->m_pguieCapture != NULL)
               {
                  return WIN_WINDOW(m_pguie->get_wnd())->m_pguieCapture;
               }
               else
               {
                  return m_pguie;
               }
            }
            else
            {
               return this;
            }
         }
      }
      else
      {
         return window::GetCapture()->get_capture();
      }
   }



   // window
   /* window::operator HWND() const
      { return this == NULL ? NULL : get_handle(); }*/
   BOOL window::operator==(const ::ca::window& wnd) const
   { 
      return ((HWND) WIN_WINDOW(const_cast < ::ca::window * > (&wnd))->get_handle()) == get_handle(); 
   }

   BOOL window::operator!=(const ::ca::window& wnd) const
   { 
      return ((HWND) WIN_WINDOW(const_cast < ::ca::window * > (&wnd))->get_handle()) != get_handle(); 
   }

   DWORD window::GetStyle()
   {
      ASSERT(::IsWindow(get_handle())); 
      return (DWORD)::GetWindowLong(get_handle(), GWL_STYLE); 
   }
   
   DWORD window::GetExStyle()
   {
      ASSERT(::IsWindow(get_handle())); 
      return (DWORD)::GetWindowLong(get_handle(), GWL_EXSTYLE); 
   }

   BOOL window::ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
   { 
      ASSERT(::IsWindow(get_handle())); 
      return ModifyStyle(get_handle(), dwRemove, dwAdd, nFlags); 
   }

   BOOL window::ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
   { 
      ASSERT(::IsWindow(get_handle())); 
      return ModifyStyleEx(get_handle(), dwRemove, dwAdd, nFlags); 
   }

   void window::SetOwner(::user::interaction * pOwnerWnd)
   { 
      m_pguieOwner = pOwnerWnd; 
   }

   LRESULT window::_AFX_FUNCNAME(SendMessage)(UINT message, WPARAM wParam, LPARAM lParam)
   {
      //ASSERT(::IsWindow(get_handle())); 
      return ::SendMessage(get_handle(), message, wParam, lParam);
   }
   
#pragma push_macro("SendMessage")
#undef SendMessage
   LRESULT window::SendMessage(UINT message, WPARAM wParam, LPARAM lParam)
   { 
      return _AFX_FUNCNAME(SendMessage)(message, wParam, lParam); 
   }

#pragma pop_macro("SendMessage")
   BOOL window::PostMessage(UINT message, WPARAM wParam, LPARAM lParam)
   { 
      return ::PostMessage(get_handle(), message, wParam, lParam); 
   }
   
   BOOL window::DragDetect(POINT pt) const
   { 
      ASSERT(::IsWindow(get_handle())); 
      return ::DragDetect(get_handle(), pt); 
   }

   void window::SetWindowText(const char * lpszString)
   { 
      m_strWindowText = lpszString;
   }

   int window::GetWindowText(__out_ecount_part_z(nMaxCount, return + 1) LPTSTR lpszString, __in int nMaxCount)
   { 
      strncpy(lpszString, m_strWindowText, nMaxCount);
      return min(nMaxCount, m_strWindowText.get_length());
   }
   
   int window::GetWindowTextLength()
   { 
      ASSERT(::IsWindow(get_handle())); 
      return ::GetWindowTextLength(get_handle()); 
   }

   void window::SetFont(::ca::font* pfont, BOOL bRedraw)
   { 
      UNREFERENCED_PARAMETER(bRedraw);
      ASSERT(::IsWindow(get_handle())); m_pfont = new ::ca::font(*pfont); 
   }

   ::ca::font* window::GetFont()
   { 
      ASSERT(::IsWindow(get_handle())); 
      return m_pfont; 
   }

   void window::DragAcceptFiles(BOOL bAccept)
   { 
      ASSERT(::IsWindow(get_handle())); 
      ::DragAcceptFiles(get_handle(), bAccept); 
   }

   frame_window * window::EnsureParentFrame()
   {
      frame_window * pFrameWnd=GetParentFrame();
      ENSURE_VALID(pFrameWnd);
      return pFrameWnd;
   }
   
   ::user::interaction* window::EnsureTopLevelParent()
   {
       ::user::interaction *pWnd=GetTopLevelParent();
       ENSURE_VALID(pWnd);
       return pWnd;
   }
   
   void window::MoveWindow(LPCRECT lpRect, BOOL bRepaint)
   {
      MoveWindow(lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, bRepaint); 
   }
   
   UINT window::ArrangeIconicWindows()
   {
      ASSERT(::IsWindow(get_handle())); return ::ArrangeIconicWindows(get_handle()); 
   }

   int window::SetWindowRgn(HRGN hRgn, BOOL bRedraw)
   {
      ASSERT(::IsWindow(get_handle())); return ::SetWindowRgn(get_handle(), hRgn, bRedraw); 
   }
   
   int window::GetWindowRgn(HRGN hRgn)
   {
      ASSERT(::IsWindow(get_handle()) && hRgn != NULL); return ::GetWindowRgn(get_handle(), hRgn); 
   }

   bool window::BringWindowToTop()
   {

      return ::BringWindowToTop(get_handle()) != FALSE; 

   }
   
   void window::MapWindowPoints(::ca::window * pwndTo, LPPOINT lpPoint, UINT nCount)
   {
      ASSERT(::IsWindow(get_handle())); 
      ::MapWindowPoints(get_handle(), (HWND) pwndTo->get_os_data(), lpPoint, nCount); 
   }

   void window::MapWindowPoints(::ca::window * pwndTo, LPRECT lpRect)
   { 
      ASSERT(::IsWindow(get_handle())); 
      ::MapWindowPoints(get_handle(), (HWND) pwndTo->get_os_data(), (LPPOINT)lpRect, 2); 
   }
   
   ::ca::graphics * window::GetDC()
   { 
      ::ca::graphics_sp g(get_app());
      if(get_handle() == NULL)
      {
         (dynamic_cast < ::win::graphics * >(g.m_p))->Attach(::GetDC(NULL));
      }
      else
      {
         (dynamic_cast < ::win::graphics * >(g.m_p))->Attach(::GetDC(get_handle()));
      }
      return g.detach();
   }
   
   ::ca::graphics * window::GetWindowDC()
   {
      ASSERT(::IsWindow(get_handle())); 
      ::ca::graphics_sp g(get_app());
      g->attach(::GetWindowDC(get_handle()));
      return g.detach();
   }

   int window::ReleaseDC(::ca::graphics * pgraphics)
   { 
      if(get_handle() == NULL)
      {
         ::ReleaseDC(NULL, ((Gdiplus::Graphics *)(dynamic_cast<::win::graphics * >(pgraphics))->get_os_data())->GetHDC()); 
         pgraphics->release();
      }
      else
      {
         ::ReleaseDC(get_handle(), ((Gdiplus::Graphics *)(dynamic_cast<::win::graphics * >(pgraphics))->get_os_data())->GetHDC()); 
         pgraphics->release();
      }
      return 1;
   }
   
   void window::UpdateWindow()
   {
      ::UpdateWindow(get_handle()); 
   }
   
   void window::SetRedraw(BOOL bRedraw)
   { 
      ASSERT(::IsWindow(get_handle())); 
      ::SendMessage(get_handle(), WM_SETREDRAW, bRedraw, 0); 
   }

   BOOL window::GetUpdateRect(LPRECT lpRect, BOOL bErase)
   {
      ASSERT(::IsWindow(get_handle())); 
      return ::GetUpdateRect(get_handle(), lpRect, bErase); 
   }

   int window::GetUpdateRgn(::ca::region* pRgn, BOOL bErase)
   { 
      ASSERT(::IsWindow(get_handle())); 
      return ::GetUpdateRgn(get_handle(), (HRGN)pRgn->get_os_data(), bErase); 
   }
   
   void window::Invalidate(BOOL bErase)
   { 
      ASSERT(::IsWindow(get_handle())); 
      ::InvalidateRect(get_handle(), NULL, bErase); 
   }

   void window::InvalidateRect(LPCRECT lpRect, BOOL bErase)
   {
      ASSERT(::IsWindow(get_handle())); 
      ::InvalidateRect(get_handle(), lpRect, bErase); 
   }
   
   void window::InvalidateRgn(::ca::region* pRgn, BOOL bErase)
   {
      ASSERT(::IsWindow(get_handle())); 
      ::InvalidateRgn(get_handle(), (HRGN)pRgn->get_os_data(), bErase); 
   }

    void window::ValidateRect(LPCRECT lpRect)
      { ASSERT(::IsWindow(get_handle())); ::ValidateRect(get_handle(), lpRect); }
    void window::ValidateRgn(::ca::region* pRgn)
      { ASSERT(::IsWindow(get_handle())); ::ValidateRgn(get_handle(), (HRGN)pRgn->get_os_data()); }
    
   BOOL window::IsWindowVisible()
   {

      if(!::IsWindow(get_handle()))
         return FALSE;

      if(m_pguie != NULL)
      {

         if(!m_pguie->m_bVisible)
            return FALSE;

         if(m_pguie->GetParent() != NULL && !m_pguie->GetParent()->IsWindowVisible())
            return FALSE;

      }

      if(!::IsWindowVisible(get_handle()))
         return FALSE;
      
      return TRUE;

   }


    void window::ShowOwnedPopups(BOOL bShow)
      { ASSERT(::IsWindow(get_handle())); ::ShowOwnedPopups(get_handle(), bShow); }

   void window::SendMessageToDescendants(UINT message, WPARAM wParam, LPARAM lParam, BOOL bDeep, BOOL bOnlyPerm)
   { 
      ASSERT(::IsWindow(get_handle())); 
      //window::SendMessageToDescendants(get_handle(), message, wParam, lParam, bDeep, bOnlyPerm); 
    
       // walk through HWNDs to avoid creating temporary window objects
      // unless we need to call this function recursively
      user::interaction * pui = m_pguie->get_top_child();
      while(pui != NULL)
      {
         try
         {
            pui->SendMessage(message, wParam, lParam);
         }
         catch(...)
         {
         }
         if (bDeep)
         {
            // send to child windows after parent
            try
            {
               pui->SendMessageToDescendants(message, wParam, lParam, bDeep, bOnlyPerm);
            }
            catch(...)
            {
            }
         }
         try
         {
            pui = pui->under_sibling();
         }
         catch(...)
         {
            break;
         }
      }
   }

   ::user::interaction * window::GetDescendantWindow(id id)
   {
      ASSERT(::IsWindow(get_handle())); 
      return window::GetDescendantWindow(this, id); 
   }


    ::ca::graphics * window::GetDCEx(::ca::region* prgnClip, DWORD flags)
    {
    
       ASSERT(::IsWindow(get_handle())); 
       ::ca::graphics_sp g(get_app());
       g->attach(::GetDCEx(get_handle(), (HRGN)prgnClip->get_os_data(), flags));
       return g.detach(); 
    
    }

    BOOL window::LockWindowUpdate()
    {
       ASSERT(::IsWindow(get_handle())); 
       return ::LockWindowUpdate(get_handle());
    }
    
    void window::UnlockWindowUpdate()
    {
       ASSERT(::IsWindow(get_handle())); ::LockWindowUpdate(NULL);
    }
    
    BOOL window::RedrawWindow(LPCRECT lpRectUpdate, ::ca::region* prgnUpdate, UINT flags)
    { 
       if(System.get_twf() == NULL)
          return FALSE;
       if(System.get_twf()->m_bProDevianMode)
          return TRUE;
       ASSERT(::IsWindow(get_handle())); 
       return ::RedrawWindow(get_handle(), lpRectUpdate, prgnUpdate == NULL ? NULL : (HRGN)prgnUpdate->get_os_data(), flags);
    }

    BOOL window::EnableScrollBar(int nSBFlags, UINT nArrowFlags)
      { ASSERT(::IsWindow(get_handle())); return (BOOL)::EnableScrollBar(get_handle(), nSBFlags, nArrowFlags); }
    BOOL window::DrawAnimatedRects(int idAni, CONST RECT *lprcFrom, CONST RECT *lprcTo)
      { ASSERT(::IsWindow(get_handle())); return (BOOL)::DrawAnimatedRects(get_handle(), idAni, lprcFrom, lprcTo); }
    BOOL window::DrawCaption(::ca::graphics * pgraphics, LPCRECT lprc, UINT uFlags)
      { ASSERT(::IsWindow(get_handle())); return (BOOL)::DrawCaption(get_handle(), (HDC)(dynamic_cast<::win::graphics * >(pgraphics))->get_os_data(), lprc, uFlags); }

    UINT_PTR window::SetTimer(UINT_PTR nIDEvent, UINT nElapse,
         void (CALLBACK* lpfnTimer)(HWND, UINT, UINT_PTR, DWORD))
      { ASSERT(::IsWindow(get_handle())); return ::SetTimer(get_handle(), nIDEvent, nElapse,
         lpfnTimer); }
    BOOL window::KillTimer(UINT_PTR nIDEvent)
      { ASSERT(::IsWindow(get_handle())); return ::KillTimer(get_handle(), nIDEvent); }
    BOOL window::IsWindowEnabled()
      { ASSERT(::IsWindow(get_handle())); return ::IsWindowEnabled(get_handle()); }
    BOOL window::EnableWindow(BOOL bEnable)
      { ASSERT(::IsWindow(get_handle())); return ::EnableWindow(get_handle(), bEnable); }
    ::user::interaction * window::GetActiveWindow()
      { return ::win::window::from_handle(::GetActiveWindow()); }
    ::user::interaction* window::SetActiveWindow()
      { ASSERT(::IsWindow(get_handle())); return ::win::window::from_handle(::SetActiveWindow(get_handle())); }
    ::ca::window * PASCAL window::GetCapture()
      { return ::win::window::from_handle(::GetCapture()); }

   ::user::interaction * window::set_capture(::user::interaction* pinterface)
   { 
      ASSERT(::IsWindow(get_handle())); 
      if(pinterface != NULL) 
         m_pguieCapture = pinterface; 
      return dynamic_cast < ::ca::window * > (::win::window::from_handle(::SetCapture(get_handle()))); 
   }

    ::ca::window * PASCAL window::GetFocus()
      { return ::win::window::from_handle(::GetFocus()); }
    ::ca::window * window::SetFocus()
      { ASSERT(::IsWindow(get_handle())); return ::win::window::from_handle(::SetFocus(get_handle())); }
    ::ca::window * PASCAL window::GetDesktopWindow()
      { return ::win::window::from_handle(::GetDesktopWindow()); }

// Helper for radio buttons
int window::GetCheckedRadioButton(int nIDFirstButton, int nIDLastButton)
{
   for (int nID = nIDFirstButton; nID <= nIDLastButton; nID++)
   {
      if (IsDlgButtonChecked(nID))
         return nID; // id that matched
   }
   return 0; // invalid ID
}

    void window::CheckDlgButton(int nIDButton, UINT nCheck)
      { ASSERT(::IsWindow(get_handle())); ::CheckDlgButton(get_handle(), nIDButton, nCheck); }
    void window::CheckRadioButton(int nIDFirstButton, int nIDLastButton,
         int nIDCheckButton)
      { ASSERT(::IsWindow(get_handle())); ::CheckRadioButton(get_handle(), nIDFirstButton, nIDLastButton, nIDCheckButton); }
    int window::DlgDirList(__inout_z LPTSTR lpPathSpec, __in int nIDListBox,
         __in int nIDStaticPath, __in UINT nFileType)
      { ASSERT(::IsWindow(get_handle())); return ::DlgDirList(get_handle(), lpPathSpec, nIDListBox,
            nIDStaticPath, nFileType); }
    int window::DlgDirListComboBox(__inout_z LPTSTR lpPathSpec, __in int nIDComboBox,
         __in int nIDStaticPath, __in UINT nFileType)
      { ASSERT(::IsWindow(get_handle())); return ::DlgDirListComboBox(get_handle(), lpPathSpec,
            nIDComboBox, nIDStaticPath, nFileType); }
    BOOL window::DlgDirSelect(LPTSTR lpString, int nSize, int nIDListBox)
      { ASSERT(::IsWindow(get_handle())); return ::DlgDirSelectEx(get_handle(), lpString, nSize, nIDListBox); }
    BOOL window::DlgDirSelectComboBox(LPTSTR lpString, int nSize, int nIDComboBox)
      { ASSERT(::IsWindow(get_handle())); return ::DlgDirSelectComboBoxEx(get_handle(), lpString, nSize, nIDComboBox);}
    void window::GetDlgItem(id id, HWND* phWnd) const
      { ASSERT(::IsWindow(get_handle())); ASSERT(phWnd != NULL); *phWnd = ::GetDlgItem(get_handle(), id); }
    UINT window::GetDlgItemInt(int nID, BOOL* lpTrans,
         BOOL bSigned) const
      { ASSERT(::IsWindow(get_handle())); return ::GetDlgItemInt(get_handle(), nID, lpTrans, bSigned);}
    int window::GetDlgItemText(__in int nID, __out_ecount_part_z(nMaxCount, return + 1) LPTSTR lpStr, __in int nMaxCount) const
      { ASSERT(::IsWindow(get_handle())); return ::GetDlgItemText(get_handle(), nID, lpStr, nMaxCount);}
    
    ::ca::window * window::GetNextDlgGroupItem(::ca::window * pWndCtl, BOOL bPrevious) const
    {
       ASSERT(::IsWindow(get_handle()));
       return ::win::window::from_handle(::GetNextDlgGroupItem(get_handle(), (HWND) pWndCtl->get_os_data(), bPrevious)); 
    }

    ::ca::window * window::GetNextDlgTabItem(::ca::window * pWndCtl, BOOL bPrevious) const
    {
       ASSERT(::IsWindow(get_handle())); 
       return ::win::window::from_handle(::GetNextDlgTabItem(get_handle(), (HWND) pWndCtl->get_os_data(), bPrevious)); 
    }

    UINT window::IsDlgButtonChecked(int nIDButton) const
      { ASSERT(::IsWindow(get_handle())); return ::IsDlgButtonChecked(get_handle(), nIDButton); }
    LPARAM window::SendDlgItemMessage(int nID, UINT message, WPARAM wParam, LPARAM lParam)
      { ASSERT(::IsWindow(get_handle())); return ::SendDlgItemMessage(get_handle(), nID, message, wParam, lParam); }
    void window::SetDlgItemInt(int nID, UINT nValue, BOOL bSigned)
      { ASSERT(::IsWindow(get_handle())); ::SetDlgItemInt(get_handle(), nID, nValue, bSigned); }
    void window::SetDlgItemText(int nID, const char * lpszString)
      { ASSERT(::IsWindow(get_handle())); ::SetDlgItemText(get_handle(), nID, lpszString); }
    int window::ScrollWindowEx(int dx, int dy,
      LPCRECT lpRectScroll, LPCRECT lpRectClip,
      ::ca::region* prgnUpdate, LPRECT lpRectUpdate, UINT flags)
      { ASSERT(::IsWindow(get_handle())); return ::ScrollWindowEx(get_handle(), dx, dy, lpRectScroll, lpRectClip,
            (HRGN)prgnUpdate->get_os_data(), lpRectUpdate, flags); }

    void window::ShowScrollBar(UINT nBar, BOOL bShow)
      { ASSERT(::IsWindow(get_handle())); ::ShowScrollBar(get_handle(), nBar, bShow); }
    ::ca::window * window::ChildWindowFromPoint(POINT point)
      { ASSERT(::IsWindow(get_handle())); return ::win::window::from_handle(::ChildWindowFromPoint(get_handle(), point)); }
    ::ca::window * window::ChildWindowFromPoint(POINT point, UINT nFlags)
      { ASSERT(::IsWindow(get_handle())); return ::win::window::from_handle(::ChildWindowFromPointEx(get_handle(), point, nFlags)); }
    ::ca::window * PASCAL window::FindWindow(const char * lpszClassName, const char * lpszWindowName)
      { return ::win::window::from_handle(::FindWindow(lpszClassName, lpszWindowName)); }
    ::ca::window * window::FindWindowEx(HWND hwndParent, HWND hwndChildAfter, const char * lpszClass, const char * lpszWindow)
      { return ::win::window::from_handle(::FindWindowEx(hwndParent, hwndChildAfter, lpszClass, lpszWindow)); }

    ::user::interaction* window::GetNextWindow(UINT nFlag)
      { ASSERT(::IsWindow(get_handle())); return ::win::window::from_handle(::GetNextWindow(get_handle(), nFlag)); }
    ::user::interaction* window::GetTopWindow()
      { ASSERT(::IsWindow(get_handle())); return ::win::window::from_handle(::GetTopWindow(get_handle())); }
    ::user::interaction* window::GetWindow(UINT nCmd)
      { ASSERT(::IsWindow(get_handle())); return ::win::window::from_handle(::GetWindow(get_handle(), nCmd)); }
    ::user::interaction* window::GetLastActivePopup()
      { ASSERT(::IsWindow(get_handle())); return ::win::window::from_handle(::GetLastActivePopup(get_handle())); }
    
    ::ca::window * window::SetParent(::ca::window * pWndNewParent)
    {
       ASSERT(::IsWindow(get_handle())); 
       return ::win::window::from_handle(::SetParent(get_handle(), (HWND) pWndNewParent->get_os_data())); 
    }

    ::ca::window * PASCAL window::WindowFromPoint(POINT point)
      { return ::win::window::from_handle(::WindowFromPoint(point)); }
   #pragma push_macro("MessageBox")
   #undef MessageBox
    int window::MessageBox(const char * lpszText, const char * lpszCaption, UINT nType)
      { return _AFX_FUNCNAME(MessageBox)(lpszText, lpszCaption, nType); }
   #pragma pop_macro("MessageBox")
    BOOL window::FlashWindow(BOOL bInvert)
      { ASSERT(::IsWindow(get_handle())); return ::FlashWindow(get_handle(), bInvert); }
    BOOL window::ChangeClipboardChain(HWND hWndNext)
      { ASSERT(::IsWindow(get_handle())); return ::ChangeClipboardChain(get_handle(), hWndNext); }
    HWND window::SetClipboardViewer()
      { ASSERT(::IsWindow(get_handle())); return ::SetClipboardViewer(get_handle()); }
    BOOL window::OpenClipboard()
      { ASSERT(::IsWindow(get_handle())); return ::OpenClipboard(get_handle()); }
    ::ca::window * PASCAL window::GetOpenClipboardWindow()
      { return ::win::window::from_handle(::GetOpenClipboardWindow()); }
    ::ca::window * PASCAL window::GetClipboardOwner()
      { return ::win::window::from_handle(::GetClipboardOwner()); }
    ::ca::window * PASCAL window::GetClipboardViewer()
      { return ::win::window::from_handle(::GetClipboardViewer()); }
    void window::CreateCaret(::ca::bitmap* pBitmap)
      { ASSERT(::IsWindow(get_handle())); ::CreateCaret(get_handle(), (HBITMAP)pBitmap->get_os_data(), 0, 0); }
    void window::CreateSolidCaret(int nWidth, int nHeight)
      { ASSERT(::IsWindow(get_handle())); ::CreateCaret(get_handle(), (HBITMAP)0, nWidth, nHeight); }
    void window::CreateGrayCaret(int nWidth, int nHeight)
      { ASSERT(::IsWindow(get_handle())); ::CreateCaret(get_handle(), (HBITMAP)1, nWidth, nHeight); }
    point PASCAL window::GetCaretPos()
      { point point; ::GetCaretPos((LPPOINT)&point); return point; }
    void PASCAL window::SetCaretPos(POINT point)
      { ::SetCaretPos(point.x, point.y); }
    void window::HideCaret()
      { ::HideCaret(get_handle()); }
    void window::ShowCaret()
      { ::ShowCaret(get_handle()); }

    BOOL window::SetForegroundWindow()
    {

       return ::SetForegroundWindow(get_handle()); 

    }

    ::ca::window * PASCAL window::GetForegroundWindow()
    {
    
       return ::win::window::from_handle(::GetForegroundWindow());
    
    }

    BOOL window::SendNotifyMessage(UINT message, WPARAM wParam, LPARAM lParam)
    { 
    
       return ::SendNotifyMessage(get_handle(), message, wParam, lParam);
    
    }

   // Win4
    HICON window::SetIcon(HICON hIcon, BOOL bBigIcon)
      { return (HICON)SendMessage(WM_SETICON, bBigIcon, (LPARAM)hIcon); }
    HICON window::GetIcon(BOOL bBigIcon) const
      { ASSERT(::IsWindow(get_handle())); return (HICON)const_cast < window * > (this)->SendMessage(WM_GETICON, bBigIcon, 0); }
    void window::Print(::ca::graphics * pgraphics, DWORD dwFlags) const
      { ASSERT(::IsWindow(get_handle())); const_cast < window * > (this)->SendMessage(WM_PRINT, (WPARAM)(dynamic_cast<::win::graphics * >(pgraphics))->get_os_data(), dwFlags); }
    void window::PrintClient(::ca::graphics * pgraphics, DWORD dwFlags) const
      { ASSERT(::IsWindow(get_handle())); const_cast < window * > (this)->SendMessage(WM_PRINTCLIENT, (WPARAM)(dynamic_cast<::win::graphics * >(pgraphics))->get_os_data(), dwFlags); }
    BOOL window::SetWindowContextHelpId(DWORD dwContextHelpId)
      { ASSERT(::IsWindow(get_handle())); return ::SetWindowContextHelpId(get_handle(), dwContextHelpId); }
    DWORD window::GetWindowContextHelpId() const
      { ASSERT(::IsWindow(get_handle())); return ::GetWindowContextHelpId(get_handle()); }


   // Default message ::collection::map implementations
    void window::OnActivateApp(BOOL, DWORD)
      { Default(); }
    void window::OnActivate(UINT, ::ca::window *, BOOL)
      { Default(); }
    void window::OnCancelMode()
      { Default(); }
    void window::OnChildActivate()
      { Default(); }
    void window::OnClose()
      { Default(); }
    void window::OnContextMenu(::ca::window *, point)
      { Default(); }
    int window::OnCopyData(::ca::window *, COPYDATASTRUCT*)
      { return (int)Default(); }
    void window::OnEnable(BOOL)
      { Default(); }
    void window::OnEndSession(BOOL)
      { Default(); }
    BOOL window::OnEraseBkgnd(::ca::graphics *)
      { return (BOOL)Default(); }
    void window::OnGetMinMaxInfo(MINMAXINFO*)
      { Default(); }
    void window::OnIconEraseBkgnd(::ca::graphics *)
      { Default(); }
    void window::OnKillFocus(::ca::window *)
      { Default(); }
    LRESULT window::OnMenuChar(UINT, UINT, ::userbase::menu*)
      { return Default(); }
    void window::OnMenuSelect(UINT, UINT, HMENU)
      { Default(); }
    void window::OnMove(int, int)
      { Default(); }
    HCURSOR window::OnQueryDragIcon()
      { return (HCURSOR)Default(); }
    BOOL window::OnQueryEndSession()
      { return (BOOL)Default(); }
    BOOL window::OnQueryNewPalette()
      { return (BOOL)Default(); }
    BOOL window::OnQueryOpen()
      { return (BOOL)Default(); }
    void window::_001OnSetCursor(gen::signal_object * pobj)
    {
       SCAST_PTR(::gen::message::base, pbase, pobj);
       if(System.get_cursor() != NULL
          && System.get_cursor()->m_ecursor != ::visual::cursor_system)
       {
         ::SetCursor(NULL);
       }
       pbase->set_lresult(1);
       pbase->m_bRet = true;
       //(BOOL)Default(); 
    }
    void window::OnShowWindow(BOOL, UINT)
      { Default(); }
    void window::OnSize(UINT, int, int)
      { Default(); }
    void window::OnTCard(UINT, DWORD)
      { Default(); }
    void window::OnWindowPosChanging(WINDOWPOS*)
      { Default(); }
    void window::OnWindowPosChanged(WINDOWPOS*)
      { Default(); }
    void window::OnDropFiles(HDROP)
      { Default(); }
    void window::OnPaletteIsChanging(::ca::window *)
      { Default(); }
    BOOL window::OnNcActivate(BOOL)
      { return (BOOL)Default(); }
    void window::OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*)
      { Default(); }
    BOOL window::OnNcCreate(LPCREATESTRUCT)
      { return (BOOL)Default(); }
    LRESULT window::OnNcHitTest(point)
      { return Default(); }
    void window::OnNcLButtonDblClk(UINT, point)
      { Default(); }
    void window::OnNcLButtonDown(UINT, point)
      { Default(); }
    void window::OnNcLButtonUp(UINT, point)
      { Default(); }
    void window::OnNcMButtonDblClk(UINT, point)
      { Default(); }
    void window::OnNcMButtonDown(UINT, point)
      { Default(); }
    void window::OnNcMButtonUp(UINT, point)
      { Default(); }
    void window::OnNcMouseMove(UINT, point)
      { Default(); }
    void window::OnNcPaint()
      { Default(); }
    void window::OnNcRButtonDblClk(UINT, point)
      { Default(); }
    void window::OnNcRButtonDown(UINT, point)
      { Default(); }
    void window::OnNcRButtonUp(UINT, point)
      { Default(); }
    void window::OnSysChar(UINT, UINT, UINT)
      { Default(); }
    void window::OnSysCommand(UINT, LPARAM)
      { Default(); }
    void window::OnSysDeadChar(UINT, UINT, UINT)
      { Default(); }
    void window::OnSysKeyDown(UINT, UINT, UINT)
      { Default(); }
    void window::OnSysKeyUp(UINT, UINT, UINT)
      { Default(); }
    void window::OnCompacting(UINT)
      { Default(); }
    void window::OnFontChange()
      { Default(); }
    void window::OnPaletteChanged(::ca::window *)
      { Default(); }
    void window::OnSpoolerStatus(UINT, UINT)
      { Default(); }
    void window::OnTimeChange()
      { Default(); }
    void window::OnChar(UINT, UINT, UINT)
      { Default(); }
    void window::OnDeadChar(UINT, UINT, UINT)
      { Default(); }
    void window::OnKeyDown(UINT, UINT, UINT)
      { Default(); }
    void window::OnKeyUp(UINT, UINT, UINT)
      { Default(); }
    void window::OnLButtonDblClk(UINT, point)
      { Default(); }
    void window::OnLButtonDown(UINT, point)
      { Default(); }
    void window::OnLButtonUp(UINT, point)
      { Default(); }
    void window::OnMButtonDblClk(UINT, point)
      { Default(); }
    void window::OnMButtonDown(UINT, point)
      { Default(); }
    void window::OnMButtonUp(UINT, point)
      { Default(); }
    int window::OnMouseActivate(::ca::window *, UINT, UINT)
      { return (int)Default(); }
    void window::OnMouseMove(UINT, point)
      { Default(); }
    BOOL window::OnMouseWheel(UINT, short, point)
      { return (BOOL)Default(); }
    LRESULT window::OnRegisteredMouseWheel(WPARAM, LPARAM)
      { return Default(); }
    void window::OnRButtonDblClk(UINT, point)
      { Default(); }
    void window::OnRButtonDown(UINT, point)
      { Default(); }
    void window::OnRButtonUp(UINT, point)
      { Default(); }
    void window::OnTimer(UINT_PTR)
      { Default(); }
    void window::OnInitMenu(::userbase::menu*)
      { Default(); }
    void window::OnInitMenuPopup(::userbase::menu*, UINT, BOOL)
      { Default(); }
    void window::OnAskCbFormatName(__in UINT nMaxCount, __out_ecount_z(nMaxCount) LPTSTR pszName)
   {
      (nMaxCount);
      if(nMaxCount>0)
      {
         /* defwindow proc should do this for us, but to be safe, we'll do it here too */
         pszName[0]='\0';
      }
      Default(); 
   }
    void window::OnChangeCbChain(HWND, HWND)
      { Default(); }
    void window::OnDestroyClipboard()
      { Default(); }
    void window::OnDrawClipboard()
      { Default(); }
    void window::OnHScrollClipboard(::ca::window *, UINT, UINT)
      { Default(); }
    void window::OnPaintClipboard(::ca::window *, HGLOBAL)
      { Default(); }
    void window::OnRenderAllFormats()
      { Default(); }
    void window::OnRenderFormat(UINT)
      { Default(); }
    void window::OnSizeClipboard(::ca::window *, HGLOBAL)
      { Default(); }
    void window::OnVScrollClipboard(::ca::window *, UINT, UINT)
      { Default(); }
    UINT window::OnGetDlgCode()
      { return (UINT)Default(); }
    void window::OnMDIActivate(BOOL, ::ca::window *, ::ca::window *)
      { Default(); }
    void window::OnEnterMenuLoop(BOOL)
      { Default(); }
    void window::OnExitMenuLoop(BOOL)
      { Default(); }
   // Win4 support
    void window::OnStyleChanged(int, LPSTYLESTRUCT)
      { Default(); }
    void window::OnStyleChanging(int, LPSTYLESTRUCT)
      { Default(); }
    void window::OnSizing(UINT, LPRECT)
      { Default(); }
    void window::OnMoving(UINT, LPRECT)
      { Default(); }
    void window::OnCaptureChanged(::ca::window *)
      { Default(); }
    BOOL window::OnDeviceChange(UINT, DWORD_PTR)
      { return (BOOL)Default(); }
    void window::OnWinIniChange(const char *)
      { Default(); }
    void window::OnChangeUIState(UINT, UINT)
      { Default(); }
    void window::OnUpdateUIState(UINT, UINT)
      { Default(); }
    UINT window::OnQueryUIState()
      { return (UINT)Default(); }

   // window dialog data support
//    void window::DoDataExchange(CDataExchange*)
   //   { } // default does nothing

   // window modality support
    void window::BeginModalState()
      { ::EnableWindow(get_handle(), FALSE); }
    void window::EndModalState()
      { ::EnableWindow(get_handle(), TRUE); }

   // frame_window
/*    void frame_window::DelayUpdateFrameTitle()
      { m_nIdleFlags |= idleTitle; }
    void frame_window::DelayRecalcLayout(BOOL bNotify)
      { m_nIdleFlags |= (idleLayout | (bNotify ? idleNotify : 0)); };
    BOOL frame_window::InModalState() const
      { return m_cModalStack != 0; }
    void frame_window::set_title(const char * lpszTitle)
      { m_strTitle = lpszTitle; }
    string frame_window::get_title() const
      { return m_strTitle; }
*/

   // MDI functions
  /*  void CMDIFrameWnd::MDIActivate(::user::interaction* pWndActivate)
      {  m_pguieMdiClient->SendMessage(WM_MDIACTIVATE,
         (WPARAM)pWndActivate, 0); }
    void CMDIFrameWnd::MDIIconArrange()
      {  m_pguieMdiClient->SendMessage(WM_MDIICONARRANGE, 0, 0); }
    void CMDIFrameWnd::MDIMaximize(::user::interaction * pWnd)
      {  m_pguieMdiClient->SendMessage(WM_MDIMAXIMIZE, (WPARAM)pWnd, 0); }
    void CMDIFrameWnd::MDIPrev()
      {  m_pguieMdiClient->SendMessage(WM_MDINEXT, 0, 1); }
    void CMDIFrameWnd::MDINext()
      {  m_pguieMdiClient->SendMessage(WM_MDINEXT, 0, 0); }
    void CMDIFrameWnd::MDIRestore(::user::interaction * pWnd)
      {  m_pguieMdiClient->SendMessage(WM_MDIRESTORE, (WPARAM)pWnd, 0); }
    void CMDIFrameWnd::MDITile()
      {  m_pguieMdiClient->SendMessage(WM_MDITILE, 0, 0); }
    void CMDIFrameWnd::MDICascade()
      {  m_pguieMdiClient->SendMessage(WM_MDICASCADE, 0, 0); }

    void CMDIFrameWnd::MDICascade(int nType)
      {  m_pguieMdiClient->SendMessage(WM_MDICASCADE, nType, 0); }
    void CMDIFrameWnd::MDITile(int nType)
      {  m_pguieMdiClient->SendMessage(WM_MDITILE, nType, 0); }
    void CMDIChildWnd::MDIDestroy()
      {  GetParent()->SendMessage(WM_MDIDESTROY, (WPARAM)this, 0L); }
    void CMDIChildWnd::MDIActivate()
      {  GetParent()->SendMessage( WM_MDIACTIVATE, (WPARAM)this, 0L); }
    void CMDIChildWnd::MDIMaximize()
      {  GetParent()->SendMessage( WM_MDIMAXIMIZE, (WPARAM)this, 0L); }
    void CMDIChildWnd::MDIRestore()
      {  GetParent()->SendMessage( WM_MDIRESTORE, (WPARAM)this, 0L); }

   // ::view
    document * view::get_document() const
      { ASSERT(this != NULL); return ::view::get_document(); }

   // document
    const string & document::get_title() const
      { ASSERT(this != NULL); return m_strTitle; }
    const string & document::get_path_name() const
      { ASSERT(this != NULL); return m_strPathName; }
    document_template * document::get_document_template() const
      { ASSERT(this != NULL); return m_pdocumentemplate; }
    BOOL document::is_modified()
      { ASSERT(this != NULL); return m_bModified; }
    void document::set_modified_flag(BOOL bModified)
      { ASSERT(this != NULL); m_bModified = bModified; }

   // thread
    thread::operator HANDLE() const
      { return this == NULL ? NULL : m_hThread; }
    BOOL thread::SetThreadPriority(int nPriority)
      { ASSERT(m_hThread != NULL); return ::SetThreadPriority(m_hThread, nPriority); }
    int thread::GetThreadPriority()
      { ASSERT(m_hThread != NULL); return ::GetThreadPriority(m_hThread); }
    DWORD thread::ResumeThread()
      { ASSERT(m_hThread != NULL); return ::ResumeThread(m_hThread); }
    DWORD thread::SuspendThread()
      { ASSERT(m_hThread != NULL); return ::SuspendThread(m_hThread); }
    BOOL thread::PostThreadMessage(UINT message, WPARAM wParam, LPARAM lParam)
      { ASSERT(m_hThread != NULL); return ::PostThreadMessage(m_nThreadID, message, wParam, lParam); }*/



   /////////////////////////////////////////////////////////////////////////////
   // Obsolete and non-portable

    void window::CloseWindow()
      { ASSERT(::IsWindow(get_handle())); ::CloseWindow(get_handle()); }
    BOOL window::OpenIcon()
      { ASSERT(::IsWindow(get_handle())); return ::OpenIcon(get_handle()); }

   ////////////////////////////////////////////////////////////////////////////
   // UI related ::ca::window functions

   HWND PASCAL window::GetSafeOwner_(HWND hParent, HWND* pWndTop)
   {
      // get ::ca::window to start with
      HWND hWnd = hParent;
      if (hWnd == NULL)
      {
   /* trans      frame_window* pFrame = command_target::GetRoutingFrame_();
         if (pFrame != NULL)
            hWnd = pFrame->get_handle();
         else
            hWnd = System.GetMainWnd()->get_handle();*/
      }

      // a popup ::ca::window cannot be owned by a child ::ca::window
      while (hWnd != NULL && (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD))
         hWnd = ::GetParent(hWnd);

      // determine toplevel ::ca::window to disable as well
      HWND hWndTop = hWnd, hWndTemp = hWnd;
      for (;;)
      {
         if (hWndTemp == NULL)
            break;
         else
            hWndTop = hWndTemp;
         hWndTemp = ::GetParent(hWndTop);
      }

      // get last active popup of first non-child that was found
      if (hParent == NULL && hWnd != NULL)
         hWnd = ::GetLastActivePopup(hWnd);

      // disable and store top level parent ::ca::window if specified
      if (pWndTop != NULL)
      {
         if (hWndTop != NULL && ::IsWindowEnabled(hWndTop) && hWndTop != hWnd)
         {
            *pWndTop = hWndTop;
            ::EnableWindow(hWndTop, FALSE);
         }
         else
            *pWndTop = NULL;
      }

      return hWnd;    // return the owner as HWND
   }

   void window::on_delete(::ca::ca * pui)
   {
      UNREFERENCED_PARAMETER(pui);
  }


   /////////////////////////////////////////////////////////////////////////////
   // Official way to send message to a window

   CLASS_DECL_VMSWIN LRESULT AfxCallWndProc(::user::interaction * pinteraction, HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
   {
      _AFX_THREAD_STATE* pThreadState = _afxThreadState.get_data();
      MSG oldState = pThreadState->m_lastSentMsg;   // save for nesting
      pThreadState->m_lastSentMsg.hwnd = hWnd;
      pThreadState->m_lastSentMsg.message = nMsg;
      pThreadState->m_lastSentMsg.wParam = wParam;
      pThreadState->m_lastSentMsg.lParam = lParam;

      // Catch exceptions thrown outside the scope of a callback
      // in debug builds and warn the ::fontopus::user.
      ::ca::smart_pointer < ::gen::message::base > spbase;

      spbase(pinteraction->get_base(hWnd, nMsg, wParam, lParam));

      _AfxTraceMsg("WndProc", spbase);

      try
      {

         // special case for WM_INITDIALOG
         rect rectOld;
         DWORD dwStyle = 0;
         if (nMsg == WM_INITDIALOG)
            _AfxPreInitDialog(pinteraction, &rectOld, &dwStyle);

         // delegate to object's message_handler
         if(pinteraction->m_pguie != NULL && pinteraction->m_pguie != pinteraction)
         {
            pinteraction->m_pguie->message_handler(spbase);
         }
         else
         {
            pinteraction->message_handler(spbase);
         }
         // more special case for WM_INITDIALOG
         if (nMsg == WM_INITDIALOG)
            _AfxPostInitDialog(pinteraction, rectOld, dwStyle);
      }
      catch(const ::ca::exception & e)
      {
         try
         {
            if(App(pinteraction->m_papp).on_run_exception((::ca::exception &) e))
               goto run;
         }
         catch(...)
         {
         }
         return -1;
      }
      catch(base_exception * pe)
      {
         AfxProcessWndProcException(pe, spbase);
//         TRACE(::radix::trace::category_AppMsg, 0, "Warning: Uncaught exception in message_handler (returning %ld).\n", spbase->get_lresult());
         pe->Delete();
      }
   run:
      pThreadState->m_lastSentMsg = oldState;
      LRESULT lresult = spbase->get_lresult();
      return lresult;
   }


   /*CDataExchange::CDataExchange(::ca::window * pDlgWnd, BOOL bSaveAndValidate)
   {
      ASSERT_VALID(pDlgWnd);
      m_bSaveAndValidate = bSaveAndValidate;
      m_pDlgWnd = pDlgWnd;
      m_idLastControl = 0;
   }*/

   /////////////////////////////////////////////////////////////////////////////
   // Window creation hooks

   LRESULT CALLBACK _AfxCbtFilterHook(int code, WPARAM wParam, LPARAM lParam)
   {
      _AFX_THREAD_STATE* pThreadState = _afxThreadState.get_data();
      if (code != HCBT_CREATEWND)
      {
         // wait for HCBT_CREATEWND just pass others on...
         return CallNextHookEx(pThreadState->m_hHookOldCbtFilter, code,
            wParam, lParam);
      }

      ASSERT(lParam != NULL);
      LPCREATESTRUCT lpcs = ((LPCBT_CREATEWND)lParam)->lpcs;
      ASSERT(lpcs != NULL);

      ::user::interaction * pWndInit = pThreadState->m_pWndInit;
      BOOL bContextIsDLL = afxContextIsDLL;
      if (pWndInit != NULL || (!(lpcs->style & WS_CHILD) && !bContextIsDLL))
      {
         // Note: special check to avoid subclassing the IME window
         //if (_afxDBCS)
         {
            // check for cheap CS_IME style first...
            if (GetClassLong((HWND)wParam, GCL_STYLE) & CS_IME)
               goto lCallNextHook;

            // get class name of the window that is being created
            const char * pszClassName;
            char szClassName[_countof("ime")+1];
            if (DWORD_PTR(lpcs->lpszClass) > 0xffff)
            {
               pszClassName = lpcs->lpszClass;
            }
            else
            {
               szClassName[0] = '\0';
               GlobalGetAtomName((ATOM)lpcs->lpszClass, szClassName, _countof(szClassName));
               pszClassName = szClassName;
            }

            // a little more expensive to test this way, but necessary...
            if (::AfxInvariantStrICmp(pszClassName, "ime") == 0)
               goto lCallNextHook;
         }

         ASSERT(wParam != NULL); // should be non-NULL HWND
         HWND hWnd = (HWND)wParam;
         WNDPROC oldWndProc;
         if (pWndInit != NULL)
         {
            // the window should not be in the permanent ::collection::map at this time
            ASSERT(::win::window::FromHandlePermanent(hWnd) == NULL);

            pWndInit->m_pthread = dynamic_cast < ::radix::thread * > (::win::get_thread());
            pWndInit->m_pthread->add(pWndInit);
            pWndInit->m_pguie->m_pthread = pWndInit->m_pthread;
            pWndInit->m_pguie->m_pthread->add(pWndInit->m_pguie);
            pWndInit->m_pguie->m_pimpl = pWndInit;
         
            // connect the HWND to pWndInit...
            pWndInit->Attach(hWnd);
            // allow other subclassing to occur first
            pWndInit->pre_subclass_window();

            WNDPROC *pOldWndProc = pWndInit->GetSuperWndProcAddr();
            ASSERT(pOldWndProc != NULL);

            // subclass the window with standard AfxWndProc
            WNDPROC afxWndProc = AfxGetAfxWndProc();
            oldWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC,
               (DWORD_PTR)afxWndProc);
            ASSERT(oldWndProc != NULL);
            if (oldWndProc != afxWndProc)
               *pOldWndProc = oldWndProc;

            pThreadState->m_pWndInit = NULL;
         }
         else
         {
            ASSERT(!bContextIsDLL);   // should never get here

            static ATOM s_atomMenu = 0;
            bool bSubclass = true;         

            if (s_atomMenu == 0)
            {
               WNDCLASSEX wc;
               memset(&wc, 0, sizeof(WNDCLASSEX));
               wc.cbSize = sizeof(WNDCLASSEX);
               s_atomMenu = (ATOM)::GetClassInfoEx(NULL, "#32768", &wc);
            }

            // Do not subclass menus.
            if (s_atomMenu != 0)
            {
               ATOM atomWnd = (ATOM)::GetClassLongPtr(hWnd, GCW_ATOM);
               if (atomWnd == s_atomMenu)
                     bSubclass = false;
            }
            else
            {         
               char szClassName[256];
               if (::GetClassName(hWnd, szClassName, 256))
               {
                  szClassName[255] = NULL;
                  if (_tcscmp(szClassName, "#32768") == 0)
                     bSubclass = false;
               }
            }         
            if (bSubclass)
            {
               // subclass the window with the proc which does gray backgrounds
               oldWndProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
               if (oldWndProc != NULL && GetProp(hWnd, _afxOldWndProc) == NULL)
               {
                  SetProp(hWnd, _afxOldWndProc, oldWndProc);
                  if ((WNDPROC)GetProp(hWnd, _afxOldWndProc) == oldWndProc)
                  {
                     GlobalAddAtom(_afxOldWndProc);
                     SetWindowLongPtr(hWnd, GWLP_WNDPROC, (DWORD_PTR)_AfxActivationWndProc);
                     ASSERT(oldWndProc != NULL);
                  }
               }
            }
         }
      }

   lCallNextHook:
      LRESULT lResult = CallNextHookEx(pThreadState->m_hHookOldCbtFilter, code,
         wParam, lParam);

   #ifndef _ApplicationFrameworkDLL
      if (bContextIsDLL)
      {
         ::UnhookWindowsHookEx(pThreadState->m_hHookOldCbtFilter);
         pThreadState->m_hHookOldCbtFilter = NULL;
      }
   #endif
      return lResult;
   }



   void window::_001OnEraseBkgnd(gen::signal_object * pobj)
   {
      SCAST_PTR(::gen::message::erase_bkgnd, perasebkgnd, pobj);
      perasebkgnd->m_bRet = true;
      perasebkgnd->set_result(TRUE);
   }

   void window::_001BaseWndInterfaceMap()
   {
      System.window_map().set((INT_PTR)get_handle(), this);
   }



} // namespace win


CTestCmdUI::CTestCmdUI(::ca::application * papp) :
   ca(papp),
   cmd_ui(papp)
{
   m_bEnabled = TRUE;  // assume it is enabled
}

void CTestCmdUI::Enable(BOOL bOn)
{
   m_bEnabled = bOn;
   m_bEnableChanged = TRUE;
}

void CTestCmdUI::SetCheck(int)
{
   // do nothing -- just want to know about calls to Enable
}

void CTestCmdUI::SetRadio(BOOL)
{
   // do nothing -- just want to know about calls to Enable
}

void CTestCmdUI::SetText(const char *)
{
   // do nothing -- just want to know about calls to Enable
}


/////////////////////////////////////////////////////////////////////////////
// Map from HWND to ::ca::window *

hwnd_map* PASCAL afxMapHWND(BOOL bCreate)
{
   UNREFERENCED_PARAMETER(bCreate);
   try
   {
      AFX_MODULE_STATE* pState = AfxGetModuleState();
      if(pState == NULL)
         return NULL;
      return pState->m_pmapHWND;
   }
   catch(...)
   {
      return NULL;
   }
}


   /////////////////////////////////////////////////////////////////////////////
// The WndProc for all window's and derived classes

#undef AfxWndProc

LRESULT CALLBACK AfxWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
   // special message which identifies the window as using AfxWndProc
   if (nMsg == WM_QUERYAFXWNDPROC)
      return 1;

   // all other messages route through message ::collection::map
   ::ca::window * pWnd = ::win::window::FromHandlePermanent(hWnd);
   //ASSERT(pWnd != NULL);               
   //ASSERT(pWnd==NULL || WIN_WINDOW(pWnd)->get_handle() == hWnd);
   if (pWnd == NULL || WIN_WINDOW(pWnd)->get_handle() != hWnd)
      return ::DefWindowProc(hWnd, nMsg, wParam, lParam);
   return win::AfxCallWndProc(pWnd, hWnd, nMsg, wParam, lParam);
}

// always indirectly accessed via AfxGetAfxWndProc
WNDPROC CLASS_DECL_VMSWIN AfxGetAfxWndProc()
{
#ifdef _ApplicationFrameworkDLL
   return AfxGetModuleState()->m_pfnAfxWndProc;
#else
   return &AfxWndProc;
#endif
}
   /////////////////////////////////////////////////////////////////////////////
   // Special helpers for certain windows messages

   AFX_STATIC void CLASS_DECL_VMSWIN _AfxPreInitDialog(
      ::user::interaction * pWnd, LPRECT lpRectOld, DWORD* pdwStyleOld)
   {
      ASSERT(lpRectOld != NULL);   
      ASSERT(pdwStyleOld != NULL);

      WIN_WINDOW(pWnd)->GetWindowRect(lpRectOld);
      *pdwStyleOld = WIN_WINDOW(pWnd)->GetStyle();
   }

   AFX_STATIC void CLASS_DECL_VMSWIN _AfxPostInitDialog(
      ::user::interaction * pWnd, const RECT& rectOld, DWORD dwStyleOld)
   {
      // must be hidden to start with      
      if (dwStyleOld & WS_VISIBLE)
         return;

      // must not be visible after WM_INITDIALOG
      if (WIN_WINDOW(pWnd)->GetStyle() & (WS_VISIBLE|WS_CHILD))
         return;

      // must not move during WM_INITDIALOG
      rect rect;
      WIN_WINDOW(pWnd)->GetWindowRect(rect);
      if (rectOld.left != rect.left || rectOld.top != rect.top)
         return;

      // must be unowned or owner disabled
      ::user::interaction * pParent = WIN_WINDOW(pWnd)->GetWindow(GW_OWNER);
      if (pParent != NULL && pParent->IsWindowEnabled())
         return;

      if (!WIN_WINDOW(pWnd)->CheckAutoCenter())
         return;

      // center modal dialog boxes/message boxes
      //WIN_WINDOW(pWnd)->CenterWindow();
   }



CLASS_DECL_VMSWIN void AfxHookWindowCreate(::user::interaction * pWnd)
{
   _AFX_THREAD_STATE* pThreadState = _afxThreadState.get_data();
   if (pThreadState->m_pWndInit == pWnd)
      return;

   if (pThreadState->m_hHookOldCbtFilter == NULL)
   {
      pThreadState->m_hHookOldCbtFilter = ::SetWindowsHookEx(WH_CBT,
         win::_AfxCbtFilterHook, NULL, ::GetCurrentThreadId());
      if (pThreadState->m_hHookOldCbtFilter == NULL)
         AfxThrowMemoryException();
   }
   ASSERT(pThreadState->m_hHookOldCbtFilter != NULL);
   ASSERT(pWnd != NULL);
// trans   ASSERT(WIN_WINDOW(pWnd)->get_handle() == NULL);   // only do once

   ASSERT(pThreadState->m_pWndInit == NULL);   // hook not already in progress
   pThreadState->m_pWndInit = pWnd;
}

CLASS_DECL_VMSWIN BOOL AfxUnhookWindowCreate()
{
   _AFX_THREAD_STATE* pThreadState = _afxThreadState.get_data();
#ifndef _ApplicationFrameworkDLL
   if (afxContextIsDLL && pThreadState->m_hHookOldCbtFilter != NULL)
   {
      ::UnhookWindowsHookEx(pThreadState->m_hHookOldCbtFilter);
      pThreadState->m_hHookOldCbtFilter = NULL;
   }
#endif
   if (pThreadState->m_pWndInit != NULL)
   {
      pThreadState->m_pWndInit = NULL;
      return FALSE;   // was not successfully hooked
   }
   return TRUE;
}



CLASS_DECL_VMSWIN const char * AfxRegisterWndClass(UINT nClassStyle,
   HCURSOR hCursor, HBRUSH hbrBackground, HICON hIcon)
{
   // Returns a temporary string name for the class
   //  Save in a string if you want to use it for a long time
   LPTSTR lpszName = AfxGetThreadState()->m_szTempClassName;

   // generate a synthetic name for this class
   HINSTANCE hInst = Sys(::win::get_thread()->m_papp).m_hInstance;

   if (hCursor == NULL && hbrBackground == NULL && hIcon == NULL)
   {
      ATL_CRT_ERRORCHECK_SPRINTF(_sntprintf_s(lpszName, _AFX_TEMP_CLASS_NAME_SIZE, _AFX_TEMP_CLASS_NAME_SIZE - 1, "Afx:%p:%x", hInst, nClassStyle));
   }
   else
   {
      ATL_CRT_ERRORCHECK_SPRINTF(_sntprintf_s(lpszName, _AFX_TEMP_CLASS_NAME_SIZE, _AFX_TEMP_CLASS_NAME_SIZE - 1, "Afx:%p:%x:%p:%p:%p", hInst, nClassStyle,
         hCursor, hbrBackground, hIcon));
   }
   
   // see if the class already exists
   WNDCLASS wndcls;
   if (::GetClassInfo(hInst, lpszName, &wndcls))
   {
      // already registered, assert everything is good
      ASSERT(wndcls.style == nClassStyle);

      // NOTE: We have to trust that the hIcon, hbrBackground, and the
      //  hCursor are semantically the same, because sometimes Windows does
      //  some internal translation or copying of those handles before
      //  storing them in the internal WNDCLASS retrieved by GetClassInfo.
      return lpszName;
   }

   // otherwise we need to register a new class
   wndcls.style = nClassStyle;
   wndcls.lpfnWndProc = DefWindowProc;
   wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
   wndcls.hInstance = hInst;
   wndcls.hIcon = hIcon;
   //wndcls.hCursor = hCursor;
   wndcls.hCursor = NULL;
   wndcls.hbrBackground = hbrBackground;
   wndcls.lpszMenuName = NULL;
   wndcls.lpszClassName = lpszName;
   if (!AfxRegisterClass(&wndcls))
      AfxThrowResourceException();

   // return thread-local pointer
   return lpszName;
}


   AFX_STATIC void CLASS_DECL_VMSWIN
_AfxHandleActivate(::ca::window * pWnd, WPARAM nState, ::ca::window * pWndOther)
{
   ASSERT(pWnd != NULL);      

   // send WM_ACTIVATETOPLEVEL when top-level parents change
   if (!(WIN_WINDOW(pWnd)->GetStyle() & WS_CHILD))
   {
      ::user::interaction * pTopLevel= WIN_WINDOW(pWnd)->GetTopLevelParent();
      if (pTopLevel && (pWndOther == NULL || !::IsWindow(WIN_WINDOW(pWndOther)->get_handle()) || pTopLevel != WIN_WINDOW(pWndOther)->GetTopLevelParent()))
      {
         // lParam points to window getting the WM_ACTIVATE message and
         //  hWndOther from the WM_ACTIVATE.
         HWND hWnd2[2];
         hWnd2[0] = WIN_WINDOW(pWnd)->get_handle();
         if(pWndOther == NULL || WIN_WINDOW(pWndOther) == NULL)
         {
            hWnd2[1] = NULL;
         }
         else
         {
            hWnd2[1] = WIN_WINDOW(pWndOther)->get_handle();
         }
         // send it...
         pTopLevel->SendMessage(WM_ACTIVATETOPLEVEL, nState, (LPARAM)&hWnd2[0]);
      }
   }
}

AFX_STATIC BOOL CLASS_DECL_VMSWIN
_AfxHandleSetCursor(::ca::window * pWnd, UINT nHitTest, UINT nMsg)
{
   if (nHitTest == HTERROR &&
      (nMsg == WM_LBUTTONDOWN || nMsg == WM_MBUTTONDOWN ||
       nMsg == WM_RBUTTONDOWN))
   {
      // activate the last active window if not active
      ::user::interaction * pLastActive = WIN_WINDOW(pWnd)->GetTopLevelParent();      
      if (pLastActive != NULL)
         pLastActive = pLastActive->GetLastActivePopup();
      if (pLastActive != NULL &&
         pLastActive != ::win::window::GetForegroundWindow() &&
         pLastActive->IsWindowEnabled())
      {
         pLastActive->SetForegroundWindow();
         return TRUE;
      }
   }
   return FALSE;
}



/////////////////////////////////////////////////////////////////////////////
// Standard init called by WinMain

AFX_STATIC BOOL CLASS_DECL_VMSWIN _AfxRegisterWithIcon(WNDCLASS* pWndCls,
   const char * lpszClassName, UINT nIDIcon)
{
   pWndCls->lpszClassName = lpszClassName;
   HINSTANCE hInst = AfxFindResourceHandle(
      ATL_MAKEINTRESOURCE(nIDIcon), ATL_RT_GROUP_ICON);
   if ((pWndCls->hIcon = ::LoadIcon(hInst, ATL_MAKEINTRESOURCE(nIDIcon))) == NULL)
   {
      // use default icon
      pWndCls->hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
   }
   return AfxRegisterClass(pWndCls);
}


BOOL CLASS_DECL_VMSWIN AfxEndDeferRegisterClass(LONG fToRegisterParam, const char ** ppszClass)
{
   // mask off all classes that are already registered
   AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
   LONG fToRegister = fToRegisterParam & ~pModuleState->m_fRegisteredClasses;
   if (fToRegister == 0)
   {
      fToRegister = fToRegisterParam;
      if(ppszClass != NULL)
      {
         if(fToRegister & AFX_WND_REG)
         {
            *ppszClass = _afxWnd;
         }
         else if (fToRegister & AFX_WNDOLECONTROL_REG)
         {
            *ppszClass = _afxWndOleControl;
         }
         else if (fToRegister & AFX_WNDCONTROLBAR_REG)
         {
            *ppszClass = _afxWndControlBar;
         }
         else if(fToRegister & AFX_WNDMDIFRAME_REG)
         {
            *ppszClass = _afxWndMDIFrame;
         }
         else if(fToRegister & AFX_WNDFRAMEORVIEW_REG)
         {
            *ppszClass = _afxWndFrameOrView;
         }
      }
      return TRUE;
   }

   LONG fRegisteredClasses = 0;

   // common initialization
   WNDCLASS wndcls;
   memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL defaults
   wndcls.lpfnWndProc = DefWindowProc;
   wndcls.hInstance = Sys(::win::get_thread()->m_papp).m_hInstance;
   wndcls.hCursor = afxData.hcurArrow;

   INITCOMMONCONTROLSEX init;
   init.dwSize = sizeof(init);

   // work to register classes as specified by fToRegister, populate fRegisteredClasses as we go
   if (fToRegister & AFX_WND_REG)
   {
      // Child windows - no brush, no icon, safest default class styles
      wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
      wndcls.lpszClassName = _afxWnd;
      if (AfxRegisterClass(&wndcls))
      {
         if(ppszClass != NULL)
         {
            *ppszClass = wndcls.lpszClassName;
         }
         fRegisteredClasses |= AFX_WND_REG;
      }
   }
   if (fToRegister & AFX_WNDOLECONTROL_REG)
   {
      // OLE control windows - use parent DC for speed
      wndcls.style |= CS_PARENTDC | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
      wndcls.lpszClassName = _afxWndOleControl;
      if (AfxRegisterClass(&wndcls))
      {
         if(ppszClass != NULL)
         {
            *ppszClass = wndcls.lpszClassName;
         }
         fRegisteredClasses |= AFX_WNDOLECONTROL_REG;
      }
   }
   if (fToRegister & AFX_WNDCONTROLBAR_REG)
   {
      // control bar windows
      wndcls.style = 0;   // control bars don't handle double click
      wndcls.lpszClassName = _afxWndControlBar;
      wndcls.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
      if (AfxRegisterClass(&wndcls))
      {
         if(ppszClass != NULL)
         {
            *ppszClass = wndcls.lpszClassName;
         }
         fRegisteredClasses |= AFX_WNDCONTROLBAR_REG;
      }
   }
   if (fToRegister & AFX_WNDMDIFRAME_REG)
   {
      // MDI Frame window (also used for splitter window)
      wndcls.style = CS_DBLCLKS;
      wndcls.hbrBackground = NULL;
/*      if (_AfxRegisterWithIcon(&wndcls, _afxWndMDIFrame, AFX_IDI_STD_MDIFRAME))
      {
         if(ppszClass != NULL)
         {
            *ppszClass = _afxWndMDIFrame;
         }
         fRegisteredClasses |= AFX_WNDMDIFRAME_REG;
      }*/
   }
   if (fToRegister & AFX_WNDFRAMEORVIEW_REG)
   {
      // SDI Frame or MDI Child windows or views - normal colors
      wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
      wndcls.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
      if (_AfxRegisterWithIcon(&wndcls, _afxWndFrameOrView, 123))
      {
         if(ppszClass != NULL)
         {
            *ppszClass = _afxWndFrameOrView;
         }
         fRegisteredClasses |= AFX_WNDFRAMEORVIEW_REG;
      }
   }


   // save new state of registered controls
   pModuleState->m_fRegisteredClasses |= fRegisteredClasses;

   // special case for all common controls registered, turn on AFX_WNDCOMMCTLS_REG
   if ((pModuleState->m_fRegisteredClasses & AFX_WIN95CTLS_MASK) == AFX_WIN95CTLS_MASK)
   {
      pModuleState->m_fRegisteredClasses |= AFX_WNDCOMMCTLS_REG;
      fRegisteredClasses |= AFX_WNDCOMMCTLS_REG;
   }

   // must have registered at least as mamy classes as requested
   return (fToRegister & fRegisteredClasses) == fToRegister;
}


/////////////////////////////////////////////////////////////////////////////
// Special WndProcs (activation handling & gray dialogs)


LRESULT CALLBACK
_AfxActivationWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
   WNDPROC oldWndProc = (WNDPROC)::GetProp(hWnd, _afxOldWndProc);
   ASSERT(oldWndProc != NULL);   

   LRESULT lResult = 0;
   try
   {
      BOOL bCallDefault = TRUE;
      switch (nMsg)
      {
      case WM_INITDIALOG:
         {
            DWORD dwStyle;
            rect rectOld;
            ::ca::window * pWnd = ::win::window::from_handle(hWnd);
            _AfxPreInitDialog(pWnd, &rectOld, &dwStyle);
            bCallDefault = FALSE;
            lResult = CallWindowProc(oldWndProc, hWnd, nMsg, wParam, lParam);
            _AfxPostInitDialog(pWnd, rectOld, dwStyle);
         }
         break;

      case WM_ACTIVATE:
         _AfxHandleActivate(::win::window::from_handle(hWnd), wParam,
            ::win::window::from_handle((HWND)lParam));
         break;

      case WM_SETCURSOR:
         bCallDefault = !_AfxHandleSetCursor(::win::window::from_handle(hWnd),
            (short)LOWORD(lParam), HIWORD(lParam));
         break;

      case WM_NCDESTROY:
         SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<INT_PTR>(oldWndProc));
         RemoveProp(hWnd, _afxOldWndProc);
         GlobalDeleteAtom(GlobalFindAtom(_afxOldWndProc));
         break;
      }

      // call original wndproc for default handling
      if (bCallDefault)
         lResult = CallWindowProc(oldWndProc, hWnd, nMsg, wParam, lParam);
   }
   catch(base_exception * pe)
   {
      // handle exception
      MSG msg;
      msg.hwnd = hWnd;
      msg.message = nMsg;
      msg.wParam = wParam;
      msg.lParam = lParam;

      //lResult = AfxProcessWndProcException(pe, &msg);
//      TRACE(::radix::trace::category_AppMsg, 0, "Warning: Uncaught exception in _AfxActivationWndProc (returning %ld).\n",
  //       lResult);
      pe->Delete();
   }
   

   return lResult;
}




/////////////////////////////////////////////////////////////////////////////
// Additional helpers for WNDCLASS init

// like RegisterClass, except will automatically call UnregisterClass
BOOL CLASS_DECL_VMSWIN AfxRegisterClass(WNDCLASS* lpWndClass)
{
   WNDCLASS wndcls;      
   if (GetClassInfo(lpWndClass->hInstance, lpWndClass->lpszClassName,
      &wndcls))
   {
      // class already registered
      return TRUE;
   }

   if (!::RegisterClass(lpWndClass))
   {
//      TRACE(::radix::trace::category_AppMsg, 0, "Can't register window class named %s\n",
  //       lpWndClass->lpszClassName);
      return FALSE;
   }

   BOOL bRet = TRUE;

   if (afxContextIsDLL)
   {
      AfxLockGlobals(CRIT_REGCLASSLIST);
      try
      {
         // class registered successfully, add to registered list
         AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
         if(pModuleState->m_pstrUnregisterList == NULL)
            pModuleState->m_pstrUnregisterList = new string;
         *pModuleState->m_pstrUnregisterList += lpWndClass->lpszClassName;
         *pModuleState->m_pstrUnregisterList +='\n';
      }
      catch(base_exception * pe)
      {
         AfxUnlockGlobals(CRIT_REGCLASSLIST);
         ::ca::rethrow(pe);
         // Note: DELETE_EXCEPTION not required.
      }
      
      AfxUnlockGlobals(CRIT_REGCLASSLIST);
   }

   return bRet;
}


#undef new
#include <gdiplus.h>


namespace win
{

   void window::_001DeferPaintLayeredWindowBackground(::ca::graphics * pdc)
   {

      rect rectClient;

      GetClientRect(rectClient);

      pdc->set_alpha_mode(::ca::alpha_mode_set);

      pdc->FillSolidRect(rectClient, ARGB(0, 0, 0, 0));

   }

   void window::_001UpdateWindow()
   {
      
      rect rectWindow;

      GetWindowRect(rectWindow);

      if(rectWindow.area() <= 0)
         return;

      ::visual::dib_sp dib(get_app());

      if(!dib->create(rectWindow.size()))
         return;

      Gdiplus::Graphics * pg = (Gdiplus::Graphics *) dib->get_graphics()->get_os_data();

      BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
      
      _001OnDeferPaintLayeredWindowBackground(dib->get_graphics());
      _001Print(dib->get_graphics());

      pg->Flush();

      HDC hdcScreen = ::GetDC(get_handle());

//      HDC hdcMemory = ::CreateCompatibleDC(NULL);

  //    HBITMAP hbitmap = CreateCompatibleBitmap(hdc, iWidth, iHeight);

    //  ::B

//      ::SelectObject(hdcMemory, hbitmap);


      point pt;
      size sz;

      pt = point(rectWindow.top_left());
      sz = size(rectWindow.size());

      point ptSrc(0, 0);

      ::visual::dib_sp dibMultAlphaWork(get_app());

      dib->mult_alpha(dibMultAlphaWork);


/*      ::Gdiplus::Graphics g(hdc);

      g.DrawImage((Gdiplus::Bitmap *) (dynamic_cast < ::win::dib * > (dib.m_p))->m_spbitmap->get_os_data(), 0, 0);*/
      //HDC hdcDib = pg->GetHDC();
 // Default to upper left of screen
     //POINT ptOrigin = { 0, 0 };
     //SIZE sizeSplash = { 128, 128 };

     // Get the actual screen location
     //GetPointOfOrigin(ptOrigin, sizeSplash);

     // Our in memory database of GDI+ Bitmaps
     //data::image::BoxOfBits *box = 
       //dynamic_cast<data::image::BoxOfBits *>(&Images.get_package("skin_layout_008"));

     // Create a display context as a canvas to draw the images
     //HDC hdcScreen = GetDC(NULL);

      BITMAPINFO bi;

      ZeroMemory(&bi, sizeof (BITMAPINFO));
	   bi.bmiHeader.biSize=sizeof (BITMAPINFOHEADER);
	   bi.bmiHeader.biWidth= rectWindow.width();
	   bi.bmiHeader.biHeight=-rectWindow.height();
	   bi.bmiHeader.biPlanes=1;
	   bi.bmiHeader.biBitCount=32;
	   bi.bmiHeader.biCompression=BI_RGB;
	   bi.bmiHeader.biSizeImage=abs(rectWindow.width()*rectWindow.height()*4);

     HDC hdcMem = CreateCompatibleDC(hdcScreen);
     COLORREF * pcolorref = (dynamic_cast < ::win::dib * > (dib.m_p))->m_pcolorref;
     
     HBITMAP bmMem = CreateDIBSection(hdcMem, &bi, DIB_RGB_COLORS, (void**) &pcolorref, NULL, 0);
     //memcpy(pcolorref, (dynamic_cast < ::win::dib * > (dib.m_p))->m_pcolorref, bi.bmiHeader.biSizeImage);
     // Prep canvas for rendering graphic
     HBITMAP hbmpOld = (HBITMAP)SelectObject(hdcMem, bmMem);
     Gdiplus::Graphics graphics(hdcMem);


     graphics.DrawImage((Gdiplus::Bitmap *) (dynamic_cast < ::win::dib * > (dib.m_p))->m_spbitmap->get_os_data(), 0, 0, 0, 0, rectWindow.width(), rectWindow.height(), Gdiplus::UnitPixel);

     // Prepare to alpha blend the canvas with the screen
     /*BLENDFUNCTION blend = { 0 };
     blend.BlendOp = AC_SRC_OVER;
     blend.SourceConstantAlpha = 255;
     blend.AlphaFormat = AC_SRC_ALPHA;*/

     // Composite the canvas with the screen into the layered window
     POINT ptZero = { 0 };
     ::UpdateLayeredWindow(get_handle(), hdcScreen, &pt, &sz, hdcMem, &ptSrc, RGB(0, 0, 0), &blendPixelFunction, ULW_ALPHA);

     // Delete temporary objects used for canvas
     ::SelectObject(hdcMem, hbmpOld);
     ::DeleteObject(bmMem);
     ::DeleteDC(hdcMem);
     ::ReleaseDC(NULL, hdcScreen);

      /*class rect rectWin;
      GetWindowRect(rectWin);
      if(rect(rectWindow) != rectWin || (m_pguie != NULL && (bool) m_pguie->oprop("pending_layout")))
      {

            
         if(m_pguie != NULL && (bool) m_pguie->oprop("pending_layout"))
         {
            HWND hwndZOrder = (HWND) m_pguie->oprop("pending_zorder").get_integer();
            SetWindowPos((int) HWND_TOPMOST, 
               rectWindow.left, rectWindow.top, rectWindow.width(), rectWindow.height(), SWP_SHOWWINDOW);
            SetWindowPos((int) HWND_NOTOPMOST, 
               rectWindow.left, rectWindow.top, rectWindow.width(), rectWindow.height(), SWP_SHOWWINDOW);
            SetWindowPos((int) hwndZOrder, 
               rectWindow.left, rectWindow.top, rectWindow.width(), rectWindow.height(), SWP_SHOWWINDOW | SWP_FRAMECHANGED);
            /*simple_frame_window * pframe = dynamic_cast < simple_frame_window * > (pwnd->m_pguie);
            if(pframe != NULL)
            {
               pframe->ActivateFrame();
            }*/
/*            m_pguie->oprop("pending_layout") = false;
         }
         else
         {
            SetWindowPos(NULL, rectWindow.left, rectWindow.top, rectWindow.width(), rectWindow.height(), SWP_SHOWWINDOW);
         }
      }
      */


   }



}