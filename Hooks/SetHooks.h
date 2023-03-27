#pragma once
#include "../ReverseLib/ReverseHook.h"

#include <unordered_map>
#include <codecvt>
#include <locale>

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

#include "CreateProcessInternalW.h"
#include "NtCreateThreadEx.h"

#include "URLDownloadToFileA.h"
#include "InternetOpenUrlW.h"

#include "IsDebuggerPresent.h"
#include "CheckRemoteDebuggerPresent.h"

void HookSyscalls() {
    oCreateProcessInternalW = (CreateProcessInternalW_t)GetProcAddress(GetModuleHandleA("kernelbase.dll"), "CreateProcessInternalW");
    if (oCreateProcessInternalW != NULL) {
        ReverseHook::hook(oCreateProcessInternalW, hkCreateProcessInternalW, original_createprocess_bytes);
    }
    
    oNtCreateThreadEx = (NtCreateThreadEx_t)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtCreateThreadEx");
    if (oNtCreateThreadEx != NULL) {
        ReverseHook::hook(oNtCreateThreadEx, hkNtCreateThreadEx, original_createthread_bytes);
    }

    oURLDownloadToFileA = (URLDownloadToFileA_t)GetProcAddress(GetModuleHandleA("urlmon.dll"), "URLDownloadToFileA");
    if (oURLDownloadToFileA != NULL) {
        ReverseHook::hook(oURLDownloadToFileA, hkURLDownloadToFileA, original_urlmoniker_bytes);
    }

    oInternetOpenUrlW = (InternetOpenUrlW_t)GetProcAddress(GetModuleHandleA("wininet.dll"), "InternetOpenUrlW");
    if (oInternetOpenUrlW != NULL) {
        ReverseHook::hook(oInternetOpenUrlW, hkInternetOpenUrlW, original_openurl_bytes);
    }

    oIsDebuggerPresent = (IsDebuggerPresent_t)GetProcAddress(GetModuleHandleA("kernel32.dll"), "IsDebuggerPresent");
    if (oIsDebuggerPresent != NULL) {
        ReverseHook::hook(oIsDebuggerPresent, hkIsDebuggerPresent, original_isdebug_bytes);
    }

    oCheckRemoteDebuggerPresent = (CheckRemoteDebuggerPresent_t)GetProcAddress(GetModuleHandleA("kernel32.dll"), "CheckRemoteDebuggerPresent");
    if (oCheckRemoteDebuggerPresent != NULL) {
        ReverseHook::hook(oCheckRemoteDebuggerPresent, hkCheckRemoteDebuggerPresent, original_remotedebug_bytes);
    }
}

void UnhookSyscalls() {
    ReverseHook::Trampoline::unhook(oCreateProcessInternalW, original_createprocess_bytes);
    ReverseHook::Trampoline::unhook(oNtCreateThreadEx, original_createthread_bytes);
    ReverseHook::Trampoline::unhook(oURLDownloadToFileA, original_urlmoniker_bytes);
    ReverseHook::Trampoline::unhook(oInternetOpenUrlW, original_openurl_bytes);
    ReverseHook::Trampoline::unhook(oIsDebuggerPresent, original_isdebug_bytes);
    ReverseHook::Trampoline::unhook(oCheckRemoteDebuggerPresent, original_remotedebug_bytes);
}