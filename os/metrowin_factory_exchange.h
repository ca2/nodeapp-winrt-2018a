#pragma once

namespace metrowin
{

   class CLASS_DECL_metrowin factory_exchange :
      virtual public ::object
   {
   public:
      factory_exchange(base_application * papp);
      virtual ~factory_exchange();
   };

} // namespace metrowin