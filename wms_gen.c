#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "data.h"
#include "wms_gen.h"

int prettify(const char *input_34, char *output) {
  sprintf(output, "%.5s %.7s %.5s\n%.5s %.7s %.5s",
          input_34,      // chars 0-4
          input_34 + 5,  // chars 5-11
          input_34 + 12, // chars 12-16
          input_34 + 17, // chars 17-21
          input_34 + 22, // chars 22-28
          input_34 + 29  // chars 29-33
  );
  return 0;
}

int scramble_string(const char *input_34, char *output_34,
                    const uint8_t *swap_table) {
  for (int i = 0; i < 34; i++) {
    int target_pos = swap_table[i];
    output_34[target_pos] = input_34[i];
  }
  output_34[34] = '\0';

  return 0;
}

int bits_to_chars(const char *bit_stream_170, char *output_34_chars) {
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

  return 0;
}

int encryption_bitstream(const char *input_136_bits, char *output_170_bits) {
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

  return 0;
}

int get_reset_byte(uint32_t full_checksum) {
  uint8_t checksum_byte = full_checksum;
  int reset = (checksum_byte / 16) + 8 + (checksum_byte % 16);

  if (reset < 17) {
    return reset;
  }
  return -1;
}

int get_encryption_entries(uint8_t checksum_byte, uint8_t *entries) {
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

  return 0;
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

int init_crc32_table(void) {
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

  return 0;
}

int wonder_to_bits(struct WonderMailData *wondermail_data, char *output) {
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

  return 0;
}

int dec_to_bit(uint32_t num, int bits, char *output) {
  for (int i = bits - 1; i >= 0; i--) {
    if ((num >> i) & 1) {
      output[bits - 1 - i] = '1';
    } else {
      output[bits - 1 - i] = '0';
    }
  }
  output[bits] = '\0';

  return 0;
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
