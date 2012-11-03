/*
Module : IPENUM.H
Purpose: Interface for an ca2 API class wrapper for IP address enumeration
Created: PJN / 21-04-1998
History: None

Copyright (c) 1998 by PJ Naughter.  


*/


namespace win
{


   class CLASS_DECL_win ip_enum :
      virtual public ::ca4::ip_enum
   {
   public:

      
      bool     m_bWinsockInitialized;

      
      ip_enum(::ca::application * papp);
      virtual ~ip_enum();

      
      virtual ::count enumerate(ip_array & ipa);
      virtual ::count enumerate(stringa & stra);

   };


} // namespace win



