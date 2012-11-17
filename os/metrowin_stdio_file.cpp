#include "framework.h"
#include "WinStdioFile.h"

#include <errno.h>
#include <io.h>
#include <fcntl.h>

WinStdioFile::WinStdioFile(::ca::application * papp) :
   ca(papp),
   WinFile(papp)
{
   m_pStream = NULL;
}

WinStdioFile::~WinStdioFile()
{
//   ASSERT_VALID(this);

//   if (m_pStream != NULL && m_bCloseOnDelete)
      //close();
   if (m_pStream != NULL)
      close();
}

bool WinStdioFile::open(const char * lpszFileName, UINT nOpenFlags)
{

   ASSERT(lpszFileName != NULL);

   ASSERT(__is_valid_string(lpszFileName));

   if(nOpenFlags  & ::ex1::file::defer_create_directory)
   {
      Application.dir().mk(System.dir().name(lpszFileName));
   }

   m_pStream = NULL;
   if (!WinFile::open(lpszFileName, (nOpenFlags & ~::ex1::file::type_text)))
      return FALSE;

   ASSERT(m_hFile != hFileNull);
   ASSERT(m_bCloseOnDelete);

   char szMode[4]; // C-runtime open string
   int nMode = 0;

   // determine read/write mode depending on ex1::filesp mode
   if (nOpenFlags & mode_create)
   {
      if (nOpenFlags & modeNoTruncate)
         szMode[nMode++] = 'a';
      else
         szMode[nMode++] = 'w';
   }
   else if (nOpenFlags & mode_write)
      szMode[nMode++] = 'a';
   else
      szMode[nMode++] = 'r';

   // add '+' if necessary (when read/write modes mismatched)
   if (szMode[0] == 'r' && (nOpenFlags & mode_read_write) ||
      szMode[0] != 'r' && !(nOpenFlags & mode_write))
   {
      // ::ex1::seek_current szMode mismatched, need to add '+' to fix
      szMode[nMode++] = '+';
   }

   // will be inverted if not necessary
   int nFlags = _O_RDONLY|_O_TEXT;
   if (nOpenFlags & (mode_write|mode_read_write))
      nFlags ^= _O_RDONLY;

   if (nOpenFlags & type_binary)
      szMode[nMode++] = 'b', nFlags ^= _O_TEXT;
   else
      szMode[nMode++] = 't';
   szMode[nMode++] = '\0';

   // open a C-runtime low-level file handle
   int nHandle = _open_osfhandle(m_hFile, nFlags);

   // open a C-runtime stream from that handle
   if (nHandle != -1)
      m_pStream = _fdopen(nHandle, szMode);

   if (m_pStream == NULL)
   {
      // an error somewhere along the way...
//      if (pException != NULL)
  //    {
//         pException->m_lOsError = _doserrno;
//         pException->m_cause = ::ex1::file_exception::OsErrorToException(_doserrno);
    //  }

      WinFile::Abort(); // close m_hFile
      return FALSE;
   }

   return TRUE;
}

::primitive::memory_size WinStdioFile::read(void * lpBuf, ::primitive::memory_size nCount)
{
   ASSERT_VALID(this);
   ASSERT(m_pStream != NULL);

   if (nCount == 0)
      return 0;   // avoid Win32 "null-read"

   ASSERT(__is_valid_address(lpBuf, nCount));

   primitive::memory_size nRead = 0;

   if ((nRead = fread(lpBuf, sizeof(BYTE), nCount, m_pStream)) == 0 && !feof(m_pStream))
      vfxThrowFileException(get_app(), ::ex1::file_exception::generic, _doserrno, m_strFileName);
   if (ferror(m_pStream))
   {
      clearerr(m_pStream);
      vfxThrowFileException(get_app(), ::ex1::file_exception::generic, _doserrno, m_strFileName);
   }
   return nRead;
}

void WinStdioFile::write(const void * lpBuf, ::primitive::memory_size nCount)
{
   ASSERT_VALID(this);
   ASSERT(m_pStream != NULL);
   ASSERT(__is_valid_address(lpBuf, nCount, FALSE));

   if (fwrite(lpBuf, sizeof(BYTE), nCount, m_pStream) != nCount)
      vfxThrowFileException(get_app(), ::ex1::file_exception::generic, _doserrno, m_strFileName);
}

void WinStdioFile::write_string(const char * lpsz)
{
   ASSERT(lpsz != NULL);
   ASSERT(m_pStream != NULL);

   if (_fputts(lpsz, m_pStream) == _TEOF)
      vfxThrowFileException(get_app(), ::ex1::file_exception::diskFull, _doserrno, m_strFileName);
}

LPTSTR WinStdioFile::read_string(LPTSTR lpsz, UINT nMax)
{
   ASSERT(lpsz != NULL);
   ASSERT(__is_valid_address(lpsz, nMax));
   ASSERT(m_pStream != NULL);

   LPTSTR lpszResult = _fgetts(lpsz, nMax, m_pStream);
   if (lpszResult == NULL && !feof(m_pStream))
   {
      clearerr(m_pStream);
      vfxThrowFileException(get_app(), ::ex1::file_exception::generic, _doserrno, m_strFileName);
   }
   return lpszResult;
}

