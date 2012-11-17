#include "framework.h"
#include "CubeRenderer.h"
#include <DirectXMath.h>

#undef System

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::System;
using namespace Windows::Graphics::Display;

CubeRenderer::CubeRenderer() :
    m_loadingComplete(false),
    m_indexCount(0)
{
}

CubeRenderer::~CubeRenderer()
{
}

void CubeRenderer::CreateDeviceResources()
{
    Direct3DBase::CreateDeviceResources();

/*    auto loadVSTask = DX::ReadDataAsync("SimpleVertexShader.cso");
    auto loadPSTask = DX::ReadDataAsync("SimplePixelShader.cso");

    auto createVSTask = loadVSTask.then([this](DX::ByteArray ba) {
        auto bytecodeVS = ba.data;
        ::win::throw_if_failed(
            m_d3dDevice->CreateVertexShader(
                bytecodeVS->Data,
                bytecodeVS->Length,
                nullptr,
                &m_vertexShader
                )
            );

        const D3D11_INPUT_ELEMENT_DESC vertexDesc[] = 
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        ::win::throw_if_failed(
            m_d3dDevice->CreateInputLayout(
                vertexDesc,
                ARRAYSIZE(vertexDesc),
                bytecodeVS->Data,
                bytecodeVS->Length,
                &m_inputLayout
                )
            );
    });

    auto createPSTask = loadPSTask.then([this](DX::ByteArray ba) {
        auto bytecodePS = ba.data;
        ::win::throw_if_failed(
            m_d3dDevice->CreatePixelShader(
                bytecodePS->Data,
                bytecodePS->Length,
                nullptr,
                &m_pixelShader
                )
            );

        ::win::throw_if_failed(
            m_d3dDevice->CreateBuffer(
                &CD3D11_BUFFER_DESC(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER),
                nullptr,
                &m_constantBuffer
                )
            );
    });

    auto createCubeTask = (createPSTask && createVSTask).then([this] () {
        VertexPositionColor cubeVertices[] = 
        {
            {XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
            {XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
            {XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
            {XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f)},
            {XMFLOAT3( 0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
            {XMFLOAT3( 0.5f, -0.5f,  0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f)},
            {XMFLOAT3( 0.5f,  0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f)},
            {XMFLOAT3( 0.5f,  0.5f,  0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f)},
        };

        D3D11_SUBRESOURCE_DATA vertexBufferData = {0};
        vertexBufferData.pSysMem = cubeVertices;
        vertexBufferData.SysMemPitch = 0;
        vertexBufferData.SysMemSlicePitch = 0;
        ::win::throw_if_failed(
            m_d3dDevice->CreateBuffer(
                &CD3D11_BUFFER_DESC(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER),
                &vertexBufferData,
                &m_vertexBuffer
                )
            );

        unsigned short cubeIndices[] = 
        {
            0,2,1, // -x
            1,2,3,

            4,5,6, // +x
            5,7,6,

            0,1,5, // -y
            0,5,4,

            2,6,7, // +y
            2,7,3,

            0,4,6, // -z
            0,6,2,

            1,3,7, // +z
            1,7,5,
        };

        m_indexCount = ARRAYSIZE(cubeIndices);

        D3D11_SUBRESOURCE_DATA indexBufferData = {0};
        indexBufferData.pSysMem = cubeIndices;
        indexBufferData.SysMemPitch = 0;
        indexBufferData.SysMemSlicePitch = 0;
        ::win::throw_if_failed(
            m_d3dDevice->CreateBuffer(
                &CD3D11_BUFFER_DESC(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER),
                &indexBufferData,
                &m_indexBuffer
                )
            );
    });

    createCubeTask.then([this] () {
        m_loadingComplete = true;
    });*/
}

void CubeRenderer::CreateWindowSizeDependentResources()
{
    Direct3DBase::CreateWindowSizeDependentResources();

    float aspectRatio = m_windowBounds.Width / m_windowBounds.Height;
    float fovAngleY = 70.0f * XM_PI / 180.0f;
    if (aspectRatio < 1.0f)
    {
        fovAngleY /= aspectRatio;
    }

    m_constantBufferData.projection = XMMatrixTranspose(XMMatrixPerspectiveFovRH(
        fovAngleY,
        aspectRatio,
        0.01f,
        100.0f
        ));
}

