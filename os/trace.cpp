#include "StdAfx.h"
#include "dde.h"


/////////////////////////////////////////////////////////////////////////////
// Build data tables by including data file three times

struct CLASS_DECL_ca AFX_MAP_MESSAGE
{
   UINT    nMsg;
   const char *  lpszMsg;
};

#define DEFINE_MESSAGE(wm)  { wm, #wm }

static const AFX_MAP_MESSAGE allMessages[] =
{
   DEFINE_MESSAGE(WM_CREATE),
   DEFINE_MESSAGE(WM_DESTROY),
   DEFINE_MESSAGE(WM_MOVE),
   DEFINE_MESSAGE(WM_SIZE),
   DEFINE_MESSAGE(WM_ACTIVATE),
   DEFINE_MESSAGE(WM_SETFOCUS),
   DEFINE_MESSAGE(WM_KILLFOCUS),
   DEFINE_MESSAGE(WM_ENABLE),
   DEFINE_MESSAGE(WM_SETREDRAW),
   DEFINE_MESSAGE(WM_SETTEXT),
   DEFINE_MESSAGE(WM_GETTEXT),
   DEFINE_MESSAGE(WM_GETTEXTLENGTH),
   DEFINE_MESSAGE(WM_PAINT),
   DEFINE_MESSAGE(WM_CLOSE),
   DEFINE_MESSAGE(WM_QUERYENDSESSION),
   DEFINE_MESSAGE(WM_QUIT),
   DEFINE_MESSAGE(WM_QUERYOPEN),
   DEFINE_MESSAGE(WM_ERASEBKGND),
   DEFINE_MESSAGE(WM_SYSCOLORCHANGE),
   DEFINE_MESSAGE(WM_ENDSESSION),
   DEFINE_MESSAGE(WM_SHOWWINDOW),
   DEFINE_MESSAGE(WM_CTLCOLORMSGBOX),
   DEFINE_MESSAGE(WM_CTLCOLOREDIT),
   DEFINE_MESSAGE(WM_CTLCOLORLISTBOX),
   DEFINE_MESSAGE(WM_CTLCOLORBTN),
   DEFINE_MESSAGE(WM_CTLCOLORDLG),
   DEFINE_MESSAGE(WM_CTLCOLORSCROLLBAR),
   DEFINE_MESSAGE(WM_CTLCOLORSTATIC),
   DEFINE_MESSAGE(WM_WININICHANGE),
   DEFINE_MESSAGE(WM_SETTINGCHANGE),
   DEFINE_MESSAGE(WM_DEVMODECHANGE),
   DEFINE_MESSAGE(WM_ACTIVATEAPP),
   DEFINE_MESSAGE(WM_FONTCHANGE),
   DEFINE_MESSAGE(WM_TIMECHANGE),
   DEFINE_MESSAGE(WM_CANCELMODE),
   DEFINE_MESSAGE(WM_SETCURSOR),
   DEFINE_MESSAGE(WM_MOUSEACTIVATE),
   DEFINE_MESSAGE(WM_CHILDACTIVATE),
   DEFINE_MESSAGE(WM_QUEUESYNC),
   DEFINE_MESSAGE(WM_GETMINMAXINFO),
   DEFINE_MESSAGE(WM_ICONERASEBKGND),
   DEFINE_MESSAGE(WM_NEXTDLGCTL),
   DEFINE_MESSAGE(WM_SPOOLERSTATUS),
   DEFINE_MESSAGE(WM_DRAWITEM),
   DEFINE_MESSAGE(WM_MEASUREITEM),
   DEFINE_MESSAGE(WM_DELETEITEM),
   DEFINE_MESSAGE(WM_VKEYTOITEM),
   DEFINE_MESSAGE(WM_CHARTOITEM),
   DEFINE_MESSAGE(WM_SETFONT),
   DEFINE_MESSAGE(WM_GETFONT),
   DEFINE_MESSAGE(WM_QUERYDRAGICON),
   DEFINE_MESSAGE(WM_COMPAREITEM),
   DEFINE_MESSAGE(WM_COMPACTING),
   DEFINE_MESSAGE(WM_NCCREATE),
   DEFINE_MESSAGE(WM_NCDESTROY),
   DEFINE_MESSAGE(WM_NCCALCSIZE),
   DEFINE_MESSAGE(WM_NCHITTEST),
   DEFINE_MESSAGE(WM_NCPAINT),
   DEFINE_MESSAGE(WM_NCACTIVATE),
   DEFINE_MESSAGE(WM_GETDLGCODE),
   DEFINE_MESSAGE(WM_NCMOUSEMOVE),
   DEFINE_MESSAGE(WM_NCLBUTTONDOWN),
   DEFINE_MESSAGE(WM_NCLBUTTONUP),
   DEFINE_MESSAGE(WM_NCLBUTTONDBLCLK),
   DEFINE_MESSAGE(WM_NCRBUTTONDOWN),
   DEFINE_MESSAGE(WM_NCRBUTTONUP),
   DEFINE_MESSAGE(WM_NCRBUTTONDBLCLK),
   DEFINE_MESSAGE(WM_NCMBUTTONDOWN),
   DEFINE_MESSAGE(WM_NCMBUTTONUP),
   DEFINE_MESSAGE(WM_NCMBUTTONDBLCLK),
   DEFINE_MESSAGE(WM_KEYDOWN),
   DEFINE_MESSAGE(WM_KEYUP),
   DEFINE_MESSAGE(WM_CHAR),
   DEFINE_MESSAGE(WM_DEADCHAR),
   DEFINE_MESSAGE(WM_SYSKEYDOWN),
   DEFINE_MESSAGE(WM_SYSKEYUP),
   DEFINE_MESSAGE(WM_SYSCHAR),
   DEFINE_MESSAGE(WM_SYSDEADCHAR),
   DEFINE_MESSAGE(WM_KEYLAST),
   DEFINE_MESSAGE(WM_INITDIALOG),
   DEFINE_MESSAGE(WM_COMMAND),
   DEFINE_MESSAGE(WM_SYSCOMMAND),
   DEFINE_MESSAGE(WM_TIMER),
   DEFINE_MESSAGE(WM_HSCROLL),
   DEFINE_MESSAGE(WM_VSCROLL),
   DEFINE_MESSAGE(WM_INITMENU),
   DEFINE_MESSAGE(WM_INITMENUPOPUP),
   DEFINE_MESSAGE(WM_MENUSELECT),
   DEFINE_MESSAGE(WM_MENUCHAR),
   DEFINE_MESSAGE(WM_ENTERIDLE),
   DEFINE_MESSAGE(WM_MOUSEWHEEL),
   DEFINE_MESSAGE(WM_MOUSEMOVE),
   DEFINE_MESSAGE(WM_LBUTTONDOWN),
   DEFINE_MESSAGE(WM_LBUTTONUP),
   DEFINE_MESSAGE(WM_LBUTTONDBLCLK),
   DEFINE_MESSAGE(WM_RBUTTONDOWN),
   DEFINE_MESSAGE(WM_RBUTTONUP),
   DEFINE_MESSAGE(WM_RBUTTONDBLCLK),
   DEFINE_MESSAGE(WM_MBUTTONDOWN),
   DEFINE_MESSAGE(WM_MBUTTONUP),
   DEFINE_MESSAGE(WM_MBUTTONDBLCLK),
   DEFINE_MESSAGE(WM_PARENTNOTIFY),
   DEFINE_MESSAGE(WM_MDICREATE),
   DEFINE_MESSAGE(WM_MDIDESTROY),
   DEFINE_MESSAGE(WM_MDIACTIVATE),
   DEFINE_MESSAGE(WM_MDIRESTORE),
   DEFINE_MESSAGE(WM_MDINEXT),
   DEFINE_MESSAGE(WM_MDIMAXIMIZE),
   DEFINE_MESSAGE(WM_MDITILE),
   DEFINE_MESSAGE(WM_MDICASCADE),
   DEFINE_MESSAGE(WM_MDIICONARRANGE),
   DEFINE_MESSAGE(WM_MDIGETACTIVE),
   DEFINE_MESSAGE(WM_MDISETMENU),
   DEFINE_MESSAGE(WM_CUT),
   DEFINE_MESSAGE(WM_COPYDATA),
   DEFINE_MESSAGE(WM_COPY),
   DEFINE_MESSAGE(WM_PASTE),
   DEFINE_MESSAGE(WM_CLEAR),
   DEFINE_MESSAGE(WM_UNDO),
   DEFINE_MESSAGE(WM_RENDERFORMAT),
   DEFINE_MESSAGE(WM_RENDERALLFORMATS),
   DEFINE_MESSAGE(WM_DESTROYCLIPBOARD),
   DEFINE_MESSAGE(WM_DRAWCLIPBOARD),
   DEFINE_MESSAGE(WM_PAINTCLIPBOARD),
   DEFINE_MESSAGE(WM_VSCROLLCLIPBOARD),
   DEFINE_MESSAGE(WM_SIZECLIPBOARD),
   DEFINE_MESSAGE(WM_ASKCBFORMATNAME),
   DEFINE_MESSAGE(WM_CHANGECBCHAIN),
   DEFINE_MESSAGE(WM_HSCROLLCLIPBOARD),
   DEFINE_MESSAGE(WM_QUERYNEWPALETTE),
   DEFINE_MESSAGE(WM_PALETTEISCHANGING),
   DEFINE_MESSAGE(WM_PALETTECHANGED),
   DEFINE_MESSAGE(WM_DDE_INITIATE),
   DEFINE_MESSAGE(WM_DDE_TERMINATE),
   DEFINE_MESSAGE(WM_DDE_ADVISE),
   DEFINE_MESSAGE(WM_DDE_UNADVISE),
   DEFINE_MESSAGE(WM_DDE_ACK),
   DEFINE_MESSAGE(WM_DDE_DATA),
   DEFINE_MESSAGE(WM_DDE_REQUEST),
   DEFINE_MESSAGE(WM_DDE_POKE),
   DEFINE_MESSAGE(WM_DDE_EXECUTE),
   DEFINE_MESSAGE(WM_DROPFILES),
   DEFINE_MESSAGE(WM_POWER),
   DEFINE_MESSAGE(WM_WINDOWPOSCHANGED),
   DEFINE_MESSAGE(WM_WINDOWPOSCHANGING),
// ca2 API specific messages
   DEFINE_MESSAGE(WM_SIZEPARENT),
   DEFINE_MESSAGE(WM_SETMESSAGESTRING),
   DEFINE_MESSAGE(WM_IDLEUPDATECMDUI),
   DEFINE_MESSAGE(WM_INITIALUPDATE),
   DEFINE_MESSAGE(WM_COMMANDHELP),
   DEFINE_MESSAGE(WM_HELPHITTEST),
   DEFINE_MESSAGE(WM_EXITHELPMODE),
   DEFINE_MESSAGE(WM_HELP),
   DEFINE_MESSAGE(WM_NOTIFY),
   DEFINE_MESSAGE(WM_CONTEXTMENU),
   DEFINE_MESSAGE(WM_TCARD),
   DEFINE_MESSAGE(WM_MDIREFRESHMENU),
   DEFINE_MESSAGE(WM_MOVING),
   DEFINE_MESSAGE(WM_STYLECHANGED),
   DEFINE_MESSAGE(WM_STYLECHANGING),
   DEFINE_MESSAGE(WM_SIZING),
   DEFINE_MESSAGE(WM_SETHOTKEY),
   DEFINE_MESSAGE(WM_PRINT),
   DEFINE_MESSAGE(WM_PRINTCLIENT),
   DEFINE_MESSAGE(WM_POWERBROADCAST),
   DEFINE_MESSAGE(WM_HOTKEY),
   DEFINE_MESSAGE(WM_GETICON),
   DEFINE_MESSAGE(WM_EXITMENULOOP),
   DEFINE_MESSAGE(WM_ENTERMENULOOP),
   DEFINE_MESSAGE(WM_DISPLAYCHANGE),
   DEFINE_MESSAGE(WM_STYLECHANGED),
   DEFINE_MESSAGE(WM_STYLECHANGING),
   DEFINE_MESSAGE(WM_GETICON),
   DEFINE_MESSAGE(WM_SETICON),
   DEFINE_MESSAGE(WM_SIZING),
   DEFINE_MESSAGE(WM_MOVING),
   DEFINE_MESSAGE(WM_CAPTURECHANGED),
   DEFINE_MESSAGE(WM_DEVICECHANGE),
   { 0, NULL, }    // end of message list
};

