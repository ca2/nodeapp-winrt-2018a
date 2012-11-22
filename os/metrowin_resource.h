#pragma once

#if FALSE

class CLASS_DECL_metrowin WinResource :
   virtual public ex1::resource
{
public:


   WinResource(::ca::application * papp);
   virtual ~WinResource();
   
   bool ReadResource(string & str, HINSTANCE hinst, UINT nID, const char * lpcszType);
   bool ReadResource(ex1::file & file, HINSTANCE hinst, UINT nID, const char * lpcszType);


};

#endif