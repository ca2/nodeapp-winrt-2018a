#include "framework.h"


namespace metrowin
{

   file_system::file_system(::ca::application * papp) :
      ca(papp)
   {

   }


   file_system::~file_system()
   {

   }

   bool file_system::FullPath(string &str, const char * lpszFileIn)
   {
      /*      if(::ca::file_system::FullPath(str, lpszFileIn))
      return true;*/
      if(::ca::str::begins_ci(lpszFileIn, "http://"))
      {
         str = lpszFileIn;
         return true;
      }
      else if(::ca::str::begins_ci(lpszFileIn, "https://"))
      {
         str = lpszFileIn;
         return true;
      }
      wstring wstrFileIn;
      wstrFileIn = ::ca::international::utf8_to_unicode(lpszFileIn);
      wstring wstrFileOut;
      //      bool b = vfxFullPath(wstrFileOut.alloc(MAX_PATH * 8), wstrFileIn) != FALSE;
      /*      if(b)
      {
      ::ca::international::unicode_to_utf8(str, wstrFileOut);
      }
      return b;*/

      return false;
   }

   bool file_system::FullPath(wstring & wstrFullPath, const wstring & wstrPath)
   {

      //if(::ca::file_system::FullPath(wstrFullPath, wstrPath))
      // return true;

      if(::ca::str::begins_ci(wstrPath, L"http://"))
      {
         wstrFullPath = wstrPath;
         return true;
      }
      else if(::ca::str::begins_ci(wstrPath, L"https://"))
      {
         wstrFullPath = wstrPath;
         return true;
      }

      //return vfxFullPath(wstrFullPath, wstrPath) != FALSE;
      return false;

   }


   UINT file_system::GetFileName(const char * lpszPathName, string & str)
   {
      int nMax = MAX_PATH * 8;
      wstring wstrPathName;
      wstrPathName = ::ca::international::utf8_to_unicode(lpszPathName);
      wstring wstrTitle;
      //UINT user = vfxGetFileName(wstrPathName, wstrTitle.alloc(nMax), nMax);
      str = ::ca::international::unicode_to_utf8(wstrTitle);
      //return user;
      return 0;
   }

   void file_system::GetModuleShortFileName(HINSTANCE hInst, string & strShortName)
   {
      //vfxGetModuleShortFileName(hInst, strShortName);
   }


   var file_system::length(const char * pszPath)
   {

      var varRet;

      WIN32_FILE_ATTRIBUTE_DATA data;

      if(!GetFileAttributesExW(::ca::international::utf8_to_unicode(pszPath), GetFileExInfoStandard, &data))
      {
         varRet.set_type(var::type_null);
      }
      else
      {
         varRet = (uint32_t) data.nFileSizeLow;
      }


      return varRet;

   }



} // namespace metrowin




