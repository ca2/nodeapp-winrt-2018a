#pragma once


namespace metrowin
{


   ref class directx_application : 
      public Windows::ApplicationModel::Core::IFrameworkView,
      public plane::system_window
   {
   internal:


      Platform::String ^ m_strId;

      Platform::Agile<Windows::UI::Core::CoreWindow>  m_window;

      ::plane::system * m_psystem;
      ::ca::application * m_papp;

      ::ca::application * get_app() const
      {
         return m_papp;
      }


      directx_application(Platform::String ^ strId);

      

      // DirectXBase Methods
      //virtual void CreateDeviceIndependentResources() override;
      //virtual void CreateDeviceResources() override;
      //virtual void CreateWindowSizeDependentResources() override;
      //virtual void Render() override;

   public:
      // IFrameworkView Methods
      virtual void Initialize(_In_ Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
      virtual void SetWindow(_In_ Windows::UI::Core::CoreWindow^ window);
      virtual void Load(_In_ Platform::String^ entryPoint);
      virtual void Run();
      virtual void Uninitialize();
      virtual Windows::Foundation::Rect get_window_rect();
      virtual Windows::Foundation::Point get_cursor_pos();
      

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

void OnPointerMoved(
    _In_ Windows::UI::Core::CoreWindow^,
    _In_ Windows::UI::Core::PointerEventArgs^ args);


      //Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>                    m_blackBrush;
      //Microsoft::WRL::ComPtr<IDWriteTextFormat>                       m_textFormat;
      //Microsoft::WRL::ComPtr<IDWriteTypography>                       m_textTypography;
      //Microsoft::WRL::ComPtr<IDWriteTextLayout>                       m_textLayout;
      //SampleOverlay^                                                  m_sampleOverlay;
      
   };


   ref class directx_application_source :
      Windows::ApplicationModel::Core::IFrameworkViewSource
   {
   internal:

      Platform::String ^ m_strId;

      directx_application_source(Platform::String ^ strId);

   public:

      virtual Windows::ApplicationModel::Core::IFrameworkView ^ CreateView();

   };


   CLASS_DECL_metrowin directx_application_source ^ new_directx_application_source(Platform::String ^ id);


} // namespace metrowin



