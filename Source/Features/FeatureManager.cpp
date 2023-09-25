#include "FeatureManager.hpp"

#include "Local.hpp"

namespace Onward
{
	FeatureManager::FeatureManager()
	{
		auto Scr = std::make_unique<Script>(&Features::Local::LocalFeaturesTick);

		g_ScriptManager.AddScript(std::move(Scr));

		g_FeatureManager = this;
	}

	FeatureManager::~FeatureManager()
	{
		g_FeatureManager = nullptr;
	}
}