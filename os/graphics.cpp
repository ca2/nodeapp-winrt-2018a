#include "StdAfx.h"

#include <math.h>


#undef new


namespace win
{

   ::ca::window * graphics::GetWindow() const
   { 
      ASSERT(get_handle1() != NULL); return ::win::window::from_handle(::WindowFromDC(get_handle1())); 
   }
   
   BOOL graphics::IsPrinting() const
   {
      return m_bPrinting; 
   }
   
   BOOL graphics::CreateDC(const char * lpszDriverName,
      const char * lpszDeviceName, const char * lpszOutput, const void * lpInitData)
   {
      return Attach(::CreateDC(lpszDriverName, lpszDeviceName, lpszOutput, (const DEVMODE*)lpInitData)); 
   }
   
   BOOL graphics::CreateIC(const char * lpszDriverName, const char * lpszDeviceName, const char * lpszOutput, const void * lpInitData)
   { 
      return Attach(::CreateIC(lpszDriverName, lpszDeviceName, lpszOutput, (const DEVMODE*) lpInitData)); 
   }

   BOOL graphics::CreateCompatibleDC(::ca::graphics * pgraphics)
   { 
//      throw "not needed exception";
  //    return FALSE;
      if(pgraphics == NULL)
      {
         return Attach(::CreateCompatibleDC(NULL));
      }
      else
      {
         return Attach(::CreateCompatibleDC((HDC)(dynamic_cast<::win::graphics * >(pgraphics))->get_handle1())); 
      }
   }

   int graphics::ExcludeUpdateRgn(::ca::window * pWnd)
   { 
      ASSERT(get_handle1() != NULL); 
      return ::ExcludeUpdateRgn(get_handle1(), WIN_WINDOW(pWnd)->get_handle()); 
   }

   int graphics::GetDeviceCaps(int nIndex) const
   { 
      ASSERT(get_handle2() != NULL); 
      return ::GetDeviceCaps(get_handle2(), nIndex); 
   }

   point graphics::GetBrushOrg() const
   {
      ASSERT(get_handle1() != NULL);
      POINT point;
      VERIFY(::GetBrushOrgEx(get_handle1(), &point));
      return point;
   }
   
   point graphics::SetBrushOrg(int x, int y)
   {
      ASSERT(get_handle1() != NULL);
      POINT point;
      VERIFY(::SetBrushOrgEx(get_handle1(), x, y, &point));
      return point;
   }
   
   point graphics::SetBrushOrg(POINT point)
   {
      ASSERT(get_handle1() != NULL);
      VERIFY(::SetBrushOrgEx(get_handle1(), point.x, point.y, &point));
      return point;
   }

