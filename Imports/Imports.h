#pragma once
#include <Includes.h>

struct ImportInfo {
    std::string dllName;
    std::string functionName;
    void* functionAddress;
};

extern std::vector<ImportInfo> imports;

void GetImportsFromIAT();

