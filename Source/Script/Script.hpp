#pragma once
#include "../Common.hpp"

namespace Onward
{
	class Script
	{
		bool m_Done;

	public:
		using Func_T = std::function<void(void)>;

	public:
		explicit Script(const Func_T Function, const std::optional<size_t> Stack = std::nullopt);
		~Script();

		[[nodiscard]] bool IsDone();

		void Tick();
		void YieldScript(std::optional<std::chrono::high_resolution_clock::duration> Duration = std::nullopt);
		static Script* GetCurrent();

	private:
		void FiberFunction();

	private:
		void* m_ScriptFiber;
		void* m_MainFiber;
		Func_T m_Function;
		std::optional<std::chrono::high_resolution_clock::time_point> m_WakeTime;
	};
}