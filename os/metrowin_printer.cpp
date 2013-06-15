#include "framework.h"
#include <WinSpool.h>


namespace metrowin
{


   printer::printer(::ca2::application * papp) :
      ca2(papp),
      ::user::printer(papp),
      m_documentproperties(papp)
   {
   }

   printer::~printer()
   {
   }

   bool printer::open(const char * pszDeviceName)
   {
      throw todo(get_app());
      //if(is_opened())
      //   close();
      //return OpenPrinter((LPSTR) (LPCSTR) pszDeviceName, &m_hPrinter, NULL) != FALSE && m_hPrinter != NULL;
   }

   bool printer::is_opened()
   {
      return m_hPrinter != NULL;
   }

   ::ca2::graphics * printer::create_graphics()
   {
      if(!m_documentproperties.initialize(this))
         return NULL;
      return m_documentproperties.create_graphics();
   }


   bool printer::close()
   {
      throw todo(get_app());
      //bool bOk = true;
      //if(m_hPrinter != NULL)
      //{
      //   bOk = ::ClosePrinter(m_hPrinter) != FALSE;
      //   m_hPrinter = NULL;
      //}
      //return bOk;
   }


   printer::document_properties::document_properties(::ca2::application * papp) :
      ca2(papp)
   {
      m_hdc = NULL;
      m_pdevmode = NULL;
   }

   printer::document_properties::~document_properties()
   {
      close();
   }

   bool printer::document_properties::initialize(::metrowin::printer * pprinter, DEVMODE * pdevmode)
   {
      throw todo(get_app());
      //UNREFERENCED_PARAMETER(pdevmode);
      //if(m_pdevmode != NULL)
      //   return false;
      //if(m_hdc != NULL)
      //   return false;
      //int iSize = DocumentProperties(NULL, pprinter->m_hPrinter, (LPSTR)(LPCSTR) pprinter->m_strName, NULL, NULL, 0);
      //m_pdevmode = (DEVMODE *) malloc(iSize);
      //if(!DocumentProperties(NULL, pprinter->m_hPrinter, (LPSTR) (LPCSTR) pprinter->m_strName, m_pdevmode, NULL, DM_OUT_BUFFER))
      //{
      //   throw "failed to get printer DocumentProperties";
      //   return false;
      //}
      //return true;
   }

   bool printer::document_properties::close()
   {
      throw todo(get_app());
      //if(m_hdc != NULL)
      //{
      //   ::DeleteDC(m_hdc);
      //   m_hdc = NULL;
      //}
      //if(m_pdevmode != NULL)
      //{
      //   free(m_pdevmode);
      //   m_pdevmode = NULL;
      //}
      //return true;
   }



   ::ca2::graphics * printer::document_properties::create_graphics()
   {
      throw todo(get_app());
      //if(m_pdevmode == NULL)
      //   return NULL;
      //if(m_hdc != NULL)
      //   return NULL;
      //m_hdc = ::CreateDC("WINSPOOL", (LPCSTR) m_pdevmode->dmDeviceName, NULL, m_pdevmode);
      //::ca2::graphics_sp g(get_app());
      //g->Attach(m_hdc);
      //return g.detach();
   }


} // namespace win2

