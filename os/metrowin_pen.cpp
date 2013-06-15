#include "framework.h"

HRESULT RenderPatternToCommandList(ID2D1RenderTarget * pdc, D2D1_COLOR_F *pcr)
{

   HRESULT hr;

   pdc->BeginDraw();

   pdc->Clear(pcr);

   ID2D1SolidColorBrush * pbr = NULL;

   //hr = pdc->CreateSolidColorBrush(*pcr, &pbr);

   //pdc->DrawRectangle(D2D1::RectF(0.f, 0.f, 256.f, 256.f), pbr, 0.f);

   //pbr->Release();

   hr = pdc->EndDraw();

   return hr;

}

HRESULT
CreatePatternBrush(
     __in ID2D1DeviceContext *pDeviceContext,
     D2D1_COLOR_F * pcr,
     __deref_out ID2D1ImageBrush **ppImageBrush
     )
{

   HRESULT hrEndDraw = pDeviceContext->EndDraw();

    HRESULT hr = S_OK;
    ID2D1Image *pOldTarget = NULL;
    pDeviceContext->GetTarget(&pOldTarget);

    ID2D1CommandList *pCommandList = NULL;
    hr = pDeviceContext->CreateCommandList(&pCommandList);
     
    if (SUCCEEDED(hr))
    {   
        pDeviceContext->SetTarget(pCommandList);
        hr = RenderPatternToCommandList(pDeviceContext, pcr);
    }

    pDeviceContext->SetTarget(pOldTarget);

    ID2D1ImageBrush *pImageBrush = NULL;

    if(SUCCEEDED(hr))
    {
       hr = pCommandList->Close();
    }

    if (SUCCEEDED(hr))
    {        

        D2D1_IMAGE_BRUSH_PROPERTIES props;

        props.sourceRectangle.left = 0.f;
        props.sourceRectangle.top = 0.f;
        props.sourceRectangle.right = 256.f;
        props.sourceRectangle.bottom = 256.f;

        props.extendModeX = D2D1_EXTEND_MODE_WRAP;
        props.extendModeY = D2D1_EXTEND_MODE_WRAP;

        props.interpolationMode = D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;

        hr = pDeviceContext->CreateImageBrush(
                 pCommandList, &props, NULL, &pImageBrush);
    }
    
    // Fill a rectangle with the image brush.
    /*if (SUCCEEDED(hr))
    {
        pDeviceContext->FillRectangle(
            D2D1::RectF(0, 0, 100, 100), pImageBrush);
    }*/

    //pImageBrush->Release();
    pCommandList->Release();
    //pOldTarget->Release();

    if(hrEndDraw == S_OK)
    {
       pDeviceContext->BeginDraw();
    }

    if(SUCCEEDED(hr))
    {

       *ppImageBrush = pImageBrush;

    }
    else
    {

       *ppImageBrush = NULL;
    }

    return hr;
}

namespace metrowin
{


   pen::pen(::ca2::application * papp) :
      ca2(papp)
   { 

      m_pimagebrush     = NULL;
      m_bMetroColor     = false;
      m_crMetro         = 0;
   }

   pen::~pen()
   { 
   }

   /*bool pen::CreatePen(int nPenStyle, int nWidth, COLORREF crColor)
   { return Attach(::CreatePen(nPenStyle, nWidth, crColor)); }
   bool pen::CreatePenIndirect(LPLOGPEN lpLogPen)
   { return Attach(::CreatePenIndirect(lpLogPen)); }
   bool pen::CreatePen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,
   int nStyleCount, const DWORD* lpStyle)
   { return Attach(::ExtCreatePen(nPenStyle, nWidth, pLogBrush, nStyleCount,
   lpStyle)); }
   int pen::GetExtLogPen(EXTLOGPEN* pLogPen)
   { ASSERT(get_handle() != NULL);
   return ::GetObject(get_handle(), sizeof(EXTLOGPEN), pLogPen); }

   int pen::GetLogPen(LOGPEN* pLogPen)
   { 
   if(get_handle() == NULL)
   return 0;
   return ::GetObject(get_handle(), sizeof(LOGPEN), pLogPen); 
   }


   void pen::construct(int nPenStyle, double dWidth, COLORREF crColor)
   {

   if(m_ppen == NULL)
   delete m_ppen;

   m_nPenStyle    = nPenStyle;
   m_dWidth       = dWidth;
   m_crColor      = crColor;
   m_ppen         = new Gdiplus::Pen(Gdiplus::Color(crColor), dWidth);

   }

   void pen::construct(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,
   int nStyleCount, const DWORD* lpStyle)
   {
   if (!Attach(::ExtCreatePen(nPenStyle, nWidth, pLogBrush, nStyleCount,
   lpStyle)))
   throw resource_exception();
   }*/

