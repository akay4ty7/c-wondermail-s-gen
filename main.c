#include "d_data.h"
#include "i_data.h"
#include "m_data.h"
#include "p_data.h"
#include "r_data.h"
#include <stdio.h>

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
int mission_type_selection(void);

int main(void) {
  printf("Wondermail-S Generator\n");
  mission_type_selection();
  int mission_type = getchar();

  return 0;
}

int mission_type_selection(void) {
  printf("Mission Type: \n");
  int space_flag = 0;
  for (int i = 0; i < MISSION_TYPE_COUNT; i++) {
    printf("%d. %-30s", i + 1, mission_types[i].name);

    space_flag++;

    if (space_flag == 1) {
      printf("\t");
    }

    if (space_flag == 2) {
      printf("\n");
      space_flag = 0;
    }
  }

  printf("Please select a Mission Type:\n");

  return 0;
}
