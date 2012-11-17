#include "framework.h"

namespace win
{

   //////////////////////////////////////////////////////////////////////
   // draw_dib.cpp: implementation of the draw_dib class.
   //
   //   Creator : El Barto (ef00@luc.ac.be)
   //   Location : http://www.luc.ac.be/~ef00/ebgfx
   //   Date : 09-04-98
   //
   //////////////////////////////////////////////////////////////////////

   draw_dib::draw_dib(::ca::application * papp) :
      ca(papp)
   {
      m_hdrawdib = NULL;
   }

   draw_dib::~draw_dib()
   {
      close();
   }

   int_ptr draw_dib::get_os_data()
   {
      return (int_ptr) m_hdrawdib;
   }

   bool draw_dib::open()
   {
      return (m_hdrawdib = DrawDibOpen()) != NULL; 
   }

   bool draw_dib::close()
   {
      if(m_hdrawdib == NULL)
         return TRUE;
      bool b = DrawDibClose(m_hdrawdib) != FALSE;
      m_hdrawdib = NULL;
      return b;
   }
      
   bool draw_dib::Begin (::ca::graphics * pdc, int dxDest, int dyDest,
      LPBITMAPINFOHEADER lpbi, int dxSrc, int dySrc, UINT wFlags )
   {
      return DrawDibBegin ( m_hdrawdib, (HDC)pdc->get_os_data(), dxDest, dyDest, lpbi, 
         dxSrc, dySrc, wFlags ) != FALSE;
   }

   bool draw_dib::End ()
   {
      return DrawDibEnd ( m_hdrawdib ) != FALSE;
   }

   bool draw_dib::draw (::ca::graphics * pdc, int xDst, int yDst,
      int dxDst, int dyDst, LPBITMAPINFOHEADER lpbi, LPVOID lpBits,
      int xSrc, int ySrc, int dxSrc, int dySrc, UINT wFlags )
   {
      return ::DrawDibDraw ( m_hdrawdib, (HDC)pdc->get_os_data(), xDst, yDst, dxDst, dyDst, 
         lpbi, lpBits, xSrc, ySrc, dxSrc, dySrc, wFlags ) != FALSE;
   }

   bool draw_dib::draw (::ca::dib *dib, ::ca::graphics * pdc, int xDst, int yDst,
      int dxDst, int dyDst, UINT wFlags)
   {
      return ::DrawDibDraw( m_hdrawdib, (HDC)pdc->get_os_data(), xDst, yDst, dxDst, dyDst, 
         &((dynamic_cast < ::win::dib * > (dib))->m_info.bmiHeader), dib->get_data(), 0, 0, dib->width(),
         dib->height(), wFlags ) != FALSE;
   }

   bool draw_dib::draw (
      ::ca::graphics * pdc, 
      int      xDst,
      int      yDst,
      int      dxDst,
      int      dyDst,
      ::ca::dib *    pdib,
      int      xSrc,
      int      ySrc,
      int      dxSrc,
      int      dySrc,
      UINT     wFlags )
   {
      UNREFERENCED_PARAMETER(wFlags);
      return ::StretchDIBits(
         (HDC)pdc->get_os_data(), 
         xDst, yDst, dxDst, dyDst, 
         xSrc, ySrc, dxSrc, dySrc, 
         pdib->get_data(), &(dynamic_cast < ::win::dib * > (pdib))->m_info, 
         DIB_RGB_COLORS,
         SRCCOPY) != FALSE;
   }
      
   LPVOID draw_dib::GetBuffer ( LPBITMAPINFOHEADER lpbi,
      DWORD dwSize, DWORD dwFlags )
   {
      return DrawDibGetBuffer ( m_hdrawdib, lpbi, dwSize, dwFlags );
   }
      
   bool draw_dib::ProfileDisplay ( LPBITMAPINFOHEADER lpbi )
   {
      return DrawDibProfileDisplay ( lpbi ) != FALSE;
   }

   ::ca::palette * draw_dib::get_palette ()
   {
      return ::win::palette::from_handle(get_app(), DrawDibGetPalette ( m_hdrawdib ));
   }

   bool draw_dib::set_palette (  ::ca::palette * ppal)
   {
      return DrawDibSetPalette ( m_hdrawdib, (HPALETTE) ppal->get_os_data()  ) != FALSE;
   }

   bool draw_dib::ChangePalette ( ::ca::draw_dib * pdd, int iStart, int iLen, LPPALETTEENTRY lppe )
   {
      UNREFERENCED_PARAMETER(pdd);
      return DrawDibChangePalette ( m_hdrawdib, iStart, iLen, lppe ) != FALSE;
   }

   UINT draw_dib::Realize ( ::ca::graphics * pdc, bool fBackground )
   {
      return DrawDibRealize ( m_hdrawdib,  (HDC) pdc->get_os_data(),  fBackground );
   }
      
   bool draw_dib::Start ( LONG rate )
   {
      return DrawDibStart ( m_hdrawdib, rate ) != FALSE;
   }

   bool draw_dib::Stop ()
   {
      return DrawDibStop ( m_hdrawdib ) != FALSE;
   }

   bool draw_dib::time ( LPDRAWDIBTIME lpddtime )
   {
      return DrawDibTime ( m_hdrawdib, lpddtime ) != FALSE;
   }

} // namespace win