#ifndef WMS_GEN_H
#define WMS_GEN_H

#include <stdint.h>
// Structure for the wondermail
struct WonderMailData {
  uint8_t null_bits; // = 0 (always)
  uint8_t special_floor;
  uint8_t floor;
  uint8_t dungeon;
  uint32_t flavor_text; // = 350000 (random number 300000-400000)
  uint16_t restriction;
  uint8_t restriction_type;
  uint16_t reward; // = 109 (Apple - default)
  uint8_t reward_type;
  uint16_t target_item; // = 109 (Apple - default)
  uint16_t target2;
  uint16_t target;
  uint16_t client;
  uint8_t mission_special;
  uint8_t mission_type;
  uint8_t mail_type; // = 4 (always 4 for Wonder Mail S)
};

uint16_t get_true_pokemon_id(uint16_t base_id, int is_female);
int dec_to_bit(uint32_t num, int bits, char *output);
int wonder_to_bits(struct WonderMailData *wondermail_data, char *output);
int init_crc32_table(void);
uint32_t calculate_crc32(const char *bit_string, int length);
int get_encryption_entries(uint8_t checksum_byte, uint8_t *entries);
int get_reset_byte(uint32_t full_checksum);
int encryption_bitstream(const char *input_136_bits, char *output_170_bits);
int bits_to_chars(const char *bit_stream_170, char *output_34_chars);
int scramble_string(const char *input_34, char *output_34,
                    const uint8_t *swap_table);
int prettify(const char *input_34, char *output);

#endif
