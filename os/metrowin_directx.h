#pragma once


#include <wrl/client.h>
#include <d3d11_1.h>
#include <d2d1_1.h>
#include <dwrite_1.h>
#include <DirectXMath.h>
#include <wincodec.h>
#include <memory>
#include <agile.h>

namespace metrowin
{


   inline void throw_if_failed(HRESULT hr)
   {

      if(FAILED(hr))
         throw hresult_exception(::ca2::get_thread_app(), hr);
      //if(FAILED(hr))
      // throw hr;

   }




   // Helper class that initializes DirectX APIs for both 2D and 3D rendering.
   // Some of the code in this class may be omitted if only 2D or only 3D rendering is being used.
   ref class directx_base
   {
   internal:

      ::ca2::application * m_papp;

      mutex    m_mutexDc;

      bool m_bInitialized;

      directx_base(::ca2::application * papp);

      ::ca2::application * get_app() const
      {
         return m_papp;
      }
   

      virtual void Initialize(Windows::UI::Core::CoreWindow^ window, float dpi);
      virtual void HandleDeviceLost();
      virtual void CreateDeviceIndependentResources();
      virtual void CreateDeviceResources();
      virtual void SetDpi(float dpi);
      virtual void UpdateForWindowSizeChange();
      virtual void CreateWindowSizeDependentResources();
      virtual void Render(::user::interaction_ptr_array & uiptra);
      virtual void Present();
      void ValidateDevice();

   protected private:
      Platform::Agile<Windows::UI::Core::CoreWindow>  m_window;

      // DirectWrite & Windows Imaging Component Objects.
      Microsoft::WRL::ComPtr<IDWriteFactory1>         m_dwriteFactory;
      Microsoft::WRL::ComPtr<IWICImagingFactory2>     m_wicFactory;

      // DirectX Core Objects. Required for 2D and 3D.
      Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice;
      Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext;
      Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain;
      Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_d3dRenderTargetView;

      // Direct2D Rendering Objects. Required for 2D.
      //Microsoft::WRL::ComPtr<ID2D1Factory1>           m_d2dFactory;
      Microsoft::WRL::ComPtr<ID2D1Device>             m_d2dDevice;
      Microsoft::WRL::ComPtr<ID2D1DeviceContext>      m_d2dContext;
      Microsoft::WRL::ComPtr<ID2D1Bitmap1>            m_d2dTargetBitmap;

      // Direct3D Rendering Objects. Required for 3D.
      Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_d3dDepthStencilView;

      // Cached renderer properties.
      D3D_FEATURE_LEVEL                               m_featureLevel;
      Windows::Foundation::Size                       m_renderTargetSize;
      Windows::Foundation::Rect                       m_windowBounds;
      float                                           m_dpi;
      bool                                            m_windowSizeChangeInProgress;



      Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>                    m_blackBrush;
      Microsoft::WRL::ComPtr<IDWriteTextFormat>                       m_textFormat;
      Microsoft::WRL::ComPtr<IDWriteTypography>                       m_textTypography;
      Microsoft::WRL::ComPtr<IDWriteTextLayout>                       m_textLayout;
      //SampleOverlay^                                                  m_sampleOverlay;


   };


} // namespace metrowin




