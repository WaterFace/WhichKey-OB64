#pragma once

namespace RE {
struct HUDInfoMenu {};
struct TESFullName : BaseFormComponent {
  BSString fullName;
};
struct TESObjectREFR : TESForm {};
struct TESObjectMISC : TESBoundObject, TESFullName {};
struct TESKey : TESObjectMISC {};
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
struct BaseExtraList {
  void *vtable;
  BSExtraData *head;
  char flags[16];
};
struct TESGameSoundHandle {};
struct TESContainer {};
struct ExtraDataList : BaseExtraList {};
struct InventoryChanges {};
struct MobileObject : TESObjectREFR {};
struct Actor : MobileObject {};
struct Character : Actor {};
struct PlayerCharacter : Character {
  inline static constexpr auto RTTI = RTTI::PlayerCharacter;
  inline static constexpr auto VTABLE = VTABLE::PlayerCharacter;
  inline static constexpr auto FORMTYPE = FormType::ActorCharacter;
};

}; // namespace RE