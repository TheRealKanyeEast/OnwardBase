#pragma once
#include "../Common.hpp"

#include <imgui.h>

namespace Onward
{
	class UIManager
	{
	public:
		void OnTick();
		static LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		void Initialize();
		void Uninitialize();

		bool m_Opened = false;
	private:
		ImFontConfig m_FontConfig{};
		ImFont* Raleway = nullptr;
		ImFont* Segoe = nullptr;
	private:
		IDXGISwapChain* m_Swapchain{};
		ID3D11Device* m_Device{};
		ID3D11DeviceContext* m_Context{};
		WNDPROC WindowProcessOriginal = nullptr;
	};

	namespace UI
	{
		UIManager* GetInstance();
	}

	inline UIManager* g_UIManager;
}