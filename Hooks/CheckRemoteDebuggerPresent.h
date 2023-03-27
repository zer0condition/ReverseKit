#pragma once

typedef BOOL(WINAPI* CheckRemoteDebuggerPresent_t)(HANDLE, PBOOL);

CheckRemoteDebuggerPresent_t oCheckRemoteDebuggerPresent;

BOOL WINAPI hkCheckRemoteDebuggerPresent(HANDLE hProcess, PBOOL pbDebuggerPresent)
{
    printf("[ReverseKit] CheckRemoteDebuggerPresent called\n");

    *pbDebuggerPresent = FALSE;
    return TRUE;
}