void CubeRenderer::Update(float timeTotal, float timeDelta)
{
    XMVECTOR eye = XMVectorSet(0.0f, 0.7f, 1.5f, 0.0f);
    XMVECTOR at = XMVectorSet(0.0f, -0.1f, 0.0f, 0.0f);
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    m_constantBufferData.view = XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up));
    m_constantBufferData.model = XMMatrixTranspose(XMMatrixRotationY(timeTotal * XM_PIDIV4));
}

void CubeRenderer::Render()
{
    const float midnightBlue[] = { 0.098f, 0.098f, 0.439f, 1.000f };
    m_d3dContext->ClearRenderTargetView(
        m_d3dRenderTargetView.Get(),
        midnightBlue
        );

    m_d3dContext->ClearDepthStencilView(
        m_d3dDepthStencilView.Get(),
        D3D11_CLEAR_DEPTH,
        1.0f,
        0
        );

    // only draw the cube once it's loaded (this is async)
    if (!m_loadingComplete)
        return;

    m_d3dContext->OMSetRenderTargets(
        1,
        m_d3dRenderTargetView.GetAddressOf(),
        m_d3dDepthStencilView.Get()
        );

    m_d3dContext->UpdateSubresource(
        m_constantBuffer.Get(),
        0,
        NULL,
        &m_constantBufferData,
        0,
        0
        );

    UINT stride = sizeof(VertexPositionColor);
    UINT offset = 0;
    m_d3dContext->IASetVertexBuffers(
        0,
        1,
        m_vertexBuffer.GetAddressOf(),
        &stride,
        &offset
        );

    m_d3dContext->IASetIndexBuffer(
        m_indexBuffer.Get(),
        DXGI_FORMAT_R16_UINT,
        0
        );

    m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_d3dContext->IASetInputLayout(m_inputLayout.Get());

    m_d3dContext->VSSetShader(
        m_vertexShader.Get(),
        nullptr,
        0
        );

    m_d3dContext->VSSetConstantBuffers(
        0,
        1,
        m_constantBuffer.GetAddressOf()
        );

    m_d3dContext->PSSetShader(
        m_pixelShader.Get(),
        nullptr,
        0
        );

    m_d3dContext->DrawIndexed(
        m_indexCount,
        0,
        0
        );
}




app::app()
{
}

void app::CreateDeviceIndependentResources()
{
    Direct3DBase::CreateDeviceIndependentResources();

    // Create a DirectWrite text format object.
    ::win::throw_if_failed(
        m_dwriteFactory->CreateTextFormat(
            L"Gabriola",
            nullptr,
            DWRITE_FONT_WEIGHT_REGULAR,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            64.0f,
            L"en-US", // locale
            &m_textFormat
            )
        );

    // Center the text horizontally.
    ::win::throw_if_failed(
        m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER)
        );

    // Center the text vertically.
    ::win::throw_if_failed(
        m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER)
        );
}

void app::CreateDeviceResources()
{
    Direct3DBase::CreateDeviceResources();

    m_sampleOverlay = ref new SampleOverlay();

    m_sampleOverlay->Initialize(
        m_d2dDevice.Get(),
        m_d2dContext.Get(),
        m_wicFactory.Get(),
        m_dwriteFactory.Get(),
        "DirectWrite Hello World sample"
        );

    ::win::throw_if_failed(
        m_d2dContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &m_blackBrush
            )
        );
}

