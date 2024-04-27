#pragma once

#pragma pack(1)


enum D2C_AiSpecialState : uint32_t
{
	AISPECIALSTATE_NONE = 0,
	AISPECIALSTATE_NO_MON_STATS = 1,					// Set when classid > nMonStatsRecord
	AISPECIALSTATE_2 = 2,								// Unused
	AISPECIALSTATE_3 = 3,								// Unused
	AISPECIALSTATE_4 = 4,								// Unused
	AISPECIALSTATE_ROGUE_HIREABLE_NON_PLAYER_OWNER = 5,
	AISPECIALSTATE_HIREABLE_NON_PLAYER_OWNER = 6,
	AISPECIALSTATE_REVIVED = 7,							// as necromancer pet
	AISPECIALSTATE_8 = 8,								// Unused
	AISPECIALSTATE_9 = 9,								// Unused
	// Curses
	AISPECIALSTATE_DIMVISION = 10,
	AISPECIALSTATE_TERROR = 11,
	AISPECIALSTATE_TAUNT = 12,

	AISPECIALSTATE_COUNTESS = 13,
	AISPECIALSTATE_14 = 14,				// Unused
	AISPECIALSTATE_WHIPPED = 15,		// When whipped by overseer, for suicide minions
	AISPECIALSTATE_MOUNTING_UNIT = 16,	// When a unit is mounting another (tower / siege beast)
	AISPECIALSTATE_17 = 17,				// Unused

	AISPECIALSTATE_TABLE_COUNT = 18,
	AISPECIALSTATE_INVALID = 19,		// Could be for objects ?
};

#pragma pack()
