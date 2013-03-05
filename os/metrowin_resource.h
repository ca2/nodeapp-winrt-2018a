#pragma once

#if FALSE

class CLASS_DECL_metrowin WinResource :
   virtual public ::ca::resource
{
public:


   WinResource(::ca::application * papp);
   virtual ~WinResource();
   
   bool ReadResource(string & str, HINSTANCE hinst, UINT nID, const char * lpcszType);
   bool ReadResource(::ca::file & file, HINSTANCE hinst, UINT nID, const char * lpcszType);


};

#endif