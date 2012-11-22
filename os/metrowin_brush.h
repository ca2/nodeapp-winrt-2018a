#pragma once


namespace metrowin
{


   class CLASS_DECL_metrowin brush : 
      virtual public ::ca::brush
   {
   public:


      //      Gdiplus::Brush * m_pbrush;


      union
      {

         ID2D1Brush *                  m_pbrush;

         ID2D1SolidColorBrush *        m_psolidbrush;

         ID2D1LinearGradientBrush *    m_plineargradientbrush;

      };


      brush(::ca::application * papp);
      //virtual void construct(COLORREF crColor);                // CreateSolidBrush
      //virtual void construct(int nIndex, COLORREF crColor);    // CreateHatchBrush
      //virtual void construct(::ca::bitmap * pbitmap);                // CreatePatternBrush

      virtual ~brush();

      virtual ID2D1Brush * get_os_brush(::metrowin::graphics * pdc) const;

      //bool CreateSolidBrush(COLORREF crColor);
      //bool CreateHatchBrush(int nIndex, COLORREF crColor);
      //bool CreatePatternBrush(::ca::bitmap* pBitmap);
      //bool CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage);
      //bool CreateDIBPatternBrush(const void * lpPackedDIB, UINT nUsage);
      //bool CreateSysColorBrush(int nIndex);


      virtual bool destroy();

      virtual void dump(dump_context & dumpcontext) const;
   };


} // namespace metrowin



