#pragma once

typedef HINTERNET(WINAPI* InternetOpenUrlW_t)(HINTERNET hInternet, LPCWSTR lpszUrl, LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext);

InternetOpenUrlW_t oInternetOpenUrlW;

HINTERNET WINAPI hkInternetOpenUrlW(HINTERNET hInternet, LPCWSTR lpszUrl, LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext)
{
    InterceptedCallInfo Temp;

    Temp.functionName = "InternetOpenUrlW";
    Temp.additionalInfo = ws2s(lpszUrl).c_str();

    interceptedCalls.push_back(Temp);

    ReverseHook::unhook(oInternetOpenUrlW, original_openurl_bytes);

    auto result = oInternetOpenUrlW(hInternet, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext);

    ReverseHook::hook(oInternetOpenUrlW, hkInternetOpenUrlW, original_openurl_bytes);

    return result;
}