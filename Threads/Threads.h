#pragma once
#include <Includes.h>


struct ThreadInfo {
    DWORD threadId;
    DWORD cpuUsage;

    bool operator==(const ThreadInfo& other) const {
        return threadId == other.threadId;
    }
};

extern std::vector<ThreadInfo> threadInfo;

void GetThreadInformation();