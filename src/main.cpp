#include "key_manager.h"
#include "types.h"

namespace Hooks {
static inline REL::Relocation<RE::REFR_LOCK *(RE::TESObjectREFR *)> GetLock{REL::Offset(0x65a8670)};
static inline REL::Relocation<const char *(RE::TESForm *)> GetFullName{REL::Offset(0x65A8170)};

static inline REL::Relocation<std::int32_t(RE::Actor *, RE::TESBoundObject *)> GetItemCountInContainer{
    REL::Offset(0x65DA780)};

// struct Hook_ShowText {
//   // we're deliberately ignoring that argument, that's the whole point of this hook
// #pragma warning(suppress : 4100)
//   static char ShowTextFunc(const char *text, RE::TESGameSoundHandle *handle, int isNotice, float showSeconds) {
//     return ShowTextFuncHook(text, handle, 1, showSeconds);
//   }

//   static inline REL::Hook ShowTextFuncHook{REL::Offset(0x67580a0), 0x108, ShowTextFunc};
// };

// struct Hook_CreateMessageMenu {
// #pragma warning(suppress : 4100)
//   static bool CreateMessageMenuFunc(void *pairingData, void *fp, int firstButtonResult, const char *message, int idk)
//   {
//     // do nothing
//     return false;
//   }

//   static inline REL::Hook CreateMessageMenuFuncHook{REL::Offset(0x67580a0), 0x286, CreateMessageMenuFunc};
// };

struct Hook_SPrintF {
  static std::int64_t BSStringSPrintFFunc(RE::BSString &outName, const char *fmtString, const char *arg) {

    auto key = WhichKey::KeyManager::GetSingleton()->GetSavedKey();
    if (key) {

      auto player = RE::TESForm::LookupByID<RE::PlayerCharacter>(0x14);

      auto playerHasKey = GetItemCountInContainer(player, key) > 0;
      if (playerHasKey) {
        if (auto keyName = GetFullName(key); keyName) {
          return BSStringSPrintFFuncHook(outName, fmtString, keyName);
        }
      }
    }

    return BSStringSPrintFFuncHook(outName, fmtString, arg);
  }

  static inline REL::Hook BSStringSPrintFFuncHook{REL::Offset(0x6912370), 0xEB6, BSStringSPrintFFunc};
};

struct Hook_SetInfoForRef {
  static RE::HUDInfoMenu *SetInfoForRefFunc(RE::TESObjectREFR *a_object, bool a_telekinesis, bool a_sameOkay) {
    auto lock = Hooks::GetLock(a_object);
    auto key = lock ? lock->key : nullptr;
    WhichKey::KeyManager::GetSingleton()->SetSavedKey(key);
    return SetInfoForRefFuncHook(a_object, a_telekinesis, a_sameOkay);
  }

  static inline REL::Hook SetInfoForRefFuncHook{REL::Offset(0x65ffbc0), 0x63, SetInfoForRefFunc};
};

} // namespace Hooks

OBSE_PLUGIN_LOAD(const OBSE::LoadInterface *a_obse) {
  OBSE::Init(a_obse, {.trampoline = true, .trampolineSize = 64});

  return true;
}
