#include "UIManager.hpp"

namespace Onward
{
	void UIManager::OnTick()
	{
		
	}

	namespace UI
	{
		UIManager* GetInstance()
		{
			static UIManager instance;
			return &instance;
		}
	}
}