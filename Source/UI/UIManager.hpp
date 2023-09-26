#pragma once
#include "../Common.hpp"

#include <imgui.h>
#include "../Rage/Types.hpp"

namespace Onward
{
	class UIManager
	{
	public:
		void OnTick();
		void WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		void Initialize();
		void Uninitialize();

		bool LoadTextureFromFile(const char* filename, ID3D11Device* d3dDevice, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);

		bool m_Opened = false;

	public:
		ID3D11ShaderResourceView* m_Header{};
		ImageDimensions m_HeaderSize = { 0, 0 };

	private:
		ImFontConfig m_FontConfig{};
		ImFont* Raleway = nullptr;
		ImFont* Segoe = nullptr;
		ImFont* Monospace;

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