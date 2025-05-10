#pragma once

namespace RE {
struct HUDInfoMenu {};
struct REFR_LOCK {
  char base_level;
  TESKey *key;
  char flags;
};
struct BSExtraData {
  void *vtable;
  std::uint8_t EType;
  BSExtraData *next;
};
struct TESGameSoundHandle {};
struct InventoryChanges {};

struct InterfaceManager {
  uint8_t pad[0x128];
  uint8_t lastMessageButtonClicked;
};

}; // namespace RE