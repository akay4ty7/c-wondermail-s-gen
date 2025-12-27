#include "d_data.h"
#include "i_data.h"
#include "m_data.h"
#include "p_data.h"
#include "r_data.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

static const char bit_values[] = "&67NPR89F0+#STXY45MCHJ-K12=%3Q@W";

static const uint8_t encryption_data[256] = {
    0x2E, 0x75, 0x3F, 0x99, 0x09, 0x6C, 0xBC, 0x61, 0x7C, 0x2A, 0x96, 0x4A,
    0xF4, 0x6D, 0x29, 0xFA, 0x90, 0x14, 0x9D, 0x33, 0x6F, 0xCB, 0x49, 0x3C,
    0x48, 0x80, 0x7B, 0x46, 0x67, 0x01, 0x17, 0x59, 0xB8, 0xFA, 0x70, 0xC0,
    0x44, 0x78, 0x48, 0xFB, 0x26, 0x80, 0x81, 0xFC, 0xFD, 0x61, 0x70, 0xC7,
    0xFE, 0xA8, 0x70, 0x28, 0x6C, 0x9C, 0x07, 0xA4, 0xCB, 0x3F, 0x70, 0xA3,
    0x8C, 0xD6, 0xFF, 0xB0, 0x7A, 0x3A, 0x35, 0x54, 0xE9, 0x9A, 0x3B, 0x61,
    0x16, 0x41, 0xE9, 0xA3, 0x90, 0xA3, 0xE9, 0xEE, 0x0E, 0xFA, 0xDC, 0x9B,
    0xD6, 0xFB, 0x24, 0xB5, 0x41, 0x9A, 0x20, 0xBA, 0xB3, 0x51, 0x7A, 0x36,
    0x3E, 0x60, 0x0E, 0x3D, 0x02, 0xB0, 0x34, 0x57, 0x69, 0x81, 0xEB, 0x67,
    0xF3, 0xEB, 0x8C, 0x47, 0x93, 0xCE, 0x2A, 0xAF, 0x35, 0xF4, 0x74, 0x87,
    0x50, 0x2C, 0x39, 0x68, 0xBB, 0x47, 0x1A, 0x02, 0xA3, 0x93, 0x64, 0x2E,
    0x8C, 0xAD, 0xB1, 0xC4, 0x61, 0x04, 0x5F, 0xBD, 0x59, 0x21, 0x1C, 0xE7,
    0x0E, 0x29, 0x26, 0x97, 0x70, 0xA9, 0xCD, 0x18, 0xA3, 0x7B, 0x74, 0x70,
    0x96, 0xDE, 0xA6, 0x72, 0xDD, 0x13, 0x93, 0xAA, 0x90, 0x6C, 0xA7, 0xB5,
    0x76, 0x2F, 0xA8, 0x7A, 0xC8, 0x81, 0x06, 0xBB, 0x85, 0x75, 0x11, 0x0C,
    0xD2, 0xD1, 0xC9, 0xF8, 0x81, 0x70, 0xEE, 0xC8, 0x71, 0x53, 0x3D, 0xAF,
    0x76, 0xCB, 0x0D, 0xC1, 0x56, 0x28, 0xE8, 0x3C, 0x61, 0x64, 0x4B, 0xB8,
    0xEF, 0x3B, 0x41, 0x09, 0x72, 0x07, 0x50, 0xAD, 0xF3, 0x2E, 0x5C, 0x43,
    0xFF, 0xC3, 0xB3, 0x32, 0x7A, 0x3E, 0x9C, 0xA3, 0xC2, 0xAB, 0x10, 0x60,
    0x99, 0xFB, 0x08, 0x8A, 0x90, 0x57, 0x8A, 0x7F, 0x61, 0x90, 0x21, 0x88,
    0x55, 0xE8, 0xFC, 0x4B, 0x0D, 0x4A, 0x7A, 0x48, 0xC9, 0xB0, 0xC7, 0xA6,
    0xD0, 0x04, 0x7E, 0x05

};

static const uint8_t byte_swap_us[34] = {
    0x07, 0x1B, 0x0D, 0x1F, 0x15, 0x1A, 0x06, 0x01, 0x17, 0x1C, 0x09, 0x1E,
    0x0A, 0x20, 0x10, 0x21, 0x0F, 0x08, 0x1D, 0x11, 0x14, 0x00, 0x13, 0x16,
    0x05, 0x12, 0x0E, 0x04, 0x03, 0x18, 0x02, 0x0B, 0x0C, 0x19};

static const uint8_t byte_swap_eu[34] = {
    0x0E, 0x04, 0x03, 0x18, 0x09, 0x1E, 0x0A, 0x20, 0x10, 0x21, 0x14, 0x00,
    0x13, 0x16, 0x05, 0x12, 0x06, 0x01, 0x17, 0x1C, 0x07, 0x1B, 0x0D, 0x1F,
    0x15, 0x1A, 0x02, 0x0B, 0x0C, 0x19, 0x0F, 0x08, 0x1D, 0x11};

static uint32_t crc32_table[256];

