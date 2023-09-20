#pragma once
#include "../Common.hpp"

namespace Onward
{
	class UIManager
	{
	public:
		void OnTick();

		bool m_Opened = true;
	};

	namespace UI
	{
		UIManager* GetInstance();
	}

	inline UIManager* g_UIManager;
}