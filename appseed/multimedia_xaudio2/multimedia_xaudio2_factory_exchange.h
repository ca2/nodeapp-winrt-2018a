#pragma once


namespace multimedia
{


   namespace xaudio2
   {


      class CLASS_DECL_MULTIMEDIA_XAUDIO2 factory_exchange :
         virtual public ::object
      {
      public:


         factory_exchange(sp(base_application) papp);
         virtual ~factory_exchange();

      };


   } // namespace xaudio2


} // namespace multimedia






