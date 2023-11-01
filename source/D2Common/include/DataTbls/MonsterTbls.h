#pragma once 

#include <D2BasicTypes.h>
#include <D2Monsters.h>

#include "ItemsTbls.h"

#pragma pack(1)

//TODO: Find a better name
struct D2UnkMonCompStrc
{
	char nComposit[12];
	//int unk0x04;
	//int unk0x08;
};


struct D2TCExInfoStrc
{
	int32_t nClassic;						//0x00
	int32_t nProb;							//0x04
	int16_t nItemId;						//0x08
	int16_t nTxtRow;						//0x0A
	uint8_t nFlags;							//0x0C
	int8_t unk0x0D;							//0x0D
	int16_t nMagic;							//0x0E
	int16_t nRare;							//0x10
	int16_t nSet;							//0x12
	int16_t nUnique;						//0x14
	int16_t nSuperior;						//0x16
	int16_t nNormal;						//0x18
	int16_t unk0x1A;						//0x1A
};

struct D2TCExShortStrc
{
	int16_t nGroup;							//0x00
	int16_t nLevel;							//0x02
	int32_t nTypes;							//0x04
	int32_t nClassic;						//0x08
	int32_t nProb;							//0x0C
	int32_t nPicks;							//0x10
	int32_t nNoDrop;						//0x14
	int16_t unk0x18;						//0x18
	int16_t nMagic;							//0x1A
	int16_t nRare;							//0x1C
	int16_t nSet;							//0x1E
	int16_t nUnique;						//0x20
	int16_t nSuperior;						//0x22
	int16_t nNormal;						//0x24
	int16_t unk0x26;						//0x26
	D2TCExInfoStrc* pInfo;					//0x28
};

struct D2MonEquipTxt
{
	int16_t nMonster;						//0x00
	uint16_t wLevel;						//0x02
	uint8_t nOnInit;						//0x04
	uint8_t pad0x05[3];						//0x05
	uint32_t dwItem[3];						//0x08
	uint8_t nLoc[3];						//0x14
	uint8_t nMod[3];						//0x17
	uint16_t pad0x1A;						//0x1A
};

struct D2MonItemPercentTxt
{
	uint8_t nHeartPercent;					//0x00
	uint8_t nBodyPartPercent;				//0x01
	uint8_t nTreasureClassPercent;			//0x02
	uint8_t nComponentPercent;				//0x03
};

struct D2MonItemPercentDataTbl						//sgptDataTable + 0x105C
{
	int nMonItemPercentTxtRecordCount;				//0x00
	D2MonItemPercentTxt* pMonItemPercentTxt;		//0x04
};

struct D2MonLvlTxt
{
	uint32_t dwAC[3];						//0x00
	uint32_t dwLAC[3];						//0x0C
	uint32_t dwTH[3];						//0x18
	uint32_t dwLTH[3];						//0x24
	uint32_t dwHP[3];						//0x30
	uint32_t dwLHP[3];						//0x3C
	uint32_t dwDM[3];						//0x48
	uint32_t dwLDM[3];						//0x54
	uint32_t dwXP[3];						//0x60
	uint32_t dwLXP[3];						//0x6C
};

struct D2MonPresetTxt
{
	uint8_t nAct;							//0x00
	uint8_t nType;							//0x01
	uint16_t wPlace;						//0x02
};

struct D2MonPropTxt
{
	uint32_t dwId;							//0x00
	D2PropertyStrc props[3][6];				//0x04
	uint8_t nChance[3][6];					//0x124
	uint16_t pad0x136;						//0x136
};


struct D2MonSoundsTxt
{
	uint32_t dwId;							//0x00
	uint32_t dwAttack1;						//0x04
	uint32_t dwAtt1Del;						//0x08
	uint32_t dwAtt1Prb;						//0x0C
	uint32_t dwAttack2;						//0x10
	uint32_t dwAtt2Del;						//0x14
	uint32_t dwAtt2Prb;						//0x18
	uint32_t dwWeapon1;						//0x1C
	uint32_t dwWea1Del;						//0x20
	uint32_t dwWea1Vol;						//0x24
	uint32_t dwWeapon2;						//0x28
	uint32_t dwWea2Del;						//0x2C
	uint32_t dwWea2Vol;						//0x30
	uint32_t dwHitSound;					//0x34
	uint32_t dwHitDelay;					//0x38
	uint32_t dwDeathSound;					//0x3C
	uint32_t dwDeaDelay;					//0x40
	uint32_t dwSkill[4];					//0x44
	uint32_t dwFootstep;					//0x54
	uint32_t dwFootstepLayer;				//0x58
	uint32_t dwFsCnt;						//0x5C
	uint32_t dwFsOff;						//0x60
	uint32_t dwFsPrb;						//0x64
	uint32_t dwNeutral;						//0x68
	uint32_t dwNeuTime;						//0x6C
	uint32_t dwInit;						//0x70
	uint32_t dwTaunt;						//0x74
	uint32_t dwFlee;						//0x78
	uint8_t nCvtMo1;						//0x7C
	uint8_t nCvtTgt1;						//0x7D
	uint16_t pad0x7E;						//0x7E
	uint32_t dwCvtSk1;						//0x80
	uint8_t nCvtMo2;						//0x84
	uint8_t nCvtTgt2;						//0x85
	uint16_t pad0x86;						//0x86
	uint32_t dwCvtSk2;						//0x88
	uint8_t nCvtMo3;						//0x8C
	uint8_t nCvtTgt3;						//0x8D
	uint16_t pad0x8E;						//0x8E
	uint32_t dwCvtSk3;						//0x90
};

