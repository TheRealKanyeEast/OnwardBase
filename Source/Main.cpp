#include "Common.hpp"

#include "Hooking/Hooking.hpp"
#include "Memory/Patterns.hpp"
#include "Discord RPC/Discord.hpp"
#include "UI/UIManager.hpp"
#include "Invoker/Invoker.hpp"

#include "Script/FiberPool.hpp"
#include "Script/ScriptManager.hpp"
#include "Features/FeatureManager.hpp"

#include "Invoker/Natives.h"

void Unload()
{
	using namespace Onward;

	
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

	Sleep(250);
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
	else
	{
		#ifndef ONWARD_DEBUG
			g_Logger->Custom(eLogColor::Gray, "Patterns", "Found %i/%i Patterns", NumPointersFound, NumPointers);
		#endif
	}

	if (strcmp(ONWARD_SUPPORT_VERSION, g_Patterns->m_GameBuild) != 0)
	{
		#ifdef ONWARD_DEBUG
			g_Logger->Custom(eLogColor::Red, "Main", "'%s' ---> '%s' Unsupported Version! Contact Support if you think this is a mistake.", g_Patterns->m_GameBuild, ONWARD_SUPPORT_VERSION);
		#else
			g_Logger->Custom(eLogColor::Red, "Main", "Unsupported Version! Contact Support if you think this is a mistake.");
		#endif
		Sleep(5000);
		Discord::Shutdown();
		g_Logger->Uninitialize();
		FreeLibraryAndExitThread(g_Module, 0);
	}

	WaitForLoad();

	//g_Invoker = Invoker::GetInstance();
	//g_Invoker->CacheHandlers();

	g_UIManager = UI::GetInstance();
	g_UIManager->Initialize();

	auto Fiber_Pool_Instance = std::make_unique<FiberPool>(11);

	auto FeatureManagerInstance = std::make_unique<FeatureManager>();

	g_Hooking = Hook::GetInstance();
	g_Hooking->Initialize();

	Sleep(100);

	g_FiberPool->QueueJob([] {
		g_Logger->Custom(eLogColor::Cyan, "Main", "%s | Version: %s | Loaded", ONWARD_NAME, ONWARD_VERSION);
		AUDIO::PLAY_SOUND_FRONTEND(-1, "PROPERTY_PURCHASE", "HUD_AWARDS", false);
	});

	g_Logger->Custom(eLogColor::Magneta, "Game Version", "'%s'", g_Patterns->m_GameBuild);

	while (g_Running) {

		if (GetAsyncKeyState(VK_END) & 0x8000)
			g_Running = false;

		Sleep(100);
	}

	Sleep(350);

	FeatureManagerInstance.reset();

	g_ScriptManager.RemoveAllScripts();

	g_UIManager->Uninitialize();

	Discord::Shutdown();

	g_Hooking->Uninitialize();
	Sleep(1250);

	Fiber_Pool_Instance.reset();

	Sleep(1250);

	Sleep(300);

	g_Logger->Custom(eLogColor::Cyan, "Main", "%s | Version: %s | Unloaded", ONWARD_NAME, ONWARD_VERSION);
	g_Logger->Uninitialize();

	FreeLibraryAndExitThread(g_Module, 0);
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