   int graphics::EnumObjects(int nObjectType, int (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData)
   {
      ASSERT(get_handle2() != NULL);
      return ::EnumObjects(get_handle2(), nObjectType, (GOBJENUMPROC)lpfn, lpData); 
   }

   ::ca::bitmap* graphics::SelectObject(::ca::bitmap* pBitmap)
   { 

      
      if(pBitmap == NULL)
         return NULL;

/*      if(get_handle1() == NULL)
         return NULL;
      if(pBitmap == NULL)
         return NULL;
      return dynamic_cast < ::ca::bitmap* > (SelectGdiObject(get_app(), get_handle1(), pBitmap->get_os_data()));*/
      if(m_pgraphics != NULL)
      {
         delete m_pgraphics;
      }

      

      m_pgraphics = new Gdiplus::Graphics((Gdiplus::Bitmap *) pBitmap->get_os_data());
      m_bitmap = pBitmap;
      return m_bitmap;
   }


   ::ca::graphics_object* graphics::SelectObject(::ca::graphics_object* pObject)
   {
   /*      ASSERT(get_handle1() != NULL); 
      if(pObject == NULL)
         return NULL;
      return SelectGdiObject(get_app(), get_handle1(), pObject->get_os_data()); */
      return NULL;
   }

   HGDIOBJ graphics::SelectObject(HGDIOBJ hObject) // Safe for NULL handles
   {
      //*ASSERT(get_handle1() == get_handle2()); // ASSERT a simple graphics object
      //return (hObject != NULL) ? ::SelectObject(get_handle1(), hObject) : NULL; */
      return NULL;
   }

   COLORREF graphics::GetNearestColor(COLORREF crColor) const
   { 
      return ::GetNearestColor(get_handle2(), crColor); 
   }

   UINT graphics::RealizePalette()
   { 
      return ::RealizePalette(get_handle1()); 
   }
   
   void graphics::UpdateColors()
   { 
      ::UpdateColors(get_handle1()); 
   }
   
   COLORREF graphics::GetBkColor() const
   { 
      return ::GetBkColor(get_handle2()); 
   }
   
   int graphics::GetBkMode() const
   { 
      return ::GetBkMode(get_handle2()); 
   }
   
   int graphics::GetPolyFillMode() const
   { 
      return ::GetPolyFillMode(get_handle2()); 
   }
   
   int graphics::GetROP2() const
   { 
      return ::GetROP2(get_handle2()); 
   }
   
   int graphics::GetStretchBltMode() const
   { 
      return ::GetStretchBltMode(get_handle2()); 
   }
   
   COLORREF graphics::GetTextColor() const
   { 
      return ::GetTextColor(get_handle2()); 
   }
   
   int graphics::GetMapMode() const
   { 
      return ::GetMapMode(get_handle2()); 
   }
   
   int graphics::GetGraphicsMode() const
   { 
      return ::GetGraphicsMode(get_handle2()); 
   }
   
   BOOL graphics::GetWorldTransform(XFORM* pXform) const
   { 
      return ::GetWorldTransform(get_handle2(),pXform); 
   }

   point graphics::GetViewportOrg() const
   {
      //POINT point;
      //::GetViewportOrgEx(get_handle2(), &point);

      Gdiplus::Matrix m;

      m_pgraphics->GetTransform(&m);

      Gdiplus::PointF origin(0, 0);

      m.TransformPoints(&origin);

      return point(origin.X, origin.Y);
   }

   size graphics::GetViewportExt() const
   {
      SIZE size;
      ::GetViewportExtEx(get_handle2(), &size);
      return size;
   }

   point graphics::GetWindowOrg() const
   {
      POINT point;
      ::GetWindowOrgEx(get_handle2(), &point);
      return point;
   }

   size graphics::GetWindowExt() const
   {
      SIZE size;
      ::GetWindowExtEx(get_handle2(), &size);
      return size;
   }

   // non-virtual helpers calling virtual mapping functions
   point graphics::SetViewportOrg(POINT point)
   { 
      return SetViewportOrg(point.x, point.y); 
   }

   size graphics::SetViewportExt(SIZE size)
   {
      return SetViewportExt(size.cx, size.cy); 
   }

   point graphics::SetWindowOrg(POINT point)
   {
      return SetWindowOrg(point.x, point.y); 
   }

   size graphics::SetWindowExt(SIZE size)
   {
      return SetWindowExt(size.cx, size.cy); 
   }

   void graphics::DPtoLP(LPPOINT lpPoints, int nCount) const
   { 
      ::DPtoLP(get_handle2(), lpPoints, nCount); 
   }

   void graphics::DPtoLP(LPRECT lpRect) const
   { 
      ::DPtoLP(get_handle2(), (LPPOINT)lpRect, 2); 
   }

   void graphics::LPtoDP(LPPOINT lpPoints, int nCount) const
   {
      ::LPtoDP(get_handle2(), lpPoints, nCount); 
   }

   void graphics::LPtoDP(LPRECT lpRect) const
   {
      ::LPtoDP(get_handle2(), (LPPOINT)lpRect, 2);
   }

   BOOL graphics::FillRgn(::ca::region* pRgn, ::ca::brush* pBrush)
   { 
      return ::FillRgn(get_handle1(), (HRGN)pRgn->get_os_data(), (HBRUSH)pBrush->get_os_data());
   }

   BOOL graphics::FrameRgn(::ca::region* pRgn, ::ca::brush* pBrush, int nWidth, int nHeight)
   { 
      return ::FrameRgn(get_handle1(), (HRGN)pRgn->get_os_data(), (HBRUSH)pBrush->get_os_data(), nWidth, nHeight); 
   }

   BOOL graphics::InvertRgn(::ca::region* pRgn)
   { ASSERT(get_handle1() != NULL); return ::InvertRgn(get_handle1(), (HRGN)pRgn->get_os_data()); }
   BOOL graphics::PaintRgn(::ca::region* pRgn)
   { ASSERT(get_handle1() != NULL); return ::PaintRgn(get_handle1(), (HRGN)pRgn->get_os_data()); }
   BOOL graphics::PtVisible(int x, int y) const
   { ASSERT(get_handle1() != NULL); return ::PtVisible(get_handle1(), x, y); }
   BOOL graphics::PtVisible(POINT point) const
   { ASSERT(get_handle1() != NULL); return PtVisible(point.x, point.y); } // call virtual
   BOOL graphics::RectVisible(LPCRECT lpRect) const
   { ASSERT(get_handle1() != NULL); return ::RectVisible(get_handle1(), lpRect); }
   point graphics::GetCurrentPosition() const
   {
      ASSERT(get_handle2() != NULL);
      POINT point;
      VERIFY(::GetCurrentPositionEx(get_handle2(), &point));
      return point;
   }

   BOOL graphics::Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
   { 

      ::Gdiplus::RectF rectf(x1, y1, x2, y2);

      double centerx    = (x2 + x1) / 2.0;
      double centery    = (y2 + y1) / 2.0;
      
      double start      = atan2(y3 - centery, x3 - centerx);
      double end        = atan2(y4 - centery, x4 - centerx);

   
      return m_pgraphics->DrawArc(gdiplus_pen(), rectf, start, end) == Gdiplus::Status::Ok;
      
   }
   BOOL graphics::Arc(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
   { ASSERT(get_handle1() != NULL); return ::Arc(get_handle1(), lpRect->left, lpRect->top,
   lpRect->right, lpRect->bottom, ptStart.x, ptStart.y,
   ptEnd.x, ptEnd.y); }
   BOOL graphics::Polyline(const POINT* lpPoints, int nCount)
   { ASSERT(get_handle1() != NULL); return ::Polyline(get_handle1(), lpPoints, nCount); }
   void graphics::FillRect(LPCRECT lpRect, ::ca::brush* pBrush)
   { ASSERT(get_handle1() != NULL); ::FillRect(get_handle1(), lpRect, (HBRUSH)pBrush->get_os_data()); }
   void graphics::FrameRect(LPCRECT lpRect, ::ca::brush* pBrush)
   { ASSERT(get_handle1() != NULL); ::FrameRect(get_handle1(), lpRect, (HBRUSH)pBrush->get_os_data()); }
   void graphics::InvertRect(LPCRECT lpRect)
   { ASSERT(get_handle1() != NULL); ::InvertRect(get_handle1(), lpRect); }

   BOOL graphics::DrawIcon(int x, int y, ::visual::icon * picon)
   {
      ASSERT(get_handle1() != NULL); 
      if(picon == NULL)
         return FALSE;
      return ::DrawIcon(get_handle1(), x, y, picon->m_hicon); 
   }

   BOOL graphics::DrawIcon(POINT point, ::visual::icon * picon)
   { 
      ASSERT(get_handle1() != NULL); 
      if(picon == NULL)
         return FALSE;
      return ::DrawIcon(get_handle1(), point.x, point.y, picon->m_hicon); 
   }

   BOOL graphics::DrawIcon(int x, int y, ::visual::icon * picon, int cx, int cy, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags)
   { 
      if(picon == NULL)
         return FALSE;
      Gdiplus::Bitmap b(picon->m_hicon);
      m_pgraphics->DrawImage(&b, x, y, 0, 0, cx, cy, Gdiplus::UnitPixel);
      //return ::DrawIconEx(get_handle1(), x, y, picon->m_hicon, cx, cy, istepIfAniCur, hbrFlickerFreeDraw, diFlags); 
   }

   BOOL graphics::DrawState(point pt, size size, HBITMAP hBitmap, UINT nFlags, HBRUSH hBrush)
   { ASSERT(get_handle1() != NULL); return ::DrawState(get_handle1(), hBrush,
   NULL, (LPARAM)hBitmap, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_BITMAP); }
   BOOL graphics::DrawState(point pt, size size, ::ca::bitmap* pBitmap, UINT nFlags, ::ca::brush* pBrush)
   { ASSERT(get_handle1() != NULL); return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(),
   NULL, (LPARAM)pBitmap->get_os_data(), 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_BITMAP); }
   BOOL graphics::DrawState(point pt, size size, HICON hIcon, UINT nFlags, HBRUSH hBrush)
   { ASSERT(get_handle1() != NULL); return ::DrawState(get_handle1(), hBrush, NULL,
   (LPARAM)hIcon, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_ICON); }
   BOOL graphics::DrawState(point pt, size size, HICON hIcon, UINT nFlags, ::ca::brush* pBrush)
   { ASSERT(get_handle1() != NULL); return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), NULL,
   (LPARAM)hIcon, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_ICON); }
   BOOL graphics::DrawState(point pt, size size, const char * lpszText, UINT nFlags, BOOL bPrefixText, int nTextLen, HBRUSH hBrush)
   { ASSERT(get_handle1() != NULL); return ::DrawState(get_handle1(), hBrush,
   NULL, (LPARAM)lpszText, (WPARAM)nTextLen, pt.x, pt.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)); }
   BOOL graphics::DrawState(point pt, size size, const char * lpszText, UINT nFlags, BOOL bPrefixText, int nTextLen, ::ca::brush* pBrush)
   { ASSERT(get_handle1() != NULL); return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(),
   NULL, (LPARAM)lpszText, (WPARAM)nTextLen, pt.x, pt.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)); }
   BOOL graphics::DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc, LPARAM lData, UINT nFlags, HBRUSH hBrush)
   { ASSERT(get_handle1() != NULL); return ::DrawState(get_handle1(), hBrush,
   lpDrawProc, lData, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_COMPLEX); }
   BOOL graphics::DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc, LPARAM lData, UINT nFlags, ::ca::brush* pBrush)
   { ASSERT(get_handle1() != NULL); return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(),
   lpDrawProc, lData, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_COMPLEX); }
   BOOL graphics::DrawEdge(LPRECT lpRect, UINT nEdge, UINT nFlags)
   { ASSERT(get_handle1() != NULL); return ::DrawEdge(get_handle1(), lpRect, nEdge, nFlags); }
   BOOL graphics::DrawFrameControl(LPRECT lpRect, UINT nType, UINT nState)
   { ASSERT(get_handle1() != NULL); return ::DrawFrameControl(get_handle1(), lpRect, nType, nState); }

   BOOL graphics::Chord(int x1, int y1, int x2, int y2, int x3, int y3,
      int x4, int y4)
   { ASSERT(get_handle1() != NULL); return ::Chord(get_handle1(), x1, y1, x2, y2, x3, y3, x4, y4); }
   BOOL graphics::Chord(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
   { ASSERT(get_handle1() != NULL); return ::Chord(get_handle1(), lpRect->left, lpRect->top,
   lpRect->right, lpRect->bottom, ptStart.x, ptStart.y,
   ptEnd.x, ptEnd.y); }
   void graphics::DrawFocusRect(LPCRECT lpRect)
   { ASSERT(get_handle1() != NULL); ::DrawFocusRect(get_handle1(), lpRect); }
   
   BOOL graphics::DrawEllipse(int x1, int y1, int x2, int y2)
   {
      
      return (m_pgraphics->DrawEllipse(gdiplus_pen(), x1, y1, x2 - x1, y2 - y1)) == Gdiplus::Status::Ok;

   }

   BOOL graphics::DrawEllipse(LPCRECT lpRect)
   { 
   
      /*return ::Ellipse(get_handle1(), lpRect->left, lpRect->top,
   lpRect->right, lpRect->bottom); */

      return (m_pgraphics->DrawEllipse(gdiplus_pen(), lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top)) == Gdiplus::Status::Ok;
   
   }

   BOOL graphics::FillEllipse(int x1, int y1, int x2, int y2)
   {
      
      return (m_pgraphics->FillEllipse(gdiplus_brush(), x1, y1, x2 - x1, y2 - y1)) == Gdiplus::Status::Ok;

   }

   BOOL graphics::FillEllipse(LPCRECT lpRect)
   { 
   
      /*return ::Ellipse(get_handle1(), lpRect->left, lpRect->top,
   lpRect->right, lpRect->bottom); */

      return (m_pgraphics->FillEllipse(gdiplus_brush(), lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top)) == Gdiplus::Status::Ok;
   
   }

   BOOL graphics::Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
   { ASSERT(get_handle1() != NULL); return ::Pie(get_handle1(), x1, y1, x2, y2, x3, y3, x4, y4); }
   BOOL graphics::Pie(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
   { ASSERT(get_handle1() != NULL); return ::Pie(get_handle1(), lpRect->left, lpRect->top,
   lpRect->right, lpRect->bottom, ptStart.x, ptStart.y,
   ptEnd.x, ptEnd.y); }
   BOOL graphics::Polygon(const POINT* lpPoints, int nCount)
   {

      if(nCount <= 0)
         return TRUE;

      BOOL bOk1 = FALSE;
      BOOL bOk2 = FALSE;

      Gdiplus::Point * ppoints = new Gdiplus::Point[nCount];
      try
      {

         for(int i = 0; i < nCount; i++)
         {
            ppoints[i].X = lpPoints[i].x;
            ppoints[i].Y = lpPoints[i].y;
         }
   
         bOk1 = m_pgraphics->FillPolygon(gdiplus_brush(), ppoints, nCount, gdiplus_get_fill_mode()) == Gdiplus::Status::Ok;
         bOk2 = m_pgraphics->DrawPolygon(gdiplus_pen(), ppoints, nCount) == Gdiplus::Status::Ok;
      }
      catch(...)
      {
      }
      
      try
      {
         delete ppoints;         
      }
      catch(...)
      {
      }


      return bOk1 && bOk2;
   
   }
   BOOL graphics::PolyPolygon(const POINT* lpPoints, const INT* lpPolyCounts, int nCount)
   { ASSERT(get_handle1() != NULL); return ::PolyPolygon(get_handle1(), lpPoints, lpPolyCounts, nCount); }
   
   BOOL graphics::Rectangle(int x1, int y1, int x2, int y2)
   { 
      
      Gdiplus::RectF rectf(x1, y1, x2 - x1, y2 - y1);

      BOOL bOk1 = m_pgraphics->FillRectangle(gdiplus_brush(), rectf) == Gdiplus::Status::Ok;
      BOOL bOk2 = m_pgraphics->DrawRectangle(gdiplus_pen(), rectf) == Gdiplus::Status::Ok;

      return bOk1 && bOk2;

   }
   
   BOOL graphics::Rectangle(LPCRECT lpRect)
   { 

      return Rectangle(lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top);

   }

   BOOL graphics::DrawRectangle(int x1, int y1, int x2, int y2)
   { 
      
      Gdiplus::RectF rectf(x1, y1, x2 - x1, y2 - y1);

      BOOL bOk = m_pgraphics->DrawRectangle(gdiplus_pen(), rectf) == Gdiplus::Status::Ok;

      return bOk;

   }
   
   BOOL graphics::DrawRectangle(LPCRECT lpRect)
   { 

      return DrawRectangle(lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top);

   }

   BOOL graphics::FillRectangle(int x1, int y1, int x2, int y2)
   { 
      
      Gdiplus::RectF rectf(x1, y1, x2 - x1, y2 - y1);

      BOOL bOk = m_pgraphics->FillRectangle(gdiplus_brush(), rectf) == Gdiplus::Status::Ok;

      return bOk;

   }
   
   BOOL graphics::FillRectangle(LPCRECT lpRect)
   { 

      return FillRectangle(lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top);

   }

   BOOL graphics::RoundRect(int x1, int y1, int x2, int y2, int x3, int y3)
   { ASSERT(get_handle1() != NULL); return ::RoundRect(get_handle1(), x1, y1, x2, y2, x3, y3); }
   BOOL graphics::RoundRect(LPCRECT lpRect, POINT point)
   { ASSERT(get_handle1() != NULL); return ::RoundRect(get_handle1(), lpRect->left, lpRect->top,
   lpRect->right, lpRect->bottom, point.x, point.y); }
   BOOL graphics::PatBlt(int x, int y, int nWidth, int nHeight, DWORD dwRop)
   { ASSERT(get_handle1() != NULL); return ::PatBlt(get_handle1(), x, y, nWidth, nHeight, dwRop); }
   
   
   BOOL graphics::BitBlt(int x, int y, int nWidth, int nHeight, ::ca::graphics * pgraphicsSrc, int xSrc, int ySrc, DWORD dwRop)
   { 
      
      if(pgraphicsSrc == NULL)
         return FALSE;

      try
      {
         return m_pgraphics->DrawImage(
            (Gdiplus::Bitmap *) (dynamic_cast < ::win::graphics * > (pgraphicsSrc))->m_bitmap->get_os_data(),
            x, y , xSrc, ySrc, nWidth, nHeight, Gdiplus::UnitPixel) == Gdiplus::Status::Ok;
      }
      catch(...)
      {
         return FALSE;
      }
      //return ::BitBlt(get_handle1(), x, y, nWidth, nHeight, WIN_HDC(pgraphicsSrc), xSrc, ySrc, dwRop); 
   }


   BOOL graphics::StretchBlt(int x, int y, int nWidth, int nHeight, ::ca::graphics * pgraphicsSrc,
      int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop)
   { ASSERT(get_handle1() != NULL); return ::StretchBlt(get_handle1(), x, y, nWidth, nHeight,
   WIN_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth, nSrcHeight,
   dwRop); }
   COLORREF graphics::GetPixel(int x, int y) const
   { ASSERT(get_handle1() != NULL); return ::GetPixel(get_handle1(), x, y); }
   COLORREF graphics::GetPixel(POINT point) const
   { ASSERT(get_handle1() != NULL); return ::GetPixel(get_handle1(), point.x, point.y); }
   COLORREF graphics::SetPixel(int x, int y, COLORREF crColor)
   { ASSERT(get_handle1() != NULL); return ::SetPixel(get_handle1(), x, y, crColor); }
   COLORREF graphics::SetPixel(POINT point, COLORREF crColor)
   { ASSERT(get_handle1() != NULL); return ::SetPixel(get_handle1(), point.x, point.y, crColor); }
   BOOL graphics::FloodFill(int x, int y, COLORREF crColor)
   { ASSERT(get_handle1() != NULL); return ::FloodFill(get_handle1(), x, y, crColor); }
   BOOL graphics::ExtFloodFill(int x, int y, COLORREF crColor, UINT nFillType)
   { ASSERT(get_handle1() != NULL); return ::ExtFloodFill(get_handle1(), x, y, crColor, nFillType); }
   

   // true blend
   // COLOR_DEST = SRC_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA) * COLOR_DST

   // double blend
   //// COLOR_DEST = SRC_ALPHA * BLEND_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA * BLEND_ALPHA) * COLOR_DST

   BOOL graphics::TextOut(int x, int y, const string & str)
   { 
      if(m_pdibAlphaBlend != NULL)
      {
         if(GetBkMode() == TRANSPARENT)
         {
         //   return TRUE;
            rect rectIntersect(m_ptAlphaBlend, m_pdibAlphaBlend->size());
            rect rectText(point(x, y), GetTextExtent(str));
            if(rectIntersect.intersect(rectIntersect, rectText))
            {
               ::ca::dib_sp dib0(get_app());
               dib0->create(rectText.size());
               dib0->get_graphics()->SetTextColor(RGB(255, 255, 255));
               dib0->get_graphics()->SelectObject(&GetCurrentFont());
               dib0->get_graphics()->SetBkMode(TRANSPARENT);
               dib0->get_graphics()->TextOut(0, 0, str);
               dib0->ToAlpha(0);
               ::ca::dib_sp dib1(get_app());
               dib1->create(rectText.size());
               dib1->get_graphics()->SetTextColor(GetTextColor());
               dib1->get_graphics()->SelectObject(&GetCurrentFont());
               dib1->get_graphics()->SetBkMode(TRANSPARENT);
               dib1->get_graphics()->TextOut(0, 0, str);
               dib1->channel_from(visual::rgba::channel_alpha, dib0);
               ::ca::dib_sp dib2(get_app());
               dib2->create(rectText.size());
               dib2->Fill(255, 0, 0, 0);
               dib2->from(point(max(0, m_ptAlphaBlend.x - x), max(0, m_ptAlphaBlend.y - y)),
                  m_pdibAlphaBlend->get_graphics(), point(max(0, x - m_ptAlphaBlend.x), max(0, y - m_ptAlphaBlend.y)), rectText.size());
               dib1->channel_multiply(visual::rgba::channel_alpha, dib2);
               /*::ca::dib_sp dib3(get_app());
               dib1->mult_alpha(dib3);*/

               keeper < ::ca::dib * > keep(&m_pdibAlphaBlend, NULL, m_pdibAlphaBlend, true);

               return System.imaging().true_blend(this, point(x, y), rectText.size(), dib1->get_graphics(), null_point());

               /*BLENDFUNCTION bf;
               bf.BlendOp     = AC_SRC_OVER;
               bf.BlendFlags  = 0;
               bf.SourceConstantAlpha = 0xFF;
               bf.AlphaFormat = AC_SRC_ALPHA;
               return ::AlphaBlend(get_handle1(), x, y, 
                  rectText.width(), rectText.height(), WIN_HDC(dib1->get_graphics()), 0, 0, rectText.width(), 
                  rectText.height(), bf) != FALSE; */
            }
         }
      }
      
      //ASSERT(get_handle1() != NULL); 
      //wstring wstr = gen::international::utf8_to_unicode(str);
      return TextOut(x, y, str, str.get_length()); 
   
   } // call virtual

   BOOL graphics::ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect, const char * lpszString, UINT nCount, LPINT lpDxWidths)
   { 
      ASSERT(get_handle1() != NULL); 
      return ::ExtTextOut(get_handle1(), x, y, nOptions, lpRect, lpszString, nCount, lpDxWidths); 
   }

   BOOL graphics::ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect,
      const string & str, LPINT lpDxWidths)
   { ASSERT(get_handle1() != NULL); return ::ExtTextOut(get_handle1(), x, y, nOptions, lpRect,
   str, (UINT)str.get_length(), lpDxWidths); }
   size graphics::TabbedTextOut(int x, int y, const char * lpszString, int nCount,
      int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin)
   { ASSERT(get_handle1() != NULL); return ::TabbedTextOut(get_handle1(), x, y, lpszString, nCount,
   nTabPositions, lpnTabStopPositions, nTabOrigin); }
   size graphics::TabbedTextOut(int x, int y, const string & str,
      int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin)
   { ASSERT(get_handle1() != NULL); return ::TabbedTextOut(get_handle1(), x, y, str, (int)str.get_length(),
   nTabPositions, lpnTabStopPositions, nTabOrigin); }
   #pragma push_macro("DrawText")
   #pragma push_macro("DrawTextEx")
   #undef DrawText
   #undef DrawTextEx
   int graphics::DrawText(const char * lpszString, int nCount, LPRECT lpRect, UINT nFormat)
   { return _AFX_FUNCNAME(DrawText)(lpszString, nCount, lpRect, nFormat); }
   int graphics::DrawText(const string & str, LPRECT lpRect, UINT nFormat)
   { return _AFX_FUNCNAME(DrawText)(str, lpRect, nFormat); }
   int graphics::DrawTextEx(LPTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat, LPDRAWTEXTPARAMS lpDTParams)
   { return _AFX_FUNCNAME(DrawTextEx)(lpszString, nCount, lpRect, nFormat, lpDTParams); }
   int graphics::DrawTextEx(const string & str, LPRECT lpRect, UINT nFormat, LPDRAWTEXTPARAMS lpDTParams)
   { return _AFX_FUNCNAME(DrawTextEx)(str, lpRect, nFormat, lpDTParams); }
   #pragma pop_macro("DrawText")
   #pragma pop_macro("DrawTextEx")

   size graphics::GetTabbedTextExtent(const char * lpszString, int nCount,
      int nTabPositions, LPINT lpnTabStopPositions) const
   { ASSERT(get_handle2() != NULL); return ::GetTabbedTextExtent(get_handle2(), lpszString, nCount,
   nTabPositions, lpnTabStopPositions); }
   size graphics::GetTabbedTextExtent(const string & str,
      int nTabPositions, LPINT lpnTabStopPositions) const
   { ASSERT(get_handle2() != NULL); return ::GetTabbedTextExtent(get_handle2(),
   str, (int)str.get_length(), nTabPositions, lpnTabStopPositions); }
   size graphics::GetOutputTabbedTextExtent(const char * lpszString, int nCount,
      int nTabPositions, LPINT lpnTabStopPositions) const
   { ASSERT(get_handle1() != NULL); return ::GetTabbedTextExtent(get_handle1(), lpszString, nCount,
   nTabPositions, lpnTabStopPositions); }
   size graphics::GetOutputTabbedTextExtent(const string & str,
      int nTabPositions, LPINT lpnTabStopPositions) const
   { ASSERT(get_handle1() != NULL); return ::GetTabbedTextExtent(get_handle1(),
   str, (int)str.get_length(), nTabPositions, lpnTabStopPositions); }
   BOOL graphics::GrayString(::ca::brush* pBrush,
      BOOL (CALLBACK* lpfnOutput)(HDC, LPARAM, int),
      LPARAM lpData, int nCount,
      int x, int y, int nWidth, int nHeight)
   { ASSERT(get_handle1() != NULL); return ::GrayString(get_handle1(), (HBRUSH)pBrush->get_os_data(),
   (GRAYSTRINGPROC)lpfnOutput, lpData, nCount, x, y, nWidth, nHeight); }
   UINT graphics::GetTextAlign() const
   { ASSERT(get_handle2() != NULL); return ::GetTextAlign(get_handle2()); }
   int graphics::GetTextFace(__in int nCount, __out_ecount_part_z(nCount, return + 1) LPTSTR lpszFacename) const
   { ASSERT(get_handle2() != NULL); return ::GetTextFace(get_handle2(), nCount, lpszFacename); }
   int graphics::GetTextFace(string & rString) const
   { ASSERT(get_handle2() != NULL); int nResult = ::GetTextFace(get_handle2(),
   256, rString.GetBuffer(256)); rString.ReleaseBuffer();
   return nResult; }
   BOOL graphics::_AFX_FUNCNAME(GetTextMetrics)(LPTEXTMETRIC lpMetrics) const
   { 
      //ASSERT(get_handle2() != NULL); return ::GetTextMetrics(get_handle2(), lpMetrics); 
      /*wstring wstr(L"123AWZwmc");
      Gdiplus::RectF rect;
      Gdiplus::RectF rect2;
      Gdiplus::PointF origin(0, 0);
      m_pgraphics->MeasureString(wstr.m_pwsz, -1, (Gdiplus::Font *) m_font->get_os_data(), origin, &rect);

      wstr = L"123AWZwmcpQ�g";
      m_pgraphics->MeasureString(wstr.m_pwsz, -1, (Gdiplus::Font *) m_font->get_os_data(), origin, &rect2);

      lpMetrics->tmAveCharWidth = rect.Width / (double) wstr.get_length();
      lpMetrics->tmAscent = rect.Height;
      lpMetrics->tmDescent = rect2.Height - rect.Height;*/

      Gdiplus::FontFamily family;

      ((graphics * )this)->gdiplus_font()->GetFamily(&family);

      double dHeight = family.GetEmHeight(((graphics * )this)->gdiplus_font()->GetStyle());

      lpMetrics->tmAscent = ((graphics * )this)->gdiplus_font()->GetSize() * family.GetCellAscent(((graphics * )this)->gdiplus_font()->GetStyle()) / dHeight;
      lpMetrics->tmDescent = ((graphics * )this)->gdiplus_font()->GetSize() * family.GetCellDescent(((graphics * )this)->gdiplus_font()->GetStyle()) / dHeight;
      lpMetrics->tmHeight = ((graphics * )this)->gdiplus_font()->GetSize() * family.GetLineSpacing(((graphics * )this)->gdiplus_font()->GetStyle()) / dHeight;
      
      const Gdiplus::FontFamily * pfamilyMono = family.GenericMonospace();

      ::Gdiplus::Font font2(pfamilyMono, pfamilyMono->GetEmHeight(((graphics * )this)->gdiplus_font()->GetStyle()));
      
      wstring wstr(L"123AWZwmc123AWZwmcpQ�g");
      Gdiplus::RectF rect;
      Gdiplus::RectF rect2;
      Gdiplus::PointF origin(0, 0);
      m_pgraphics->MeasureString(wstr.m_pwsz, -1, ((graphics * )this)->gdiplus_font(), origin, &rect);

      /*wstr = L"";
      m_pgraphics->MeasureString(wstr.m_pwsz, -1, (Gdiplus::Font *) m_font->get_os_data(), origin, &rect2);*/

      lpMetrics->tmAveCharWidth = rect.Width / (double) wstr.get_length();


      return TRUE;
   }
