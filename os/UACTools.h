#pragma once

#ifdef IMPLEMENT_VISTA_TOOLS

namespace VistaTools 
{

BOOL CLASS_DECL_VMSWIN IsVista();

/*
Use IsVista() to determine whether the current process is running under Windows Vista or 
(or a later version of Windows, whatever it will be)

Return Values:
   If the function succeeds, and the current version of Windows is Vista or later, 
      the return value is TRUE. 
   If the function fails, or if the current version of Windows is older than Vista 
      (that is, if it is Windows XP, Windows 2000, Windows Server 2003, Windows 98, etc.)
      the return value is FALSE.
*/

#ifndef WIN64

BOOL CLASS_DECL_VMSWIN
IsWow64();

/*
Use IsWow64() to determine whether the current 32-bit process is running under 64-bit Windows 
(Vista or XP)

Return Values:
   If the function succeeds, and the current version of Windows is x64, 
      the return value is TRUE. 
   If the function fails, or if the current version of Windows is 32-bit,
      the return value is FALSE.

While this function is not Vista specific (it works under XP as well), 
we include it here to be able to prevent execution of the 32-bit code under 64-bit Windows,
when required.
*/

#endif//WIN64


HRESULT CLASS_DECL_VMSWIN
GetElevationType( __out TOKEN_ELEVATION_TYPE * ptet );

/*
Use GetElevationType() to determine the elevation type of the current process.

Parameters:

ptet
   [out] Pointer to a var that receives the elevation type of the current process.

   The possible values are:

   TokenElevationTypeDefault - User is not using a "split" token. 
      This value indicates that either UAC is disabled, or the process is started
      by a standard ::fontopus::user (not a member of the Administrators group).

   The following two values can be returned only if both the UAC is enabled and
   the ::fontopus::user is a member of the Administrator's group (that is, the ::fontopus::user has a "split" token):

   TokenElevationTypeFull - the process is running elevated. 

   TokenElevationTypeLimited - the process is not running elevated.

Return Values:
   If the function succeeds, the return value is S_OK. 
   If the function fails, the return value is E_FAIL. To get extended error information, 
   call GetLastError().
*/

HRESULT CLASS_DECL_VMSWIN
IsElevated( __out_opt BOOL * pbElevated = NULL );

/*
Use IsElevated() to determine whether the current process is elevated or not.

Parameters:

pbElevated
   [out] [optional] Pointer to a BOOL var that, if non-NULL, receives the result.

   The possible values are:

   TRUE - the current process is elevated.
      This value indicates that either UAC is enabled, and the process was elevated by 
      the administrator, or that UAC is disabled and the process was started by a ::fontopus::user 
      who is a member of the Administrators group.

   FALSE - the current process is not elevated (limited).
      This value indicates that either UAC is enabled, and the process was started normally, 
      without the elevation, or that UAC is disabled and the process was started by a standard ::fontopus::user. 

Return Values
   If the function succeeds, and the current process is elevated, the return value is S_OK. 
   If the function succeeds, and the current process is not elevated, the return value is S_FALSE. 
   If the function fails, the return value is E_FAIL. To get extended error information, 
   call GetLastError().
*/

BOOL CLASS_DECL_VMSWIN
RunElevated(
   __in      HWND   hwnd, 
   __in      const char * pszPath, 
   __in_opt   const char * pszParameters   = NULL, 
   __in_opt   const char * pszDirectory   = NULL,
   __out_opt   HANDLE *phProcess      = NULL );

/*
Use RunElevated() to start an elevated process. This function calls ShellExecEx() with the verb "runas" 
to start the elevated process.

Parameters:

hwnd
   [in] Window handle to any message boxes that the system might produce while executing this function.

pszPath
   [in] Address of a null-terminated string that specifies the name of the executable file that 
      should be used to start the process.

pszParameters
   [in] [optional] Address of a null-terminated string that contains the command-line parameters for the process. 
      If NULL, no parameters are passed to the process.

pszDirectory
   [in] [optional] Address of a null-terminated string that specifies the name of the working directory. 
      If NULL, the current directory is used as the working directory. .

phProcess
   [out] [optional] Address where to return the handle to the newly started process. 
      You must call CloseHandle( *phProcess ) when the handle is no longer needed.
      If phProcess is NULL, no handle is returned.

Return Values
   If the function succeeds, the return value is TRUE. 
   If the function fails, the return value is FALSE. To get extended error information, 
   call GetLastError().

NOTE: This function will start a process elevated no matter which attribute (asInvoker, 
highestAvailable, or requireAdministrator) is specified in its manifest, and even if 
there is no such attribute at all.
*/

#ifndef DONTWANT_RunNonElevated

#ifdef NO_DLL_IMPORTS
#   error RunNonElevated must be used in a DLL project!
#endif//NO_DLL_IMPORTS


BOOL CLASS_DECL_VMSWIN
RunNonElevated(
   __in      HWND   hwnd, 
   __in      const char * pszPath, 
   __in_opt   const char * pszParameters   = NULL, 
   __in_opt   const char * pszDirectory   = NULL,
   __out_opt   HANDLE *phProcess      = NULL );

/*
Use RunNonElevated() to start a non-elevated process. If the current process is not elevated,
it calls ShellExecuteEx() to start the new process. If the current process is elevated,
it injects itself into the (non-elevated) shell process, and starts a non-elevated process from there.

Parameters:

hwnd
   [in] Window handle to any message boxes that the system might produce while executing this function.

pszPath
   [in] Address of a null-terminated string that specifies the executable file that 
      should be used to start the process.

pszParameters
   [in] [optional] Address of a null-terminated string that contains the command-line parameters for 
      the process. If NULL, no parameters are passed to the process.

pszDirectory
   [in] [optional] Address of a null-terminated string that specifies the name of the working directory. 
      If NULL, the current directory is used as the working directory. .

phProcess
   [out] [optional] Address where to return the handle to the newly started process. 
      You must call CloseHandle( *phProcess ) when the handle is no longer needed.
      If phProcess is NULL, no handle is returned.

Return Values
   If the function succeeds, the return value is TRUE. 
   If the function fails, the return value is FALSE. To get extended error information, 
   call GetLastError().

NOTE: For this function to work, the application must be marked with the asInvoker or 
highestAvailable attributes in its manifest. If the executable to be started is marked 
as requireAdministrator, it will be started elevated! 
*/
#endif //DONTWANT_RunNonElevated

//////////////////////////////////////////////////////////
// MyShellExec is just a wrapper around a call to ShellExecuteEx, 
// to be able to specify the verb easily.

BOOL CLASS_DECL_VMSWIN
MyShellExec(   HWND hwnd, 
            const char * pszVerb, 
            const char * pszPath, 
            const char * pszParameters   = NULL,
            const char * pszDirectory   = NULL,
            HANDLE * phProcess      = NULL );

} // namespace VistaTools 


#endif