#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <winternl.h>
#include <ntstatus.h>
#include <wininet.h>
#include <string>
#include <vector>

#include <d3d9.h>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx9.h"

#pragma comment(lib, "d3dx9")
#pragma comment(lib,"d3d9.lib")

LPDIRECT3D9             g_pD3D = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;
D3DPRESENT_PARAMETERS   g_d3dpp;
HWND                    main_hwnd = NULL;
WNDCLASSEX              wc;

#include "Window/D3DWindow.h"
#include "Window/ImGuiSetup.h"

struct InterceptedCallInfo {
    std::string functionName;
    std::string additionalInfo;
};

std::vector<InterceptedCallInfo> interceptedCalls;

#include "Imports/Imports.h"
#include "Threads/Threads.h"
#include "Hooks/SetHooks.h"
#include "Menu/Menu.h"

DWORD WINAPI RenderThread(LPVOID lpParameter)
{
    CreateOverlayWindow();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(main_hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (msg.message != WM_QUIT) {

        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        DrawImports();
        DrawThreadInformation();
        DrawHookedFunctions(); 

        ImGui::EndFrame();
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
            ResetD3DDevice();
        }

    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupD3DDevice();

    return 0;
}

DWORD WINAPI RetrievalThread(LPVOID lpParameter)
{
    for (;;) {
        GetImportsFromIAT();
        GetThreadInformation();
        Sleep(5000);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        SetConsoleTitleA("ReverseKit Attached");

        HookSyscalls();

        CloseHandle(CreateThread(NULL, 0, RetrievalThread, NULL, 0, NULL));
        CloseHandle(CreateThread(NULL, 0, RenderThread, NULL, 0, NULL));

        break;

    case DLL_PROCESS_DETACH:
        UnhookSyscalls();
        break;
    }
    return TRUE;
}
