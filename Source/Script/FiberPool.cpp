#include "FiberPool.hpp"

#include "Script.hpp"
#include "ScriptManager.hpp"

namespace Onward
{
	FiberPool::FiberPool(size_t NumberOfFibers) :
		m_NumberOfFibers(NumberOfFibers)
	{
		for (size_t i = 0; i < NumberOfFibers; ++i)
		{
			g_ScriptManager.AddScript(std::make_unique<Script>(&FiberFunction));
		}

		g_FiberPool = this;
	}

	FiberPool::~FiberPool()
	{
		g_FiberPool = nullptr;
	}

	void FiberPool::QueueJob(std::function<void()> Function)
	{
		if (Function)
		{
			std::lock_guard lock(m_Mutex);

			m_Jobs.push(std::move(Function));
		}
	}

	void FiberPool::FiberTick()
	{
		std::unique_lock lock(m_Mutex);

		if (!m_Jobs.empty())
		{
			auto Job = std::move(m_Jobs.top());
			m_Jobs.pop();
			lock.unlock();

			std::invoke(std::move(Job));
		}
	}

	void FiberPool::FiberFunction()
	{
		while (true)
		{
			g_FiberPool->FiberTick();
			Script::GetCurrent()->YieldScript();
		}
	}

	int FiberPool::GetTotalFibers()
	{
		return m_NumberOfFibers;
	}

	int FiberPool::GetUsedFibers()
	{
		m_Jobs.size();
	}

	void FiberPool::reset()
	{
		std::lock_guard lock(m_Mutex);

		while (!m_Jobs.empty())
			m_Jobs.pop();
	}
}