enum D2C_MonStatsTxtFlagIndices
{
	MONSTATSFLAGINDEX_ISSPAWN = 0,
	MONSTATSFLAGINDEX_ISMELEE,
	MONSTATSFLAGINDEX_NORATIO,
	MONSTATSFLAGINDEX_OPENDOORS,
	MONSTATSFLAGINDEX_SETBOSS,
	MONSTATSFLAGINDEX_BOSSXFER,
	MONSTATSFLAGINDEX_BOSS,
	MONSTATSFLAGINDEX_PRIMEEVIL,
	MONSTATSFLAGINDEX_NPC,
	MONSTATSFLAGINDEX_INTERACT, // Player may interact with the monster instead of attacking it.
	MONSTATSFLAGINDEX_INTOWN,
	MONSTATSFLAGINDEX_LUNDEAD,
	MONSTATSFLAGINDEX_HUNDEAD,
	MONSTATSFLAGINDEX_DEMON,
	MONSTATSFLAGINDEX_FLYING,
	MONSTATSFLAGINDEX_KILLABLE,
	MONSTATSFLAGINDEX_SWITCHAI,
	MONSTATSFLAGINDEX_NOMULTISHOT,
	MONSTATSFLAGINDEX_NEVERCOUNT,
	MONSTATSFLAGINDEX_PETIGNORE,
	MONSTATSFLAGINDEX_DEATHDMG,
	MONSTATSFLAGINDEX_GENERICSPAWN,
	MONSTATSFLAGINDEX_ZOO,
	MONSTATSFLAGINDEX_PLACESPAWN,
	MONSTATSFLAGINDEX_INVENTORY,
	MONSTATSFLAGINDEX_ENABLED,
	MONSTATSFLAGINDEX_NOSHLDBLOCK,
	MONSTATSFLAGINDEX_NOAURA,
	MONSTATSFLAGINDEX_RANGEDTYPE,
};

enum D2C_MonStatsTxtFlags
{
	MONSTATSFLAG_ISSPAWN = (1 << MONSTATSFLAGINDEX_ISSPAWN),
	MONSTATSFLAG_ISMELEE = (1 << MONSTATSFLAGINDEX_ISMELEE),
	MONSTATSFLAG_NORATIO = (1 << MONSTATSFLAGINDEX_NORATIO),
	MONSTATSFLAG_OPENDOORS = (1 << MONSTATSFLAGINDEX_OPENDOORS),
	MONSTATSFLAG_SETBOSS = (1 << MONSTATSFLAGINDEX_SETBOSS),
	MONSTATSFLAG_BOSSXFER = (1 << MONSTATSFLAGINDEX_BOSSXFER),
	MONSTATSFLAG_BOSS = (1 << MONSTATSFLAGINDEX_BOSS),
	MONSTATSFLAG_PRIMEEVIL = (1 << MONSTATSFLAGINDEX_PRIMEEVIL),
	MONSTATSFLAG_NPC = (1 << MONSTATSFLAGINDEX_NPC),
	MONSTATSFLAG_INTERACT = (1 << MONSTATSFLAGINDEX_INTERACT),
	MONSTATSFLAG_INTOWN = (1 << MONSTATSFLAGINDEX_INTOWN),
	MONSTATSFLAG_LUNDEAD = (1 << MONSTATSFLAGINDEX_LUNDEAD),
	MONSTATSFLAG_HUNDEAD = (1 << MONSTATSFLAGINDEX_HUNDEAD),
	MONSTATSFLAG_DEMON = (1 << MONSTATSFLAGINDEX_DEMON),
	MONSTATSFLAG_FLYING = (1 << MONSTATSFLAGINDEX_FLYING),
	MONSTATSFLAG_KILLABLE = (1 << MONSTATSFLAGINDEX_KILLABLE),
	MONSTATSFLAG_SWITCHAI = (1 << MONSTATSFLAGINDEX_SWITCHAI),
	MONSTATSFLAG_NOMULTISHOT = (1 << MONSTATSFLAGINDEX_NOMULTISHOT),
	MONSTATSFLAG_NEVERCOUNT = (1 << MONSTATSFLAGINDEX_NEVERCOUNT),
	MONSTATSFLAG_PETIGNORE = (1 << MONSTATSFLAGINDEX_PETIGNORE),
	MONSTATSFLAG_DEATHDMG = (1 << MONSTATSFLAGINDEX_DEATHDMG),
	MONSTATSFLAG_GENERICSPAWN = (1 << MONSTATSFLAGINDEX_GENERICSPAWN),
	MONSTATSFLAG_ZOO = (1 << MONSTATSFLAGINDEX_ZOO),
	MONSTATSFLAG_PLACESPAWN = (1 << MONSTATSFLAGINDEX_PLACESPAWN),
	MONSTATSFLAG_INVENTORY = (1 << MONSTATSFLAGINDEX_INVENTORY),
	MONSTATSFLAG_ENABLED = (1 << MONSTATSFLAGINDEX_ENABLED),
	MONSTATSFLAG_NOSHLDBLOCK = (1 << MONSTATSFLAGINDEX_NOSHLDBLOCK),
	MONSTATSFLAG_NOAURA = (1 << MONSTATSFLAGINDEX_NOAURA),
	MONSTATSFLAG_RANGEDTYPE = (1 << MONSTATSFLAGINDEX_RANGEDTYPE),
};

