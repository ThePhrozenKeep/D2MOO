#pragma once 

#pragma pack(1)

enum D2C_PlayerModes
{
	PLRMODE_DEATH,		// DT
	PLRMODE_NEUTRAL,	// NU
	PLRMODE_WALK,		// WL
	PLRMODE_RUN,		// RN
	PLRMODE_GETHIT,		// GH
	PLRMODE_TNEUTRAL,	// TN
	PLRMODE_TWALK,		// TW
	PLRMODE_ATTACK1,	// A1
	PLRMODE_ATTACK2,	// A2
	PLRMODE_BLOCK,		// BL
	PLRMODE_CAST,		// SC
	PLRMODE_THROW,		// TH
	PLRMODE_KICK,		// KK
	PLRMODE_SPECIAL1,	// S1
	PLRMODE_SPECIAL2,	// S2
	PLRMODE_SPECIAL3,	// S3
	PLRMODE_SPECIAL4,	// S4
	PLRMODE_DEAD,		// DD
	PLRMODE_SEQUENCE,	// SQ
	PLRMODE_KNOCKBACK	// KB
};

enum D2C_PlayerClasses
{
	PCLASS_AMAZON,
	PCLASS_SORCERESS,
	PCLASS_NECROMANCER,
	PCLASS_PALADIN,
	PCLASS_BARBARIAN,
	PCLASS_DRUID,
	PCLASS_ASSASSIN,
	PCLASS_EVILFORCE,
	PCLASS_INVALID = 7,
	NUMBER_OF_PLAYERCLASSES = 7,
};

enum D2C_PlayerFlags
{
	PLRFLAG_NEWBIE = 0x01,
	PLRFLAG_HARDCORE = 0x04,
	PLRFLAG_DEAD = 0x08,
	PLRFLAG_EXPANSION = 0x20,
	PLRFLAG_LADDER = 0x40,
};

struct D2PlayerCountBonusStrc
{
	int32_t nHp;								//0x00
	int32_t nExperience;						//0x04
	int32_t nMonsterSkillBonus;					//0x08
	int32_t nDifficulty;						//0x0C
	int32_t nPlayerCount;						//0x10
};

struct D2PlayerTradeStrc
{
	int32_t nSaveLength;						//0x00
	uint8_t* pSaveData;						//0x04
	int32_t unk0x08[4];
};

struct D2PlayerDataStrc
{
	char szName[16];						//0x00
	D2BitBufferStrc* pQuestData[3];			//0x10
	D2WaypointDataStrc* pWaypointData[3];	//0x1C
	uint32_t unk0x28;							//0x28
	int32_t nPortalFlags;						//0x2C
	int32_t unk0x30;							//0x30
	D2ArenaUnitStrc* pArenaUnit;			//0x34
	uint32_t unk0x38[4];						//0x38
	uint32_t dwUniqueId;						//0x48
	uint32_t dwTradeTick;						//0x4C
	uint32_t dwTradeState;						//0x50
	uint32_t unk0x54;							//0x54
	uint32_t dwAcceptTradeTick;				//0x58
	D2PlayerTradeStrc* pTrade;				//0x5C
	uint32_t unk0x60[3];						//0x60
	uint32_t dwBoughtItemId;					//0x6C
	int32_t nRightSkillId;						//0x70
	int32_t nLeftSkillId;						//0x74
	int32_t nRightSkillFlags;					//0x78
	int32_t nLeftSkillFlags;					//0x7C
	int32_t nSwitchRightSkillId;				//0x80
	int32_t nSwitchLeftSkillId;					//0x84
	int32_t nSwitchRightSkillFlags;				//0x88
	int32_t nSwitchLeftSkillFlags;				//0x8C
	int32_t nWeaponGUID;						//0x90
	uint32_t unk0x94[2];						//0x94
	D2ClientStrc* pClient;					//0x9C
	uint32_t unk0xA0[48];						//0xA0
	uint32_t dwHostileDelay;					//0x160
	uint32_t unk0x164;							//0x164
	uint32_t dwGameFrame;						//0x168
};


#pragma pack()
