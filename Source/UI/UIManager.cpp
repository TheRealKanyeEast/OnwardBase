#include "UIManager.hpp"

#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>

#include "Fonts/Fonts.h"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Onward
{
	void UIManager::Initialize()
	{
		m_Swapchain = g_Patterns->m_Swapchain;
		Sleep(100);
		if (SUCCEEDED(m_Swapchain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&m_Device))))
		{
			m_Device->GetImmediateContext(&m_Context);
			{
				//WindowProcessOriginal = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(g_Patterns->m_GameWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WndProc)));

				ImGui::CreateContext();

				ImGui::StyleColorsDark();
				/*ImGuiStyle& style = ImGui::GetStyle();
				style.WindowRounding = 10.f;
				style.PopupRounding = 10.f;
				style.FrameRounding = 10.f;
				style.ChildBorderSize = 0.f;
				style.FrameBorderSize = 0.f;
				style.PopupBorderSize = 0.f;
				style.TabBorderSize = 0.f;
				style.WindowBorderSize = 0.f;*/

				//ImGui::GetIO().IniFilename = nullptr;

				ImGui_ImplWin32_Init(g_Patterns->m_GameWindow);
				ImGui_ImplDX11_Init(m_Device, m_Context);

				m_FontConfig.FontDataOwnedByAtlas = false;
				strcpy(m_FontConfig.Name, "Raleway");
				Segoe = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 15.f);
				Raleway = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<uint8_t*>(font_raleway), sizeof(font_raleway), 20.f, &m_FontConfig);
				ImGui::GetIO().Fonts->Build();
			}
			m_Opened = true;
		}
		else {
			g_Logger->Custom(eLogColor::DarkMagneta, "Render", "Some Issue");
		}
	}

	void UIManager::Uninitialize()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		//SetWindowLongPtrW(g_Patterns->m_GameWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcessOriginal));
	}

	void UIManager::OnTick()
	{
		if (g_UIManager->m_Opened)
		{
			//g_Logger->Custom(eLogColor::Cyan, "IMGUI", "Hello");
			ImGuiIO& io = ImGui::GetIO();
			ImVec2 Size = ImVec2((0 & 1) ? io.DisplaySize.x - 10.f : 10.f, (0 & 2) ? io.DisplaySize.y - 82.f : 82.f);
			ImGui::SetNextWindowSize(ImVec2(800, 430));
			ImGui::SetNextWindowPos(Size, ImGuiCond_Once);
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1.f));
			if (ImGui::Begin("Yup"), false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings)
			{
				ImGui::Button("Hi");
			}
			ImGui::End();
			ImGui::PopStyleColor();
		}
	}

	void UIManager::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (GetAsyncKeyState(VK_INSERT) & 1)
			g_UIManager->m_Opened = !g_UIManager->m_Opened;

		if (ImGui::GetCurrentContext())
			ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
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