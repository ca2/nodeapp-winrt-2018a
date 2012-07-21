#include "framework.h"


namespace win
{


   dir::dir(::ca::application * papp) :
      ca(papp),
      ::ca::dir::system(papp),
      m_path(papp)
   {
      
      string strCa2Module = ca2module();
      
      m_strCa2 = strCa2Module;

      System.file().path().eat_end_level(m_strCa2, 2, "\\");

   }

   path::path(::ca::application * papp) :
      ca(papp)
   {
   }


   inline bool myspace(char ch)
   {
      return ch == ' ' ||
             ch == '\t' ||
             ch == '\r' ||
             ch == '\n';
   }

   string dir::path(const char * pszFolder, strsize iLenFolder, const char * pszRelative, strsize iLenRelative, const char * psz2, strsize iLen2, bool bUrl)
   {

      bool bEmptyRelative = iLenRelative == 0 || pszRelative == NULL || *pszRelative == '\0';
      bool bEmpty2 = iLen2 == 0 || psz2 == NULL || *psz2 == '\0';

      if(bEmptyRelative && bEmpty2)
         return pszFolder;

      string strPath;
      LPSTR lpsz;

      if(bEmptyRelative)
      {
         pszRelative = psz2;
         iLenRelative = iLen2;
      }

      while((pszFolder[iLenFolder - 1] == '\\' || pszFolder[iLenFolder - 1] == '/') && iLenFolder > 0)
      {
         if(bUrl)
         {
            if((iLenFolder - 2) >= 0 && (pszFolder[iLenFolder - 2] == '\\' || pszFolder[iLenFolder - 2] == '/' || pszFolder[iLenFolder - 2] == ':'))
            {
               if(pszFolder[iLenFolder - 2] == ':')
               {
                  break;
               }
               else
               {
                  if((iLenFolder - 3) >= 0 && (pszFolder[iLenFolder - 3] == ':'))
                  {
                     iLenFolder--;
                     break;
                  }
               }
            }
         }
         else
         {
            iLenFolder--;
         }
      }

      while(*pszRelative != '\0' && (*pszRelative == '\\' || *pszRelative == '/') && iLenRelative > 0)
      {
         pszRelative++;
         iLenRelative--;
      }

      if(bEmptyRelative || bEmpty2)
      {
         lpsz = strPath.GetBufferSetLength(iLenFolder + 1 + iLenRelative);
         strncpy(lpsz, pszFolder, iLenFolder);
         if(strnicmp(&lpsz[iLenFolder - 5], ".zip:", 5) == 0)
         {
            iLenFolder--;
         }
         else
         {
            lpsz[iLenFolder] = '/';
         }
         strncpy(&lpsz[iLenFolder + 1], pszRelative, iLenRelative);
         lpsz[iLenFolder + 1 + iLenRelative] = '\0';
         {
            if(bUrl)
            {
               while(*lpsz++ != '\0')
                  if(*lpsz == '\\') *lpsz = '/';
            }
            else
            {
               while(*lpsz++ != '\0')
                  if(*lpsz == '/') *lpsz = '\\';
            }
         }
         strPath.ReleaseBuffer(iLenFolder + 1 + iLenRelative);
         return strPath;
      }

      while((pszRelative[iLenRelative - 1] == '\\' || pszRelative[iLenRelative - 1] == '/') && iLenRelative > 0)
      {
         iLenRelative--;
      }

      while(*psz2 != '\0' && (*psz2 == '\\' || *psz2 == '/') && iLen2 > 0)
      {
         psz2++;
         iLen2--;
      }

      lpsz = strPath.GetBufferSetLength(iLenFolder + 1 + iLenRelative + 1 + iLen2);
      strncpy(lpsz, pszFolder, iLenFolder);
      lpsz[iLenFolder] = '/';
      strncpy(&lpsz[iLenFolder + 1], pszRelative, iLenRelative);
      lpsz[iLenFolder + 1 + iLenRelative] = '/';
      strncpy(&lpsz[iLenFolder + 1 + iLenRelative + 1], psz2, iLen2);
      lpsz[iLenFolder + 1 + iLenRelative + 1 + iLen2] = '\0';
      {
         if(bUrl)
         {
            while(*lpsz++ != '\0')
               if(*lpsz == '\\') *lpsz = '/';
         }
         else
         {
            while(*lpsz++ != '\0')
               if(*lpsz == '/') *lpsz = '\\';
         }
      }
      strPath.ReleaseBuffer(iLenFolder + 1 + iLenRelative + 1 + iLen2);
      return strPath;
   }



