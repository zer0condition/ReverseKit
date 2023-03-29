#pragma once

#include <Includes.h>

struct InterceptedCallInfo {
    std::string functionName;
    std::string additionalInfo;
};

extern std::vector<InterceptedCallInfo> interceptedCalls;

void HookSyscalls();

void UnhookSyscalls();