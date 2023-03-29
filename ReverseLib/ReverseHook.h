#pragma once

namespace ReverseHook {
	void hook(void* original_function, void* hooked_function, unsigned char* original_bytes);

	void unhook(void* original_function, unsigned char* original_bytes);

	namespace Trampoline {
		//createTrampoline not needed for other classes

		void hook(void* original_function, void* hooked_function, unsigned char* original_bytes);

		void unhook(void* original_function, unsigned char* original_bytes);
	}
}
