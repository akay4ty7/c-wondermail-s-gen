#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "data.h"
#include "wms_gen.h"

#define FINAL_CODE_L 50
#define CHAR_S_US_L 35
#define BITS_L 200

// generic display
#define DISPLAY_ARRAY(arr, count, member)                                      \
  do {                                                                         \
    int space_flag = 0;                                                        \
    for (int i = 0; i < count; i++) {                                          \
      printf("%d. %-35s", i + 1, arr[i].member);                               \
      space_flag++;                                                            \
      if (space_flag % 2 == 1) {                                               \
        printf("\t");                                                          \
      }                                                                        \
      if (space_flag == 4) {                                                   \
        printf("\n");                                                          \
        space_flag = 0;                                                        \
      }                                                                        \
      if (i == count - 1) {                                                    \
        printf("\n");                                                          \
      }                                                                        \
    }                                                                          \
  } while (0)

int clear_buffer(void);
int confirm_dialogue(void);
int user_input(void);
int to_upper(char input_char);
int process_custom_mission(void);
int random_mission(int version);
int random_range_result(int min, int max);

int main(void) {
  printf("Wondermail-S Generator\n1.\tCustom Mission\n2.\tRandom "
         "Mission\n0.\tExit\n");
  while (1) {
    int input = user_input();

    srand(time(NULL));
    printf("> ");

    if (input == 1) {
      process_custom_mission();

      break;
    } else if (input == 2) {
      printf("Generation number\n> ");
      int amount = user_input();

      printf("Euro Version?(Y/n)\n> ");
      int version_check = confirm_dialogue();

      for (int i = 0; i < amount; i++) {
        printf("WONDERMAIL S PASSWORD %d\n", i + 1);
        random_mission(version_check);
      }

      break;
    } else if (input == 0) {

      break;
    } else {

      printf("Invalid input, try agian.\n");
    }
  }

  return 0;
}

int random_mission(int version_check) {
  struct WonderMailData wondermail_data = {0};
  // Defaults
  wondermail_data.mail_type = 4;
  wondermail_data.null_bits = 0;
  wondermail_data.flavor_text = 300000 + (rand() % 100000);
  wondermail_data.restriction = 0;
  wondermail_data.restriction_type = 0;
  wondermail_data.target2 = 0;

  // Mission Type
  int mission_index =
      random_range_result(1, MISSION_TYPE_COUNT) - 1; // Random range here
  wondermail_data.mission_type = mission_types[mission_index].main_type;
  wondermail_data.mission_special = mission_types[mission_index].special_type;

  // Dungeon

  int dungeon_index = random_range_result(1, DUNGEON_COUNT) - 1;
  wondermail_data.dungeon = dungeons[dungeon_index].id;

  // Floor
  if (mission_types[mission_index].special_floor) {
    wondermail_data.floor =
        random_range_result(1, dungeons[dungeon_index].max_floors);
    wondermail_data.special_floor = mission_types[mission_index].special_floor;
  } else if (wondermail_data.mission_type == 12) {
    wondermail_data.floor =
        random_range_result(1, dungeons[dungeon_index].max_floors);
    wondermail_data.special_floor =
        random_range_result(TREASURE_HUNT_FLOOR_MIN, TREASURE_HUNT_FLOOR_MAX);
  } else {
    wondermail_data.floor =
        random_range_result(1, dungeons[dungeon_index].max_floors);
  }

  // Client
  if (mission_types[mission_index].force_client) {
    wondermail_data.client = mission_types[mission_index].force_client;
  } else {
    int base_client_id =
        pokemon_list[random_range_result(1, POKEMON_COUNT) - 1].id;
    int client_gender = random_range_result(0, 1);
    wondermail_data.client = get_true_pokemon_id(base_client_id, client_gender);
  }

  // Target
  if (mission_types[mission_index].client_is_target) {
    wondermail_data.target = wondermail_data.client;
  } else if (mission_types[mission_index].force_target) {
    wondermail_data.target = mission_types[mission_index].force_target;
  } else {
    int base_target_id =
        pokemon_list[random_range_result(1, POKEMON_COUNT) - 1].id;
    int target_gender = random_range_result(0, 1);
    wondermail_data.target = get_true_pokemon_id(base_target_id, target_gender);
  }

  // Target Item
  if (mission_types[mission_index].use_target_item) {
    wondermail_data.target_item =
        items[random_range_result(1, ITEM_COUNT) - 1].id;
  } else {
    wondermail_data.target_item = 109; // Apple because game seems to complain
  }

  // Reward
  if (!mission_types[mission_index].no_reward) {
    wondermail_data.reward_type =
        reward_types[random_range_result(1, REWARD_TYPE_COUNT) - 1].id;

    if (wondermail_data.reward_type >= 1 && wondermail_data.reward_type <= 4) {
      wondermail_data.reward = items[random_range_result(1, ITEM_COUNT) - 1].id;
      if (wondermail_data.reward == 0) {
        wondermail_data.reward = 109;
      }
    } else if (wondermail_data.reward_type == 5 ||
               wondermail_data.reward_type == 6) {
      wondermail_data.reward = wondermail_data.client;
    } else {
      wondermail_data.reward = 109;
    }
  } else {
    wondermail_data.reward_type = 1;
    wondermail_data.reward = 109;
  }

  int euro_version_check = version_check;

  init_crc32_table();

  // ===== STAGE 1: 136-bit bitstream =====
  char bits[BITS_L];
  wonder_to_bits(&wondermail_data, bits);

  // ===== STAGE 2: Encrypted 170-bit bitstream =====
  char encrypted[BITS_L];
  encryption_bitstream(bits, encrypted);

  // ===== STAGE 3: Unscrambled characters =====
  char unscrambled[CHAR_S_US_L];
  bits_to_chars(encrypted, unscrambled);

  const uint8_t *swap_table;

  if (euro_version_check) {
    swap_table = byte_swap_eu;
  } else {
    swap_table = byte_swap_us;
  }
  char scrambled[CHAR_S_US_L];
  scramble_string(unscrambled, scrambled, swap_table);

  char final_code[FINAL_CODE_L];
  prettify(scrambled, final_code);

  printf("Dungeon Name: %s\nDungeon Floor: %d\nMax Dungeon Floor: "
         "%d\nWONDERMAIL S PASSWORD\n%s\n\n",
         dungeons[dungeon_index].name, wondermail_data.floor,
         dungeons[dungeon_index].max_floors, final_code);

  return 0;
}

