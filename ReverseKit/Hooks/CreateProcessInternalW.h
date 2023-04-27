#pragma once

typedef BOOL(NTAPI* CreateProcessInternalW_t)(
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

inline CreateProcessInternalW_t oCreateProcessInternalW;

inline BOOL NTAPI hkCreateProcessInternalW(
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
    InterceptedCallInfo Temp;

    Temp.functionName = "CreateProcessInternalW";
    Temp.additionalInfo = ws2s(lpCommandLine);

    interceptedCalls.push_back(Temp);

    ReverseHook::unhook(oCreateProcessInternalW, original_createprocess_bytes);

    const auto result = oCreateProcessInternalW(hUserToken, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation, hNewToken);

    ReverseHook::hook(oCreateProcessInternalW, hkCreateProcessInternalW, original_createprocess_bytes);

    return result;
}