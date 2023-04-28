#pragma once
#include <string>
#include <vector>

struct ImportInfo
{
    std::string dllName;
    std::string functionName;
    void* functionAddress;
};

inline std::vector<ImportInfo> imports = {};

void GetImportsFromIAT();