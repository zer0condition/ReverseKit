#pragma once


typedef LONG(NTAPI* RegOpenKeyExW_t)(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);
inline RegOpenKeyExW_t oRegOpenKeyExW;


inline LONG NTAPI hkRegOpenKeyExW(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult)
{
    InterceptedCallInfo Temp;
    Temp.functionName = "RegOpenKeyExW";

    const std::string narrowSubKey = ws2s(lpSubKey);

    Temp.additionalInfo = narrowSubKey;

    interceptedCalls.push_back(Temp);

    ReverseHook::unhook(oRegOpenKeyExW, original_regopenkey_bytes);

    const auto result = oRegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);

    ReverseHook::hook(oRegOpenKeyExW, hkRegOpenKeyExW, original_regopenkey_bytes);

    return result;
}