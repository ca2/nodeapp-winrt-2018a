#pragma once


#ifdef __cplusplus

class metrowin_todo { public: metrowin_todo() {}  ~metrowin_todo() {} };

#endif

#include "get_platform.h"


#define IMPLEMENT_VISTA_TOOLS
#define WIN32
#define DRAWDIB_INCLUDE_STRETCHDIB
#ifdef __cplusplus
#pragma pointers_to_members( full_generality, virtual_inheritance )
#endif

#include "win32_system.h"
#define _ApplicationFrameworkDLL


#if defined(_M_X64)

#define _AMD64_

#else 

#define _X86_

#endif


#include "inttypes.h"
//#include "integer_type.h"




typedef int_ptr ssize_t;
typedef void * HDROP;


