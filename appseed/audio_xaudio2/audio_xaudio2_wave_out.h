#pragma once


namespace multimedia
{


   namespace audio_xaudio2
   {


      class  CLASS_DECL_AUDIO_XAUDIO2 wave_out :
         virtual public ::multimedia::audio::wave_out,
         public IXAudio2VoiceCallback
      {
      public:



         struct buffer
         {

            buffer(wave_out * pwaveout, int iBuffer, void * pdata) :
               m_pwaveout(pwaveout),
               m_iBuffer(iBuffer),
               m_pvoice(nullptr),
               m_pdata(pdata),
               m_bIsPlaying(false)
            {

            }

            ~buffer()
            {
               if (m_pvoice != nullptr)
               {
                  m_pvoice->DestroyVoice();
                  m_pvoice = nullptr;
               }
            }

            int                     m_iBuffer;
            wave_out *              m_pwaveout;
            IXAudio2SourceVoice *   m_pvoice;
            void *                  m_pdata;
            bool                    m_bIsPlaying;


         };



         interface IXAudio2 *                               m_paudio;
         interface IXAudio2MasteringVoice *                 m_pvoice;
         IXAudio2SourceVoice *                              m_psourcevoice;
         raw_array < buffer * >                             m_bufferptra;
         WAVEFORMATEX                                       m_waveformatex;


         wave_out(sp(base_application) papp);
         virtual ~wave_out();


         void install_message_handling(::message::dispatch * pinterface);

         virtual imedia::time wave_out_get_position_millis();
         imedia::position wave_out_get_position();
         virtual void wave_out_buffer_ready(int iBuffer);

         virtual ::multimedia::e_result wave_out_open(::thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount);
         virtual ::multimedia::e_result wave_out_open_ex(::thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount, uint32_t uiSamplesPerSec, uint32_t uiChannelCount, uint32_t uiBitsPerSample);
         virtual ::multimedia::e_result wave_out_stop();
         virtual ::multimedia::e_result wave_out_close();
         virtual ::multimedia::e_result wave_out_pause();
         virtual ::multimedia::e_result wave_out_restart();
         virtual void * get_os_data();

         virtual void wave_out_on_playback_end();
         virtual void wave_out_free(int iBuffer);

         virtual bool initialize_instance();
         virtual int32_t exit_instance();

         DECL_GEN_SIGNAL(OnMultimediaOpen)
            void OnMultimediaDone(int iBuffer);
         DECL_GEN_SIGNAL(OnMultimediaClose)

//         virtual void wave_out_out_buffer_done(int iBuffer);
  //       virtual void wave_out_out_buffer_done(LPWAVEHDR lpwavehdr);

         WAVEFORMATEX * wave_format();


            //
            // Voice callbacks from IXAudio2VoiceCallback
            //
            STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 bytesRequired);
            STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS);
            STDMETHOD_(void, OnStreamEnd) (THIS);
            STDMETHOD_(void, OnBufferStart) (THIS_ void* bufferContext);
            STDMETHOD_(void, OnBufferEnd) (THIS_ void* bufferContext);
            STDMETHOD_(void, OnLoopEnd) (THIS_ void* bufferContext);
            STDMETHOD_(void, OnVoiceError) (THIS_ void* bufferContext, HRESULT error);

            ::multimedia::e_result wave_out_start(const imedia::position & position);
      };


   } // namespace audio_xaudio2


} // namespace multimedia



