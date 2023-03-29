#include "SetHooks.h"








std::string ws2s(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

unsigned char original_createprocess_bytes[14];
unsigned char original_createthread_bytes[14];
unsigned char original_urlmoniker_bytes[14];
unsigned char original_openurl_bytes[14];
unsigned char original_isdebug_bytes[14];
unsigned char original_remotedebug_bytes[14];



#include "ReverseHook.h"

std::vector<InterceptedCallInfo> interceptedCalls;

#include "CreateProcessInternalW.h"
#include "NtCreateThreadEx.h"

#include "URLDownloadToFileA.h"
#include "InternetOpenUrlW.h"

#include "IsDebuggerPresent.h"
#include "CheckRemoteDebuggerPresent.h"




void HookSyscalls() {
    

    oCreateProcessInternalW = reinterpret_cast<CreateProcessInternalW_t>(GetProcAddress(GetModuleHandleA("kernelbase.dll"), "CreateProcessInternalW"));
    if (oCreateProcessInternalW)
        ReverseHook::hook(oCreateProcessInternalW, hkCreateProcessInternalW, original_createprocess_bytes);

    oNtCreateThreadEx = reinterpret_cast<NtCreateThreadEx_t>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtCreateThreadEx"));
    if (oNtCreateThreadEx)
        ReverseHook::hook(oNtCreateThreadEx, hkNtCreateThreadEx, original_createthread_bytes);

    oURLDownloadToFileA = reinterpret_cast<URLDownloadToFileA_t>(GetProcAddress(GetModuleHandleA("urlmon.dll"), "URLDownloadToFileA"));
    if (oURLDownloadToFileA)
        ReverseHook::hook(oURLDownloadToFileA, hkURLDownloadToFileA, original_urlmoniker_bytes);

    oInternetOpenUrlW = reinterpret_cast<InternetOpenUrlW_t>(GetProcAddress(GetModuleHandleA("wininet.dll"), "InternetOpenUrlW"));
    if (oInternetOpenUrlW)
        ReverseHook::hook(oInternetOpenUrlW, hkInternetOpenUrlW, original_openurl_bytes);

    oIsDebuggerPresent = reinterpret_cast<IsDebuggerPresent_t>(GetProcAddress(GetModuleHandleA("kernel32.dll"), "IsDebuggerPresent"));
    if (oIsDebuggerPresent)
        ReverseHook::hook(oIsDebuggerPresent, hkIsDebuggerPresent, original_isdebug_bytes);

    oCheckRemoteDebuggerPresent = reinterpret_cast<CheckRemoteDebuggerPresent_t>(GetProcAddress(GetModuleHandleA("kernel32.dll"),
	    "CheckRemoteDebuggerPresent"));
    if (oCheckRemoteDebuggerPresent)
        ReverseHook::hook(oCheckRemoteDebuggerPresent, hkCheckRemoteDebuggerPresent, original_remotedebug_bytes);

    std::cout << "hooked!";
}

void UnhookSyscalls() {
    if (oCreateProcessInternalW)
        ReverseHook::unhook(oCreateProcessInternalW, original_createprocess_bytes);

    if (oNtCreateThreadEx)
        ReverseHook::unhook(oNtCreateThreadEx, original_createthread_bytes);

    if (oURLDownloadToFileA)
        ReverseHook::unhook(oURLDownloadToFileA, original_urlmoniker_bytes);

    if (oInternetOpenUrlW)
        ReverseHook::unhook(oInternetOpenUrlW, original_openurl_bytes);

    if (oIsDebuggerPresent)
        ReverseHook::unhook(oIsDebuggerPresent, original_isdebug_bytes);

    if (oCheckRemoteDebuggerPresent)
        ReverseHook::unhook(oCheckRemoteDebuggerPresent, original_remotedebug_bytes);

    std::cout << "Unhooked!";
    Sleep(1000);
}