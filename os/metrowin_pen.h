#pragma once



#undef new

#include <GdiPlus.h>


namespace metrowin
{


   class CLASS_DECL_metrowin pen : 
      virtual public ::metrowin::graphics_object,
      virtual public ::ca::pen
   {
   public:


//      ::Gdiplus::Pen *  m_ppen;
   enum e_style
   {
      style_stock,
      style_solid,
   };
   
   
   int                     m_iStock;
   e_style                 m_estyle;
   int                     m_iWidth;
   COLORREF                m_cr;
   ID2D1SolidColorBrush *  m_psolidbrush;

      pen(::ca::application * papp);
      /*virtual void construct(int nPenStyle, double nWidth, COLORREF crColor);
      virtual void construct(int nPenStyle, double nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL);
      bool CreatePen(int nPenStyle, double nWidth, COLORREF crColor);
      bool CreatePen(int nPenStyle, double nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL);*/


      virtual void * get_os_data() const;


      virtual ~pen();
      virtual void dump(dump_context & dumpcontext) const;

   };

} // namespace metrowin