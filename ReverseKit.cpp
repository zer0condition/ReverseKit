#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <winternl.h>
#include <ntstatus.h>
#include <wininet.h>

#include "ReverseLib\ReverseHook.h"

#include "Hooks\CreateProcessInternalW.h"
#include "Hooks\NtCreateThreadEx.h"

#include "Hooks\UrlDownloadToFileW.h"
#include "Hooks\InternetOpenUrlW.h"

#include "Hooks\IsDebuggerPresent.h"
#include "Hooks\CheckRemoteDebuggerPresent.h"

unsigned char original_createprocess_bytes[14];
unsigned char original_createthread_bytes[14];
unsigned char original_urlmoniker_bytes[14];
unsigned char original_openurl_bytes[14];
unsigned char original_isdebug_bytes[14];
unsigned char original_remotedebug_bytes[14];

void GetImportsFromIAT()
{
    auto hModule = GetModuleHandle(NULL);

    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hModule;
    PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((BYTE*)hModule + pDosHeader->e_lfanew);
    PIMAGE_IMPORT_DESCRIPTOR  pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((BYTE*)hModule + pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    DWORD* pAddress;

    while (pImportDesc->Name)
    {
        PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)((BYTE*)hModule + pImportDesc->FirstThunk);
        PIMAGE_THUNK_DATA pThunkOrig = (PIMAGE_THUNK_DATA)((BYTE*)hModule + pImportDesc->OriginalFirstThunk);
        while (pThunkOrig->u1.AddressOfData)
        {
            if (pThunkOrig->u1.Ordinal & IMAGE_ORDINAL_FLAG)
            {
                pAddress = (DWORD*)&pThunk->u1.Function;
                printf("[ReverseKit] DLL: %s, Ordinal: %u, Address: %p\n", (char*)((BYTE*)hModule + pImportDesc->Name), IMAGE_ORDINAL(pThunkOrig->u1.Ordinal), (void*)*pAddress);
            }
            else
            {
                PIMAGE_IMPORT_BY_NAME pImportByName = (PIMAGE_IMPORT_BY_NAME)((BYTE*)hModule + pThunkOrig->u1.AddressOfData);
                pAddress = (DWORD*)&pThunk->u1.Function;
                printf("[ReverseKit] DLL: %s, Function: %s, Address: %p\n", (char*)((BYTE*)hModule + pImportDesc->Name), pImportByName->Name, (void*)*pAddress);
            }
            pThunk++;
            pThunkOrig++;
        }

        pImportDesc++;
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);

        AllocConsole();
        SetConsoleTitleA("ReverseKit Attached");

        FILE* pFile;
        freopen_s(&pFile, "CONOUT$", "w", stdout);

        // Imports info
        GetImportsFromIAT();

        // Place hooks
        oCreateProcessInternalW = (CreateProcessInternalW_t)GetProcAddress(GetModuleHandleA("kernelbase.dll"), "CreateProcessInternalW");
        ReverseHook::Trampoline::hook(oCreateProcessInternalW, hkCreateProcessInternalW, original_createprocess_bytes);

        oNtCreateThreadEx = (NtCreateThreadEx_t)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtCreateThreadEx");
        ReverseHook::Trampoline::hook(oNtCreateThreadEx, hkNtCreateThreadEx, original_createthread_bytes);

        oURLDownloadToFileA = (URLDownloadToFileA_t)GetProcAddress(GetModuleHandleA("urlmon.dll"), "URLDownloadToFileA");
        ReverseHook::Trampoline::hook(oURLDownloadToFileA, hkURLDownloadToFileA, original_urlmoniker_bytes);

        oInternetOpenUrlW = (InternetOpenUrlW_t)GetProcAddress(GetModuleHandleA("wininet.dll"), "InternetOpenUrlW");
        ReverseHook::Trampoline::hook(oInternetOpenUrlW, hkInternetOpenUrlW, original_openurl_bytes);

        oIsDebuggerPresent = (IsDebuggerPresent_t)GetProcAddress(GetModuleHandleA("kernel32.dll"), "IsDebuggerPresent");
        ReverseHook::Trampoline::hook(oIsDebuggerPresent, hkIsDebuggerPresent, original_isdebug_bytes);

        oCheckRemoteDebuggerPresent = (CheckRemoteDebuggerPresent_t)GetProcAddress(GetModuleHandleA("kernel32.dll"), "CheckRemoteDebuggerPresent");
        ReverseHook::Trampoline::hook(oCheckRemoteDebuggerPresent, hkCheckRemoteDebuggerPresent, original_remotedebug_bytes);

        break;

    case DLL_PROCESS_DETACH:
        FreeConsole();
        ReverseHook::Trampoline::unhook(oCreateProcessInternalW, original_createprocess_bytes);
        ReverseHook::Trampoline::unhook(oNtCreateThreadEx, original_createthread_bytes);
        ReverseHook::Trampoline::unhook(oURLDownloadToFileA, original_urlmoniker_bytes);
        ReverseHook::Trampoline::unhook(oInternetOpenUrlW, original_openurl_bytes);
        ReverseHook::Trampoline::unhook(oIsDebuggerPresent, original_isdebug_bytes);
        ReverseHook::Trampoline::unhook(oCheckRemoteDebuggerPresent, original_remotedebug_bytes);
        break;
    }
    return TRUE;
}
