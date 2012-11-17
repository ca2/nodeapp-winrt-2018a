#include "framework.h"
#include "Direct3DBase.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;

// Constructor.
Direct3DBase::Direct3DBase()
{
}

// Initialize the Direct3D resources required to run.
void Direct3DBase::Initialize(CoreWindow^ window, float dpi)
{

    m_window = window;
    
    CreateDeviceIndependentResources();
    CreateDeviceResources();
    CreateWindowSizeDependentResources();
    SetDpi(dpi);

}

// These are the resources that depend on the device.
void Direct3DBase::CreateDeviceResources()
{
    // This flag adds support for surfaces with a different color channel ordering
    // than the API default. It is required for compatibility with Direct2D.
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    ComPtr<IDXGIDevice> dxgiDevice;

#if defined(_DEBUG)
    // If the project is in a debug build, enable debugging via SDK Layers with this flag.
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // This array defines the set of DirectX hardware feature levels this app will support.
    // Note the ordering should be preserved.
    // Don't forget to declare your application's minimum required feature level in its
    // description.  All applications are assumed to support 9.1 unless otherwise stated.
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    // Create the Direct3D 11 API device object and a corresponding context.
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    ::win::throw_if_failed(
        D3D11CreateDevice(
            nullptr,                    // Specify nullptr to use the default adapter.
            D3D_DRIVER_TYPE_HARDWARE,
            0,
            creationFlags,              // Set debug and Direct2D compatibility flags.
            featureLevels,              // List of feature levels this app can support.
            ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Metro style apps.
            &device,                    // Returns the Direct3D device created.
            &m_featureLevel,            // Returns feature level of device created.
            &context                    // Returns the device immediate context.
            )
        );

    // Get the Direct3D 11.1 API device and context interfaces.
    ::win::throw_if_failed(
        device.As(&m_d3dDevice)
        );

    ::win::throw_if_failed(
        context.As(&m_d3dContext)
        );

    // Get the underlying DXGI device of the Direct3D device.
    ::win::throw_if_failed(
        m_d3dDevice.As(&dxgiDevice)
        );

    // Create the Direct2D device object and a corresponding context.
    ::win::throw_if_failed(
        m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice)
        );

    ::win::throw_if_failed(
        m_d2dDevice->CreateDeviceContext(
            D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
            &m_d2dContext
            )
        );

}

