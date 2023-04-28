#pragma once

#include <Windows.h>

namespace ReverseHook
{
    void hook(void* original_function, void* hooked_function, unsigned char* original_bytes);
	void unhook(void* original_function, const unsigned char* original_bytes);
       

    namespace Trampoline
	{
	    void* createTrampoline(void* original_function);
	    void hook(void* original_function, void* hooked_function, unsigned char* original_bytes);
	    void unhook(void* original_function, const unsigned char* original_bytes);
    }
}