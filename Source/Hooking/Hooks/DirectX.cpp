#include "../../Common.hpp"
#include "../Hooking.hpp"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>

namespace Onward
{
	HRESULT Hooks::Present(IDXGISwapChain* this_, UINT syncInterval, UINT flags)
	{
		g_Logger->Custom(eLogColor::Blue, "Present", "Hello from inside Present");
		//return 0;
		return g_Hooking->DXHook.GetOriginal<decltype(&Present)>(8)(this_, syncInterval, flags);
	}

	HRESULT Hooks::ResizeBuffers(IDXGISwapChain* this_, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
	{
		return g_Hooking->DXHook.GetOriginal<decltype(&ResizeBuffers)>(13)(this_, BufferCount, Width, Height, NewFormat, SwapChainFlags);
	}
}