   string dir::relpath(const char * lpcszSource, const char * lpcszRelative, const char * psz2)
   {
      const char * pszRequest;
      if(::ca2::is_url(lpcszSource, &pszRequest))
      {
         if(gen::str::begins(lpcszRelative, "/"))
         {
            return path((const char *) string(lpcszSource, pszRequest - lpcszSource), lpcszRelative);
         }
         else if(*pszRequest == '\0' || gen::str::ends(lpcszSource, "/"))
         {
            return path(lpcszSource, lpcszRelative, psz2);
         }
         else
         {
            return path((const char *) name(lpcszSource), lpcszRelative, psz2);
         }
      }
      else
      {
         if(gen::str::ends(lpcszSource, "\\") || gen::str::ends(lpcszSource, "/"))
         {
            return path(lpcszSource, lpcszRelative, psz2);
         }
         else
         {
            return path((const char *) name(lpcszSource), lpcszRelative, psz2);
         }
      }
   }

   bool path::is_equal(const char * lpcsz1, const char * lpcsz2)
   {
      return System.file_system().ComparePath(lpcsz1, lpcsz2);
   }

   void dir::root_ones(stringa & stra, ::ca::application * papp)
   {
      DWORD dwSize = ::GetLogicalDriveStrings(0, NULL);
      LPTSTR lpszAlloc = (LPTSTR) malloc(dwSize + 1);
      LPTSTR lpsz = lpszAlloc;
      dwSize = ::GetLogicalDriveStrings(dwSize + 1, lpsz);

      string str;
      while(*lpsz)
      {
         str.Empty();
         while(*lpsz)
         {
            str += *lpsz;  
            lpsz++;
         }
         stra.add(str);
         lpsz++;
      }

      free(lpszAlloc);
   }

   void dir::ls_pattern(::ca::application * papp, const char * lpcsz, const char * pszPattern, stringa * pstraPath, stringa * pstraTitle, base_array < bool, bool > * pbaIsDir, base_array < int64_t, int64_t > * piaSize)
   {
      if(::ca::dir::system::is(lpcsz, papp)) // if base class "already" "says" it is a dir, let it handle it: may be not a operational system dir, e.g., zip or compressed directory...
      {
         return ::ca::dir::system::ls_pattern(papp, lpcsz, pszPattern, pstraPath, pstraTitle, pbaIsDir, piaSize);
      }
      FileFind filefind;
      bool bWorking;
      bWorking = filefind.FindFile(System.dir().path(lpcsz, pszPattern));
      while(bWorking)
      {
         bWorking = filefind.FindNextFileA();
         if(!filefind.IsDots())
         {
            if(pstraPath != NULL)
            {
               pstraPath->add(filefind.GetFilePath());
            }
            if(pstraTitle != NULL)
            {
               pstraTitle->add(filefind.GetFileName());
            }
            if(pbaIsDir != NULL)
            {
               pbaIsDir->add(filefind.IsDirectory() != FALSE);
            }
            if(piaSize != NULL)
            {
               piaSize->add(filefind.get_length());
            }
         }
      }
   }

   void dir::rls(::ca::application * papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle, stringa * pstraRelative, e_extract eextract)
   {
      rls_pattern(papp, lpcsz, "*.*", pstraPath, pstraTitle, pstraRelative, NULL, NULL, eextract);
   }

