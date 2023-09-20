#pragma once
#include "../Common.hpp"

namespace Onward
{
	struct LocalPlayer
	{
		std::string Name = "Unknown";
		bool isOnline = false;
		int numConnectedPlayers = 1;
	};
}