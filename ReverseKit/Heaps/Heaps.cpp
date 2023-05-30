#include "Heaps.h"

#include <stdexcept>
#include <Windows.h>
#include <TlHelp32.h>

void GetHeapsInformation()
{
	const HANDLE hProcessHeapSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST | TH32CS_SNAPMODULE, GetCurrentProcessId());

	if (hProcessHeapSnap == INVALID_HANDLE_VALUE)
		throw std::runtime_error("Failed to create heap snapshot.");

	HEAPLIST32 hl = { sizeof(HEAPLIST32) };
	if (Heap32ListFirst(hProcessHeapSnap, &hl))
	{
		do
		{
			HANDLE hProcess = GetCurrentProcess();
			HEAPENTRY32 heapEntry = { sizeof(HEAPENTRY32) };

			// Start with the first heap of the process
			Heap32First(&heapEntry, hl.th32ProcessID, hl.th32HeapID);

			while (heapEntry.hHandle != nullptr)
			{
				HeapInfo info;

				info.address = heapEntry.dwAddress;
				info.id = heapEntry.th32HeapID;
				info.flags = heapEntry.dwFlags;

				bool alreadyExists = false;
				for (const auto& heap : heaps) {
					if (heap.address == info.address && heap.flags == info.flags && heap.id == info.id) {
						alreadyExists = true;
						break;
					}
				}

				if (!alreadyExists) {
					heaps.push_back(info);
				}

				// Move to the next heap
				Heap32Next(&heapEntry);
			}

			// Move to the next heap
			hl.dwSize = sizeof(HEAPLIST32);
		} while (Heap32ListNext(hProcessHeapSnap, &hl));
	}

	CloseHandle(hProcessHeapSnap);
}