void app::CreateWindowSizeDependentResources()
{
    Direct3DBase::CreateWindowSizeDependentResources();

    Platform::String^ text = "Hello World From ... DirectWrite!";

    D2D1_SIZE_F size = m_d2dContext->GetSize();

    // Create a DirectWrite Text Layout object
    ::win::throw_if_failed(
        m_dwriteFactory->CreateTextLayout(
            text->Data(),                       // Text to be displayed
            text->Length(),                     // Length of the text
            m_textFormat.Get(),                 // DirectWrite Text Format object
            size.width,                         // Width of the Text Layout
            size.height,                        // Height of the Text Layout
            &m_textLayout
            )
        );

    // Text range for the "DirectWrite!" at the end of the string
    DWRITE_TEXT_RANGE textRange = {21, 12}; // 21 references the "D" in DirectWrite! and 12 is the number of characters in the word

    // Set the font size on that text range to 100
    ::win::throw_if_failed(
        m_textLayout->SetFontSize(100.0f, textRange)
        );

    // Create a DirectWrite Typography object
    ::win::throw_if_failed(
        m_dwriteFactory->CreateTypography(
            &m_textTypography
            )
        );

    // Enumerate a stylistic set 6 font feature for application to our text layout
    DWRITE_FONT_FEATURE fontFeature = {DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_6, 1};

    // Apply the previously enumerated font feature to our Text Typography object
    ::win::throw_if_failed(
        m_textTypography->AddFontFeature(fontFeature)
        );

    // Move our text range to the entire length of the string
    textRange.length = text->Length();
    textRange.startPosition = 0;

    // Apply our recently defined typography to our entire text range
    ::win::throw_if_failed(
        m_textLayout->SetTypography(
            m_textTypography.Get(),
            textRange
            )
        );

    // Move the text range to the end again
    textRange.length = 12;
    textRange.startPosition = 21;

    // Set the underline on the text range
    ::win::throw_if_failed(
        m_textLayout->SetUnderline(TRUE, textRange)
        );

    // Set the font weight on the text range
    ::win::throw_if_failed(
        m_textLayout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, textRange)
        );
}

void app::Render()
{
    m_d2dContext->BeginDraw();

    m_d2dContext->Clear(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));
    m_d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());

    m_d2dContext->DrawTextLayout(
        D2D1::Point2F(0.0f, 0.0f),
        m_textLayout.Get(),
        m_blackBrush.Get()
        );

    // We ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
    // is lost. It will be handled during the next call to Present.
    HRESULT hr = m_d2dContext->EndDraw();
    if (hr != D2DERR_RECREATE_TARGET)
    {
        ::win::throw_if_failed(hr);
    }

    m_sampleOverlay->Render();
}

void app::Initialize(
    _In_ CoreApplicationView^ applicationView
    )
{
    applicationView->Activated +=
        ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &app::OnActivated);

    CoreApplication::Suspending +=
        ref new EventHandler<SuspendingEventArgs^>(this, &app::OnSuspending);

    CoreApplication::Resuming +=
        ref new EventHandler<Platform::Object^>(this, &app::OnResuming);
}

void app::SetWindow(
    _In_ CoreWindow^ window
    )
{
    window->PointerCursor = ref new CoreCursor(CoreCursorType::Arrow, 0);

    window->SizeChanged +=
        ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &app::OnWindowSizeChanged);

    DisplayProperties::LogicalDpiChanged +=
        ref new DisplayPropertiesEventHandler(this, &app::OnLogicalDpiChanged);

    DisplayProperties::DisplayContentsInvalidated +=
        ref new DisplayPropertiesEventHandler(this, &app::OnDisplayContentsInvalidated);

    Direct3DBase::Initialize(window, DisplayProperties::LogicalDpi);
}

void app::Load(
    _In_ Platform::String^ entryPoint
    )
{
}

void app::Run()
{
    Render();
    Present();

    m_window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
}

void app::Uninitialize()
{
}

void app::OnWindowSizeChanged(
    _In_ CoreWindow^ sender,
    _In_ WindowSizeChangedEventArgs^ args
    )
{
    UpdateForWindowSizeChange();
    m_sampleOverlay->UpdateForWindowSizeChange();
    Render();
    Present();
}

void app::OnLogicalDpiChanged(
    _In_ Platform::Object^ sender
    )
{
    SetDpi(DisplayProperties::LogicalDpi);
    Render();
    Present();
}

void app::OnDisplayContentsInvalidated(
    _In_ Platform::Object^ sender
    )
{
    // Ensure the D3D Device is available for rendering.
    ValidateDevice();

    Render();
    Present();
}

void app::OnActivated(
    _In_ CoreApplicationView^ applicationView,
    _In_ IActivatedEventArgs^ args
    )
{
    m_window->Activate();
}

void app::OnSuspending(
    _In_ Platform::Object^ sender,
    _In_ SuspendingEventArgs^ args
    )
{
}

void app::OnResuming(
    _In_ Platform::Object^ sender,
    _In_ Platform::Object^ args
    )
{
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
    return ref new app();
}


[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
    auto directXAppSource = ref new Direct3DApplicationSource();
    ::Windows::ApplicationModel::Core::CoreApplication::Run(directXAppSource);
    return 0;
}
