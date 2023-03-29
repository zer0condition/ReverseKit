#pragma once



typedef BOOL(WINAPI* IsDebuggerPresent_t)(VOID);

IsDebuggerPresent_t oIsDebuggerPresent;

BOOL WINAPI hkIsDebuggerPresent(VOID)
{
    InterceptedCallInfo info;
    info.functionName = "IsDebuggerPresent";
    info.additionalInfo = "returned false";

    interceptedCalls.push_back(info);

    return FALSE;
}