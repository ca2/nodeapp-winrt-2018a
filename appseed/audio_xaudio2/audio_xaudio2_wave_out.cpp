#include "framework.h"


namespace multimedia
{


   namespace audio_xaudio2
   {


      wave_out::wave_out(sp(base_application) papp) :
         element(papp),
         ::thread(papp),
         wave_base(papp),
         ::multimedia::audio::wave_out(papp)
      {

         m_estate             = state_initial;
         m_pthreadCallback    = NULL;
         m_iBufferedCount     = 0;
         m_peffect            = NULL;
         m_dwLostSampleCount  = 0;

         m_paudio             = nullptr;
         m_pvoice             = nullptr;
         m_psourcevoice       = nullptr;

         HRESULT hr;

         if (FAILED(hr = XAudio2Create(&m_paudio, 0, XAUDIO2_DEFAULT_PROCESSOR)))
            throw memory_exception(get_app());

      }

      wave_out::~wave_out()
      {



         if (m_paudio != nullptr)
         {
            
            m_paudio->Release();
            
            m_paudio = nullptr;

         }

      }

      void wave_out::install_message_handling(::message::dispatch * pinterface)
      {

         ::multimedia::audio::wave_out::install_message_handling(pinterface);

//         IGUI_WIN_MSG_LINK(MM_WOM_OPEN, pinterface, this, &wave_out::OnMultimediaOpen);
//         IGUI_WIN_MSG_LINK(MM_WOM_DONE, pinterface, this, &wave_out::OnMultimediaDone);
//         IGUI_WIN_MSG_LINK(MM_WOM_CLOSE, pinterface, this, &wave_out::OnMultimediaClose);

      }


      bool wave_out::initialize_instance()
      {

         if(!::multimedia::audio::wave_out::initialize_instance())
            return false;

         return true;

      }

      int32_t wave_out::exit_instance()
      {

         ::multimedia::audio::wave_out::exit_instance();

         return thread::exit_instance();

      }

