#include "Discord.hpp"

#include <Discord/discord_register.h>
#include <Discord/discord_rpc.h>

namespace Onward::Discord
{
	DiscordRichPresence Presence;
	void UpdatePresence()
	{
		DiscordRichPresence Presence;
		memset(&Presence, 0, sizeof(Presence));

		static int64_t Start = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		const char* Name = g_LocalPlayer.Name.c_str();

		Presence.state = "Offline";
		if (g_LocalPlayer.isOnline)
		{
			if (g_LocalPlayer.numConnectedPlayers == 32)
			{
				Presence.state = "Full Lobby";
			}
			else if (g_LocalPlayer.numConnectedPlayers == 1)
			{
				Presence.state = "Online Solo";
			}
		}

		std::string Detail = "";

		#ifdef ONWARD_DEBUG
			Detail.append("Devloper | RS:");
		#endif // ONWARD_DEBUG

		Detail.append(Name);

		Presence.details = Detail.c_str();
		Presence.largeImageKey = "e_gg";
		Presence.largeImageText = "Making GTAV Better";
		Presence.startTimestamp = Start;
		Discord_UpdatePresence(&Presence);
	}

	void Initialize()
	{
		DiscordEventHandlers handlers;
		memset(&handlers, 0, sizeof(handlers));
		Discord_Initialize("1137815513301520414", &handlers, TRUE, nullptr);
	}

	void Shutdown()
	{
		Discord_Shutdown();
	}
}