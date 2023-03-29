#include "Imports.h"

std::vector<ImportInfo> imports;

void GetImportsFromIAT()
{
    auto hModule = GetModuleHandle(NULL);

    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hModule;
    PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((BYTE*)hModule + pDosHeader->e_lfanew);
    PIMAGE_IMPORT_DESCRIPTOR  pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((BYTE*)hModule + pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    while (pImportDesc->Name)
    {
        const char* dllName = (const char*)((BYTE*)hModule + pImportDesc->Name);
        PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)((BYTE*)hModule + pImportDesc->FirstThunk);
        PIMAGE_THUNK_DATA pThunkOrig = (PIMAGE_THUNK_DATA)((BYTE*)hModule + pImportDesc->OriginalFirstThunk);
        while (pThunkOrig->u1.AddressOfData)
        {
            ImportInfo info;
            info.dllName = dllName;
            if (pThunkOrig->u1.Ordinal & IMAGE_ORDINAL_FLAG)
            {
                info.functionName = std::to_string(IMAGE_ORDINAL(pThunkOrig->u1.Ordinal));
                info.functionAddress = (void*)pThunk->u1.Function;
            }
            else
            {
                PIMAGE_IMPORT_BY_NAME pImportByName = (PIMAGE_IMPORT_BY_NAME)((BYTE*)hModule + pThunkOrig->u1.AddressOfData);
                info.functionName = (const char*)pImportByName->Name;
                info.functionAddress = (void*)pThunk->u1.Function;
            }

            bool alreadyExists = false;
            for (const auto& imp : imports) {
                if (imp.dllName == info.dllName && imp.functionName == info.functionName) {
                    alreadyExists = true;
                    break;
                }
            }

            if (!alreadyExists) {
                imports.push_back(info);
            }

            pThunk++;
            pThunkOrig++;
        }

        pImportDesc++;
    }
}