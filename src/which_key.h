#pragma once

#include <OBSE/OBSE.h>
#include <REX/REX.h>

#include "types.h"

namespace WhichKey {
struct KeyManager : REX::Singleton<KeyManager> {
  RE::TESKey *key;

  void SetSavedKey(RE::TESKey *a_key) { this->key = a_key; }
  RE::TESKey *GetSavedKey() { return this->key; }
};

RE::PlayerCharacter *GetPlayerCharacter() {
  static RE::PlayerCharacter *ptr = RE::TESForm::LookupByID<RE::PlayerCharacter>(0x14);
  return ptr;
}
} // namespace WhichKey