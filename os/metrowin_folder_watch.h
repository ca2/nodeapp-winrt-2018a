#pragma once

namespace metrowin
{

   class CLASS_DECL_metrowin folder_watch :
      virtual public ::ca2::folder_watch,
      virtual public ::ca2::thread
   {
   public:
      
      folder_watch(::ca2::application * papp);

      virtual bool watch(const char * pszPath);

      virtual e_action translate_os_action(int iAction);

      virtual int run();

   };


} // namespace metrowin