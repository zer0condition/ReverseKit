#include <Includes.h>
#include <Imports.h>
#include <Threads.h>
#include <SetHooks.h>
#include <Render.h>


DWORD WINAPI RetrievalThread(LPVOID lpParameter) 
{
    /*
    * Crashing, fixing it asap.
    if (!Instrumentation::Initialize()) {
        printf("[ReverseKit] Instrumentation::Initialize() failed\n");
    }
    */

    
    while (true) {
        std::cout << "retrieving...\n";
        GetImportsFromIAT();
        GetThreadInformation();
        Sleep(5000);
    }
    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) 
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        SetConsoleTitleA("ReverseKit Attached");
    	AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stderr);
        freopen("CONOUT$", "w", stdout);
        
        HookSyscalls();
        
        CreateThread(nullptr, 0, RetrievalThread, hModule, 0, nullptr);
        CreateThread(nullptr, 0, RenderThread, hModule, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        UnhookSyscalls();
        break;
    }

    return TRUE;
}
