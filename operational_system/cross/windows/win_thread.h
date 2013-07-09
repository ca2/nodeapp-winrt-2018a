// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (ca) Microsoft Corporation. All rights reserved.
//
//
// Emulates a subset of the Win32 threading API as a layer on top of WinRT threadpools.
//
// Supported features:
//
//    - CreateThread (returns a standard Win32 handle which can be waited on, then closed)
//    - CREATE_SUSPENDED and ResumeThread
//    - Partial support for SetThreadPriority (see below)
//    - Sleep
//    - Thread local storage (TlsAlloc, TlsFree, TlsGetValue, TlsSetValue)
//
// Differences from Win32:
//
//    - If using TLS other than from this CreateThread emulation, call TlsShutdown before thread/task exit
//    - No ExitThread or TerminateThread (just return from the thread function to exit)
//    - No SuspendThread, so ResumeThread is only useful in combination with CREATE_SUSPENDED
//    - SetThreadPriority is only available while a thread is in CREATE_SUSPENDED state
//    - SetThreadPriority only supports three priority levels (negative, zero, or positive)
//    - No thread identifier APIs (GetThreadId, GetCurrentThreadId, OpenThread)
//    - No affinity APIs
//    - No GetExitCodeThread
//    - Failure cases return error codes but do not always call SetLastError

#pragma once

#ifndef CREATE_SUSPENDED
#define CREATE_SUSPENDED 0x00000004
#endif

class hthread;

typedef hthread * HTHREAD;

CLASS_DECL_ca HTHREAD WINAPI CreateThread(LPSECURITY_ATTRIBUTES unusedThreadAttributes, uint_ptr unusedStackSize,  uint32_t (* pfn)(void *), void * pv, uint32_t dwCreationFlags, uint32_t * puiId);
CLASS_DECL_ca DWORD WINAPI ResumeThread(HTHREAD hThread);
CLASS_DECL_ca BOOL WINAPI SetThreadPriority(HTHREAD hThread, int nPriority);
CLASS_DECL_ca int WINAPI GetThreadPriority(HTHREAD hThread);

CLASS_DECL_ca VOID WINAPI Sleep(_In_ DWORD dwMilliseconds);

CLASS_DECL_ca DWORD WINAPI TlsAlloc();
CLASS_DECL_ca BOOL WINAPI TlsFree(DWORD dwTlsIndex);
CLASS_DECL_ca LPVOID WINAPI TlsGetValue(DWORD dwTlsIndex);
CLASS_DECL_ca BOOL WINAPI TlsSetValue(DWORD dwTlsIndex, LPVOID lpTlsValue);

void WINAPI TlsShutdown();


#ifdef METROWIN


interface IDWriteFactory;
interface ID2D1Factory1;
interface ID3D11Device;
interface ID3D11DeviceContext;
interface ID3D11Device1;
interface IDXGIDevice;

CLASS_DECL_ca IDWriteFactory * TlsGetWriteFactory();
CLASS_DECL_ca ID2D1Factory1 * GetD2D1Factory1();
CLASS_DECL_ca ID3D11Device * TlsGetD3D11Device();
CLASS_DECL_ca ID3D11DeviceContext * TlsGetD3D11DeviceContext();
CLASS_DECL_ca ID3D11Device1 * TlsGetD3D11Device1();
CLASS_DECL_ca IDXGIDevice * TlsGetDXGIDevice();

#define TLS_WRITE_FACTORY     12
#define TLS_D2D1_FACTORY1     13
#define TLS_d3_1              14
#define TLS_d3_2              15
#define TLS_d3_3              16
#define TLS_d3_4              17
#define TLS_MESSAGE_QUEUE     127

#endif