#pragma once
#include <TlHelp32.h>

struct ThreadInfo {
    DWORD threadId;
    DWORD cpuUsage;

    bool operator==(const ThreadInfo& other) const {
        return threadId == other.threadId;
    }
};

inline std::vector<ThreadInfo> threadInfo;

inline void GetThreadInformation() {
	const DWORD processID = GetCurrentProcessId();
    
    std::vector<ThreadInfo> updatedThreadInfo;

	const HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hThreadSnap == INVALID_HANDLE_VALUE) {
        return;
    }

    THREADENTRY32 te32 = { 0 };
    te32.dwSize = sizeof(THREADENTRY32);

    if (!Thread32First(hThreadSnap, &te32)) {
        CloseHandle(hThreadSnap);
        return;
    }

    do {
        if (te32.th32OwnerProcessID != processID) {
            continue;
        }

        const HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, te32.th32ThreadID);
        if (hThread == nullptr) {
            continue;
        }

        FILETIME createTime, exitTime, kernelTime, userTime;
        if (!GetThreadTimes(hThread, &createTime, &exitTime, &kernelTime, &userTime)) {
            CloseHandle(hThread);
            continue;
        }

        const ULONGLONG kernelTimeDiff = (((ULONGLONG)kernelTime.dwHighDateTime) << 32) | kernelTime.dwLowDateTime;
        const ULONGLONG userTimeDiff = (((ULONGLONG)userTime.dwHighDateTime) << 32) | userTime.dwLowDateTime;
        const ULONGLONG elapsedTime = kernelTimeDiff + userTimeDiff;
        const ULONGLONG systemTime = GetTickCount64() * 10000;  // Convert from milliseconds to 100-nanosecond intervals
        const DWORD cpuUsage = static_cast<DWORD>((elapsedTime * 100) / systemTime);

        auto it = std::find_if(threadInfo.begin(), threadInfo.end(), [&](const ThreadInfo& info) {
            return info.threadId == te32.th32ThreadID;
            });

        if (it != threadInfo.end()) {
            it->cpuUsage = cpuUsage;
            updatedThreadInfo.push_back(*it);
        }
        else {
            updatedThreadInfo.push_back({ te32.th32ThreadID, cpuUsage });
        }

        CloseHandle(hThread);
    } while (Thread32Next(hThreadSnap, &te32));

    CloseHandle(hThreadSnap);

    threadInfo = updatedThreadInfo;
}
