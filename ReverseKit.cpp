#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <fstream>

#include "ReverseHook.h"
#include "CreateProcessInternalW.h"

unsigned char original_bytes[14];

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
        SetConsoleTitleA("ReverseKit Attached");

        AllocConsole();

        FILE* pFile;
        freopen_s(&pFile, "CONOUT$", "w", stdout);

        // Imports info
        GetImportsFromIAT();

        // Place hooks
        oCreateProcessInternalW = (CreateProcessInternalW_t)GetProcAddress(GetModuleHandleA("kernelbase.dll"), "CreateProcessInternalW");
        ReverseHook::hook(oCreateProcessInternalW, hkCreateProcessInternalW, original_bytes);

        break;

    case DLL_PROCESS_DETACH:
        FreeConsole();
        ReverseHook::unhook(oCreateProcessInternalW, original_bytes);
        break;
    }
    return TRUE;
}
