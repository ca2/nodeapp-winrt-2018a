#pragma once


#include "app-core/appseed/multimedia/multimedia/multimedia.h"
#include <xaudio2.h>


#if defined(_MULTIMEDIA_XAUDIO2_LIBRARY)
   #define CLASS_DECL_MULTIMEDIA_XAUDIO2  CLASS_DECL_EXPORT
#else
   #define  CLASS_DECL_MULTIMEDIA_XAUDIO2  CLASS_DECL_IMPORT
#endif


#include "multimedia_xaudio2_translation.h"


#include "multimedia_xaudio2_factory_exchange.h"