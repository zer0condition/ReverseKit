#pragma once

typedef NTSTATUS(NTAPI* NtCreateThreadEx_t)(OUT PHANDLE ThreadHandle, IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes, IN HANDLE ProcessHandle,
    IN LPTHREAD_START_ROUTINE StartRoutine, IN LPVOID Argument,
    IN ULONG CreateFlags, IN SIZE_T StackZeroBits, IN SIZE_T SizeOfStackCommit,
    IN SIZE_T SizeOfStackReserve, OUT LPVOID BytesBuffer);

inline NtCreateThreadEx_t oNtCreateThreadEx;

inline NTSTATUS NTAPI hkNtCreateThreadEx(OUT PHANDLE ThreadHandle, IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes, IN HANDLE ProcessHandle,
	IN LPTHREAD_START_ROUTINE StartRoutine, IN LPVOID Argument,
    IN ULONG CreateFlags, IN SIZE_T StackZeroBits, IN SIZE_T SizeOfStackCommit,
    IN SIZE_T SizeOfStackReserve, OUT LPVOID BytesBuffer)
{
    InterceptedCallInfo Temp;

    Temp.functionName = "NtCreateThreadEx";

    char buffer[64];
    sprintf(buffer, "Created Thread: %p", StartRoutine);
    Temp.additionalInfo = buffer;

    interceptedCalls.push_back(Temp);

    ReverseHook::unhook(oNtCreateThreadEx, original_createthread_bytes);

    const auto result = oNtCreateThreadEx(ThreadHandle, DesiredAccess, ObjectAttributes, ProcessHandle, StartRoutine, Argument, CreateFlags, StackZeroBits, SizeOfStackCommit, SizeOfStackReserve, BytesBuffer);

    ReverseHook::hook(oNtCreateThreadEx, hkNtCreateThreadEx, original_createthread_bytes);

    return result;
}