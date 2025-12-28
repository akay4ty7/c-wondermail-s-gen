#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "data.h"
#include "wms_gen.h"

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

int main(void) {
  srand(time(NULL));

  struct WonderMailData wondermail_data = {0};
  wondermail_data.mail_type = 4;
  wondermail_data.null_bits = 0;
  wondermail_data.flavor_text = 300000 + (rand() % 100000);
  wondermail_data.restriction = 0;
  wondermail_data.restriction_type = 0;
  wondermail_data.target2 = 0;

  printf("Wondermail-S Generator\n");

  // Mission Type
  printf("Mission Types:\n");
  DISPLAY_ARRAY(mission_types, MISSION_TYPE_COUNT, name);
  printf("Select a Mission Type:\n> ");

  int mission_index = user_input() - 1;
  wondermail_data.mission_type = mission_types[mission_index].main_type;
  wondermail_data.mission_special = mission_types[mission_index].special_type;

  printf("%s\n", mission_types[mission_index].name);

  // Dungeon
  printf("Dungeons:\n");
  DISPLAY_ARRAY(dungeons, DUNGEON_COUNT, name);
  printf("Select a Dungeon:\n> ");
  wondermail_data.dungeon = dungeons[user_input() - 1].id;

  // Floor
  printf("Floor:\n> ");
  if (mission_types[mission_index].special_floor) {
    wondermail_data.floor = user_input();
    wondermail_data.special_floor = mission_types[mission_index].special_floor;
  } else {
    wondermail_data.floor = user_input();
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

  printf("Euro Version?\n> ");
  int euro_version_check = confirm_dialogue();

  printf("\nUser Selection\nMission Type: %d\nDungeon: %d\nFloor: %d\nClient: "
         "%d\nTarget: %d\nReward Type: %d\nReward: %d\nFlavor Text: "
         "%d\nMission Special: %d\n",
         wondermail_data.mission_type, wondermail_data.dungeon,
         wondermail_data.floor, wondermail_data.client, wondermail_data.target,
         wondermail_data.reward_type, wondermail_data.reward,
         wondermail_data.flavor_text, wondermail_data.mission_special);

  init_crc32_table();

  // ===== STAGE 1: 136-bit bitstream =====
  char bits[200];
  wonder_to_bits(&wondermail_data, bits);

  // ===== STAGE 2: Encrypted 170-bit bitstream =====
  char encrypted[200];
  encryption_bitstream(bits, encrypted);

  // ===== STAGE 3: Unscrambled characters =====
  char unscrambled[35];
  bits_to_chars(encrypted, unscrambled);

  const uint8_t *swap_table;

  if (euro_version_check) {
    swap_table = byte_swap_eu;
  } else {
    swap_table = byte_swap_us;
  }
  char scrambled[35];
  scramble_string(unscrambled, scrambled, swap_table);

  char final_code[50];
  prettify(scrambled, final_code);

  printf("WONDERMAIL S PASSWORD\n%s\n", final_code);
  return 0;
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