enum D2C_MonStats2TxtFlagIndices
{
	MONSTATS2FLAGINDEX_NOGFXHITTEST = 0,
	MONSTATS2FLAGINDEX_NOMAP,
	MONSTATS2FLAGINDEX_NOOVLY,
	MONSTATS2FLAGINDEX_ISSEL,
	MONSTATS2FLAGINDEX_ALSEL,
	MONSTATS2FLAGINDEX_NOSEL,
	MONSTATS2FLAGINDEX_SHIFTSEL,
	MONSTATS2FLAGINDEX_CORPSESEL,
	MONSTATS2FLAGINDEX_REVIVE,// Monster may be revived.
	MONSTATS2FLAGINDEX_ISATT, // Monster may be attacked.
	MONSTATS2FLAGINDEX_SMALL,
	MONSTATS2FLAGINDEX_LARGE,
	MONSTATS2FLAGINDEX_SOFT,
	MONSTATS2FLAGINDEX_CRITTER,
	MONSTATS2FLAGINDEX_SHADOW,
	MONSTATS2FLAGINDEX_NOUNIQUESHIFT,
	MONSTATS2FLAGINDEX_COMPOSITEDEATH,
	MONSTATS2FLAGINDEX_INERT,
	MONSTATS2FLAGINDEX_OBJCOL,
	MONSTATS2FLAGINDEX_DEADCOL,
	MONSTATS2FLAGINDEX_UNFLATDEAD,
};

enum D2C_MonStats2TxtFlags
{
	MONSTATS2FLAG_NOGFXHITTEST = (1 << MONSTATS2FLAGINDEX_NOGFXHITTEST),
	MONSTATS2FLAG_NOMAP = (1 << MONSTATS2FLAGINDEX_NOMAP),
	MONSTATS2FLAG_NOOVLY = (1 << MONSTATS2FLAGINDEX_NOOVLY),
	MONSTATS2FLAG_ISSEL = (1 << MONSTATS2FLAGINDEX_ISSEL),
	MONSTATS2FLAG_ALSEL = (1 << MONSTATS2FLAGINDEX_ALSEL),
	MONSTATS2FLAG_NOSEL = (1 << MONSTATS2FLAGINDEX_NOSEL),
	MONSTATS2FLAG_SHIFTSEL = (1 << MONSTATS2FLAGINDEX_SHIFTSEL),
	MONSTATS2FLAG_CORPSESEL = (1 << MONSTATS2FLAGINDEX_CORPSESEL),
	MONSTATS2FLAG_REVIVE = (1 << MONSTATS2FLAGINDEX_REVIVE),
	MONSTATS2FLAG_ISATT = (1 << MONSTATS2FLAGINDEX_ISATT),
	MONSTATS2FLAG_SMALL = (1 << MONSTATS2FLAGINDEX_SMALL),
	MONSTATS2FLAG_LARGE = (1 << MONSTATS2FLAGINDEX_LARGE),
	MONSTATS2FLAG_SOFT = (1 << MONSTATS2FLAGINDEX_SOFT),
	MONSTATS2FLAG_CRITTER = (1 << MONSTATS2FLAGINDEX_CRITTER),
	MONSTATS2FLAG_SHADOW = (1 << MONSTATS2FLAGINDEX_SHADOW),
	MONSTATS2FLAG_NOUNIQUESHIFT = (1 << MONSTATS2FLAGINDEX_NOUNIQUESHIFT),
	MONSTATS2FLAG_COMPOSITEDEATH = (1 << MONSTATS2FLAGINDEX_COMPOSITEDEATH),
	MONSTATS2FLAG_INERT = (1 << MONSTATS2FLAGINDEX_INERT),
	MONSTATS2FLAG_OBJCOL = (1 << MONSTATS2FLAGINDEX_OBJCOL),
	MONSTATS2FLAG_DEADCOL = (1 << MONSTATS2FLAGINDEX_DEADCOL),
	MONSTATS2FLAG_UNFLATDEAD = (1 << MONSTATS2FLAGINDEX_UNFLATDEAD),
};

enum D2C_MonStats2TxtComponentFlagIndices
{
	MONSTATS2COMPONENTFLAGINDEX_HD = 0,
	MONSTATS2COMPONENTFLAGINDEX_TR,
	MONSTATS2COMPONENTFLAGINDEX_LG,
	MONSTATS2COMPONENTFLAGINDEX_RA,
	MONSTATS2COMPONENTFLAGINDEX_LA,
	MONSTATS2COMPONENTFLAGINDEX_RH,
	MONSTATS2COMPONENTFLAGINDEX_LH,
	MONSTATS2COMPONENTFLAGINDEX_SH,
	MONSTATS2COMPONENTFLAGINDEX_S1,
	MONSTATS2COMPONENTFLAGINDEX_S2,
	MONSTATS2COMPONENTFLAGINDEX_S3,
	MONSTATS2COMPONENTFLAGINDEX_S4,
	MONSTATS2COMPONENTFLAGINDEX_S5,
	MONSTATS2COMPONENTFLAGINDEX_S6,
	MONSTATS2COMPONENTFLAGINDEX_S7,
	MONSTATS2COMPONENTFLAGINDEX_S8,
};

