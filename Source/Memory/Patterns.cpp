#include "Patterns.hpp"
#include "Memory.hpp"

namespace Onward
{
	bool Pointers::Intialize()
	{
		using namespace Memory;

		NumPointers = 5;

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

		//m_GameWindow = FindWindowA("grcWindow", "Grand Theft Auto V");

		Scan("Swapchain", "44 38 35 ? ? ? ? 48 8B 01", [this](Handle Ptr)
		{
			m_Swapchain = Ptr.Sub(13).Rip().As<decltype(m_Swapchain)&>();
		});

		Scan("Game Build Version", "48 83 EC 60 48 8D 0D ? ? ? ? E8", [this](Handle Ptr)
		{
			m_GameBuild = Ptr.Sub(17).Add(268).Rip().As<decltype(m_GameBuild)>();
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