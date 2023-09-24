#pragma once
#include "../Common.hpp"

namespace Onward::Util
{
	template<typename F, typename... Args>
	void execute_as_script(rage::scrThread* thread, F&& callback, Args&&... args)
	{
		auto tls_ctx = rage::tlsContext::get();
		auto og_thread = tls_ctx->m_script_thread;

		tls_ctx->m_script_thread = thread;
		tls_ctx->m_is_script_thread_active = true;

		std::invoke(std::forward<F>(callback), std::forward<Args>(args)...);

		tls_ctx->m_script_thread = og_thread;
		tls_ctx->m_is_script_thread_active = og_thread != nullptr;
	}

	template<typename F, typename... Args>
	void execute_as_script(rage::joaat_t script_hash, F&& callback, Args&&... args)
	{
		for (auto thread : *g_Patterns->m_ScriptThreads)
		{
			if (!thread || !thread->m_context.m_thread_id || thread->m_context.m_script_hash != script_hash)
				continue;

			execute_as_script(thread, callback, args...);

			return;
		}
	}
}