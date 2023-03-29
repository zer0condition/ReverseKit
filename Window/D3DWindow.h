#pragma once
#include <Includes.h>

class D3DWindow
{
public:

	LPDIRECT3D9             g_pD3D = nullptr;
	LPDIRECT3DDEVICE9       g_pd3dDevice = nullptr;
	D3DPRESENT_PARAMETERS   g_d3dpp;
	HWND                    main_hwnd = nullptr;
	WNDCLASSEX              wc;

	D3DWindow();

	bool CreateD3DDevice();

	void CleanupD3DDevice();

	void CreateOverlayWindow();
};