   void dir::rls_pattern(::ca::application * papp, const char * lpcsz, const char * lpszPattern, stringa * pstraPath, stringa * pstraTitle, stringa * pstraRelative, base_array < bool, bool > * pbaIsDir, base_array < int64_t, int64_t > * piaSize, e_extract eextract)
   {
      stringa straDir;
      ls_dir(papp, lpcsz, &straDir);
      for(int i = 0; i < straDir.get_count(); i++)
      {
         string strDir = straDir[i];
         if(strDir == lpcsz)
            continue;
         index iStart = 0;
         if(pstraRelative != NULL)
         {
            iStart = pstraRelative->get_size();
         }
         rls_pattern(papp, strDir, lpszPattern, pstraPath, pstraTitle, pstraRelative, pbaIsDir, piaSize, eextract == extract_all ? extract_all : extract_none);
         if(pstraRelative != NULL)
         {
            for(index i = iStart; i < pstraRelative->get_size(); i++)
            {
               pstraRelative->element_at(i) = System.dir().path(System.file().name_(strDir), pstraRelative->element_at(i));
            }
         }
      }

      FileFind filefind;
      bool bWorking = filefind.FindFile(System.dir().path(lpcsz, lpszPattern)) != FALSE;
      if(bWorking)
      {
         while(bWorking)
         {
            bWorking = filefind.FindNextFileA() != FALSE;
            if(!filefind.IsDots() && filefind.GetFilePath() != lpcsz)
            {
               if(pstraPath != NULL)
               {
                  pstraPath->add(filefind.GetFilePath());
               }
               if(pstraTitle != NULL)
               {
                  pstraTitle->add(filefind.GetFileName());
               }
               if(pstraRelative != NULL)
               {
                  pstraRelative->add(filefind.GetFileName());
               }
               if(pbaIsDir != NULL)
               {
                  pbaIsDir->add(filefind.IsDirectory() != FALSE);
               }
               if(piaSize != NULL)
               {
                  piaSize->add(filefind.get_length());
               }
               /*if(filefind.IsDirectory())
               {
                  int iStart = 0;
                  if(pstraRelative != NULL)
                  {
                     iStart = pstraRelative->get_size();
                  }
                  rls_pattern(filefind.GetFilePath(), lpszPattern, pstraPath, pstraTitle, pstraRelative, pbaIsDir, piaSize);
                  if(pstraRelative != NULL)
                  {
                     for(int i = iStart; i < pstraRelative->get_size(); i++)
                     {
                        pstraRelative->element_at(i) = System.dir().path(filefind.GetFileName(), pstraRelative->element_at(i));
                     }
                  }
               }*/
            }
         }
      }
      else
      {
         ::ca::dir::system::rls(papp, lpcsz, pstraPath, pstraTitle, pstraRelative, eextract == extract_all ? extract_all : extract_none);
      }
   }

   void dir::rls_dir(::ca::application * papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle, stringa * pstraRelative)
   {
      FileFind filefind;
      bool bWorking;
      bWorking = filefind.FindFile(System.dir().path(lpcsz, "*.*"));
      while(bWorking)
      {
         bWorking = filefind.FindNextFileA();
         if(!filefind.IsDots() && filefind.IsDirectory())
         {
            if(pstraPath != NULL)
            {
               pstraPath->add(filefind.GetFilePath());
            }
            if(pstraTitle != NULL)
            {
               pstraTitle->add(filefind.GetFileName());
            }
            if(pstraRelative != NULL)
            {
               pstraRelative->add(filefind.GetFileName());
            }
            if(filefind.IsDirectory())
            {
               index iStart = 0;
               if(pstraRelative != NULL)
               {
                  iStart = pstraRelative->get_size();
               }
               rls_dir(papp, filefind.GetFilePath(), pstraPath, pstraTitle, pstraRelative);
               if(pstraRelative != NULL)
               {
                  for(index i = iStart; i < pstraRelative->get_size(); i++)
                  {
                     pstraRelative->element_at(i) = System.dir().path(filefind.GetFileName(), pstraRelative->element_at(i));
                  }
               }
            }
         }
      }
   }