   /////////////////////////////////////////////////////////////////////////////


   void pen::dump(dump_context & dumpcontext) const
   {
      ::ca2::graphics_object::dump(dumpcontext);

      /*      if (get_handle() == NULL)
      return;

      if (!afxData.bWin95 && ::GetObjectType(get_handle()) != OBJ_PEN)
      {
      // not a valid object
      dumpcontext << "has ILLEGAL HPEN!";
      return;
      }

      LOGPEN lp;
      VERIFY(GetObject(sizeof(lp), &lp));
      dumpcontext << "lgpn.lopnStyle = " << lp.lopnStyle;
      dumpcontext << "\nlgpn.lopnWidth.x (width) = " << lp.lopnWidth.x;
      dumpcontext << "\nlgpn.lopnColor = " << (void *)(dword_ptr)lp.lopnColor;

      dumpcontext << "\n";*/
   }





   // IMPLEMENT_DYNAMIC(resource_exception, base_exception)
   //resource_exception _simpleResourceException(FALSE, __IDS_RESOURCE_EXCEPTION);

   // IMPLEMENT_DYNAMIC(user_exception, base_exception)
   //user_exception _simpleUserException(FALSE, __IDS_USER_EXCEPTION);

   // IMPLEMENT_DYNCREATE(::ca2::graphics_sp, ::ca2::object)
   // IMPLEMENT_DYNAMIC(CClientDC, ::ca2::graphics_sp)
   // IMPLEMENT_DYNAMIC(CWindowDC, ::ca2::graphics_sp)
   // IMPLEMENT_DYNAMIC(CPaintDC, ::ca2::graphics_sp)
   // IMPLEMENT_DYNCREATE(::ca2::graphics_object, ::ca2::object)

   // IMPLEMENT_DYNAMIC(pen, ::ca2::graphics_object)
   // IMPLEMENT_DYNAMIC(::ca2::brush, ::ca2::graphics_object)
   // IMPLEMENT_DYNAMIC(::ca2::font, ::ca2::graphics_object)
   // IMPLEMENT_DYNAMIC(::ca2::bitmap, ::ca2::graphics_object)
   // IMPLEMENT_DYNAMIC(::ca2::palette, ::ca2::graphics_object)
   // IMPLEMENT_DYNAMIC(::ca2::region, ::ca2::graphics_object)

   /////////////////////////////////////////////////////////////////////////////
   // Standard exception processing


