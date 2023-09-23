#pragma once
#include "../Common.hpp"

#include "ped/CPedFactory.hpp"
#include "script/scrNativeRegistrationTable.hpp"
#include "Functions.h"

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

		rage::scrNativeRegistrationTable* m_NativeRegistrationTable;
		GetNativeHandler* m_GetNativeHandler;
		FixVectors* m_FixVectors;

		IDXGISwapChain* m_Swapchain;
		WndProc* m_WndProc;
	};

	namespace Patterns
	{
		Pointers* GetInstance();
	}
}