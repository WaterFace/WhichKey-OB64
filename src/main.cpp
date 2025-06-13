#include "config.h"
#include "types.h"
#include "which_key.h"

namespace Hooks
{
	static inline REL::Relocation<RE::REFR_LOCK*(RE::TESObjectREFR*)> GetLock{ REL::ID(405461) };

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
					RE::InterfaceManager::GetInstance(false, true)->lastMessageButtonClicked = 1;
					fp();
				} else {
					// TODO: play sound?
				}
				return true;
			}
		}

		static inline REL::Hook CreateMessageMenuFuncHook{ "Hook_CreateMessageMenu_DOOR", REL::ID(409981), 0x44c, CreateMessageMenuFunc };
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
					RE::InterfaceManager::GetInstance(false, true)->lastMessageButtonClicked = 1;
					fp();
				} else {
					// TODO: play sound?
				}
				return true;
			}
		}

		static inline REL::Hook CreateMessageMenuFuncHook{ "Hook_CreateMessageMenu_CONT", REL::ID(417896), 0x177, CreateMessageMenuFunc };
	};

	struct Hook_SPrintF
	{
		static std::int64_t BSStringSPrintFFunc(RE::BSString& outName, const char* fmtString, const char* arg)
		{
			auto key = WhichKey::KeyManager::GetSingleton()->GetSavedKey();
			if (key && RE::PlayerCharacter::GetSingleton()->GetItemCountInContainer(key) > 0) {
				if (auto keyName = RE::TESFullName::GetFullName(key); keyName) {
					return BSStringSPrintFFuncHook(outName, fmtString, keyName);
				}
			}

			return BSStringSPrintFFuncHook(outName, fmtString, arg);
		}

		static inline ptrdiff_t Offset()
		{
			// temporary workaround for namespace nonsense
			constexpr REL::Version RUNTIME_1_511_102{ 1, 511, 102, 0 };
			REL::Version           runtimeVersion = REL::Module::GetSingleton()->version();

			ptrdiff_t offset = 0x0;
			if (runtimeVersion < RUNTIME_1_511_102) {
				offset = 0xEB6;
			} else {
				offset = 0xF69;
			}
			return offset;
		}

		static inline REL::Hook BSStringSPrintFFuncHook{ "Hook_SPrintF", REL::ID(416127), Offset(), BSStringSPrintFFunc };
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

		static inline REL::Hook SetInfoForRefFuncHook{ "Hook_SetInfoForRef", REL::ID(406425), 0x63, SetInfoForRefFunc };
	};

}  // namespace Hooks

OBSE_PLUGIN_LOAD(const OBSE::LoadInterface* a_obse)
{
	OBSE::Init(a_obse, { .trampoline = true, .trampolineSize = 64 });

	// initialize config
	WhichKey::Config::GetSingleton();

	return true;
}