#pragma push_macro("GetTextMetrics")
#undef GetTextMetrics
   BOOL graphics::GetTextMetrics(LPTEXTMETRIC lpMetrics) const
   { return _AFX_FUNCNAME(GetTextMetrics)(lpMetrics); }
#pragma pop_macro("GetTextMetrics")
   BOOL graphics::GetOutputTextMetrics(LPTEXTMETRIC lpMetrics) const
   { ASSERT(get_handle1() != NULL); return ::GetTextMetrics(get_handle1(), lpMetrics); }
   int graphics::GetTextCharacterExtra() const
   { ASSERT(get_handle2() != NULL); return ::GetTextCharacterExtra(get_handle2()); }
   BOOL graphics::GetCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const
   { ASSERT(get_handle2() != NULL); return ::GetCharWidth(get_handle2(), nFirstChar, nLastChar, lpBuffer); }
   BOOL graphics::GetOutputCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const
   { ASSERT(get_handle1() != NULL); return ::GetCharWidth(get_handle1(), nFirstChar, nLastChar, lpBuffer); }
   DWORD graphics::GetFontLanguageInfo() const
   { ASSERT(get_handle1() != NULL); return ::GetFontLanguageInfo(get_handle1()); }

   DWORD graphics::GetCharacterPlacement(const char * lpString, int nCount, int nMaxExtent, LPGCP_RESULTS lpResults, DWORD dwFlags) const
   { ASSERT(get_handle1() != NULL); return ::GetCharacterPlacement(get_handle1(), lpString, nCount, nMaxExtent, lpResults, dwFlags); }
   DWORD graphics::GetCharacterPlacement(string & str, int nMaxExtent, LPGCP_RESULTS lpResults, DWORD dwFlags) const
   { ASSERT(get_handle1() != NULL); return ::GetCharacterPlacement(get_handle1(), (const char *)str, str.get_length(), nMaxExtent, lpResults, dwFlags); }


   size graphics::GetAspectRatioFilter() const
   {
      ASSERT(get_handle2() != NULL);
      SIZE size;
      VERIFY(::GetAspectRatioFilterEx(get_handle2(), &size));
      return size;
   }
   BOOL graphics::ScrollDC(int dx, int dy,
      LPCRECT lpRectScroll, LPCRECT lpRectClip,
      ::ca::region* pRgnUpdate, LPRECT lpRectUpdate)
   { ASSERT(get_handle1() != NULL); return ::ScrollDC(get_handle1(), dx, dy, lpRectScroll,
   lpRectClip, (HRGN)pRgnUpdate->get_os_data(), lpRectUpdate); }

   // Printer Escape Functions
   int graphics::Escape(int nEscape, int nCount, const char * lpszInData, LPVOID lpOutData)
   { ASSERT(get_handle1() != NULL); return ::Escape(get_handle1(), nEscape, nCount, lpszInData, lpOutData);}

   // graphics 3.1 Specific functions
   UINT graphics::SetBoundsRect(LPCRECT lpRectBounds, UINT flags)
   { ASSERT(get_handle1() != NULL); return ::SetBoundsRect(get_handle1(), lpRectBounds, flags); }
   UINT graphics::GetBoundsRect(LPRECT lpRectBounds, UINT flags)
   { ASSERT(get_handle2() != NULL); return ::GetBoundsRect(get_handle2(), lpRectBounds, flags); }
   BOOL graphics::ResetDC(const DEVMODE* lpDevMode)
   { ASSERT(get_handle2() != NULL); return ::ResetDC(get_handle2(), lpDevMode) != NULL; }
   UINT graphics::GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRIC lpotm) const
   { ASSERT(get_handle2() != NULL); return ::GetOutlineTextMetrics(get_handle2(), cbData, lpotm); }
   BOOL graphics::GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABC lpabc) const
   { ASSERT(get_handle2() != NULL); return ::GetCharABCWidths(get_handle2(), nFirstChar, nLastChar, lpabc); }
   DWORD graphics::GetFontData(DWORD dwTable, DWORD dwOffset, LPVOID lpData,
      DWORD cbData) const
   { ASSERT(get_handle2() != NULL); return ::GetFontData(get_handle2(), dwTable, dwOffset, lpData, cbData); }
   int graphics::GetKerningPairs(int nPairs, LPKERNINGPAIR lpkrnpair) const
   { ASSERT(get_handle2() != NULL); return ::GetKerningPairs(get_handle2(), nPairs, lpkrnpair); }
   DWORD graphics::GetGlyphOutline(UINT nChar, UINT nFormat, LPGLYPHMETRICS lpgm,
      DWORD cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2) const
   { ASSERT(get_handle2() != NULL); return ::GetGlyphOutline(get_handle2(), nChar, nFormat,
   lpgm, cbBuffer, lpBuffer, lpmat2); }

   // ::userbase::document handling functions
   int graphics::StartDoc(LPDOCINFO lpDocInfo)
   { ASSERT(get_handle1() != NULL); return ::StartDoc(get_handle1(), lpDocInfo); }
   int graphics::StartPage()
   { ASSERT(get_handle1() != NULL); return ::StartPage(get_handle1()); }
   int graphics::EndPage()
   { ASSERT(get_handle1() != NULL); return ::EndPage(get_handle1()); }
   int graphics::SetAbortProc(BOOL (CALLBACK* lpfn)(HDC, int))
   { ASSERT(get_handle1() != NULL); return ::SetAbortProc(get_handle1(), (ABORTPROC)lpfn); }
   int graphics::AbortDoc()
   { ASSERT(get_handle1() != NULL); return ::AbortDoc(get_handle1()); }
   int graphics::EndDoc()
   { ASSERT(get_handle1() != NULL); return ::EndDoc(get_handle1()); }

   BOOL graphics::MaskBlt(int x, int y, int nWidth, int nHeight, ::ca::graphics * pgraphicsSrc,
      int xSrc, int ySrc, ::ca::bitmap& maskBitmap, int xMask, int yMask, DWORD dwRop)
   { ASSERT(get_handle1() != NULL); return ::MaskBlt(get_handle1(), x, y, nWidth, nHeight, WIN_HDC(pgraphicsSrc),
   xSrc, ySrc,  (HBITMAP)maskBitmap.get_os_data(), xMask, yMask, dwRop); }
   BOOL graphics::PlgBlt(LPPOINT lpPoint, ::ca::graphics * pgraphicsSrc, int xSrc, int ySrc,
      int nWidth, int nHeight, ::ca::bitmap& maskBitmap, int xMask, int yMask)
   { ASSERT(get_handle1() != NULL); 
   return ::PlgBlt(get_handle1(), lpPoint, WIN_HDC(pgraphicsSrc), xSrc, ySrc, nWidth,
   nHeight, (HBITMAP)maskBitmap.get_os_data(), xMask, yMask); }
   BOOL graphics::SetPixelV(int x, int y, COLORREF crColor)
   { ASSERT(get_handle1() != NULL); 
   return ::SetPixelV(get_handle1(), x, y, crColor); }
   BOOL graphics::SetPixelV(POINT point, COLORREF crColor)
   { ASSERT(get_handle1() != NULL); 
   return ::SetPixelV(get_handle1(), point.x, point.y, crColor); }
   BOOL graphics::AngleArc(int x, int y, int nRadius,
      float fStartAngle, float fSweepAngle)
   { ASSERT(get_handle1() != NULL); 
   return ::AngleArc(get_handle1(), x, y, nRadius, fStartAngle, fSweepAngle); }
   BOOL graphics::ArcTo(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
   { ASSERT(get_handle1() != NULL); 
   return ArcTo(lpRect->left, lpRect->top, lpRect->right,
   lpRect->bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y); }
   int graphics::GetArcDirection() const
   { ASSERT(get_handle2() != NULL);
   return ::GetArcDirection(get_handle2()); }
   BOOL graphics::PolyPolyline(const POINT* lpPoints, const DWORD* lpPolyPoints,
      int nCount)
   { ASSERT(get_handle1() != NULL); 
   return ::PolyPolyline(get_handle1(), lpPoints, lpPolyPoints, nCount); }
   BOOL graphics::GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust) const
   { ASSERT(get_handle2() != NULL); 
   return ::GetColorAdjustment(get_handle2(), lpColorAdjust); }

   ::ca::pen & graphics::GetCurrentPen() const
   {

      return *m_sppen.m_p;

   }

   ::ca::brush & graphics::GetCurrentBrush() const
   {
      
      return *m_spbrush.m_p;

   }
   
   ::ca::palette & graphics::GetCurrentPalette() const
   {

      return *(::ca::palette *)NULL;

   }

   ::ca::font & graphics::GetCurrentFont() const
   {

      return *m_spfont.m_p;

   }

   ::ca::bitmap & graphics::GetCurrentBitmap() const
   {

      return *m_bitmap.m_p;

   }

   BOOL graphics::PolyBezier(const POINT* lpPoints, int nCount)
   {
      ASSERT(get_handle1() != NULL); 
      return ::PolyBezier(get_handle1(), lpPoints, nCount); 
   }

   int graphics::DrawEscape(int nEscape, int nInputSize, const char * lpszInputData)
   {
      ASSERT(get_handle1() != NULL); 
      return ::DrawEscape(get_handle1(), nEscape, nInputSize, lpszInputData); 
   }
   
   int graphics::Escape(__in int nEscape, __in int nInputSize, __in_bcount(nInputSize) const char * lpszInputData,  __in int nOutputSize, __out_bcount(nOutputSize) char * lpszOutputData)
   {
       ASSERT(get_handle1() != NULL); 
       return ::ExtEscape(get_handle1(), nEscape, nInputSize, lpszInputData, nOutputSize, lpszOutputData); 
   }

   BOOL graphics::GetCharABCWidths(UINT nFirstChar, UINT nLastChar,
      LPABCFLOAT lpABCF) const
   { 
      ASSERT(get_handle2() != NULL); 
      return ::GetCharABCWidthsFloat(get_handle2(), nFirstChar, nLastChar, lpABCF); 
   }
   BOOL graphics::GetCharWidth(UINT nFirstChar, UINT nLastChar, float* lpFloatBuffer) const
   {
      ASSERT(get_handle2() != NULL); 
      return ::GetCharWidthFloat(get_handle2(), nFirstChar, nLastChar, lpFloatBuffer); 
   }

   BOOL graphics::AbortPath()
   {
      ASSERT(get_handle1() != NULL); 
      return ::AbortPath(get_handle1()); 
   }
   BOOL graphics::BeginPath()
   { 
      ASSERT(get_handle1() != NULL); 
      return ::BeginPath(get_handle1()); 
   }
   BOOL graphics::CloseFigure()
   {
      ASSERT(get_handle1() != NULL); 
      return ::CloseFigure(get_handle1()); 
   }
   BOOL graphics::EndPath()
   { 
      ASSERT(get_handle1() != NULL); 
      return ::EndPath(get_handle1()); 
   }
   BOOL graphics::FillPath()
   { 
      ASSERT(get_handle1() != NULL); 
      return ::FillPath(get_handle1()); 
   }
   BOOL graphics::FlattenPath()
   {
      ASSERT(get_handle1() != NULL); 
      return ::FlattenPath(get_handle1()); 
   }
   float graphics::GetMiterLimit() const
   {
      ASSERT(get_handle1() != NULL); 
      float fMiterLimit;
      VERIFY(::GetMiterLimit(get_handle1(), &fMiterLimit)); 
      return fMiterLimit; 
   }
   int graphics::GetPath(LPPOINT lpPoints, LPBYTE lpTypes, int nCount) const
   { 
      ASSERT(get_handle1() != NULL); 
      return ::GetPath(get_handle1(), lpPoints, lpTypes, nCount); 
   }
   BOOL graphics::SetMiterLimit(float fMiterLimit)
   {
      ASSERT(get_handle1() != NULL); 
      return ::SetMiterLimit(get_handle1(), fMiterLimit, NULL); 
   }
   BOOL graphics::StrokeAndFillPath()
   { 
      ASSERT(get_handle1() != NULL); 
      return ::StrokeAndFillPath(get_handle1()); 
   }
   BOOL graphics::StrokePath()
   {
      ASSERT(get_handle1() != NULL); 
      return ::StrokePath(get_handle1()); 
   }
   BOOL graphics::WidenPath()
   {
      ASSERT(get_handle1() != NULL); 
      return ::WidenPath(get_handle1()); 
   }

   BOOL graphics::AddMetaFileComment(UINT nDataSize, const BYTE* pCommentData)
   { 
      ASSERT(get_handle1() != NULL); 
      return ::GdiComment(get_handle1(), nDataSize, pCommentData); 
   }
   
   BOOL graphics::PlayMetaFile(HENHMETAFILE hEnhMF, LPCRECT lpBounds)
   { 
      
      return ::PlayEnhMetaFile(get_handle1(), hEnhMF, lpBounds); 
   
   }

   // true blend
   // COLOR_DEST = SRC_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA) * COLOR_DST

   // double blend
   // COLOR_DEST = SRC_ALPHA * BLEND_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA * BLEND_ALPHA) * COLOR_DST

   bool graphics::alpha_blend(int xDest, int yDest, int nDestWidth, int nDestHeight,
      ::ca::graphics * pgraphicsSrc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, BLENDFUNCTION blend)
   { 
      

      if(get_handle1() == NULL)
         return false;

      
      if(m_pdibAlphaBlend != NULL)
      {


         rect rectIntersect(m_ptAlphaBlend, m_pdibAlphaBlend->size());


         ::ca::dib * pdibWork = NULL;
         ::ca::dib * pdibWork2 = NULL;
//         ::ca::dib * pdibWork3 = NULL;
         ::ca::dib * pdibWork4 = NULL;

            
         class point ptSrc(xSrc, ySrc);
         class point ptDest(xDest, yDest);
         class size size(nDestWidth, nDestHeight);



         ::ca::dib_sp spdib;
         if(pdibWork == NULL)
         {
            spdib.create(get_app());
            pdibWork = spdib;
         }
         if(pdibWork == NULL)
            return false;
         if(!pdibWork->create(size))
            return false;
         if(!pdibWork->from(null_point(), pgraphicsSrc, ptSrc, size))
            return false;




         ::ca::dib_sp spdib2;
         if(pdibWork2 == NULL)
         {
            spdib2.create(get_app());
            pdibWork2 = spdib2;
         }


         ::ca::dib_sp spdib4;
         if(pdibWork4 == NULL)
         {
            spdib4.create(get_app());
            pdibWork4 = spdib4;
         }
         if(pdibWork4 == NULL)
            return false;
         if(!pdibWork4->create(size))
            return false;


         pdibWork4->Fill(255, 0, 0, 0);
            
         pdibWork4->from(point(max(0, m_ptAlphaBlend.x - xDest), max(0, m_ptAlphaBlend.y - yDest)),
            m_pdibAlphaBlend->get_graphics(), point(max(0, xDest - m_ptAlphaBlend.x), max(0, yDest - m_ptAlphaBlend.y)), size);
   
         pdibWork->channel_multiply(visual::rgba::channel_alpha, pdibWork4);


         keeper < ::ca::dib * > keep(&m_pdibAlphaBlend, NULL, m_pdibAlphaBlend, true);


         return System.imaging().true_blend(this, ptDest, size, pdibWork->get_graphics(), ptSrc); 


      }

      return ::AlphaBlend(get_handle1(), xDest, yDest, 
         nDestWidth, nDestHeight, WIN_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth, 
         nSrcHeight, blend) != FALSE; 
   }


   BOOL graphics::TransparentBlt(int xDest, int yDest, int nDestWidth, 
      int nDestHeight, ::ca::graphics * pgraphicsSrc, int xSrc, int ySrc, int nSrcWidth, 
      int nSrcHeight, UINT crTransparent)
   { ASSERT(get_handle1() != NULL); return ::TransparentBlt(get_handle1(), xDest, yDest, 
   nDestWidth, nDestHeight, WIN_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth, 
   nSrcHeight, crTransparent); }
   BOOL graphics::GradientFill(TRIVERTEX* pVertices, ULONG nVertices,
      void * pMesh, ULONG nMeshElements, DWORD dwMode)
   { ASSERT(get_handle1() != NULL); return ::GradientFill(get_handle1(), pVertices, nVertices,
   pMesh, nMeshElements, dwMode); }

   // This is ca2 API library.
   // 
   // 
   //
   // 
   // 
   // 
   // 
   // 

   // Always Inline. Functions only in Win98/Win2K or later

   inline COLORREF graphics::GetDCBrushColor() const
   { ASSERT(get_handle1() != NULL); return ::GetDCBrushColor(get_handle1()); }
   inline COLORREF graphics::SetDCBrushColor(COLORREF crColor)
   { ASSERT(get_handle1() != NULL); return ::SetDCBrushColor(get_handle1(), crColor); }

   inline COLORREF graphics::GetDCPenColor() const
   { ASSERT(get_handle1() != NULL); return ::GetDCPenColor(get_handle1()); }
   inline COLORREF graphics::SetDCPenColor(COLORREF crColor)
   { ASSERT(get_handle1() != NULL); return ::SetDCPenColor(get_handle1(), crColor); }

   inline BOOL graphics::GetCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pgi, LPABC lpabc) const
   { ASSERT(get_handle1() != NULL); return ::GetCharABCWidthsI(get_handle1(), giFirst, cgi, pgi, lpabc); }
   inline BOOL graphics::GetCharWidthI(UINT giFirst, UINT cgi, LPWORD pgi, LPINT lpBuffer) const
   { ASSERT(get_handle1() != NULL); return ::GetCharWidthI(get_handle1(), giFirst, cgi, pgi, lpBuffer); }

   inline BOOL graphics::GetTextExtentExPointI(LPWORD pgiIn, int cgi, int nMaxExtent, LPINT lpnFit, LPINT alpDx, LPSIZE lpSize) const
   { 
      ENSURE(lpSize != NULL); 
      ASSERT(get_handle1() != NULL); 
      return ::GetTextExtentExPointI(get_handle1(), pgiIn, cgi, nMaxExtent, lpnFit, alpDx, lpSize); 
   }
   inline BOOL graphics::GetTextExtentPointI(LPWORD pgiIn, int cgi, LPSIZE lpSize) const
   {
      ENSURE(lpSize != NULL); 
      ASSERT(get_handle1() != NULL); 
      return ::GetTextExtentPointI(get_handle1(), pgiIn, cgi, lpSize); 
   }






   AFX_STATIC_DATA HBRUSH _afxHalftoneBrush = 0;

   void AFX_CDECL AfxWingdixTerm()
   {
      AfxDeleteObject((HGDIOBJ*)&_afxHalftoneBrush);
   }
   char _afxWingdixTerm = 0;

   /////////////////////////////////////////////////////////////////////////////
   // More coordinate transforms (in separate file to avoid transitive refs)

