#include <Windows.h>
#include <stdio.h>
#include <stdint.h>

#include "ReverseHook.h"
#include "CreateProcessInternalW.h"

unsigned char original_bytes[14];

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        SetConsoleTitleA("ReverseKit Attached");

        oCreateProcessInternalW = (CreateProcessInternalW_t)GetProcAddress(GetModuleHandleA("kernelbase.dll"), "CreateProcessInternalW");
        ReverseHook::hook(oCreateProcessInternalW, hkCreateProcessInternalW, original_bytes);
        break;

    case DLL_PROCESS_DETACH:
        ReverseHook::unhook(oCreateProcessInternalW, original_bytes);
        break;
    }
    return TRUE;
}
