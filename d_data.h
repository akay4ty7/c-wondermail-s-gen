#ifndef D_DATA_H
#define D_DATA_H

struct Dungeon {
  int id;
  const char *name;
};

const struct Dungeon dungeons[] = {{0x01, "Beach Cave"},
                                   {0x03, "Drenched Bluff"},
                                   {0x04, "Mt. Bristle"},
                                   {0x06, "Waterfall Cave"},
                                   {0x07, "Apple Woods"},
                                   {0x08, "Craggy Coast"},
                                   {0x0A, "Mt. Horn"},
                                   {0x0C, "Foggy Forest"},
                                   {0x0E, "Steam Cave"},
                                   {0x11, "Amp Plains"},
                                   {0x14, "Northern Desert"},
                                   {0x15, "Quicksand Cave"},
                                   {0x18, "Crystal Cave"},
                                   {0x19, "Crystal Crossing"},
                                   {0x22, "Treeshroud Forest"},
                                   {0x23, "Brine Cave"},
                                   {0x2C, "Mystifying Forest"},
                                   {0x2E, "Blizzard Island"},
                                   {0x2F, "Crevice Cave"},
                                   {0x32, "Surrounded Sea"},
                                   {0x33, "Miracle Sea"},
                                   {0x3E, "Mt. Travail"},
                                   {0x40, "Spacial Rift"},
                                   {0x43, "Dark Crater"},
                                   {0x46, "Concealed Ruins"},
                                   {0x48, "Marine Resort"},
                                   {0x49, "Bottomless Sea"},
                                   {0x4B, "Shimmer Desert"},
                                   {0x4D, "Mt. Avalanche"},
                                   {0x4F, "Giant Volcano"},
                                   {0x51, "World Abyss"},
                                   {0x53, "Sky Stairway"},
                                   {0x55, "Mystery Jungle"},
                                   {0x57, "Serenity River"},
                                   {0x58, "Landslide Cave"},
                                   {0x59, "Lush Prairie"},
                                   {0x5A, "Tiny Meadow"},
                                   {0x5B, "Labyrinth Cave"},
                                   {0x5C, "Oran Forest"},
                                   {0x5D, "Lake Afar"},
                                   {0x5E, "Happy Outlook"},
                                   {0x5F, "Mt. Mistral"},
                                   {0x60, "Shimmer Hill"},
                                   {0x61, "Lost Wilderness"},
                                   {0x62, "Midnight Forest"},
                                   {0x63, "Zero Isle North"},
                                   {0x64, "Zero Isle East"},
                                   {0x65, "Zero Isle West"},
                                   {0x66, "Zero Isle South"},
                                   {0x67, "Zero Isle Center"},
                                   {0x6B, "Oblivion Forest"},
                                   {0x6C, "Treacherous Waters"},
                                   {0x6D, "Southeastern Islands"},
                                   {0x6E, "Inferno Cave"}};

const int DUNGEON_COUNT = sizeof(dungeons) / sizeof(dungeons[0]);

#endif