#undef DEFINE_MESSAGE

/////////////////////////////////////////////////////////////////////////////
// DDE special case

static void TraceDDE(const char * lpszPrefix, const MSG* pMsg)
{
   ENSURE_ARG(pMsg != NULL);
   if (pMsg->message == WM_DDE_EXECUTE)
   {
      UINT_PTR nDummy;
      HGLOBAL hCommands;
      if (!UnpackDDElParam(WM_DDE_EXECUTE, pMsg->lParam,
         &nDummy, (UINT_PTR*)&hCommands))
      {
//         ::OutputDebugString(::radix::trace::category_AppMsg, 0, "Warning: Unable to unpack WM_DDE_EXECUTE lParam %08lX.\n",
  //          pMsg->lParam);
         return;
      }
      ASSERT(hCommands != NULL);

      const char * lpszCommands = (const char *)::GlobalLock(hCommands);
      ENSURE_THROW(lpszCommands != NULL, ::AfxThrowMemoryException() );
//      ::OutputDebugString(::radix::trace::category_AppMsg, 0, "%s: Execute '%s'.\n", lpszPrefix, lpszCommands);
      ::GlobalUnlock(hCommands);
   }
   else if (pMsg->message == WM_DDE_ADVISE)
   {
      UINT_PTR nItem;
      ATOM aItem;
      HGLOBAL hAdvise;
      if (!UnpackDDElParam(WM_DDE_ADVISE, pMsg->lParam,
         (UINT_PTR*)&hAdvise, &nItem))
      {
//         ::OutputDebugString(::radix::trace::category_AppMsg, 0, "Warning: Unable to unpack WM_DDE_ADVISE lParam %08lX.\n",
//            pMsg->lParam);
       return;
      }
      aItem = (ATOM)nItem;
      ASSERT(aItem != NULL);
      ASSERT(hAdvise != NULL);

      DDEADVISE* lpAdvise = (DDEADVISE*)::GlobalLock(hAdvise);
      ENSURE_THROW(lpAdvise != NULL, ::AfxThrowMemoryException() );
      char szItem[80];
      szItem[0] = '\0';

      if (aItem != 0)
         ::GlobalGetAtomName(aItem, szItem, _countof(szItem));

      char szFormat[80];
      szFormat[0] = '\0';
      if (((UINT)0xC000 <= (UINT)lpAdvise->cfFormat) &&
            ((UINT)lpAdvise->cfFormat <= (UINT)0xFFFF))
      {
         ::GetClipboardFormatName(lpAdvise->cfFormat,
            szFormat, _countof(szFormat));

         // User defined clipboard formats have a range of 0xC000->0xFFFF
         // System clipboard formats have other ranges, but no printable
         // format names.
      }

      AfxTrace(
         "%s: Advise item='%s', Format='%s', Ack=%d, Defer Update= %d\n",
          lpszPrefix, szItem, szFormat, lpAdvise->fAckReq,
         lpAdvise->fDeferUpd);
      ::GlobalUnlock(hAdvise);
   }
}

