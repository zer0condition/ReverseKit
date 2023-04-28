#pragma once

#include <Windows.h>
#include <dbghelp.h>
#include <string>
#include <vector>

struct FunctionCallInfo
{
    std::string function_name;
    ULONG_PTR return_address;
};

inline std::vector<FunctionCallInfo> function_calls;

extern "C" void InstrumentationCallbackThunk(void);
extern "C" void InstrumentationCallback(PCONTEXT ctx);

#define RIP_SANITY_CHECK(Rip, BaseAddress, ModuleSize) ((Rip > BaseAddress) && (Rip < (BaseAddress + ModuleSize)))
#define MAX_DLLS 1

struct ProcessInstrumentationCallbackInfo_t {
    ULONG Version;
    ULONG Reserved;
    PVOID Callback;
};

typedef struct _DLL_INFO {
    TCHAR name[MAX_PATH];
    ULONG_PTR baseAddress;
    DWORD size;
} DLL_INFO, * PDLL_INFO;

namespace Instrumentation
{
	void AddDllInfo(LPCTSTR lpDllName, ULONG_PTR baseAddress, DWORD size);
    PDLL_INFO GetDllInfo(ULONG_PTR Rip);
    void GetBaseAddresses();
    uintptr_t GetProcAddress(void* hModule, const char* wAPIName);
    bool Initialize();
}

void InstrumentationCallback(PCONTEXT ctx);