enum D2C_MonStats2TxtComponentFlags
{
	MONSTATS2COMPONENTFLAG_HD = (1 << MONSTATS2COMPONENTFLAGINDEX_HD),
	MONSTATS2COMPONENTFLAG_TR = (1 << MONSTATS2COMPONENTFLAGINDEX_TR),
	MONSTATS2COMPONENTFLAG_LG = (1 << MONSTATS2COMPONENTFLAGINDEX_LG),
	MONSTATS2COMPONENTFLAG_RA = (1 << MONSTATS2COMPONENTFLAGINDEX_RA),
	MONSTATS2COMPONENTFLAG_LA = (1 << MONSTATS2COMPONENTFLAGINDEX_LA),
	MONSTATS2COMPONENTFLAG_RH = (1 << MONSTATS2COMPONENTFLAGINDEX_RH),
	MONSTATS2COMPONENTFLAG_LH = (1 << MONSTATS2COMPONENTFLAGINDEX_LH),
	MONSTATS2COMPONENTFLAG_SH = (1 << MONSTATS2COMPONENTFLAGINDEX_SH),
	MONSTATS2COMPONENTFLAG_S1 = (1 << MONSTATS2COMPONENTFLAGINDEX_S1),
	MONSTATS2COMPONENTFLAG_S2 = (1 << MONSTATS2COMPONENTFLAGINDEX_S2),
	MONSTATS2COMPONENTFLAG_S3 = (1 << MONSTATS2COMPONENTFLAGINDEX_S3),
	MONSTATS2COMPONENTFLAG_S4 = (1 << MONSTATS2COMPONENTFLAGINDEX_S4),
	MONSTATS2COMPONENTFLAG_S5 = (1 << MONSTATS2COMPONENTFLAGINDEX_S5),
	MONSTATS2COMPONENTFLAG_S6 = (1 << MONSTATS2COMPONENTFLAGINDEX_S6),
	MONSTATS2COMPONENTFLAG_S7 = (1 << MONSTATS2COMPONENTFLAGINDEX_S7),
	MONSTATS2COMPONENTFLAG_S8 = (1 << MONSTATS2COMPONENTFLAGINDEX_S8),
};

enum D2C_MonStats2TxtModeFlags
{
	MONSTATS2MODEFLAG_DT = (1 << MONMODE_DEATH),
	MONSTATS2MODEFLAG_NU = (1 << MONMODE_NEUTRAL),
	MONSTATS2MODEFLAG_WL = (1 << MONMODE_WALK),
	MONSTATS2MODEFLAG_GH = (1 << MONMODE_GETHIT),
	MONSTATS2MODEFLAG_A1 = (1 << MONMODE_ATTACK1),
	MONSTATS2MODEFLAG_A2 = (1 << MONMODE_ATTACK2),
	MONSTATS2MODEFLAG_BL = (1 << MONMODE_BLOCK),
	MONSTATS2MODEFLAG_SC = (1 << MONMODE_CAST),
	MONSTATS2MODEFLAG_S1 = (1 << MONMODE_SKILL1),
	MONSTATS2MODEFLAG_S2 = (1 << MONMODE_SKILL2),
	MONSTATS2MODEFLAG_S3 = (1 << MONMODE_SKILL3),
	MONSTATS2MODEFLAG_S4 = (1 << MONMODE_SKILL4),
	MONSTATS2MODEFLAG_DD = (1 << MONMODE_DEAD),
	MONSTATS2MODEFLAG_KB = (1 << MONMODE_KNOCKBACK),
	MONSTATS2MODEFLAG_SQ = (1 << MONMODE_SEQUENCE),
	MONSTATS2MODEFLAG_RN = (1 << MONMODE_RUN),
};

enum D2C_MonsterAlignment
{
	MONALIGN_EVIL = 0,
	MONALIGN_GOOD = 1,
	MONALIGN_NEUTRAL = 2,
};

