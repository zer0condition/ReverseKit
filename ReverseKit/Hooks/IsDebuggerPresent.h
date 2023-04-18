#pragma once
typedef BOOL(NTAPI* IsDebuggerPresent_t)(VOID);

IsDebuggerPresent_t oIsDebuggerPresent;

BOOL NTAPI hkIsDebuggerPresent(VOID)
{
    InterceptedCallInfo info;
    info.functionName = "IsDebuggerPresent";
    info.additionalInfo = "returned false";

    interceptedCalls.push_back(info);

    return FALSE;
}