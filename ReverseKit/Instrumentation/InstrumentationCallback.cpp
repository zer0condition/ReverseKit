#include "InstrumentationCallback.h"

static DLL_INFO dlls[MAX_DLLS];
static DWORD numDlls = 0;

void Instrumentation::AddDllInfo(LPCTSTR lpDllName, ULONG_PTR baseAddress, DWORD size)
{
	if (numDlls >= MAX_DLLS)
		return;

	lstrcpy(dlls[numDlls].name, lpDllName);
	dlls[numDlls].baseAddress = baseAddress;
	dlls[numDlls].size = size;
	numDlls++;
}

PDLL_INFO Instrumentation::GetDllInfo(ULONG_PTR Rip)
{
	for (DWORD i = 0; i < numDlls; i++)
	{
		if (RIP_SANITY_CHECK(Rip, dlls[i].baseAddress, dlls[i].size))
			return &dlls[i];
	}

	return NULL;
}

void Instrumentation::GetBaseAddresses()
{
	HMODULE hNtDll = GetModuleHandleA("ntdll.dll");
	if (hNtDll) {
		PIMAGE_DOS_HEADER piDH = (PIMAGE_DOS_HEADER)hNtDll;
		PIMAGE_NT_HEADERS piNH = (PIMAGE_NT_HEADERS)((ULONG_PTR)hNtDll + piDH->e_lfanew);
		AddDllInfo(TEXT("ntdll.dll"), (ULONG_PTR)hNtDll, piNH->OptionalHeader.SizeOfImage);
	}

	/*
	HMODULE hWin32u = GetModuleHandleA("win32u.dll");
	if (hWin32u) {
		PIMAGE_DOS_HEADER piDH = (PIMAGE_DOS_HEADER)hWin32u;
		PIMAGE_NT_HEADERS piNH = (PIMAGE_NT_HEADERS)((ULONG_PTR)hWin32u + piDH->e_lfanew);
		AddDllInfo(TEXT("win32u.dll"), (ULONG_PTR)hWin32u, piNH->OptionalHeader.SizeOfImage);
	}


	HMODULE hKernelBase = GetModuleHandleA("kernelbase.dll");
	if (hKernelBase) {
		PIMAGE_DOS_HEADER piDH = (PIMAGE_DOS_HEADER)hKernelBase;
		PIMAGE_NT_HEADERS piNH = (PIMAGE_NT_HEADERS)((ULONG_PTR)hKernelBase + piDH->e_lfanew);
		AddDllInfo(TEXT("kernelbase.dll"), (ULONG_PTR)hKernelBase, piNH->OptionalHeader.SizeOfImage);
	}*/
}

