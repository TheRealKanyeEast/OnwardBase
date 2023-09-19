#pragma once
#include "../Common.hpp"
#include "VMTHook.hpp"

namespace Onward
{
	struct Hooks
	{
		static HRESULT Present(IDXGISwapChain* this_, UINT syncInterval, UINT flags);
		static HRESULT ResizeBuffers(IDXGISwapChain* this_, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
	};

	class Hooking
	{
	public:
		void Initialize();
		void Uninitialize();

		friend struct Hooks;
	private:
		VMTHook DXHook;
	};

	namespace Hook
	{
		Hooking* GetInstance();
	}

	inline Hooking* g_Hooking;
}