#pragma once
#include "../Common.hpp"

namespace Onward::Features::Local
{
	inline void LocalFeaturesTick()
	{
		while (g_Running)
		{
			Script::GetCurrent()->YieldScript(5s);
			g_Logger->Custom(eLogColor::Yellow, "Local Features", "Hello from local feature fiber. That Took 5s.");
		}
	}
}