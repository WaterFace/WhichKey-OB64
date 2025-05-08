#pragma once

#include <REX/REX.h>

namespace WhichKey {
class Config {
  REX::INI::Bool<REX::INI::SettingStore> door_message_enabled;
  REX::INI::Bool<REX::INI::SettingStore> cont_message_enabled;
  REX::INI::Bool<REX::INI::SettingStore> open_door_after_unlock;
  REX::INI::Bool<REX::INI::SettingStore> open_cont_after_unlock;
  Config()
      : door_message_enabled("messages", "door_message_enabled", false)
      , cont_message_enabled("messages", "cont_message_enabled", false)
      , open_door_after_unlock("actions", "open_door_after_unlock", true)
      , open_cont_after_unlock("actions", "open_cont_after_unlock", true) {
    auto store = REX::INI::SettingStore::GetSingleton();
    store->Init(INI_PATH, "");
    store->Load();
    store->Save();
  }

public:
  static constexpr const char *INI_PATH = "OBSE/Plugins/WhichKey-OB64.ini";
  static Config *GetSingleton() {
    static Config config = Config();
    return &config;
  }

  bool DoorMessageEnabled() { return door_message_enabled.GetValue(); }
  bool ContMessageEnabled() { return cont_message_enabled.GetValue(); }
  bool OpenDoorAfterUnlock() { return open_door_after_unlock.GetValue(); }
  bool OpenContAfterUnlock() { return open_cont_after_unlock.GetValue(); }
};
} // namespace WhichKey
