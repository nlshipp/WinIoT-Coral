/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#pragma once

#include <Mfidl.h>

class IVpuMft;   // Defined in IVpuMft.h, included in the cpp for this class

class CDecodeTask:
    public IMFAsyncCallback
{
public:
    // Creation Function
    static HRESULT Create(
                       const DWORD         dwDecodeWorkQueueID,
                       IMFSample*          pInputSample,
                       IMFAsyncCallback**  ppTask
                       );
    HRESULT Begin(
                IVpuMft* pMYMFT
                );
    HRESULT End(void);

    // IMFAsyncCallback Implementations
    HRESULT __stdcall GetParameters(
                          DWORD*  pdwFlags,
                          DWORD*  pdwQueue
                          );
    HRESULT __stdcall Invoke(
                          IMFAsyncResult *pAsyncResult
                          );

    // IUnknown Implementations
    ULONG   __stdcall AddRef(void);
    HRESULT __stdcall QueryInterface(
                          REFIID  riid,
                          void**  ppvObject
                          );
    ULONG   __stdcall Release(void);

protected:
    CDecodeTask(void);
    ~CDecodeTask(void);

    volatile ULONG m_ulRef;
    IMFSample*  m_pInputSample;
    DWORD       m_dwDecodeWorkQueueID;
};