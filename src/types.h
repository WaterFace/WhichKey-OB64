#pragma once

namespace RE
{
	struct REFR_LOCK
	{
		char    base_level;
		TESKey* key;
		char    flags;
	};

	struct InterfaceManager
	{
		uint8_t pad[0x128];
		uint8_t lastMessageButtonClicked;
	};

};  // namespace RE