#define HIMETRIC_INCH   2540    // HIMETRIC units per inch

   void graphics::DPtoHIMETRIC(LPSIZE lpSize) const
   {
      ASSERT(fx_is_valid_address(lpSize, sizeof(SIZE)));

      int nMapMode;
      if (this != NULL && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
         nMapMode != MM_TEXT)
      {
         // when using a constrained ::collection::map mode, ::collection::map against physical inch
         ((::ca::graphics *)this)->SetMapMode(MM_HIMETRIC);
         DPtoLP(lpSize);
         ((::ca::graphics *)this)->SetMapMode(nMapMode);
      }
      else
      {
         // ::collection::map against logical inch for non-constrained mapping modes
         int cxPerInch, cyPerInch;
         if (this != NULL)
         {
            ASSERT_VALID(this);
            ASSERT(get_handle1() != NULL);  // no HDC attached or created?
            cxPerInch = GetDeviceCaps(LOGPIXELSX);
            cyPerInch = GetDeviceCaps(LOGPIXELSY);
         }
         else
         {
            cxPerInch = afxData.cxPixelsPerInch;
            cyPerInch = afxData.cyPixelsPerInch;
         }
         ASSERT(cxPerInch != 0 && cyPerInch != 0);
         lpSize->cx = MulDiv(lpSize->cx, HIMETRIC_INCH, cxPerInch);
         lpSize->cy = MulDiv(lpSize->cy, HIMETRIC_INCH, cyPerInch);
      }
   }

   void graphics::HIMETRICtoDP(LPSIZE lpSize) const
   {
      ASSERT(fx_is_valid_address(lpSize, sizeof(SIZE)));

      int nMapMode;
      if (this != NULL && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
         nMapMode != MM_TEXT)
      {
         // when using a constrained ::collection::map mode, ::collection::map against physical inch
         ((::ca::graphics *)this)->SetMapMode(MM_HIMETRIC);
         LPtoDP(lpSize);
         ((::ca::graphics *)this)->SetMapMode(nMapMode);
      }
      else
      {
         // ::collection::map against logical inch for non-constrained mapping modes
         int cxPerInch, cyPerInch;
         if (this != NULL)
         {
            ASSERT_VALID(this);
            ASSERT(get_handle1() != NULL);  // no HDC attached or created?
            cxPerInch = GetDeviceCaps(LOGPIXELSX);
            cyPerInch = GetDeviceCaps(LOGPIXELSY);
         }
         else
         {
            cxPerInch = afxData.cxPixelsPerInch;
            cyPerInch = afxData.cyPixelsPerInch;
         }
         ASSERT(cxPerInch != 0 && cyPerInch != 0);
         lpSize->cx = MulDiv(lpSize->cx, cxPerInch, HIMETRIC_INCH);
         lpSize->cy = MulDiv(lpSize->cy, cyPerInch, HIMETRIC_INCH);
      }
   }

   void graphics::LPtoHIMETRIC(LPSIZE lpSize) const
   {
      ASSERT(fx_is_valid_address(lpSize, sizeof(SIZE)));

      LPtoDP(lpSize);
      DPtoHIMETRIC(lpSize);
   }

   void graphics::HIMETRICtoLP(LPSIZE lpSize) const
   {
      ASSERT(fx_is_valid_address(lpSize, sizeof(SIZE)));

      HIMETRICtoDP(lpSize);
      DPtoLP(lpSize);
   }

   /////////////////////////////////////////////////////////////////////////////
   // special graphics drawing primitives/helpers

   ::ca::brush* PASCAL graphics::GetHalftoneBrush(::ca::application * papp)
   {
      AfxLockGlobals(CRIT_HALFTONEBRUSH);
      if (_afxHalftoneBrush == NULL)
      {
         WORD grayPattern[8];
         for (int i = 0; i < 8; i++)
            grayPattern[i] = (WORD)(0x5555 << (i & 1));
         HBITMAP grayBitmap = CreateBitmap(8, 8, 1, 1, grayPattern);
         if (grayBitmap != NULL)
         {
            _afxHalftoneBrush = ::CreatePatternBrush(grayBitmap);
            ::DeleteObject(grayBitmap);
         }
      }
      if (!_afxWingdixTerm)
         _afxWingdixTerm = (char)!atexit(&AfxWingdixTerm);
      AfxUnlockGlobals(CRIT_HALFTONEBRUSH);

//      return ::win::brush::from_handle(papp, _afxHalftoneBrush);
      return NULL;
   }

   void graphics::DrawDragRect(LPCRECT lpRect, SIZE size,
      LPCRECT lpRectLast, SIZE sizeLast, ::ca::brush* pBrush, ::ca::brush* pBrushLast)
   {
      ASSERT(fx_is_valid_address(lpRect, sizeof(RECT), FALSE));
      ASSERT(lpRectLast == NULL ||
         fx_is_valid_address(lpRectLast, sizeof(RECT), FALSE));

      // first, determine the update region and select it
      ::ca::region rgnNew;
      ::ca::region rgnOutside, rgnInside;
      rgnOutside.CreateRectRgnIndirect(lpRect);
      rect rect = *lpRect;
      rect.inflate(-size.cx, -size.cy);
      rect.intersect(rect, lpRect);
      rgnInside.CreateRectRgnIndirect(rect);
      rgnNew.CreateRectRgn(0, 0, 0, 0);
      rgnNew.CombineRgn(&rgnOutside, &rgnInside, RGN_XOR);

      ::ca::brush* pBrushOld = NULL;
      if (pBrush == NULL)
      {
         pBrush = graphics::GetHalftoneBrush(get_app());
      }

      ENSURE(pBrush);

      if (pBrushLast == NULL)
      {
         pBrushLast = pBrush;
      }

      ::ca::region rgnLast, rgnUpdate;
      if (lpRectLast != NULL)
      {
         // find difference between new region and old region
         rgnLast.CreateRectRgn(0, 0, 0, 0);
         rgnOutside.SetRectRgn(lpRectLast);
         rect = *lpRectLast;
         rect.inflate(-sizeLast.cx, -sizeLast.cy);
         rect.intersect(rect, lpRectLast);
         rgnInside.SetRectRgn(rect);
         rgnLast.CombineRgn(&rgnOutside, &rgnInside, RGN_XOR);

         // only diff them if brushes are the same
         if (pBrush->get_os_data() == pBrushLast->get_os_data())
         {
            rgnUpdate.CreateRectRgn(0, 0, 0, 0);
            rgnUpdate.CombineRgn(&rgnLast, &rgnNew, RGN_XOR);
         }
      }
      if (pBrush->get_os_data() != pBrushLast->get_os_data() && lpRectLast != NULL)
      {
         // brushes are different -- erase old region first
         SelectClipRgn(&rgnLast);
         GetClipBox(&rect);
         pBrushOld = SelectObject(pBrushLast);
         PatBlt(rect.left, rect.top, rect.width(), rect.height(), PATINVERT);
         SelectObject(pBrushOld);
         pBrushOld = NULL;
      }

      // draw into the update/new region
      SelectClipRgn(rgnUpdate.get_os_data() != NULL ? &rgnUpdate : &rgnNew);
      GetClipBox(&rect);
      pBrushOld = SelectObject(pBrush);
      PatBlt(rect.left, rect.top, rect.width(), rect.height(), PATINVERT);

      // cleanup DC
      if (pBrushOld != NULL)
         SelectObject(pBrushOld);
      SelectClipRgn(NULL);
   }

   /*void graphics::FillSolidRect(LPCRECT lpRect, COLORREF clr)
   {
      ::SetBkColor(get_handle1(), clr);
      ::ExtTextOut(get_handle1(), 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
   }*/

   void graphics::FillSolidRect(const __rect64 * lpRect, COLORREF clr)
   {
      rect rect32;
      ::copy(rect32, lpRect);
      FillSolidRect(rect32, clr);
   }


   /*
    
   void graphics::FillSolidRect(int x, int y, int cx, int cy, COLORREF clr)
   {
      ::SetBkColor(get_handle1(), clr);
      rect rect(x, y, x + cx, y + cy);
      ::ExtTextOut(get_handle1(), 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
   }

   */

   void graphics::Draw3dRect(LPCRECT lpRect,
      COLORREF clrTopLeft, COLORREF clrBottomRight)
   {
      Draw3dRect(lpRect->left, lpRect->top, lpRect->right - lpRect->left,
         lpRect->bottom - lpRect->top, clrTopLeft, clrBottomRight);
   }

   void graphics::Draw3dRect(int x, int y, int cx, int cy,
      COLORREF clrTopLeft, COLORREF clrBottomRight)
   {
      FillSolidRect(x, y, cx - 1, 1, clrTopLeft);
      FillSolidRect(x, y, 1, cy - 1, clrTopLeft);
      FillSolidRect(x + cx, y, -1, cy, clrBottomRight);
      FillSolidRect(x, y + cy, cx, -1, clrBottomRight);
   }



   /////////////////////////////////////////////////////////////////////////////
   // graphics
   graphics::graphics(::ca::application * papp) :
      ca(papp)
   {
//      set_handle1(NULL);
  //    set_handle2(NULL);
      m_bPrinting = FALSE;
      m_pdibAlphaBlend = NULL;
      m_pgraphics = NULL;
      m_bHdc            = false;
   }

   graphics::graphics()
   {
    //  set_handle1(NULL);
      //set_handle2(NULL);
      m_bPrinting = FALSE;
      m_pgraphics = NULL;
      m_pgraphics = NULL;
      m_bHdc            = false;
   }

