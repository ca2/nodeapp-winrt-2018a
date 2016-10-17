#pragma once



namespace music
{


   namespace midi
   {


      namespace winrt
      {


         class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_WINRT thread :
            virtual public ::thread
         {
         public:


            sequence *              m_pseq;

            manual_reset_event      m_evRun;

            imedia_position         m_tkPosition;


            thread(::aura::application * papp);
            virtual ~thread();


            virtual int32_t run();

         };


      } // namespace winrt


   } // namespace midi_winrt


} // namespace music





