#pragma once


#define METROWIN
#define WINDOWS
#define WIN32

#if defined(_M_X64)  // X64


#define AMD64
#define OS64BIT
#define OSBIT 64


#elif defined(_M_IX86)


#define X86
#define OS32BIT
#define OSBIT 32


#endif


#ifdef _DEBUG
#define DEBUG
#endif

#ifndef _DEBUG
#define _DEBUG
#endif

#define NOMINMAX

#define DECL_C __cdecl


#define CLASS_DECL_EXPORT __declspec(dllexport)
#define CLASS_DECL_IMPORT __declspec(dllimport)
#define CLASS_DECL_THREAD __declspec(thread)

//#define NO_VARIADIC_TEMPLATE


#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_RAND_S

#define __PLACEMENT_NEW_INLINE



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



#pragma warning(disable:4250) // disable warning if inherited via dominance




#define DRAWDIB_INCLUDE_STRETCHDIB

#ifdef cplus

#define inline static

#endif


#define static_inline static