#ifdef _DEBUG
   void graphics::assert_valid() const
   {
      ::radix::object::assert_valid();
   }

   void graphics::dump(dump_context & dumpcontext) const
   {
      ::radix::object::dump(dumpcontext);

      dumpcontext << "get_handle1() = " << get_handle1();
      dumpcontext << "\nm_hAttribDC = " << get_handle2();
      dumpcontext << "\nm_bPrinting = " << m_bPrinting;

      dumpcontext << "\n";
   }
#endif //_DEBUG


   //::ca::graphics * PASCAL ::win::graphics::from_handle(HDC hDC)
   //{
      //hdc_map* pMap = afxMapHDC(TRUE); //create ::collection::map if not exist
      //ASSERT(pMap != NULL);
//      ::ca::graphics * pgraphics = (::ca::graphics *)pMap->from_handle(hDC);
  //    ASSERT(pgraphics == NULL || (dynamic_cast<::win::graphics * >(pgraphics))->get_handle1() == hDC);
    //  return pgraphics;
     // return NULL;
   //}

   BOOL graphics::Attach(HDC hdc)
   {

      
      if(m_pgraphics != NULL)
      {

         if(hdc == m_pgraphics->GetHDC())
            return TRUE;

         delete m_pgraphics;

      }
      
      if(hdc != NULL)
      {
         m_pgraphics = new ::Gdiplus::Graphics(hdc);
         m_bHdc = true;
      }

      return m_pgraphics != NULL;

      /*ASSERT(get_handle1() == NULL);      // only attach once, detach on destroy
      ASSERT(get_handle2() == NULL);    // only attach to an is_empty DC

      if (hDC == NULL)
      {
         return FALSE;
      }
      // remember early to avoid leak
      set_handle1(hDC);
      hdc_map* pMap = afxMapHDC(TRUE); // create ::collection::map if not exist
      ASSERT(pMap != NULL);
      pMap->set_permanent(get_handle1(), this);

      SetAttribDC(get_handle1());     // Default to same as output
      return TRUE;*/
   }

   HDC graphics::Detach()
   {

      if(!m_bHdc)
         return NULL;

      if(m_pgraphics == NULL)
         return NULL;

      HDC hdc = m_pgraphics->GetHDC();

      delete m_pgraphics;
      m_pgraphics = NULL;

      return hdc;
      
   }

   BOOL graphics::DeleteDC()
   {

      if(get_handle() == NULL)
         return FALSE;

      return ::DeleteDC(Detach());
   }

   graphics::~graphics()
   {
      
      HDC hdc = Detach();
      
      if(hdc != NULL)
      {
         ::DeleteDC(hdc);
      }

   }


   void graphics::SetAttribDC(HDC hDC)  // Set the Attribute DC
   {
//      set_handle2(hDC);
   }

   void graphics::SetOutputDC(HDC hDC)  // Set the Output DC
   {
#ifdef _DEBUG
/*      hdc_map* pMap = afxMapHDC();
      if (pMap != NULL && pMap->lookup_permanent(get_handle1()) == this)
      {
         TRACE(::radix::trace::category_AppMsg, 0, "Cannot Set Output hDC on Attached graphics.\n");
         ASSERT(FALSE);
      }*/
#endif
  //    set_handle1(hDC);
   }

   void graphics::ReleaseAttribDC()     // Release the Attribute DC
   {
//      set_handle2(NULL);
   }

   void graphics::ReleaseOutputDC()     // Release the Output DC
   {
#ifdef _DEBUG
/*      hdc_map* pMap = afxMapHDC();
      if (pMap != NULL && pMap->lookup_permanent(get_handle1()) == this)
      {
         TRACE(::radix::trace::category_AppMsg, 0, "Cannot Release Output hDC on Attached graphics.\n");
         ASSERT(FALSE);
      }*/
#endif
      //set_handle1(NULL);
   }

   /////////////////////////////////////////////////////////////////////////////
   // Out-of-line routines

   int graphics::StartDoc(const char * lpszDocName)
   {
      DOCINFO di;
      memset(&di, 0, sizeof(DOCINFO));
      di.cbSize = sizeof(DOCINFO);
      di.lpszDocName = lpszDocName;
      return StartDoc(&di);
   }

   int graphics::SaveDC()
   {
      int nRetVal = 0;
      if(get_handle2() != NULL)
         nRetVal = ::SaveDC(get_handle2());
      if(get_handle1() != NULL && get_handle1() != get_handle2() && ::SaveDC(get_handle1()) != 0)
         nRetVal = -1;   // -1 is the only valid restore value for complex DCs
      return nRetVal;
   }

   BOOL graphics::RestoreDC(int nSavedDC)
   {
      BOOL bRetVal = TRUE;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         bRetVal = ::RestoreDC(get_handle1(), nSavedDC);
      if(get_handle2() != NULL)
         bRetVal = (bRetVal && ::RestoreDC(get_handle2(), nSavedDC));
      return bRetVal;
   }

//   ::ca::graphics_object* PASCAL graphics::SelectGdiObject(::ca::application * papp, HDC hDC, HGDIOBJ h)
  // {
