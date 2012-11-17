#pragma once

#include "framework.h"

#include "CubeRenderer.h"

ref class app : public Direct3DBase, public Windows::ApplicationModel::Core::IFrameworkView
{
internal:
    app();

    // DirectXBase Methods
    virtual void CreateDeviceIndependentResources() override;
    virtual void CreateDeviceResources() override;
    virtual void CreateWindowSizeDependentResources() override;
    virtual void Render() override;

public:
    // IFrameworkView Methods
    virtual void Initialize(_In_ Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
    virtual void SetWindow(_In_ Windows::UI::Core::CoreWindow^ window);
    virtual void Load(_In_ Platform::String^ entryPoint);
    virtual void Run();
    virtual void Uninitialize();

private:
    // Event Handlers
    void OnWindowSizeChanged(
        _In_ Windows::UI::Core::CoreWindow^ sender,
        _In_ Windows::UI::Core::WindowSizeChangedEventArgs^ args
        );

    void OnLogicalDpiChanged(
        _In_ Platform::Object^ sender
        );

    void OnDisplayContentsInvalidated(
        _In_ Platform::Object^ sender
        );

    void OnActivated(
        _In_ Windows::ApplicationModel::Core::CoreApplicationView^ applicationView,
        _In_ Windows::ApplicationModel::Activation::IActivatedEventArgs^ args
        );

    void OnSuspending(
        _In_ Platform::Object^ sender,
        _In_ Windows::ApplicationModel::SuspendingEventArgs^ args
        );

    void OnResuming(
        _In_ Platform::Object^ sender,
        _In_ Platform::Object^ args
        );

    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>                    m_blackBrush;
    Microsoft::WRL::ComPtr<IDWriteTextFormat>                       m_textFormat;
    Microsoft::WRL::ComPtr<IDWriteTypography>                       m_textTypography;
    Microsoft::WRL::ComPtr<IDWriteTextLayout>                       m_textLayout;
    SampleOverlay^                                                  m_sampleOverlay;
};


ref class Direct3DApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
{
public:
    virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};
