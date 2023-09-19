#include "Common.hpp"

#include "Logger/Logger.hpp"

DWORD WINAPI Main(LPVOID handle)
{
	using namespace Onward;

	Logger* g_Logger = Log::GetInstance();
	g_Logger->Initialize();

	Sleep(100);

	g_Logger->Custom(eLogColor::Cyan, "Main", "%s | Version: %s | Loaded", ONWARD_NAME, ONWARD_VERSION);

	while (g_Running) {

		if (GetAsyncKeyState(VK_END) & 0x8000)
			g_Running = false;

		Sleep(100);
	}

	Sleep(300);

	g_Logger->Custom(eLogColor::Cyan, "Main", "%s | Version: %s | Unloaded", ONWARD_NAME, ONWARD_VERSION);
	g_Logger->Uninitialize();

	FreeLibraryAndExitThread(g_Module, 0);
}

BOOL WINAPI DllMain(HINSTANCE handle, DWORD reason, LPVOID reserved)
{
	using namespace Onward;

	switch (reason)
	{
	case DLL_PROCESS_ATTACH:

		DisableThreadLibraryCalls(handle);

		g_Module = handle;
		if (HANDLE hThread = CreateThread(nullptr, 0, Main, handle, 0, nullptr)) {
			CloseHandle(hThread);
		}
		
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}