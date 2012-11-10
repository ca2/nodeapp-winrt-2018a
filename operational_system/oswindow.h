#pragma once



// backlink at inha-ubuntu at veriverse at Curitiba near Finados Holyday 2012-11-03 from ca.dylib/ca.so/ca.dll
// backlink on 2012-11-04 in metrowin also and macos - see macos
namespace ca
{

   class CLASS_DECL_c null
   {
   public:


      null() {}

      operator HWND() { return NULL; }


   };

} // namespace

class oswindow_data;

class oswindow_dataptra;

class CLASS_DECL_c oswindow
{
public:


   oswindow_data *   m_pdata;


   static oswindow_dataptra * s_pdataptra;
   static int find(Windows::UI::Core::CoreWindow ^ window);
   static oswindow_data * get(Windows::UI::Core::CoreWindow ^ window);



public:


   oswindow();
   oswindow(const ::ca::null & null);
   oswindow(Windows::UI::Core::CoreWindow ^ window);
   oswindow(const oswindow & oswindow);
   oswindow(const void * p);
   oswindow(const LPARAM & lparam);
   oswindow(const WPARAM & wparam);



   operator void *()
   {
      return m_pdata;
   }

   operator void *() const
   {
      return m_pdata;
   }


   static bool remove(Windows::UI::Core::CoreWindow ^ window);


   oswindow & operator = (const oswindow & window);

   bool operator == (const void * p) const
   {
      return m_pdata == p;
   }
   bool operator != (const void * p) const
   {
      return m_pdata != p;
   }
   bool operator == (const oswindow & w) const
   {
      return m_pdata == w.m_pdata;
   }
   bool operator != (const oswindow & w) const
   {
      return m_pdata != w.m_pdata;
   }

   Windows::UI::Core::CoreWindow ^ window();
   Windows::UI::Core::CoreWindow ^ window() const;


};


