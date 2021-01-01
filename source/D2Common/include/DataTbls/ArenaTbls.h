#pragma once 

#include <D2BasicTypes.h>

struct D2ArenaTxt
{
	uint32_t dwSuicide;					//0x00
	uint32_t dwPlayerKill;				//0x04
	uint32_t dwPlayerKillPercent;		//0x08
	uint32_t dwMonsterKill;				//0x0C
	uint32_t dwPlayerDeath;				//0x10
	uint32_t dwPlayerDeathPercent;		//0x14
	uint32_t dwMonsterDeath;			//0x18
};

struct D2CharItemStrc
{
	uint32_t dwItemCode;				//0x00
	uint8_t nBodyLoc;					//0x04
	int8_t nItemCount;					//0x05
	uint16_t pad0x06;					//0x06
};

struct D2CharTemplateTxt
{
	char szName[30];					//0x00
	uint8_t nClass;						//0x1E
	uint8_t nLevel;						//0x1F
	uint8_t nAct;						//0x20
	uint8_t nStr;						//0x21
	uint8_t nDex;						//0x22
	uint8_t nInt;						//0x23
	uint8_t nVit;						//0x24
	uint8_t nMana;						//0x25
	uint8_t nHitpoints;					//0x26
	uint8_t nManaRegenBonus;			//0x27
	uint8_t nVelocity;					//0x28
	uint8_t nAttackRate;				//0x29
	uint8_t nOtherRate;					//0x2A
	uint8_t pad0x2B;					//0x2B
	uint32_t dwRightSkill;				//0x2C
	uint32_t dwSkill[9];				//0x30
	uint32_t dwSkillLevel[9];			//0x54
	D2CharItemStrc pItems[15];			//0x78
};


//D2Common.0x6FD47840
void __fastcall DATATBLS_LoadArenaTxt(void* pMemPool);
//D2Common.0x6FD47970 (#10596)
D2ArenaTxt* __fastcall DATATBLS_GetArenaTxtRecord(int nArenaType);
//D2Common.0x6FD479B0
void __fastcall DATATBLS_UnloadArenaTxt();
//D2Common.0x6FD479D0
void __fastcall DATATBLS_LoadCharTemplateTxt(void* pMemPool);
//D2Common.0x6FD48770
void __fastcall DATATBLS_UnloadCharTemplateTxt();
//D2Common.0x6FD48790 (#10664)
int __fastcall DATATBLS_GetCharTemplateTxtRecordCount();
//D2Common.0x6FD487A0 (#10665)
D2CharTemplateTxt* __fastcall DATATBLS_GetCharTemplateTxtRecord(int nTemplate, int nLevel);
//D2Common.0x6FD48810 (#10666)
int __fastcall DATATBLS_GetClassFromCharTemplateTxtRecord(int nTemplate, int nLevel);


