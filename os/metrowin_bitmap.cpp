#include "framework.h"


#undef new


namespace metrowin
{


   bitmap::bitmap(::ca::application * papp) :
      ca(papp)
   { 

      m_pbitmap   = NULL;

   }

   bitmap::~bitmap()
   { 

      if(m_pbitmap != NULL)
      {

         destroy();

      }

   }

   bool bitmap::CreateBitmap(::ca::graphics * pgraphics, int cx, int cy, UINT nPlanes, UINT nBitcount, const void * lpBits)
   { 

      if(m_pbitmap != NULL)
      {

         destroy();

      }

      D2D1_SIZE_U size;

      size.width = cx;
      size.height = cy;

      D2D1_BITMAP_PROPERTIES1 props;

      props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_STRAIGHT;
      props.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
      props.dpiX = 72.0;
      props.dpiY = 72.0;
      props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CPU_READ;

      //if(ppdata != NULL)
      {
        // g.m_pdc->CreateBitmap(size, *ppdata, cx * sizeof(COLORREF), props, &m_pbitmap);
      }
      //else
      {
          METROWIN_DC(pgraphics)->m_pdc->CreateBitmap(size, lpBits, cx * sizeof(COLORREF), props, &m_pbitmap);
      }

      m_pbitmap->Map(D2D1_MAP_OPTIONS_READ | D2D1_MAP_OPTIONS_WRITE, &m_map);

      //if(ppdata != NULL)
        // *ppdata = (COLORREF *) m_map.bits;


      return true;

   }

   bool bitmap::CreateBitmapIndirect(::ca::graphics * pgraphics, LPBITMAP lpBitmap)
   { 
      return FALSE;
   }


   bool bitmap::CreateDIBSection(::ca::graphics * pgraphics, const BITMAPINFO * lpbmi, UINT usage, void **ppvBits, HANDLE hSection, DWORD offset)
   { 

      if(m_pbitmap != NULL)
      {

         destroy();

      }

      D2D1_SIZE_U size;

      size.width = lpbmi->bmiHeader.biWidth;
      size.height = lpbmi->bmiHeader.biHeight;

      D2D1_BITMAP_PROPERTIES1 props;

      props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_STRAIGHT;
      props.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
      props.dpiX = 72.0;
      props.dpiY = 72.0;
      props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CPU_READ;

      if(ppvBits != NULL)
      {
         METROWIN_DC(pgraphics)->m_pdc->CreateBitmap(size, *ppvBits, size.width * sizeof(COLORREF), props, &m_pbitmap);
      }
      else
      {
         METROWIN_DC(pgraphics)->m_pdc->CreateBitmap(size, NULL, size.width * sizeof(COLORREF), props, &m_pbitmap);
      }

      m_pbitmap->Map(D2D1_MAP_OPTIONS_READ | D2D1_MAP_OPTIONS_WRITE, &m_map);

      if(ppvBits != NULL)
         *ppvBits = m_map.bits;


      return true;

   }


   bool bitmap::CreateDIBitmap(::ca::graphics * pdc, const BITMAPINFOHEADER *pbmih, DWORD flInit, const void *pjBits, const BITMAPINFO *pbmi, UINT iUsage)
   { 
      return FALSE;
   }


   DWORD bitmap::SetBitmapBits(DWORD dwCount, const void * lpBits)
   { 

      //return ::SetBitmapBits((HBITMAP)get_handle(), dwCount, lpBits);
      return 0;

   }
   DWORD bitmap::GetBitmapBits(DWORD dwCount, LPVOID lpBits) const
   { 
      //return ::GetBitmapBits((HBITMAP)get_handle(), dwCount, lpBits);
      return 0;
   }
   bool bitmap::LoadBitmap(const char * lpszResourceName)
   { 
      //   return Attach(::LoadBitmap(gen::FindResourceHandle(
      //lpszResourceName, RT_BITMAP), lpszResourceName));
      return FALSE;

   }

   size bitmap::SetBitmapDimension(int nWidth, int nHeight)
   {


      throw not_implemented(get_app());

      //SIZE size;
      //VERIFY(::SetBitmapDimensionEx((HBITMAP)get_handle(), nWidth, nHeight, &size));
      //return size;
      return class ::size(0, 0);

   }

   size bitmap::GetBitmapDimension() const
   {

      if(m_pbitmap == NULL)
         return class size(0, 0);

      D2D1_SIZE_U size = m_pbitmap->GetPixelSize();

      return ::size(size.width, size.height);

   }

