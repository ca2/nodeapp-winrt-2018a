#pragma once


namespace metrowin
{


   class CLASS_DECL_metrowin bitmap : 
      virtual public ::ca2::bitmap
   {
   public:

      ID2D1Bitmap *           m_pbitmap;
      ID2D1Bitmap1 *           m_pbitmap1;
      D2D1_MAPPED_RECT        m_map;
      primitive::memory       m_memory;
      //      ::Gdiplus::Bitmap *  m_pbitmap;
      //    void *               m_pdata;


      bitmap(::ca2::application * papp);
      virtual ~bitmap();

      virtual void * get_os_data() const;


      bool LoadBitmap(const char * lpszResourceName);
      bool LoadBitmap(UINT nIDResource);
      bool LoadOEMBitmap(UINT nIDBitmap); // for OBM_/OCR_/OIC_

#ifndef ___NO_AFXCMN_SUPPORT
      //      bool LoadMappedBitmap(UINT nIDBitmap, UINT nFlags = 0,
      //       LPCOLORMAP lpColorMap = NULL, int nMapSize = 0);
#endif

      virtual bool CreateBitmap(::ca2::graphics * pdc, int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void * lpBits, int stride);
      virtual bool CreateBitmapIndirect(::ca2::graphics * pdc, LPBITMAP lpBitmap);
      virtual bool CreateCompatibleBitmap(::ca2::graphics * pgraphics, int nWidth, int nHeight);
      virtual bool CreateDiscardableBitmap(::ca2::graphics * pgraphics, int nWidth, int nHeight);
      virtual bool CreateDIBSection(::ca2::graphics * pdc, const BITMAPINFO * lpbmi, UINT usage, void **ppvBits, int * stride, HANDLE hSection, uint32_t offset);
      virtual bool CreateDIBitmap(::ca2::graphics * pdc, const BITMAPINFOHEADER *pbmih, uint32_t flInit, const void *pjBits, const BITMAPINFO *pbmi, UINT iUsage);

      virtual bool attach(void * posdata);
      virtual void * detach();

      uint32_t SetBitmapBits(uint32_t dwCount, const void * lpBits);
      uint32_t GetBitmapBits(uint32_t dwCount, LPVOID lpBits) const;
      class size SetBitmapDimension(int nWidth, int nHeight);
      class size GetBitmapDimension() const;

      virtual void dump(dump_context & dumpcontext) const;


//      virtual bool Attach(HBITMAP hbitmap);


      virtual bool destroy();


   };


} // namespace metrowin

