#pragma once
#include <Includes.h>

struct InstrumentationCallbackInfo {
    void* retValue;
    void* retAddr;
    std::string functionName;
    uint64_t offset;
};

extern std::vector<InstrumentationCallbackInfo> instrumentationCallbackInfo;

extern "C" void Bridge();
extern "C" void CallbackRoutine(CONTEXT * ctx);

namespace Instrumentation
{
	void SymbolInitialize();

    std::string ParseFunction(void* Address, uint64_t* Output);

    bool SetThreadHandlingSyscall(bool value);

    bool IsThreadHandlingSyscall();

    bool Initialize();
}

void CallbackRoutine(CONTEXT* Ctx);