      ::multimedia::e_result wave_out::wave_out_open(thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount)
      {
         
         single_lock sLock(&m_mutex, TRUE);

         if (m_pvoice != NULL && m_estate != state_initial)
            return ::multimedia::result_success;



         m_pthreadCallback = pthreadCallback;
         ::multimedia::e_result mmr;
         ASSERT(m_pvoice == nullptr);
         ASSERT(m_estate == state_initial);

         m_pwaveformat->wFormatTag = WAVE_FORMAT_PCM;
         m_pwaveformat->nChannels = 2;
         m_pwaveformat->nSamplesPerSec = 44100;
         m_pwaveformat->wBitsPerSample = sizeof(::multimedia::audio::WAVEBUFFERDATA) * 8;
         m_pwaveformat->nBlockAlign = m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
         m_pwaveformat->nAvgBytesPerSec = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         m_pwaveformat->cbSize = 0;
         sp(::multimedia::audio::wave) audiowave = Application.audiowave();

         if (::multimedia::result_success == (mmr = xaudio2::translate(m_paudio->CreateMasteringVoice(
            &m_pvoice,
            m_pwaveformat->nChannels,
            m_pwaveformat->nSamplesPerSec))))
            goto Opened;
         m_pwaveformat->nSamplesPerSec = 22050;
         m_pwaveformat->nAvgBytesPerSec = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         if (::multimedia::result_success == (mmr = xaudio2::translate(m_paudio->CreateMasteringVoice(
            &m_pvoice,
            m_pwaveformat->nChannels,
            m_pwaveformat->nSamplesPerSec))))
            goto Opened;
         m_pwaveformat->nSamplesPerSec = 11025;
         m_pwaveformat->nAvgBytesPerSec = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         if (::multimedia::result_success == (mmr = xaudio2::translate(m_paudio->CreateMasteringVoice(
            &m_pvoice,
            m_pwaveformat->nChannels,
            m_pwaveformat->nSamplesPerSec))))
            goto Opened;

         if(mmr != ::multimedia::result_success)
         {
            return mmr;
         }

Opened:
         uint32_t uiBufferSizeLog2;
         uint32_t uiBufferSize;
         uint32_t uiAnalysisSize;
         uint32_t uiAllocationSize;
         uint32_t uiInterestSize;
         uint32_t uiSkippedSamplesCount;
         uint32_t uiBufferCount = iBufferCount;

         if(m_pwaveformat->nSamplesPerSec == 44100)
         {
            uiBufferSizeLog2 = 16;
            uiBufferSize = m_pwaveformat->nChannels * 2 * iBufferSampleCount; // 512 kbytes
            uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
            if(iBufferCount > 0)
            {
               uiAllocationSize = iBufferCount * uiAnalysisSize;
            }
            else
            {
               uiAllocationSize = 8 * uiAnalysisSize;
            }
            uiInterestSize = 200;
            uiSkippedSamplesCount = 2;
         }
         else if(m_pwaveformat->nSamplesPerSec == 22050)
         {
            uiBufferSizeLog2 = 10;
            uiBufferSize = 4 * 1 << uiBufferSizeLog2;
            uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
            uiAllocationSize = 4 * uiAnalysisSize;
            uiInterestSize = 200;
            uiSkippedSamplesCount = 1;
         }
         else if(m_pwaveformat->nSamplesPerSec == 11025)
         {
            uiBufferSizeLog2 = 10;
            uiBufferSize = 2 * 1 << uiBufferSizeLog2;
            uiAnalysisSize = 2 * 1 << uiBufferSizeLog2;
            uiAllocationSize = 4 * uiAnalysisSize;
            uiInterestSize = 200;
            uiSkippedSamplesCount = 1;
         }
         
         wave_out_get_buffer()->PCMOutOpen(this, uiBufferSize, uiBufferCount, m_pwaveformat, m_pwaveformat);

         m_pprebuffer->open(
            this, // callback thread (thread)
            m_pwaveformat->nChannels, // channel count
            uiBufferCount, // group count
            iBufferSampleCount); // group sample count

         int32_t i, iSize;
         iSize = wave_out_get_buffer()->GetBufferCount();
         buffer  * pbuffer = NULL;

         //
         // Create the source voice
         //
         if (::multimedia::result_success != (mmr = xaudio2::translate(
            m_paudio->CreateSourceVoice(
            &m_psourcevoice,
            &m_waveformatex,
            0,
            1.0f,
            reinterpret_cast < IXAudio2VoiceCallback * > (this),
            nullptr,
            nullptr
            ))))
         {

            TRACE("ERROR OPENING Preparing INPUT DEVICE buffer");

            return mmr;

         }

         for (i = 0; i < iSize; i++)
         {

            pbuffer = new buffer(this, i, wave_out_get_buffer_data(i));


               pbuffer->m_pvoice = m_psourcevoice;

            m_bufferptra.add(pbuffer);

         }

         
         m_estate = state_opened;

         return ::multimedia::result_success;

      }

