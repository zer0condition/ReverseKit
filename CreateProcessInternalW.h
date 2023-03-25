#pragma once

typedef BOOL(WINAPI* CreateProcessInternalW_t)(
    HANDLE hUserToken,
    LPCWSTR lpApplicationName,
    LPWSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL bInheritHandles,
    DWORD dwCreationFlags,
    LPVOID lpEnvironment,
    LPCWSTR lpCurrentDirectory,
    LPSTARTUPINFOW lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation,
    PHANDLE hNewToken);

CreateProcessInternalW_t oCreateProcessInternalW;

BOOL WINAPI hkCreateProcessInternalW(
    HANDLE hUserToken,
    LPCWSTR lpApplicationName,
    LPWSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL bInheritHandles,
    DWORD dwCreationFlags,
    LPVOID lpEnvironment,
    LPCWSTR lpCurrentDirectory,
    LPSTARTUPINFOW lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation,
    PHANDLE hNewToken)
{

    char message[256];
    sprintf(message, "Block Commands Intercepted: %ls?", lpCommandLine);

    int result = MessageBoxA(NULL, message, "[ReverseKit] CreateProcessInternalW", MB_YESNO);

    if (result == IDYES) {
        printf("[ReverseKit] Blocked Commands: %ls\n", lpCommandLine);
        return TRUE;
    }

    printf("[ReverseKit] Accepted Commands: %ls\n", lpCommandLine);

    return oCreateProcessInternalW(hUserToken, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation, hNewToken);
}