#include "framework.h"

#define d2d1_fax_options D2D1_FACTORY_OPTIONS // fax of merde
#define single_threaded D2D1_FACTORY_TYPE_SINGLE_THREADED // ???? muliple performance multi thread hidden option there exists cost uses?

namespace metrowin
{


   

   graphics_path::graphics_path(::ca2::application * papp) :
      ca2(papp)
   {

      m_ppath = NULL;

      m_psink  = NULL;


   }

   graphics_path::~graphics_path() 
   {


   }


   bool graphics_path::internal_begin_figure(bool bFill, ::ca2::e_fill_mode efillmode)
   {

      if(m_psink != NULL)
      {
         
         internal_end_figure(false);


      }


      return true;


   }


   bool graphics_path::internal_end_figure(bool bClose)
   {

      if(bClose)
      {

         m_psink->EndFigure(D2D1_FIGURE_END_CLOSED);

      }
      else
      {

         m_psink->EndFigure(D2D1_FIGURE_END_OPEN);

      }

      return true;

   }

   bool graphics_path::internal_add_arc(const RECT & rect, int iStart, int iAngle)
   {

      D2D1_POINT_2F pt;

      D2D1_ARC_SEGMENT arcseg;

      internal_get_arc(pt, arcseg, rect, iStart, iAngle);

      if(!internal_prepare(pt))
         return false;

      m_psink->AddArc(arcseg);

      return true;

   }

   
   bool graphics_path::internal_add_line(int x1, int y1, int x2, int y2)
   {

      bool bOk1 = add_line(x1, y1);

      bool bOk2 = add_line(x2, y2);

      return bOk1 && bOk2;

   }


   bool graphics_path::internal_add_line(int x, int y)
   {

      D2D1_POINT_2F pt;

      pt.x = (FLOAT) x;
      pt.y = (FLOAT) y;

      return internal_prepare(pt);
   }


   bool graphics_path::internal_add_move(int x, int y)
   {

      end_figure(false);

      D2D1_POINT_2F pt;

      pt.x = (FLOAT) x;
      pt.y = (FLOAT) y;

      return internal_prepare(pt);

   }

   bool graphics_path::internal_prepare(D2D1_POINT_2F pt)
   {

      if(m_psink == NULL)
      {

         m_ppath->Open(&m_psink);

         m_psink->BeginFigure(pt, m_bFill ? D2D1_FIGURE_BEGIN_FILLED : D2D1_FIGURE_BEGIN_HOLLOW);

         if(m_efillmode == ::ca2::fill_mode_winding)
         {

            m_psink->SetFillMode(D2D1_FILL_MODE_WINDING);

         }
         else if(m_efillmode == ::ca2::fill_mode_alternate)
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

   bool graphics_path::internal_get_arc(D2D1_POINT_2F & pt, D2D1_ARC_SEGMENT & arcseg, const RECT & rect, int iStart, int iAngle)
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
      defer_update();

      return (ID2D1PathGeometry *) m_ppath;

   }

   ID2D1PathGeometry * graphics_path::get_os_path()
   {

      return (ID2D1PathGeometry *) get_os_data();

   }

   void * graphics_path::detach()
   {

      ID2D1PathGeometry * pgeometry = (ID2D1PathGeometry *) get_os_data();

      m_ppath = NULL;

      return pgeometry;

   }

   bool graphics_path::destroy()
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

      return true;
   
   }

   bool graphics_path::create()
   {

      if(m_ppath != NULL)
      {
         m_ppath->Release();
      }

      m_ppath = NULL;

      HRESULT hr = ::GetD2D1Factory1()->CreatePathGeometry(&m_ppath);

      ::metrowin::throw_if_failed(hr);
         
      m_psink  = NULL;

      for(int32_t i = 0; i < m_elementa.get_count(); i++)
      {

         set(m_elementa[i]);

      }

      m_psink->Close();

      m_psink->Release();

      return true;
      

   }

   bool graphics_path::set(const ::ca2::graphics_path::element & e)
   {

      switch(e.m_etype)
      {
      case ::ca2::graphics_path::element::type_move:
         set(e.m_move);
         break;
      case ::ca2::graphics_path::element::type_arc:
         set(e.m_arc);
         break;
      case ::ca2::graphics_path::element::type_line:
         set(e.m_line);
         break;
      case ::ca2::graphics_path::element::type_end:
         internal_end_figure(e.m_end.m_bClose);
         break;
      default:
         throw "unexpected simple os graphics element type";
      }

      return false;

   }


   bool graphics_path::set(const ::ca2::graphics_path::arc & arc)
   {
      
      rect rect;

      rect.left      = (LONG) (arc.m_xCenter - arc.m_dRadiusX);
      rect.right     = (LONG) (arc.m_xCenter + arc.m_dRadiusX);
      rect.top       = (LONG) (arc.m_yCenter - arc.m_dRadiusY);
      rect.bottom    = (LONG) (arc.m_yCenter + arc.m_dRadiusY);

      return internal_add_arc(rect, (int) arc.m_dAngle1, (int) arc.m_dAngle2);

   }


   bool graphics_path::set(const ::ca2::graphics_path::move & move)
   {

      return internal_add_move((int) move.m_x, (int) move.m_y);

   }


   bool graphics_path::set(const ::ca2::graphics_path::line & line)
   {

      return internal_add_line((int) line.m_x, (int) line.m_y);

   }


} // namespace metrowin




