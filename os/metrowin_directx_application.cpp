#include "framework.h"
#include <DirectXMath.h>

#undef System

using namespace Platform;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::System;
using namespace Windows::Graphics::Display;
using namespace Windows::System::Threading;

      UINT system_main(LPVOID lp)
      {
         ::plane::system * m_psystem = (::plane::system *) lp;
         try
         {
            m_psystem->set_thread(m_psystem);
            m_psystem->m_iReturnCode = 0;
            m_psystem->m_bReady = true;
            m_psystem->m_iReturnCode = m_psystem->on_run();
            if(m_psystem->m_iReturnCode != 0)
            {
               ::OutputDebugStringW(L"application::main on_run termination failure");
            }
            if(m_psystem->is_system())
            {
               m_psystem->os().post_to_all_threads(WM_QUIT, 0, 0);
            }
            try
            {
               m_psystem->m_iReturnCode = m_psystem->exit();
            }
            catch(...)
            {
               m_psystem->m_iReturnCode = -1;
            }

         }
         catch(::exit_exception &)
         {

            m_psystem->os().post_to_all_threads(WM_QUIT, 0, 0);

         }

         return 0;
      }
      


namespace metrowin
{


   directx_application::directx_application(::Platform::String ^ strId)
   {
      
      m_strId = strId;

      if(!main_initialize())
         throw "";


      //_set_purecall_handler(_ca2_purecall);

      m_psystem = new ::plane::system();

      m_psystem->m_pwindow = this;

      m_papp = m_psystem;

   //   ASSERT(hPrevInstance == NULL);

      int nReturnCode = 0;


      ::metrowin::main_init_data * pinitmaindata  = new ::metrowin::main_init_data;



   //pinitmaindata->m_hInstance             = hInstance;
   //pinitmaindata->m_hPrevInstance         = hPrevInstance;
      pinitmaindata->m_vssCommandLine        = gen::international::unicode_to_utf8(strId->Begin());
   //pinitmaindata->m_nCmdShow              = nCmdShow;

      m_psystem->init_main_data(pinitmaindata);

      m_psystem->m_papp = m_psystem;
      m_psystem->m_psystem = m_psystem;



      //MessageBox(NULL, "box1", "box1", MB_ICONINFORMATION);

      nReturnCode = m_psystem->main_start();

      string strLibrary = "m_" + m_psystem->command().m_varTopicQuery["app"];

      strLibrary.replace("\\", "_");
      strLibrary.replace("/", "_");
      strLibrary.replace("-", "_");

      m_psystem->m_mapAppLibrary[m_psystem->command().m_varTopicQuery["app"]] = strLibrary;
      m_psystem->m_mapAppLibrary["app/ca2/cube"] = "m_ca2";
      m_psystem->m_mapAppLibrary["app/ca2/bergedge"] = "m_ca2";

      __begin_thread(m_psystem, &system_main, m_psystem);



   }

/*   void directx_application::CreateDeviceIndependentResources()
   {
      directx_base::CreateDeviceIndependentResources();

      // Create a DirectWrite text format object.
      ::metrowin::throw_if_failed(
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
      ::metrowin::throw_if_failed(
         m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER)
         );

      // Center the text vertically.
      ::metrowin::throw_if_failed(
         m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER)
         );
   }

   void directx_application::CreateDeviceResources()
   {
      directx_base::CreateDeviceResources();

      m_sampleOverlay = ref new SampleOverlay();

      m_sampleOverlay->Initialize(
         m_d2dDevice.Get(),
         m_d2dContext.Get(),
         m_wicFactory.Get(),
         m_dwriteFactory.Get(),
         "DirectWrite Hello World sample"
         );

      ::metrowin::throw_if_failed(
         m_d2dContext->CreateSolidColorBrush(
         D2D1::ColorF(D2D1::ColorF::Black),
         &m_blackBrush
         )
         );
   }

   void directx_application::CreateWindowSizeDependentResources()
   {
      directx_base::CreateWindowSizeDependentResources();

      Platform::String^ text = "Hello World From ... DirectWrite!";

      D2D1_SIZE_F size = m_d2dContext->GetSize();

      // Create a DirectWrite Text Layout object
      ::metrowin::throw_if_failed(
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
      ::metrowin::throw_if_failed(
         m_textLayout->SetFontSize(100.0f, textRange)
         );

      // Create a DirectWrite Typography object
      ::metrowin::throw_if_failed(
         m_dwriteFactory->CreateTypography(
         &m_textTypography
         )
         );

      // Enumerate a stylistic set 6 font feature for application to our text layout
      DWRITE_FONT_FEATURE fontFeature = {DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_6, 1};

      // Apply the previously enumerated font feature to our Text Typography object
      ::metrowin::throw_if_failed(
         m_textTypography->AddFontFeature(fontFeature)
         );

      // Move our text range to the entire length of the string
      textRange.length = text->Length();
      textRange.startPosition = 0;

      // Apply our recently defined typography to our entire text range
      ::metrowin::throw_if_failed(
         m_textLayout->SetTypography(
         m_textTypography.Get(),
         textRange
         )
         );

      // Move the text range to the end again
      textRange.length = 12;
      textRange.startPosition = 21;

      // Set the underline on the text range
      ::metrowin::throw_if_failed(
         m_textLayout->SetUnderline(TRUE, textRange)
         );

      // Set the font weight on the text range
      ::metrowin::throw_if_failed(
         m_textLayout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, textRange)
         );
   }

   void directx_application::Render()
   {
      m_d2dContext->BeginDraw();

      D2D1_COLOR_F cr = D2D1::ColorF(D2D1::ColorF::White);
      m_d2dContext->Clear(cr);


      ::ca::graphics_sp dc(::ca::get_thread_app());

      dc->attach((ID2D1DeviceContext * ) m_d2dContext);

      Sys(::ca::get_thread_app())._001OnDraw(&dc);

      dc->detach();

      m_d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());

      m_d2dContext->DrawTextLayout(
         D2D1::Point2F(0.0f, 0.0f),
         m_textLayout.Get(),
         m_blackBrush.Get()
         );

      simple_graphics g;

      g.reference_os_data(m_d2dContext.Get());

      RECT rect;

      rect.left     = 200;
      rect.top      = 200;
      rect.right    = 300;
      rect.bottom   = 100;

      g.fill_solid_rect(rect, ARGB(127, 255, 255, 240));

      g.m_pdc = NULL;


      // We ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
      // is lost. It will be handled during the next call to Present.
      HRESULT hr = m_d2dContext->EndDraw();
      if (hr != D2DERR_RECREATE_TARGET)
      {
         ::metrowin::throw_if_failed(hr);
      }

//      m_sampleOverlay->Render();


   }*/