      ::multimedia::e_result wave_out::wave_out_open_ex(thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount, uint32_t uiSamplesPerSec, uint32_t uiChannelCount, uint32_t uiBitsPerSample)
      {

         single_lock sLock(&m_mutex, TRUE);

         if(m_pvoice != NULL && m_estate != state_initial)
            return ::multimedia::result_success;

         m_pthreadCallback = pthreadCallback;
         ::multimedia::e_result mmr;
         ASSERT(m_pvoice == NULL);
         ASSERT(m_estate == state_initial);

         m_pwaveformat->wFormatTag        = WAVE_FORMAT_PCM;
         m_pwaveformat->nChannels         = (WORD) uiChannelCount;
         m_pwaveformat->nSamplesPerSec    = uiSamplesPerSec;
         m_pwaveformat->wBitsPerSample    = (WORD) uiBitsPerSample;
         m_pwaveformat->nBlockAlign       = m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
         m_pwaveformat->nAvgBytesPerSec   = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         m_pwaveformat->cbSize            = sizeof(m_waveformatex);

         sp(::multimedia::audio::wave) audiowave = Application.audiowave();

         try
         {

            if (::multimedia::result_success == (mmr = xaudio2::translate(m_paudio->CreateMasteringVoice(
               &m_pvoice,
               m_pwaveformat->nChannels,
               m_pwaveformat->nSamplesPerSec))))
               goto Opened;

         }
         catch(const ::exception::exception &)
         {

            return ::multimedia::result_error;

         }
         catch(...)
         {

            return ::multimedia::result_error;

         }

         if(mmr != ::multimedia::result_success)
         {

            TRACE0(::multimedia::strerror(mmr));

            return mmr;

         }

Opened:

         uint32_t uiBufferSizeLog2;
         uint32_t uiBufferSize;
         uint32_t uiAnalysisSize;
         uint32_t uiAllocationSize;
         uint32_t uiInterestSize;
         uint32_t uiSkippedSamplesCount;
         uint32_t uiBufferCount = iBufferCount;

         //   if(m_pwaveformat->nSamplesPerSec == 44100)
         if(true)
         {
            uiBufferSizeLog2 = 16;
            uiBufferSize = m_pwaveformat->nChannels * 2 * iBufferSampleCount; // 512 kbytes
            uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
            if(iBufferCount > 0)
            {
               uiAllocationSize = iBufferCount * uiAnalysisSize;
            }
            else
            {
               uiAllocationSize = 8 * uiAnalysisSize;
            }
            uiInterestSize = 200;
            uiSkippedSamplesCount = 2;
         }
         else if(m_pwaveformat->nSamplesPerSec == 22050)
         {
            uiBufferSizeLog2 = 10;
            uiBufferSize = 4 * 1 << uiBufferSizeLog2;
            uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
            uiAllocationSize = 4 * uiAnalysisSize;
            uiInterestSize = 200;
            uiSkippedSamplesCount = 1;
         }
         else if(m_pwaveformat->nSamplesPerSec == 11025)
         {
            uiBufferSizeLog2 = 10;
            uiBufferSize = 2 * 1 << uiBufferSizeLog2;
            uiAnalysisSize = 2 * 1 << uiBufferSizeLog2;
            uiAllocationSize = 4 * uiAnalysisSize;
            uiInterestSize = 200;
            uiSkippedSamplesCount = 1;
         }

         wave_out_get_buffer()->PCMOutOpen(this, uiBufferSize, uiBufferCount, m_pwaveformat, m_pwaveformat);

         m_pprebuffer->open(this, m_pwaveformat->nChannels, uiBufferCount, iBufferSampleCount); 

         int32_t i, iSize;
         
         iSize = wave_out_get_buffer()->GetBufferCount();

         xaudio2::translate(m_waveformatex, m_pwaveformat);

         if (::multimedia::result_success != (mmr = xaudio2::translate(
            m_paudio->CreateSourceVoice(
            &m_psourcevoice,
            &m_waveformatex,
            0,
            1.0f,
            reinterpret_cast <IXAudio2VoiceCallback *> (this),
            nullptr,
            nullptr
            ))))
            return mmr;


            buffer * pbuffer;

         for(i = 0; i < iSize; i++)
         {

            pbuffer = new buffer(this, i, wave_out_get_buffer_data(i));


            pbuffer->m_pvoice = m_psourcevoice;

            m_bufferptra.add(pbuffer);

         }

         m_pprebuffer->SetMinL1BufferCount(wave_out_get_buffer()->GetBufferCount() + 4);

         m_estate = state_opened;

         return ::multimedia::result_success;

      }



      ::multimedia::e_result wave_out::wave_out_close()
      {

         single_lock sLock(&m_mutex, TRUE);

         if(m_estate == state_playing)
         {
            wave_out_stop();
         }

         if(m_estate != state_opened)
            return ::multimedia::result_success;

         ::multimedia::e_result mmr;

         int32_t i, iSize;

         iSize =  wave_out_get_buffer()->GetBufferCount();


         mmr = xaudio2::translate(m_psourcevoice->Stop());

         for(i = 0; i < iSize; i++)
         {

            delete m_bufferptra[i];

         }


         m_psourcevoice->DestroyVoice();

         m_psourcevoice = nullptr;

         m_bufferptra.remove_all();


         if (m_pvoice != nullptr)
         {

            m_pvoice->DestroyVoice();

            m_pvoice = nullptr;

         }

         m_pprebuffer->Reset();

         m_estate = state_initial;

         return ::multimedia::result_success;

      }


