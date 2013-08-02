#pragma once

namespace metrowin
{
   
   class CLASS_DECL_metrowin printer :
      virtual public ::user::printer
   {
   public:


      class CLASS_DECL_metrowin document_properties :
         virtual public ::ca2::object
      {
      public:


         DEVMODE *      m_pdevmode;
         HDC            m_hdc;


         document_properties(::ca2::application * papp);
         virtual ~document_properties();


         virtual bool initialize(::metrowin::printer * pprinter, DEVMODE * pdevmode = NULL);
         virtual bool close();
         virtual ::draw2d::graphics * create_graphics();

      };


      HANDLE                  m_hPrinter;
      document_properties     m_documentproperties;


      printer(::ca2::application * papp);
      virtual ~printer();


      virtual bool open(const char * pszDeviceName);
      virtual ::draw2d::graphics * create_graphics();
      virtual bool is_opened();
      virtual bool close();



   };



} // namespace user


