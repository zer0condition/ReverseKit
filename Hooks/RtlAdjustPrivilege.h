#pragma once

typedef NTSTATUS(NTAPI* RtlAdjustPrivilege_t)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);

RtlAdjustPrivilege_t oRtlAdjustPrivilege;

NTSTATUS NTAPI hkRtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled)
{
	/*
		Anti-bsod for common methods like:
		https://github.com/zer0condition/CritBSOD/blob/master/CritBSOD/CritBSOD.cpp
		https://stackoverflow.com/questions/7034592/create-bsod-from-user-mode
	*/

	return STATUS_SUCCESS;
}