      void wave_out::OnMultimediaOpen(::signal_details * pobj)
      {

         UNREFERENCED_PARAMETER(pobj);

      }


      void wave_out::OnMultimediaDone(int iBuffer)
      {
         
         m_iBufferedCount--;

         wave_out_out_buffer_done(iBuffer);

      }

      void wave_out::OnMultimediaClose(::signal_details * pobj)
      {

         UNREFERENCED_PARAMETER(pobj);

      }


      void wave_out::wave_out_buffer_ready(int iBuffer)
      {
         
         if (wave_out_get_state() != state_playing)
         {
            TRACE("ERROR wave_out::BufferReady while wave_out_get_state() != state_playing");
            return;
         }

         ::multimedia::e_result mmr;
         if (m_peffect != NULL)
         {
            m_peffect->Process16bits((int16_t *) wave_out_get_buffer_data(iBuffer), wave_out_get_buffer_size() / 2);
         }

         single_lock sLock(&m_mutex, TRUE);

         //
         // Setup buffer
         //
         XAUDIO2_BUFFER playBuffer = { 0 };
         buffer * pbuffer = m_bufferptra[iBuffer];
         playBuffer.AudioBytes = wave_out_get_buffer_size();
         playBuffer.pAudioData = (const BYTE *) wave_out_get_buffer_data(iBuffer);
         playBuffer.pContext = pbuffer;
         

         //
         // Submit the sound buffer and (re)start (ignore any 'stop' failures)
         //
         HRESULT hr = m_psourcevoice->SubmitSourceBuffer(&playBuffer);

         mmr = xaudio2::translate(hr);

         VERIFY(::multimedia::result_success == mmr);

         if (mmr == ::multimedia::result_success)
         {

            m_iBufferedCount++;

         }

      }



      ::multimedia::e_result wave_out::wave_out_stop()
      {

         single_lock sLock(&m_mutex, TRUE);

         if(m_estate != state_playing && m_estate != state_paused)
            return ::multimedia::result_error;

         m_eventStopped.ResetEvent();

         m_pprebuffer->Stop();

         m_estate = state_stopping;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_xaudio2 output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         m_mmr = xaudio2::translate(m_psourcevoice->Stop());

         if(m_mmr == ::multimedia::result_success)
         {

            m_estate = state_opened;

         }

         return m_mmr;

      }


      ::multimedia::e_result wave_out::wave_out_pause()
      {

         single_lock sLock(&m_mutex, TRUE);

         ASSERT(m_estate == state_playing);

         if(m_estate != state_playing)
            return ::multimedia::result_error;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_xaudio2 output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.

         m_mmr = xaudio2::translate(m_psourcevoice->Stop());

         ASSERT(m_mmr == ::multimedia::result_success);

         if(m_mmr == ::multimedia::result_success)
         {
            m_estate = state_paused;
         }

         return m_mmr;

      }

      ::multimedia::e_result wave_out::wave_out_restart()
      {

         single_lock sLock(&m_mutex, TRUE);

         ASSERT(m_estate == state_paused);

         if(m_estate != state_paused)
            return ::multimedia::result_error;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_xaudio2 output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         m_mmr = xaudio2::translate(m_psourcevoice->Start(0, XAUDIO2_COMMIT_NOW));

         ASSERT(m_mmr == ::multimedia::result_success);

         if(m_mmr == ::multimedia::result_success)
         {

            m_estate = state_playing;

         }

         return m_mmr;

      }


      /*imedia::time wave_out::GetPositionMillisForSynch()
      {
         int64_t dwMillis = GetPositionMillis();
         int64_t dwPosition = m_pprebuffer->m_position * 8;
         dwPosition /= m_pwaveformat->wBitsPerSample;
         dwPosition *= 1000;
         dwPosition /= m_pwaveformat->nChannels * m_pwaveformat->nSamplesPerSec;
         if(m_pprebuffer != NULL && m_pprebuffer->m_pdecoder != NULL)
            return dwMillis + dwPosition - m_pprebuffer->m_pdecoder->DecoderGetLostMillis(dwMillis + dwPosition) - (((int64_t) m_dwLostSampleCount) /  ((int64_t) m_pwaveformat->nSamplesPerSec));
         else
            return dwMillis + dwPosition - ((m_dwLostSampleCount) * 1000 / m_pwaveformat->nSamplesPerSec);
      }*/

