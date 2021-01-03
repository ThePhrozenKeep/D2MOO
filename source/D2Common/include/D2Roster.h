#pragma once

#pragma pack(1)

enum D2C_RosterInfoFlags
{
	ROSTERINFOFLAG_LOOT = 0x01,
	ROSTERINFOFLAG_IGNORE = 0x02,
	ROSTERINFOFLAG_SQUELCH = 0x04,
	ROSTERINFOFLAG_HOSTILE = 0x08,
};

enum D2C_RosterControlFlags
{
	ROSTERCONTROLFLAG_INVITE = 0,
	ROSTERCONTROLFLAG_INPARTY = 1,
	ROSTERCONTROLFLAG_ACCEPT = 2,
	ROSTERCONTROLFLAG_LEAVE = 3,
	ROSTERCONTROLFLAG_CANCEL = 4,
};

enum D2C_RosterButtonTypes
{
	ROSTERBUTTONTYPE_HOSTILE = 1,
	ROSTERBUTTONTYPE_LOOT = 2,
	ROSTERBUTTONTYPE_IGNORE = 3,
	ROSTERBUTTONTYPE_SQUELCH = 4,
};


struct D2RosterButtonStrc
{
	int32_t unk0x00;						//0x00
	int32_t nX;								//0x04
	int32_t nY;								//0x08
	D2CellFileStrc* pCellFile;				//0x0C
	int32_t nFrame;							//0x10
	int32_t nButtonType;					//0x14
	int32_t nPartyFlags;					//0x18
	int32_t unk0x1C;						//0x1C
};

struct D2RosterControlStrc
{
	D2RosterButtonStrc pButtons[5];			//0x00
	char szNameEx[16];						//0xA0
	uint32_t unk0xB0;						//0xB0
	uint32_t unk0xB4;						//0xB4
	uint8_t unk0xB8;						//0xB8
	uint32_t dwClassId;						//0xB9
	uint32_t dwLevel;						//0xBD
	uint32_t dwLevelId;						//0xC1
	uint32_t dwUnitId;						//0xC5
	uint32_t unk0xC9;						//0xC9
	D2RosterControlStrc* pNext;				//0xCD
};

struct D2RosterCorpseStrc
{
	uint32_t dwCorpseId;					//0x00
	D2RosterCorpseStrc* pPrev;				//0x04
};

struct D2RosterInfoStrc
{
	uint32_t dwUnitId;						//0x00
	uint32_t dwPartyFlags;					//0x04
	D2RosterInfoStrc* pNext;				//0x08
};

struct D2RosterPetStrc
{
	int32_t nMonStatsId;					//0x00
	int32_t nPetTypeId;						//0x04
	uint32_t dwUnitId;						//0x08
	uint32_t dwOwnerId;						//0x0C
	uint32_t unk0x10;						//0x10
	uint32_t unk0x14;						//0x14
	uint32_t unk0x18;						//0x18
	int32_t nLifePercent;					//0x1C
	BOOL bDrawIcon;							//0x20
	void* unk0x24;							//0x24
	uint32_t unk0x28;						//0x28
	uint32_t unk0x2C;						//0x2C
	D2RosterPetStrc* pNext;					//0x30
};

struct D2RosterUnitStrc
{
	char szName[16];						//0x00
	uint32_t dwUnitId;						//0x10
	uint32_t dwPartyLife;					//0x14
	uint32_t dwKillCount;					//0x18
	uint32_t dwClassId;						//0x1C
	uint16_t wLevel;						//0x20
	uint16_t wPartyId;						//0x22
	uint32_t dwLevelId;						//0x24
	uint32_t dwPosX;						//0x28
	uint32_t dwPosY;						//0x2C
	uint32_t dwPartyFlags;					//0x30
	D2RosterInfoStrc** pRosterInfo;			//0x34
	D2RosterCorpseStrc* pCorpse;			//0x38
	uint32_t dwSrcPortalGUID;				//0x3C
	uint32_t dwDstPortalGUID;				//0x40
	uint16_t unk0x44;						//0x44
	uint8_t unk0x46[32];					//0x46
	char szNameEx[26];						//0x66 - name with clan tag
	D2RosterUnitStrc* pNext;				//0x80
};

#pragma pack()

