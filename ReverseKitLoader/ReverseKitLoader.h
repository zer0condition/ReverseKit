#pragma once

#include <stdio.h> 
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

#include <Shlwapi.h>
#pragma comment( lib, "shlwapi.lib")

class ReverseKitLoader
{
public:
	static bool LoadDLL(DWORD ProcessID, const char* DLLName)
	{
		if (!ProcessID)
			return false;

		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
		if (!hProcess)
		{
			printf("Error: OpenProcess() failed: %d", GetLastError());
			return false;
		}

		char DllName[MAX_PATH];

		GetFullPathNameA(DLLName, MAX_PATH, DllName, NULL);

		HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");

		if (hKernel32)
		{
			LPVOID LoadLib = (LPVOID)GetProcAddress(hKernel32, "LoadLibraryA");

			LPVOID RemoteString = VirtualAllocEx(hProcess, NULL, strlen(DllName), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

			if (RemoteString) {
				WriteProcessMemory(hProcess, RemoteString, DllName, strlen(DllName), NULL);
				CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLib, (LPVOID)RemoteString, NULL, NULL);
			}
		}

		CloseHandle(hProcess);

		return true;
	}

	static DWORD GetProcessID(const char* ProcName)
	{
		HANDLE thSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (thSnapShot == INVALID_HANDLE_VALUE)
		{
			printf("Error: Unable to create toolhelp snapshot!");
			return 0;
		}

		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(PROCESSENTRY32);

		BOOL retval = Process32First(thSnapShot, &pe);
		while (retval)
		{
			if (!strcmp(pe.szExeFile, ProcName))
			{
				CloseHandle(thSnapShot);
				return pe.th32ProcessID;
			}
			retval = Process32Next(thSnapShot, &pe);
		}

		CloseHandle(thSnapShot);

		return 0;
	}
};