   void directx_application::Initialize(
      _In_ CoreApplicationView^ applicationView
      )
   {
      applicationView->Activated +=
         ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &directx_application::OnActivated);

      CoreApplication::Suspending +=
         ref new EventHandler<SuspendingEventArgs^>(this, &directx_application::OnSuspending);

      CoreApplication::Resuming +=
         ref new EventHandler<Platform::Object^>(this, &directx_application::OnResuming);
   }

   void directx_application::SetWindow(
      _In_ CoreWindow^ window
      )
   {
      m_window = window;

      m_psystem->m_window = m_window;

      window->PointerCursor = ref new CoreCursor(CoreCursorType::Arrow, 0);

      window->SizeChanged +=
         ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &directx_application::OnWindowSizeChanged);

   window->PointerMoved +=
        ref new Windows::Foundation::TypedEventHandler<
            Windows::UI::Core::CoreWindow^, Windows::UI::Core::PointerEventArgs^>(
                this, &directx_application::OnPointerMoved);

      DisplayProperties::LogicalDpiChanged +=
         ref new DisplayPropertiesEventHandler(this, &directx_application::OnLogicalDpiChanged);

      DisplayProperties::DisplayContentsInvalidated +=
         ref new DisplayPropertiesEventHandler(this, &directx_application::OnDisplayContentsInvalidated);

      m_psystem->defer_initialize_twf();

      ::metrowin::window_draw * pdraw = dynamic_cast < ::metrowin::window_draw * > (m_psystem->get_twf());

      pdraw->m_directx->Initialize(window, DisplayProperties::LogicalDpi);
   }

   void directx_application::Load(
      _In_ Platform::String^ entryPoint
      )
   {
   }

   void directx_application::Run()
   {
/*      Render();
      Present();
      */
      m_window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
   }

   void directx_application::Uninitialize()
   {
   }

   void directx_application::OnWindowSizeChanged(
      _In_ CoreWindow^ sender,
      _In_ WindowSizeChangedEventArgs^ args
      )
   {
      ::metrowin::window_draw * pdraw = dynamic_cast < ::metrowin::window_draw * > (m_psystem->get_twf());

      pdraw->m_directx->UpdateForWindowSizeChange();
      //UpdateForWindowSizeChange();
//      m_sampleOverlay->UpdateForWindowSizeChange();
/*      Render();
      Present();*/
   }

   void directx_application::OnLogicalDpiChanged(
      _In_ Platform::Object^ sender
      )
   {
      ::metrowin::window_draw * pdraw = dynamic_cast < ::metrowin::window_draw * > (m_psystem->get_twf());

      pdraw->m_directx->SetDpi(DisplayProperties::LogicalDpi);
      /*SetDpi(DisplayProperties::LogicalDpi);
      Render();
      Present();*/
   }
   void directx_application::OnDisplayContentsInvalidated(
      _In_ Platform::Object^ sender
      )
   {
      // Ensure the D3D Device is available for rendering.
      ::metrowin::window_draw * pdraw = dynamic_cast < ::metrowin::window_draw * > (m_psystem->get_twf());

      pdraw->m_directx->ValidateDevice();
      /*ValidateDevice();

      Render();
      Present();*/
   }

   void directx_application::OnActivated(
      _In_ CoreApplicationView^ applicationView,
      _In_ IActivatedEventArgs^ args
      )
   {
      m_window->Activate();
   }

   void directx_application::OnSuspending(
      _In_ Platform::Object^ sender,
      _In_ SuspendingEventArgs^ args
      )
   {
   }

   void directx_application::OnResuming(
      _In_ Platform::Object^ sender,
      _In_ Platform::Object^ args
      )
   {
   }