      imedia::time wave_out::wave_out_get_position_millis()
      {

         single_lock sLock(&m_mutex, TRUE);

         ::multimedia::e_result                mmr;

         XAUDIO2_VOICE_STATE s;

         if(m_psourcevoice != NULL)
         {

            m_psourcevoice->GetState(&s);

            int64_t i = s.SamplesPlayed;
            i *= 1000;
            i /= m_pwaveformat->nChannels * m_pwaveformat->nSamplesPerSec;
            return i;

         }
         else
            return 0;


      }

      /*imedia::position wave_out::get_position_for_synch()
      {
         imedia::position position = get_position();
         if(m_pprebuffer != NULL && m_pprebuffer->m_pdecoder != NULL)
            return m_pprebuffer->m_position + position - m_pprebuffer->m_pdecoder->DecoderGetLostPositionOffset(position) - m_dwLostSampleCount * m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
         else
            return m_pprebuffer->m_position + position - m_dwLostSampleCount * m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
      }*/

      imedia::position wave_out::wave_out_get_position()
      {
         
         single_lock sLock(&m_mutex, TRUE);

         ::multimedia::e_result                mmr;
         
         XAUDIO2_VOICE_STATE s;

         if (m_psourcevoice != NULL)
         {

            m_psourcevoice->GetState(&s);

               return s.SamplesPlayed;
         }
         else
            return 0;

      }

      void wave_out::wave_out_free(int iBuffer)
      {

         //wave_out_free(wave_hdr(iBuffer));

         ::multimedia::audio::wave_out::wave_out_free(iBuffer);

      }

      void wave_out::wave_out_on_playback_end()
      {

         wave_out_stop();

         if(m_pprebuffer->m_pstreameffectOut != NULL)
         {
            ::multimedia::iaudio::wave_stream_effect * peffect = m_pprebuffer->m_pstreameffectOut;
            m_pprebuffer->m_pstreameffectOut = NULL;
            delete peffect;
         }

         m_eventStopped.SetEvent();

         m_pplayer->OnEvent(::multimedia::audio::wave_player::EventPlaybackEnd);

      }



      WAVEFORMATEX * wave_out::wave_format()
      {

         xaudio2::translate(m_waveformatex, m_pwaveformat);

         return &m_waveformatex;

      }


      void * wave_out::get_os_data()
      {
         return m_psourcevoice;
      }

      //
      // Callback handlers, only implement the buffer events for maintaining play state
      //
      void wave_out::OnVoiceProcessingPassStart(UINT32 /*bytesRequired*/)
      {
      }
      void wave_out::OnVoiceProcessingPassEnd()
      {
      }
      void wave_out::OnStreamEnd()
      {
      }
      void wave_out::OnBufferStart(void* bufferContext)
      {
         buffer * pbuffer = (buffer *)bufferContext;
         pbuffer->m_bIsPlaying =  true;
      }
      void wave_out::OnBufferEnd(void* bufferContext)
      {
         buffer * pbuffer = (buffer *)bufferContext;
         pbuffer->m_bIsPlaying = false;
         OnMultimediaDone(pbuffer->m_iBuffer);
      }
      void wave_out::OnLoopEnd(void* /*bufferContext*/)
      {
      }
      void wave_out::OnVoiceError(void* /*bufferContext*/, HRESULT /*error*/)
      {
      }

      ::multimedia::e_result wave_out::wave_out_start(const imedia::position & position)
      {

         ::multimedia::e_result mmr;

         if (failed(mmr = ::multimedia::audio::wave_out::wave_out_start(position)))
            return mmr;


         mmr = xaudio2::translate(m_psourcevoice->Start(0, XAUDIO2_COMMIT_NOW));

         return mmr;

      }


   } // namespace audio_xaudio2


} // namespace multimedia






