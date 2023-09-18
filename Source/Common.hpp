#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
/*#define NOMINMAX
#define _WIN32_WINNT _WIN32_WINNT_WIN7*/

#include <Windows.h>

#define CRT_SECURE_WARNINGS

#include <chrono>
#include <atomic>
#include <fstream>
#include <filesystem>
#include <cstdarg>

#define ONWARD_NAME "Onward"
#define ONWARD_VERSION "1.0.0"

#define ONWARD_DEBUG
#define ONWARD_SUPPORT_VERSION "1.67"

namespace Onward
{
	using namespace std::chrono_literals;

	inline std::atomic<bool> g_Running{};

	inline HMODULE g_Module{};
}