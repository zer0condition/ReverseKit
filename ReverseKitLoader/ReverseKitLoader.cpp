#include "ReverseKitLoader.h"

std::string ProcessName = "";
std::string DLLName = "ReverseKit.dll";
DWORD ProcessID = 0;

int main()
{
	if (!PathFileExistsA(DLLName.c_str()))
	{
		printf("ReverseKit.dll not found\n");
		system("pause");
		return 0;
	}

	printf("Process Name: ");

	std::cin >> ProcessName;

	system("cls");

	printf("Waiting for process to start...");

	while (!ProcessID)
		ProcessID = ReverseKitLoader::GetProcessID(ProcessName.c_str());

	if (!ReverseKitLoader::InjectDLL(ProcessID, DLLName.c_str()))
	{
		printf("Failed to inject.\n");
		system("pause");
	}

	return 0;
}