/////////////////////////////////////////////////////////////////////////////

void _AfxTraceMsg(const char * lpszPrefix, gen::signal_object * pobj)
{
   ENSURE_ARG(AfxIsValidString(lpszPrefix));
   ENSURE_ARG(pobj != NULL);
   SCAST_PTR(::gen::message::base, pbase, pobj);

   if (pbase->m_uiMessage == WM_MOUSEMOVE || pbase->m_uiMessage == WM_NCMOUSEMOVE ||
      pbase->m_uiMessage == WM_NCHITTEST || pbase->m_uiMessage == WM_SETCURSOR ||
      pbase->m_uiMessage == WM_CTLCOLORBTN ||
      pbase->m_uiMessage == WM_CTLCOLORDLG ||
      pbase->m_uiMessage == WM_CTLCOLOREDIT ||
      pbase->m_uiMessage == WM_CTLCOLORLISTBOX ||
      pbase->m_uiMessage == WM_CTLCOLORMSGBOX ||
      pbase->m_uiMessage == WM_CTLCOLORSCROLLBAR ||
      pbase->m_uiMessage == WM_CTLCOLORSTATIC ||
      pbase->m_uiMessage == WM_ENTERIDLE || pbase->m_uiMessage == WM_CANCELMODE ||
      pbase->m_uiMessage == 0x0118)    // WM_SYSTIMER (caret blink)
   {
      // don't report very frequently sent messages
      return;
   }

   const char * lpszMsgName = NULL;
   char szBuf[80];

   // find message name
   if (pbase->m_uiMessage >= 0xC000)
   {
      // Window message registered with 'RegisterWindowMessage'
      //  (actually a USER atom)
      if (::GetClipboardFormatNameA(pbase->m_uiMessage, szBuf, _countof(szBuf)))
         lpszMsgName = szBuf;
   }
   else if (pbase->m_uiMessage >= WM_USER)
   {
      // User message
      sprintf_s(szBuf, _countof(szBuf), "WM_USER+0x%04X", pbase->m_uiMessage - WM_USER);
      lpszMsgName = szBuf;
   }
   else
   {
      // a system windows message
      const AFX_MAP_MESSAGE* pMapMsg = allMessages;
      for (/*null*/; pMapMsg->lpszMsg != NULL; pMapMsg++)
      {
         if (pMapMsg->nMsg == pbase->m_uiMessage)
         {
            lpszMsgName = pMapMsg->lpszMsg;
            break;
         }
      }
   }

   if (lpszMsgName != NULL)
   {
#ifdef _WIN64
      //TRACE(::radix::trace::category_WinMsg, 4, "%s: hwnd=%p, msg = %s (%p, %p)\n",
        // lpszPrefix, pbase->m_hwnd, lpszMsgName,
         //pbase->m_wparam, pbase->m_lparam);
#else
    //  ::OutputDebugString(::radix::trace::category_WinMsg, 4, "%s: hwnd=0x%08X, msg = %s (0x%08X, 0x%08X)\n",
//         lpszPrefix, pbase->m_hwnd, lpszMsgName,
  //       pbase->m_wparam, pbase->m_lparam);
#endif
   }
   else
   {
#ifdef _WIN64
//      ::OutputDebugString(::radix::trace::category_WinMsg, 4, "%s: hwnd=%p, msg = 0x%04X (%p, %p)\n",
  //       lpszPrefix, pbase->m_hwnd, lpszMsgName,
    //     pbase->m_wparam, pbase->m_lparam);
#else
//      ::OutputDebugString(::radix::trace::category_WinMsg, 4, "%s: hwnd=0x%08X, msg = 0x%04X (0x%08X, 0x%08X)\n",
  //       lpszPrefix, pbase->m_hwnd, lpszMsgName,
    //     pbase->m_wparam, pbase->m_lparam);
#endif
   }

   /*if (pbase->m_uiMessage >= WM_DDE_FIRST && pbase->m_uiMessage <= WM_DDE_LAST)
      TraceDDE(lpszPrefix, pMsg);*/
}


