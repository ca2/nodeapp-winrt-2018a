#pragma once

namespace metrowin
{

   class CLASS_DECL_metrowin factory_exchange :
      virtual public ::radix::object
   {
   public:
      factory_exchange(::ca::application * papp);
      virtual ~factory_exchange();
   };

} // namespace metrowin