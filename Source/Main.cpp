#include "Common.hpp"

#include "Hooking/Hooking.hpp"
#include "Discord RPC/Discord.hpp"

void Unload()
{
	using namespace Onward;

	Discord::Shutdown();

	g_Hooking->Uninitialize();
	Sleep(3500);

	Sleep(300);

	g_Logger->Custom(eLogColor::Cyan, "Main", "%s | Version: %s | Unloaded", ONWARD_NAME, ONWARD_VERSION);
	g_Logger->Uninitialize();

	FreeLibraryAndExitThread(g_Module, 0);
}

void WaitForLoad()
{
	using namespace Onward;

	do
	{
		if (GetAsyncKeyState(VK_END) & 0x8000)
			g_Running = false;

		Discord::UpdatePresence();

		Sleep(100);
	} while (*g_Patterns->m_GameState != eGameState::Playing);
}

DWORD Main(LPVOID handle)
{
	using namespace Onward;

	g_Logger = Log::GetInstance();
	g_Logger->Initialize();

	Discord::Initialize();

	g_Patterns = Patterns::GetInstance();

	g_Patterns->Intialize();

	if (FoundPointers == false)
	{
		Sleep(4500);
		Unload();
	}

	WaitForLoad();

	g_Hooking = Hook::GetInstance();
	g_Hooking->Initialize();

	Sleep(100);

	g_Logger->Custom(eLogColor::Cyan, "Main", "%s | Version: %s | Loaded", ONWARD_NAME, ONWARD_VERSION);

	while (g_Running) {

		if (GetAsyncKeyState(VK_END) & 0x8000)
			g_Running = false;

		Sleep(100);
	}

	Unload();
}

BOOL WINAPI DllMain(HINSTANCE handle, DWORD reason, LPVOID reserved)
{
	using namespace Onward;

	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(handle);

		g_Module = handle;
		if (HANDLE hThread = CreateThread(nullptr, 0, Main, handle, 0, nullptr)) {
			CloseHandle(hThread);
		}
	}

	return TRUE;
}