int process_custom_mission(void) {
  struct WonderMailData wondermail_data = {0};
  // Defaults
  wondermail_data.mail_type = 4;
  wondermail_data.null_bits = 0;
  wondermail_data.flavor_text = 300000 + (rand() % 100000);
  wondermail_data.restriction = 0;
  wondermail_data.restriction_type = 0;
  wondermail_data.target2 = 0;

  // Mission Type
  printf("Mission Types:\n");
  DISPLAY_ARRAY(mission_types, MISSION_TYPE_COUNT, name);
  printf("Select a Mission Type:\n> ");

  int mission_index = user_input() - 1;
  wondermail_data.mission_type = mission_types[mission_index].main_type;
  wondermail_data.mission_special = mission_types[mission_index].special_type;

  // Dungeon
  printf("Dungeons:\n");
  DISPLAY_ARRAY(dungeons, DUNGEON_COUNT, name);
  printf("Select a Dungeon:\n> ");
  int dungeon_index = user_input() - 1;
  wondermail_data.dungeon = dungeons[dungeon_index].id;

  // Floor
  while (1) {
    printf("Floor (max = %d):\n> ", dungeons[dungeon_index].max_floors);
    if (mission_types[mission_index].special_floor) {
      wondermail_data.floor = user_input();
      wondermail_data.special_floor =
          mission_types[mission_index].special_floor;
    } else if (wondermail_data.mission_type == 12) {
      wondermail_data.floor = user_input();
      wondermail_data.special_floor =
          random_range_result(TREASURE_HUNT_FLOOR_MIN, TREASURE_HUNT_FLOOR_MAX);
    } else {
      wondermail_data.floor = user_input();
    }

    if ((wondermail_data.floor > dungeons[dungeon_index].max_floors)) {
      printf("Dungeon floor is too high, try again\n");
    } else {
      break;
    }
  }

  // Client
  if (mission_types[mission_index].force_client) {
    wondermail_data.client = mission_types[mission_index].force_client;
  } else {
    printf("Client:\n");
    DISPLAY_ARRAY(pokemon_list, POKEMON_COUNT, name);
    printf("Select a Client:\n> ");
    int base_client_id = pokemon_list[user_input() - 1].id;
    printf("Female?(Y/n)\n> ");
    int client_gender = confirm_dialogue();
    wondermail_data.client = get_true_pokemon_id(base_client_id, client_gender);
  }

  // Target
  if (mission_types[mission_index].client_is_target) {
    wondermail_data.target = wondermail_data.client;
  } else if (mission_types[mission_index].force_target) {
    wondermail_data.target = mission_types[mission_index].force_target;
  } else {
    printf("Target:\n");
    DISPLAY_ARRAY(pokemon_list, POKEMON_COUNT, name);
    printf("Select a Target:\n> ");
    int base_target_id = pokemon_list[user_input() - 1].id;
    printf("Female?(Y/n)\n> ");
    int target_gender = confirm_dialogue();
    wondermail_data.target = get_true_pokemon_id(base_target_id, target_gender);
  }

  // Target Item
  if (mission_types[mission_index].use_target_item) {
    printf("Target Item:\n");
    DISPLAY_ARRAY(items, ITEM_COUNT, name);
    printf("Select an Item:\n> ");
    wondermail_data.target_item = items[user_input() - 1].id;
  } else {
    wondermail_data.target_item = 109; // Apple because game seems to complain
  }

  // Reward
  if (!mission_types[mission_index].no_reward) {
    printf("Reward Type:\n");
    DISPLAY_ARRAY(reward_types, REWARD_TYPE_COUNT, name);
    printf("Select a Reward Type:\n> ");
    wondermail_data.reward_type = reward_types[user_input() - 1].id;

    if (wondermail_data.reward_type >= 1 && wondermail_data.reward_type <= 4) {
      printf("Reward Item:\n");
      DISPLAY_ARRAY(items, ITEM_COUNT, name);
      printf("Select a Reward Item:\n> ");
      wondermail_data.reward = items[user_input() - 1].id;
      if (wondermail_data.reward == 0) {
        wondermail_data.reward = 109;
      }
    } else if (wondermail_data.reward_type == 5 ||
               wondermail_data.reward_type == 6) {
      wondermail_data.reward = wondermail_data.client;
    } else {
      wondermail_data.reward = 109;
    }
  } else {
    wondermail_data.reward_type = 1;
    wondermail_data.reward = 109;
  }

  printf("Euro Version?(Y/n)\n> ");
  int euro_version_check = confirm_dialogue();

  init_crc32_table();

  // ===== STAGE 1: 136-bit bitstream =====
  char bits[BITS_L];
  wonder_to_bits(&wondermail_data, bits);

  // ===== STAGE 2: Encrypted 170-bit bitstream =====
  char encrypted[BITS_L];
  encryption_bitstream(bits, encrypted);

  // ===== STAGE 3: Unscrambled characters =====
  char unscrambled[CHAR_S_US_L];
  bits_to_chars(encrypted, unscrambled);

  const uint8_t *swap_table;

  if (euro_version_check) {
    swap_table = byte_swap_eu;
  } else {
    swap_table = byte_swap_us;
  }
  char scrambled[CHAR_S_US_L];
  scramble_string(unscrambled, scrambled, swap_table);

  char final_code[FINAL_CODE_L];
  prettify(scrambled, final_code);

  printf("Dungeon Name: %s\nDungeon Floor: %d\nMax Dungeon Floor: "
         "%d\nWONDERMAIL S PASSWORD\n%s\n",
         dungeons[dungeon_index].name, wondermail_data.floor,
         dungeons[dungeon_index].max_floors, final_code);

  return 0;
}

int random_range_result(int min, int max) {
  return (rand() % (max - min + 1)) + min;
}
int user_input(void) {
  int input;

  while (1) {
    if (scanf("%d", &input) != 1) {
      printf("Invalid input, Try Again\n> ");
      clear_buffer();
    } else {
      break;
    }
  }
  clear_buffer();
  return input;
}

int to_upper(char input_char) {
  return (input_char >= 'a' && input_char <= 'z') ? (input_char - ('a' - 'A'))
                                                  : input_char;
}

int confirm_dialogue(void) {
  char choice = getchar();

  choice = to_upper(choice);

  if (choice == 'Y') {
    return 1;
  } else if (choice == 'N') {
    return 0;
  } else {
    printf("Invalid Input\n");
    return -1;
  }
  return 0;
}

int clear_buffer(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
  }
  return 0;
}
