#include "framework.h"


namespace metrowin
{


   factory_exchange::factory_exchange(::ca2::application * papp) :
      ca2(papp)
   {
      System.factory().cloneable_large < stdio_file          >  (System.template type_info < ::ca2::text_file         > ());
      System.factory().cloneable_large < file               >  (System.template type_info < ::ca2::file              > ());
      System.factory().cloneable_large < file_set            >  (System.template type_info < ::ca2::file_set          > ());
      System.factory().cloneable < file_system               >  (System.template type_info < ::ca2::file_system       > (), 1);
//      System.factory().cloneable < WinResource                 >  (System.template type_info < ::ca2::resource          > (), 1);
      System.factory().cloneable < application                 >  (System.template type_info < ::ca2::application_base       > (), 1);
      System.factory().cloneable < dir                         >  (System.template type_info < ::ca2::dir::system        > (), 1);
      System.factory().cloneable_small < folder_watch          >  (System.template type_info < ::ca2::folder_watch      > ());
      System.factory().cloneable_large < dib                   >  (System.template type_info < ::ca2::dib                > ());
      System.factory().cloneable_large < graphics              >  (System.template type_info < ::ca2::graphics           > ());
      System.factory().cloneable_large < graphics_path         >  (System.template type_info < ::ca2::graphics_path      > ());
      System.factory().cloneable_large < bitmap                >  (System.template type_info < ::ca2::bitmap             > ());
      System.factory().cloneable_large < pen                   >  (System.template type_info < ::ca2::pen                > ());
      System.factory().cloneable_large < brush                 >  (System.template type_info < ::ca2::brush              > ());
      System.factory().cloneable_large < palette               >  (System.template type_info < ::ca2::palette            > ());
      System.factory().cloneable_large < region                >  (System.template type_info < ::ca2::region             > ());
      System.factory().cloneable_large < font                  >  (System.template type_info < ::ca2::font               > ());
      System.factory().creatable < window_draw                 >  (System.template type_info < ::ca2::window_draw        > (), 1);
      System.factory().creatable_large < thread                >  (System.template type_info < ::ca2::thread             > ());
      System.factory().creatable_large < window                >  (System.template type_info < ::ca2::window             > ());
//      System.factory().cloneable_small < draw_dib              >  (System.template type_info < ::ca2::draw_dib           > ());
      System.factory().creatable < os                          >  (System.template type_info < ::ca2::os                 > (), 1);
      System.factory().creatable < port_forward                >  (System.template type_info < ::ca2::port_forward      > (), 1);
      System.factory().creatable_small < copydesk              >  (System.template type_info < ::ca2::copydesk          > (), 1);
      System.factory().creatable_small < crypt                 >  (System.template type_info < ::ca2::crypt             > (), 1);
      System.factory().creatable_small < ip_enum               >  (System.template type_info < ::ca2::ip_enum           > ());
      System.factory().creatable < printer                     >  (System.template type_info < ::user::printer          > (), 1);
      

      ::metrowin::dib::s_initialize();
   }


   factory_exchange::~factory_exchange()
   {
   }


} // namespace metrowin



void ca2_factory_exchange(::ca2::application * papp)
{
   metrowin::factory_exchange factoryexchange(papp);
}



