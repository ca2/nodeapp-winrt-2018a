#pragma once

//#if core_level_3
//#define IMPLEMENT_VISTA_TOOLS
//#endif

#include "metrowin.h"
#include "metrowin_internal.h"

CLASS_DECL_metrowin void vfxThrowFileException(::ca::application * papp, int cause, LONG lOsError, const char * lpszFileName = NULL);