   /*void __get_gray_bitmap(::ca2::application * papp, const ::ca2::bitmap &rSrc, ::ca2::bitmap *pDest, COLORREF crBackground)
   {
   ASSERT(pDest);
   ASSERT_KINDOF(::ca2::bitmap, pDest);

   BITMAP bm;
   ::ca2::graphics_sp graphicsMem, graphicsMask;
   COLORREF cr;
   ::ca2::bitmap_sp bmpMask(papp);
   ::ca2::bitmap *pOldMask, *pOldMem;
   const DWORD   CP_ROP = 0xE20746;
   ::ca2::brush_sp brHighLight(papp, ::GetSysColor(COLOR_3DHIGHLIGHT)),
   brShadow(papp, ::GetSysColor(COLOR_3DSHADOW)), spbr;

   if(graphicsMem->CreateCompatibleDC(NULL) &&
   graphicsMask->CreateCompatibleDC(NULL))
   {
   const_cast<::ca2::bitmap &>(rSrc).GetBitmap(&bm);
   //         pDest->delete_object();
   if(pDest->CreateBitmap(bm.bmWidth, bm.bmHeight, bm.bmPlanes, bm.bmBitsPixel, NULL) &&
   bmpMask->CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL))
   {
   pOldMem = graphicsMem->SelectObject(const_cast<::ca2::bitmap *>(&rSrc));
   pOldMask = graphicsMask->SelectObject(bmpMask);

   ASSERT(pOldMem && pOldMask);
   if(!pOldMem || !pOldMask)
   return;

   // Make the upper left corner pixel the "transparent" pixel
   cr = graphicsMem->SetBkColor(graphicsMem->GetPixel(0, 0));
   graphicsMask->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsMem, 0, 0, SRCCOPY);
   // Make white pixels transparent too
   graphicsMem->SetBkColor(RGB(255, 255, 255));
   graphicsMask->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsMem, 0, 0, NOTSRCERASE);

   if(graphicsMem->SelectObject(pDest))
   {
   graphicsMem->FillSolidRect(0, 0, bm.bmWidth, bm.bmHeight, crBackground);

   graphicsMem->SetBkColor(RGB(255, 255, 255));

   spbr = graphicsMem->SelectObject(brHighLight);
   graphicsMem->BitBlt(1, 1, bm.bmWidth, bm.bmHeight, graphicsMask, 0, 0, CP_ROP);

   graphicsMem->SelectObject(brShadow);
   graphicsMem->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsMask, 0, 0, CP_ROP);

   graphicsMem->SelectObject(spbr);

   graphicsMem->SetBkColor(cr);
   }
   graphicsMask->SelectObject(pOldMask);
   graphicsMem->SelectObject(pOldMem);
   }
   }
   }

   void __draw_gray_bitmap(::ca2::application * papp, ::ca2::graphics * pgraphics, int x, int y, const ::ca2::bitmap &rSrc, COLORREF crBackground)
   {
   ASSERT(pgraphics);
   ASSERT_KINDOF(::ca2::graphics_sp, pgraphics);

   BITMAP bm;
   ::ca2::graphics_sp graphicsMem, graphicsMask;
   COLORREF cr;
   ::ca2::bitmap_sp bmpMask(papp);
   ::ca2::bitmap *pOldMask, *pOldMem;
   const DWORD   CP_ROP = 0xE20746;
   ::ca2::brush_sp brHighLight(papp, ::GetSysColor(COLOR_3DHIGHLIGHT)),
   brShadow(papp, ::GetSysColor(COLOR_3DSHADOW)), spbr;

   if(graphicsMem->CreateCompatibleDC(pgraphics) &&
   graphicsMask->CreateCompatibleDC(pgraphics) &&
   const_cast<::ca2::bitmap &>(rSrc).GetBitmap(&bm) &&
   bmpMask->CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL))
   {
   pOldMem = graphicsMem->SelectObject(const_cast<::ca2::bitmap *>(&rSrc));
   pOldMask = graphicsMask->SelectObject(bmpMask);

   ASSERT(pOldMem && pOldMask);
   if(!pOldMem || !pOldMask)
   return;

   cr = graphicsMem->SetBkColor(graphicsMem->GetPixel(0, 0));
   graphicsMask->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsMem, 0, 0, SRCCOPY);
   graphicsMem->SetBkColor(RGB(255, 255, 255));
   graphicsMask->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsMem, 0, 0, NOTSRCERASE);

   pgraphics->FillSolidRect(x, y, bm.bmWidth, bm.bmHeight, crBackground);

   pgraphics->SetBkColor(RGB(255, 255, 255));

   spbr = pgraphics->SelectObject(brHighLight);
   pgraphics->BitBlt(x + 1, y + 1, bm.bmWidth, bm.bmHeight, graphicsMask, 0, 0, CP_ROP);

   pgraphics->SelectObject(brShadow);
   pgraphics->BitBlt(x, y, bm.bmWidth, bm.bmHeight, graphicsMask, 0, 0, CP_ROP);

   pgraphics->SelectObject(spbr);

   pgraphics->SetBkColor(cr);
   graphicsMask->SelectObject(pOldMask);
   }
   }

   void __get_dithered_bitmap(::ca2::application * papp, const ::ca2::bitmap &rSrc, ::ca2::bitmap *pDest, COLORREF cr1, COLORREF cr2)
   {
   ASSERT(pDest);
   ASSERT_KINDOF(::ca2::bitmap, pDest);

   BITMAP bm;
   ::ca2::graphics_sp graphicsSrc, graphicsMask, graphicsDest;
   COLORREF cr;
   ::ca2::bitmap_sp bmpMask(papp);
   ::ca2::bitmap *pOldMask, *pOldSrc;
   ::ca2::brush brChecker;
   static const WORD wPat[8] = {0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa};

   if(graphicsSrc->CreateCompatibleDC(NULL) &&
   graphicsMask->CreateCompatibleDC(NULL) &&
   graphicsDest->CreateCompatibleDC(NULL))
   {
   if(const_cast<::ca2::bitmap &>(rSrc).GetBitmap(&bm))
   {
   //pDest->delete_object();
   if(pDest->CreateBitmap(bm.bmWidth, bm.bmHeight, bm.bmPlanes, bm.bmBitsPixel, NULL))
   {
   // create checker brush
   bmpMask->CreateBitmap(8, 8, 1, 1, wPat);
   brChecker.CreatePatternBrush(bmpMask);
   //bmpMask->delete_object();

   // Mask
   bmpMask->CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

   pOldSrc = graphicsSrc->SelectObject(const_cast<::ca2::bitmap *>(&rSrc));
   pOldMask = graphicsMask->SelectObject(bmpMask);

   ASSERT(pOldSrc && pOldMask);
   if(!pOldSrc || !pOldMask)
   return;

   // Make the upper left corner pixel of the source a "transparent" color
   cr = graphicsSrc->SetBkColor(graphicsSrc->GetPixel(0, 0));
   graphicsMask->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsSrc, 0, 0, SRCCOPY);
   // Make white pixels of the source "transparent" too
   graphicsSrc->SetBkColor(RGB(255, 255, 255));
   graphicsMask->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsSrc, 0, 0, SRCPAINT);
   graphicsSrc->SetBkColor(cr);

   // Checker the background with white and crBackground
   pDest = graphicsDest->SelectObject(pDest);
   ASSERT(pDest);
   if(pDest)
   {
   cr1 = graphicsDest->SetTextColor(cr1);
   cr2 = graphicsDest->SetBkColor(cr2);
   graphicsDest->FillRect(rect(0, 0, bm.bmWidth, bm.bmHeight), &brChecker);
   graphicsDest->SetTextColor(cr1);
   graphicsDest->SetBkColor(cr2);

   // Blt it
   graphicsDest->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsSrc, 0, 0, SRCINVERT);
   graphicsDest->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsMask, 0, 0, SRCAND);
   graphicsDest->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsSrc, 0, 0, SRCINVERT);
   }
   graphicsDest->SelectObject(pDest);
   graphicsMask->SelectObject(pOldMask);
   graphicsSrc->SelectObject(pOldSrc);
   }
   }
   }
   }

   void __draw_dithered_bitmap(::ca2::application * papp, ::ca2::graphics * pgraphics, int x, int y, const ::ca2::bitmap &rSrc, COLORREF cr1, COLORREF cr2)
   {
   ASSERT(pgraphics);
   ASSERT_KINDOF(::ca2::graphics_sp, pgraphics);

   BITMAP bm;
   ::ca2::graphics_sp graphicsSrc, graphicsMask;
   COLORREF cr;
   ::ca2::bitmap_sp bmpMask(papp);
   ::ca2::bitmap *pOldMask, *pOldSrc;
   ::ca2::brush brChecker;
   static const WORD wPat[8] = {0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa};

   if(graphicsSrc->CreateCompatibleDC(pgraphics) &&
   graphicsMask->CreateCompatibleDC(pgraphics) &&
   const_cast<::ca2::bitmap &>(rSrc).GetBitmap(&bm))
   {
   // create checker brush
   bmpMask->CreateBitmap(8, 8, 1, 1, wPat);
   brChecker.CreatePatternBrush(bmpMask);
   //bmpMask->delete_object();

   // Mask
   bmpMask->CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

   pOldSrc = graphicsSrc->SelectObject(const_cast<::ca2::bitmap *>(&rSrc));
   pOldMask = graphicsMask->SelectObject(bmpMask);

   ASSERT(pOldSrc && pOldMask);
   if(!pOldSrc || !pOldMask)
   return;

   // Make the upper left corner pixel of the source a "transparent" color
   cr = graphicsSrc->SetBkColor(graphicsSrc->GetPixel(0, 0));
   graphicsMask->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsSrc, 0, 0, SRCCOPY);
   // Make white pixels of the source "transparent" too
   graphicsSrc->SetBkColor(RGB(255, 255, 255));
   graphicsMask->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsSrc, 0, 0, SRCPAINT);
   graphicsSrc->SetBkColor(cr);

   // Checker the background with white and crBackground
   cr1 = pgraphics->SetTextColor(cr1);
   cr2 = pgraphics->SetBkColor(cr2);
   pgraphics->FillRect(rect(x, y, x + bm.bmWidth, y + bm.bmHeight), &brChecker);
   pgraphics->SetTextColor(cr1);
   pgraphics->SetBkColor(cr2);

   // Blt it
   pgraphics->BitBlt(x, y, bm.bmWidth, bm.bmHeight, graphicsSrc, 0, 0, SRCINVERT);
   pgraphics->BitBlt(x, y, bm.bmWidth, bm.bmHeight, graphicsMask, 0, 0, SRCAND);
   pgraphics->BitBlt(x, y, bm.bmWidth, bm.bmHeight, graphicsSrc, 0, 0, SRCINVERT);

   graphicsMask->SelectObject(pOldMask);
   graphicsSrc->SelectObject(pOldSrc);
   }
   }

   */

