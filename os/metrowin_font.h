#pragma once


#undef new


#include <GdiPlus.h>


namespace metrowin
{


   class CLASS_DECL_metrowin font : 
      virtual public ::ca2::font
   {
   public:

      
      //Gdiplus::Font *   m_pfont;
      IDWriteTextFormat *                  m_pformat;


      font(::ca2::application * papp);
      virtual ~font();


      virtual IDWriteTextFormat * get_os_font(::metrowin::graphics * pdc) const;


/*      virtual void construct(const ::ca2::font & fontParam);

      virtual bool CreateFontIndirect(const LOGFONT* lpLogFont);
      virtual bool CreateFont(int nHeight, int nWidth, int nEscapement,
            int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline,
            BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
            BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
            const char * lpszFacename);
      virtual bool CreatePointFont(int nPointSize, const char * lpszFaceName, ::ca2::graphics * pgraphics = NULL);
      virtual bool CreatePointFontIndirect(const LOGFONT* lpLogFont, ::ca2::graphics * pgraphics = NULL);

      virtual int GetLogFont(LOGFONT* pLogFont);*/

      virtual void dump(dump_context & dumpcontext) const;

      virtual bool destroy();


   };


} // namespace metrowin


#define new DEBUG_NEW



