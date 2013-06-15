#pragma once


namespace user
{


   class interaction;
   class interaction_base;


} // namespace user


class CLASS_DECL_ca oswindow_data
{
public:

   
   ::user::interaction_base * m_pui;


   ::user::interaction * window();

};


class oswindow_dataptra;


typedef oswindow_data * oswindow;


///static oswindow_dataptra * s_pdataptra;
CLASS_DECL_ca int oswindow_find(::user::interaction_base * pui);
CLASS_DECL_ca oswindow oswindow_get(::user::interaction_base * pui);
CLASS_DECL_ca bool oswindow_remove(::user::interaction_base * pui);
CLASS_DECL_ca Platform::Agile<Windows::UI::Core::CoreWindow> get_os_window(oswindow window);






