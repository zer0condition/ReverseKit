#pragma once


typedef HRESULT(WINAPI* URLDownloadToFileA_t)(LPUNKNOWN, LPCSTR, LPCSTR, DWORD, LPBINDSTATUSCALLBACK);

URLDownloadToFileA_t oURLDownloadToFileA;

HRESULT WINAPI hkURLDownloadToFileA(LPUNKNOWN pCaller, LPCSTR szURL, LPCSTR szFileName, DWORD dwReserved, LPBINDSTATUSCALLBACK lpfnCB)
{
    printf("[ReverseKit] URLDownloadToFileA URL: %s\n", szURL);

    return oURLDownloadToFileA(pCaller, szURL, szFileName, dwReserved, lpfnCB);
}