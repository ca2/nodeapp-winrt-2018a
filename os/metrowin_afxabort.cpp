#include "framework.h"


// Note: in separate module so it can be replaced if needed

void CLASS_DECL_metrowin __abort()
{
//   TRACE(::core::trace::category_AppMsg, 0, "__abort called.\n");

   __win_term();
   abort();
}

/////////////////////////////////////////////////////////////////////////////
