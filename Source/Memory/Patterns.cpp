#include "Patterns.hpp"
#include "Memory.hpp"

namespace Onward
{
	bool Pointers::Intialize()
	{
		using namespace Memory;

		NumPointers = 6;

		Scan("Game State", "83 3D ? ? ? ? ? 75 17 8B 43 20", [this](Handle Ptr)
		{
			m_GameState = Ptr.Add(2).Rip().As<decltype(m_GameState)>();
		});

		Scan("Ped Factory", "48 8B 05 ? ? ? ? 48 8B 48 08 48 85 C9 74 52 8B 81", [this](Handle Ptr)
		{
			m_CPedFactory = Ptr.Add(3).Rip().As<decltype(m_CPedFactory)>();
		});

		Scan("Session State", "40 38 35 ? ? ? ? 75 0E 4C 8B C3 49 8B D7 49 8B CE", [this](Handle Ptr)
		{
			m_IsSessionStarted = Ptr.Add(3).Rip().As<decltype(m_IsSessionStarted)>();
		});

		m_GameWindow = FindWindowA("grcWindow", "Grand Theft Auto V");

		Scan("Swapchain", "44 38 35 ? ? ? ? 48 8B 01", [this](Handle Ptr)
		{
			m_Swapchain = Ptr.Sub(13).Rip().As<decltype(m_Swapchain)&>();
		});

		Scan("Game Build Version", "48 83 EC 60 48 8D 0D ? ? ? ? E8", [this](Handle Ptr)
		{
			m_GameBuild = Ptr.Sub(17).Add(268).Rip().As<decltype(m_GameBuild)>();
		});

		Scan("Window Process", "44 8B E2 4C 8B E9 3B D0", [this](Handle Ptr)
		{
			m_WndProc = Ptr.Sub(52).As<decltype(m_WndProc)>();
		});

		Scan("Get Native Handler", "48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A", [this](Handle Ptr)
		{
			m_GetNativeHandler = Ptr.Add(12).Rip().As<decltype(m_GetNativeHandler)>();
		});

		Scan("Native Registration Table", "76 32 48 8B 53 40 48 8D 0D", [this](Handle Ptr)
		{
			m_NativeRegistrationTable = Ptr.Add(9).Rip().As<decltype(m_NativeRegistrationTable)>();
		});

		Scan("Script Threads", "45 33 F6 8B E9 85 C9 B8", [this](Handle Ptr)
		{
			m_ScriptThreads = Ptr.Sub(4).Rip().Sub(8).As<decltype(m_ScriptThreads)>();
		});

		Scan("Run Script Threads", "45 33 F6 8B E9 85 C9 B8", [this](Handle Ptr)
		{
			m_RunScriptThreads = Ptr.Sub(0x1F).As<RunScriptThreads>();
		});

		Scan("Fix Vectors", "83 79 18 00 48 8B D1 74 4A FF 4A 18 48 63 4A 18 48 8D 41 04 48 8B 4C CA", [this](Handle Ptr)
		{
			m_FixVectors = Ptr.As<FixVectors>();
		});
	}

	namespace Patterns
	{
		Pointers* GetInstance()
		{
			static Pointers instance;
			return &instance;
		}
	}
}