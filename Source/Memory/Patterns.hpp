#pragma once
#include "../Common.hpp"

#include "ped/CPedFactory.hpp"

namespace Onward
{
	class Pointers
	{
	public:
		bool Intialize();

	public:
		eGameState* m_GameState;
		bool* m_IsSessionStarted;
		HWND m_GameWindow;
		char* m_GameBuild;

		CPedFactory* m_CPedFactory;

		IDXGISwapChain* m_Swapchain;
	};

	namespace Patterns
	{
		Pointers* GetInstance();
	}
}