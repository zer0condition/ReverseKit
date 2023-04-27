#pragma once

typedef BOOL(NTAPI* CheckRemoteDebuggerPresent_t)(HANDLE, PBOOL);

inline CheckRemoteDebuggerPresent_t oCheckRemoteDebuggerPresent;

inline BOOL NTAPI hkCheckRemoteDebuggerPresent(HANDLE hProcess, PBOOL pbDebuggerPresent)
{
    InterceptedCallInfo info;
    info.functionName = "CheckRemoteDebuggerPresent";
    info.additionalInfo = "pbDebuggerPresent returned false";

    interceptedCalls.push_back(info);

    *pbDebuggerPresent = FALSE;

    return TRUE;
}