struct WonderMailData {
  uint8_t null_bits;        // = 0 (always)
  uint8_t special_floor;    // = 0 (not used for this mission)
  uint8_t floor;            // = 5
  uint8_t dungeon;          // = 0x01 (Beach Cave)
  uint32_t flavor_text;     // = 350000 (random number 300000-400000)
  uint16_t restriction;     // = 0 (not used)
  uint8_t restriction_type; // = 0
  uint16_t reward;          // = 109 (Apple - default)
  uint8_t reward_type;      // = 0 (Cash)
  uint16_t target_item;     // = 109 (Apple - default)
  uint16_t target2;         // = 0 (not used)
  uint16_t target;          // = 1 (Bulbasaur)
  uint16_t client;          // = 1 (Bulbasaur)
  uint8_t mission_special;  // = 0 (Normal)
  uint8_t mission_type;     // = 0 (Rescue client)
  uint8_t mail_type;        // = 4 (always 4 for Wonder Mail S)
};

int clear_buffer(void);
int confirm_dialogue(void);
int user_input(void);
uint16_t get_true_pokemon_id(uint16_t base_id, int is_female);
void dec_to_bit(uint32_t num, int bits, char *output);
void wonder_to_bits(struct WonderMailData *wondermail_data, char *output);
void init_crc32_table(void);
uint32_t calculate_crc32(const char *bit_string, int length);
void get_encryption_entries(uint8_t checksum_byte, uint8_t *entries);
int get_reset_byte(uint32_t full_checksum);
void encryption_bitstream(const char *input_136_bits, char *output_170_bits);
void bits_to_chars(const char *bit_stream_170, char *output_34_chars);
void scramble_string(const char *input_34, char *output_34,
                     const uint8_t *swap_table);
void prettify(const char *input_34, char *output);

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

  wondermail_data.flavor_text = 397371;
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

void prettify(const char *input_34, char *output) {
  // Format: "XX XX XX XX XX XX XX" (7 groups per line, 2 lines)
  sprintf(output, "%.5s %.7s %.5s\n%.5s %.7s %.5s",
          input_34,      // chars 0-4
          input_34 + 5,  // chars 5-11
          input_34 + 12, // chars 12-16
          input_34 + 17, // chars 17-21
          input_34 + 22, // chars 22-28
          input_34 + 29  // chars 29-33
  );
}

void scramble_string(const char *input_34, char *output_34,
                     const uint8_t *swap_table) {
  for (int i = 0; i < 34; i++) {
    int target_pos = swap_table[i];
    output_34[target_pos] = input_34[i];
  }
  output_34[34] = '\0';
}

void bits_to_chars(const char *bit_stream_170, char *output_34_chars) {
  int num_chars = 34;

  for (int i = 0; i < num_chars; i++) {
    int bit_pos = (num_chars - i - 1) * 5;

    char five_bits[6];
    for (int j = 0; j < 5; j++) {
      five_bits[j] = bit_stream_170[bit_pos + j];
    }
    five_bits[5] = '\0';

    uint8_t value = 0;
    for (int j = 0; j < 5; j++) {
      value = (value << 1) | (five_bits[j] - '0');
    }

    if (value < 32) {
      output_34_chars[i] = bit_values[value];
    } else {
      printf("Invalid 5-bit value: %d\n", value);
    }
  }
  output_34_chars[34] = '\0';
}

void encryption_bitstream(const char *input_136_bits, char *output_170_bits) {
  uint32_t checksum = calculate_crc32(input_136_bits, 136);

  // Extract 17 bytes read backwards
  uint8_t bytes[17];
  for (int i = 0; i < 17; i++) {
    bytes[i] = 0;
    int start_bit = (16 - i) * 8; // Start from last byte
    for (int j = 0; j < 8; j++) {
      bytes[i] = (bytes[i] << 1) | (input_136_bits[start_bit + j] - '0');
    }
  }

  // Get encryption entries
  uint8_t checksum_byte = checksum & 0xFF;
  uint8_t enc_entries[17];
  get_encryption_entries(checksum_byte, enc_entries);

  // Get reset byte
  int reset_byte = get_reset_byte(checksum);

  // Encrypt each byte
  int enc_ptr = 0;
  for (int i = 0; i < 17; i++) {
    if (enc_ptr == reset_byte) {
      enc_ptr = 0;
    }
    bytes[i] = (bytes[i] + enc_entries[enc_ptr]) & 0xFF;
    enc_ptr++;
  }

  // Build output: "00" + encrypted bytes + checksum
  strcpy(output_170_bits, "00");

  char temp[40];

  // Write encrypted bytes backwards
  for (int i = 16; i >= 0; i--) {
    dec_to_bit(bytes[i], 8, temp);
    strcat(output_170_bits, temp);
  }

  // Add checksum at end
  dec_to_bit(checksum, 32, temp);
  strcat(output_170_bits, temp);
}
int get_reset_byte(uint32_t full_checksum) {
  uint8_t checksum_byte = full_checksum;
  int reset = (checksum_byte / 16) + 8 + (checksum_byte % 16);

  if (reset < 17) {
    return reset;
  }
  return -1;
}

