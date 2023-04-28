#pragma once
#include <d3d9.h>

inline LPDIRECT3D9             g_pD3D = nullptr;
inline LPDIRECT3DDEVICE9       g_pd3dDevice = nullptr;
inline D3DPRESENT_PARAMETERS   g_d3dpp;
inline HWND                    main_hwnd = nullptr;
inline WNDCLASSEX              wc;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

bool CreateD3DDevice();
void CleanupD3DDevice();
void CreateOverlayWindow();