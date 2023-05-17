#pragma once
#include "../ReverseLib/ReverseHook.h"
#include "../ReverseLib/Includes.h"

#include <unordered_map>
#include <codecvt>
#include <locale>

inline std::string ws2s(const std::wstring& wstr) {
	const std::wstring_view wsv(wstr);
	std::string result;
	std::mbstate_t state = std::mbstate_t();
	const wchar_t* pwcs = wsv.data();
	const size_t len = std::wcsrtombs(nullptr, &pwcs, 0, &state);
	if (len != static_cast<size_t>(-1)) {
		result.resize(len);
		std::wcsrtombs(result.data(), &pwcs, len, &state);
	}
	return result;
}

inline unsigned char original_createprocess_bytes[14];
inline unsigned char original_createthread_bytes[14];
inline unsigned char original_urlmoniker_bytes[14];
inline unsigned char original_openurl_bytes[14];
inline unsigned char original_isdebug_bytes[14];
inline unsigned char original_remotedebug_bytes[14];
inline unsigned char original_rtladjustprivilege_bytes[14];
inline unsigned char original_regopenkey_bytes[14];
inline unsigned char original_writeprocessmemory_bytes[14];
inline unsigned char original_system_bytes[14];
inline unsigned char original_wsystem_bytes[14];

struct InterceptedCallInfo {
	std::string functionName;
	std::string additionalInfo;
};

namespace SetHooks
{
	inline std::vector<InterceptedCallInfo> interceptedCalls = {};

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

	typedef BOOL(NTAPI* CheckRemoteDebuggerPresent_t)(HANDLE, PBOOL);

	typedef HINTERNET(NTAPI* InternetOpenUrlW_t)(HINTERNET hInternet, LPCWSTR lpszUrl,
		LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext);

	typedef BOOL(NTAPI* IsDebuggerPresent_t)(VOID);

	typedef NTSTATUS(NTAPI* NtCreateThreadEx_t)(OUT PHANDLE ThreadHandle, IN ACCESS_MASK DesiredAccess,
		IN POBJECT_ATTRIBUTES ObjectAttributes, IN HANDLE ProcessHandle,
		IN LPTHREAD_START_ROUTINE StartRoutine, IN LPVOID Argument,
		IN ULONG CreateFlags, IN SIZE_T StackZeroBits, IN SIZE_T SizeOfStackCommit,
		IN SIZE_T SizeOfStackReserve, OUT LPVOID BytesBuffer);

	typedef LONG(NTAPI* RegOpenKeyExW_t)(HKEY hKey, LPCWSTR lpSubKey,
		DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);

	typedef NTSTATUS(NTAPI* RtlAdjustPrivilege_t)(ULONG Privilege, BOOLEAN Enable,
		BOOLEAN CurrentThread, PBOOLEAN Enabled);

	typedef HRESULT(NTAPI* URLDownloadToFileA_t)(LPUNKNOWN, LPCSTR, LPCSTR, DWORD,
		LPBINDSTATUSCALLBACK);

	typedef BOOL(NTAPI* WriteProcessMemory_t)(HANDLE, LPVOID, LPCVOID, SIZE_T, SIZE_T*);

	typedef int(NTAPI* System_t)(const char*);
	typedef int(NTAPI* WSystem_t)(const wchar_t*);

	inline URLDownloadToFileA_t oURLDownloadToFileA;
	inline RtlAdjustPrivilege_t oRtlAdjustPrivilege;
	inline RegOpenKeyExW_t oRegOpenKeyExW;
	inline NtCreateThreadEx_t oNtCreateThreadEx;
	inline IsDebuggerPresent_t oIsDebuggerPresent;
	inline InternetOpenUrlW_t oInternetOpenUrlW;
	inline CheckRemoteDebuggerPresent_t oCheckRemoteDebuggerPresent;
	inline CreateProcessInternalW_t oCreateProcessInternalW;
	inline WriteProcessMemory_t oWriteProcessMemory;
	inline System_t oSystem;
	inline WSystem_t oWSystem;

	BOOL NTAPI hkCreateProcessInternalW(
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

	BOOL NTAPI hkCheckRemoteDebuggerPresent(HANDLE hProcess, PBOOL pbDebuggerPresent);

	HINTERNET NTAPI hkInternetOpenUrlW(HINTERNET hInternet, LPCWSTR lpszUrl,
		LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext);

	BOOL NTAPI hkIsDebuggerPresent(VOID);

	NTSTATUS NTAPI hkNtCreateThreadEx(OUT PHANDLE ThreadHandle, IN ACCESS_MASK DesiredAccess,
		IN POBJECT_ATTRIBUTES ObjectAttributes, IN HANDLE ProcessHandle,
		IN LPTHREAD_START_ROUTINE StartRoutine, IN LPVOID Argument,
		IN ULONG CreateFlags, IN SIZE_T StackZeroBits, IN SIZE_T SizeOfStackCommit,
		IN SIZE_T SizeOfStackReserve, OUT LPVOID BytesBuffer);

	LONG NTAPI hkRegOpenKeyExW(HKEY hKey, LPCWSTR lpSubKey,
		DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);

	NTSTATUS NTAPI hkRtlAdjustPrivilege(ULONG Privilege,
		BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);

	HRESULT NTAPI hkURLDownloadToFileA(LPUNKNOWN pCaller, LPCSTR szURL,
		LPCSTR szFileName, DWORD dwReserved, LPBINDSTATUSCALLBACK lpfnCB);

	BOOL hkWriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize,
		SIZE_T *lpNumberOfBytesWritten);

	int hkSystem(const char* command);
	int hkWSystem(const wchar_t* command);

	void HookSyscalls();
	void UnhookSyscalls();
}