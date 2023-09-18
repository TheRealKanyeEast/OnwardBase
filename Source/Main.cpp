#include "Common.hpp"

DWORD WINAPI Main(LPVOID handle)
{
	using namespace Onward;

	while (g_Running) {

		if (GetAsyncKeyState(VK_END) & 0x8000)
			g_Running = false;

		Sleep(100);
	}

	FreeLibraryAndExitThread(g_Module, 0);
}

BOOL WINAPI DllMain(HINSTANCE handle, DWORD reason, LPVOID reserved)
{
	using namespace Onward;

	switch (reason)
	{
	case DLL_PROCESS_ATTACH:

		g_Module = handle;
		if (HANDLE hThread = CreateThread(nullptr, 0, Main, handle, 0, nullptr)) {
			CloseHandle(hThread);
		}
		
	case DLL_PROCESS_DETACH:
		break;
	}
}