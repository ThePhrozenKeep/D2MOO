#pragma once 

#include <D2BasicTypes.h>

#pragma pack(1)

enum D2C_SkillsTxtFlagIndices
{
	SKILLSFLAGINDEX_DECQUANT = 0,
	SKILLSFLAGINDEX_LOB,
	SKILLSFLAGINDEX_PROGRESSIVE,
	SKILLSFLAGINDEX_FINISHING,
	SKILLSFLAGINDEX_PASSIVE,
	SKILLSFLAGINDEX_AURA,
	SKILLSFLAGINDEX_PERIODIC,
	SKILLSFLAGINDEX_PRGSTACK,
	SKILLSFLAGINDEX_INTOWN,
	SKILLSFLAGINDEX_KICK,
	SKILLSFLAGINDEX_INGAME,
	SKILLSFLAGINDEX_REPEAT,
	SKILLSFLAGINDEX_STSUCCESSONLY,
	SKILLSFLAGINDEX_STSOUNDDELAY,
	SKILLSFLAGINDEX_WEAPONSND,
	SKILLSFLAGINDEX_IMMEDIATE,
	SKILLSFLAGINDEX_NOAMMO,
	SKILLSFLAGINDEX_ENHANCEABLE,
	SKILLSFLAGINDEX_DURABILITY,
	SKILLSFLAGINDEX_USEATTACKRATE,
	SKILLSFLAGINDEX_TARGETABLEONLY,
	SKILLSFLAGINDEX_SEARCHENEMYXY,
	SKILLSFLAGINDEX_SEARCHENEMYNEAR,
	SKILLSFLAGINDEX_SEARCHOPENXY,
	SKILLSFLAGINDEX_TARGETCORPSE,
	SKILLSFLAGINDEX_TARGETPET,
	SKILLSFLAGINDEX_TARGETALLY,
	SKILLSFLAGINDEX_TARGETITEM,
	SKILLSFLAGINDEX_ATTACKNOMANA,
	SKILLSFLAGINDEX_ITEMTGTDO,
	SKILLSFLAGINDEX_LEFTSKILL,
	SKILLSFLAGINDEX_INTERRUPT,

	//Bits above 31 need to restart at 0
	SKILLSFLAGINDEX2_TGTPLACECHECK = 0,
	SKILLSFLAGINDEX2_ITEMCHECKSTART,
	SKILLSFLAGINDEX2_ITEMCLTCHECKSTART,
	SKILLSFLAGINDEX2_GENERAL,
	SKILLSFLAGINDEX2_SCROLL,
	SKILLSFLAGINDEX2_USEMANAONDO,
	SKILLSFLAGINDEX2_WARP,
};

