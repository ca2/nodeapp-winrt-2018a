#pragma once

CLASS_DECL_metrowin bool vfxFullPath(wchar_t * lpszPathOut, const wchar_t * lpszFileIn);
CLASS_DECL_metrowin bool vfxFullPath(wstring & wstrFullPath, const wstring & wstrPath);
CLASS_DECL_metrowin UINT vfxGetFileName(const wchar_t * lpszPathName, wchar_t * lpszTitle, UINT nMax);
CLASS_DECL_metrowin void vfxGetModuleShortFileName(HINSTANCE hInst, string & strShortName);
CLASS_DECL_metrowin void vfxGetModuleShortFileName(HINSTANCE hInst, string& strShortName);

class WinFileSystem :
   virtual public ex1::file_system
{
public:
   WinFileSystem(::ca::application * papp);
   virtual ~WinFileSystem();

   virtual bool FullPath(string & str, const char * lpszFileIn);
   virtual bool FullPath(wstring & wstrFullPath, const wstring & wstrPath);
   virtual UINT GetFileName(const char * lpszPathName, string & str);
   virtual void GetModuleShortFileName(HINSTANCE hInst, string & strShortName);

};
