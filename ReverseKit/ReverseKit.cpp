#include "ReverseLib/Includes.h"
#include "Instrumentation/InstrumentationCallback.h"
#include "Window/D3DWindow.h"
#include "Window/ImGuiSetup.h"
#include "Imports/Imports.h"
#include "Threads/Threads.h"
#include "Hooks/SetHooks.h"
#include "Menu/Menu.h"
#include "Window/Render.h"
#include "Heaps/Heaps.h"

DWORD WINAPI RetrievalThread(LPVOID lpParameter)
{
    while (true) {
        GetImportsFromIAT();
        GetThreadInformation();
        GetHeapsInformation();
        Sleep(5000);
    }
}

/* Unstable. */
DWORD WINAPI InstrumentationThread(LPVOID lpParameter)
{
    return Instrumentation::Initialize();
}

void InitializeReverseKit()
{
    /* Create threads here instead of directly in DllMain.
     * If a new thread created by CreateThread attempts to access a resource
     * that has not yet been initialized by DllMain,
     * it may cause a race condition or deadlock.
     * Similarly, if a thread created by CreateThread attempts to access a resource
     * that is being unloaded by DllMain, it may cause a crash or other unpredictable behavior.
     */
    CreateThread(nullptr, 0, RetrievalThread, nullptr, 0, nullptr);
    CreateThread(nullptr, 0, RenderThread, nullptr, 0, nullptr);
    CreateThread(nullptr, 0, InstrumentationThread, nullptr, 0, nullptr); // Unstable, May crash.
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) 
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        SetConsoleTitleA("ReverseKit Attached");
        SetHooks::HookSyscalls();
        InitializeReverseKit();
        break;
    case DLL_PROCESS_DETACH:
        SetHooks::UnhookSyscalls();
        break;
	default: 
        return TRUE;
    }

    return TRUE;
}
