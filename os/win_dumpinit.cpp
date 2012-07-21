#include "framework.h"


/////////////////////////////////////////////////////////////////////////////
// ___DEBUG_STATE implementation

#ifndef ___NO_DEBUG_CRT
static _CRT_DUMP_CLIENT pfnOldCrtDumpClient = NULL;

#ifdef DEBUG


void __cdecl __crt_dump_client(void * pvData, size_t nBytes)
{
   char sz[1024];
   try
   {
      
      if(_CrtReportBlockType(pvData) != ___CLIENT_BLOCK)
         return;

//      ::radix::object * pca = (::radix::object * ) pvData;

      ::radix::object * pobject = NULL;

      /*for(int i = 0; i < 256; i++)
      {
         try
         {
            pobject = dynamic_cast < ::radix::object * > ((::radix::object *)&((int_ptr *)pca)[i]);
         }
         catch(std::__non_rtti_object & e)
         {
            pobject = NULL;
         }
         catch(...)
         {
            pobject = NULL;
         }
         if(pobject != NULL)
            break;
      }*/

      if(pobject == NULL)
      {
            C_RUNTIME_ERRORCHECK_SPRINTF(_snprintf_s(sz, _countof(sz), _countof(sz) - 1, "unknown object at $%p, %u bytes long\n", pvData, nBytes));
      }
      else if(g_dumpcontext.GetDepth() > 0)
      {
         // long form
         pobject->dump(g_dumpcontext);
         g_dumpcontext << "\n";
      }
      if(false) // else
      {
         ::radix::object & obj = *pobject;
         // short form
         C_RUNTIME_ERRORCHECK_SPRINTF(_snprintf_s(sz, _countof(sz), _countof(sz) - 1, "a %hs object at $%p, %u bytes long\n", typeid(obj).name(), pvData, nBytes));
         g_dumpcontext << sz;
      }
   }
   catch(std::__non_rtti_object & e)
   {
      g_dumpcontext << "_gen::CrtdumpClient __non_rtti_object ";
      g_dumpcontext << e.what();
   }
   catch(...)
   {
      // short form for trashed objects
      sprintf_s(sz, _countof(sz), "faulted while dumping object at $%p, %u bytes long\n", pvData, nBytes);
      g_dumpcontext << sz;
   }
   if (pfnOldCrtDumpClient != NULL)
      (*pfnOldCrtDumpClient)(pvData, nBytes);
}

int __cdecl __crt_report_hook(int nRptType, __in char *szMsg, int* pResult)
{
   // no hook on asserts or when m_pFile is NULL
   if (nRptType == _CRT_ASSERT || g_dumpcontext.m_pFile == NULL)
      return FALSE;

   ASSERT( pResult != NULL );
   if( pResult == NULL )
      throw invalid_argument_exception();

   ASSERT( szMsg != NULL );
   if( szMsg == NULL )
      throw invalid_argument_exception();

   // non-NULL m_pFile, so go through g_dumpcontext for the message
   *pResult = FALSE;
   g_dumpcontext << szMsg;
   //Allow other report hooks to be called.
   return FALSE;
}

#endif
#endif // ___NO_DEBUG_CRT


#ifdef DEBUG

___DEBUG_STATE::___DEBUG_STATE()
{
#ifndef ___NO_DEBUG_CRT
   ASSERT(pfnOldCrtDumpClient == NULL);
   pfnOldCrtDumpClient = _CrtSetDumpClient(__crt_dump_client);

   ASSERT(_CrtSetReportHook2(_CRT_RPTHOOK_INSTALL,__crt_report_hook) != -1);
   _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_WNDW);
#endif // ___NO_DEBUG_CRT
}

___DEBUG_STATE::~___DEBUG_STATE()
{
#ifndef ___NO_DEBUG_CRT
   if(::IsDebuggerPresent() && false)
   {
      try
      {
         _CrtDumpMemoryLeaks();
      }
      catch(std::__non_rtti_object & e)
      {
         ::OutputDebugString("~___DEBUG_STATE _CrtdumpMemoryLeaks std::__non_rtti_object\n");
         ::OutputDebugString(e.what());
         ::OutputDebugString("\n");
      }
      catch(...)
      {
         ::OutputDebugString("~___DEBUG_STATE _CrtdumpMemoryLeaks exception\n");
      }
   }
   int nOldState = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
   _CrtSetDbgFlag(nOldState & ~_CRTDBG_LEAK_CHECK_DF);

   ASSERT(_CrtSetReportHook2(_CRT_RPTHOOK_REMOVE,__crt_report_hook) != -1);   
   _CrtSetDumpClient(pfnOldCrtDumpClient);
#endif // ___NO_DEBUG_CRT
}

PROCESS_LOCAL(___DEBUG_STATE, afxDebugState)

bool CLASS_DECL_win __diagnostic_init()
{
   // just get the debug state to cause initialization
   ___DEBUG_STATE* pState = afxDebugState.get_data();
   ASSERT(pState != NULL);

   return TRUE;
}

#endif


#include "framework.h"
#include "sal.h"


//__DATADEF bool g_bTraceEnabled = TRUE;
//__DATADEF UINT g_uiTraceFlags = 0;
static bool gen_DiagnosticInit = __diagnostic_init();