struct D2MonStatsTxt
{
	int16_t nId;							//0x00
	int16_t nBaseId;						//0x02
	int16_t nNextInClass;					//0x04
	uint16_t wNameStr;						//0x06
	uint16_t wDescStr;						//0x08
	uint16_t unk0x0A;						//0x0A
	union
	{
		uint32_t dwMonStatsFlags;				//0x0C D2C_MonStatsTxtFlags
		uint8_t nMonStatsFlags[4];				//0x0C
	};
	uint32_t dwCode;						//0x10
	uint16_t wMonSound;						//0x14
	uint16_t wUMonSound;					//0x16
	uint16_t wMonStatsEx;					//0x18
	uint16_t wMonProp;						//0x1A
	int16_t wMonType;						//0x1C
	uint16_t wAI;							//0x1E
	uint16_t wSpawn;						//0x20
	uint8_t nSpawnX;						//0x22
	uint8_t nSpawnY;						//0x23
	uint8_t nSpawnMode;						//0x24
	uint8_t unk0x25;						//0x25
	int16_t wMinion[2];						//0x26
	int16_t nMonEquipTxtRecordId;			//0x2A
	uint8_t nPartyMin;						//0x2C
	uint8_t nPartyMax;						//0x2D
	uint8_t nRarity;						//0x2E
	uint8_t nMinGrp;						//0x2F
	uint8_t nMaxGrp;						//0x30
	uint8_t nSparsePopulate;				//0x31
	int16_t nVelocity;						//0x32
	int16_t nRun;							//0x34
	int16_t nWalkAnimSpeed;					//0x36 - Special Velocity (see DATATBLS_LoadMonStatsTxt)
	int16_t nRunAnimSpeed;						//0x38 - Special RunSpeed (see DATATBLS_LoadMonStatsTxt)
	uint16_t wMissA1;						//0x3A
	uint16_t wMissA2;						//0x3C
	uint16_t wMissS1;						//0x3E
	uint16_t wMissS2;						//0x40
	uint16_t wMissS3;						//0x42
	uint16_t wMissS4;						//0x44
	uint16_t wMissC;						//0x46
	uint16_t wMissSQ;						//0x48
	uint8_t nMaxChainId;					//0x4A
	uint8_t nChainId;						//0x4B
	uint8_t nAlign;							//0x4C D2C_MonsterAlignment
	uint8_t nTransLvl;						//0x4D
	uint8_t nThreat;						//0x4E
	uint8_t nAIdel[3];						//0x4F
	uint8_t nAiDist[3];						//0x52
	uint8_t unk0x55;						//0x55
	int16_t wAiParam[8][3];					//0x56
	uint16_t wTreasureClass[3][4];			//0x86
	uint8_t nTCQuestId;						//0x9E
	uint8_t nTCQuestCP;						//0x9F
	uint8_t nDrain[3];						//0xA0
	uint8_t nToBlock[3];					//0xA3
	uint8_t nCrit;							//0xA6
	uint8_t unk0xA7;						//0xA7
	int16_t wSkillDamage;					//0xA8
	uint16_t nLevel[3];						//0xAA
	uint16_t nMinHP[3];						//0xB0
	uint16_t nMaxHP[3];						//0xB6
	uint16_t nAC[3];						//0xBC
	uint16_t nA1TH[3];						//0xC2
	uint16_t nA2TH[3];						//0xC8
	uint16_t nS1TH[3];						//0xCE
	uint16_t nExp[3];						//0xD4
	uint16_t nA1MinD[3];					//0xDA
	uint16_t nA1MaxD[3];					//0xE0
	uint16_t nA2MinD[3];					//0xE6
	uint16_t nA2MaxD[3];					//0xEC
	uint16_t nS1MinD[3];					//0xF2
	uint16_t nS1MaxD[3];					//0xF8
	uint8_t nElMode[3];						//0xFE
	uint8_t nElType[3];						//0x101
	uint8_t nElPct[3][3];					//0x104
	uint8_t unk0x10D;						//0x10D
	uint16_t nElMinD[3][3];					//0x10E
	uint16_t nElMaxD[3][3];					//0x120
	uint16_t nElDur[3][3];					//0x132
	uint16_t nResistances[6][3];			//0x144
	int8_t nColdEffect[3];					//0x168
	uint8_t unk0x16B;						//0x16B
	uint8_t nSendSkills[4];					//0x16C
	int16_t nSkill[8];						//0x170
	uint8_t nSkillMode[8];					//0x180
	uint16_t nSequence[8];					//0x188
	uint8_t nSkLvl[8];						//0x198
	uint32_t dwDamageRegen;					//0x1A0
	uint8_t nSplEndDeath;					//0x1A4
	uint8_t nSplGetModeChart;				//0x1A5
	uint8_t nSplEndGeneric;					//0x1A6
	uint8_t nSplClientEnd;					//0x1A7
};

struct D2MonStats2Txt
{
	uint32_t dwId;							//0x00
	union
	{
		uint32_t dwFlags;						//0x04
		uint8_t nFlags[4];						//0x04
	};
	uint8_t nSizeX;							//0x08
	uint8_t nSizeY;							//0x09
	uint8_t nSpawnCol;						//0x0A
	uint8_t nHeight;						//0x0B
	uint8_t nOverlayHeight;					//0x0C
	uint8_t nPixHeight;						//0x0D
	uint8_t nMeleeRng;						//0x0E
	uint8_t pad0x0F;						//0x0F
	int32_t nBaseW;							//0x10 - Maybe union?
	//char szBaseW[4];						//0x10
	uint8_t nHitClass;						//0x14
	uint8_t unk0x15[16];					//0x15 - see CompositLinker
	uint8_t unk0x25;						//0x25 - see CompositLinker
	D2UnkMonCompStrc unk0x26[16];			//0x26 - see CompositLinker
	uint8_t pad0xE6[2];						//0xE6
	uint32_t dwComponentFlags;				//0xE8
	uint8_t nTotalPieces;					//0xEC
	uint8_t pad0xED[3];						//0xED
	union
	{
		uint32_t dwModeFlags;					//0xF0 If flag is set, mode is enabled. See D2C_MonModes/D2C_MonStats2TxtModeFlags.
		uint8_t nModeFlags[4];					//0xF0
	};
	uint8_t nModeDirections[NUMBER_OF_MONMODES]; //0xF4
	uint8_t nModeEnabledWhenMovingFlags[4];	//0x104 See D2C_MonStats2TxtModeFlags
	uint8_t nInfernoLen;					//0x108
	uint8_t nInfernoAnim;					//0x109
	uint8_t nInfernoRollback;				//0x10A
	uint8_t nResurrectMode;					//0x10B
	uint16_t nResurrectSkill;				//0x10C
	uint16_t whtTop;						//0x10E
	uint16_t whtLeft;						//0x110
	uint16_t whtWidth;						//0x112
	uint16_t whtHeight;						//0x114
	uint8_t pad0x116[2];					//0x116
	uint32_t dwAutomapCel;					//0x118
	uint8_t nLocalBlood;					//0x11C
	uint8_t nBleed;							//0x11D
	uint8_t nLight;							//0x11E
	uint8_t nLight_R;						//0x11F
	uint8_t nLight_G;						//0x120
	uint8_t nLight_B;						//0x121
	uint8_t nUtrans[3];						//0x122
	uint8_t pad0x125[3];					//0x125
	char szHeart[4];						//0x128
	char szBodyPart[4];						//0x12C
	uint8_t nRestore;						//0x130
	uint8_t pad0x131[3];					//0x131
};

