#pragma once





// backlink at inha-ubuntu at veriverse at Curitiba near Finados Holyday 2012-11-03 from ca.dylib/ca.so/ca.dll
// backlink on 2012-11-04 in metrowin also and macos - see macos
namespace ca
{

   class CLASS_DECL_c null
   {
   public:


      null() {}

      null(const null &&) {}

      null & operator = (const null && n)
      {
         return *this;
      }

      template < typename T >
      operator T *() { return (T *) (uint_ptr) 0; }

      //operator HWND () { return 0; }


   };


} // namespace

inline ::ca::null null()
{
   return ::ca::null();
}

template < typename T >
inline bool operator != (const T t, const ::ca::null & n)
{
   return t != ((int_ptr) 0);
}

template < typename T >
inline bool operator != (const ::ca::null & n, const T t)
{
   return t != ((int_ptr) 0);
}

template < typename T >
inline bool operator == (const T t, const ::ca::null & n)
{
   return t == ((int_ptr) 0);
}

template < typename T >
inline bool operator == (const ::ca::null & n, const T t)
{
   return t == ((int_ptr) 0);
}



namespace user
{


   class interaction;
   class interaction_base;


} // namespace user




class oswindow_data;

class oswindow_dataptra;

class CLASS_DECL_c oswindow
{
public:


   oswindow_data *   m_pdata;


   ///static oswindow_dataptra * s_pdataptra;
   static int find(::user::interaction_base * pui);
   static oswindow_data * get(::user::interaction_base * pui);



public:


   oswindow();
   oswindow(const ::ca::null & null);
   oswindow(::user::interaction_base * pui);
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


   static bool remove(::user::interaction_base * pui);


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

   ::user::interaction * window();
   ::user::interaction * window() const;

   virtual Platform::Agile<Windows::UI::Core::CoreWindow> get_os_window();


};


