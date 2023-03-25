#pragma once

typedef NTSTATUS(NTAPI* NtCreateThreadEx_t)(OUT PHANDLE ThreadHandle, IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes, IN HANDLE ProcessHandle,
    IN LPTHREAD_START_ROUTINE StartRoutine, IN LPVOID Argument,
    IN ULONG CreateFlags, IN SIZE_T StackZeroBits, IN SIZE_T SizeOfStackCommit,
    IN SIZE_T SizeOfStackReserve, OUT LPVOID BytesBuffer);

NtCreateThreadEx_t oNtCreateThreadEx;

NTSTATUS NTAPI hkNtCreateThreadEx(OUT PHANDLE ThreadHandle, IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes, IN HANDLE ProcessHandle,
    IN LPTHREAD_START_ROUTINE StartRoutine, IN LPVOID Argument,
    IN ULONG CreateFlags, IN SIZE_T StackZeroBits, IN SIZE_T SizeOfStackCommit,
    IN SIZE_T SizeOfStackReserve, OUT LPVOID BytesBuffer)
{

    char message[256];
    sprintf(message, "Block thread creation for %p?", StartRoutine);

    int result = MessageBoxA(NULL, message, "[ReverseKit] NtCreateThreadEx", MB_YESNO);

    if (result == IDYES) {
        printf("[ReverseKit] Blocked NtCreateThreadEx for %p\n", StartRoutine);
        return STATUS_SUCCESS;
    }

    printf("[ReverseKit] Accepted NtCreateThreadEx for %p\n", StartRoutine);

    return oNtCreateThreadEx(ThreadHandle, DesiredAccess, ObjectAttributes, ProcessHandle, StartRoutine, Argument, CreateFlags, StackZeroBits, SizeOfStackCommit, SizeOfStackReserve, BytesBuffer);
}