struct D2MonTypeTxt
{
	uint16_t wType;							//0x00
	short nEquiv[3];						//0x02
	uint16_t wStrSing;						//0x08
	uint16_t wStrPlur;						//0x0A
};

struct D2MonUModTxt
{
	uint32_t dwUniqueMod;					//0x00
	uint16_t wVersion;						//0x04
	uint8_t nEnabled;						//0x06
	uint8_t nXfer;							//0x07
	uint8_t nChampion;						//0x08
	uint8_t nFPick;							//0x09
	int16_t wExclude[2];					//0x0A
	uint16_t wCPick[3];						//0x0E
	uint16_t wUPick[3];						//0x14
	uint16_t pad0x1A;						//0x1A
	uint32_t dwConstants;					//0x1C
};

struct D2NpcTxt
{
	uint32_t dwNpc;							//0x00
	uint32_t dwSellMult;					//0x04
	uint32_t dwBuyMult;						//0x08
	uint32_t dwRepMult;						//0x0C
	uint32_t dwQuestFlag[3];				//0x10
	uint32_t dwQuestSellMult[3];			//0x1C
	uint32_t dwQuestBuyMult[3];				//0x28
	uint32_t dwQuestRepMult[3];				//0x34
	int32_t nMaxBuy[3];						//0x40
};

struct D2SuperUniquesTxt
{
	uint16_t wSuperUnique;					//0x00
	uint16_t wNameStr;						//0x02
	uint32_t dwClass;						//0x04
	uint32_t dwHcIdx;						//0x08
	uint32_t dwMod[3];						//0x0C
	uint32_t dwMonSound;					//0x18
	uint32_t dwMinGrp;						//0x1C
	uint32_t dwMaxGrp;						//0x20
	uint8_t nAutopos;						//0x24
	uint8_t nEclass;						//0x25
	uint8_t nStacks;						//0x26
	uint8_t nReplaceable;					//0x27
	uint8_t nUtrans[3];						//0x28
	uint8_t pad0x2B;						//0x2B
	uint16_t dwTC[3];						//0x2C
	uint16_t pad0x32;						//0x32
};

struct D2TreasureClassExTxt
{
	char szTreasureClass[32];				//0x00
	int32_t nPicks;							//0x20
	int16_t nGroup;							//0x24
	int16_t nLevel;							//0x26
	int16_t nMagic;							//0x28
	int16_t nRare;							//0x2A
	int16_t nSet;							//0x2C
	int16_t nUnique;						//0x2E
	int16_t nSuperior;						//0x30
	int16_t nNormal;						//0x32
	int32_t nNoDrop;						//0x34
	char szItem[10][64];					//0x38
	int32_t nProb[10];						//0x2B8
};

struct D2UniqueAppellationTxt
{
	uint16_t wStringId;						//0x00
};

struct D2UniquePrefixTxt
{
	uint16_t wStringId;						//0x00
};

struct D2UniqueSuffixTxt
{
	uint16_t wStringId;						//0x00
};

struct D2UniqueTitleTxt
{
	uint16_t wStringId;						//0x00
};

struct D2HirelingTxt
{
	uint16_t wVersion;						//0x00
	uint16_t pad0x02;						//0x02
	int32_t nId;							//0x04
	uint32_t dwClass;						//0x08
	uint32_t dwAct;							//0x0C
	uint32_t dwDifficulty;					//0x10
	uint32_t dwSeller;						//0x14
	int32_t nGold;							//0x18
	int32_t nHirelingLevel;					//0x1C
	uint32_t dwExpPerLvl;					//0x20
	uint32_t dwHitpoints;					//0x24
	uint32_t dwHitpointsPerLvl;				//0x28
	int32_t dwDefense;						//0x2C
	int32_t dwDefensePerLvl;				//0x30
	int32_t nStr;							//0x34
	int32_t nStrPerLvl;						//0x38
	int32_t nDex;							//0x3C
	int32_t nDexPerLvl;						//0x40
	int32_t dwAttackRate;					//0x44
	int32_t dwAttackRatePerLvl;				//0x48
	uint32_t dwShare;						//0x4C
	int32_t nDmgMin;						//0x50
	int32_t nDmgMax;						//0x54
	int32_t nDmgPerLvl;						//0x58
	int32_t dwResist;						//0x5C
	int32_t dwResistPerLvl;					//0x60
	uint32_t dwDefaultChance;				//0x64
	uint32_t dwHead;						//0x68
	uint32_t dwTorso;						//0x6C
	uint32_t dwWeapon;						//0x70
	uint32_t dwShield;						//0x74
	uint32_t dwSkill[6];					//0x78
	uint32_t dwChance[6];					//0x90
	uint32_t dwChancePerLvl[6];				//0xA8
	uint8_t nMode[6];						//0xC0
	uint8_t nLevel[6];						//0xC6
	uint8_t nSLvlPerLvl[6];					//0xCC
	uint8_t nHireDesc;						//0xD2
	char szNameFirst[32];					//0xD3
	char szNameLast[32];					//0xF3
	uint8_t pad0x113;						//0x113
	uint16_t wNameFirst;					//0x114
	uint16_t wNameLast;						//0x116
};

