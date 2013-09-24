#pragma once



#include "metrowin.h"
#include "metrowin_internal.h"


CLASS_DECL_metrowin void vfxThrowFileException(base_application * papp, int cause, LONG lOsError, const char * lpszFileName = NULL);
