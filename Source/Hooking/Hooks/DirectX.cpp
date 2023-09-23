#include "../../Common.hpp"
#include "../Hooking.hpp"
#include "../../UI/UIManager.hpp"

#include <ImGui/imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

namespace Onward
{
	HRESULT Hooks::Present(IDXGISwapChain* this_, UINT syncInterval, UINT flags)
	{
		if (g_Running && ((flags & (UINT)DXGI_PRESENT_TEST) != (UINT)DXGI_PRESENT_TEST))
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			{
				g_UIManager->OnTick();
			}
			ImGui::EndFrame();
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}

		return g_Hooking->DXHook.GetOriginal<decltype(&Present)>(8)(this_, syncInterval, flags);
	}

	HRESULT Hooks::ResizeBuffers(IDXGISwapChain* this_, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
	{
		if (g_Running)
		{
			ImGui_ImplDX11_InvalidateDeviceObjects();
			const auto result = g_Hooking->DXHook.GetOriginal<decltype(&ResizeBuffers)>(13)(this_, BufferCount, Width, Height, NewFormat, SwapChainFlags);

			if (SUCCEEDED(result))
			{
				ImGui_ImplDX11_CreateDeviceObjects();
			}

			return result;
		}

		return g_Hooking->DXHook.GetOriginal<decltype(&ResizeBuffers)>(13)(this_, BufferCount, Width, Height, NewFormat, SwapChainFlags);
	}
}