#pragma once

class CLASS_DECL_win WinFileSet : 
   public ex2::file_set
{
public:


   stringa        m_straSearch;
   bool_array     m_baRecursive;
   stringa        m_straFilter;
   stringa        m_straFile;


   WinFileSet(::ca::application * papp);
   virtual ~WinFileSet();


   virtual void add_search(stringa & stra, bool_array & baRecursive);
   virtual void add_filter(stringa & stra);
   virtual ::count get_file_count();
   virtual void file_at(::index i, string & str);
   virtual ::index find_first_file(const char * lpcszFilePath, ::index iStart = 0);
   virtual void clear_search();
   virtual void clear_filter();
   virtual void clear_file();
   virtual void refresh();


};

