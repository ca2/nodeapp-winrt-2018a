#pragma once

#if FALSE

class CLASS_DECL_metrowin WinResource :
   virtual public ::ca2::resource
{
public:


   WinResource(::ca2::application * papp);
   virtual ~WinResource();
   
   bool ReadResource(string & str, HINSTANCE hinst, UINT nID, const char * lpcszType);
   bool ReadResource(::ca2::file & file, HINSTANCE hinst, UINT nID, const char * lpcszType);


};

#endif