#include "ReverseKitLoader.h"

std::string ProcessName = "";
DWORD ProcessID = 0;

int main()
{
	printf("Process Name: ");

	std::cin >> ProcessName;

	system("cls");

	printf("Waiting for process to start...");

	while (!ProcessID)
		ProcessID = Injector::GetTargetThreadIDFromProcName(ProcessName.c_str());

	if (Injector::InjectDLL(ProcessID, "ReverseKit.dll"))
	{
		printf("Injected.\n");
	}
	else
	{
		printf("Failed.\n");
		system("pause");
	}

	return 0;
}