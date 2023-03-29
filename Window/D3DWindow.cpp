#include "D3DWindow.h"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

bool D3DWindow::CreateD3DDevice() {
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, main_hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}
void D3DWindow::CleanupD3DDevice()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
    UnregisterClass(wc.lpszClassName, wc.hInstance);
}

void D3DWindow::CreateOverlayWindow()
{
    RegisterClassEx(&wc);
    main_hwnd = CreateWindow(wc.lpszClassName, L"ReverseKit", WS_POPUP, 0, 0, 5, 5, NULL, NULL, wc.hInstance, NULL);

    if (!CreateD3DDevice()) {
        CleanupD3DDevice();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return;
    }
    ShowWindow(main_hwnd, SW_HIDE);
    UpdateWindow(main_hwnd);
}

D3DWindow::D3DWindow()
{
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
	wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL,L"ReverseKit", NULL };

}
