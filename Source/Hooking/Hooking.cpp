#include "Hooking.hpp"
#include "../Memory/Patterns.hpp"
#include "../UI/UIManager.hpp"
#include "../Script/ScriptManager.hpp"

#include <MinHook/MinHook.h>

namespace Onward
{
	void Hooking::Initialize()
	{
		DXHook.Create(g_Patterns->m_Swapchain, 18);

		//g_Logger->Custom(eLogColor::Green, "Hooking Created", "Here");
		MH_Initialize();

		MH_CreateHook(g_Patterns->m_WndProc, &Hooks::WndProc, &m_OriginalWndProc);

		MH_CreateHook(g_Patterns->m_RunScriptThreads, &Hooks::RunScriptThreads, &m_OriginalRunScriptThreads);

		DXHook.Hook(&Hooks::Present, 8);
		DXHook.Hook(&Hooks::ResizeBuffers, 13);
		DXHook.Enable();

		MH_EnableHook(MH_ALL_HOOKS);
	}

	void Hooking::Uninitialize()
	{
		//g_Logger->Custom(eLogColor::Green, "Hooking Destroyed", "Not Here");

		MH_DisableHook(MH_ALL_HOOKS);

		MH_RemoveHook(g_Patterns->m_RunScriptThreads);

		MH_RemoveHook(g_Patterns->m_WndProc);

		MH_Uninitialize();

		if (DXHook.m_Created)
		{
			DXHook.Disable();
		}
	}

	LRESULT __stdcall Hooks::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		g_UIManager->WndProc(hWnd, msg, wParam, lParam);
		return static_cast<decltype(&WndProc)>(g_Hooking->m_OriginalWndProc)(hWnd, msg, wParam, lParam);
	}

	bool Hooks::RunScriptThreads(uint32_t OperationsToExecute)
	{
		if (g_Running)
		{
			g_ScriptManager.Tick();
		}

		return static_cast<decltype(&RunScriptThreads)>(g_Hooking->m_OriginalRunScriptThreads)(OperationsToExecute);
	}

	namespace Hook
	{
		Hooking* GetInstance()
		{
			static Hooking instance;
			return &instance;
		}
	}
}