#pragma once
#include "../Common.hpp"

namespace Onward
{
	class FiberPool
	{
	public:
		explicit FiberPool(size_t NumberOfFibers);
		~FiberPool();

		void QueueJob(std::function<void()> Function);

		void FiberTick();
		static void FiberFunction();

		int GetTotalFibers();
		int GetUsedFibers();
		
		void reset();

	private:
		std::recursive_mutex m_Mutex;
		std::stack<std::function<void()>> m_Jobs;
		int m_NumberOfFibers;
	};

	inline FiberPool* g_FiberPool{};
}