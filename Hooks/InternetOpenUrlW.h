#pragma once

typedef HINTERNET(WINAPI* InternetOpenUrlW_t)(HINTERNET hInternet, LPCWSTR lpszUrl, LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext);

InternetOpenUrlW_t oInternetOpenUrlW;

HINTERNET WINAPI hkInternetOpenUrlW(HINTERNET hInternet, LPCWSTR lpszUrl, LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext)
{
    wprintf(L"[ReverseKit] InternetOpenUrlW URL: %s\n", lpszUrl);

    return (InternetOpenUrlW_t)oInternetOpenUrlW(hInternet, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext);
}
