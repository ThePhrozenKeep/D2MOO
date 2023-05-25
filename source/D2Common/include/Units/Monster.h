#pragma once

#include <DataTbls/MonsterTbls.h>

#pragma pack(1)
struct D2AiControlStrc; // From D2Game
struct D2AiParamStrc;	// From D2Game

struct D2InteractInfoStrc
{
	D2UnitStrc* pUnit;						//0x00
	int32_t nInteract;						//0x04
	D2InteractInfoStrc* pNext;				//0x08
};


struct D2MonsterInteractStrc
{
	D2InteractInfoStrc* pInteractInfo;		//0x00
};

struct D2UnkMonsterDataStrc
{
	int32_t unk0x00;						//0x00
	int32_t nLifePercentage;				//0x04
	int32_t unk0x08;						//0x08
	uint8_t nCount;							//0x0C
	uint8_t pad0x0D[3];						//0x0D
};

struct D2MonsterDataStrc
{
	D2MonStatsTxt* pMonstatsTxt;			//0x00
	uint8_t nComponent[16];					//0x04
	uint16_t wNameSeed;							//0x14
	uint8_t nTypeFlag;							//0x16
	uint8_t nLastAnimMode;						//0x17
	uint32_t dwDurielFlag;						//0x18
	uint8_t nMonUmod[10];						//0x1C
	uint16_t wBossHcIdx;						//0x26
	D2AiControlStrc* pAiControl;			//0x28
	union									//0x2C
	{
		D2AiParamStrc* pAiParam;	//Server pMonster
		struct Unicode* wszMonName;	//Client pMonster
	};
	D2MonsterInteractStrc* pMonInteract;	//0x30
	uint32_t unk0x34;							//0x34
	uint32_t unk0x38;							//0x38
	D2UnkMonsterDataStrc unk0x3C;				//0x3C
	uint32_t unk0x4C;							//0x4C
	uint32_t pVision;							//0x50
	uint32_t dwAiState;						//0x54
	uint32_t dwTxtLevelNo;						//0x58
	uint16_t wSummonerFlag;						//0x5C
	uint8_t pad0x5E[2];						//0x5E
};

#pragma pack()
