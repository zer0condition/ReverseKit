#include "ReverseKitLoader.h"

std::string ProcessName = "";
std::string DLLName = "ReverseKit.dll";
DWORD ProcessID = 0;

int main()
{
	if (!PathFileExistsA(DLLName.c_str()))
	{
		printf("Error: ReverseKit.dll not found\n");
		system("pause");
		return 0;
	}

	printf("Process Name: ");

	std::cin >> ProcessName;

	system("cls");

	printf("Waiting for process to start...\n");

	while (!ProcessID)
		ProcessID = ReverseKitLoader::GetProcessID(ProcessName.c_str());

	if (!ReverseKitLoader::LoadDLL(ProcessID, DLLName.c_str()))
	{
		printf("Error: Failed to inject\n");
		system("pause");
	}
	return 0;
}