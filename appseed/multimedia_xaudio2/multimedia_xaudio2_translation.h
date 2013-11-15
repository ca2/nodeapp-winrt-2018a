#pragma once


namespace multimedia
{


   namespace xaudio2
   {


      CLASS_DECL_MULTIMEDIA_XAUDIO2 void translate(WAVEFORMATEX & formatex, ::multimedia::audio::wave_format * pwaveformat);
//      CLASS_DECL_MULTIMEDIA_XAUDIO2 void translate(WAVEHDR & wavehdr, ::multimedia::audio::wave_buffer * pwavebuffer, int iIndex);
/*      CLASS_DECL_MULTIMEDIA_XAUDIO2 LPWAVEHDR create_new_WAVEHDR(::multimedia::audio::wave_buffer * pwavebuffer, int iIndex);
      CLASS_DECL_MULTIMEDIA_XAUDIO2 LPWAVEHDR get_os_data(::multimedia::audio::wave_buffer * pwavebuffer, int iIndex);

      */
      CLASS_DECL_MULTIMEDIA_XAUDIO2 ::multimedia::e_result translate(HRESULT hr);
      

   } // namespace xaudio2


} // namespace multimedia





