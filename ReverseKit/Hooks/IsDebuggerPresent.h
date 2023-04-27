#pragma once
typedef BOOL(NTAPI* IsDebuggerPresent_t)(VOID);

inline IsDebuggerPresent_t oIsDebuggerPresent;

inline BOOL NTAPI hkIsDebuggerPresent(VOID)
{
    InterceptedCallInfo info;
    info.functionName = "IsDebuggerPresent";
    info.additionalInfo = "returned false";

    interceptedCalls.push_back(info);

    return FALSE;
}