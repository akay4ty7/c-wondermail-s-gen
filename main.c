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

int main(void) {
  printf("Wondermail-S Generator\n");

  // Mission Type
  printf("Mission Types:\n");
  DISPLAY_ARRAY(mission_types, MISSION_TYPE_COUNT, name);
  printf("Select a Mission Type:\n> ");
  int mission_type = getchar();
  clear_buffer();

  printf("Dungeons:\n");
  DISPLAY_ARRAY(dungeons, DUNGEON_COUNT, name);
  printf("Select a Dungeon:\n> ");
  int dungeon = getchar();
  clear_buffer();

  printf("Floor:\n> ");
  int floor = getchar();
  clear_buffer();

  printf("Client:\n");
  DISPLAY_ARRAY(pokemon_list, POKEMON_COUNT, name);
  printf("Select a Client:\n> ");
  int client = getchar();
  clear_buffer();

  printf("Female?\n(Y/n)> ");
  int client_gender = confirm_dialogue(); // Male

  // If Statements or switches need to be used to toggle Target
  printf("Select a Target:\n> ");
  int target = getchar();
  clear_buffer();

  printf("Female?\n(Y/n)> ");
  int target_gender = confirm_dialogue(); // Male

  // If statements or switches needed for target Item
  printf("Target Item:\n");
  DISPLAY_ARRAY(items, ITEM_COUNT, name);
  int target_item = getchar();

  return 0;
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
