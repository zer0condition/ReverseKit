#pragma once

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