void get_encryption_entries(uint8_t checksum_byte, uint8_t *entries) {
  int backwards = !(checksum_byte & 0x01);
  int index = checksum_byte;

  for (int i = 0; i < 17; i++) {
    // Set the entry before modifying index
    entries[i] = encryption_data[index];

    if (backwards) {
      index--;
      if (index < 0) {
        index = 255;
      }
    } else {
      index++;
      if (index > 255) {
        index = 0;
      }
    }
  }
}

uint32_t calculate_crc32(const char *bit_string, int length) {
  if (length != 136) {
    printf("Invalid: CRC32 expects 136 bits, got %d\n", length);
  }

  uint32_t checksum = 0xFFFFFFFF;

  for (int i = 16; i >= 0; i--) {
    uint8_t byte_val = 0;
    for (int j = 0; j < 8; j++) {
      byte_val = (byte_val << 1) | (bit_string[i * 8 + j] - '0');
    }
    uint8_t table_index = (checksum ^ byte_val) & 0xFF;
    checksum = (checksum >> 8) ^ crc32_table[table_index];
  }

  checksum = checksum ^ 0xFFFFFFFF;
  return checksum;
}

void init_crc32_table(void) {
  for (int i = 0; i < 256; i++) {
    uint32_t entry = i;
    for (int j = 0; j < 8; j++) {
      if (entry & 1) {
        entry = 0xEDB88320 ^ (entry >> 1);
      } else {
        entry = entry >> 1;
      }
    }
    crc32_table[i] = entry;
  }
}

void wonder_to_bits(struct WonderMailData *wondermail_data, char *output) {
  char temp[40];
  output[0] = '\0';

  dec_to_bit(wondermail_data->null_bits, 8, temp); // 8 bits
  strcat(output, temp);

  dec_to_bit(wondermail_data->special_floor, 8, temp); // 8 bits
  strcat(output, temp);

  dec_to_bit(wondermail_data->floor, 8, temp); // 8 bits
  strcat(output, temp);

  dec_to_bit(wondermail_data->dungeon, 8, temp); // 8 bits
  strcat(output, temp);

  dec_to_bit(wondermail_data->flavor_text, 24, temp); // 24 bits
  strcat(output, temp);

  dec_to_bit(wondermail_data->restriction, 11, temp); // 11 bits
  strcat(output, temp);

  dec_to_bit(wondermail_data->restriction_type, 1, temp); // 1 bit
  strcat(output, temp);

  dec_to_bit(wondermail_data->reward, 11, temp); // 11 bits
  strcat(output, temp);

  dec_to_bit(wondermail_data->reward_type, 4, temp); // 4 bits
  strcat(output, temp);

  dec_to_bit(wondermail_data->target_item, 10, temp); // 10 bits
  strcat(output, temp);

  dec_to_bit(wondermail_data->target2, 11, temp); // 11 bits
  strcat(output, temp);

  dec_to_bit(wondermail_data->target, 11, temp); // 11 bits
  strcat(output, temp);

  dec_to_bit(wondermail_data->client, 11, temp); // 11 bits
  strcat(output, temp);

  dec_to_bit(wondermail_data->mission_special, 4, temp); // 4 bits
  strcat(output, temp);

  dec_to_bit(wondermail_data->mission_type, 4, temp); // 4 bits
  strcat(output, temp);

  dec_to_bit(wondermail_data->mail_type, 4, temp); // 4 bits (always value 4)
  strcat(output, temp);

  memmove(output, output + 2, 136);
  output[136] = '\0';

  printf("\n%s\n", output);
  return;
}

void dec_to_bit(uint32_t num, int bits, char *output) {
  for (int i = bits - 1; i >= 0; i--) {
    if ((num >> i) & 1) {
      output[bits - 1 - i] = '1';
    } else {
      output[bits - 1 - i] = '0';
    }
  }
  output[bits] = '\0';

  return;
}

uint16_t get_true_pokemon_id(uint16_t base_id, int is_female) {
  base_id = base_id % 600;

  const uint16_t nidoran_female = 0x18;
  const uint16_t nidoran_male = 0x20;

  if (base_id == nidoran_female || base_id == nidoran_male) {
    if (is_female) {
      return nidoran_female;
    } else {
      return nidoran_male;
    }
  }

  const uint16_t male_only[] = {0x205, 0x6A, 0x6B, 0x108, 0x190, 0x1C5,
                                0x22,  0x21, 0x80, 0x107, 0x155};

  for (int i = 0; i < 11; i++) {
    if (base_id == male_only[i] && is_female) {
      return base_id;
    }
  }

  const uint16_t female_only[] = {0x10D, 0x71,  0x212, 0x208, 0x1E2, 0x156,
                                  0x7C,  0x73,  0x19C, 0x10C, 0x01F, 0x01E,
                                  0x109, 0x1C7, 0x1C2, 0x1C3, 0x1C4};

  for (int i = 0; i < 17; i++) {
    if (base_id == female_only[i]) {
      return base_id;
    }
  }

  // Normal Pokemon: Female = ID + 600
  if (is_female) {
    return base_id + 600;
  }

  return base_id;
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

  choice = toUpper(choice);

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
