#pragma once
#include <Windows.h>
#include <vector>

struct ThreadInfo
{
    DWORD threadId;
    DWORD cpuUsage;

    bool operator==(const ThreadInfo& other) const
	{
        return threadId == other.threadId;
    }
};

inline std::vector<ThreadInfo> threadInfo = {};

void GetThreadInformation();