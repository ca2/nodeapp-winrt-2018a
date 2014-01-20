#pragma once


namespace metrowin
{


   class CLASS_DECL_metrowin copydesk : 
      virtual public ::core::copydesk,
      virtual public ::user::window_sp
   {
   public:

      
      copydesk(base_application * papp);
      virtual ~copydesk();

      
      virtual bool initialize();
      virtual bool finalize();

      virtual void set_filea(stringa & stra);

      virtual int get_file_count();
      virtual void get_filea(stringa & stra);

      virtual void set_plain_text(const char * psz);
      virtual string get_plain_text();


      virtual bool desk_to_dib(::draw2d::dib * pdib);
      // todo
      //bool dib_to_desk(::draw2d::dib * pdib);

   };


} // namespace metrowin



