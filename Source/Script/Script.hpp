#pragma once
#include "../Common.hpp"

namespace Onward
{
	class Script
	{
	public:
		explicit Script(std::function<void()> Function, std::optional<size_t> Stack);
		~Script();

		void Tick();
#undef Yield
		void Yield(std::optional<std::chrono::high_resolution_clock::duration> Duration = std::nullopt);
		static Script* GetCurrent();
	private:
		void FiberFunction();
	private:
		void* m_ScriptFiber;
		void* m_MainFiber;
		std::function<void()> m_Function;
		std::optional<std::chrono::high_resolution_clock::time_point> m_WakeTime;
	};
}