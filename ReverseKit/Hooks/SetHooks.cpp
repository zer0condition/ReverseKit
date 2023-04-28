#include "SetHooks.h"

void SetHooks::HookSyscalls()
{
	oCreateProcessInternalW = (CreateProcessInternalW_t)GetProcAddress(GetModuleHandleA("kernelbase.dll"), "CreateProcessInternalW");
	if (oCreateProcessInternalW)
		ReverseHook::hook(oCreateProcessInternalW, hkCreateProcessInternalW, original_createprocess_bytes);

	oNtCreateThreadEx = (NtCreateThreadEx_t)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtCreateThreadEx");
	if (oNtCreateThreadEx)
		ReverseHook::hook(oNtCreateThreadEx, hkNtCreateThreadEx, original_createthread_bytes);

	oURLDownloadToFileA = (URLDownloadToFileA_t)GetProcAddress(GetModuleHandleA("urlmon.dll"), "URLDownloadToFileA");
	if (oURLDownloadToFileA)
		ReverseHook::hook(oURLDownloadToFileA, hkURLDownloadToFileA, original_urlmoniker_bytes);

	oInternetOpenUrlW = (InternetOpenUrlW_t)GetProcAddress(GetModuleHandleA("wininet.dll"), "InternetOpenUrlW");
	if (oInternetOpenUrlW)
		ReverseHook::hook(oInternetOpenUrlW, hkInternetOpenUrlW, original_openurl_bytes);

	oIsDebuggerPresent = (IsDebuggerPresent_t)GetProcAddress(GetModuleHandleA("kernel32.dll"), "IsDebuggerPresent");
	if (oIsDebuggerPresent)
		ReverseHook::hook(oIsDebuggerPresent, hkIsDebuggerPresent, original_isdebug_bytes);

	oCheckRemoteDebuggerPresent = (CheckRemoteDebuggerPresent_t)GetProcAddress(GetModuleHandleA("kernel32.dll"), "CheckRemoteDebuggerPresent");
	if (oCheckRemoteDebuggerPresent)
		ReverseHook::hook(oCheckRemoteDebuggerPresent, hkCheckRemoteDebuggerPresent, original_remotedebug_bytes);

	oRtlAdjustPrivilege = (RtlAdjustPrivilege_t)GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlAdjustPrivilege");
	if (oRtlAdjustPrivilege)
		ReverseHook::hook(oRtlAdjustPrivilege, hkRtlAdjustPrivilege, original_rtladjustprivilege_bytes);

	oRegOpenKeyExW = (RegOpenKeyExW_t)GetProcAddress(GetModuleHandleA("advapi32.dll"), "RegOpenKeyExW");
	if (oRegOpenKeyExW)
		ReverseHook::hook(oRegOpenKeyExW, hkRegOpenKeyExW, original_regopenkey_bytes);
}

void SetHooks::UnhookSyscalls()
{
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

	if (oRtlAdjustPrivilege)
		ReverseHook::unhook(oRtlAdjustPrivilege, original_rtladjustprivilege_bytes);

	if (oRegOpenKeyExW)
		ReverseHook::unhook(oRegOpenKeyExW, original_regopenkey_bytes);
}

BOOL NTAPI SetHooks::hkCheckRemoteDebuggerPresent(HANDLE hProcess, PBOOL pbDebuggerPresent)
{
	InterceptedCallInfo info;
	info.functionName = "CheckRemoteDebuggerPresent";
	info.additionalInfo = "pbDebuggerPresent returned false";

	interceptedCalls.push_back(info);

	*pbDebuggerPresent = FALSE;

	return TRUE;
}

BOOL NTAPI SetHooks::hkCreateProcessInternalW(
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

HINTERNET NTAPI SetHooks::hkInternetOpenUrlW(HINTERNET hInternet, LPCWSTR lpszUrl,
	LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext)
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

BOOL NTAPI SetHooks::hkIsDebuggerPresent(VOID)
{
	InterceptedCallInfo info;
	info.functionName = "IsDebuggerPresent";
	info.additionalInfo = "returned false";

	interceptedCalls.push_back(info);

	return FALSE;
}

NTSTATUS NTAPI SetHooks::hkNtCreateThreadEx(OUT PHANDLE ThreadHandle, IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes, IN HANDLE ProcessHandle,
	IN LPTHREAD_START_ROUTINE StartRoutine, IN LPVOID Argument,
	IN ULONG CreateFlags, IN SIZE_T StackZeroBits, IN SIZE_T SizeOfStackCommit,
	IN SIZE_T SizeOfStackReserve, OUT LPVOID BytesBuffer)
{
	InterceptedCallInfo Temp;

	Temp.functionName = "NtCreateThreadEx";

	char buffer[64];
	sprintf(buffer, "Created Thread: %p", StartRoutine);
	Temp.additionalInfo = buffer;

	interceptedCalls.push_back(Temp);

	ReverseHook::unhook(oNtCreateThreadEx, original_createthread_bytes);

	const auto result = oNtCreateThreadEx(ThreadHandle, DesiredAccess, ObjectAttributes, ProcessHandle, StartRoutine, Argument, CreateFlags, StackZeroBits, SizeOfStackCommit, SizeOfStackReserve, BytesBuffer);

	ReverseHook::hook(oNtCreateThreadEx, hkNtCreateThreadEx, original_createthread_bytes);

	return result;
}

LONG NTAPI SetHooks::hkRegOpenKeyExW(HKEY hKey, LPCWSTR lpSubKey,
	DWORD ulOptions, REGSAM samDesired, PHKEY phkResult)
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

NTSTATUS NTAPI SetHooks::hkRtlAdjustPrivilege(ULONG Privilege,
	BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled)
{
	/*
		Anti-bsod for common methods like:
		https://github.com/zer0condition/CritBSOD/blob/master/CritBSOD/CritBSOD.cpp
		https://stackoverflow.com/questions/7034592/create-bsod-from-user-mode
	*/

	// https://www.pinvoke.net/default.aspx/ntdll/RtlAdjustPrivilege.html
	if (Privilege == (int)19/*SeShutdownPrivilege*/ || Privilege == (int)20/*SeDebugPrivilege*/)
	{
		InterceptedCallInfo info;
		info.functionName = "RtlAdjustPrivilege";
		info.additionalInfo = "blocked";

		interceptedCalls.push_back(info);
	}

	return STATUS_SUCCESS;
}

HRESULT NTAPI SetHooks::hkURLDownloadToFileA(LPUNKNOWN pCaller, LPCSTR szURL,
	LPCSTR szFileName, DWORD dwReserved, LPBINDSTATUSCALLBACK lpfnCB)
{
	InterceptedCallInfo Temp;

	Temp.functionName = "URLDownloadToFileA";
	Temp.additionalInfo = szURL;

	interceptedCalls.push_back(Temp);

	ReverseHook::unhook(oURLDownloadToFileA, original_urlmoniker_bytes);

	const auto result = oURLDownloadToFileA(pCaller, szURL, szFileName, dwReserved, lpfnCB);

	ReverseHook::hook(oURLDownloadToFileA, hkURLDownloadToFileA, original_urlmoniker_bytes);

	return result;
}