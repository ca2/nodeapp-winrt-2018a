#include "StdAfx.h"
#include "WinFileSet.h"
#include "WFileFind.h"


WinFileSet::WinFileSet(::ca::application * papp) :
   ca(papp)
{
}

WinFileSet::~WinFileSet()
{

}

void WinFileSet::add_search(stringa & stra, bool_array & baRecursive)
{
   m_straSearch.add(stra);
   m_baRecursive.add(baRecursive);
   refresh();
}

void WinFileSet::add_filter(stringa & stra)
{
   m_straFilter.add(stra);
   refresh();
}

int WinFileSet::get_file_count()
{
   return m_straFile.get_size();
}

void WinFileSet::file_at(int i, string & str)
{
   str = m_straFile[i];
}

int WinFileSet::find_first_file(const char * lpcsz)
{
   return m_straFile.find_first_ci(lpcsz);
}

void WinFileSet::clear_search()
{
  m_straSearch.remove_all();
}

void WinFileSet::clear_filter()
{
  m_straFilter.remove_all();
}

void WinFileSet::clear_file()
{
   m_straFile.remove_all();
}

void WinFileSet::refresh()
{
   clear_file();
   string strFilter;
   int i, j;
   FileFind filefind;
   string strFile;
   string str;
   
   for(i = 0; i < m_straSearch.get_size(); i++)
   {
      bool bRecursive = true;
      if(i < m_baRecursive.get_size())
         bRecursive = m_baRecursive[i];
      for(j = 0; j < m_straFilter.get_size(); j++)
      {
         string strFilter = m_straFilter.element_at(j);
         strFilter.trim("\\/");
         if(bRecursive)
         {
            System.dir().rls_pattern(m_straSearch.element_at(i), strFilter, &m_straFile);
         }
         else
         {
            System.dir().ls_pattern(m_straSearch.element_at(i), strFilter, &m_straFile);
         }
      }
   }
}