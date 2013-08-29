#pragma once




#include "integer_type.h"
//
//#ifde
//#define IMPLEMENT_VISTA_TOOLS
#define DRAWDIB_INCLUDE_STRETCHDIB


#ifdef __cplusplus
#pragma pointers_to_members( full_generality, virtual_inheritance )
#endif



#define NOTHROW throw()
#define THROWS throw(...)

#ifdef DEBUG
#define RELEASENOTHROW throw(...)
#else
#define RELEASENOTHROW throw()
#endif













#include "bare_operational_system.h"

#define MOVE_SEMANTICS