   bool bitmap::LoadBitmap(UINT nIDResource)
   { 
      //return Attach(::LoadBitmap(gen::FindResourceHandle(MAKEINTRESOURCE(nIDResource), RT_BITMAP), MAKEINTRESOURCE(nIDResource))); 
      return FALSE;
   }
   bool bitmap::LoadOEMBitmap(UINT nIDBitmap)
   { 
      //return Attach(::LoadBitmap(NULL, MAKEINTRESOURCE(nIDBitmap))); 
      return FALSE;
   }
   bool bitmap::CreateCompatibleBitmap(::ca::graphics * pgraphics, int nWidth, int nHeight)
   {


      if(m_pbitmap != NULL)
      {

         destroy();

      }

      D2D1_SIZE_U size;

      size.width = nWidth;
      size.height = nHeight;

      D2D1_BITMAP_PROPERTIES1 props;

      props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_STRAIGHT;
      props.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
      props.dpiX = 72.0;
      props.dpiY = 72.0;
      props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CPU_READ;

      //if(ppdata != NULL)
      {
        // g.m_pdc->CreateBitmap(size, *ppdata, cx * sizeof(COLORREF), props, &m_pbitmap);
      }
      //else
      {
          METROWIN_DC(pgraphics)->m_pdc->CreateBitmap(size, NULL, size.width * sizeof(COLORREF), props, &m_pbitmap);
      }

      m_pbitmap->Map(D2D1_MAP_OPTIONS_READ | D2D1_MAP_OPTIONS_WRITE, &m_map);

      //if(ppdata != NULL)
        // *ppdata = (COLORREF *) m_map.bits;


      return true;

/*
      if(m_pdata != NULL)
      {
         ca2_free(m_pdata);
         m_pdata = NULL;
      }

      if(m_pbitmap != NULL)
      {
         delete m_pbitmap;
         m_pbitmap = NULL;
      }

      m_pbitmap = new ::Gdiplus::Bitmap(nWidth, nHeight, Gdiplus::PixelOffsetModeHighQuality);

      return TRUE;*/

   }
   bool bitmap::CreateDiscardableBitmap(::ca::graphics * pgraphics, int nWidth, int nHeight)
   { 


      if(m_pbitmap != NULL)
      {

         destroy();

      }

      D2D1_SIZE_U size;

      size.width = nWidth;
      size.height = nHeight;

      D2D1_BITMAP_PROPERTIES1 props;

      props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_STRAIGHT;
      props.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
      props.dpiX = 72.0;
      props.dpiY = 72.0;
      props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CPU_READ;

      //if(ppdata != NULL)
      {
        // g.m_pdc->CreateBitmap(size, *ppdata, cx * sizeof(COLORREF), props, &m_pbitmap);
      }
      //else
      {
          METROWIN_DC(pgraphics)->m_pdc->CreateBitmap(size, NULL, size.width * sizeof(COLORREF), props, &m_pbitmap);
      }

      m_pbitmap->Map(D2D1_MAP_OPTIONS_READ | D2D1_MAP_OPTIONS_WRITE, &m_map);

      //if(ppdata != NULL)
        // *ppdata = (COLORREF *) m_map.bits;


      return true;

   }

/*
   int bitmap::GetBitmap(BITMAP* pBitMap)
   { 
      //   ASSERT(get_handle() != NULL);
      // return ::GetObject(get_handle(), sizeof(BITMAP), pBitMap); 
      return 0;
   }
   */

   /////////////////////////////////////////////////////////////////////////////


   void bitmap::dump(dump_context & dumpcontext) const
   {
      ::ca::graphics_object::dump(dumpcontext);

      /*         if (get_handle() == NULL)
      return;

      if (!afxData.bWin95 && ::GetObjectType(get_handle()) != OBJ_BITMAP)
      {
      // not a valid object
      dumpcontext << "has ILLEGAL HBITMAP!";
      return;
      }*/

      /*BITMAP bm;
      VERIFY(GetObject(sizeof(bm), &bm));
      dumpcontext << "bm.bmType = " << bm.bmType;
      dumpcontext << "\nbm.bmHeight = " << bm.bmHeight;
      dumpcontext << "\nbm.bmWidth = " << bm.bmWidth;
      dumpcontext << "\nbm.bmWidthBytes = " << bm.bmWidthBytes;
      dumpcontext << "\nbm.bmPlanes = " << bm.bmPlanes;
      dumpcontext << "\nbm.bmBitsPixel = " << bm.bmBitsPixel;
      */
      dumpcontext << "\n";
   }




   void * bitmap::get_os_data() const
   {

      return (void *) m_pbitmap;

   }

   bool bitmap::attach(void * hbitmap)
   {

      if(m_pbitmap != NULL)
      {

         destroy();

      }

      m_pbitmap = (ID2D1Bitmap1 *) hbitmap;

      m_pbitmap->Map(D2D1_MAP_OPTIONS_READ | D2D1_MAP_OPTIONS_WRITE, &m_map);

      return true;

   }

   void * bitmap::detach()
   {

      m_pbitmap->Unmap();

      ID2D1Bitmap * pbitmap = m_pbitmap;

      m_pbitmap = NULL;

      return pbitmap;

   }


   bool bitmap::destroy()
   {

      m_pbitmap->Unmap();

      m_pbitmap->Release();

      return true;

   }


} // namespace metrowin