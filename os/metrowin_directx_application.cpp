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

uint_ptr keycode_to_char(uint_ptr n)
{
   return n;
}


uint_ptr virtualkey_to_char(::Windows::System::VirtualKey e)
{
   switch(e)
   {
   case ::Windows::System::VirtualKey::Number0:
      return '0';
   case ::Windows::System::VirtualKey::Number1:
      return '1';
   case ::Windows::System::VirtualKey::Number2:
      return '2';
   case ::Windows::System::VirtualKey::Number3:
      return '3';
   case ::Windows::System::VirtualKey::Number4:
      return '4';
   case ::Windows::System::VirtualKey::Number5:
      return '5';
   case ::Windows::System::VirtualKey::Number6:
      return '6';
   case ::Windows::System::VirtualKey::Number7:
      return '7';
   case ::Windows::System::VirtualKey::Number8:
      return '8';
   case ::Windows::System::VirtualKey::Number9:
      return '9';
   case ::Windows::System::VirtualKey::A:
      return 'a';
   case ::Windows::System::VirtualKey::B:
      return 'b';
   case ::Windows::System::VirtualKey::C:
      return 'c';
   case ::Windows::System::VirtualKey::D:
      return 'd';
   case ::Windows::System::VirtualKey::E:
      return 'e';
   case ::Windows::System::VirtualKey::F:
      return 'f';
   case ::Windows::System::VirtualKey::G:
      return 'g';
   case ::Windows::System::VirtualKey::H:
      return 'h';
   case ::Windows::System::VirtualKey::I:
      return 'i';
   case ::Windows::System::VirtualKey::J:
      return 'j';
   case ::Windows::System::VirtualKey::K:
      return 'k';
   case ::Windows::System::VirtualKey::L:
      return 'l';
   case ::Windows::System::VirtualKey::M:
      return 'm';
   case ::Windows::System::VirtualKey::N:
      return 'n';
   case ::Windows::System::VirtualKey::O:
      return 'o';
   case ::Windows::System::VirtualKey::P:
      return 'p';
   case ::Windows::System::VirtualKey::Q:
      return 'q';
   case ::Windows::System::VirtualKey::R:
      return 'r';
   case ::Windows::System::VirtualKey::S:
      return 's';
   case ::Windows::System::VirtualKey::T:
      return 't';
   case ::Windows::System::VirtualKey::U:
      return 'u';
   case ::Windows::System::VirtualKey::V:
      return 'v';
   case ::Windows::System::VirtualKey::W:
      return 'w';
   case ::Windows::System::VirtualKey::Y:
      return 'y';
   case ::Windows::System::VirtualKey::Z:
      return 'z';
   case ::Windows::System::VirtualKey::Shift:
      return VK_SHIFT;
   case ::Windows::System::VirtualKey::LeftShift:
      return VK_LSHIFT;
   case ::Windows::System::VirtualKey::RightShift:
      return VK_RSHIFT;
   case ::Windows::System::VirtualKey::Control:
      return VK_CONTROL;
   case ::Windows::System::VirtualKey::LeftControl:
      return VK_LCONTROL;
   case ::Windows::System::VirtualKey::RightControl:
      return VK_RCONTROL;
   case ::Windows::System::VirtualKey::Menu:
      return VK_MENU;
   case ::Windows::System::VirtualKey::LeftMenu:
      return VK_LMENU;
   case ::Windows::System::VirtualKey::RightMenu:
      return VK_RMENU;
   case ::Windows::System::VirtualKey::Tab:
      return VK_TAB;
   case ::Windows::System::VirtualKey::Enter:
      return VK_RETURN;
   default:;
   }
   return (int)e;
}

