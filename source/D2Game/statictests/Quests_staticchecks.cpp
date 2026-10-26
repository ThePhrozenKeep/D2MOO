
#include <QUESTS/Quests.h>
#include <cstddef>

#if defined(D2_VERSION_110F)
static_assert(sizeof(D2QuestDataStrc) == 0xF4, "Size of D2ClientStrc does not match v1.10f");
#elif defined(D2_VERSION_114D)
static_assert(sizeof(D2QuestDataStrc) == 0xF8, "Size of D2QuestDataStrc does not match v1.14d");
static_assert(offsetof(D2QuestDataStrc, tPlayerGUIDs) == 0x1C, "");
static_assert(offsetof(D2QuestDataStrc, nQuestFilter) == 0xE0, "");
static_assert(offsetof(D2QuestDataStrc, nRespecQuestFilter) == 0xE4, "");
static_assert(offsetof(D2QuestDataStrc, pfStatusFilter) == 0xE8, "");
static_assert(offsetof(D2QuestDataStrc, pfActiveFilter) == 0xEC, "");
static_assert(offsetof(D2QuestDataStrc, pfSeqFilter) == 0xF0, "");
static_assert(offsetof(D2QuestDataStrc, pPrev) == 0xF4, "");
static_assert(QUESTSTATEFLAG_RESPEC == 41 && MAX_QUEST_STATUS == 42 && QUEST_RESPEC == 30, "");
#endif
