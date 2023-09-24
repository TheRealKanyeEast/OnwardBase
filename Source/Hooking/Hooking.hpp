#pragma once
#include "../Common.hpp"
#include "VMTHook.hpp"

namespace Onward
{
	struct Hooks
	{
		static HRESULT Present(IDXGISwapChain* this_, UINT syncInterval, UINT flags);
		static HRESULT ResizeBuffers(IDXGISwapChain* this_, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);

		static LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		static bool RunScriptThreads(uint32_t OperationsToExecute);
	};

	class Hooking
	{
	public:
		void Initialize();
		void Uninitialize();

		friend struct Hooks;
	private:
		VMTHook DXHook;
		void* m_OriginalWndProc{};

		void* m_OriginalRunScriptThreads{};
	};

	namespace Hook
	{
		Hooking* GetInstance();
	}

	inline Hooking* g_Hooking;
}