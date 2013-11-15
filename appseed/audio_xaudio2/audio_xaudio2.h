#pragma once


#include "nodeapp/appseed/multimedia_xaudio2/multimedia_xaudio2.h"


#if defined(_AUDIO_XAUDIO2_LIBRARY)
   #define CLASS_DECL_AUDIO_XAUDIO2  CLASS_DECL_EXPORT
#else
   #define  CLASS_DECL_AUDIO_XAUDIO2  CLASS_DECL_IMPORT
#endif


#include "audio_xaudio2_wave_out.h"
#include "audio_xaudio2_wave_in.h"


#include "audio_xaudio2_factory_exchange.h"