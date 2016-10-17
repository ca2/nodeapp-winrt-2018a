#include "framework.h"



namespace music
{


   namespace midi
   {


      namespace winrt
      {


         thread::thread(::aura::application * papp) :
            ::object(papp),
            ::thread(papp),
            m_evPause(papp)
         {
               
            m_pseq = NULL;

         }

         thread::~thread()
         {

         }


         

         int32_t thread::run()
         {

            int iChannel = 0;
            unsigned char nDo = 64;
            unsigned char uchVelocity = 100;
            unsigned char nRe = 64 + 2;
            unsigned char nMi = 64 + 4;
            unsigned char nFa = 64 + 5;
            unsigned char nSol = 64 + 7;
            unsigned char nLa = 64 + 9;
            unsigned char nSi = 64 + 11;
            unsigned char nDo2 = 64 + 12;

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            Sleep(200);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            Sleep(200);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            Sleep(200);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            Sleep(1600);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            Sleep(200);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            Sleep(200);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            Sleep(200);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            Sleep(1600);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            Sleep(200);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            Sleep(200);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            Sleep(200);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            m_pseq->m_io->note_on(iChannel, nMi, uchVelocity);

            Sleep(600);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            m_pseq->m_io->note_on(iChannel, nMi, uchVelocity);

            Sleep(200);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            m_pseq->m_io->note_on(iChannel, nMi, uchVelocity);

            Sleep(200);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            m_pseq->m_io->note_on(iChannel, nMi, uchVelocity);

            Sleep(200);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            m_pseq->m_io->note_on(iChannel, nMi, uchVelocity);

            Sleep(600);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            m_pseq->m_io->note_on(iChannel, nMi, uchVelocity);

            Sleep(200);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            m_pseq->m_io->note_on(iChannel, nMi, uchVelocity);

            Sleep(200);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            m_pseq->m_io->note_on(iChannel, nMi, uchVelocity);

            Sleep(200);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            m_pseq->m_io->note_on(iChannel, nMi, uchVelocity);

            m_pseq->m_io->note_on(iChannel, nSol, uchVelocity);

            Sleep(600);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            m_pseq->m_io->note_on(iChannel, nMi, uchVelocity);

            m_pseq->m_io->note_on(iChannel, nSol, uchVelocity);

            Sleep(200);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            m_pseq->m_io->note_on(iChannel, nMi, uchVelocity);

            m_pseq->m_io->note_on(iChannel, nSol, uchVelocity);

            Sleep(200);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            m_pseq->m_io->note_on(iChannel, nMi, uchVelocity);

            m_pseq->m_io->note_on(iChannel, nSol, uchVelocity);

            Sleep(200);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            m_pseq->m_io->note_on(iChannel, nMi, uchVelocity);

            m_pseq->m_io->note_on(iChannel, nSol, uchVelocity);

            Sleep(500);

            m_pseq->m_io->note_on(iChannel, nDo, uchVelocity);

            Sleep(100);

            m_pseq->m_io->note_on(iChannel, nRe, uchVelocity);

            Sleep(100);

            m_pseq->m_io->note_on(iChannel, nMi, uchVelocity);

            Sleep(100);

            m_pseq->m_io->note_on(iChannel, nFa, uchVelocity);

            Sleep(100);

            m_pseq->m_io->note_on(iChannel, nSol, uchVelocity);

            Sleep(100);

            m_pseq->m_io->note_on(iChannel, nLa, uchVelocity);

            Sleep(100);

            m_pseq->m_io->note_on(iChannel, nSi, uchVelocity);

            Sleep(100);
            m_pseq->m_io->note_on(iChannel, nDo2, uchVelocity);
            m_pseq->m_io->note_on(iChannel, nDo -12, uchVelocity);
            m_pseq->m_io->note_on(iChannel, nDo - 24, uchVelocity);

            Sleep(100);

            while (m_bRun)
            {

               m_evPause.wait();

               if (!m_bRun)
               {

                  break;

               }
               
               Sleep(1);

            }

            return 0;

         }


      } // namespace winrt


   } // namespace midi_winrt


} // namespace music