void _AfxTraceMsg(const char * lpszPrefix, LPMSG lpmsg)
{
   ENSURE_ARG(AfxIsValidString(lpszPrefix));
   ENSURE_ARG(lpmsg != NULL);

   if (lpmsg->message == WM_MOUSEMOVE || lpmsg->message == WM_NCMOUSEMOVE ||
      lpmsg->message == WM_NCHITTEST || lpmsg->message == WM_SETCURSOR ||
      lpmsg->message == WM_CTLCOLORBTN ||
      lpmsg->message == WM_CTLCOLORDLG ||
      lpmsg->message == WM_CTLCOLOREDIT ||
      lpmsg->message == WM_CTLCOLORLISTBOX ||
      lpmsg->message == WM_CTLCOLORMSGBOX ||
      lpmsg->message == WM_CTLCOLORSCROLLBAR ||
      lpmsg->message == WM_CTLCOLORSTATIC ||
      lpmsg->message == WM_ENTERIDLE || lpmsg->message == WM_CANCELMODE ||
      lpmsg->message == 0x0118)    // WM_SYSTIMER (caret blink)
   {
      // don't report very frequently sent messages
      return;
   }

   const char * lpszMsgName = NULL;
   char szBuf[80];

   // find message name
   if (lpmsg->message >= 0xC000)
   {
      // Window message registered with 'RegisterWindowMessage'
      //  (actually a USER atom)
      if (::GetClipboardFormatNameA(lpmsg->message, szBuf, _countof(szBuf)))
         lpszMsgName = szBuf;
   }
   else if (lpmsg->message >= WM_USER)
   {
      // User message
      sprintf_s(szBuf, _countof(szBuf), "WM_USER+0x%04X", lpmsg->message - WM_USER);
      lpszMsgName = szBuf;
   }
   else
   {
      // a system windows message
      const AFX_MAP_MESSAGE* pMapMsg = allMessages;
      for (/*null*/; pMapMsg->lpszMsg != NULL; pMapMsg++)
      {
         if (pMapMsg->nMsg == lpmsg->message)
         {
            lpszMsgName = pMapMsg->lpszMsg;
            break;
         }
      }
   }

   if (lpszMsgName != NULL)
   {
#ifdef WIN64
//      TRACE(::radix::trace::category_WinMsg, 4, "%s: hwnd=%p, msg = %hs (%p, %p)\n",
  //       lpszPrefix, pMsg->hwnd, lpszMsgName,
    //     pMsg->wParam, pMsg->lParam);
#else
//      ::OutputDebugString(::radix::trace::category_WinMsg, 4, "%s: hwnd=0x%08X, msg = %hs (0x%08X, 0x%08X)\n",
  //       lpszPrefix, lpmsg->hwnd, lpszMsgName,
    //     lpmsg->wParam, lpmsg->lParam);
#endif
   }
   else
   {
#ifdef WIN64
      //::OutputDebugString(::radix::trace::category_WinMsg, 4, "%s: hwnd=%p, msg = 0x%04X (%p, %p)\n",
        // lpszPrefix, pMsg->hwnd, lpszMsgName,
         //pMsg->wParam, pMsg->lParam);
#else
      //::OutputDebugString(::radix::trace::category_WinMsg, 4, "%s: hwnd=0x%08X, msg = 0x%04X (0x%08X, 0x%08X)\n",
        // lpszPrefix, lpmsg->hwnd, lpszMsgName,
         //lpmsg->wParam, lpmsg->lParam);
#endif
   }

   /*if (lpmsg->message >= WM_DDE_FIRST && lpmsg->message <= WM_DDE_LAST)
      TraceDDE(lpszPrefix, pMsg);*/
}
