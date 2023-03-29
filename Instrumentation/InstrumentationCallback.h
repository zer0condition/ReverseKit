//thanks to https://github.com/Deputation/instrumentation_callbacks
#pragma once

struct InstrumentationCallbackInfo {
    void* retValue;
    void* retAddr;
    std::string functionName;
    uint64_t offset;
};

std::vector<InstrumentationCallbackInfo> instrumentationCallbackInfo;

extern "C" void Bridge();
extern "C" void CallbackRoutine(CONTEXT * ctx);

namespace Instrumentation
{
    using BridgeFunction_t = void (*)();

    struct ProcessInstrumentationCallbackInfo_t {
        uint32_t Version;
        uint32_t Reserved;
        BridgeFunction_t Callback;
    };

    DWORD TlsIndex;

    void SymbolInitialize() {
        SymSetOptions(SYMOPT_UNDNAME);
        SymInitialize((HANDLE)-1, nullptr, true);
    }

    std::string ParseFunction(void* Address, uint64_t* Output) {
        auto Buffer = std::malloc(sizeof(SYMBOL_INFO) + MAX_SYM_NAME);

        if (!Buffer) {
            return "Couldn't retrieve function name.";
        }

        RtlZeroMemory(Buffer, sizeof(Buffer));

        auto symbol_information = (PSYMBOL_INFO)Buffer;
        symbol_information->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol_information->MaxNameLen = MAX_SYM_NAME;

        DWORD64 Displacement = 0;

        auto Result = SymFromAddr((HANDLE)-1, (DWORD64)Address, &Displacement, symbol_information);

        if (!Result) {
            std::free(Buffer);
            return "Couldn't retrieve function name.";
        }

        if (Output) {
            *Output = Displacement;
        }

        auto OutputString = std::string(symbol_information->Name);

        std::free(Buffer);

        return OutputString;
    }

    bool* GetThreadDataPointer() {
        void* ThreadData = TlsGetValue(TlsIndex);

        if (ThreadData == NULL) {
            ThreadData = reinterpret_cast<void*>(LocalAlloc(LPTR, 256));

            if (ThreadData == NULL) {
                return NULL;
            }

            RtlZeroMemory(ThreadData, 256);

            if (!TlsSetValue(TlsIndex, ThreadData)) {
                return NULL;
            }
        }

        return (bool*)ThreadData;
    }

    bool SetThreadHandlingSyscall(bool value) {
        if (auto DataPointer = GetThreadDataPointer()) {
            *DataPointer = value;
            return true;
        }

        return false;
    }

    bool IsThreadHandlingSyscall() {
        if (auto DataPointer = GetThreadDataPointer()) {
            return *DataPointer;
        }

        return false;
    }

    bool Initialize()
    {
        SymbolInitialize();

        using NtSetInformationProcess_t = NTSTATUS(NTAPI*)(HANDLE, PROCESS_INFORMATION_CLASS, PVOID, ULONG);

        auto NtSetInformationProcess = (NtSetInformationProcess_t)(GetProcAddress(LoadLibraryA("ntdll.dll"), "NtSetInformationProcess"));

        if (NtSetInformationProcess != NULL) {

            TlsIndex = TlsAlloc();

            if (TlsIndex == TLS_OUT_OF_INDEXES) {
                return false;
            }

            ProcessInstrumentationCallbackInfo_t CallbackInfo = { 0, 0, Bridge };

            NtSetInformationProcess(GetCurrentProcess(), (PROCESS_INFORMATION_CLASS)40, &CallbackInfo, sizeof(CallbackInfo));

            return true;
        }

        return false;
    }
}


void CallbackRoutine(CONTEXT* Ctx)
{
    uint64_t Teb = (uint64_t)NtCurrentTeb();

    Ctx->Rip = *(uint64_t*)((uint8_t*)Teb + 0x02d8 /* InstrumentationCallbackPreviousPc */);
    Ctx->Rsp = *(uint64_t*)((uint8_t*)Teb + 0x02e0 /* InstrumentationCallbackPreviousSp */);
    Ctx->Rcx = Ctx->R10;

    if (Instrumentation::IsThreadHandlingSyscall()) {
        RtlRestoreContext(Ctx, NULL);
    }

    if (!Instrumentation::SetThreadHandlingSyscall(TRUE)) {
        RtlRestoreContext(Ctx, NULL);
    }

    void* RetAddr = (void*)Ctx->Rip;
    void* RetValue = (void*)Ctx->Rax;

    uint64_t Offset = 0;

    std::string FunctionName = Instrumentation::ParseFunction(RetAddr, &Offset);

    InstrumentationCallbackInfo Temp;
    Temp.retValue = (void*)Ctx->Rax;
    Temp.retAddr = (void*)Ctx->Rip;
    Temp.functionName = Instrumentation::ParseFunction(RetAddr, &Offset);
    Temp.offset = Offset;
    instrumentationCallbackInfo.push_back(Temp);

    Instrumentation::SetThreadHandlingSyscall(FALSE);
    RtlRestoreContext(Ctx, NULL);
}