enum D2C_SkillsTxtFlags
{
	SKILLSFLAG_DECQUANT = (1 << SKILLSFLAGINDEX_DECQUANT),
	SKILLSFLAG_LOB = (1 << SKILLSFLAGINDEX_LOB),
	SKILLSFLAG_PROGRESSIVE = (1 << SKILLSFLAGINDEX_PROGRESSIVE),
	SKILLSFLAG_FINISHING = (1 << SKILLSFLAGINDEX_FINISHING),
	SKILLSFLAG_PASSIVE = (1 << SKILLSFLAGINDEX_PASSIVE),
	SKILLSFLAG_AURA = (1 << SKILLSFLAGINDEX_AURA),
	SKILLSFLAG_PERIODIC = (1 << SKILLSFLAGINDEX_PERIODIC),
	SKILLSFLAG_PRGSTACK = (1 << SKILLSFLAGINDEX_PRGSTACK),
	SKILLSFLAG_INTOWN = (1 << SKILLSFLAGINDEX_INTOWN),
	SKILLSFLAG_KICK = (1 << SKILLSFLAGINDEX_KICK),
	SKILLSFLAG_INGAME = (1 << SKILLSFLAGINDEX_INGAME),
	SKILLSFLAG_REPEAT = (1 << SKILLSFLAGINDEX_REPEAT),
	SKILLSFLAG_STSUCCESSONLY = (1 << SKILLSFLAGINDEX_STSUCCESSONLY),
	SKILLSFLAG_STSOUNDDELAY = (1 << SKILLSFLAGINDEX_STSOUNDDELAY),
	SKILLSFLAG_WEAPONSND = (1 << SKILLSFLAGINDEX_WEAPONSND),
	SKILLSFLAG_IMMEDIATE = (1 << SKILLSFLAGINDEX_IMMEDIATE),
	SKILLSFLAG_NOAMMO = (1 << SKILLSFLAGINDEX_NOAMMO),
	SKILLSFLAG_ENHANCEABLE = (1 << SKILLSFLAGINDEX_ENHANCEABLE),
	SKILLSFLAG_DURABILITY = (1 << SKILLSFLAGINDEX_DURABILITY),
	SKILLSFLAG_USEATTACKRATE = (1 << SKILLSFLAGINDEX_USEATTACKRATE),
	SKILLSFLAG_TARGETABLEONLY = (1 << SKILLSFLAGINDEX_TARGETABLEONLY),
	SKILLSFLAG_SEARCHENEMYXY = (1 << SKILLSFLAGINDEX_SEARCHENEMYXY),
	SKILLSFLAG_SEARCHENEMYNEAR = (1 << SKILLSFLAGINDEX_SEARCHENEMYNEAR),
	SKILLSFLAG_SEARCHOPENXY = (1 << SKILLSFLAGINDEX_SEARCHOPENXY),
	SKILLSFLAG_TARGETCORPSE = (1 << SKILLSFLAGINDEX_TARGETCORPSE),
	SKILLSFLAG_TARGETPET = (1 << SKILLSFLAGINDEX_TARGETPET),
	SKILLSFLAG_TARGETALLY = (1 << SKILLSFLAGINDEX_TARGETALLY),
	SKILLSFLAG_TARGETITEM = (1 << SKILLSFLAGINDEX_TARGETITEM),
	SKILLSFLAG_ATTACKNOMANA = (1 << SKILLSFLAGINDEX_ATTACKNOMANA),
	SKILLSFLAG_ITEMTGTDO = (1 << SKILLSFLAGINDEX_ITEMTGTDO),
	SKILLSFLAG_LEFTSKILL = (1 << SKILLSFLAGINDEX_LEFTSKILL),
	SKILLSFLAG_INTERRUPT = (1 << SKILLSFLAGINDEX_INTERRUPT),

	//Bits above 31 need to restart at 0
	SKILLSFLAG2_TGTPLACECHECK = (1 << SKILLSFLAGINDEX2_TGTPLACECHECK),
	SKILLSFLAG2_ITEMCHECKSTART = (1 << SKILLSFLAGINDEX2_ITEMCHECKSTART),
	SKILLSFLAG2_ITEMCLTCHECKSTART = (1 << SKILLSFLAGINDEX2_ITEMCLTCHECKSTART),
	SKILLSFLAG2_GENERAL = (1 << SKILLSFLAGINDEX2_GENERAL),
	SKILLSFLAG2_SCROLL = (1 << SKILLSFLAGINDEX2_SCROLL),
	SKILLSFLAG2_USEMANAONDO = (1 << SKILLSFLAGINDEX2_USEMANAONDO),
	SKILLSFLAG2_WARP = (1 << SKILLSFLAGINDEX2_WARP),
};

// Targeting behaviour when skill is cast by object
enum D2C_SkillsTxtItemTarget {
	SKILLSITEMTARGET_ATTACKER = 0, // default
	SKILLSITEMTARGET_CASTER = 1,
	SKILLSITEMTARGET_RANDOM = 2, // Random walkable location in a radius of size 20
	SKILLSITEMTARGET_RANDOM_CORPSE = 3,
	SKILLSITEMTARGET_LAST_ATTACKER = 4, // Attacker or last known attacker
};

struct D2SkillCalcTxt
{
	uint32_t dwCode;						//0x00
};

struct D2SkillDescTxt
{
	uint16_t wSkillDesc;					//0x00
	uint8_t nSkillPage;					//0x02
	uint8_t nSkillRow;						//0x03
	uint8_t nSkillColumn;					//0x04
	uint8_t nListRow;						//0x05
	uint8_t nListPool;						//0x06
	uint8_t nIconCel;						//0x07
	uint16_t wStrName;						//0x08
	uint16_t wStrShort;						//0x0A
	uint16_t wStrLong;						//0x0C
	uint16_t wStrAlt;						//0x0E
	uint16_t wStrMana;						//0x10
	uint16_t wDescDam;						//0x12
	uint16_t wDescAtt;						//0x14
	uint16_t pad0x16;						//0x16
	uint32_t dwDamCalc[2];					//0x18
	uint8_t nPrgDamElem[3];				//0x20
	uint8_t pad0x23;						//0x23
	uint32_t dwProgDmgMin[3];				//0x24
	uint32_t dwProgDmgMax[3];				//0x30
	uint16_t wDescMissile[3];				//0x3C
	uint8_t nDescLine[17];					//0x42
	uint8_t pad0x53;						//0x53
	uint16_t wDescTextA[17];				//0x54
	uint16_t wDescTextB[17];				//0x76
	uint32_t dwDescCalcA[17];				//0x98
	uint32_t dwDescCalcB[17];				//0xDC
};

