#pragma once
#include "../Common.hpp"

#include "../Script/ScriptManager.hpp"

namespace Onward
{
	class FeatureManager
	{
	public:
		explicit FeatureManager();
		~FeatureManager();
	};

	inline FeatureManager* g_FeatureManager{};
}