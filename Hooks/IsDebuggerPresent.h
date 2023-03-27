#pragma once
typedef BOOL(WINAPI* IsDebuggerPresent_t)(VOID);

IsDebuggerPresent_t oIsDebuggerPresent;

BOOL WINAPI hkIsDebuggerPresent(VOID)
{
    printf("[ReverseKit] IsDebuggerPresent called\n");

    return FALSE;
}