#include "ReverseHook.h"

#include <Includes.h>

void ReverseHook::hook(void* original_function, void* hooked_function, unsigned char* original_bytes) {
    DWORD oldProtect;
    
    VirtualProtect(original_function, 14, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(original_bytes, original_function, 14);
    
    /*
           mov rax, hooked_function
           jmp rax
    */
    
    BYTE bytes[] = { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0};
    
    const uint64_t func_address = reinterpret_cast<uint64_t>(hooked_function);
    
    //write address into the array
    memcpy(bytes + 2, &func_address, sizeof(void*));
    
    //place hook
    memcpy(original_function, bytes, sizeof(bytes));
    
    //*static_cast<unsigned char*>(original_function) = 0x48;
    //*(unsigned char*)((unsigned char*)original_function + 1) = 0xB8;
    //*(int64_t*)((unsigned char*)original_function + 2) = (int64_t)hooked_function;
    //*(unsigned char*)((unsigned char*)original_function + 10) = 0xFF;
    //*(unsigned char*)((unsigned char*)original_function + 11) = 0xE0;
    
    VirtualProtect(original_function, 14, oldProtect, &oldProtect);
}

void ReverseHook::unhook(void* original_function, unsigned char* original_bytes) {
    DWORD oldProtect;
    
    VirtualProtect(original_function, 14, PAGE_EXECUTE_READWRITE, &oldProtect);
    
    memcpy(original_function, original_bytes, 14);
    
    VirtualProtect(original_function, 14, oldProtect, &oldProtect);
}

void* createTrampoline(void* original_function) {

            /*
                mov rax, original_function
                jmp rax
            */

            //some additional nops for the hook func
        	BYTE bytes[] = { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};

        	void* trampoline = VirtualAlloc(NULL, sizeof(bytes), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

            if (trampoline == NULL) {
                return NULL;
            }


			const uint64_t func_address = reinterpret_cast<uint64_t>(original_function);

			//write address into the array
			memcpy(bytes + 2, &func_address, sizeof(void*));

            memcpy(trampoline, bytes, sizeof(bytes));

            //*(unsigned char*)trampoline = 0x48;
            //*(unsigned char*)((unsigned char*)trampoline + 1) = 0xB8;
            //*(void**)((unsigned char*)trampoline + 2) = original_function;
            //*(unsigned char*)((unsigned char*)trampoline + 10) = 0xFF;
            //*(unsigned char*)((unsigned char*)trampoline + 11) = 0xE0;

            return trampoline;
        }

void ReverseHook::Trampoline::hook(void* original_function, void* hooked_function, unsigned char* original_bytes) {
	DWORD oldProtect;

    void* trampoline = createTrampoline(original_function);

    if (trampoline == nullptr) {
        return;
    }

    memcpy(original_bytes, original_function, 14);

    /*
        mov rax, hooked_function
        jmp rax
    */

	BYTE bytes[] = { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0};

	const uint64_t func_address = reinterpret_cast<uint64_t>(hooked_function);

	//write address into the array
	memcpy(reinterpret_cast<void*>(reinterpret_cast<uint64_t>(bytes) + 2), &func_address, sizeof(void*));

	//place hook
	memcpy(original_function, bytes, sizeof(bytes));

    //place small jump
    *(static_cast<unsigned char*>(trampoline) + 14) = 0xE9;
    *reinterpret_cast<int*>(static_cast<unsigned char*>(trampoline) + 15) = 
        static_cast<int>(static_cast<unsigned char*>(original_function) + 14 - (static_cast<unsigned char*>(trampoline) + 14));
    
    VirtualProtect(original_function, 14, oldProtect, &oldProtect);
}

void ReverseHook::Trampoline::unhook(void* original_function, unsigned char* original_bytes) {
            DWORD oldProtect;

            VirtualProtect(original_function, 14, PAGE_EXECUTE_READWRITE, &oldProtect);

            memcpy(original_function, original_bytes, 14);

            VirtualProtect(original_function, 14, oldProtect, &oldProtect);
}