// Allocate all memory resources that change on a window SizeChanged event.
void Direct3DBase::CreateWindowSizeDependentResources()
{ 
    // Store the window bounds so the next time we get a SizeChanged event we can
    // avoid rebuilding everything if the size is identical.
    m_windowBounds = m_window->Bounds;

    // If the swap chain already exists, resize it.
    if(m_swapChain != nullptr)
    {
        ::win::throw_if_failed(
            m_swapChain->ResizeBuffers(2, 0, 0, DXGI_FORMAT_B8G8R8A8_UNORM, 0)
            );
    }
    // Otherwise, create a new one.
    else
    {
        // Create a descriptor for the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
        swapChainDesc.Width = 0;                                     // use automatic sizing
        swapChainDesc.Height = 0;
        swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;           // this is the most common swapchain format
        swapChainDesc.Stereo = false; 
        swapChainDesc.SampleDesc.Count = 1;                          // don't use multi-sampling
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 2;                               // use two buffers to enable flip effect
        swapChainDesc.Scaling = DXGI_SCALING_NONE;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // we recommend using this swap effect for all applications
        swapChainDesc.Flags = 0;

        // Once the desired swap chain description is configured, it must be created on the same adapter as our D3D Device

        // First, retrieve the underlying DXGI Device from the D3D Device
        ComPtr<IDXGIDevice1>  dxgiDevice;
        ::win::throw_if_failed(
            m_d3dDevice.As(&dxgiDevice)
            );

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        ::win::throw_if_failed(
            dxgiDevice->GetAdapter(&dxgiAdapter)
            );

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory2> dxgiFactory;
        ::win::throw_if_failed(
            dxgiAdapter->GetParent(
                __uuidof(IDXGIFactory2), 
                &dxgiFactory
                )
            );

		Windows::UI::Core::CoreWindow^ p = m_window.Get();

        // Create a swap chain for this window from the DXGI factory.
        ::win::throw_if_failed(
            dxgiFactory->CreateSwapChainForCoreWindow(
                m_d3dDevice.Get(),
                reinterpret_cast<IUnknown*>(p),
                &swapChainDesc,
                nullptr,    // allow on all displays
                &m_swapChain
                )
            );
            
        // Ensure that DXGI does not queue more than one frame at a time. This both reduces 
        // latency and ensures that the application will only render after each VSync, minimizing 
        // power consumption.
        ::win::throw_if_failed(
            dxgiDevice->SetMaximumFrameLatency(1)
            );
    }
    
    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    ::win::throw_if_failed(
        m_swapChain->GetBuffer(
            0,
            __uuidof(ID3D11Texture2D),
            &backBuffer
            )
        );

    // Create a view interface on the rendertarget to use on bind.
    ::win::throw_if_failed(
        m_d3dDevice->CreateRenderTargetView(
            backBuffer.Get(),
            nullptr,
            &m_d3dRenderTargetView
            )
        );

    // Cache the rendertarget dimensions in our helper class for convenient use.
    D3D11_TEXTURE2D_DESC backBufferDesc;
    backBuffer->GetDesc(&backBufferDesc);
    m_renderTargetSize.Width  = static_cast<float>(backBufferDesc.Width);
    m_renderTargetSize.Height = static_cast<float>(backBufferDesc.Height);

    // Create a descriptor for the depth/stencil buffer.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(
        DXGI_FORMAT_D24_UNORM_S8_UINT, 
        backBufferDesc.Width,
        backBufferDesc.Height,
        1,
        1,
        D3D11_BIND_DEPTH_STENCIL);

    // Allocate a 2-D surface as the depth/stencil buffer.
    ComPtr<ID3D11Texture2D> depthStencil;
    ::win::throw_if_failed(
        m_d3dDevice->CreateTexture2D(
            &depthStencilDesc,
            nullptr,
            &depthStencil
            )
        );

    // Create a DepthStencil view on this surface to use on bind.
    ::win::throw_if_failed(
        m_d3dDevice->CreateDepthStencilView(
            depthStencil.Get(),
            &CD3D11_DEPTH_STENCIL_VIEW_DESC(D3D11_DSV_DIMENSION_TEXTURE2D),
            &m_d3dDepthStencilView
            )
        );

    // Create a viewport descriptor of the full window size.
    CD3D11_VIEWPORT viewPort(
        0.0f,
        0.0f,
        static_cast<float>(backBufferDesc.Width),
        static_cast<float>(backBufferDesc.Height)
        );
        
    // Set the current viewport using the descriptor.
    m_d3dContext->RSSetViewports(1, &viewPort);
}

void Direct3DBase::UpdateForWindowSizeChange()
{
    if (m_window->Bounds.Width  != m_windowBounds.Width ||
        m_window->Bounds.Height != m_windowBounds.Height)
    {
        m_d3dRenderTargetView = nullptr;
        m_d3dDepthStencilView = nullptr;
        CreateWindowSizeDependentResources();
    }
}

void Direct3DBase::Present()
{
   
   // The application may optionally specify "dirty" or "scroll" rects to improve efficiency
    // in certain scenarios.  In this sample, however, we do not utilize those features.
    DXGI_PRESENT_PARAMETERS parameters = {0};
    parameters.DirtyRectsCount = 0;
    parameters.pDirtyRects = nullptr;
    parameters.pScrollRect = nullptr;
    parameters.pScrollOffset = nullptr;

    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present1(1, 0, &parameters);

    // Discard the contents of the render target.
    // This is a valid operation only when the existing contents will be entirely
    // overwritten. If dirty or scroll rects are used, this call should be removed.
    m_d3dContext->DiscardView(m_d3dRenderTargetView.Get());

    // Discard the contents of the depth stencil.
    m_d3dContext->DiscardView(m_d3dDepthStencilView.Get());

    // If the device was removed either by a disconnect or a driver upgrade, we
    // must recreate all device resources.
    if (hr == DXGI_ERROR_DEVICE_REMOVED)
    {
        HandleDeviceLost();
    }
    else
    {
        ::win::throw_if_failed(hr);
    }

    if (m_windowSizeChangeInProgress)
    {
        // A window size change has been initiated and the app has just completed presenting
        // the first frame with the new size. Notify the resize manager so we can short
        // circuit any resize animation and prevent unnecessary delays.
        CoreWindowResizeManager::GetForCurrentView()->NotifyLayoutCompleted();
        m_windowSizeChangeInProgress = false;
    }
}



