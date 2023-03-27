#pragma once

typedef BOOL(WINAPI* CheckRemoteDebuggerPresent_t)(HANDLE, PBOOL);

CheckRemoteDebuggerPresent_t oCheckRemoteDebuggerPresent;

BOOL WINAPI hkCheckRemoteDebuggerPresent(HANDLE hProcess, PBOOL pbDebuggerPresent)
{
    InterceptedCallInfo info;
    info.functionName = "CheckRemoteDebuggerPresent";
    info.additionalInfo = "pbDebuggerPresent returned false";

    interceptedCalls.push_back(info);

    *pbDebuggerPresent = FALSE;

    return TRUE;
}