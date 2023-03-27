#pragma once

typedef HRESULT(WINAPI* URLDownloadToFileA_t)(LPUNKNOWN, LPCSTR, LPCSTR, DWORD, LPBINDSTATUSCALLBACK);

URLDownloadToFileA_t oURLDownloadToFileA;

HRESULT WINAPI hkURLDownloadToFileA(LPUNKNOWN pCaller, LPCSTR szURL, LPCSTR szFileName, DWORD dwReserved, LPBINDSTATUSCALLBACK lpfnCB)
{
    InterceptedCallInfo Temp;

    Temp.functionName = "URLDownloadToFileA";
    Temp.additionalInfo = szURL;

    interceptedCalls.push_back(Temp);

    ReverseHook::unhook(oURLDownloadToFileA, original_urlmoniker_bytes);

    auto result = oURLDownloadToFileA(pCaller, szURL, szFileName, dwReserved, lpfnCB);

    ReverseHook::hook(oURLDownloadToFileA, hkURLDownloadToFileA, original_urlmoniker_bytes);

    return result;

}