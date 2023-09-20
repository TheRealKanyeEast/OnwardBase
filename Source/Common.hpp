#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _WIN32_WINNT _WIN32_WINNT_WIN7

#include <Windows.h>

#include <d3d11.h>

#include <chrono>
#include <atomic>
#include <fstream>
#include <filesystem>
#include <cstdarg>
#include <functional>
#include <vector>

#define ONWARD_NAME "Onward"
#define ONWARD_VERSION "1.0.0"

//#define ONWARD_DEBUG
#define ONWARD_SUPPORT_VERSION "1.67"

#include "Rage/LocalPlayer.hpp"
#include "Memory/Patterns.hpp"
#include "Logger/Logger.hpp"

namespace Onward
{
	using namespace std::chrono_literals;

	inline std::atomic<bool> g_Running = true;

	inline HMODULE g_Module{};

	inline LocalPlayer g_LocalPlayer{};

	inline Logger* g_Logger{};
	inline Pointers* g_Patterns;

	inline bool FoundPointers = true;
	inline int NumPointers = 0;
	inline int NumPointersFound = 0;
}
