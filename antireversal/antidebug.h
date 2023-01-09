//
// Created by zerte on 4/28/2020.
//

#ifndef XDG_ANTIDEBUG_H
#define XDG_ANTIDEBUG_H

#ifndef XDG_DEBUG
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma section(".CRT$XLY", long, read) __declspec(thread) int var = 0xDEADBEEF; VOID NTAnopPI TlsCallback(PVOID DllHandle, DWORD Reason, VOID Reserved) { var = 0xB15BADB0; if (IsDebuggerPresent()) { MessageBoxA(NULL, "Stop debugging program!", "Error", MB_OK | MB_ICONERROR); TerminateProcess(GetCurrentProcess(), 0xBABEFACE); } } __declspec(allocate(".CRT$XLY"))PIMAGE_TLS_CALLBACK g_tlsCallback = TlsCallback;
#endif

namespace Anti::Debugging {

    inline bool CheckProcessDebugFlags()
    {
#ifndef XDG_DEBUG
        // Much easier in ASM but C/C++ looks so much better
        typedef NTSTATUS (WINAPI *pNtQueryInformationProcess)
            (HANDLE ,UINT ,PVOID ,ULONG , PULONG);

        DWORD NoDebugInherit = 0;
        NTSTATUS Status;

        // Get NtQueryInformationProcess
        auto NtQIP = (pNtQueryInformationProcess)
            GetProcAddress( GetModuleHandle( TEXT("ntdll.dll") ),
            "NtQueryInformationProcess" );

        Status = NtQIP(GetCurrentProcess(),
                0x1f, // ProcessDebugFlags
                &NoDebugInherit, 4, nullptr);

        if (Status != 0x00000000)
            return false;

        return !NoDebugInherit;
#else
        return false;
#endif
    }

    inline bool DebugObjectCheck()
    {
#ifndef XDG_DEBUG
        // Much easier in ASM but C/C++ looks so much better
        typedef NTSTATUS (WINAPI *pNtQueryInformationProcess)
        (HANDLE ,UINT ,PVOID ,ULONG , PULONG);

        HANDLE hDebugObject = nullptr;
        NTSTATUS Status;

        // Get NtQueryInformationProcess
        auto NtQIP = (pNtQueryInformationProcess)
                GetProcAddress(  GetModuleHandle( TEXT("ntdll.dll") ),
                                 "NtQueryInformationProcess" );

        Status = NtQIP(GetCurrentProcess(),
                       0x1e, // ProcessDebugObjectHandle
                       &hDebugObject, 4, nullptr);

        if (Status != 0x00000000)
            return false;

        return hDebugObject != nullptr;
#else
        return false;
#endif
    }

    inline bool CheckOutputDebugString(const char* String)
    {
#ifndef XDG_DEBUG
        OutputDebugString(String);
        return GetLastError() == 0;
#else
      return false;
#endif
    }

}

#endif //XDG_ANTIDEBUG_H
