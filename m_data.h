#ifndef M_DATA_H
#define M_DATA_H

#define TREASURE_HUNT_FLOOR_MIN 114
#define TREASURE_HUNT_FLOOR_MAX 144

struct MissionType {
  int main_type;        // struct.mainType field (0-12)
  int special_type;     // struct.missionSpecial field
  const char *name;     // Display name for menu
  int use_target_item;  // Enable item selection for target_item
  int client_is_target; // If 1, target = client
  int force_client;     // Force specific client Pokemon (0 = none)
  int force_target;     // Force specific target Pokemon (0 = none)
  int special_floor;    // Special floor number (0 = none/random)
  int no_reward;        // Disable reward selection
};

static inline int mission_needs_item(const struct MissionType *mission_types);
static inline int
mission_has_forced_client(const struct MissionType *mission_types);
static inline int
mission_has_forced_target(const struct MissionType *mission_types);
static inline int
mission_disables_reward(const struct MissionType *mission_types);

const struct MissionType mission_types[] = {
    // Simple missions
    {0, 0, "Rescue client", 0, 1, 0, 0, 0, 0},
    {1, 0, "Rescue target", 0, 0, 0, 0, 0, 0},
    {2, 0, "Escort to target", 0, 0, 0, 0, 0, 0},

    // Explore with client variants
    {3, 0, "Explore - Normal", 0, 1, 0, 0, 0, 0},
    {3, 1, "Explore - Sealed Chamber", 0, 1, 0, 0, 165, 0},
    {3, 2, "Explore - Golden Chamber", 0, 1, 0, 0, 111, 0},

    // More simple missions
    {4, 0, "Prospect with client", 1, 1, 0, 0, 0, 0},
    {5, 0, "Guide client", 0, 1, 0, 0, 0, 0},
    {6, 0, "Find target item", 1, 1, 0, 0, 0, 0},
    {7, 0, "Deliver target item", 1, 1, 0, 0, 0, 0},
    {8, 0, "Search for client", 0, 0, 0, 0, 0, 0},

    // Steal from target variants
    {9, 0, "Steal - Normal", 1, 0, 0, 0, 0, 0},
    {9, 1, "Steal - Target hidden", 1, 0, 0, 0, 0, 0},
    {9, 2, "Steal - Target runs", 1, 0, 0, 0, 0, 0},

    // Challenge Request variants (skip subtype 0 - broken)
    {11, 1, "Challenge - Mewtwo", 0, 0, 150, 150, 145, 0},
    {11, 2, "Challenge - Entei", 0, 0, 271, 271, 146, 0},
    {11, 3, "Challenge - Raikou", 0, 0, 270, 270, 147, 0},
    {11, 4, "Challenge - Suicune", 0, 0, 272, 272, 148, 0},
    {11, 5, "Challenge - Jirachi", 0, 0, 417, 417, 149, 0},

    // Treasure hunt (special_floor picked randomly from 114-144)
    {12, 0, "Treasure hunt", 0, 0, 422, 422, 0, 1}};

const int MISSION_TYPE_COUNT = sizeof(mission_types) / sizeof(mission_types[0]);

// Helper inline functions
static inline int mission_needs_item(const struct MissionType *mission_types) {
  return mission_types->use_target_item;
}

static inline int
mission_has_forced_client(const struct MissionType *mission_types) {
  return mission_types->force_client != 0;
}

static inline int
mission_has_forced_target(const struct MissionType *mission_types) {
  return mission_types->force_target != 0;
}

static inline int
mission_disables_reward(const struct MissionType *mission_types) {
  return mission_types->no_reward;
}

#endif
