#include "framework.h"

#undef new

namespace metrowin
{


   brush::brush(::ca::application * papp) :
      ca(papp)
   { 

      m_pbrush = NULL;

   }

   brush::~brush()
   { 
      if(m_pbrush != NULL)
      {
         destroy();
      }
   }

   //bool brush::CreateSolidBrush(COLORREF crColor)
   //{

   //   if(m_pbrush != NULL)
   //   {
   //      delete m_pbrush;
   //   }

   //   m_pbrush = new Gdiplus::SolidBrush(Gdiplus::Color(GetAValue(crColor), GetRValue(crColor), GetGValue(crColor), GetBValue(crColor)));

   //   return m_pbrush != NULL;
   //      
   //}

   // bool brush::CreateHatchBrush(int nIndex, COLORREF crColor)
   // { 
   //    //return Attach(::CreateHatchBrush(nIndex, crColor));
   //    return FALSE;
   // }
   //    bool brush::CreateBrushIndirect(const LOGBRUSH* lpLogBrush)
   //  {
   //return Attach(::CreateBrushIndirect(lpLogBrush)); 
   //   return FALSE;
   //}
   //bool brush::CreatePatternBrush(::ca::bitmap* pBitmap)
   //{ 
   //   //return Attach(::CreatePatternBrush((HBITMAP)pBitmap->get_os_data()));
   //   return FALSE;
   //}
   //bool brush::CreateDIBPatternBrush(const void * lpPackedDIB, UINT nUsage)
   //{ 
   //   //return Attach(::CreateDIBPatternBrushPt(lpPackedDIB, nUsage));
   //   return FALSE;
   //}
   //bool brush::CreateSysColorBrush(int nIndex)
   //{ 
   //   //return Attach(::GetSysColorBrush(nIndex));
   //   return FALSE;

   //}
   /*    int brush::GetLogBrush(LOGBRUSH* pLogBrush)
   { 
   //ASSERT(get_os_data() != NULL);
   //return ::GetObject(get_os_data(), sizeof(LOGBRUSH), pLogBrush); 
   return FALSE;
   }*/


   //   void brush::construct(COLORREF crColor)
   //   {
   ////      if (!Attach(::CreateSolidBrush(crColor)))
   //  //       throw resource_exception();
   //   }
   //
   //   void brush::construct(int nIndex, COLORREF crColor)
   //   {
   //    //  if (!Attach(::CreateHatchBrush(nIndex, crColor)))
   //      //   throw resource_exception();
   //   }
   //
   //   void brush::construct(::ca::bitmap* pBitmap)
   //   {
   //      //ASSERT_VALID(pBitmap);
   //
   //      //if (!Attach(::CreatePatternBrush((HBITMAP)pBitmap->get_os_data())))
   //        // throw resource_exception();
   //   }
   //
   //   bool brush::CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage)
   //   {
   //      //ASSERT(hPackedDIB != NULL);
   //      //const void * lpPackedDIB = ::GlobalLock(hPackedDIB);
   //      //ASSERT(lpPackedDIB != NULL);
   //      //bool bResult = Attach(::CreateDIBPatternBrushPt(lpPackedDIB, nUsage));
   //      //::GlobalUnlock(hPackedDIB);
   //      //return bResult;
   //      return FALSE;
   //   }


   void brush::dump(dump_context & dumpcontext) const
   {
      ::ca::graphics_object::dump(dumpcontext);

      //if (get_os_data() == NULL)
      // return;

      /*      LOGBRUSH lb;
      VERIFY(GetObject(sizeof(lb), &lb));
      dumpcontext << "lb.lbStyle = " << lb.lbStyle;
      dumpcontext << "\nlb.lbHatch = " << lb.lbHatch;
      dumpcontext << "\nlb.lbColor = " << (void *)(dword_ptr)lb.lbColor;

      dumpcontext << "\n";
      */
   }



   ID2D1Brush * brush::get_os_brush(::metrowin::graphics * pdc) const
   {

      if(m_etype == type_solid)
      {

         if(!m_bUpdated || m_psolidbrush == NULL)
         {

            if(m_pbrush != NULL)
            {

               ((brush *)this)->destroy();

            }

            D2D1_COLOR_F c;

            c.a = GetAValue(m_cr) / 255.0f;
            c.r = GetRValue(m_cr) / 255.0f;
            c.g = GetGValue(m_cr) / 255.0f;
            c.b = GetBValue(m_cr) / 255.0f;

            pdc->m_pdc->CreateSolidColorBrush(c, (ID2D1SolidColorBrush **) &m_psolidbrush);

            if(m_psolidbrush != NULL)
            {
               ((font *) this)->m_bUpdated = true;
            }

         }

         return (ID2D1Brush *) m_psolidbrush;

      }
      else if(m_etype == type_linear_gradient_point_color)
      {

         if(!m_bUpdated || m_plineargradientbrush == NULL)
         {

            if(m_pbrush != NULL)
            {

               ((brush *)this)->destroy();

            }

            D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES prop;

            prop.startPoint.x    = (FLOAT) m_pt1.x;
            prop.startPoint.y    = (FLOAT) m_pt1.y;
            prop.endPoint.x      = (FLOAT) m_pt2.x;
            prop.endPoint.y      = (FLOAT) m_pt2.y;

            // Create an array of gradient stops to put in the gradient stop
            // collection that will be used in the gradient brush.
            ID2D1GradientStopCollection * pgradientstops = NULL;

            D2D1_GRADIENT_STOP gradientstops[2];

            gradientstops[0].color.a = GetAValue(m_cr1) / 255.0f;
            gradientstops[0].color.r = GetRValue(m_cr1) / 255.0f;
            gradientstops[0].color.g = GetGValue(m_cr1) / 255.0f;
            gradientstops[0].color.b = GetBValue(m_cr1) / 255.0f;
            gradientstops[0].position = 0.0f;
            gradientstops[1].color.a = GetAValue(m_cr2) / 255.0f;
            gradientstops[1].color.r = GetRValue(m_cr2) / 255.0f;
            gradientstops[1].color.g = GetGValue(m_cr2) / 255.0f;
            gradientstops[1].color.b = GetBValue(m_cr2) / 255.0f;
            gradientstops[1].position = 1.0f;

            // Create the ID2D1GradientStopCollection from a previously
            // declared array of D2D1_GRADIENT_STOP structs.
            HRESULT hr = pdc->m_pdc->CreateGradientStopCollection(gradientstops, 2, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &pgradientstops);

            hr = pdc->m_pdc->CreateLinearGradientBrush(&prop, NULL, pgradientstops, (ID2D1LinearGradientBrush **) &m_plineargradientbrush);

            pgradientstops->Release();

            if(m_plineargradientbrush != NULL)
            {
               ((font *) this)->m_bUpdated = true;
            }


         }


         return (ID2D1Brush *) m_plineargradientbrush;

      }
      else
      {

         return (ID2D1Brush *) m_pbrush;

      }

      return NULL;


   }

   bool brush::destroy()
   {

      if(m_pbrush == NULL && m_psolidbrush == NULL && m_plineargradientbrush == NULL)
      {

         return true;

      }

      if(m_etype == type_solid)
      {

         m_psolidbrush->Release();

      }
      else if(m_etype == type_linear_gradient_point_color)
      {

         m_plineargradientbrush->Release();

      }
      else
      {

         m_pbrush->Release();

      }


      m_pbrush = NULL;

      m_psolidbrush = NULL;

      m_plineargradientbrush = NULL;

      return true;

   }


} // namespace metrowin




