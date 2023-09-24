#pragma once
#include "../Common.hpp"

#include "Script.hpp"

namespace Onward
{
	using ScriptList = std::vector<std::unique_ptr<Script>>;

	class ScriptManager
	{
	public:
		explicit ScriptManager() = default;
		~ScriptManager() = default;

		void AddScript(std::unique_ptr<Script> Script);
		void RemoveAllScripts();

		inline void ForEachScript(auto Function)
		{
			std::lock_guard lock(m_Mutex);

			for (const auto& Script : m_Scripts)
			{
				Function(Script);
			}
		}

		void Tick();

		[[nodiscard]] inline bool CanTick() const
		{
			return m_CanTick;
		}

	private:
		void EnsureMain();
		void InternalTick();

	private:
		ScriptList m_Scripts;
		std::recursive_mutex m_Mutex;
		bool m_CanTick;
	};

	inline ScriptManager g_ScriptManager;
}