#pragma once


namespace multimedia
{


   namespace audio_xaudio2
   {


      class CLASS_DECL_AUDIO_XAUDIO2 factory_exchange :
         virtual public ::multimedia::xaudio2::factory_exchange
      {
      public:


         factory_exchange(sp(base_application) papp);
         virtual ~factory_exchange();

      };


   } // namespace audio_xaudio2


} // namespace multimedia






