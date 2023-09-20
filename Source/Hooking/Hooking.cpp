#include "Hooking.hpp"
#include "../Memory/Patterns.hpp"

#include <MinHook/MinHook.h>

namespace Onward
{
	void Hooking::Initialize()
	{
		DXHook.Create(g_Patterns->m_Swapchain, 18);

		//g_Logger->Custom(eLogColor::Green, "Hooking Created", "Here");
		MH_Initialize();

		DXHook.Hook(&Hooks::Present, 8);
		DXHook.Hook(&Hooks::ResizeBuffers, 13);
		DXHook.Enable();
	}

	void Hooking::Uninitialize()
	{
		//g_Logger->Custom(eLogColor::Green, "Hooking Destroyed", "Not Here");

		if (DXHook.m_Created)
		{
			DXHook.Disable();
		}

		MH_DisableHook(MH_ALL_HOOKS);
		MH_Uninitialize();
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