   void dir::ls_dir(::ca::application * papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle)
   {
      
      FileFind filefind;
      bool bWorking;
      bWorking = filefind.FindFile(System.dir().path(lpcsz, "*.*"));
      if(!bWorking)
      {
         ::ca::dir::system::ls_dir(papp, lpcsz, pstraPath, pstraTitle);
         return;
      }
      while(bWorking)
      {
         bWorking = filefind.FindNextFileA();
         if(filefind.IsDirectory() && !filefind.IsDots())
         {
            if(pstraPath != NULL)
            {
               pstraPath->add(filefind.GetFilePath());
            }
            if(pstraTitle != NULL)
            {
               pstraTitle->add(filefind.GetFileName());
            }
         }
      }
   }

   void dir::ls_file(::ca::application * papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle)
   {
      FileFind filefind;
      bool bWorking;
      bWorking = filefind.FindFile(System.dir().path(lpcsz, "*.*"));
      while(bWorking)
      {
         bWorking = filefind.FindNextFileA();
         if(!filefind.IsDirectory() && !filefind.IsDots())
         {
            if(pstraPath != NULL)
            {
               pstraPath->add(filefind.GetFilePath());
            }
            if(pstraTitle != NULL)
            {
               pstraTitle->add(filefind.GetFileName());
            }
         }
      }
   }

   void dir::ls(::ca::application * papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle, base_array < bool, bool > * pbaIsDir, base_array < int64_t, int64_t > * piaSize)
   {
      return ls_pattern(papp, lpcsz, "*.*", pstraPath, pstraTitle, pbaIsDir, piaSize);
   }

