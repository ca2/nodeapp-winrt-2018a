#include "framework.h"



#ifdef METROWIN
[Platform::MTAThread]
#endif
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
   UNREFERENCED_PARAMETER(lpReserved);

   if (dwReason == DLL_PROCESS_ATTACH)
   {
      ::OutputDebugString(L"::ca2:: metrowin.dll :: initializing!\n");
      

      ::ca::g_pfn_get_thread = &::metrowin::get_thread;
      ::ca::g_pfn_get_thread_state = &::metrowin::get_thread_state;

   }
   else if (dwReason == DLL_PROCESS_DETACH)
   {

//      HMODULE hmodule;
   
      /*try
      {
      
         if(::GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, "c.dll", &hmodule) != FALSE)
         {

            try
            {
                  
               ::FreeLibrary(hmodule);

            }
            catch(...)
            {

            }

         }

      }
      catch(...)
      {
      } */

      ::OutputDebugStringW(L"::ca2:: metrowin.dll :: terminating!\n");

   }

   return 1;   // ok

}




