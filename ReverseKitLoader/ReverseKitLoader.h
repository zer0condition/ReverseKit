#pragma once

#include <stdio.h> 
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

class Injector
{
public:
	static bool InjectDLL(DWORD processID, const char* relativeDllName)
	{
		if (processID == 0)
			return false;

		HANDLE Proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
		if (Proc == 0)
		{
			printf("OpenProcess() failed: %d", GetLastError());
			return false;
		}

		char DllName[MAX_PATH];

		GetFullPathNameA(relativeDllName, MAX_PATH, DllName, NULL);

		LPVOID LoadLib = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");

		LPVOID RemoteString = VirtualAllocEx(Proc, NULL, strlen(DllName), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		WriteProcessMemory(Proc, RemoteString, DllName, strlen(DllName), NULL);
		CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLib, (LPVOID)RemoteString, NULL, NULL);

		CloseHandle(Proc);
		return true;
	}

	static DWORD GetTargetThreadIDFromProcName(const char* ProcName)
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
				// names match
				// close the handle and return the process id
				CloseHandle(thSnapShot);
				return pe.th32ProcessID;
			}
			retval = Process32Next(thSnapShot, &pe);
		}

		CloseHandle(thSnapShot);

		return 0;
	}
};
