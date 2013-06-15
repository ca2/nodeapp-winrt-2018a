#pragma once


#ifdef _DEBUG
#define DEBUG
#endif

#define METROWIN  // Merde Windows : Metro Style Application
#define WINDOWS
#define WIN32
#define DECL_C __cdecl


#define CLASS_DECL_EXPORT _declspec(dllexport)
#define CLASS_DECL_IMPORT _declspec(dllimport)
#define CLASS_DECL_THREAD _declspec(thread)

#define NO_VARIADIC_TEMPLATE

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_RAND_S


#define _WIDEN(x) L#x



#if defined(_M_X64)  // X64

#define AMD64
#define OS64BIT
#define OSBIT 64

#elif defined(_M_IX86)

#define X86
#define OS32BIT
#define OSBIT 32

#endif




#define _BIND_TO_CURRENT_CRT_VERSION 1
#define _CRT_NOFORCE_MANIFEST
#define _STL_NOFORCE_MANIFEST


BEGIN_EXTERN_C


__declspec(selectany)  int _forceCRTManifest;


END_EXTERN_C

