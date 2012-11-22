#pragma once


namespace metrowin
{


   class CLASS_DECL_metrowin pen : 
      virtual public ::metrowin::graphics_object,
      virtual public ::ca::pen
   {
   public:


      ID2D1SolidColorBrush *  m_psolidbrush;

      pen(::ca::application * papp);
      /*virtual void construct(int nPenStyle, double nWidth, COLORREF crColor);
      virtual void construct(int nPenStyle, double nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL);
      bool CreatePen(int nPenStyle, double nWidth, COLORREF crColor);
      bool CreatePen(int nPenStyle, double nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL);*/

      virtual bool create_solid(::ca::graphics * pgraphics, double dWidth, COLORREF crColor);

      virtual ID2D1Brush * get_os_pen_brush(::metrowin::graphics * pdc) const;


      virtual ~pen();
      virtual void dump(dump_context & dumpcontext) const;

   };

} // namespace metrowin