UINT WinStdioFile::read_string(string & rString)
{
   ASSERT_VALID(this);

   //rString = &afxWchNil;    // is_empty string without deallocating
   rString.Empty();
   const int nMaxSize = 128;
   char * lpsz = rString.GetBuffer(nMaxSize);
   char * lpszResult;
   ::primitive::memory_size nLen = 0;
   for (;;)
   {
      lpszResult = fgets(lpsz, nMaxSize+1, m_pStream);
      rString.ReleaseBuffer();

      // handle error/eof case
      if (lpszResult == NULL && !feof(m_pStream))
      {
         clearerr(m_pStream);
         vfxThrowFileException(get_app(), ::ex1::file_exception::generic, _doserrno,
            m_strFileName);
      }

      // if string is read completely or EOF
      if (lpszResult == NULL ||
         (nLen = lstrlen(lpsz)) < nMaxSize ||
         lpsz[nLen-1] == '\n')
         break;

      nLen = rString.get_length();
      lpsz = rString.GetBuffer(nMaxSize + nLen) + nLen;
   }

   // remov '\n' from ::ex1::seek_end of string if present
   lpsz = rString.GetBuffer(0);
   nLen = rString.get_length();
   if (nLen != 0 && lpsz[nLen-1] == '\n')
      rString.GetBufferSetLength(nLen-1);

   return lpszResult != NULL;
}

/*void WinStdioFile::write_string(const char * lpsz)
{
   ASSERT(lpsz != NULL);
   ASSERT(m_pStream != NULL);

   if (fputws(lpsz, m_pStream) == _TEOF)
      vfxThrowFileException(get_app(), ::ex1::file_exception::diskFull, _doserrno, m_strFileName);
}*/

/*wchar_t * WinStdioFile::read_string(wchar_t * lpsz, UINT nMax)
{
   ASSERT(lpsz != NULL);
   ASSERT(__is_valid_address(lpsz, nMax));
   ASSERT(m_pStream != NULL);

   wchar_t * lpszResult = fgetws(lpsz, nMax, m_pStream);
   if (lpszResult == NULL && !feof(m_pStream))
   {
      clearerr(m_pStream);
      vfxThrowFileException(get_app(), ::ex1::file_exception::generic, _doserrno, m_strFileName);
   }
   return lpszResult;
}*/

file_position WinStdioFile::seek(file_offset lOff, ::ex1::e_seek nFrom)
{
   ASSERT_VALID(this);
   ASSERT(nFrom == ::ex1::seek_begin || nFrom == ::ex1::seek_end || nFrom == ::ex1::seek_current);
   ASSERT(m_pStream != NULL);

   if (fseek(m_pStream, (long) lOff, nFrom) != 0)
      vfxThrowFileException(get_app(), ::ex1::file_exception::badSeek, _doserrno,
         m_strFileName);

   long pos = ftell(m_pStream);
   return pos;
}

file_position WinStdioFile::get_position() const
{
   ASSERT_VALID(this);
   ASSERT(m_pStream != NULL);

   long pos = ftell(m_pStream);
   if (pos == -1)
      vfxThrowFileException(get_app(), ::ex1::file_exception::invalidFile, _doserrno,
         m_strFileName);
   return pos;
}

void WinStdioFile::Flush()
{
   ASSERT_VALID(this);

   if (m_pStream != NULL && fflush(m_pStream) != 0)
      vfxThrowFileException(get_app(), ::ex1::file_exception::diskFull, _doserrno,
         m_strFileName);
}

void WinStdioFile::close()
{
   ASSERT_VALID(this);
   ASSERT(m_pStream != NULL);

   int nErr = 0;

   if (m_pStream != NULL)
      nErr = fclose(m_pStream);

   m_hFile = (UINT) hFileNull;
   m_bCloseOnDelete = FALSE;
   m_pStream = NULL;

   if (nErr != 0)
      vfxThrowFileException(get_app(), ::ex1::file_exception::diskFull, _doserrno,
         m_strFileName);
}

void WinStdioFile::Abort()
{
   ASSERT_VALID(this);

   if (m_pStream != NULL && m_bCloseOnDelete)
      fclose(m_pStream);  // close but ignore errors
   m_hFile = (UINT) hFileNull;
   m_pStream = NULL;
   m_bCloseOnDelete = FALSE;
}

ex1::file * WinStdioFile::Duplicate() const
{
   ASSERT_VALID(this);
   ASSERT(m_pStream != NULL);

   throw not_supported_exception();
   return NULL;
}

void WinStdioFile::LockRange(file_position /* dwPos */, file_size /* dwCount */)
{
   ASSERT_VALID(this);
   ASSERT(m_pStream != NULL);

   throw not_supported_exception();
}

void WinStdioFile::UnlockRange(file_position /* dwPos */, file_size /* dwCount */)
{
   ASSERT_VALID(this);
   ASSERT(m_pStream != NULL);

   throw not_supported_exception();
}


void WinStdioFile::dump(dump_context & dumpcontext) const
{
   WinFile::dump(dumpcontext);

   dumpcontext << "m_pStream = " << (void *)m_pStream;
   dumpcontext << "\n";
}




file_size WinStdioFile::get_length() const
{
   ASSERT_VALID(this);

   LONG nCurrent;
   LONG nLength;
   LONG nResult;

   nCurrent = ftell(m_pStream);
   if (nCurrent == -1)
     vfxThrowFileException(get_app(), ::ex1::file_exception::invalidFile, _doserrno,
       m_strFileName);

   nResult = fseek(m_pStream, 0, SEEK_END);
   if (nResult != 0)
     vfxThrowFileException(get_app(), ::ex1::file_exception::badSeek, _doserrno,
       m_strFileName);

   nLength = ftell(m_pStream);
   if (nLength == -1)
     vfxThrowFileException(get_app(), ::ex1::file_exception::invalidFile, _doserrno,
       m_strFileName);
   nResult = fseek(m_pStream, nCurrent, SEEK_SET);
   if (nResult != 0)
     vfxThrowFileException(get_app(), ::ex1::file_exception::badSeek, _doserrno,
       m_strFileName);

   return nLength;
}
