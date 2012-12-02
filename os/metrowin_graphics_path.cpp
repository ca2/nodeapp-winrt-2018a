#include "framework.h"

#define d2d1_fax_options D2D1_FACTORY_OPTIONS // fax of merde
#define single_threaded D2D1_FACTORY_TYPE_SINGLE_THREADED // ???? muliple performance multi thread hidden option there exists cost uses?

namespace metrowin
{


   

   graphics_path::graphics_path(::ca::application * papp) :
      ca(papp)
   {

      m_bFill  = false;

      m_ppath = NULL;

      HRESULT hr = ::GetD2D1Factory1()->CreatePathGeometry(&m_ppath);

      ::metrowin::throw_if_failed(hr);
         

      m_psink  = NULL;


   }

   graphics_path::~graphics_path() 
   {

      if(m_ppath != NULL)
      {

         m_ppath->Release();

         m_ppath = NULL;

      }


   }


   bool graphics_path::begin_figure(bool bFill, ::ca::e_fill_mode efillmode)
   {

      if(m_psink != NULL)
      {
         
         m_psink->Release();

         m_psink = NULL;

      }

      if(m_ppath != NULL)
      {

         m_ppath->Release();

         m_ppath = NULL;

      }


      //m_ppath  = new Gdiplus::GraphicsPath();


      m_bFill        = true;

      m_efillmode    = efillmode;

      return true;


   }


   bool graphics_path::end_figure(bool bClose)
   {

      if(bClose)
      {

         m_psink->EndFigure(D2D1_FIGURE_END_CLOSED);

      }
      else
      {

         m_psink->EndFigure(D2D1_FIGURE_END_OPEN);

      }

      m_bFigureEnded = true;

      m_bFill = true;

      return true;

   }

   bool graphics_path::add_arc(const RECT & rect, int iStart, int iAngle)
   {

      D2D1_POINT_2F pt;

      D2D1_ARC_SEGMENT arcseg;

      get_arc(pt, arcseg, rect, iStart, iAngle);

      if(!prepare(pt))
         return false;

      m_psink->AddArc(arcseg);

      return true;

   }


   bool graphics_path::add_line(int x, int y)
   {

      D2D1_POINT_2F pt;

      pt.x = (FLOAT) x;
      pt.y = (FLOAT) y;

      return prepare(pt);
   }


   bool graphics_path::add_move(int x, int y)
   {

      end_figure(false);

      D2D1_POINT_2F pt;

      pt.x = (FLOAT) x;
      pt.y = (FLOAT) y;

      return prepare(pt);

   }

   bool graphics_path::prepare(D2D1_POINT_2F pt)
   {

      if(m_psink == NULL)
      {

         m_ppath->Open(&m_psink);

         m_psink->BeginFigure(pt, m_bFill ? D2D1_FIGURE_BEGIN_FILLED : D2D1_FIGURE_BEGIN_HOLLOW);

         if(m_efillmode == ::ca::fill_mode_winding)
         {

            m_psink->SetFillMode(D2D1_FILL_MODE_WINDING);

         }
         else if(m_efillmode == ::ca::fill_mode_alternate)
         {

            m_psink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);

         }

         m_bFigureEnded = false;

      }
      else
      {

         m_psink->AddLine(pt);

      }

      return true;

   }

   bool graphics_path::get_arc(D2D1_POINT_2F & pt, D2D1_ARC_SEGMENT & arcseg, const RECT & rect, int iStart, int iAngle)
   {

      float pi = 3.1415927f;

      D2D1_POINT_2F ptCenter;

      ptCenter.x = ((FLOAT) rect.left + (FLOAT) rect.right) / 2.0f;
      ptCenter.y = ((FLOAT) rect.top + (FLOAT) rect.bottom) / 2.0f;

      float rx = (FLOAT) rect.right    - ptCenter.x;
      float ry = (FLOAT) rect.bottom   - ptCenter.y;

      pt.x = ptCenter.x + cos(iStart * pi / 180.0f) * rx;
      pt.y = ptCenter.y - sin(iStart * pi / 180.0f) * ry;

      arcseg.point.x = ptCenter.x + cos((iStart + iAngle) * pi / 180.0f) * rx;
      arcseg.point.y = ptCenter.y + sin((iStart + iAngle) * pi / 180.0f) * ry;

      if(iAngle > 0)
      {
         arcseg.sweepDirection = D2D1_SWEEP_DIRECTION_CLOCKWISE;
      }
      else
      {
         arcseg.sweepDirection = D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE;
      }

      if(abs(iAngle) < 180)
      {
         arcseg.arcSize = D2D1_ARC_SIZE_SMALL;
      }
      else
      {
         arcseg.arcSize = D2D1_ARC_SIZE_LARGE;
      }

      arcseg.rotationAngle = (FLOAT) abs(iAngle);

      arcseg.size.width    = rx;

      arcseg.size.height   = ry;

      return true;

   }


   void * graphics_path::get_os_data()
   {

      if(m_psink != NULL)
      {

         if(!m_bFigureEnded)
            throw "figure not ended";

         m_psink->Close();

         m_psink->Release();

         m_psink = NULL;

      }

      return (ID2D1PathGeometry *) m_ppath;

   }

   ID2D1PathGeometry * graphics_path::get_os_path()
   {

      return (ID2D1PathGeometry *) get_os_data();

   }

} // namespace metrowin




