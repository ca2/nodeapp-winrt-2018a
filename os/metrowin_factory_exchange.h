#pragma once

namespace metrowin
{

   class CLASS_DECL_metrowin factory_exchange :
      virtual public ::ca2::object
   {
   public:
      factory_exchange(::ca2::application * papp);
      virtual ~factory_exchange();
   };

} // namespace metrowin