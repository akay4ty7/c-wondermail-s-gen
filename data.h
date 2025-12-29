#ifndef DATA_H
#define DATA_H

#include <stdint.h>

#define BYTE_SWAP_L 34
#define CRC32_TABLE_L 256
#define ENCRYPTION_DATA_L 256

#define TREASURE_HUNT_FLOOR_MIN 114
#define TREASURE_HUNT_FLOOR_MAX 144

struct Dungeon {
  int id;
  const char *name;
  const int max_floors;
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

extern uint32_t crc32_table[CRC32_TABLE_L];
extern const uint8_t encryption_data[ENCRYPTION_DATA_L];

extern const char bit_values[];

extern const uint8_t byte_swap_us[BYTE_SWAP_L];
extern const uint8_t byte_swap_eu[BYTE_SWAP_L];
#endif
