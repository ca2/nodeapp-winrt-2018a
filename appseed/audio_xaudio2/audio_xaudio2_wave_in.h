#pragma once


namespace multimedia
{


   namespace audio_xaudio2
   {


      class  CLASS_DECL_AUDIO_XAUDIO2 wave_in :
         virtual public ::multimedia::audio::wave_in
      {
      public:


         interface IXAudio2 *                   m_paudio;
         interface IXAudio2MasteringVoice *     m_pvoice;
         WAVEFORMATEX                           m_waveformatex;


         wave_in(sp(base_application) papp);
         virtual ~wave_in();


         virtual bool wave_in_initialize_encoder();

         virtual ::multimedia::e_result wave_in_add_buffer(int32_t iBuffer);

         virtual void * get_os_data();
         
         ::multimedia::e_result wave_in_open(int32_t iBufferCount, int32_t iBufferSampleCount);
         ::multimedia::e_result wave_in_close();
         ::multimedia::e_result wave_in_stop();
         ::multimedia::e_result wave_in_start();
         ::multimedia::e_result wave_in_reset();

         virtual void translate_wave_in_message(::signal_details * pobj);

         virtual bool initialize_instance();
         virtual int32_t exit_instance();
         virtual void pre_translate_message(::signal_details * pobj);

         WAVEFORMATEX * wave_format();


      };


   } // namespace audio_xaudio2


} // namespace multimedia