// These are the resources required independent of the device.
void Direct3DBase::CreateDeviceIndependentResources()
{
    D2D1_FACTORY_OPTIONS options;
    ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
    // If the project is in a debug build, enable Direct2D debugging via SDK Layers.
    options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

    ::win::throw_if_failed(
        D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            __uuidof(ID2D1Factory1),
            &options,
            &m_d2dFactory
            )
        );

    ::win::throw_if_failed(
        DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            &m_dwriteFactory
            )
        );

    ::win::throw_if_failed(
        CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&m_wicFactory)
            )
        );
}



// Helps track the DPI in the helper class.
// This is called in the dpiChanged event handler in the view class.
void Direct3DBase::SetDpi(float dpi)
{
    if (dpi != m_dpi)
    {
        // Save the DPI of this display in our class.
        m_dpi = dpi;

        // Update Direct2D's stored DPI.
        m_d2dContext->SetDpi(m_dpi, m_dpi);

        // Often a DPI change implies a window size change. In some cases Windows will issue
        // both a size changed event and a DPI changed event. In this case, the resulting bounds
        // will not change, and the window resize code will only be executed once.
        UpdateForWindowSizeChange();
    }
}


void Direct3DBase::ValidateDevice()
{
    // The D3D Device is no longer valid if the default adapter changes or if
    // the device has been removed.

    // First, get the information for the adapter related to the current device.

    ComPtr<IDXGIDevice1> dxgiDevice;
    ComPtr<IDXGIAdapter> deviceAdapter;
    DXGI_ADAPTER_DESC deviceDesc;
    ::win::throw_if_failed(m_d3dDevice.As(&dxgiDevice));
    ::win::throw_if_failed(dxgiDevice->GetAdapter(&deviceAdapter));
    ::win::throw_if_failed(deviceAdapter->GetDesc(&deviceDesc));

    // Next, get the information for the default adapter.

    ComPtr<IDXGIFactory2> dxgiFactory;
    ComPtr<IDXGIAdapter1> currentAdapter;
    DXGI_ADAPTER_DESC currentDesc;
    ::win::throw_if_failed(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));
    ::win::throw_if_failed(dxgiFactory->EnumAdapters1(0, &currentAdapter));
    ::win::throw_if_failed(currentAdapter->GetDesc(&currentDesc));

    // If the adapter LUIDs don't match, or if the device reports that it has been removed,
    // a new D3D device must be created.

    if ((deviceDesc.AdapterLuid.LowPart != currentDesc.AdapterLuid.LowPart) ||
        (deviceDesc.AdapterLuid.HighPart != currentDesc.AdapterLuid.HighPart) ||
        FAILED(m_d3dDevice->GetDeviceRemovedReason()))
    {
        // Release references to resources related to the old device.
        dxgiDevice = nullptr;
        deviceAdapter = nullptr;

        // Create a new device and swap chain.
        HandleDeviceLost();
    }
}



// Recreate all device resources and set them back to the current state.
void Direct3DBase::HandleDeviceLost()
{
    // Reset these member variables to ensure that SetDpi recreates all resources.
    float dpi = m_dpi;
    m_dpi = -1.0f;
    m_windowBounds.Width = 0;
    m_windowBounds.Height = 0;
    m_swapChain = nullptr;

    CreateDeviceResources();
    SetDpi(dpi);
}