void directx_application::OnPointerMoved(
    _In_ Windows::UI::Core::CoreWindow^,
    _In_ Windows::UI::Core::PointerEventArgs^ args)
{ 
    Windows::UI::Input::PointerPoint^ pointerPoint = args->CurrentPoint;

    ::g_iMouse = pointerPoint->PointerId;

    /*// Make sure the event belongs to the pointer that is currently inking
    if (_pointerId == (int) pointerPoint->PointerId)
    {
        if (_manipulationMode == Windows::UI::Input::Inking::InkManipulationMode::Erasing)
        {
            // Erase ink that intersects line from last point to current point
            Windows::Foundation::Rect invalidateRect = _strokeContainer->SelectWithLine(_manipulationPoints->GetAt(0), pointerPoint->Position);
            if (invalidateRect.Height != 0 || invalidateRect.Width != 0)
            {
                _strokeContainer->DeleteSelected();
                Render();
            }

            // Store current point: it will be the starting point at next pointer update
            _manipulationPoints->Clear();
            _manipulationPoints->Append(pointerPoint->Position);
        }
        else
        {
            Windows::Foundation::Point previousPoint;
            ID2D1StrokeStyle* strokeStyle;
            ID2D1SolidColorBrush* brush;
            float width;

            if (_manipulationMode == Windows::UI::Input::Inking::InkManipulationMode::Inking)
            {
                // Obtain intermediate points (including the last/current one)
                Windows::Foundation::Collections::IVector<Windows::UI::Input::PointerPoint^>^ intermediatePoints = args->GetIntermediatePoints();

                // Update ink manager with all intermediate points
                int i = intermediatePoints->Size - 1;
                // AppendToStroke returns the last point that was added to the stroke builder.
                // We need to save it because it is the initial point of the new line we want to render.
                previousPoint = _strokeBuilder->AppendToStroke(intermediatePoints->GetAt(i))->Position;
                for (i = i - 1; i >= 0; i--)
                {
                    _strokeBuilder->AppendToStroke(intermediatePoints->GetAt(i));
                }

                // Setup drawing attributes for live rendering
                strokeStyle = _inkStyle.Get();
                brush = _inkBrush.Get();
                width = _drawingAttributes->Size.Width;
            }
            else // _manipulationMode == Windows::UI::Input::Inking::InkManipulationMode::Selecting
            {
                previousPoint = _manipulationPoints->GetAt(_manipulationPoints->Size - 1);
                _manipulationPoints->Append(pointerPoint->Position);

                // Setup drawing attributes for live rendering
                strokeStyle = _selectionStyle.Get();
                brush = _selectionBrush.Get();
                width = 1.0f;
            }

            // Live rendering
            // First we need to copy the content of the last presented buffer: it contains
            // the Beziers and the lines we rendered at previous pointer moves.
            m_d3dContext->CopyResource(_currentBuffer.Get(), _previousBuffer.Get());
            // Then we draw a new line, from the last position of the pointer to its current one.
            m_d2dContext->BeginDraw();
            m_d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
            m_d2dContext->DrawLine(
                D2D1::Point2F(previousPoint.X, previousPoint.Y), 
                D2D1::Point2F(pointerPoint->Position.X, pointerPoint->Position.Y), 
                brush, 
                width, 
                strokeStyle);

            // We ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
            // is lost. It will be handled during the next call to Present.
            HRESULT hr = m_d2dContext->EndDraw();
            if (hr != D2DERR_RECREATE_TARGET)
            {
                DX::ThrowIfFailed(hr);
            }

            Present();
        }
    }*/
}

      directx_application_source::directx_application_source(Platform::String ^ strId)
      {
         m_strId = strId;
      }

      Windows::ApplicationModel::Core::IFrameworkView^ directx_application_source::CreateView()
      {
         return ref new directx_application(m_strId);
      }

   directx_application_source ^ new_directx_application_source(Platform::String ^ id)
   {
      return ref new directx_application_source(id);
   }


   Windows::Foundation::Rect directx_application::get_window_rect()
   {

      Windows::Foundation::Rect rect;

      rect.X = 0;
      rect.Y = 0;
      rect.Width = 600;
      rect.Height = 480;

      if(m_window == nullptr)
         return rect;

      ::wait(m_window->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler ([=, &rect]()
      {
         try
         {
            rect = m_window->Bounds;
         }
         catch(...)
         {
         }
      })));

      return rect;

   }

   Windows::Foundation::Point directx_application::get_cursor_pos()
   {

      Windows::Foundation::Point p;

      p.X = 0;
      p.Y = 0;

      if(m_window == nullptr)
         return p;

      ::wait(m_window->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler ([=, &p]()
      {
         try
         {

            Windows::Foundation::Collections::IVectorView < Windows::Devices::Input::PointerDevice ^ > ^ deva = ::Windows::Devices::Input::PointerDevice::GetPointerDevices();

            for(unsigned int ui = 0; ui < deva->Size; ui++)
            {

               Windows::Devices::Input::PointerDevice ^ dev = deva->GetAt(ui);

               if(dev->PointerDeviceType == ::Windows::Devices::Input::PointerDeviceType::Mouse)
               {

                   Windows::UI::Input::PointerPoint ^ pointerPoint = Windows::UI::Input::PointerPoint::GetCurrentPoint(g_iMouse);

                   p = pointerPoint->RawPosition;

                   break;

               }

            }

         }
         catch(...)
         {
         }
      })));

      return p;

   }

} // namespace metrowin




