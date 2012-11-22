#pragma once


namespace metrowin
{


   class CLASS_DECL_metrowin graphics_path :
      virtual public ::ca::graphics_path,
      virtual public graphics_object
   {
   public:


      ID2D1PathGeometry *        m_ppath;
      ID2D1GeometrySink *        m_psink;
      bool                       m_bFill;
      ::ca::e_fill_mode          m_efillmode;


      graphics_path(::ca::application * papp);
      virtual ~graphics_path();


      virtual bool begin_figure(bool bFill, ::ca::e_fill_mode efillmode);
      virtual bool end_figure(bool bClose);


      //virtual bool is_empty();
      //virtual bool has_current_point();
      //virtual point current_point();

      virtual bool add_arc(const RECT & rect, int iStart, int iAngle);

      virtual bool add_line(int x, int y);

      virtual bool add_move(int x, int y);

      virtual bool prepare(D2D1_POINT_2F pt);
      virtual bool get_arc(D2D1_POINT_2F & pt, D2D1_ARC_SEGMENT & arcseg, const RECT & rect, int iStart, int iAngle);

      virtual void * get_os_data();


   };


} // namespace metrowin



