#include <cstdio>
#include <Windows.h>

float m_flTest = 0.f;

void TestThread()
{
	constexpr float flBuffer = 5.f;
	WriteProcessMemory(GetCurrentProcess(), &m_flTest, &flBuffer, sizeof(float), nullptr);

	printf("Test: %f\n", m_flTest);

	system("echo system");
	_wsystem(L"echo _wsystem");
}

int main()
{
	while (true)
	{
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)TestThread, nullptr, 0, nullptr);
		Sleep(3000);
	}
}