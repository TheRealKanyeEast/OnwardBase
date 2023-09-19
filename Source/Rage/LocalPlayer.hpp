#pragma once
#include "../Common.hpp"

namespace Onward
{
	struct LocalPlayer
	{
		std::string Name;
		bool isOnline;
		int numConnectedPlayers = 1;
	};
}