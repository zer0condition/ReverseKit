#pragma once

#include <Windows.h>
#include "../ImGui/imgui.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg,
    WPARAM wParam, LPARAM lParam);

void ResetD3DDevice();

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);