//      return ::win::graphics_object::from_handle(papp, ::SelectObject(hDC, h));
   //}

   ::ca::graphics_object* graphics::SelectStockObject(int nIndex)
   {
/*      HGDIOBJ hObject = ::GetStockObject(nIndex);
      HGDIOBJ hOldObj = NULL;
      ASSERT(hObject != NULL);
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         hOldObj = ::SelectObject(get_handle1(), hObject);
      if(get_handle2() != NULL)
         hOldObj = ::SelectObject(get_handle2(), hObject);
      return ::win::graphics_object::from_handle(get_app(), hOldObj);*/

      return NULL;
   }

   ::ca::pen* graphics::SelectObject(::ca::pen* pPen)
   {
      /*HGDIOBJ hOldObj = NULL;
      if(pPen == NULL)
         return NULL;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         hOldObj = ::SelectObject(get_handle1(), pPen->get_os_data());
      if(get_handle2() != NULL)
         hOldObj = ::SelectObject(get_handle2(), pPen->get_os_data());
      return dynamic_cast < pen * > (::win::graphics_object::from_handle(get_app(), hOldObj));*/
      m_penxyz = *pPen;
      return &m_penxyz;
   }

   ::ca::brush* graphics::SelectObject(::ca::brush* pBrush)
   {
/*      HGDIOBJ hOldObj = NULL;
      if(pBrush == NULL)
         return NULL;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         hOldObj = ::SelectObject(get_handle1(), pBrush->get_os_data());
      if(get_handle2() != NULL)
         hOldObj = ::SelectObject(get_handle2(), pBrush->get_os_data());
      return dynamic_cast < ::ca::brush * > (::win::graphics_object::from_handle(get_app(), hOldObj));*/
      m_brushxyz = *pBrush;
      return &m_brushxyz;

   }

   ::ca::font* graphics::SelectObject(::ca::font* pFont)
   {
/*      HGDIOBJ hOldObj = NULL;
      if(pFont == NULL)
         return NULL;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         hOldObj = ::SelectObject(get_handle1(), pFont->get_os_data());
      if(get_handle2() != NULL)
         hOldObj = ::SelectObject(get_handle2(), pFont->get_os_data());
      return dynamic_cast < ::ca::font * > (::win::graphics_object::from_handle(get_app(), hOldObj));*/

      ASSERT(pFont != NULL);

      if(pFont == NULL)
         return NULL;

      m_fontxyz = *pFont;
      return &m_fontxyz;

   }

   int graphics::SelectObject(::ca::region* pRgn)
   {
      int nRetVal = GDI_ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = (int)(INT_PTR)::SelectObject(get_handle1(), pRgn->get_os_data());
      if(get_handle2() != NULL)
         nRetVal = (int)(INT_PTR)::SelectObject(get_handle2(), pRgn->get_os_data());
      return nRetVal;
   }

   ::ca::palette* graphics::SelectPalette(::ca::palette* pPalette, BOOL bForceBackground)
   {
      return NULL;
//      return dynamic_cast < ::ca::palette * > (::win::graphics_object::from_handle(get_app(), ::SelectPalette(get_handle1(), (HPALETTE)pPalette->get_os_data(), bForceBackground)));
   }

   COLORREF graphics::SetBkColor(COLORREF crColor)
   {
      COLORREF crRetVal = CLR_INVALID;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         crRetVal = ::SetBkColor(get_handle1(), crColor);
      if(get_handle2() != NULL)
         crRetVal = ::SetBkColor(get_handle2(), crColor);
      return crRetVal;
   }

   int graphics::SetBkMode(int nBkMode)
   {
      int nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SetBkMode(get_handle1(), nBkMode);
      if(get_handle2() != NULL)
         nRetVal = ::SetBkMode(get_handle2(), nBkMode);
      return nRetVal;
   }

   int graphics::SetPolyFillMode(int nPolyFillMode)
   {
      int nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SetPolyFillMode(get_handle1(), nPolyFillMode);
      if(get_handle2() != NULL)
         nRetVal = ::SetPolyFillMode(get_handle2(), nPolyFillMode);
      return nRetVal;
   }

   int graphics::SetROP2(int nDrawMode)
   {
      int nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SetROP2(get_handle1(), nDrawMode);
      if(get_handle2() != NULL)
         nRetVal = ::SetROP2(get_handle2(), nDrawMode);
      return nRetVal;
   }

   int graphics::SetStretchBltMode(int nStretchMode)
   {
      int nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SetStretchBltMode(get_handle1(), nStretchMode);
      if(get_handle2() != NULL)
         nRetVal = ::SetStretchBltMode(get_handle2(), nStretchMode);
      return nRetVal;
   }

   COLORREF graphics::SetTextColor(COLORREF crColor)
   {
      COLORREF crRetVal = m_crColor;
      m_crColor = crColor;
/*      COLORREF crRetVal = CLR_INVALID;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         crRetVal = ::SetTextColor(get_handle1(), crColor);
      if(get_handle2() != NULL)
         crRetVal = ::SetTextColor(get_handle2(), crColor);*/
      return crRetVal;
   }

   int graphics::SetGraphicsMode(int iMode)
   {
      int nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
      {
         nRetVal = ::SetGraphicsMode(get_handle1(), iMode);
      }
      if(get_handle2() != NULL)
      {
         nRetVal = ::SetGraphicsMode(get_handle2(), iMode);
      }
      return nRetVal;
   }

   BOOL graphics::SetWorldTransform(const XFORM* pXform)
   {
      BOOL nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
      {
         nRetVal = ::SetWorldTransform(get_handle1(), pXform);
      }
      if(get_handle2() != NULL)
      {
         nRetVal = ::SetWorldTransform(get_handle2(), pXform);
      }
      return nRetVal;
   }

   BOOL graphics::ModifyWorldTransform(const XFORM* pXform,DWORD iMode)
   {
      BOOL nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
      {
         nRetVal = ::ModifyWorldTransform(get_handle1(), pXform, iMode);
      }
      if(get_handle2() != NULL)
      {
         nRetVal = ::ModifyWorldTransform(get_handle2(), pXform, iMode);
      }
      return nRetVal;
   }

   int graphics::SetMapMode(int nMapMode)
   {
      int nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SetMapMode(get_handle1(), nMapMode);
      if(get_handle2() != NULL)
         nRetVal = ::SetMapMode(get_handle2(), nMapMode);
      return nRetVal;
   }

   point graphics::SetViewportOrg(int x, int y)
   {
      /*point point(0, 0);
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::SetViewportOrgEx(get_handle1(), x, y, &point);
      if(get_handle2() != NULL)
         ::SetViewportOrgEx(get_handle2(), x, y, &point);*/
      Gdiplus::Matrix m;
      m.Translate(x, y);
      g().SetTransform(&m);
      //return point;
      return point(x, y);
   }

   point graphics::OffsetViewportOrg(int nWidth, int nHeight)
   {
      
      point point = GetViewportOrg();

      point.Offset(nWidth, nHeight);

      return SetViewportOrg(point.x, point.y);

   }

   size graphics::SetViewportExt(int x, int y)
   {
      size size(0, 0);
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::SetViewportExtEx(get_handle1(), x, y, &size);
      if(get_handle2() != NULL)
         ::SetViewportExtEx(get_handle2(), x, y, &size);
      return size;
   }

   size graphics::ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom)
   {
      size size(0, 0);
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::ScaleViewportExtEx(get_handle1(), xNum, xDenom, yNum, yDenom, &size);
      if(get_handle2() != NULL)
         ::ScaleViewportExtEx(get_handle2(), xNum, xDenom, yNum, yDenom, &size);
      return size;
   }

   point graphics::SetWindowOrg(int x, int y)
   {
      point point(0, 0);
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::SetWindowOrgEx(get_handle1(), x, y, &point);
      if(get_handle2() != NULL)
         ::SetWindowOrgEx(get_handle2(), x, y, &point);
      return point;
   }

   point graphics::OffsetWindowOrg(int nWidth, int nHeight)
   {
      point point(0, 0);
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::OffsetWindowOrgEx(get_handle1(), nWidth, nHeight, &point);
      if(get_handle2() != NULL)
         ::OffsetWindowOrgEx(get_handle2(), nWidth, nHeight, &point);
      return point;
   }

   size graphics::SetWindowExt(int x, int y)
   {
      size size(0, 0);
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::SetWindowExtEx(get_handle1(), x, y, &size);
      if(get_handle2() != NULL)
         ::SetWindowExtEx(get_handle2(), x, y, &size);
      return size;
   }

   size graphics::ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom)
   {
      size size(0, 0);
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::ScaleWindowExtEx(get_handle1(), xNum, xDenom, yNum, yDenom, &size);
      if(get_handle2() != NULL)
         ::ScaleWindowExtEx(get_handle2(), xNum, xDenom, yNum, yDenom, &size);
      return size;
   }

   int graphics::GetClipBox(LPRECT lpRect) const
   {
      return ::GetClipBox(get_handle1(), lpRect);
   }

   int graphics::SelectClipRgn(::ca::region* pRgn)
   {
      int nRetVal = ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SelectClipRgn(get_handle1(), pRgn == NULL ? NULL : (HRGN) pRgn->get_os_data());
      if(get_handle2() != NULL)
         nRetVal = ::SelectClipRgn(get_handle2(), pRgn == NULL ? NULL : (HRGN) pRgn->get_os_data());
      return nRetVal;
   }

   int graphics::ExcludeClipRect(int x1, int y1, int x2, int y2)
   {
      int nRetVal = ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::ExcludeClipRect(get_handle1(), x1, y1, x2, y2);
      if(get_handle2() != NULL)
         nRetVal = ::ExcludeClipRect(get_handle2(), x1, y1, x2, y2);
      return nRetVal;
   }

   int graphics::ExcludeClipRect(LPCRECT lpRect)
   {
      int nRetVal = ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::ExcludeClipRect(get_handle1(), lpRect->left, lpRect->top,
         lpRect->right, lpRect->bottom);
      if(get_handle2() != NULL)
         nRetVal = ::ExcludeClipRect(get_handle2(), lpRect->left, lpRect->top,
         lpRect->right, lpRect->bottom);
      return nRetVal;
   }

   int graphics::IntersectClipRect(int x1, int y1, int x2, int y2)
   {
      int nRetVal = ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::IntersectClipRect(get_handle1(), x1, y1, x2, y2);
      if(get_handle2() != NULL)
         nRetVal = ::IntersectClipRect(get_handle2(), x1, y1, x2, y2);
      return nRetVal;
   }

   int graphics::IntersectClipRect(LPCRECT lpRect)
   {
      int nRetVal = ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::IntersectClipRect(get_handle1(), lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
      if(get_handle2() != NULL)
         nRetVal = ::IntersectClipRect(get_handle2(), lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
      return nRetVal;
   }

   int graphics::OffsetClipRgn(int x, int y)
   {
      int nRetVal = ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::OffsetClipRgn(get_handle1(), x, y);
      if(get_handle2() != NULL)
         nRetVal = ::OffsetClipRgn(get_handle2(), x, y);
      return nRetVal;
   }

   int graphics::OffsetClipRgn(SIZE size)
   {
      int nRetVal = ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::OffsetClipRgn(get_handle1(), size.cx, size.cy);
      if(get_handle2() != NULL)
         nRetVal = ::OffsetClipRgn(get_handle2(), size.cx, size.cy);
      return nRetVal;
   }

   /*point graphics::MoveTo(int x, int y)
   {
      point point(0, 0);
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::MoveToEx(get_handle1(), x, y, &point);
      if(get_handle2() != NULL)
         ::MoveToEx(get_handle2(), x, y, &point);
      return point;
   }*/

   UINT graphics::SetTextAlign(UINT nFlags)
   {
      UINT nRetVal = GDI_ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::SetTextAlign(get_handle1(), nFlags);
      if(get_handle2() != NULL)
         nRetVal = ::SetTextAlign(get_handle2(), nFlags);
      return nRetVal;
   }

   int graphics::SetTextJustification(int nBreakExtra, int nBreakCount)
   {
      int nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SetTextJustification(get_handle1(), nBreakExtra, nBreakCount);
      if(get_handle2() != NULL)
         nRetVal = ::SetTextJustification(get_handle2(), nBreakExtra, nBreakCount);
      return nRetVal;
   }

   int graphics::SetTextCharacterExtra(int nCharExtra)
   {
      ASSERT(get_handle1() != NULL);
      int nRetVal = 0x8000000;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SetTextCharacterExtra(get_handle1(), nCharExtra);
      if(get_handle2() != NULL)
         nRetVal = ::SetTextCharacterExtra(get_handle2(), nCharExtra);
      return nRetVal;
   }

   DWORD graphics::SetMapperFlags(DWORD dwFlag)
   {
      ASSERT(get_handle1() != NULL);
      DWORD dwRetVal = GDI_ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         dwRetVal = ::SetMapperFlags(get_handle1(), dwFlag);
      if(get_handle2() != NULL)
         dwRetVal = ::SetMapperFlags(get_handle2(), dwFlag);
      return dwRetVal;
   }

   typedef DWORD (CALLBACK* AFX_GDIGETLAYOUTPROC)(HDC);
   typedef DWORD (CALLBACK* AFX_GDISETLAYOUTPROC)(HDC, DWORD);

   DWORD graphics::GetLayout() const
   {
      HINSTANCE hInst = ::GetModuleHandleA("GDI32.DLL");
      ASSERT(hInst != NULL);
      DWORD dwGetLayout = LAYOUT_LTR;
      AFX_GDIGETLAYOUTPROC pfn;
      pfn = (AFX_GDIGETLAYOUTPROC) GetProcAddress(hInst, "GetLayout");
      // if they API is available, just call it. If it is not
      // available, indicate an error.
      if (pfn != NULL)
         dwGetLayout = (*pfn)(get_handle1());
      else
      {
         dwGetLayout = GDI_ERROR;
         SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
      }
      return dwGetLayout;
   }

   DWORD graphics::SetLayout(DWORD dwSetLayout)
   {
      HINSTANCE hInst = ::GetModuleHandleA("GDI32.DLL");
      ASSERT(hInst != NULL);
      DWORD dwGetLayout = LAYOUT_LTR;
      AFX_GDISETLAYOUTPROC pfn;
      pfn = (AFX_GDISETLAYOUTPROC) GetProcAddress(hInst, "SetLayout");
      // If the API is availalbe, just call it. If it's not available,
      // setting anything other than LAYOUT_LTR is an error.
      if (pfn != NULL)
         dwGetLayout = (*pfn)(get_handle1(), dwSetLayout);
      else if (dwSetLayout != LAYOUT_LTR)
      {
         dwGetLayout = GDI_ERROR;
         SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
      }
      return dwGetLayout;
   }
   /*
   void window::ScreenToClient(LPRECT lpRect)
   {
   ASSERT(::IsWindow(get_handle1()));
   ::ScreenToClient(get_handle1(), (LPPOINT)lpRect);
   ::ScreenToClient(get_handle1(), ((LPPOINT)lpRect)+1);
   if (GetExStyle() & WS_EX_LAYOUTRTL)
   rect::swap_left_right(lpRect);
   }

   void window::ClientToScreen(LPRECT lpRect)
   {
   ASSERT(::IsWindow(get_handle1()));
   ::ClientToScreen(get_handle1(), (LPPOINT)lpRect);
   ::ClientToScreen(get_handle1(), ((LPPOINT)lpRect)+1);
   if (GetExStyle() & WS_EX_LAYOUTRTL)
   rect::swap_left_right(lpRect);
   }*/


   /////////////////////////////////////////////////////////////////////////////
   // Advanced Win32 GDI functions

   BOOL graphics::ArcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
   {
      ASSERT(get_handle1() != NULL);
      BOOL bResult = ::ArcTo(get_handle1(), x1, y1, x2, y2, x3, y3, x4, y4);
      if (get_handle1() != get_handle2())
      {
         point pt;
         VERIFY(::GetCurrentPositionEx(get_handle1(), &pt));
         VERIFY(::MoveToEx(get_handle2(), pt.x, pt.y, NULL));
      }
      return bResult;
   }

   int graphics::SetArcDirection(int nArcDirection)
   {
      ASSERT(get_handle1() != NULL);
      int nResult = 0;
      if (get_handle1() != get_handle2())
         nResult = ::SetArcDirection(get_handle1(), nArcDirection);
      if (get_handle2() != NULL)
         nResult = ::SetArcDirection(get_handle2(), nArcDirection);
      return nResult;
   }

   BOOL graphics::PolyDraw(const POINT* lpPoints, const BYTE* lpTypes, int nCount)
   {
      ASSERT(get_handle1() != NULL);
      BOOL bResult = ::PolyDraw(get_handle1(), lpPoints, lpTypes, nCount);
      if (get_handle1() != get_handle2())
      {
         point pt;
         VERIFY(::GetCurrentPositionEx(get_handle1(), &pt));
         VERIFY(::MoveToEx(get_handle2(), pt.x, pt.y, NULL));
      }
      return bResult;
   }

   BOOL graphics::PolylineTo(const POINT* lpPoints, int nCount)
   {
      ASSERT(get_handle1() != NULL);
      BOOL bResult = ::PolylineTo(get_handle1(), lpPoints, nCount);
      if (get_handle1() != get_handle2())
      {
         point pt;
         VERIFY(::GetCurrentPositionEx(get_handle1(), &pt));
         VERIFY(::MoveToEx(get_handle2(), pt.x, pt.y, NULL));
      }
      return bResult;
   }

   BOOL graphics::SetColorAdjustment(const COLORADJUSTMENT* lpColorAdjust)
   {
      ASSERT(get_handle1() != NULL);
      BOOL bResult = FALSE;
      if (get_handle1() != get_handle2())
         bResult = ::SetColorAdjustment(get_handle1(), lpColorAdjust);
      if (get_handle2() != NULL)
         bResult = ::SetColorAdjustment(get_handle2(), lpColorAdjust);
      return bResult;
   }

   BOOL graphics::PolyBezierTo(const POINT* lpPoints, int nCount)
   {
      ASSERT(get_handle1() != NULL);
      BOOL bResult = ::PolyBezierTo(get_handle1(), lpPoints, nCount);
      if (get_handle1() != get_handle2())
      {
         point pt;
         VERIFY(::GetCurrentPositionEx(get_handle1(), &pt));
         VERIFY(::MoveToEx(get_handle2(), pt.x, pt.y, NULL));
      }
      return bResult;
   }

   BOOL graphics::SelectClipPath(int nMode)
   {
      ASSERT(get_handle1() != NULL);

      // output DC always holds the current path
      if (!::SelectClipPath(get_handle1(), nMode))
         return FALSE;

      // transfer clipping region into the attribute DC
      BOOL bResult = TRUE;
      if (get_handle1() != get_handle2())
      {
         HRGN hRgn = ::CreateRectRgn(0, 0, 0, 0);
         if (::GetClipRgn(get_handle1(), hRgn) < 0 || !::SelectClipRgn(get_handle2(), hRgn))
         {
            TRACE(::radix::trace::category_AppMsg, 0, "Error: unable to transfer clip region in graphics::SelectClipPath!\n");
            bResult = FALSE;
         }
         ::DeleteObject(hRgn);
      }
      return bResult;
   }

   int graphics::SelectClipRgn(::ca::region* pRgn, int nMode)
   {
      ASSERT(get_handle1() != NULL);
      int nRetVal = ERROR;
      if (get_handle1() != get_handle2())
         nRetVal = ::ExtSelectClipRgn(get_handle1(), (HRGN)pRgn->get_os_data(), nMode);
      if (get_handle2() != NULL)
         nRetVal = ::ExtSelectClipRgn(get_handle2(), (HRGN)pRgn->get_os_data(), nMode);
      return nRetVal;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Special handling for metafile playback

   int CALLBACK AfxEnumMetaFileProc(HDC hDC,
      HANDLETABLE* pHandleTable, METARECORD* pMetaRec, int nHandles, LPARAM lParam)
   {
      ::ca::graphics * pgraphics = (::ca::graphics *)lParam;
      ASSERT_VALID(pgraphics);

      switch (pMetaRec->rdFunction)
      {
         // these records have effects different for each graphics derived class
      case META_SETMAPMODE:
         (dynamic_cast<::win::graphics * >(pgraphics))->SetMapMode((int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETWINDOWEXT:
         (dynamic_cast<::win::graphics * >(pgraphics))->SetWindowExt(
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETWINDOWORG:
         (dynamic_cast<::win::graphics * >(pgraphics))->SetWindowOrg(
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETVIEWPORTEXT:
         (dynamic_cast<::win::graphics * >(pgraphics))->SetViewportExt(
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETVIEWPORTORG:
         (dynamic_cast<::win::graphics * >(pgraphics))->SetViewportOrg(
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SCALEWINDOWEXT:
         (dynamic_cast<::win::graphics * >(pgraphics))->ScaleWindowExt(
            (int)(short)pMetaRec->rdParm[3], (int)(short)pMetaRec->rdParm[2],
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SCALEVIEWPORTEXT:
         (dynamic_cast<::win::graphics * >(pgraphics))->ScaleViewportExt(
            (int)(short)pMetaRec->rdParm[3], (int)(short)pMetaRec->rdParm[2],
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_OFFSETVIEWPORTORG:
         (dynamic_cast<::win::graphics * >(pgraphics))->OffsetViewportOrg(
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SAVEDC:
         (dynamic_cast<::win::graphics * >(pgraphics))->SaveDC();
         break;
      case META_RESTOREDC:
         (dynamic_cast<::win::graphics * >(pgraphics))->RestoreDC((int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETBKCOLOR:
         (dynamic_cast<::win::graphics * >(pgraphics))->SetBkColor(*(UNALIGNED COLORREF*)&pMetaRec->rdParm[0]);
         break;
      case META_SETTEXTCOLOR:
         (dynamic_cast<::win::graphics * >(pgraphics))->SetTextColor(*(UNALIGNED COLORREF*)&pMetaRec->rdParm[0]);
         break;

         // need to watch out for SelectObject(HFONT), for custom font mapping
      case META_SELECTOBJECT:
         {
            HGDIOBJ hObject = pHandleTable->objectHandle[pMetaRec->rdParm[0]];
            UINT nObjType = GetObjectType(hObject);
            if (nObjType == 0)
            {
               // object type is unknown, determine if it is a font
               HFONT hStockFont = (HFONT)::GetStockObject(SYSTEM_FONT);
               HFONT hFontOld = (HFONT)::SelectObject((dynamic_cast<::win::graphics * >(pgraphics))->get_handle1(), hStockFont);
               HGDIOBJ hObjOld = ::SelectObject((dynamic_cast<::win::graphics * >(pgraphics))->get_handle1(), hObject);
               if (hObjOld == hStockFont)
               {
                  // got the stock object back, so must be selecting a font
                  throw not_implemented_exception();
//                  (dynamic_cast<::win::graphics * >(pgraphics))->SelectObject(::win::font::from_handle(pgraphics->get_app(), (HFONT)hObject));
                  break;  // don't play the default record
               }
               else
               {
                  // didn't get the stock object back, so restore everything
                  ::SelectObject((dynamic_cast<::win::graphics * >(pgraphics))->get_handle1(), hFontOld);
                  ::SelectObject((dynamic_cast<::win::graphics * >(pgraphics))->get_handle1(), hObjOld);
               }
               // and fall through to PlayMetaFileRecord...
            }
            else if (nObjType == OBJ_FONT)
            {
               // play back as graphics::SelectObject(::ca::font*)
//               (dynamic_cast<::win::graphics * >(pgraphics))->SelectObject(::win::font::from_handle(pgraphics->get_app(), (HFONT)hObject));
               throw not_implemented_exception();
               break;  // don't play the default record
            }
         }
         // fall through...

      default:
         ::PlayMetaFileRecord(hDC, pHandleTable, pMetaRec, nHandles);
         break;
      }

      return 1;
   }

   BOOL graphics::PlayMetaFile(HMETAFILE hMF)
   {
      if (::GetDeviceCaps(get_handle1(), TECHNOLOGY) == DT_METAFILE)
      {
         // playing metafile in metafile, just use core windows API
         return ::PlayMetaFile(get_handle1(), hMF);
      }

      // for special playback, lParam == pgraphics
      return ::EnumMetaFile(get_handle1(), hMF, AfxEnumMetaFileProc, (LPARAM)this);
   }

   /////////////////////////////////////////////////////////////////////////////
   // Coordinate transforms

   void graphics::LPtoDP(LPSIZE lpSize) const
   {
      ASSERT(fx_is_valid_address(lpSize, sizeof(SIZE)));

      size sizeWinExt = GetWindowExt();
      size sizeVpExt = GetViewportExt();
      lpSize->cx = MulDiv(lpSize->cx, abs(sizeVpExt.cx), abs(sizeWinExt.cx));
      lpSize->cy = MulDiv(lpSize->cy, abs(sizeVpExt.cy), abs(sizeWinExt.cy));
   }

   void graphics::DPtoLP(LPSIZE lpSize) const
   {
      ASSERT(fx_is_valid_address(lpSize, sizeof(SIZE)));

      size sizeWinExt = GetWindowExt();
      size sizeVpExt = GetViewportExt();
      lpSize->cx = MulDiv(lpSize->cx, abs(sizeWinExt.cx), abs(sizeVpExt.cx));
      lpSize->cy = MulDiv(lpSize->cy, abs(sizeWinExt.cy), abs(sizeVpExt.cy));
   }



   int graphics::_AFX_FUNCNAME(DrawText)(const char * lpszString, int nCount, LPRECT lpRect,
      UINT nFormat)
   { 
      if(get_handle1() == NULL)
         return -1;
      // these flags would modify the string
      ASSERT((nFormat & (DT_END_ELLIPSIS | DT_MODIFYSTRING)) != (DT_END_ELLIPSIS | DT_MODIFYSTRING));
      ASSERT((nFormat & (DT_PATH_ELLIPSIS | DT_MODIFYSTRING)) != (DT_PATH_ELLIPSIS | DT_MODIFYSTRING));
      wstring wstr = gen::international::utf8_to_unicode(string(lpszString, nCount));
      return ::DrawTextW(get_handle1(), wstr, wcslen(wstr), lpRect, nFormat); 
   }

   int graphics::DrawText(const string & str, LPRECT lpRect, UINT nFormat)
   { 
      /*if(get_handle1() == NULL)
         return -1;
      // these flags would modify the string
      ASSERT((nFormat & (DT_END_ELLIPSIS | DT_MODIFYSTRING)) != (DT_END_ELLIPSIS | DT_MODIFYSTRING));
      ASSERT((nFormat & (DT_PATH_ELLIPSIS | DT_MODIFYSTRING)) != (DT_PATH_ELLIPSIS | DT_MODIFYSTRING));
      wstring wstr = gen::international::utf8_to_unicode(str);
      return ::DrawTextW(get_handle1(), (const wchar_t *)wstr, (int)wcslen(wstr), lpRect, nFormat); */

      Gdiplus::RectF rectf(lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top);


      Gdiplus::StringFormat format;


      m_pgraphics->DrawString(gen::international::utf8_to_unicode(str),
         -1, gdiplus_font(), rectf, &format, gdiplus_brush());


      return 1;

   }

   int graphics::DrawTextEx(LPTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat, LPDRAWTEXTPARAMS lpDTParams)
   { 
      if(get_handle1() == NULL)
         return -1;
      // these flags would modify the string
      ASSERT((nFormat & (DT_END_ELLIPSIS | DT_MODIFYSTRING)) != (DT_END_ELLIPSIS | DT_MODIFYSTRING));
      ASSERT((nFormat & (DT_PATH_ELLIPSIS | DT_MODIFYSTRING)) != (DT_PATH_ELLIPSIS | DT_MODIFYSTRING));
      wstring wstr = gen::international::utf8_to_unicode(string(lpszString, nCount));
      return ::DrawTextExW(get_handle1(), const_cast<wchar_t *>((const wchar_t *)wstr), (int)wcslen(wstr), lpRect, nFormat, lpDTParams); 
   }

   int graphics::DrawTextEx(const string & str, LPRECT lpRect, UINT nFormat, LPDRAWTEXTPARAMS lpDTParams)
   { 
      ASSERT(get_handle1() != NULL);
      // these flags would modify the string
      ASSERT((nFormat & (DT_END_ELLIPSIS | DT_MODIFYSTRING)) != (DT_END_ELLIPSIS | DT_MODIFYSTRING));
      ASSERT((nFormat & (DT_PATH_ELLIPSIS | DT_MODIFYSTRING)) != (DT_PATH_ELLIPSIS | DT_MODIFYSTRING));
      wstring wstr = gen::international::utf8_to_unicode(str);
      return ::DrawTextExW(get_handle1(), const_cast<wchar_t *>((const wchar_t *)wstr), (int)wcslen(wstr), lpRect, nFormat, lpDTParams); 
   }

   size graphics::GetTextExtent(const char * lpszString, int nCount) const
   {

      wstring wstr = gen::international::utf8_to_unicode(lpszString, nCount);

      Gdiplus::RectF box;

      Gdiplus::PointF origin(0, 0);

      m_pgraphics->MeasureString(wstr, wstr.get_length(), ((graphics *)this)->gdiplus_font(), origin, &box);

      return size(box.Width, box.Height);

      /*if(get_handle2() == NULL)
         return size(0, 0);
      SIZE size;
      string str(lpszString, nCount);
      wstring wstr = gen::international::utf8_to_unicode(str);
      if(!::GetTextExtentPoint32W(get_handle2(), wstr, (int)wstr.get_length(), &size))
      {
         return class size(0, 0);
      }
      return size;*/
   }

   size graphics::GetTextExtent(const string & str) const
   {
/*      if(get_handle2() == NULL)
         return size(0, 0);
      SIZE size;
      wstring wstr = gen::international::utf8_to_unicode(str);
      if(!::GetTextExtentPoint32W(get_handle2(), wstr, (int)wstr.get_length(), &size))
      {
         return class size(0, 0);
      }
      return size;*/

      wstring wstr = gen::international::utf8_to_unicode(str);

      Gdiplus::RectF box;

      Gdiplus::PointF origin(0, 0);

      m_pgraphics->MeasureString(wstr, wstr.get_length(), ((graphics *)this)->gdiplus_font(), origin, &box);

      return size(box.Width, box.Height);

   }

   size graphics::GetOutputTextExtent(const char * lpszString, int nCount) const
   {
      ASSERT(get_handle1() != NULL);
      SIZE size;
      string str(lpszString, nCount);
      wstring wstr = gen::international::utf8_to_unicode(str);
      VERIFY(::GetTextExtentPoint32W(get_handle1(), wstr, (int)wstr.get_length(), &size));
      return size;
   }

   size graphics::GetOutputTextExtent(const string & str) const
   {
      ASSERT(get_handle1() != NULL);
      SIZE size;
      wstring wstr = gen::international::utf8_to_unicode(str);
      VERIFY(::GetTextExtentPoint32W(get_handle1(), wstr, (int)wstr.get_length(), &size));
      return size;
   }

/*

   /////////////////////////////////////////////////////////////////////////////
   // Helper DCs

#ifdef _DEBUG
   void CClientDC::assert_valid() const
   {
      graphics::assert_valid();
      ASSERT(m_hWnd == NULL || ::IsWindow(m_hWnd));
   }

   void CClientDC::dump(dump_context & dumpcontext) const
   {
      graphics::dump(dumpcontext);

      dumpcontext << "get_handle1() = " << m_hWnd;
      dumpcontext << "\n";
   }
#endif

   CClientDC::CClientDC(window * pWnd)
   {
      ASSERT(pWnd == NULL || ::IsWindow(WIN_WINDOW(pWnd)->get_handle1()));

      if (!Attach(::GetDC(m_hWnd = WIN_WINDOW(pWnd)->GetSafeHwnd())))
         AfxThrowResourceException();
   }

   CClientDC::~CClientDC()
   {
      ASSERT(get_handle1() != NULL);
      ::ReleaseDC(m_hWnd, Detach());
   }

#ifdef _DEBUG
   void CWindowDC::assert_valid() const
   {
      graphics::assert_valid();
      ASSERT(m_hWnd == NULL || ::IsWindow(m_hWnd));
   }

   void CWindowDC::dump(dump_context & dumpcontext) const
   {
      graphics::dump(dumpcontext);

      dumpcontext << "get_handle1() = " << m_hWnd;
      dumpcontext << "\n";
   }
#endif

   CWindowDC::CWindowDC(window * pWnd)
   {
      ASSERT(pWnd == NULL || ::IsWindow(WIN_WINDOW(pWnd)->get_handle1()));

      if (!Attach(::GetWindowDC(m_hWnd = WIN_WINDOW(pWnd)->GetSafeHwnd())))
         AfxThrowResourceException();
   }

   CWindowDC::~CWindowDC()
   {
      ASSERT(get_handle1() != NULL);
      ::ReleaseDC(m_hWnd, Detach());
   }

#ifdef _DEBUG
   void CPaintDC::assert_valid() const
   {
      graphics::assert_valid();
      ASSERT(::IsWindow(m_hWnd));
   }

   void CPaintDC::dump(dump_context & dumpcontext) const
   {
      graphics::dump(dumpcontext);

      dumpcontext << "get_handle1() = " << m_hWnd;
      dumpcontext << "\nm_ps.hdc = " << m_ps.hdc;
      dumpcontext << "\nm_ps.fErase = " << m_ps.fErase;
      dumpcontext << "\nm_ps.rcPaint = " << (rect)m_ps.rcPaint;

      dumpcontext << "\n";
   }
#endif

   CPaintDC::CPaintDC(window * pWnd)
   {
      ASSERT_VALID(pWnd);
      ASSERT(::IsWindow(WIN_WINDOW(pWnd)->get_handle1()));

      if (!Attach(::BeginPaint(m_hWnd = WIN_WINDOW(pWnd)->get_handle1(), &m_ps)))
         AfxThrowResourceException();
   }

   CPaintDC::~CPaintDC()
   {
      ASSERT(get_handle1() != NULL);
      ASSERT(::IsWindow(m_hWnd));

      ::EndPaint(m_hWnd, &m_ps);
      Detach();
   }

*/







   // IMPLEMENT_DYNAMIC(resource_exception, base_exception)
   //resource_exception _simpleResourceException(FALSE, AFX_IDS_RESOURCE_EXCEPTION);

   // IMPLEMENT_DYNAMIC(user_exception, base_exception)
   //user_exception _simpleUserException(FALSE, AFX_IDS_USER_EXCEPTION);

   // IMPLEMENT_DYNCREATE(graphics, ::radix::object)
   // IMPLEMENT_DYNAMIC(CClientDC, graphics)
   // IMPLEMENT_DYNAMIC(CWindowDC, graphics)
   // IMPLEMENT_DYNAMIC(CPaintDC, graphics)
   // IMPLEMENT_DYNCREATE(::ca::graphics_object, ::radix::object)

   // IMPLEMENT_DYNAMIC(pen, ::ca::graphics_object)
   // IMPLEMENT_DYNAMIC(::ca::brush, ::ca::graphics_object)
   // IMPLEMENT_DYNAMIC(::ca::font, ::ca::graphics_object)
   // IMPLEMENT_DYNAMIC(::ca::bitmap, ::ca::graphics_object)
   // IMPLEMENT_DYNAMIC(::ca::palette, ::ca::graphics_object)
   // IMPLEMENT_DYNAMIC(::ca::region, ::ca::graphics_object)

   /////////////////////////////////////////////////////////////////////////////
   // Standard exception processing


   // resource failure
   void CLASS_DECL_VMSWIN AfxThrowResourceException()
   {
      throw resource_exception();
   }

   // ::fontopus::user alert
   void CLASS_DECL_VMSWIN AfxThrowUserException()
   {
      throw user_exception();
   }

   /////////////////////////////////////////////////////////////////////////////
} // namespace win



/*hdc_map* PASCAL afxMapHDC(BOOL bCreate)
{
   UNREFERENCED_PARAMETER(bCreate);
   try
   {
      AFX_MODULE_STATE* pState = AfxGetModuleState();
      if(pState == NULL)
         return NULL;
      return pState->m_pmapHDC;
   }
   catch(...)
   {
      return NULL;
   }

}*/



namespace win
{

   void graphics::FillSolidRect(LPCRECT lpRect, COLORREF clr)
   {

      //g.SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
      //g().SetCompositingMode(Gdiplus::CompositingModeSourceOver);
      //g().SetCompositingQuality(Gdiplus::CompositingQualityGammaCorrected);
      
      set_color(clr);

      m_pgraphics->FillRectangle(gdiplus_brush(), lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top);

      //::SetBkColor(get_handle1(), clr);
      //::ExtTextOut(get_handle1(), 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
   }

   void graphics::FillSolidRect(int x, int y, int cx, int cy, COLORREF clr)
   {
      //g.SetCompositingMode(Gdiplus::CompositingModeSourceOver);
      //g().SetCompositingMode(Gdiplus::CompositingModeSourceOver);
      //g().SetCompositingQuality(Gdiplus::CompositingQualityGammaCorrected);

      set_color(clr);

      m_pgraphics->FillRectangle(gdiplus_brush(), x, y, cx, cy);

   }


   BOOL graphics::TextOut(int x, int y, const char * lpszString, int nCount)
   {

      ::Gdiplus::PointF origin(x, y);

      string str(lpszString, nCount);
      
      wstring wstr = gen::international::utf8_to_unicode(str);
      
      //
      //m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
      m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintSingleBitPerPixelGridFit);
   
      return m_pgraphics->DrawString(
         wstr, 
         wstr.get_length(), 
         gdiplus_font(), 
         origin, 
         gdiplus_brush()) == Gdiplus::Status::Ok;

   }




   BOOL graphics::LineTo(double x, double y)
   {

//      ::Gdiplus::Pen pen(::Gdiplus::Color(GetAValue(m_crColor), GetRValue(m_crColor), GetGValue(m_crColor), GetBValue(m_crColor)), m_dPenWidth);

      m_pgraphics->DrawLine(gdiplus_pen(), Gdiplus::Point(m_x, m_y), Gdiplus::Point(x, y));


      m_x = x;
      m_y = y;

      return TRUE;

   }


   void graphics::set_alpha_mode(::ca::e_alpha_mode ealphamode)
   {
      
      ::ca::graphics::set_alpha_mode(ealphamode);
      if(m_ealphamode == ::ca::alpha_mode_blend)
      {
         m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
      }
      else if(m_ealphamode == ::ca::alpha_mode_set)
      {
         m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
      }

   }

   void * graphics::get_os_data() const
   {
      return m_pgraphics;
   }

   HDC graphics::get_handle() const
   {
      if(m_pgraphics == NULL || !m_bHdc)
         return NULL;
      return m_pgraphics->GetHDC();
   }

   HDC graphics::get_handle1() const
   {
      return get_handle();
   }

   HDC graphics::get_handle2() const
   {
      return get_handle();
   }

   void graphics::attach(void * pdata)
   {
      if(m_pgraphics != NULL)
      {
         delete m_pgraphics;
      }
      m_pgraphics = (Gdiplus::Graphics *) pdata;
   }


   Gdiplus::Font * graphics::gdiplus_font()
   {
      if(m_spfont.is_null())
      {
         m_spfont.create(get_app());
         m_spfont->operator=(m_fontxyz);
      }
      else if(!m_fontxyz.m_bUpdated)
      {
         m_fontxyz.m_bUpdated = true;
         m_spfont->operator=(m_fontxyz);
      }
      return (Gdiplus::Font *) m_spfont->get_os_data();      
   }

   Gdiplus::Brush * graphics::gdiplus_brush()
   {
      if(m_spbrush.is_null())
      {
         m_spbrush.create(get_app());
         m_spbrush->operator=(m_brushxyz);
      }
      else if(!m_brushxyz.m_bUpdated)
      {
         m_brushxyz.m_bUpdated = true;
         m_spbrush->operator=(m_brushxyz);
      }
      return (Gdiplus::Brush *) m_spbrush->get_os_data();      
   }

   Gdiplus::Pen * graphics::gdiplus_pen()
   {
      if(m_sppen.is_null())
      {
         m_sppen.create(get_app());
         m_sppen->operator=(m_penxyz);
      }
      else if(!m_penxyz.m_bUpdated)
      {
         m_penxyz.m_bUpdated = true;
         m_sppen->operator=(m_penxyz);
      }
      return (Gdiplus::Pen *) m_sppen->get_os_data();      
   }

   Gdiplus::FillMode graphics::gdiplus_get_fill_mode()
   {
      return Gdiplus::FillModeWinding;
   }

   bool graphics::blur(bool bExpand, double dRadius, LPCRECT lpcrect)
   {

      if(m_bitmap.is_null() || m_bitmap->get_os_data() == NULL)
         return false;

      Gdiplus::BlurParams myBlurParams;
      myBlurParams.expandEdge = bExpand ? 1 : 0;
      myBlurParams.radius = dRadius;

      Gdiplus::Blur myBlur;
      myBlur.SetParameters(&myBlurParams);

      

      Gdiplus::Matrix m;
      m_pgraphics->GetTransform(&m);

      Gdiplus::PointF points[2];

      points[0].X    = lpcrect->left;
      points[0].Y    = lpcrect->top;
      points[1].X    = lpcrect->right;
      points[1].Y    = lpcrect->bottom;

      m.TransformPoints(points, 2);

      //Gdiplus::RectF rectf(points[0].X, points[0].Y, points[1].X - points[0].X, points[1].Y - points[0].Y);

      RECT rect;

      rect.left      = points[0].X;
      rect.top       = points[0].Y;
      rect.right     = points[1].X;
      rect.bottom    = points[1].Y;

      Gdiplus::Bitmap * pbitmap = ((Gdiplus::Bitmap *) m_bitmap->get_os_data());

      pbitmap->ApplyEffect(&myBlur, &rect);

      return true;

   }

} // namespace win