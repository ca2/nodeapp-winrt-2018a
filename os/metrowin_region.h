#pragma once


namespace metrowin
{


   class region :
      virtual public ::ca2::region
   {
   public:


//      Gdiplus::Region *       m_pregion;
      ID2D1Geometry *            m_pgeometry;
//      ID2D1GeometrySink *        m_psink;
  //    bool                       m_bFill;
      //::ca2::e_fill_mode          m_efillmode;

      //static region * from_handle(::ca2::application * papp, HRGN hRgn);
      //operator HRGN() const;

      region(::ca2::application * papp);
      virtual ~region();

            
      void defer_update();


      /*bool CreateRectRgn(int x1, int y1, int x2, int y2);
      bool CreateRectRgnIndirect(LPCRECT lpRect);
      bool CreateEllipticRgn(int x1, int y1, int x2, int y2);
      bool CreateEllipticRgnIndirect(LPCRECT lpRect);
      bool CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode);
      bool CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts,
      int nCount, int nPolyFillMode);
      bool CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3);
      bool CreateFromPath(::ca2::graphics * pgraphics);
      bool CreateFromData(const XFORM* lpXForm, int nCount,
      const RGNDATA* pRgnData);*/

      // Operations
      void SetRectRgn(int x1, int y1, int x2, int y2);
      void SetRectRgn(LPCRECT lpRect);
      int CombineRgn(const ::ca2::region* pRgn1, const ::ca2::region* pRgn2, int nCombineMode);
      int CopyRgn(const ::ca2::region* pRgnSrc);
      bool EqualRgn(const ::ca2::region* pRgn) const;
      int OffsetRgn(int x, int y);
      int OffsetRgn(POINT point);
      int GetRgnBox(LPRECT lpRect) const;
      bool contains(int x, int y) const;
      bool contains(POINT point) const;
      bool RectInRegion(LPCRECT lpRect) const;
      int GetRegionData(LPRGNDATA lpRgnData, int nCount) const;

      virtual void * get_os_data() const;

      ID2D1Geometry * get();
      ID2D1Geometry * get_rect();
      ID2D1Geometry * get_oval();
      ID2D1Geometry * get_polygon();
      ID2D1Geometry * get_poly_polygon();
      ID2D1Geometry * get_combine();


   };


} // namespace metrowin


