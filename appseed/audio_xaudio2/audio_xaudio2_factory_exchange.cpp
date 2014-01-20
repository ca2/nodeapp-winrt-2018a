#include "framework.h"


namespace multimedia
{


   namespace audio_xaudio2
   {


      factory_exchange::factory_exchange(sp(base_application) papp) :
         element(papp),
         ::multimedia::xaudio2::factory_exchange(papp)
      {

         System.factory().cloneable_large < wave_in               >  (System.type_info < ::multimedia::audio::wave_in                  > ());
         System.factory().cloneable_large < wave_out              >  (System.type_info < ::multimedia::audio::wave_out                 > ());

      }


      factory_exchange::~factory_exchange()
      {
      }


   } // namespace audio_xaudio2


} // namespace multimedia


void ca2_factory_exchange(sp(base_application) papp)
{

   ::multimedia::audio_xaudio2::factory_exchange factoryexchange(papp);

}









