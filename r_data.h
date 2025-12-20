#ifndef R_DATA_H
#define R_DATA_H

struct RewardType {
  int id;
  const char *name;
};

const struct RewardType reward_types[] = {
    {0, "Cash"},
    {1, "Cash + ??? (Reward Item)"},
    {2, "Item"},
    {3, "Item + ??? (Random)"},
    {4, "??? (Reward Item)"},
    {5, "??? (Egg)"},
    {6, "??? (Client Joins)"},
};

const int REWARD_TYPE_COUNT = sizeof(reward_types) / sizeof(reward_types[0]);
#endif
