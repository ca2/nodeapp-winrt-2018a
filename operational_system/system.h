#pragma once


#ifdef __cplusplus

class metrowin_todo { public: metrowin_todo() {}  ~metrowin_todo() {} };

#endif

#include "get_platform.h"


//#define IMPLEMENT_VISTA_TOOLS
#define WIN32
#define DRAWDIB_INCLUDE_STRETCHDIB
#ifdef __cplusplus
#pragma pointers_to_members( full_generality, virtual_inheritance )
#endif


#define _BIND_TO_CURRENT_CRT_VERSION 1
#define _CRT_NOFORCE_MANIFEST
#define _STL_NOFORCE_MANIFEST


BEGIN_EXTERN_C


__declspec(selectany)  int _forceCRTManifest;


END_EXTERN_C


#define NOTHROW throw()
#define THROWS throw(...)


#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif


#if defined(_M_X64)

#define _AMD64_

#else 

#define _X86_

#endif


#include "inttypes.h"
//#include "integer_type.h"




typedef int_ptr ssize_t;
typedef void * HDROP;


