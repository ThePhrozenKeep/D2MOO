#pragma once

#pragma pack(1)

struct D2MonsterInteractStrc
{
	D2InteractInfoStrc* pInteractInfo;		//0x00
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
		wchar_t* wszMonName;		//Client pMonster
	};
	D2MonsterInteractStrc* pMonInteract;	//0x30
	uint32_t unk0x34[3];						//0x34
	uint32_t dwNecropetFlag;					//0x40
	uint32_t unk0x44[3];						//0x44
	uint32_t pVision;							//0x50
	uint32_t dwAiState;						//0x54
	uint32_t dwTxtLevelNo;						//0x58
	uint16_t wSummonerFlag;						//0x5C
	uint8_t pad0x5E[2];						//0x5E
};

#pragma pack()