uint_ptr virtualkey_to_code(::Windows::System::VirtualKey e)
{
   switch(e)
   {
   case 190:
      return 52;
   case ::Windows::System::VirtualKey::Number0:
      return 1;
   case ::Windows::System::VirtualKey::Number1:
      return 2;
   case ::Windows::System::VirtualKey::Number2:
      return 3;
   case ::Windows::System::VirtualKey::Number3:
      return 4;
   case ::Windows::System::VirtualKey::Number4:
      return 5;
   case ::Windows::System::VirtualKey::Number5:
      return 6;
   case ::Windows::System::VirtualKey::Number6:
      return 7;
   case ::Windows::System::VirtualKey::Number7:
      return 8;
   case ::Windows::System::VirtualKey::Number8:
      return 9;
   case ::Windows::System::VirtualKey::Number9:
      return 10;
   case ::Windows::System::VirtualKey::A:
      return 'a';
   case ::Windows::System::VirtualKey::B:
      return 'b';
   case ::Windows::System::VirtualKey::C:
      return 'c';
   case ::Windows::System::VirtualKey::D:
      return 'd';
   case ::Windows::System::VirtualKey::E:
      return 'e';
   case ::Windows::System::VirtualKey::F:
      return 'f';
   case ::Windows::System::VirtualKey::G:
      return 'g';
   case ::Windows::System::VirtualKey::H:
      return 'h';
   case ::Windows::System::VirtualKey::I:
      return 'i';
   case ::Windows::System::VirtualKey::J:
      return 'j';
   case ::Windows::System::VirtualKey::K:
      return 'k';
   case ::Windows::System::VirtualKey::L:
      return 'l';
   case ::Windows::System::VirtualKey::M:
      return 'm';
   case ::Windows::System::VirtualKey::N:
      return 'n';
   case ::Windows::System::VirtualKey::O:
      return 'o';
   case ::Windows::System::VirtualKey::P:
      return 'p';
   case ::Windows::System::VirtualKey::Q:
      return 'q';
   case ::Windows::System::VirtualKey::R:
      return 'r';
   case ::Windows::System::VirtualKey::S:
      return 's';
   case ::Windows::System::VirtualKey::T:
      return 't';
   case ::Windows::System::VirtualKey::U:
      return 'u';
   case ::Windows::System::VirtualKey::V:
      return 'v';
   case ::Windows::System::VirtualKey::W:
      return 'w';
   case ::Windows::System::VirtualKey::Y:
      return 'y';
   case ::Windows::System::VirtualKey::Z:
      return 'z';
   case ::Windows::System::VirtualKey::Shift:
      return VK_SHIFT;
   case ::Windows::System::VirtualKey::LeftShift:
      return VK_LSHIFT;
   case ::Windows::System::VirtualKey::RightShift:
      return VK_RSHIFT;
   case ::Windows::System::VirtualKey::Control:
      return VK_CONTROL;
   case ::Windows::System::VirtualKey::LeftControl:
      return VK_LCONTROL;
   case ::Windows::System::VirtualKey::RightControl:
      return VK_RCONTROL;
   case ::Windows::System::VirtualKey::Menu:
      return VK_MENU;
   case ::Windows::System::VirtualKey::LeftMenu:
      return VK_LMENU;
   case ::Windows::System::VirtualKey::RightMenu:
      return VK_RMENU;
   default:;
   }
   return (int)e;
}

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


   directx_application::directx_application(::Platform::String ^ strId) :
      m_mutex(NULL)
   {

      m_ptLastCursor.X = 0;
      m_ptLastCursor.Y = 0;

      m_bLeftButton        = false;
      m_bMiddleButton      = false;
      m_bRightButton       = false;


      m_strId = strId;

      if(!main_initialize())
         throw "";


      //_set_purecall_handler(_ca2_purecall);

      m_psystem = new ::plane::system();


      m_psystem->m_pui = new ::user::interaction(m_psystem);

      m_psystem->m_pwindow = this;

      m_papp = m_psystem;

      //   ASSERT(hPrevInstance == NULL);

      int nReturnCode = 0;


      ::metrowin::main_init_data * pinitmaindata  = new ::metrowin::main_init_data;



      pinitmaindata->m_hInstance             = NULL;
      pinitmaindata->m_hPrevInstance         = NULL;
      pinitmaindata->m_vssCommandLine        = strId;
      pinitmaindata->m_nCmdShow              = 0;

      m_psystem->init_main_data(pinitmaindata);

      m_psystem->m_papp = m_psystem;
      m_psystem->m_psystem = m_psystem;



      //MessageBox(NULL, "box1", "box1", MB_ICONINFORMATION);

      nReturnCode = m_psystem->main_start();

      stringa straLibrary = m_psystem->command().m_varTopicQuery["app"];

      for(int i = 0; i < m_psystem->command().m_varTopicQuery["app"].get_count(); i++)
      {
         string strApp = m_psystem->command().m_varTopicQuery["app"][i];
      }

      straLibrary.replace("\\", "_");
      straLibrary.replace("-", "_");

      for(int i = 0; i < straLibrary.get_count(); i++)
      {
         string strLibrary = straLibrary[i];
         int iFind = strLibrary.find("/");
         if(iFind >= 0)
            strLibrary = strLibrary.Left(iFind) + '_' + strLibrary.Mid(iFind + 1);
         iFind = strLibrary.find("/", iFind + 1);
         if(iFind >= 0)
            strLibrary.Truncate(iFind);
         m_psystem->m_mapAppLibrary[m_psystem->command().m_varTopicQuery["app"][i]] = strLibrary;
      }

      for(int i = 0; i < m_psystem->command().m_varTopicQuery["app"].get_count(); i++)
      {
         string strApp = m_psystem->command().m_varTopicQuery["app"][i];
      }

      m_psystem->m_mapAppLibrary["app/ca2/cube"] = "ca2";
      m_psystem->m_mapAppLibrary["app/ca2/bergedge"] = "ca2";

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

   void directx_application::Initialize(CoreApplicationView ^ applicationView)
   {
      applicationView->Activated +=
         ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &directx_application::OnActivated);

      CoreApplication::Suspending +=
         ref new EventHandler<SuspendingEventArgs^>(this, &directx_application::OnSuspending);

      CoreApplication::Resuming +=
         ref new EventHandler<Platform::Object^>(this, &directx_application::OnResuming);
   }

   void directx_application::SetWindow(_In_ CoreWindow^ window)
   {
      m_window = window;

      m_psystem->m_pui->initialize(m_window.Get(), this);

      m_psystem->m_window = m_window;


      window->PointerCursor = ref new CoreCursor(CoreCursorType::Arrow, 0);

      window->SizeChanged +=
         ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &directx_application::OnWindowSizeChanged);

      window->PointerMoved +=
         ref new Windows::Foundation::TypedEventHandler<
         Windows::UI::Core::CoreWindow^, Windows::UI::Core::PointerEventArgs^>(
         this, &directx_application::OnPointerMoved);

      window->CharacterReceived +=
         ref new Windows::Foundation::TypedEventHandler<
         Windows::UI::Core::CoreWindow^, Windows::UI::Core::CharacterReceivedEventArgs^>(
         this, &directx_application::OnCharacterReceived);

      window->KeyDown +=
         ref new Windows::Foundation::TypedEventHandler<
         Windows::UI::Core::CoreWindow^, Windows::UI::Core::KeyEventArgs^>(
         this, &directx_application::OnKeyDown);

      window->KeyUp +=
         ref new Windows::Foundation::TypedEventHandler<
         Windows::UI::Core::CoreWindow^, Windows::UI::Core::KeyEventArgs^>(
         this, &directx_application::OnKeyUp);

      window->PointerPressed +=
         ref new Windows::Foundation::TypedEventHandler<
         Windows::UI::Core::CoreWindow^, Windows::UI::Core::PointerEventArgs^>(
         this, &directx_application::OnPointerPressed);

      window->PointerReleased +=
         ref new Windows::Foundation::TypedEventHandler<
         Windows::UI::Core::CoreWindow^, Windows::UI::Core::PointerEventArgs^>(
         this, &directx_application::OnPointerReleased);

      DisplayProperties::LogicalDpiChanged +=
         ref new DisplayPropertiesEventHandler(this, &directx_application::OnLogicalDpiChanged);

      DisplayProperties::DisplayContentsInvalidated +=
         ref new DisplayPropertiesEventHandler(this, &directx_application::OnDisplayContentsInvalidated);

      m_psystem->defer_initialize_twf();

      ::metrowin::window_draw * pdraw = dynamic_cast < ::metrowin::window_draw * > (m_psystem->get_twf());

      pdraw->m_directx->Initialize(window, DisplayProperties::LogicalDpi);

      m_rectLastWindowRect = m_window->Bounds;

   }

   void directx_application::Load(Platform::String^ entryPoint)
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

   void directx_application::OnWindowSizeChanged(CoreWindow ^ sender, WindowSizeChangedEventArgs ^ args)
   {
      ::metrowin::window_draw * pdraw = dynamic_cast < ::metrowin::window_draw * > (m_psystem->get_twf());


      m_rectLastWindowRect = m_window->Bounds;


      pdraw->m_directx->UpdateForWindowSizeChange();



      //UpdateForWindowSizeChange();
      //      m_sampleOverlay->UpdateForWindowSizeChange();
      /*      Render();
      Present();*/
   }

   void directx_application::OnLogicalDpiChanged(Platform::Object ^ sender)
   {
      ::metrowin::window_draw * pdraw = dynamic_cast < ::metrowin::window_draw * > (m_psystem->get_twf());

      m_rectLastWindowRect = m_window->Bounds;

      pdraw->m_directx->SetDpi(DisplayProperties::LogicalDpi);
      /*SetDpi(DisplayProperties::LogicalDpi);
      Render();
      Present();*/
   }
   void directx_application::OnDisplayContentsInvalidated(Platform::Object ^ sender)
   {
      // Ensure the D3D Device is available for rendering.
      ::metrowin::window_draw * pdraw = dynamic_cast < ::metrowin::window_draw * > (m_psystem->get_twf());

      pdraw->m_directx->ValidateDevice();
      /*ValidateDevice();

      Render();
      Present();*/
   }

   void directx_application::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
   {
      m_window->Activate();
   }

   void directx_application::OnSuspending(Platform::Object ^ sender, SuspendingEventArgs ^ args)
   {
   }

   void directx_application::OnResuming(Platform::Object ^ sender, Platform::Object ^ args)
   {
   }

   void directx_application::OnCharacterReceived(Windows::UI::Core::CoreWindow ^ , Windows::UI::Core::CharacterReceivedEventArgs ^ args)
   { 
      
      if(m_psystem == NULL)
         return;

      if(m_psystem->m_pui == NULL)
         return;

      if(m_psystem->m_pui->m_pimpl == NULL)
         return;

      ::ca::smart_pointer < ::ca::message::base > spbase;

      ::ca::message::key * pkey = new  ::ca::message::key(get_app());

      spbase = pkey;

      pkey->m_uiMessage = WM_CHAR;
      pkey->m_pwnd = m_psystem->m_pui;
      pkey->m_nChar = keycode_to_char(args->KeyCode);
//      pkey->m_charrecv = args;


      m_psystem->m_pui->m_pimpl->message_handler(spbase);

   }


   void directx_application::OnKeyDown(Windows::UI::Core::CoreWindow ^ , Windows::UI::Core::KeyEventArgs ^ args)
   { 
      
      if(m_psystem == NULL)
         return;

      if(m_psystem->m_pui == NULL)
         return;

      if(m_psystem->m_pui->m_pimpl == NULL)
         return;

      ::ca::smart_pointer < ::ca::message::base > spbase;

      ::ca::message::key * pkey = new  ::ca::message::key(get_app());

      spbase = pkey;

      pkey->m_uiMessage          = WM_KEYDOWN;
      pkey->m_pwnd               = m_psystem->m_pui;
      pkey->m_nChar              = virtualkey_to_char(args->VirtualKey);
      pkey->m_wparam             = pkey->m_nChar;
      pkey->m_nFlags             = virtualkey_to_code(args->VirtualKey);
      pkey->m_lparam             = pkey->m_nFlags << 16;
//      pkey->m_key = args;


      m_psystem->m_pui->m_pimpl->message_handler(spbase);

   }

   void directx_application::OnKeyUp(Windows::UI::Core::CoreWindow ^ , Windows::UI::Core::KeyEventArgs ^ args)
   { 
      
      if(m_psystem == NULL)
         return;

      if(m_psystem->m_pui == NULL)
         return;

      if(m_psystem->m_pui->m_pimpl == NULL)
         return;

      ::ca::smart_pointer < ::ca::message::base > spbase;

      ::ca::message::key * pkey = new  ::ca::message::key(get_app());

      spbase = pkey;

      pkey->m_uiMessage       = WM_KEYUP;
      pkey->m_pwnd            = m_psystem->m_pui;
      pkey->m_nChar           = virtualkey_to_char(args->VirtualKey);
      pkey->m_wparam          = pkey->m_nChar;
//      pkey->m_key = args;


      m_psystem->m_pui->m_pimpl->message_handler(spbase);

   }


   void directx_application::OnPointerMoved(Windows::UI::Core::CoreWindow ^ , Windows::UI::Core::PointerEventArgs ^ args)
   { 

      if(m_psystem == NULL)
         return;

      if(m_psystem->m_pui == NULL)
         return;

      if(m_psystem->m_pui->m_pimpl == NULL)
         return;

      Windows::UI::Input::PointerPoint^ pointerPoint = args->CurrentPoint;

      ::g_iMouse = pointerPoint->PointerId;

      ::ca::smart_pointer < ::ca::message::base > spbase;

      ::ca::message::mouse * pmouse = new  ::ca::message::mouse(get_app());

      spbase = pmouse;

      pmouse->m_pt.x       = (LONG) pointerPoint->RawPosition.X;
      pmouse->m_pt.y       = (LONG) pointerPoint->RawPosition.Y;
      pmouse->m_uiMessage  = WM_MOUSEMOVE;
      pmouse->m_pwnd       = m_psystem->m_pui;

      m_ptLastCursor = pointerPoint->RawPosition;

      m_psystem->m_pui->m_pimpl->message_handler(spbase);

   }

   void directx_application::OnPointerPressed(Windows::UI::Core::CoreWindow ^ , Windows::UI::Core::PointerEventArgs ^ args)
   { 
      
      if(m_psystem == NULL)
         return;

      if(m_psystem->m_pui == NULL)
         return;

      if(m_psystem->m_pui->m_pimpl == NULL)
         return;

      Windows::UI::Input::PointerPoint^ pointerPoint = args->CurrentPoint;

      ::g_iMouse = pointerPoint->PointerId;

      ::ca::smart_pointer < ::ca::message::base > spbase;

      ::ca::message::mouse * pmouse = new  ::ca::message::mouse(get_app());

      spbase = pmouse;

      pmouse->m_pt.x = (LONG) pointerPoint->RawPosition.X;
      
      pmouse->m_pt.y = (LONG) pointerPoint->RawPosition.Y;

      if(args->CurrentPoint->Properties->IsLeftButtonPressed && !m_bLeftButton)
      {
         
         pmouse->m_uiMessage     = WM_LBUTTONDOWN;
         
         m_bLeftButton           = true;
         m_bMiddleButton         = false;
         m_bRightButton          = false;

      }
      else if(args->CurrentPoint->Properties->IsRightButtonPressed && !m_bRightButton)
      {
         
         pmouse->m_uiMessage     = WM_RBUTTONDOWN;
         
         m_bLeftButton           = false;
         m_bMiddleButton         = false;
         m_bRightButton          = true;

      }
      else if(args->CurrentPoint->Properties->IsMiddleButtonPressed && !m_bMiddleButton)
      {
         
         pmouse->m_uiMessage     = WM_MBUTTONDOWN;
         
         m_bLeftButton           = false;
         m_bMiddleButton         = true;
         m_bRightButton          = false;

      }

      pmouse->m_pwnd = m_psystem->m_pui;

      m_ptLastCursor = pointerPoint->RawPosition;

      m_psystem->m_pui->m_pimpl->message_handler(spbase);

   }

   void directx_application::OnPointerReleased(Windows::UI::Core::CoreWindow ^ , Windows::UI::Core::PointerEventArgs ^ args)
   { 
      
      if(m_psystem == NULL)
         return;

      if(m_psystem->m_pui == NULL)
         return;

      if(m_psystem->m_pui->m_pimpl == NULL)
         return;

      Windows::UI::Input::PointerPoint^ pointerPoint = args->CurrentPoint;

      ::g_iMouse = pointerPoint->PointerId;

      ::ca::smart_pointer < ::ca::message::base > spbase;

      ::ca::message::mouse * pmouse = new  ::ca::message::mouse(get_app());

      spbase = pmouse;

      pmouse->m_pt.x = (LONG) pointerPoint->RawPosition.X;

      pmouse->m_pt.y = (LONG) pointerPoint->RawPosition.Y;

      if(m_bLeftButton && !args->CurrentPoint->Properties->IsLeftButtonPressed)
      {

         pmouse->m_uiMessage     = WM_LBUTTONUP;
         m_bLeftButton           = false;

      }
      else if(m_bRightButton && !args->CurrentPoint->Properties->IsRightButtonPressed)
      {
         
         pmouse->m_uiMessage     = WM_RBUTTONUP;
         m_bRightButton          = false;

      }
      else if(m_bMiddleButton && !args->CurrentPoint->Properties->IsMiddleButtonPressed)
      {
         
         pmouse->m_uiMessage     = WM_MBUTTONUP;
         m_bMiddleButton         = false;

      }




      pmouse->m_pwnd = m_psystem->m_pui;

      m_ptLastCursor = pointerPoint->RawPosition;

      m_psystem->m_pui->m_pimpl->message_handler(spbase);

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

      Windows::Foundation::Rect rect = m_rectLastWindowRect;

/*      rect.X = 0;
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
      })));*/

      return rect;

   }

   Windows::Foundation::Point directx_application::get_cursor_pos()
   {

      single_lock sl(&m_mutex, true);

      Windows::Foundation::Point p = m_ptLastCursor;

/*      if(m_window == nullptr)
         return p;

      if(g_iMouse < 0)
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
      })), 5); // wait for up to 5 milliseconds

      m_ptLastCursor = p;*/

      return p;

   }

} // namespace metrowin




