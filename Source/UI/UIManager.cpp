#include "UIManager.hpp"

#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "Fonts/Fonts.h"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Onward
{
	void UIManager::Initialize()
	{
		m_Swapchain = g_Patterns->m_Swapchain;
		Sleep(100);
		if (SUCCEEDED(m_Swapchain->GetDevice(__uuidof(ID3D11Device), (void**)(&m_Device))))
		{
			m_Device->GetImmediateContext(&m_Context);
			{
				WindowProcessOriginal = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(g_Patterns->m_GameWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WndProc)));

				ImGui::CreateContext();
				ImGui_ImplDX11_Init(m_Device, m_Context);
				ImGui_ImplWin32_Init(g_Patterns->m_GameWindow);

				ImGui::StyleColorsDark();
				ImGuiStyle& style = ImGui::GetStyle();
				style.WindowRounding = 10.f;
				style.PopupRounding = 10.f;
				style.FrameRounding = 10.f;
				style.ChildBorderSize = 0.f;
				style.FrameBorderSize = 0.f;
				style.PopupBorderSize = 0.f;
				style.TabBorderSize = 0.f;
				style.WindowBorderSize = 0.f;

				ImGuiIO& io = ImGui::GetIO(); (void)io;
				io.IniFilename = nullptr;

				m_FontConfig.FontDataOwnedByAtlas = false;
				strcpy(m_FontConfig.Name, "Raleway");
				Segoe = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 15.f);
				Raleway = io.Fonts->AddFontFromMemoryTTF(const_cast<uint8_t*>(font_raleway), sizeof(font_raleway), 20.f, &m_FontConfig);
			}
		}
		else {
			g_Logger->Custom(eLogColor::DarkMagneta, "Render", "Some Issue");
		}

		m_Opened = true;
	}

	void UIManager::Uninitialize()
	{
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();
		SetWindowLongPtrW(g_Patterns->m_GameWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcessOriginal));
	}

	void UIManager::OnTick()
	{
		ImGui::SetNextWindowSize({ 300, 500 });
		ImGui::Begin("Yup");
		{
			ImGui::Button("Hi");
		}
		ImGui::End();
	}

	LRESULT UIManager::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (GetAsyncKeyState(VK_INSERT) & 1)
			g_UIManager->m_Opened = !g_UIManager->m_Opened;

		if (g_UIManager->m_Opened)
			if (ImGui::GetCurrentContext())
				if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
					return 1L;

		return CallWindowProc(g_UIManager->WindowProcessOriginal, hwnd, msg, wparam, lparam);
	}

	namespace UI
	{
		UIManager* GetInstance()
		{
			static UIManager instance;
			return &instance;
		}
	}
}