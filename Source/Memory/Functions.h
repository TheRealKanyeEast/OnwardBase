#pragma once
#include "../Common.hpp"
#include "script/scrNativeHandler.hpp"

namespace Onward
{
	using WndProc = LRESULT(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	using GetNativeHandler = rage::scrNativeHandler(rage::scrNativeRegistrationTable*, rage::scrNativeHash);
	using FixVectors = void(*)(rage::scrNativeCallContext*);

	using RunScriptThreads = bool (*)(uint32_t ops_to_execute);
}