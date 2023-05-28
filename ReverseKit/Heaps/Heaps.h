#pragma once

#include <vector>

struct HeapInfo
{
	unsigned long long address;
	unsigned long long id;
	unsigned long flags;
};

inline std::vector<HeapInfo> heaps = {};

void GetHeapsInformation();