struct D2SkillsTxt
{
	int16_t nSkillId; 						//0x00
	uint16_t unk0x02;						//0x02
	uint32_t dwFlags[2];					//0x04
	int8_t nCharClass;						//0x0C
	uint8_t unk0x0D[3];						//0x0D
	uint8_t nAnim;							//0x10
	uint8_t nMonAnim;						//0x11
	uint8_t nSeqTrans;						//0x12
	uint8_t nSeqNum;						//0x13
	uint8_t nRange;							//0x14
	uint8_t nSelectProc;					//0x15
	uint8_t nSeqInput;						//0x16
	uint8_t pad0x17;						//0x17
	int16_t nITypeA[3];						//0x18
	int16_t nITypeB[3];						//0x1E
	int16_t nETypeA[2];						//0x24
	int16_t nETypeB[2];						//0x28
	int16_t wSrvStartFunc;					//0x2C
	uint16_t wSrvDoFunc;					//0x2E
	uint16_t wSrvPrgFunc[3];				//0x30
	uint16_t pad0x36;						//0x36
	uint32_t dwPrgCalc[3];					//0x38
	uint8_t nPrgDamage;						//0x44
	uint8_t pad0x45;						//0x45
	uint16_t wSrvMissile;					//0x46
	uint16_t wSrvMissileA;					//0x48
	uint16_t wSrvMissileB;					//0x4A
	uint16_t wSrvMissileC;					//0x4C
	int16_t wSrvOverlay;					//0x4E
	uint32_t dwAuraFilter;					//0x50
	int16_t wAuraStat[6];					//0x54
	uint32_t dwAuraLenCalc;					//0x60
	uint32_t dwAuraRangeCalc;				//0x64
	int32_t dwAuraStatCalc[6];				//0x68
	int16_t nAuraState;						//0x80
	int16_t wAuraTargetState;				//0x82
	int16_t wAuraEvent[3];					//0x84
	int16_t wAuraEventFunc[3];				//0x8A
	uint16_t wAuraTgtEvent;					//0x90
	uint16_t wAuraTgtEventFunc;				//0x92
	int16_t nPassiveState;					//0x94
	int16_t nPassiveIType;					//0x96
	int16_t nPassiveStat[5];				//0x98
	uint16_t pad0xA2;						//0xA2
	uint32_t dwPassiveCalc[5];				//0xA4
	uint16_t wPassiveEvent;					//0xB8
	uint16_t wPassiveEventFunc;				//0xBA
	uint16_t wSummon;						//0xBC
	int8_t nPetType;						//0xBE
	int8_t nSumMode;						//0xBF
	uint32_t dwPetMax;						//0xC0
	uint16_t wSumSkill[5];					//0xC4
	uint16_t pad0xCE;						//0xCE
	uint32_t dwSumSkCalc[5];				//0xD0
	int16_t wSumUMod;						//0xE4
	int16_t wSumOverlay;					//0xE6
	uint16_t wCltMissile;					//0xE8
	uint16_t wCltMissileA;					//0xEA
	uint16_t wCltMissileB;					//0xEC
	uint16_t wCltMissileC;					//0xEE
	uint16_t wCltMissileD;					//0xF0
	uint16_t wCltStFunc;					//0xF2
	uint16_t wCltDoFunc;					//0xF4
	uint16_t wCltPrgFunc[3];				//0xF6
	uint16_t wStSound;						//0xFC
	uint16_t nStSoundClass;					//0x0FE	
	uint16_t wDoSound;						//0x100
	uint16_t wDoSoundA;						//0x102
	uint16_t wDoSoundB;						//0x104
	uint16_t wCastOverlay;					//0x106
	uint16_t wTgtOverlay;					//0x108
	uint16_t wTgtSound;						//0x10A
	uint16_t wPrgOverlay;					//0x10C
	uint16_t wPrgSound;						//0x10E
	uint16_t wCltOverlayA;					//0x110
	uint16_t wCltOverlayB;					//0x112
	int32_t dwCltCalc[3];					//0x114
	uint8_t nItemTarget;					//0x120 D2C_SkillsTxtItemTarget
	uint8_t pad0x121;						//0x121
	uint16_t wItemCastSound;				//0x122
	uint16_t wItemCastOverlay;				//0x124
	uint16_t pad0x126;						//0x126
	uint32_t dwPerDelay;					//0x128
	uint16_t wMaxLvl;						//0x12C
	uint16_t wResultFlags;					//0x12E
	uint32_t dwHitFlags;					//0x130
	uint32_t dwHitClass;					//0x134
	uint32_t dwCalc[4];						//0x138
	int32_t dwParam[8];						//0x148
	uint8_t nWeapSel;						//0x168
	uint8_t pad0x169;						//0x169
	uint16_t wItemEffect;					//0x16A
	uint16_t wItemCltEffect;				//0x16C
	uint16_t pad0x16E;						//0x16E
	uint32_t dwSkPoints;					//0x170
	uint16_t wReqLevel;						//0x174
	uint16_t wReqStr;						//0x176
	uint16_t wReqDex;						//0x178
	uint16_t wReqInt;						//0x17A
	uint16_t wReqVit;						//0x17C
	int16_t nReqSkill[3];					//0x17E
	int16_t wStartMana;						//0x184
	uint16_t wMinMana;						//0x186
	uint8_t nManaShift;						//0x188
	uint8_t pad0x189;						//0x189
	int16_t wMana;							//0x18A
	int16_t wLevelMana;						//0x18C
	uint8_t nAttackRank;					//0x18E
	uint8_t nLineOfSight;					//0x18F
	uint32_t dwDelay;						//0x190
	uint16_t wSkillDesc;					//0x194
	uint16_t pad0x196;						//0x196
	uint32_t dwToHit;						//0x198
	uint32_t dwLevToHit;					//0x19C
	uint32_t dwToHitCalc;					//0x1A0
	uint8_t nToHitShift;					//0x1A4
	uint8_t nSrcDam;						//0x1A5
	uint16_t pad0x1A6;						//0x1A6
	uint32_t dwMinDam;						//0x1A8
	uint32_t dwMaxDam;						//0x1AC
	uint32_t dwMinLvlDam[5];				//0x1B0
	uint32_t dwMaxLvlDam[5];				//0x1C4
	uint32_t dwDmgSymPerCalc;				//0x1D8
	uint8_t nEType;							//0x1DC
	uint8_t pad0x1DD[3];					//0x1DD
	uint32_t dwEMin;						//0x1E0
	uint32_t dwEMax;						//0x1E4
	uint32_t dwEMinLev[5];					//0x1E8
	uint32_t dwEMaxLev[5];					//0x1FC
	uint32_t dwEDmgSymPerCalc;				//0x210
	uint32_t dwELen;						//0x214
	uint32_t dwELevLen[3];					//0x218
	uint32_t dwELenSymPerCalc;				//0x224
	uint8_t nRestrict;						//0x228
	uint8_t pad0x229;						//0x229
	int16_t nState[3];						//0x22A
	uint8_t nAiType;						//0x230
	uint8_t pad0x231;						//0x231
	int16_t wAiBonus;						//0x232
	int32_t nCostMult;						//0x234
	int32_t nCostAdd;						//0x238
};

#pragma pack()

//D2Common.0x6FD498D0
int __fastcall DATATBLS_MapSkillsTxtKeywordToNumber(char* szKey);
//D2Common.0x6FD49980
int __fastcall sub_6FD49980(int nValue);
//D2Common.0x6FD49990
int __fastcall sub_6FD49990(char* szText, int* a2, int a3, int nKeywordNumber);
//D2Common.0x6FD49D20
void __fastcall DATATBLS_SkillCalcLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD49E40
void __fastcall DATATBLS_SkillDescCalcLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD49F60
void __fastcall DATATBLS_LoadSkills_SkillDescTxt(void* pMemPool);
//D2Common.0x6FD4E350
void __fastcall DATATBLS_UnloadSkills_SkillDescTxt();
//Inlined at various places
D2SkillsTxt* __fastcall DATATBLS_GetSkillsTxtRecord(int nSkillId);
//Inlined at various places
D2SkillDescTxt* __fastcall DATATBLS_GetSkillDescTxtRecord(int nSkillDesc);

