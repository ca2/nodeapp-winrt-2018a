#pragma once

namespace win
{

   class CLASS_DECL_win factory_exchange :
      virtual public ::radix::object
   {
   public:
      factory_exchange(::ca::application * papp);
      virtual ~factory_exchange();
   };

} // namespace win