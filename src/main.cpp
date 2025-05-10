#include "config.h"
#include "types.h"
#include "which_key.h"

namespace Hooks
{
	static inline REL::Relocation<RE::REFR_LOCK*(RE::TESObjectREFR*)>            GetLock{ REL::Offset(0x65a8670) };
	static inline REL::Relocation<const char*(RE::TESForm*)>                     GetFullName{ REL::Offset(0x65A8170) };
	static inline REL::Relocation<std::int32_t(RE::Actor*, RE::TESBoundObject*)> GetItemCountInContainer{
		REL::Offset(0x65DA780)
	};
	static inline REL::Relocation<RE::InterfaceManager*(bool, std::uintptr_t)> InterfaceManagerInstance{
		REL::Offset(0x6603730)
	};

	struct Hook_CreateMessageMenu_DOOR
	{
		using callback_fp = void();
		static bool CreateMessageMenuFunc(
			const void*  pairingData,
			callback_fp* fp,
			int          firstButtonResult,
			const char*  message,
			uint64_t     ok)
		{
			if (WhichKey::Config::GetSingleton()->DoorMessageEnabled()) {
				return CreateMessageMenuFuncHook(pairingData, fp, firstButtonResult, message, ok);
			} else {
				if (WhichKey::Config::GetSingleton()->OpenDoorAfterUnlock()) {
					// call the callback directly to actually use the door after unlocking it
					// the callback expects this to be set, then clears it appropriately
					InterfaceManagerInstance(0, 1)->lastMessageButtonClicked = 1;
					fp();
				} else {
					// TODO: play sound?
				}
				return true;
			}
		}

		static inline REL::Hook CreateMessageMenuFuncHook{ REL::Offset(0x67580a0), 0x44c, CreateMessageMenuFunc };
	};

	struct Hook_CreateMessageMenu_CONT
	{
		using callback_fp = void();
		static bool CreateMessageMenuFunc(
			const char*  text,
			callback_fp* fp,
			int          firstButtonResult,
			const char*  message,
			uint64_t     ok)
		{
			if (WhichKey::Config::GetSingleton()->ContMessageEnabled()) {
				return CreateMessageMenuFuncHook(text, fp, firstButtonResult, message, ok);
			} else {
				if (WhichKey::Config::GetSingleton()->OpenContAfterUnlock()) {
					// call the callback directly to actually use the Container after unlocking it
					// the callback expects this to be set, then clears it appropriately
					InterfaceManagerInstance(0, 1)->lastMessageButtonClicked = 1;
					fp();
				} else {
					// TODO: play sound?
				}
				return true;
			}
		}

		static inline REL::Hook CreateMessageMenuFuncHook{ REL::Offset(0x6968e30), 0x177, CreateMessageMenuFunc };
	};

	struct Hook_SPrintF
	{
		static std::int64_t BSStringSPrintFFunc(RE::BSString& outName, const char* fmtString, const char* arg)
		{
			auto key = WhichKey::KeyManager::GetSingleton()->GetSavedKey();
			if (key) {
				auto player = RE::PlayerCharacter::GetSingleton();

				auto playerHasKey = GetItemCountInContainer(player, key) > 0;
				if (playerHasKey) {
					if (auto keyName = GetFullName(key); keyName) {
						return BSStringSPrintFFuncHook(outName, fmtString, keyName);
					}
				}
			}

			return BSStringSPrintFFuncHook(outName, fmtString, arg);
		}

		static inline REL::Hook BSStringSPrintFFuncHook{ REL::Offset(0x6912370), 0xEB6, BSStringSPrintFFunc };
	};

	struct Hook_SetInfoForRef
	{
		static RE::HUDInfoMenu* SetInfoForRefFunc(RE::TESObjectREFR* a_object, bool a_telekinesis, bool a_sameOkay)
		{
			auto lock = Hooks::GetLock(a_object);
			auto key = lock ? lock->key : nullptr;
			WhichKey::KeyManager::GetSingleton()->SetSavedKey(key);
			return SetInfoForRefFuncHook(a_object, a_telekinesis, a_sameOkay);
		}

		static inline REL::Hook SetInfoForRefFuncHook{ REL::Offset(0x65ffbc0), 0x63, SetInfoForRefFunc };
	};

}  // namespace Hooks

OBSE_PLUGIN_LOAD(const OBSE::LoadInterface* a_obse)
{
	OBSE::Init(a_obse, { .trampoline = true, .trampolineSize = 64 });

	// initialize config
	WhichKey::Config::GetSingleton();
	REX::INFO("door message enabled = {}", WhichKey::Config::GetSingleton()->DoorMessageEnabled());
	REX::INFO("cont message enabled = {}", WhichKey::Config::GetSingleton()->ContMessageEnabled());

	return true;
}