   bool pen::create_solid(::ca2::graphics * pgraphics, double dWidth, COLORREF cr)
   {



      if(!::ca2::pen::create_solid(pgraphics, dWidth, cr))
         return false;


      D2D1_COLOR_F ca;

      ca.a = GetAValue(cr) / 255.0f;
      ca.r = GetRValue(cr) / 255.0f;
      ca.g = GetGValue(cr) / 255.0f;
      ca.b = GetBValue(cr) / 255.0f;

      //METROWIN_DC(pgraphics)->m_pdc->CreateSolidColorBrush(ca, &m_psolidbrush);

/*      CreatePatternBrush(METROWIN_DC(pgraphics)->m_pdevicecontext, &ca, &m_pimagebrush);

      if(m_pimagebrush != NULL)
      {
         m_crMetro         = m_cr;
         m_bMetroColor     = true;
      }
      */

      return TRUE;


   }

   ID2D1Brush * pen::get_os_pen_brush(::metrowin::graphics * pdc) const
   {

      if((!m_bUpdated && (!m_bMetroColor || m_crMetro != m_cr)) || m_pimagebrush == NULL)
      {

         if(m_pimagebrush != NULL)
         {

            ((pen *)this)->destroy();

         }

         D2D1_COLOR_F ca;

         ca.a = GetAValue(m_cr) / 255.0f;
         ca.r = GetRValue(m_cr) / 255.0f;
         ca.g = GetGValue(m_cr) / 255.0f;
         ca.b = GetBValue(m_cr) / 255.0f;

         CreatePatternBrush(METROWIN_DC(pdc)->m_pdevicecontext, &ca, (ID2D1ImageBrush **) &m_pimagebrush);

         if(m_pimagebrush != NULL)
         {
            ((pen *) this)->m_bUpdated      = true;
            ((pen *) this)->m_crMetro       = m_cr;
            ((pen *) this)->m_bMetroColor   = true;
         }

      }

      return (ID2D1Brush *) m_pimagebrush;

   }


   bool pen::destroy()
   {
      
      if(m_pimagebrush != NULL)
      {
         try
         {
            m_pimagebrush->Release();
         }
         catch(...)
         {
         }
         m_pimagebrush = NULL;
      }

      return true;

   }

} // namespace metrowin



