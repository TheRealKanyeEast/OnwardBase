#include "ScriptManager.hpp"

#include "../Rage/GtaUtil.hpp"

namespace Onward
{
	void ScriptManager::AddScript(std::unique_ptr<Script> Script)
	{
		std::lock_guard lock(m_Mutex);

		m_Scripts.push_back(std::move(Script));
	}

	void ScriptManager::RemoveAllScripts()
	{
		std::lock_guard lock(m_Mutex);

		m_Scripts.clear();
	}

	void ScriptManager::InternalTick()
	{
		static bool EnsureMainFiber = (EnsureMain(), true);

		std::lock_guard lock(m_Mutex);

		for (const auto& Script : m_Scripts)
		{
			Script->Tick();
		}
	}

	void ScriptManager::Tick()
	{
		Util::execute_as_script(rage::joaat("main_persistent"), std::mem_fn(&ScriptManager::InternalTick), this);
	}

	void ScriptManager::EnsureMain()
	{
		ConvertThreadToFiber(nullptr);

		m_CanTick = true;
	}
}