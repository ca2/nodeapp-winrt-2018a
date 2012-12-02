#pragma once


namespace metrowin
{


   class CLASS_DECL_metrowin bitmap : 
      virtual public ::ca::bitmap
   {
   public:

      ID2D1Bitmap *           m_pbitmap;
      ID2D1Bitmap1 *           m_pbitmap1;
      D2D1_MAPPED_RECT        m_map;
      primitive::memory       m_memory;
      //      ::Gdiplus::Bitmap *  m_pbitmap;
      //    void *               m_pdata;


      bitmap(::ca::application * papp);
      virtual ~bitmap();

      virtual void * get_os_data() const;


      bool LoadBitmap(const char * lpszResourceName);
      bool LoadBitmap(UINT nIDResource);
      bool LoadOEMBitmap(UINT nIDBitmap); // for OBM_/OCR_/OIC_

#ifndef ___NO_AFXCMN_SUPPORT
      //      bool LoadMappedBitmap(UINT nIDBitmap, UINT nFlags = 0,
      //       LPCOLORMAP lpColorMap = NULL, int nMapSize = 0);
#endif

      virtual bool CreateBitmap(::ca::graphics * pdc, int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void * lpBits, int stride);
      virtual bool CreateBitmapIndirect(::ca::graphics * pdc, LPBITMAP lpBitmap);
      virtual bool CreateCompatibleBitmap(::ca::graphics * pgraphics, int nWidth, int nHeight);
      virtual bool CreateDiscardableBitmap(::ca::graphics * pgraphics, int nWidth, int nHeight);
      virtual bool CreateDIBSection(::ca::graphics * pdc, const BITMAPINFO * lpbmi, UINT usage, void **ppvBits, HANDLE hSection, DWORD offset);
      virtual bool CreateDIBitmap(::ca::graphics * pdc, const BITMAPINFOHEADER *pbmih, DWORD flInit, const void *pjBits, const BITMAPINFO *pbmi, UINT iUsage);

      virtual bool attach(void * posdata);
      virtual void * detach();

      DWORD SetBitmapBits(DWORD dwCount, const void * lpBits);
      DWORD GetBitmapBits(DWORD dwCount, LPVOID lpBits) const;
      class size SetBitmapDimension(int nWidth, int nHeight);
      class size GetBitmapDimension() const;

      virtual void dump(dump_context & dumpcontext) const;


//      virtual bool Attach(HBITMAP hbitmap);


      virtual bool destroy();


   };


} // namespace metrowin

