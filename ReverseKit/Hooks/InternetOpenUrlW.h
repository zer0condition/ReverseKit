#pragma once

typedef HINTERNET(NTAPI* InternetOpenUrlW_t)(HINTERNET hInternet, LPCWSTR lpszUrl, LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext);

InternetOpenUrlW_t oInternetOpenUrlW;

inline HINTERNET NTAPI hkInternetOpenUrlW(HINTERNET hInternet, LPCWSTR lpszUrl, LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext)
{
    InterceptedCallInfo Temp;

    Temp.functionName = "InternetOpenUrlW";
    Temp.additionalInfo = ws2s(lpszUrl);

    interceptedCalls.push_back(Temp);

    ReverseHook::unhook(oInternetOpenUrlW, original_openurl_bytes);

    const auto result = oInternetOpenUrlW(hInternet, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext);

    ReverseHook::hook(oInternetOpenUrlW, hkInternetOpenUrlW, original_openurl_bytes);

    return result;
}