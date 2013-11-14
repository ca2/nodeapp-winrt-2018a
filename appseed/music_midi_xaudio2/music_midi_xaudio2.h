#pragma once


#include "app-veriwell/appseed/multimedia/multimedia/multimedia.h"
#include "nodeapp/appseed/multimedia_mmsystem/multimedia_mmsystem.h"


#if defined(_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM_LIBRARY)
   #define CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM  CLASS_DECL_IMPORT
#endif



#include "music_midi_mmsystem_translation.h"

#include "music_midi_mmsystem_sequence.h"

#include "player/music_midi_mmsystem_player.h"

#include "music_midi_mmsystem_out.h"

#include "music_midi_mmsystem_file.h"
          
#include "music_midi_mmsystem_sequence_thread.h"

#include "music_midi_mmsystem_factory_exchange.h"



