#include "framework.h"


/* Windows sleep in 100ns units */
BOOLEAN nanosleep(LONGLONG ns) {
   long value = ns / 1000; /* time in microseconds */

   struct timeval tv;
   tv.tv_sec = value / 1000000;
   tv.tv_usec = value % 1000000;
   select(0, NULL, NULL, NULL, &tv);
   return TRUE;

}

uint64_t get_micro()
{

   return get_nanos() / 1000;

}

namespace music
{


   namespace midi
   {


      namespace winrt
      {


         thread::thread(::aura::application * papp) :
            ::object(papp),
            ::thread(papp),
            m_evRun(papp)
         {
               
            m_pseq = NULL;

            m_evRun.SetEvent();

            m_tkPosition = 0;

         }

         thread::~thread()
         {
            if (m_pseq->m_pthreadPlay == this)
            {

               m_pseq->m_pthreadPlay = NULL;

            }
         }


                  

         int32_t thread::run()
         {

            e_result       smfrc;
            ::music::midi::event *           pevent = NULL;
            imedia_position tkMax = ::numeric_info <imedia_position>::get_maximum_value();
            imedia_position tkPosition;
            bool bGotEvent = false;
            sequence_thread * pthread = dynamic_cast < sequence_thread * > (m_pseq->m_pthread);
            uint64_t dwStart = ::get_micro() - m_pseq->TicksToMillisecs(m_pseq->m_tkBase) * 1000;

            m_tkPosition = 0;

            int_map < int_map < int > > map;

            while (m_bRun)
            {

               m_evRun.wait();

               if (!m_bRun)
               {

                  break;

               }


               if (m_pseq->file()->GetFlags().is_signalized(file::EndOfFile))
               {
                  
                  break;

               }

               uint64_t dwNow = ::get_micro() - dwStart;
               uint64_t dwFile = m_pseq->TickToMicro(m_tkPosition);

               while (dwNow >= dwFile)
               {
                  
                  if (!bGotEvent)
                  {

                     smfrc = m_pseq->file()->WorkGetNextEvent(pevent, tkMax, TRUE);

                     if (::music::success != smfrc)
                     {

                        break;

                     }

                     bGotEvent = true;

                  }

                  tkPosition = m_pseq->file()->GetPosition();

                  dwNow = ::get_micro() - dwStart;
                  dwFile = m_pseq->TickToMicro(tkPosition);

                  if (dwNow < dwFile)
                  {
                     
                     break;

                  }
                  if (dwNow - dwFile >= 20 * 1000)
                  {

                     for (auto & p1 : map)
                     {
                        for (auto & p2 : p1.m_element2)
                        {
                           if (p2.m_element2 >= 0)
                           {
                              m_pseq->m_io->note_off(p1.m_element1, p2.m_element1, p2.m_element2);
                              p2.m_element2 = -1;
                           }
                        }
                     }
                        

                  }

                  m_tkPosition = tkPosition;

                  if (pevent->GetType() == ::music::midi::NoteOn)
                  {
                     
                     if (dwNow - dwFile < 20 * 1000)
                     {
                        
                        m_pseq->m_io->note_on(pevent->GetTrack(), pevent->GetNotePitch(), pevent->GetNoteVelocity());

                        map[pevent->GetTrack()][pevent->GetNotePitch()] = pevent->GetNoteVelocity();

                     }

                  }
                  else if (pevent->GetType() == ::music::midi::NoteOff)
                  {

                     if (dwNow - dwFile < 20 * 1000)
                     {

                        m_pseq->m_io->note_off(pevent->GetTrack(), pevent->GetNotePitch(), pevent->GetNoteVelocity());

                        map[pevent->GetTrack()][pevent->GetNotePitch()] = -1;

                     }

                  }
                  else if (pevent->GetType() == ::music::midi::ProgramChange)
                  {

                     m_pseq->m_io->program_change(pevent->GetTrack(), pevent->GetProgram());

                  }



                  bGotEvent = false;

               }

               

               if (::music::success != smfrc)
               {

                  break;

               }

               Sleep(1);

            }

         end_playback:

            pthread->PostMidiSequenceEvent(
               m_pseq,
               ::music::midi::sequence::EventMidiPlaybackEnd,
               NULL);
            if (m_pseq->m_pthreadPlay == this)
            {

               m_pseq->m_pthreadPlay = NULL;

            }

            return 0;

         }


      } // namespace winrt


   } // namespace midi_winrt


} // namespace music





