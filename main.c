#include "d_data.h"
#include "i_data.h"
#include "m_data.h"
#include "p_data.h"
#include "r_data.h"
#include <stdio.h>

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

// Multi step process
// Menu systems (States?)
// Mission Type: numbered input
// Dungeon: numbered input
// Floor: unsigned integer (cannot have negative floors)
// Client: Pokemon Ids?
// Target (Pokemon): Find someone, steal from someone etc.
// Target Item: Find someone with item, find  item somewhere on floor
// Reward Type: Enum of reward type like Item + ??? etc.
// Reward: Can Only be single item (dependant on reward type)
// Yes No for Euro version of game as codes are different.

int clear_buffer(void);
int confirm_dialogue(void);
int user_input(void);

int main(void) {
  printf("Wondermail-S Generator\n");

  // Mission Type
  printf("Mission Types:\n");
  DISPLAY_ARRAY(mission_types, MISSION_TYPE_COUNT, name);
  printf("Select a Mission Type:\n> ");

  int mission_type = user_input() - 1;
  printf("%s\n", mission_types[mission_type].name);

  printf("Dungeons:\n");
  DISPLAY_ARRAY(dungeons, DUNGEON_COUNT, name);
  printf("Select a Dungeon:\n> ");
  int dungeon = user_input() - 1;

  int floor;
  if (mission_types[mission_type].special_floor) {
    floor = mission_types[mission_type].special_floor;
  } else {
    printf("Floor:\n> ");
    floor = user_input() - 1;
  }
  int client;
  int client_gender;

  if (mission_types[mission_type].force_client) {
    client = mission_types[mission_type].force_client;
  } else {
    printf("Client:\n");
    DISPLAY_ARRAY(pokemon_list, POKEMON_COUNT, name);
    printf("Select a Client:\n> ");
    client = user_input() - 1;

    printf("Female?(Y/n)\n> ");
    client_gender = confirm_dialogue(); // Male
  }

  int target;
  int target_gender;

  // If Statements or switches need to be used to toggle Target
  if (mission_types[mission_type].client_is_target) {
    target = client;
    target_gender = client_gender;
  } else if (mission_types[mission_type].force_target) {
    target = mission_types[mission_type].force_target;
  } else {
    printf("Select a Target:\n> ");
    target = user_input() - 1;
    printf("Female?(Y/n)\n> ");
    target_gender = confirm_dialogue();
  }

  // If statements or switches needed for target items
  if (mission_types[mission_type].use_target_item) {
    printf("Target Item:\n");
    DISPLAY_ARRAY(items, ITEM_COUNT, name);
    printf("Select an Item:\n> ");
    int target_item = user_input() - 1;
  }

  // Only if Reward item is there
  if (!mission_types[mission_type].no_reward) {
    printf("Reward Type:\n");
    DISPLAY_ARRAY(reward_types, REWARD_TYPE_COUNT, name);

    printf("Select a Reward Type:\n> ");
    int reward_type = user_input() - 1;

    printf("Select a Reward Item:\n> ");
    int reward_item = user_input() - 1;
  }

  printf("Euro Version?\n> ");
  int euro_version_check = confirm_dialogue();

  // generator here with all values

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

int toUpper(char input_char) {
  return (input_char >= 'a' && input_char <= 'z') ? (input_char - ('a' - 'A'))
                                                  : input_char;
}

int confirm_dialogue(void) {
  char choice = getchar();

  toUpper(choice);

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
