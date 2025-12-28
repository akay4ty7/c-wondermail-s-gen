#ifndef DATA_H
#define DATA_H

#include <stdint.h>

struct Dungeon {
  int id;
  const char *name;
};

struct MissionType {
  int main_type;
  int special_type;
  const char *name;
  int use_target_item;
  int client_is_target;
  int force_client;
  int force_target;
  int special_floor;
  int no_reward;
};

struct RewardType {
  int id;
  const char *name;
};

struct Item {
  int id;
  const char *name;
};

struct Pokemon {
  int id;
  const char *name;
};

extern const struct Dungeon dungeons[];
extern const int DUNGEON_COUNT;

extern const struct MissionType mission_types[];
extern const int MISSION_TYPE_COUNT;

extern const struct RewardType reward_types[];
extern const int REWARD_TYPE_COUNT;

extern const struct Item items[];
extern const int ITEM_COUNT;

extern const struct Pokemon pokemon_list[];
extern const int POKEMON_COUNT;

extern uint32_t crc32_table[256];
extern const uint8_t encryption_data[256];
extern const char bit_values[];
extern const uint8_t byte_swap_us[34];
extern const uint8_t byte_swap_eu[34];
#endif
