#pragma once


namespace win
{


   class region :
      virtual public ::ca::region
   {
   public:


      Gdiplus::Region *       m_pregion;

      //static region * PASCAL from_handle(::ca::application * papp, HRGN hRgn);
      //operator HRGN() const;

      region(::ca::application * papp);
      virtual ~region();

      /*bool CreateRectRgn(int x1, int y1, int x2, int y2);
      bool CreateRectRgnIndirect(LPCRECT lpRect);
      bool CreateEllipticRgn(int x1, int y1, int x2, int y2);
      bool CreateEllipticRgnIndirect(LPCRECT lpRect);
      bool CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode);
      bool CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts,
            int nCount, int nPolyFillMode);
      bool CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3);
      bool CreateFromPath(::ca::graphics * pgraphics);
      bool CreateFromData(const XFORM* lpXForm, int nCount,
         const RGNDATA* pRgnData);*/

   // Operations
      void SetRectRgn(int x1, int y1, int x2, int y2);
      void SetRectRgn(LPCRECT lpRect);
      int CombineRgn(const ::ca::region* pRgn1, const ::ca::region* pRgn2, int nCombineMode);
      int CopyRgn(const ::ca::region* pRgnSrc);
      bool EqualRgn(const ::ca::region* pRgn) const;
      int OffsetRgn(int x, int y);
      int OffsetRgn(POINT point);
      int GetRgnBox(LPRECT lpRect) const;
      bool PtInRegion(int x, int y) const;
      bool PtInRegion(POINT point) const;
      bool RectInRegion(LPCRECT lpRect) const;
      int GetRegionData(LPRGNDATA lpRgnData, int nCount) const;

      virtual int_ptr get_os_data() const;

   };


} // namespace win


