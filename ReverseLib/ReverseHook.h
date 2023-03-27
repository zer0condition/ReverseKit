#pragma once

namespace ReverseHook {
    void hook(void* original_function, void* hooked_function, unsigned char* original_bytes) {
        DWORD oldProtect;

        VirtualProtect(original_function, 14, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(original_bytes, original_function, 14);

        /*
               mov rax, hooked_function
               jmp rax
        */

        *(unsigned char*)original_function = 0x48;
        *(unsigned char*)((unsigned char*)original_function + 1) = 0xB8;
        *(int64_t*)((unsigned char*)original_function + 2) = (int64_t)hooked_function;
        *(unsigned char*)((unsigned char*)original_function + 10) = 0xFF;
        *(unsigned char*)((unsigned char*)original_function + 11) = 0xE0;

        VirtualProtect(original_function, 14, oldProtect, &oldProtect);
    }

    void unhook(void* original_function, unsigned char* original_bytes) {
        DWORD oldProtect;

        VirtualProtect(original_function, 14, PAGE_EXECUTE_READWRITE, &oldProtect);

        memcpy(original_function, original_bytes, 14);

        VirtualProtect(original_function, 14, oldProtect, &oldProtect);
    }


    namespace Trampoline {
        void* createTrampoline(void* original_function) {
            void* trampoline = VirtualAlloc(NULL, 14 + sizeof(void*), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

            if (trampoline == NULL) {
                return NULL;
            }

            /*
                mov rax, original_function
                jmp rax
            */
            *(unsigned char*)trampoline = 0x48;
            *(unsigned char*)((unsigned char*)trampoline + 1) = 0xB8;
            *(void**)((unsigned char*)trampoline + 2) = original_function;
            *(unsigned char*)((unsigned char*)trampoline + 10) = 0xFF;
            *(unsigned char*)((unsigned char*)trampoline + 11) = 0xE0;

            return trampoline;
        }

        void hook(void* original_function, void* hooked_function, unsigned char* original_bytes) {
            DWORD oldProtect;

            void* trampoline = createTrampoline(original_function);

            if (trampoline == NULL) {
                return;
            }

            memcpy(original_bytes, original_function, 14);

            /*
                mov rax, hooked_function
                jmp rax
            */

            *(unsigned char*)original_function = 0x48;
            *(unsigned char*)((unsigned char*)original_function + 1) = 0xB8;
            *(void**)((unsigned char*)original_function + 2) = hooked_function;
            *(unsigned char*)((unsigned char*)original_function + 10) = 0xFF;
            *(unsigned char*)((unsigned char*)original_function + 11) = 0xE0;

            *(unsigned char*)((unsigned char*)trampoline + 14) = 0xE9;
            *(int*)((unsigned char*)trampoline + 15) = (int)((unsigned char*)original_function + 14 - ((unsigned char*)trampoline + 14));

            VirtualProtect(original_function, 14, oldProtect, &oldProtect);
        }

        void unhook(void* original_function, unsigned char* original_bytes) {
            DWORD oldProtect;

            VirtualProtect(original_function, 14, PAGE_EXECUTE_READWRITE, &oldProtect);

            memcpy(original_function, original_bytes, 14);

            VirtualProtect(original_function, 14, oldProtect, &oldProtect);
        }
    }
}