struct D2SoundsTxtStub
{
	uint16_t wSound;						//0x00
};

struct D2MonStatsInitStrc
{
	int32_t nMinHP;								//0x00
	int32_t nMaxHP;								//0x04
	int32_t nAC;								//0x08
	int32_t nTH;								//0x0C
	int32_t nExp;								//0x10
	int32_t nA1MinD;							//0x14
	int32_t nA1MaxD;							//0x18
	int32_t nA2MinD;							//0x1C
	int32_t nA2MaxD;							//0x20
	int32_t nS1MinD;							//0x24
	int32_t nS1MaxD;							//0x28
	int32_t nElMinD;							//0x2C
	int32_t nElMaxD;							//0x30
	int32_t nElDur;								//0x34
};

#pragma pack()


//Inlined in some functions
uint32_t __fastcall DATATBLS_StringToCode(char* szText);
//D2Common.0x6FD64C40 (#10592)
D2COMMON_DLL_DECL int __stdcall DATATBLS_GetMonsterChainInfo(int nMonsterId, int* pMaxChainId, int* pChainId);
//D2Common.0x6FD64CD0
void __fastcall DATATBLS_MonStatsSkillModeLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD64DF0
void __fastcall DATATBLS_LoadMonStatsTxt(void* pMemPool);
//D2Common.0x6FD67BD0 (#11261)
D2COMMON_DLL_DECL BOOL __stdcall DATATBLS_CalculateMonsterStatsByLevel(int nMonsterId, int nGameType, int nDifficulty, int nLevel, short nFlags, D2MonStatsInitStrc* pMonStatsInit);
//D2Common.0x6FD689B0 (#10642)
D2COMMON_DLL_DECL void __stdcall DATATBLS_SetVelocityInMonStatsTxtRecord(int nMonsterId, short nVelocity);
//D2Common.0x6FD68A00
void __stdcall DATATBLS_ResetGlobalDefaultUnicodeString();
//D2Common.0x6FD68A10 (#10651)
D2COMMON_DLL_DECL const Unicode* __fastcall DATATBLS_RollRandomUniqueTitleString(D2UnitStrc* pUnit);
//D2Common.0x6FD68A80 (#10652)
D2COMMON_DLL_DECL const Unicode* __fastcall DATATBLS_RollRandomUniquePrefixString(D2UnitStrc* pUnit);
//D2Common.0x6FD68AF0 (#10653)
D2COMMON_DLL_DECL const Unicode* __fastcall DATATBLS_RollRandomUniqueSuffixString(D2UnitStrc* pUnit);
//D2Common.0x6FD68B60 (#10654)
D2COMMON_DLL_DECL const Unicode* __fastcall DATATBLS_RollRandomUniqueAppellationString(D2UnitStrc* pUnit);
//D2Common.0x6FD68BD0
void __fastcall DATATBLS_ReallocTCExInfo(D2TCExShortStrc* pTCExShort, int nNewRecordCount);
//D2Common.0x6FD68C40
void __fastcall DATATBLS_UpdateTreasureClassProbabilities(D2TCExShortStrc* pTCExShort, D2TCExInfoStrc* pTCExInfo, int nProbability, BOOL bExpansion);
//D2Common.0x6FD68D40
int __fastcall DATATBLS_ApplyRatio(signed int nValue, signed int nMultiplier, signed int nDivisor);
//D2Common.0x6FD68DC0 (#10658)
D2COMMON_DLL_DECL D2TCExShortStrc* __stdcall DATATBLS_GetTreasureClassExRecordFromName(char* szText);
//D2Common.0x6FD68DF0 (#10659)
D2COMMON_DLL_DECL D2TCExShortStrc* __stdcall DATATBLS_GetTreasureClassExRecordFromIdAndLevel(uint16_t wTCId, int nLvl);
//D2Common.0x6FD68E50 (#10660)
D2COMMON_DLL_DECL D2TCExShortStrc* __stdcall DATATBLS_GetTreasureClassExRecordFromActAndDifficulty(int nDifficulty, int nAct, int nIndex);
//D2Common.0x6FD68EC0
void __fastcall DATATBLS_LoadTreasureClassExTxt(void* pMemPool);
//D2Common.0x6FD69B70 (#10656)
D2COMMON_DLL_DECL int __stdcall DATATBLS_ShouldNeverCallInExpansion();
//D2Common.0x6FD69B90
void __fastcall DATATBLS_LoadMonItemPercentTxt(void* pMemPool);
//D2Common.0x6FD69C40
void __fastcall DATATBLS_UnloadMonItemPercentTxt();
//D2Common.0x6FD69C50 (#10662)
D2COMMON_DLL_DECL D2MonItemPercentDataTbl* __fastcall DATATBLS_GetMonItemPercentDataTables();
//D2Common.0x6FD69C60 (#10663)
D2COMMON_DLL_DECL D2MonItemPercentTxt* __stdcall DATATBLS_GetMonItemPercentTxtRecord(int nId);
//D2Common.0x6FD69CD0
void __fastcall DATATBLS_LoadMonUModTxt(void* pMemPool);
//D2Common.0x6FD69FA0
void __fastcall DATATBLS_LoadSuperUniquesTxt(void* pMemPool);
//D2Common.0x6FD6A440 (#10668)
D2COMMON_DLL_DECL D2SuperUniquesTxt* __stdcall DATATBLS_GetSuperUniquesTxtRecord(int nSuperUniqueId);
//D2Common.0x6FD6A470 (#11257)
D2COMMON_DLL_DECL int __fastcall DATATBLS_GetSuperUniquesTxtRecordCount();
//D2Common.0x6FD6A480
void __fastcall DATATBLS_LoadHirelingTxt(void* pMemPool);
//D2Common.0x6FD6B1A0
D2ItemTypesTxt* __fastcall DATATBLS_GetItemTypesTxtRecord(int nItemType);
//D2Common.0x6FD6B1D0 (#10583)
D2COMMON_DLL_DECL D2HirelingTxt* __stdcall DATATBLS_GetHirelingTxtRecordFromIdAndLevel(BOOL bExpansion, int nId, int nLevel);
//D2Common.0x6FD6B270 (#10585)
D2COMMON_DLL_DECL D2HirelingTxt* __stdcall DATATBLS_GetNextHirelingTxtRecordFromNameId(BOOL bExpansion, uint16_t nNameId, D2HirelingTxt* pOldRecord);
//D2Common.0x6FD6B310 (#10586)
D2COMMON_DLL_DECL D2HirelingTxt* __stdcall DATATBLS_GetNextHirelingTxtRecordFromClassId(BOOL bExpansion, int nClass, D2HirelingTxt* pOldRecord);
//D2Common.0x6FD6B3A0 (#10587)
D2COMMON_DLL_DECL D2HirelingTxt* __stdcall DATATBLS_GetNextHirelingTxtRecordFromVendorIdAndDifficulty(BOOL bExpansion, int nVendorId, int nDifficulty, D2HirelingTxt* pOldRecord);
//D2Common.0x6FD6B440 (#10584)
D2COMMON_DLL_DECL D2HirelingTxt* __stdcall DATATBLS_GetNextHirelingTxtRecordFromActAndDifficulty(BOOL bExpansion, int nAct, int nDifficulty, D2HirelingTxt* pOldRecord);
//D2Common.0x6FD6B4F0
void __fastcall DATATBLS_LoadNpcTxt(void* pMemPool);
//D2Common.0x6FD6B820 (#10588)
D2COMMON_DLL_DECL D2NpcTxt* __stdcall DATATBLS_GetNpcTxtRecord(uint32_t dwNpcId);
//D2Common.0x6FD6B850
void __fastcall DATATBLS_LoadMonSoundsTxt(void* pMemPool);
//D2Common.0x6FD6BF50 (#11252)
D2COMMON_DLL_DECL D2MonSoundsTxt* __stdcall DATATBLS_GetMonSoundsTxtRecordFromSoundId(int nSoundId);
//D2Common.0x6FD6BF80 (#11250)
D2COMMON_DLL_DECL D2MonSoundsTxt* __stdcall DATATBLS_GetMonSoundsTxtRecordFromMonsterId(int nMonsterId);
//D2Common.0x6FD6BFE0
void __fastcall DATATBLS_MonStats2CompositLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD6C150
void __fastcall DATATBLS_LoadMonStats2Txt(void* pMemPool);
//D2Common.0x6FD6D660
int __fastcall DATATBLS_CheckNestedMonsterTypes(int nMonType1, int nMonType2);
//D2Common.0x6FD6D730
void __fastcall DATATBLS_LoadMonTypeTxt(void* pMemPool);
//D2Common.0x6FD6D910
void __fastcall DATATBLS_UnloadMonTypeTxt();
//D2Common.0x6FD6D960
void __fastcall DATATBLS_LoadMonPropTxt(void* pMemPool);
//D2Common.0x6FD6E8E0
void __fastcall DATATBLS_LoadMonLvlTxt(void* pMemPool);
//D2Common.0x6FD6EDE0
void __fastcall DATATBLS_MonPresetPlaceLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD6EE60
void __fastcall DATATBLS_LoadMonPresetTxt(void* pMemPool);
//D2Common.0x6FD6EF30 (#11256)
D2COMMON_DLL_DECL D2MonPresetTxt* __stdcall DATATBLS_GetMonPresetTxtActSection(int nAct, int* pRecordCount);
//D2Common.0x6FD6EF70 (#11258)
D2COMMON_DLL_DECL int __stdcall DATATBLS_MapSuperUniqueId(int nType, int nSuperUnique);
//D2Common.0x6FD6EFA0
void __fastcall DATATBLS_LoadSoundsTxt(void* pMemPool);
//D2Common.0x6FD6F020
void __fastcall DATATBLS_UnloadSoundsTxt();
//D2Common.0x6FD6F230
void __fastcall DATATBLS_LoadMonEquipTxt(void* pMemPool);
//D2Common.0x6FD6F580
void __fastcall DATATBLS_LoadSomeMonsterTxts(void* pMemPool);
//D2Common.0x6FD6F810
void __fastcall DATATBLS_CreateItemTypeTreasureClasses();
//D2Common.0x6FD6FBB0
void __fastcall DATATBLS_UnloadSomeMonsterTxts();