#include "Script.hpp"

namespace Onward
{
	Script::Script(const Func_T Function, const std::optional<size_t> Stack) :
		m_Done(false),
		m_ScriptFiber(nullptr),
		m_MainFiber(nullptr),
		m_Function(Function)
	{
		m_ScriptFiber = CreateFiber(
			Stack.has_value() ? Stack.value() : 0,
			[](void* param)
			{
				auto this_script = static_cast<Script*>(param);
				this_script->FiberFunction();
			},
			this
		);
	}

	Script::~Script()
	{
		if (m_ScriptFiber)
			DeleteFiber(m_ScriptFiber);
	}

	bool Script::IsDone()
	{
		return m_Done;
	}

	void Script::Tick()
	{
		m_MainFiber = GetCurrentFiber();
		if (!m_WakeTime.has_value() || m_WakeTime.value() <= std::chrono::high_resolution_clock::now())
		{
			SwitchToFiber(m_ScriptFiber);
		}
	}

	void Script::YieldScript(std::optional<std::chrono::high_resolution_clock::duration> Duration)
	{
		if (Duration.has_value())
		{
			m_WakeTime = std::chrono::high_resolution_clock::now() + Duration.value();
		}
		else
		{
			m_WakeTime = std::nullopt;
		}
		SwitchToFiber(m_MainFiber);
	}

	Script* Script::GetCurrent()
	{
		return static_cast<Script*>(GetFiberData());
	}

	void Script::FiberFunction()
	{
		m_Function();

		m_Done = true;

		while (true)
		{
			YieldScript();
		}
	}
}