   bool dir::is(const char * lpcszPath, ::ca::application * papp)
   {
      
      bool bIsDir;

      if(m_isdirmap.lookup(lpcszPath, bIsDir))
         return bIsDir;

      if(::ca::dir::system::is(lpcszPath, papp))
         return true;


      string strPath(lpcszPath);
      if(strPath.get_length() >= MAX_PATH)
      {
         if(gen::str::begins(strPath, "\\\\"))
         {
            strPath = "\\\\?\\UNC" + strPath.Mid(1);
         }
         else
         {
            strPath = "\\\\?\\" + strPath;
         }
      }
      DWORD dwAttrib;
      dwAttrib = GetFileAttributesW(gen::international::utf8_to_unicode(strPath));
      /*if(dwAttrib == INVALID_FILE_ATTRIBUTES)
      {
         dwAttrib = GetFileAttributes(lpcszPath);
      }*/
      
      bIsDir = (dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
      
      m_isdirmap.set(lpcszPath, bIsDir);

      return bIsDir;
   }
      
   bool dir::is(const string & strPath, ::ca::application * papp)
   {
      
      if(::ca::dir::system::is(strPath, papp))
         return true;

      bool bIsDir;

      if(m_isdirmap.lookup(strPath, bIsDir))
         return bIsDir;

      wstring wstrPath;
      
      //strsize iLen = ::gen::international::utf8_to_unicode_count(strPath);
      //wstrPath.alloc(iLen + 32);
      wstrPath = ::gen::international::utf8_to_unicode(strPath);
      if(wstrPath.get_length() >= MAX_PATH)
      {
         if(::gen::str::begins(wstrPath, L"\\\\"))
         {
            ::gen::str::begin(wstrPath, L"\\\\?\\UNC");
         }
         else
         {
            ::gen::str::begin(wstrPath, L"\\\\?\\");
         }
      }
      DWORD dwAttrib;
      dwAttrib = GetFileAttributesW(wstrPath);
      /*if(dwAttrib == INVALID_FILE_ATTRIBUTES)
      {
         dwAttrib = GetFileAttributes(strPath);
      }*/
      
      bIsDir = (dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
      
      m_isdirmap.set(strPath, bIsDir);

      return bIsDir;
   }

   bool dir::name_is(const string & str, ::ca::application * papp)
   {
      //OutputDebugString(str);
      strsize iLast = str.get_length() - 1;
      while(iLast >= 0)
      {
         if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
            break;
         iLast--;
      }
      while(iLast >= 0)
      {
         if(str.m_pszData[iLast] == '\\' || str.m_pszData[iLast] == '/' || str.m_pszData[iLast] == ':')
            break;
         iLast--;
      }
      if(iLast >= 0)
      {
         while(iLast >= 0)
         {
            if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
            {
               iLast++;
               break;
            }
            iLast--;
         }
      }
      else
      {
         return true; // assume empty string is root_ones directory
      }

      
      bool bIsDir;


      if(m_isdirmap.lookup(str, bIsDir, (int) iLast))
         return bIsDir;


      if(papp->m_bZipIsDir && iLast >= 3  && !strnicmp_dup(&((const char *) str)[iLast - 3], ".zip", 4))
      {
         m_isdirmap.set(str.Left(iLast + 1), true);
         return true;
      }
      
      strsize iFind = gen::str::find_ci(".zip:", str);

      if(papp->m_bZipIsDir && iFind >= 0 && iFind < iLast)
      {
         bool bHasSubFolder;
         if(m_isdirmap.lookup(str, bHasSubFolder))
            return bHasSubFolder;
         bHasSubFolder = m_pziputil->HasSubFolder(papp, str);
         m_isdirmap.set(str.Left(iLast + 1), bHasSubFolder);
         return bHasSubFolder;
      }


      wstring wstrPath;
      
      //strsize iLen = ::gen::international::utf8_to_unicode_count(str, iLast + 1);

      //wstrPath.alloc(iLen + 32);

      wstrPath = ::gen::international::utf8_to_unicode(str, iLast + 1);

      //OutputDebugStringW(wstrPath);

      if(wstrPath.get_length() >= MAX_PATH)
      {
         if(::gen::str::begins(wstrPath, L"\\\\"))
         {
            ::gen::str::begin(wstrPath, L"\\\\?\\UNC");
         }
         else
         {
            ::gen::str::begin(wstrPath, L"\\\\?\\");
         }
      }
      DWORD dwAttrib;
      dwAttrib = GetFileAttributesW(wstrPath);
      /*if(dwAttrib == INVALID_FILE_ATTRIBUTES)
      {
         dwAttrib = GetFileAttributes(strPath);
      }*/
      
      bIsDir = (dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
      
      m_isdirmap.set(str.Left(iLast + 1), bIsDir);

      return bIsDir;
   }

   string dir::votagus(const char * lpcsz, const char * lpcsz2)
   {
      string strVotagusFolder = System.get_ca2_module_folder();
      System.file().path().eat_end_level(strVotagusFolder, 2, "\\");
      return dir::path(strVotagusFolder, lpcsz, lpcsz2);
   }

   string dir::time(const char * lpcsz, const char * lpcsz2)
   {
      return dir::path(m_strTimeFolder, lpcsz, lpcsz2);
   }

   string dir::stage(const char * lpcsz, const char * lpcsz2)
   {
      return dir::path(ca2("stage"), lpcsz, lpcsz2);
   }

   string dir::stageapp(const char * lpcsz, const char * lpcsz2)
   {
      return dir::path(stage("basis", lpcsz), lpcsz2);
   }

   string dir::netseed(const char * lpcsz, const char * lpcsz2)
   {
      return dir::path(m_strNetSeedFolder, lpcsz, lpcsz2);
   }

   // stage in ccvotagus spalib
   string dir::ca2(const char * lpcsz, const char * lpcsz2)
   {
      
      single_lock sl(&m_mutex, true);

      return dir::path(m_strCa2, lpcsz, lpcsz2);

   }

   string dir::ca2(const string & str, const char * lpcsz2)
   {
      
      single_lock sl(&m_mutex, true);

      return dir::path(m_strCa2, str, lpcsz2);

   }

   string dir::ca2(const char * lpcsz, const string & str2)
   {
      
      single_lock sl(&m_mutex, true);

      return dir::path(m_strCa2, lpcsz, str2);

   }

   string dir::ca2(const string & str, const string & str2)
   {
      
      single_lock sl(&m_mutex, true);

      return dir::path(m_strCa2, str, str2);

   }

   string dir::ca2(const string & str)
   {
      
      single_lock sl(&m_mutex, true);

      return dir::path(m_strCa2, str);

   }

   string dir::ca2()
   {
      
      single_lock sl(&m_mutex, true);

      return m_strCa2;

   }


   string dir::module(const char * lpcsz, const char * lpcsz2)
   {
      string str = System.get_module_folder();
      return dir::path(str, lpcsz, lpcsz2);
   }

   string dir::ca2module(const char * lpcsz, const char * lpcsz2)
   {
      string str = System.get_ca2_module_folder();
      return dir::path(str, lpcsz, lpcsz2);
   }


   void dir::time_square(string &str)
   {
      str = time("time");
   }

   string dir::time_log(const char * pszId)
   {
      string strLogBaseDir;
      strLogBaseDir = appdata("log");
      return path(strLogBaseDir, pszId);
   }

   bool dir::mk(const char * lpcsz, ::ca::application * papp)
   {

      if(is(lpcsz, papp))
         return true;

      stringa stra;
      System.file().get_ascendants_path(lpcsz, stra);
      for(int i = 0; i < stra.get_size(); i++)
      {
         if(!is(stra[i], papp))
         {
            
            if(!::CreateDirectoryW(gen::international::utf8_to_unicode("\\\\?\\" + stra[i]), NULL))
            {
               DWORD dwError = ::GetLastError();
               if(dwError == ERROR_ALREADY_EXISTS)
               {
                  string str;
                  str = "\\\\?\\" + stra[i];
                  str.trim_right("\\/");
                  try
                  {
                     System.file().del(str);
                  }
                  catch(...)
                  {
                  }
                  str = stra[i];
                  str.trim_right("\\/");
                  try
                  {
                     System.file().del(str);
                  }
                  catch(...)
                  {
                  }
                  if(::CreateDirectoryW(gen::international::utf8_to_unicode("\\\\?\\" + stra[i]), NULL))
                  {
                     m_isdirmap.set(stra[i], true);
                     goto try1;
                  }
                  else
                  {
                     dwError = ::GetLastError();
                  }
               }
               char * pszError;
               FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, (LPTSTR) &pszError, 8, NULL);

               //TRACE("dir::mk CreateDirectoryW last error(%d)=%s", dwError, pszError);
               ::LocalFree(pszError);
               //m_isdirmap.set(stra[i], false);
            }
            else
            {
               m_isdirmap.set(stra[i], true);
            }
            try1:
            
            if(!is(stra[i], papp))
            {
               return false;
            }
            
         }
      }
      return true;
   }

   bool dir::rm(::ca::application * papp, const char * psz, bool bRecursive)
   {
      if(bRecursive)
      {
         stringa straPath;
         stringa straTitle;
         ls(papp, psz, &straPath, &straTitle);         
         for(int i = 0; i < straPath.get_count(); i++)
         {
            if(is(straPath[i], papp))
            {
               rm(papp, path(psz, straTitle[i]), true);
            }
            else
            {
               ::DeleteFile(straPath[i]);
            }
         }
      }
      return RemoveDirectory(psz) != FALSE;
   }


   string dir::name(const char * path1)
   {
      const char * psz = path1 + strlen(path1) - 1;
      while(psz >= path1)
      {
         if(*psz != '\\' && *psz != '/' && *psz != ':')
            break;
         psz--;
      }
      while(psz >= path1)
      {
         if(*psz == '\\' || *psz == '/' || *psz == ':')
            break;
         psz--;
      }
      if(psz >= path1) // strChar == "\\" || strChar == "/"
      {
         const char * pszEnd = psz;
         /*while(psz >= path1)
         {
            if(*psz != '\\' && *psz != '/' && *psz != ':')
               break;
            psz--;
         }*/
         return string(path1, pszEnd - path1 + 1);
      }
      else
      {
         return "";
      }
   }

   string dir::name(const string & str)
   {
      
      strsize iLast = str.get_length() - 1;

      while(iLast >= 0)
      {
         if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
            break;
         iLast--;
      }
      while(iLast >= 0)
      {
         if(str.m_pszData[iLast] == '\\' || str.m_pszData[iLast] == '/' || str.m_pszData[iLast] == ':')
            break;
         iLast--;
      }
      if(iLast >= 0)
      {
         while(iLast >= 0)
         {
            if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
               break;
            iLast--;
         }
         return str.Left(iLast + 1);
      }
      else
      {
         return "";
      }
   }


   class ::ca::path & dir::path()
   {
      return m_path;
   }


   bool dir::initialize()
   {
      
      xml::document doc(get_app());
      
      doc.load(Application.file().as_string(appdata("configuration\\directory.xml")));
      
      if(doc.get_root()->get_name() == "directory_configuration")
      {

         m_strTimeFolder = doc.get_root()->get_child_value("time"); 

         m_strNetSeedFolder = doc.get_root()->get_child_value("netseed"); 

      }
      if(m_strTimeFolder.is_empty())
         m_strTimeFolder = appdata("time");

      if(m_strNetSeedFolder.is_empty())
         m_strNetSeedFolder = ca2("net/netseed");

      mk(m_strTimeFolder, get_app());

      if(!is(m_strTimeFolder, get_app()))
         return false;

      mk(path(m_strTimeFolder, "time"), get_app());

      return true;

   }

   string dir::trash_that_is_not_trash(const char * psz)
   {
      if(psz == NULL)
         return "";

      if(psz[1] == ':')
      {
         string strDir = name(psz);
         string str;
         str = strDir.Left(2);
         str += "\\trash_that_is_not_trash\\";
         string strFormat;
         ::datetime::time time;
         time = ::datetime::time::get_current_time();
         strFormat.Format("%04d-%02d-%02d %02d-%02d-%02d\\", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
         str += strFormat;
         if(strDir.m_pszData[2] == '\\')
         {
            str += strDir.Mid(3);
         }
         else
         {
            str += strDir.Mid(2);
         }
         return str;
      }

      return "";
   }

   string dir::appdata(const char * lpcsz, const char * lpcsz2)
   {
      string str;
      SHGetSpecialFolderPath(
         NULL,
         str,
         CSIDL_COMMON_APPDATA,
         FALSE);
      string strRelative;
      strRelative = ca2();
      index iFind = strRelative.find(':');
      if(iFind >= 0)
      {
         strsize iFind1 = strRelative.reverse_find("\\", iFind);
         strsize iFind2 = strRelative.reverse_find("/", iFind);
         strsize iStart = max(iFind1 + 1, iFind2 + 1);
         strRelative = strRelative.Left(iFind - 1) + "_" + strRelative.Mid(iStart, iFind - iStart) + strRelative.Mid(iFind + 1);
      }
      return path(path(str, "ca2", strRelative), lpcsz, lpcsz2);
   }

   string dir::usersystemappdata(::ca::application * papp, const char * lpcszPrefix, const char * lpcsz, const char * lpcsz2)
   {
      UNREFERENCED_PARAMETER(papp);
      return path(appdata(lpcszPrefix), lpcsz, lpcsz2);
   }

   string dir::userappdata(::ca::application * papp, const char * lpcsz, const char * lpcsz2)
   {
      return path(userfolder(papp, "appdata"), lpcsz, lpcsz2);
   }

   string dir::userdata(::ca::application * papp, const char * lpcsz, const char * lpcsz2)
   {
      return path(userfolder(papp, "data"), lpcsz, lpcsz2);
   }

   string dir::userfolder(::ca::application * papp, const char * lpcsz, const char * lpcsz2)
   {

      string str;
      SHGetSpecialFolderPath(
         NULL,
         str,
         CSIDL_PROFILE,
         FALSE);


      string strRelative;
      strRelative = ca2();
      index iFind = strRelative.find(':');
      if(iFind >= 0)
      {
         strsize iFind1 = strRelative.reverse_find("\\", iFind);
         strsize iFind2 = strRelative.reverse_find("/", iFind);
         strsize iStart = max(iFind1 + 1, iFind2 + 1);
         strRelative = strRelative.Left(iFind - 1) + "_" + strRelative.Mid(iStart, iFind - iStart) + strRelative.Mid(iFind + 1);
      }

      string strUserFolderShift;

      if(App(papp).directrix().m_varTopicQuery.has_property("user_folder_relative_path"))
      {
         strUserFolderShift = path(strRelative, App(papp).directrix().m_varTopicQuery["user_folder_relative_path"].get_string());
      }
      else
      {
         strUserFolderShift = strRelative;
      }

      return path(path(str, "ca2", strUserFolderShift), lpcsz, lpcsz2);

//      return path(path(str, "ca2"), lpcsz);
/*      if(&AppUser(papp) == NULL)
      {
         string str;
         SHGetSpecialFolderPath(
            NULL,
            str,
            CSIDL_PROFILE,
            FALSE);
         return path(path(str, "ca2\\_____default"), lpcsz);
      }
      else
      {
         return path(AppUser(papp).m_strPath, lpcsz, lpcsz2);
      }*/
   }

   string dir::default_os_user_path_prefix(::ca::application * papp)
   {
      UNREFERENCED_PARAMETER(papp);
      wchar_t buf[MAX_PATH];
      ULONG ulSize = sizeof(buf) / sizeof(wchar_t);
      if(!::GetUserNameExW(NameCanonical, buf, &ulSize))
      {
         if(!::GetUserNameW(buf, &ulSize))
         {
            memset(buf, 0, sizeof(buf));
         }
      }
      return gen::international::unicode_to_utf8(buf);
   }

   string dir::default_userappdata(::ca::application * papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
   {
      return path(default_userfolder(papp, lpcszPrefix, lpcszLogin, "appdata"), pszRelativePath);
   }

   string dir::default_userdata(::ca::application * papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
   {
      return path(default_userfolder(papp, lpcszPrefix, lpcszLogin, "data"), pszRelativePath);
   }

   string dir::default_userfolder(::ca::application * papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
   {

      return userfolder(papp, pszRelativePath);

/*      UNREFERENCED_PARAMETER(papp);
      string str;
      SHGetSpecialFolderPath(
         NULL,
         str,
         CSIDL_APPDATA,
         FALSE);
      return path(path(str, "ca2\\user", lpcszPrefix), lpcszLogin, pszRelativePath);*/
   }

   string dir::userquicklaunch(::ca::application * papp, const char * lpcszRelativePath, const char * lpcsz2)
   {
      UNREFERENCED_PARAMETER(papp);
      string str;
      SHGetSpecialFolderPath(
         NULL,
         str,
         CSIDL_APPDATA,
         FALSE);
      str = path(str, "Microsoft\\Internet Explorer\\Quick Launch");
      return path(str, lpcszRelativePath, lpcsz2);
   }

   string dir::userprograms(::ca::application * papp, const char * lpcszRelativePath, const char * lpcsz2)
   {
      UNREFERENCED_PARAMETER(papp);
      string str;
      SHGetSpecialFolderPath(
         NULL,
         str,
         CSIDL_PROGRAMS,
         FALSE);
      return path(str, lpcszRelativePath, lpcsz2);
   }

   string dir::commonprograms(const char * lpcszRelativePath, const char * lpcsz2)
   {
      string str;
      SHGetSpecialFolderPath(
         NULL,
         str,
         CSIDL_COMMON_PROGRAMS,
         FALSE);
      return path(str, lpcszRelativePath, lpcsz2);
   }

   bool dir::is_inside_time(const char * pszPath, ::ca::application * papp)
   {
      return is_inside(time(), pszPath, papp);
   }

   bool dir::is_inside(const char * pszDir, const char * pszPath, ::ca::application * papp)
   {
      return gen::str::begins_ci(pszDir, pszPath);
   }

   bool dir::has_subdir(::ca::application * papp, const char * pszDir)
   {
      FileFind filefind;
      bool bWorking;
      bWorking = filefind.FindFile(path(pszDir, "*.*"));
      while(bWorking)
      {
         bWorking = filefind.FindNextFileA();
         if(filefind.IsDirectory() && !filefind.IsDots())
         {
            return true;
         }
      }
      return false;
   }

} // namespace win