uintptr_t Instrumentation::GetProcAddress(void* hModule, const char* wAPIName)
{
	unsigned char* lpBase = reinterpret_cast<unsigned char*>(hModule);
	const IMAGE_DOS_HEADER* idhDosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(lpBase);
	if (idhDosHeader->e_magic == IMAGE_DOS_SIGNATURE)
	{
		const IMAGE_NT_HEADERS64* inhNtHeader = reinterpret_cast<IMAGE_NT_HEADERS64*>(
			lpBase + idhDosHeader->e_lfanew);

		if (inhNtHeader->Signature == IMAGE_NT_SIGNATURE)
		{
			const IMAGE_EXPORT_DIRECTORY* iedExportDirectory
				= reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(
					lpBase + inhNtHeader->OptionalHeader.DataDirectory[
						IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

			for (register unsigned int uiIter = 0; uiIter < iedExportDirectory->NumberOfNames;
				++uiIter)
			{
				const char* szNames = reinterpret_cast<char*>(lpBase + reinterpret_cast<unsigned long*>(lpBase + iedExportDirectory->AddressOfNames)[uiIter]);
				if (!strcmp(szNames, wAPIName))
				{
					const unsigned short usOrdinal = reinterpret_cast<unsigned short*>(lpBase + iedExportDirectory->AddressOfNameOrdinals)[uiIter];
					return reinterpret_cast<uintptr_t>(lpBase + reinterpret_cast<unsigned long*>(lpBase + iedExportDirectory->AddressOfFunctions)[usOrdinal]);
				}
			}
		}
	}

	return 0;
}

bool Instrumentation::Initialize()
{
	GetBaseAddresses();
	SymSetOptions(SYMOPT_UNDNAME);
	SymInitialize(GetCurrentProcess(), nullptr, TRUE);

	using NtSetInformationProcess_t = NTSTATUS(NTAPI*)(HANDLE, PROCESS_INFORMATION_CLASS, PVOID, ULONG);
	const NtSetInformationProcess_t NtSetInformationProcess = (NtSetInformationProcess_t)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtSetInformationProcess");

	if (NtSetInformationProcess)
	{
		ProcessInstrumentationCallbackInfo_t CallbackInfo = { 0, 0,
			(PVOID)(ULONG_PTR)InstrumentationCallbackThunk };

		NtSetInformationProcess(GetCurrentProcess(), (PROCESS_INFORMATION_CLASS)40,
			&CallbackInfo, sizeof(CallbackInfo));

		return true;
	}

	return false;
}

void InstrumentationCallback(PCONTEXT ctx)
{
	const ULONG_PTR pTEB = (ULONG_PTR)NtCurrentTeb();
    if (!pTEB) {
        RtlRestoreContext(ctx, nullptr);
    }

    ctx->Rip = *((ULONG_PTR*)(pTEB + 0x02D8)); // TEB->InstrumentationCallbackPreviousPc
    ctx->Rsp = *((ULONG_PTR*)(pTEB + 0x02E0)); // TEB->InstrumentationCallbackPreviousSp
    ctx->Rcx = ctx->R10;

	const BOOLEAN bInstrumentationCallbackDisabled = *((BOOLEAN*)pTEB + 0x1b8);

    if (!bInstrumentationCallbackDisabled)
    {
        *((BOOLEAN*)pTEB + 0x1b8) = TRUE;

        if (GetAsyncKeyState(VK_F1))
        {
	        const PDLL_INFO pDllInfo = Instrumentation::GetDllInfo(ctx->Rip);

            if (pDllInfo)
            {
                if (!pDllInfo->baseAddress) {
                    RtlRestoreContext(ctx, nullptr);
                }

                const auto SymbolBuffer = malloc(sizeof(SYMBOL_INFO) + MAX_SYM_NAME);
                if (!SymbolBuffer) {
                    RtlRestoreContext(ctx, nullptr);
                }

                RtlSecureZeroMemory(SymbolBuffer, sizeof(SYMBOL_INFO) + MAX_SYM_NAME);

                const PSYMBOL_INFO SymbolInfo = (PSYMBOL_INFO)SymbolBuffer;
                SymbolInfo->SizeOfStruct = sizeof(SYMBOL_INFO);
                SymbolInfo->MaxNameLen = MAX_SYM_NAME;

                DWORD64 Displacement;
                const BOOL SymbolLookupResult = SymFromAddr(GetCurrentProcess(), ctx->Rip, &Displacement, SymbolInfo);

                if (!SymbolLookupResult || !SymbolInfo) {
                    free(SymbolBuffer);
                    RtlRestoreContext(ctx, nullptr);
                }

                const uintptr_t pFunction = Instrumentation::GetProcAddress((PVOID)pDllInfo->baseAddress, SymbolInfo->Name);
				const ULONG_PTR ReturnAddress = *((ULONG_PTR*)(ctx->Rsp));

                if (!pFunction || !ReturnAddress) {
                    free(SymbolBuffer);
                    RtlRestoreContext(ctx, nullptr);
                }

                FunctionCallInfo call_info;
                call_info.function_name = SymbolInfo->Name;
                call_info.return_address = ReturnAddress;
                function_calls.push_back(call_info);

                free(SymbolBuffer);
            }
        }

        *((BOOLEAN*)pTEB + 0x1b8) = FALSE;
    }
    RtlRestoreContext(ctx, nullptr);
}