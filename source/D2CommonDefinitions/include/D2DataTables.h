#pragma once
#include <cstdint>

#pragma pack(1)

//FORWARD DECLARATIONS
struct D2MonSeqTxt;
struct D2BinFieldStrc;
struct D2DrlgFileStrc;
struct D2TileLibraryHashStrc;


//LINKER

struct D2TxtLinkNodeStrc
{
	char szText[32];				//0x00
	int nLinkIndex;					//0x20
	D2TxtLinkNodeStrc* pPrevious;	//0x24
	D2TxtLinkNodeStrc* pNext;		//0x28
};

struct D2TxtLinkTblStrc
{
	union
	{
		char szCode[4];				//0x00
		uint32_t dwCode;				//0x00
	};
	int nLinkIndex;					//0x04
};

struct D2TxtLinkStrc
{
	int nRecords;					//0x00
	int nAllocatedCells;			//0x04
	D2TxtLinkTblStrc* pTbl;			//0x08
	D2TxtLinkNodeStrc* pFirstNode;	//0x0C
};


struct D2BinFileStrc
{
	uint8_t* pDataBuffer;				//0x00
	uint8_t* pData;					//0x04
	int nRecordCount;				//0x08
	int nCellCount;					//0x0C
};

struct D2UnkExcelStrc
{
	D2UnkExcelStrc* pNext;			//0x00
	uint32_t dwHash;					//0x04
	D2BinFieldStrc* pBinField;		//0x08
};


//SUB-STRUCTURES

struct D2AutomapRandStrc
{
	int nStart;							//0x00
	int nEnd;							//0x04
};

struct D2AutomapShortStrc
{
	uint32_t dwAutomapLevelType;			//0x00
	uint32_t dwAutomapTileType;			//0x04
	char nStyle;						//0x08
	char nStartSequence;				//0x09
	char nEndSequence;					//0x0A
	uint8_t pad0x0B;						//0x0B
	int nCel[4];						//0x0C
	int nCels;							//0x1C
};

struct D2CharItemStrc
{
	uint32_t dwItemCode;					//0x00
	uint8_t nBodyLoc;						//0x04
	char nItemCount;					//0x05
	uint16_t pad0x06;						//0x06
};

struct D2CollisionFieldStrc
{
	int nWidth;							//0x00
	int nHeight;						//0x04
	int nArea;							//0x08
	int nCenterX;						//0x0C
	int nCenterY;						//0x10
};

struct D2CubeInputItem
{
	uint16_t wInputFlags;					//0x00
	uint16_t wItem;							//0x02
	uint16_t wItemID;						//0x04
	uint8_t nQuality;						//0x06
	uint8_t nQuantity;						//0x07
};

struct D2CubeOutputParamStrc
{
	int nMod;							//0x00
	uint16_t nModParam;						//0x04
	short nModMin;						//0x06
	short nModMax;						//0x08
	uint16_t nModChance;					//0x0A
};

struct D2CubeOutputItem
{
	uint16_t wItemFlags;					//0x00
	uint16_t wBaseItemId;					//0x02
	uint16_t wItemID;						//0x04
	uint8_t nQuality;						//0x06
	uint8_t nQuantity;						//0x07
	uint8_t nType;							//0x08
	uint8_t nLvl;							//0x09
	uint8_t nPLvl;							//0x0A
	uint8_t nILvl;							//0x0B
	uint16_t wPrefixId[3];					//0x0C
	uint16_t wSuffixId[3];					//0x12
	D2CubeOutputParamStrc pParam[5];	//0x18
};

struct D2DrlgGridStrc
{
	int* pCellsFlags;							//0x00
	int* pCellsRowOffsets;					//0x04
	int nWidth;								//0x08
	int nHeight;							//0x0C
	int unk0x10;							//0x10 Maybe indicates if uninitialized? No memset when set to 1
};

struct D2InvCompGridStrc
{
	int nLeft;							//0x00
	int nRight;							//0x04
	int nTop;							//0x08
	int nBottom;						//0x0C
	uint8_t nWidth;						//0x10
	uint8_t nHeight;						//0x11
	uint16_t pad0x12;						//0x12
};

struct D2InventoryGridInfoStrc
{
	uint8_t nGridX;							//0x00
	uint8_t nGridY;							//0x01
	uint16_t pad0x02;							//0x02
	int nGridLeft;							//0x04
	int nGridRight;							//0x08
	int nGridTop;							//0x0C
	int nGridBottom;						//0x10
	uint8_t nGridBoxWidth;						//0x14
	uint8_t nGridBoxHeight;					//0x15
	uint16_t pad0x16;							//0x16
};

struct D2InvRectStrc
{
	int nLeft;							//0x00
	int nRight;							//0x04
	int nTop;							//0x08
	int nBottom;						//0x0C
};

struct D2ItemStatCostDescStrc
{
	uint16_t nRecordId;						//0x00
	short nDescPriority;				//0x02
};

struct D2OpStatDataStrc
{
	uint16_t nOpBase;						//0x00
	uint16_t nStat;						//0x02
	uint8_t nOp;							//0x04
	uint8_t nOpParam;						//0x05
};

//TODO: Find a better name - Called ptTileLibrary by D2CMP.#10087
struct D2UnkLvlDataTableStrc
{
	uint8_t unk0x00[128][32];
};

struct D2PropertyStrc
{
	int nProperty;						//0x00
	int nLayer;							//0x04
	int nMin;							//0x08
	int nMax;							//0x0C
};

struct D2SeedStrc
{
	union
	{
		struct
		{
			int nLowSeed;					//0x00
			int nHighSeed;					//0x04
		};
		unsigned long long int lSeed;		//0x00
	};
};

struct D2SeqRecordStrc
{
	D2MonSeqTxt* pMonSeqTxtRecord;		//0x00
	int unk0x04;						//0x04
	int unk0x08;						//0x08
};

//TODO: Find a better name
struct D2UnkSequenceStrc
{
	D2SeqRecordStrc unk0x00[14];		//0x00
};

//TODO: Find a better name
struct D2UnkMonCompStrc
{
	char nComposit[12];
	//int unk0x04;
	//int unk0x08;
};

struct D2TCExInfoStrc
{
	int nClassic;
	int nProb;
	short nItemId;
	short nTxtRow;
	uint8_t nFlags;
	char unk0x0D;
	short unk0x0E;
	short unk0x10;
	short unk0x12;
	short unk0x14;
	short unk0x16;
	short unk0x18;
	short unk0x1A;
};

struct D2TCExShortStrc
{
	short nGroup;							//0x00
	short nLevel;							//0x02
	int nTypes;								//0x04
	int nClassic;							//0x08
	int nProb;								//0x0C
	int nPicks;								//0x10
	int nNoDrop;							//0x14
	short unk0x18;							//0x18
	short nMagic;							//0x1A
	short nRare;							//0x1C
	short nSet;								//0x1E
	short nUnique;							//0x20
	short nSuperior;						//0x22
	short nNormal;							//0x24
	short unk0x26;							//0x26
	D2TCExInfoStrc* pInfo;					//0x28
};


//DATATABLES

struct D2AnimDataRecordStrc
{
	char szAnimDataName[8];					//0x00
	uint32_t dwFrames;							//0x08
	uint32_t dwAnimSpeed;						//0x0C
	uint8_t pFrameFlags[144];					//0x10
};

struct D2AnimDataStrc
{
	D2AnimDataRecordStrc* pRecords;			//0x00
	D2AnimDataRecordStrc* pHashTable[256];	//0x04
	D2AnimDataRecordStrc pDefaultRecord;	//0x404
};

struct D2ArenaTxt
{
	uint32_t dwSuicide;					//0x00
	uint32_t dwPlayerKill;					//0x04
	uint32_t dwPlayerKillPercent;			//0x08
	uint32_t dwMonsterKill;				//0x0C
	uint32_t dwPlayerDeath;				//0x10
	uint32_t dwPlayerDeathPercent;			//0x14
	uint32_t dwMonsterDeath;				//0x18
};

struct D2ArmTypeTxt
{
	char szName[32];					//0x00
	char szToken[20];					//0x20
};

struct D2AutomapTxt
{
	char szLevelName[16];				//0x00
	char szTileName[8];					//0x10
	char nStyle;						//0x18
	char nStartSequence;				//0x19
	char nEndSequence;					//0x1A
	uint8_t pad0x1B;						//0x1B
	int nCel[4];						//0x1C
};

struct D2BeltsTxt
{
	uint8_t nIndex;						//0x00
	uint8_t pad0x01[3];					//0x01
	int nBoxes;							//0x04
	D2InvRectStrc beltBoxes[16];		//0x08
};

struct D2BodyLocsTxt
{
	uint32_t dwCode;						//0x00
};

struct D2BooksTxt
{
	uint16_t wStr;							//0x00
	uint8_t nSpellIcon;					//0x02
	uint8_t pad0x03;						//0x03
	uint32_t dwPSpell;						//0x04
	uint32_t dwScrollSkillId;				//0x08
	uint32_t dwBookSkillId;				//0x0C
	uint32_t dwBaseCost;					//0x10
	uint32_t dwCostPerCharge;				//0x14
	uint32_t dwScrollSpellCode;			//0x18
	uint32_t dwBookSpellCode;				//0x1C
};

struct D2CharStatsTxt
{
	wchar_t wszClassName[16];			//0x00
	char szClassName[16];				//0x20
	uint8_t nStr;							//0x30
	uint8_t nDex;							//0x31
	uint8_t nInt;							//0x32
	uint8_t nVit;							//0x33
	uint8_t nStamina;						//0x34
	uint8_t nLifeAdd;						//0x35
	uint8_t nPercentStr;					//0x36
	uint8_t nPercentInt;					//0x37
	uint8_t nPercentDex;					//0x38
	uint8_t nPercentVit;					//0x39
	uint8_t nManaRegen;					//0x3A
	uint8_t pad0x3B;						//0x3B
	uint32_t dwToHitFactor;				//0x3C
	uint8_t nWalkSpeed;					//0x40
	uint8_t nRunSpeed;						//0x41
	uint8_t nRunDrain;						//0x42
	uint8_t nLifePerLevel;					//0x43
	uint8_t nStaminaPerLevel;				//0x44
	uint8_t nManaPerLevel;					//0x45
	uint8_t nLifePerVitality;				//0x46
	uint8_t nStaminaPerVitality;			//0x47
	uint8_t nManaPerMagic;					//0x48
	uint8_t nBlockFactor;					//0x49
	uint16_t pad0x4A;						//0x4A
	uint32_t dwBaseWClass;					//0x4C
	uint8_t nStatsPerLevel;				//0x50
	uint8_t pad0x51;						//0x51
	uint16_t wStrAllSkills;					//0x52
	uint16_t wStrSkillTab[3];				//0x54
	uint16_t wStrClassOnly;					//0x5A
	D2CharItemStrc pItems[10];			//0x5C
	uint16_t wStartSkill;					//0xAC
	short nBaseSkill[10];				//0xAE
	uint16_t pad0xC2;						//0xC2
};

struct D2CharTemplateTxt
{
	char szName[30];					//0x00
	uint8_t nClass;						//0x1E
	uint8_t nLevel;						//0x1F
	uint8_t nAct;							//0x20
	uint8_t nStr;							//0x21
	uint8_t nDex;							//0x22
	uint8_t nInt;							//0x23
	uint8_t nVit;							//0x24
	uint8_t nMana;							//0x25
	uint8_t nHitpoints;					//0x26
	uint8_t nManaRegenBonus;				//0x27
	uint8_t nVelocity;						//0x28
	uint8_t nAttackRate;					//0x29
	uint8_t nOtherRate;					//0x2A
	uint8_t pad0x2B;						//0x2B
	uint32_t dwRightSkill;					//0x2C
	uint32_t dwSkill[9];					//0x30
	uint32_t dwSkillLevel[9];				//0x54
	D2CharItemStrc pItems[15];			//0x78
};

struct D2ColorsTxt
{
	uint32_t dwCode;						//0x00
};

struct D2CompCodeTxt
{
	uint32_t dwCode;						//0x00
};

struct D2CompositTxt
{
	char szName[32];					//0x00
	char szToken[20];					//0x20
};

struct D2CubeMainTxt
{
	uint8_t nEnabled;						//0x00
	uint8_t nLadder;						//0x01
	uint8_t nMinDiff;						//0x02
	uint8_t nClass;						//0x03
	uint8_t nOp;							//0x04
	uint8_t pad0x05[3];					//0x05
	int nParam;							//0x08
	int nValue;							//0x0C
	char nNumInputs;					//0x10
	uint8_t pad0x11;						//0x11
	uint16_t wVersion;						//0x12
	D2CubeInputItem pInputItem[7];		//0x14
	D2CubeOutputItem pOutputItem[3];	//0x4C
};

struct D2DifficultyLevelsTxt
{
	uint32_t dwResistPenalty;				//0x00
	uint32_t dwDeathExpPenalty;			//0x04
	uint32_t dwUberCodeOddsNorm;			//0x08
	uint32_t dwUberCodeOddsGood;			//0x0C
	uint32_t dwMonsterSkillBonus;			//0x10
	uint32_t dwMonsterFreezeDiv;			//0x14
	uint32_t dwMonsterColdDiv;				//0x18
	uint32_t dwAiCurseDiv;					//0x1C
	uint32_t dwUltraCodeOddsNorm;			//0x20
	uint32_t dwUltraCodeOddsGood;			//0x24
	uint32_t dwLifeStealDiv;				//0x28
	uint32_t dwManaStealDiv;				//0x2C
	uint32_t dwUniqueDmgBonus;				//0x30
	uint32_t dwChampionDmgBonus;			//0x34
	uint32_t dwHireableBossDmgPercent;		//0x38
	uint32_t dwMonsterCEDmgPercent;		//0x3C
	uint32_t dwStaticFieldMin;				//0x40
	uint32_t dwGambleRare;					//0x44
	uint32_t dwGambleSet;					//0x48
	uint32_t dwGambleUniq;					//0x4C
	uint32_t dwGambleUber;					//0x50
	uint32_t dwGambleUltra;				//0x54
};

struct D2ElemTypesTxt
{
	uint32_t dwCode;						//0x00
};

struct D2EventsTxt
{
	uint16_t wEvent;						//0x00
};

struct D2ExperienceTxt
{
	uint32_t dwClass[7];					//0x00
	uint32_t dwExpRatio;					//0x1C
};

struct D2GambleTxt
{
	uint32_t dwItemCode;					//0x00
	int nLevel;							//0x04
	int nItemId;						//0x08
	//char szItemCode[12];				//0x00
};

struct D2GemsTxt
{
	char szName[32];					//0x00
	char szLetter[8];					//0x20
	union
	{
		uint32_t dwItemCode;					//0x28
		char szItemCode[4];					//0x28
	};
	uint16_t wStringId;						//0x2C
	uint8_t nNumMods;						//0x2E
	uint8_t nTransForm;					//0x2F
	D2PropertyStrc pProperties[3][3];	//0x30
};

struct D2HireDescTxt
{
	uint32_t dwCode;						//0x00
};

struct D2HirelingTxt
{
	uint16_t wVersion;						//0x00
	uint16_t pad0x02;						//0x02
	int nId;							//0x04
	uint32_t dwClass;						//0x08
	uint32_t dwAct;						//0x0C
	uint32_t dwDifficulty;					//0x10
	uint32_t dwSeller;						//0x14
	int nGold;							//0x18
	int nHirelingLevel;					//0x1C
	uint32_t dwExpPerLvl;					//0x20
	uint32_t dwHitpoints;					//0x24
	uint32_t dwHitpointsPerLvl;			//0x28
	uint32_t dwDefense;					//0x2C
	uint32_t dwDefensePerLvl;				//0x30
	int nStr;							//0x34
	int nStrPerLvl;						//0x38
	int nDex;							//0x3C
	int nDexPerLvl;						//0x40
	uint32_t dwAttackRate;					//0x44
	uint32_t dwAttackRatePerLvl;			//0x48
	uint32_t dwShare;						//0x4C
	int nDmgMin;						//0x50
	int nDmgMax;						//0x54
	int nDmgPerLvl;						//0x58
	uint32_t dwResist;						//0x5C
	uint32_t dwResistPerLvl;				//0x60
	uint32_t dwDefaultChance;				//0x64
	uint32_t dwHead;						//0x68
	uint32_t dwTorso;						//0x6C
	uint32_t dwWeapon;						//0x70
	uint32_t dwShield;						//0x74
	uint32_t dwSkill[6];					//0x78
	uint32_t dwChance[6];					//0x90
	uint32_t dwChancePerLvl[6];			//0xA8
	uint8_t nMode[6];						//0xC0
	uint8_t nLevel[6];						//0xC6
	uint8_t nSLvlPerLvl[6];				//0xCC
	uint8_t nHireDesc;						//0xD2
	char szNameFirst[32];				//0xD3
	char szNameLast[32];				//0xF3
	uint8_t pad0x113;						//0x113
	uint16_t wNameFirst;					//0x114
	uint16_t wNameLast;						//0x116
};

struct D2HitClassTxt
{
	uint32_t dwCode;						//0x00
};

struct D2InventoryTxt
{
	D2InvRectStrc pRect;				//0x00
	D2InventoryGridInfoStrc pGridInfo;	//0x10
	D2InvCompGridStrc pComponents[10];	//0x28
};

struct D2ItemRatioTxt
{
	uint32_t dwUnique;						//0x00
	uint32_t dwUniqueDivisor;				//0x04
	uint32_t dwUniqueMin;					//0x08
	uint32_t dwRare;						//0x0C
	uint32_t dwRareDivisor;				//0x10
	uint32_t dwRareMin;					//0x14
	uint32_t dwSet;						//0x18
	uint32_t dwSetDivisor;					//0x1C
	uint32_t dwSetMin;						//0x20
	uint32_t dwMagic;						//0x24
	uint32_t dwMagicDivisor;				//0x28
	uint32_t dwMagicMin;					//0x2C
	uint32_t dwHiQuality;					//0x30
	uint32_t dwHiQualityDivisor;			//0x34
	uint32_t dwNormal;						//0x38
	uint32_t dwNormalDivisor;				//0x3C
	uint16_t wVersion;						//0x40
	uint8_t nUber;							//0x42
	uint8_t nClassSpecific;				//0x43 
};

struct D2ItemStatCostTxt
{
	uint16_t wStatId;						//0x00
	uint16_t pad0x02;						//0x02
	uint32_t dwItemStatFlags;				//0x04
	uint8_t nSendBits;						//0x08
	uint8_t nSendParamBits;				//0x09
	uint8_t nCsvBits;						//0x0A
	uint8_t nCsvParam;						//0x0B
	uint32_t dwDivide;						//0x0C
	uint32_t dwMultiply;					//0x10
	uint32_t dwAdd;						//0x14
	uint8_t nValShift;						//0x18
	uint8_t nSaveBits;						//0x19
	uint8_t n09SaveBits;					//0x1A
	uint8_t pad0x1B;						//0x1B
	uint32_t dwSaveAdd;					//0x1C
	uint32_t dw09SaveAdd;					//0x20
	uint32_t dwSaveParamBits;				//0x24
	uint32_t dw09SaveParamBits;			//0x28
	int dwMinAccr;						//0x2C
	uint8_t nEncode;						//0x30
	uint8_t pad0x31;						//0x31
	uint16_t wMaxStat;						//0x32
	short nDescPriority;				//0x34
	uint8_t nDescFunc;						//0x36
	uint8_t nDescVal;						//0x37
	uint16_t wDescStrPos;					//0x38
	uint16_t wDescStrNeg;					//0x3A
	uint16_t wDescStr2;						//0x3C
	uint16_t wDescGrp;						//0x3E
	uint8_t nDescGrpFunc;					//0x40
	uint8_t nDescGrpVal;					//0x41
	uint16_t wDescGrpStrPos;				//0x42
	uint16_t wDescGrpStrNeg;				//0x44
	uint16_t wDescGrpStr2;					//0x46
	uint16_t wItemEvent[2];					//0x48
	uint16_t wItemEventFunc[2];				//0x4C
	uint8_t nKeepZero;						//0x50
	uint8_t unk0x51[3];					//0x51 - also related to op stats (see DATATBLS_LoadItemStatCostTxt)
	uint8_t nOp;							//0x54
	uint8_t nOpParam;						//0x55
	uint16_t wOpBase;						//0x56
	uint16_t wOpStat[3];					//0x58
	uint16_t unk0x5E[64];					//0x5E - also related to op stats (see DATATBLS_LoadItemStatCostTxt)
	D2OpStatDataStrc pOpStatData[16];	//0xDE
	uint16_t pad0x13E;						//0x13E
	uint32_t dwStuff;						//0x140
};

struct D2ItemsTxt
{
	char szFlippyFile[32];				//0x00
	char szInvFile[32];					//0x20
	char szUniqueInvFile[32];			//0x40
	char szSetInvFile[32];				//0x60
	union
	{
		uint32_t dwCode;					//0x80
		char szCode[4];					//0x80
	};
	uint32_t dwNormCode;					//0x84
	uint32_t dwUberCode;					//0x88
	uint32_t dwUltraCode;					//0x8C
	uint32_t dwAlternateGfx;				//0x90
	uint32_t dwPspell;						//0x94
	uint16_t wState;						//0x98
	uint16_t wCurseState[2];				//0x9A
	uint16_t wStat[3];						//0x9E
	uint32_t dwCalc[3];					//0xA4
	uint32_t dwLen;						//0xB0
	uint8_t nSpellDesc;					//0xB4
	uint8_t pad0xB5;						//0xB5
	uint16_t wSpellDescStr;					//0xB6
	uint32_t dwSpellDescCalc;				//0xB8
	uint32_t dwBetterGem;					//0xBC
	uint32_t dwWeapClass;					//0xC0
	uint32_t dwWeapClass2Hand;				//0xC4
	uint32_t dwTransmogrifyType;			//0xC8
	int dwMinAc;						//0xCC
	int dwMaxAc;						//0xD0
	uint32_t dwGambleCost;					//0xD4
	int dwSpeed;						//0xD8
	uint32_t dwBitField1;					//0xDC
	uint32_t dwCost;						//0xE0
	uint32_t dwMinStack;					//0xE4
	uint32_t dwMaxStack;					//0xE8
	uint32_t dwSpawnStack;					//0xEC
	uint32_t dwGemOffset;					//0xF0
	uint16_t wNameStr;						//0xF4
	uint16_t wVersion;						//0xF6
	uint16_t wAutoPrefix;					//0xF8
	uint16_t wMissileType;					//0xFA
	uint8_t nRarity;						//0xFC
	uint8_t nLevel;						//0xFD
	uint8_t nMinDam;						//0xFE
	uint8_t nMaxDam;						//0xFF
	uint8_t nMinMisDam;					//0x100
	uint8_t nMaxMisDam;					//0x101
	uint8_t n2HandMinDam;					//0x102
	uint8_t n2HandMaxDam;					//0x103
	uint8_t nRangeAdder;					//0x104
	uint8_t unk0x105;						//0x105
	short nStrBonus;					//0x106
	short nDexBonus;					//0x108
	uint16_t wReqStr;						//0x10A
	uint16_t wReqDex;						//0x10C
	uint8_t nAbsorb;						//0x10E
	uint8_t nInvWidth;						//0x10F
	uint8_t nInvHeight;					//0x110
	uint8_t nBlock;						//0x111
	uint8_t nDurability;					//0x112
	uint8_t nNoDurability;					//0x113
	uint8_t nMissile;						//0x114
	uint8_t nComponent;					//0x115
	uint8_t nArmorComp[6];					//0x116
	uint8_t n2Handed;						//0x11C
	uint8_t nUseable;						//0x11D
	uint16_t wType[2];						//0x11E
	uint8_t nSubType;						//0x122
	uint8_t unk0x123;						//0x123
	uint16_t wDropSound;					//0x124
	uint16_t wUseSound;						//0x126
	uint8_t nDropSfxFrame;					//0x128
	uint8_t nUnique;						//0x129
	uint8_t nQuest;						//0x12A
	uint8_t nQuestDiffCheck;				//0x12B
	uint8_t nTransparent;					//0x12C
	uint8_t nTransTbl;						//0x12D
	uint8_t pad0x12E;						//0x12E
	uint8_t nLightRadius;					//0x12F
	uint8_t nBelt;							//0x130
	uint8_t nAutoBelt;						//0x131
	uint8_t nStackable;					//0x132
	uint8_t nSpawnable;					//0x133
	uint8_t nSpellIcon;					//0x134
	uint8_t nDurWarning;					//0x135
	uint8_t nQuantityWarning;				//0x136
	uint8_t nHasInv;						//0x137
	uint8_t nGemSockets;					//0x138
	uint8_t nTransmogrify;					//0x139
	uint8_t nTmogMin;						//0x13A
	uint8_t nTmogMax;						//0x13B
	uint8_t nHitClass;						//0x13C
	uint8_t n1or2Handed;					//0x13D
	uint8_t nGemApplyType;					//0x13E
	uint8_t nLevelReq;						//0x13F
	uint8_t nMagicLevel;					//0x140
	char nTransform;					//0x141
	char nInvTrans;						//0x142
	uint8_t nCompactSave;					//0x143
	uint8_t nSkipName;						//0x144
	uint8_t nNameable;						//0x145
	uint8_t nVendorMin[17];				//0x146
	uint8_t nVendorMax[17];				//0x157
	uint8_t nVendorMagicMin[17];			//0x168
	uint8_t nVendorMagicMax[17];			//0x179
	uint8_t nVendorMagicLvl[17];			//0x18A
	uint8_t pad0x19B;						//0x19B
	uint32_t dwNightmareUpgrade;			//0x19C
	uint32_t dwHellUpgrade;				//0x1A0
	uint8_t nPermStoreItem;				//0x1A4
	uint8_t nMultibuy;						//0x1A5
	uint16_t pad0x1A6;						//0x1A6
};

struct D2ItemTypesTxt
{
	char szCode[4];						//0x00
	short nEquiv1;						//0x04
	short nEquiv2;						//0x06
	uint8_t nRepair;						//0x08
	uint8_t nBody;							//0x09
	uint8_t nBodyLoc1;						//0x0A
	uint8_t nBodyLoc2;						//0x0B
	uint16_t wShoots;						//0x0C
	uint16_t wQuiver;						//0x0E
	uint8_t nThrowable;					//0x10
	uint8_t nReload;						//0x11
	uint8_t nReEquip;						//0x12
	uint8_t nAutoStack;					//0x13
	uint8_t nMagic;						//0x14
	uint8_t nRare;							//0x15
	uint8_t nNormal;						//0x16
	uint8_t nCharm;						//0x17
	uint8_t nGem;							//0x18
	uint8_t nBeltable;						//0x19
	uint8_t nMaxSock1;						//0x1A
	uint8_t nMaxSock25;					//0x1B
	uint8_t nMaxSock40;					//0x1C
	uint8_t nTreasureClass;				//0x1D
	uint8_t nRarity;						//0x1E
	uint8_t nStaffMods;					//0x1F
	uint8_t nCostFormula;					//0x20
	uint8_t nClass;						//0x21
	uint8_t nStorePage;					//0x22
	uint8_t nVarInvGfx;					//0x23
	char szInvGfx[6][32];				//0x24
};

struct D2LevelDefBin
{
	uint32_t dwQuestFlag;					//0x00
	uint32_t dwQuestFlagEx;				//0x04
	uint32_t dwLayer;						//0x08
	uint32_t dwSizeX[3];					//0x0C
	uint32_t dwSizeY[3];					//0x18
	uint32_t dwOffsetX;					//0x24
	uint32_t dwOffsetY;					//0x28
	uint32_t dwDepend;						//0x2C
	uint32_t dwDrlgType;					//0x30
	uint32_t dwLevelType;					//0x34
	uint32_t dwSubType;					//0x38
	uint32_t dwSubTheme;					//0x3C
	uint32_t dwSubWaypoint;				//0x40
	uint32_t dwSubShrine;					//0x44
	int dwVis[8];						//0x48
	uint32_t dwWarp[8];					//0x68
	uint8_t nIntensity;					//0x88
	uint8_t nRed;							//0x89
	uint8_t nGreen;						//0x8A
	uint8_t nBlue;							//0x8B
	uint32_t dwPortal;						//0x8C
	uint32_t dwPosition;					//0x90
	uint32_t dwSaveMonsters;				//0x94
	uint32_t dwLOSDraw;					//0x98
};

struct D2LevelsTxt
{
	uint16_t wLevelNo;						//0x00
	uint8_t nPal;							//0x02
	uint8_t nAct;							//0x03
	uint8_t nTeleport;						//0x04
	uint8_t nRain;							//0x05
	uint8_t nMud;							//0x06
	uint8_t nNoPer;						//0x07
	uint8_t nIsInside;						//0x08
	uint8_t nDrawEdges;					//0x09
	uint16_t unk0x0A;						//0x0A
	uint32_t dwWarpDist;					//0x0C
	uint16_t wMonLvl[3];					//0x10
	uint16_t wMonLvlEx[3];					//0x16
	uint32_t dwMonDen[3];					//0x1C
	uint8_t nMonUMin[3];					//0x28
	uint8_t nMonUMax[3];					//0x2B
	uint8_t nMonWndr;						//0x2E
	uint8_t nMonSpcWalk;					//0x2F
	uint8_t nQuest;						//0x30
	uint8_t nRangedSpawn;					//0x31
	uint8_t nNumMon;						//0x32
	uint8_t nNumNormMon;					//0x33
	uint8_t nNumNMon;						//0x34
	uint8_t nNumUMon;						//0x35
	short wMon[25];						//0x36
	short wNMon[25];					//0x68
	short wUMon[25];					//0x9A
	uint16_t wCMon[4];						//0xCC
	uint16_t wCPct[4];						//0xD4
	uint16_t wCAmt[4];						//0xDC
	uint8_t nWaypoint;						//0xE4
	uint8_t nObjGroup[8];					//0xE5
	uint8_t nObjPrb[8];					//0xED
	char szLevelName[40];				//0xF5
	char szLevelWarp[40];				//0x11D
	char szEntryFile[40];				//0x145
	uint8_t pad0x16D;						//0x16D
	wchar_t wszLevelName[40];			//0x16E
	wchar_t wszLevelWarp[40];			//0x1BE
	uint16_t pad0x20E;						//0x20E
	uint32_t dwThemes;						//0x210
	uint32_t dwFloorFilter;				//0x214
	uint32_t dwBlankScreen;				//0x218
	uint32_t dwSoundEnv;					//0x21C
};

struct D2LowQualityItemsTxt
{
	char szName[32];					//0x00
	uint16_t wTblId;						//0x20
};

struct D2LvlMazeTxt
{
	uint32_t dwLevelId;					//0x00
	uint32_t dwRooms[3];					//0x04
	uint32_t dwSizeX;						//0x10
	uint32_t dwSizeY;						//0x14
	uint32_t dwMerge;						//0x18
};

struct D2LvlPrestTxt
{
	uint32_t dwDef;						//0x00
	uint32_t dwLevelId;					//0x04
	uint32_t dwPopulate;					//0x08
	uint32_t dwLogicals;					//0x0C
	uint32_t dwOutdoors;					//0x10
	uint32_t dwAnimate;					//0x14
	uint32_t dwKillEdge;					//0x18
	uint32_t dwFillBlanks;					//0x1C
	uint32_t dwExpansion;					//0x20
	uint32_t nAnimSpeed;					//0x24
	int nSizeX;							//0x28
	int nSizeY;							//0x2C
	uint32_t dwAutoMap;					//0x30
	uint32_t dwScan;						//0x34
	uint32_t dwPops;						//0x38
	uint32_t dwPopPad;						//0x3C
	uint32_t dwFiles;						//0x40
	char szFile[6][60];					//0x44
	//char szFile1[60];					//0x44
	//char szFile2[60];					//0x80
	//char szFile3[60];					//0xBC
	//char szFile4[60];					//0xF8
	//char szFile5[60];					//0x134
	//char szFile6[60];					//0x170
	uint32_t dwDt1Mask;					//0x1AC
};

struct D2LvlSubTxt
{
	uint32_t dwType;						//0x00
	char szFile[60];					//0x04
	uint32_t dwCheckAll;					//0x40
	uint32_t dwBordType;					//0x44
	uint32_t dwDt1Mask;					//0x48
	uint32_t dwGridSize;					//0x4C
	D2DrlgFileStrc* pDrlgFile;			//0x50
	D2DrlgGridStrc pOrientationGrid[4];	//0x54
	D2DrlgGridStrc pWallGrid[4];		//0xA4
	D2DrlgGridStrc pFloorGrid;			//0xF4
	D2DrlgGridStrc pShadowGrid;			//0x108
	unsigned int nProb[5];				//0x11C
	int nTrials[5];						//0x130
	int nMax[5];						//0x144
	uint32_t dwExpansion;					//0x158
};

struct D2LvlTypesTxt
{
	char szFile[32][60];				//0x00
	uint8_t nAct;							//0x780
	uint8_t pad0x781[3];					//0x781
	uint32_t dwExpansion;					//0x784
};

struct D2LvlWarpTxt
{
	uint32_t dwLevelId;					//0x00
	uint32_t dwSelectX;					//0x04
	uint32_t dwSelectY;					//0x08
	uint32_t dwSelectDX;					//0x0C
	uint32_t dwSelectDY;					//0x10
	uint32_t dwExitWalkX;					//0x14
	uint32_t dwExitWalkY;					//0x18
	uint32_t dwOffsetX;					//0x1C
	uint32_t dwOffsetY;					//0x20
	uint32_t dwLitVersion;					//0x24
	uint32_t dwTiles;						//0x28
	char szDirection[4];				//0x30
};

struct D2MagicAffixTxt
{
	char szName[32];					//0x00
	uint16_t wTblIndex;						//0x20
	uint16_t wVersion;						//0x22
	D2PropertyStrc pProperties[3];		//0x24
	uint16_t wSpawnable;					//0x54
	char nTransformColor;				//0x56
	uint8_t unk0x57;						//0x57
	uint32_t dwLevel;						//0x58
	uint32_t dwGroup;						//0x5C
	uint32_t dwMaxLevel;					//0x60
	uint8_t nRare;							//0x64
	uint8_t nLevelReq;						//0x65
	uint8_t nClassSpecific;				//0x66
	uint8_t nClass;						//0x67
	uint8_t nClassLevelReq;				//0x68
	uint8_t pad0x69;						//0x69
	uint16_t wIType[7];						//0x6A
	uint16_t wEType[5];						//0x78
	uint16_t wFrequency;					//0x82
	uint32_t dwDivide;						//0x84
	uint32_t dwMultiply;					//0x88
	uint32_t dwAdd;						//0x8C
};

struct D2MissCalcTxt
{
	uint32_t dwCode;						//0x00
};

struct D2MissilesTxt
{
	uint16_t wId;							//0x00
	uint16_t pad0x02;						//0x02
	uint32_t dwMissileFlags;				//0x04
	uint16_t wCltDoFunc;					//0x08
	uint16_t wCltHitFunc;					//0x0A
	uint16_t wSrvDoFunc;					//0x0C
	uint16_t wSrvHitFunc;					//0x0E
	uint16_t wSrvDmgFunc;					//0x10
	uint16_t wTravelSound;					//0x12
	uint16_t wHitSound;						//0x14
	uint16_t wExplosionMissile;				//0x16
	uint16_t wSubMissile[3];				//0x18
	uint16_t wCltSubMissile[3];				//0x1E
	uint16_t wHitSubMissile[4];				//0x24
	uint16_t wCltHitSubMissile[4];			//0x2C
	uint16_t wProgSound;					//0x34
	uint16_t wProgOverlay;					//0x36
	uint32_t dwParam[5];					//0x38
	uint32_t dwHitPar[3];					//0x4C
	uint32_t dwCltParam[5];				//0x58
	uint32_t dwCltHitPar[3];				//0x6C
	uint32_t dwDmgParam[2];				//0x78
	uint32_t dwSrvCalc;					//0x80
	uint32_t dwCltCalc;					//0x84
	uint32_t dwHitCalc;					//0x88
	uint32_t dwCltHitCalc;					//0x8C
	uint32_t dwDmgCalc;					//0x90
	uint8_t nHitClass;						//0x94
	uint8_t pad0x95;						//0x95
	uint16_t wRange;						//0x96
	uint16_t wLevRange;						//0x98
	uint8_t nVel;							//0x9A
	uint8_t nVelLev;						//0x9B
	uint8_t nMaxVel;						//0x9C
	uint8_t pad0x9D;						//0x9D
	uint16_t wAccel;						//0x9E
	uint16_t wAnimRate;						//0xA0
	uint16_t wXoffset;						//0xA2
	uint16_t wYoffset;						//0xA4
	uint16_t wZoffset;						//0xA6
	uint32_t dwHitFlags;					//0xA8
	uint16_t wResultFlags;					//0xAC
	uint8_t nKnockBack;					//0xAE
	uint8_t pad0xAF;						//0xAF
	uint32_t dwMinDamage;					//0xB0
	uint32_t dwMaxDamage;					//0xB4
	uint32_t dwMinLevDam[5];				//0xB8
	uint32_t dwMaxLevDam[5];				//0xCC
	uint32_t dwDmgSymPerCalc;				//0xE0
	uint8_t nElemType;						//0xE4
	uint8_t pad0xE5[3];					//0xE5
	uint32_t dwElemMin;					//0xE8
	uint32_t dwElemMax;					//0xEC
	uint32_t dwMinElemLev[5];				//0xF0
	uint32_t dwMaxElemLev[5];				//0x104
	uint32_t dwElemDmgSymPerCalc;			//0x118
	uint32_t dwElemLen;					//0x11C
	uint32_t dwElemLevLen[3];				//0x120
	uint8_t nCltSrcTown;					//0x12C
	uint8_t nSrcDamage;					//0x12D
	uint8_t nSrcMissDmg;					//0x12E
	uint8_t nHoly;							//0x12F
	uint8_t nLight;						//0x130
	uint8_t nFlicker;						//0x131
	uint8_t nRGB[3];						//0x132
	uint8_t nInitSteps;					//0x135
	uint8_t nActivate;						//0x136
	uint8_t nLoopAnim;						//0x137
	char szCelFile[64];					//0x138
	uint8_t nAnimLen;						//0x178
	uint8_t pad0x179[3];					//0x179
	uint32_t dwRandStart;					//0x17C
	uint8_t nSubLoop;						//0x180
	uint8_t nSubStart;						//0x181
	uint8_t nSubStop;						//0x182
	uint8_t nCollideType;					//0x183
	uint8_t nCollision;					//0x184
	uint8_t nClientCol;					//0x185
	uint8_t nCollideKill;					//0x186
	uint8_t nCollideFriend;				//0x187
	uint8_t nNextHit;						//0x188
	uint8_t nNextDelay;					//0x189
	uint8_t nSize;							//0x18A
	uint8_t nToHit;						//0x18B
	uint8_t nAlwaysExplode;				//0x18C
	uint8_t nTrans;						//0x18D
	uint8_t nQty;							//0x18E
	uint8_t pad0x18F;						//0x18F
	uint32_t dwSpecialSetup;				//0x190
	short nSkill;						//0x194
	uint8_t nHitShift;						//0x196
	uint8_t unk0x197[5];					//0x197
	uint32_t dwDamageRate;					//0x19C
	uint8_t nNumDirections;				//0x1A0
	uint8_t nAnimSpeed;					//0x1A1
	uint8_t nLocalBlood;					//0x1A2
	uint8_t pad0x1A3;						//0x1A3
};

struct D2MonAiTxt
{
	uint16_t wAi;							//0x00
};

struct D2MonEquipTxt
{
	short nMonster;						//0x00
	uint16_t wLevel;						//0x02
	uint8_t nOnInit;						//0x04
	uint8_t pad0x05[3];					//0x05
	uint32_t dwItem[3];					//0x08
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

struct D2MonModeTxtStub
{
	uint32_t dwCode;						//0x00
};

struct D2MonModeTxt
{
	char szName[32];					//0x00
	uint32_t dwToken;						//0x20
	uint8_t nDTDir;						//0x24
	uint8_t nNUDir;						//0x25
	uint8_t nWLDir;						//0x26
	uint8_t nGHDir;						//0x27
	uint8_t nA1Dir;						//0x28
	uint8_t nA2Dir;						//0x29
	uint8_t nBLDir;						//0x2A
	uint8_t nSCDir;						//0x2B
	uint8_t nS1Dir;						//0x2C
	uint8_t nS2Dir;						//0x2D
	uint8_t nS3Dir;						//0x2E
	uint8_t nS4Dir;						//0x2F
	uint8_t nDDDir;						//0x30
	uint8_t nKBDir;						//0x31
	uint8_t nSQDir;						//0x32
	uint8_t nRNDir;						//0x33
};

struct D2MonPlaceTxt
{
	uint16_t nCode;							//0x00
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
	D2PropertyStrc props[3][6];			//0x04
	uint8_t nChance[3][6];					//0x124
	uint16_t pad0x136;						//0x136
};

struct D2MonSeqTxt
{
	uint16_t wSequence;						//0x00
	uint8_t nMode;							//0x02
	uint8_t nFrame;						//0x03
	uint8_t nDir;							//0x04
	uint8_t nEvent;						//0x05
};

struct D2MonSoundsTxt
{
	uint32_t dwId;							//0x00
	uint32_t dwAttack1;					//0x04
	uint32_t dwAtt1Del;					//0x08
	uint32_t dwAtt1Prb;					//0x0C
	uint32_t dwAttack2;					//0x10
	uint32_t dwAtt2Del;					//0x14
	uint32_t dwAtt2Prb;					//0x18
	uint32_t dwWeapon1;					//0x1C
	uint32_t dwWea1Del;					//0x20
	uint32_t dwWea1Vol;					//0x24
	uint32_t dwWeapon2;					//0x28
	uint32_t dwWea2Del;					//0x2C
	uint32_t dwWea2Vol;					//0x30
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
	uint32_t dwNeutral;					//0x68
	uint32_t dwNeuTime;					//0x6C
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

struct D2MonStatsTxt
{
	short nId;							//0x00
	short nBaseId;						//0x02
	short nNextInClass;					//0x04
	uint16_t wNameStr;						//0x06
	uint16_t wDescStr;						//0x08
	uint16_t unk0x0A;						//0x0A
	uint32_t dwMonStatsFlags;				//0x0C
	uint32_t dwCode;						//0x10
	uint16_t wMonSound;						//0x14
	uint16_t wUMonSound;					//0x16
	uint16_t wMonStatsEx;					//0x18
	uint16_t wMonProp;						//0x1A
	uint16_t wMonType;						//0x1C
	uint16_t wAI;							//0x1E
	uint16_t wSpawn;						//0x20
	uint8_t nSpawnX;						//0x22
	uint8_t nSpawnY;						//0x23
	uint8_t nSpawnMode;					//0x24
	uint8_t unk0x25;						//0x25
	uint16_t wMinion1;						//0x26
	uint16_t wMinion2;						//0x28
	short nMonEquipTxtRecordId;			//0x2A
	uint8_t nPartyMin;						//0x2C
	uint8_t nPartyMax;						//0x2D
	uint8_t nRarity;						//0x2E
	uint8_t nMinGrp;						//0x2F
	uint8_t nMaxGrp;						//0x30
	uint8_t nSparsePopulate;				//0x31
	short nVelocity;					//0x32
	short nRun;							//0x34
	short unk0x36;						//0x36 - Special Velocity (see DATATBLS_LoadMonStatsTxt)
	short unk0x38;						//0x38 - Special RunSpeed (see DATATBLS_LoadMonStatsTxt)
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
	uint8_t nAlign;						//0x4C
	uint8_t nTransLvl;						//0x4D
	uint8_t nThreat;						//0x4E
	uint8_t nAIdel[3];						//0x4F
	uint8_t nAiDist[3];					//0x52
	uint8_t unk0x55;						//0x55
	uint16_t wAiParam[8][3];				//0x56
	uint16_t wTreasureClass[3][4];			//0x86
	uint8_t nTCQuestId;					//0x9E
	uint8_t nTCQuestCP;					//0x9F
	uint8_t nDrain[3];						//0xA0
	uint8_t nToBlock[3];					//0xA3
	uint8_t nCrit;							//0xA6
	uint8_t unk0xA7;						//0xA7
	uint16_t wSkillDamage;					//0xA8
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
	uint8_t nElMode[3];					//0xFE
	uint8_t nElType[3];					//0x101
	uint8_t nElPct[3][3];					//0x104
	uint8_t unk0x10D;						//0x10D
	uint16_t nElMinD[3][3];					//0x10E
	uint16_t nElMaxD[3][3];					//0x120
	uint16_t nElDur[3][3];					//0x132
	uint16_t nResistances[6][3];			//0x144
	uint8_t nColdEffect[3];				//0x168
	uint8_t unk0x16B;						//0x16B
	uint32_t dwSendSkills;					//0x16C
	short nSkill[8];					//0x170
	uint8_t nSkillMode[8];					//0x180
	uint16_t nSequence[8];					//0x188
	uint8_t nSkLvl[8];						//0x198
	uint32_t dwDamageRegen;				//0x1A0
	uint8_t nSplEndDeath;					//0x1A4
	uint8_t nSplGetModeChart;				//0x1A5
	uint8_t nSplEndGeneric;				//0x1A6
	uint8_t nSplClientEnd;					//0x1A7
};

struct D2MonStats2Txt
{
	uint32_t dwId;							//0x00
	uint32_t dwFlags;						//0x04
	uint8_t nSizeX;						//0x08
	uint8_t nSizeY;						//0x09
	uint8_t nSpawnCol;						//0x0A
	uint8_t nHeight;						//0x0B
	uint8_t nOverlayHeight;				//0x0C
	uint8_t nPixHeight;					//0x0D
	uint8_t nMeleeRng;						//0x0E
	uint8_t pad0x0F;						//0x0F
	int nBaseW;							//0x10 - Maybe union?
	//char szBaseW[4];					//0x10
	uint8_t nHitClass;						//0x14
	uint8_t unk0x15[16];					//0x15 - see CompositLinker
	uint8_t unk0x25;						//0x25 - see CompositLinker
	D2UnkMonCompStrc unk0x26[16];		//0x26 - see CompositLinker
	uint8_t pad0xE6[2];					//0xE6
	uint32_t dwComponentFlags;				//0xE8
	uint8_t nTotalPieces;					//0xEC
	uint8_t pad0xED[3];					//0xED
	uint32_t dwModeFlags;					//0xF0
	uint8_t ndDT;							//0xF4
	uint8_t ndNU;							//0xF5
	uint8_t ndWL;							//0xF6
	uint8_t ndGH;							//0xF7
	uint8_t ndA1;							//0xF8
	uint8_t ndA2;							//0xF9
	uint8_t ndBL;							//0xFA
	uint8_t ndSC;							//0xFB
	uint8_t ndS1;							//0xFC
	uint8_t ndS2;							//0xFD
	uint8_t ndS3;							//0xFE
	uint8_t ndS4;							//0xFF
	uint8_t ndDD;							//0x100
	uint8_t ndKB;							//0x101
	uint8_t ndSQ;							//0x102
	uint8_t ndRN;							//0x103
	uint8_t unusedFlags[4];				//0x104
	uint8_t nInfernoLen;					//0x108
	uint8_t nInfernoAnim;					//0x109
	uint8_t nInfernoRollback;				//0x10A
	uint8_t nResurrectMode;				//0x10B
	uint16_t nResurrectSkill;				//0x10C
	uint16_t whtTop;						//0x10E
	uint16_t whtLeft;						//0x110
	uint16_t whtWidth;						//0x112
	uint16_t whtHeight;						//0x114
	uint8_t pad0x116[2];					//0x116
	uint32_t dwAutomapCel;					//0x118
	uint8_t nLocalBlood;					//0x11C
	uint8_t nBleed;						//0x11D
	uint8_t nLight;						//0x11E
	uint8_t nLight_R;						//0x11F
	uint8_t nLight_G;						//0x120
	uint8_t nLight_B;						//0x121
	uint8_t nUtrans[3];					//0x122
	uint8_t pad0x125[3];					//0x125
	char szHeart[4];					//0x128
	char szBodyPart[4];					//0x12C
	uint8_t nRestore;						//0x130
	uint8_t pad0x131[3];					//0x131
};

struct D2MonTypeTxt
{
	uint16_t wType;							//0x00
	short nEquiv[3];					//0x02
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
	uint8_t nFPick;						//0x09
	uint16_t wExclude[2];					//0x0A
	uint16_t wCPick[3];						//0x0E
	uint16_t wUPick[3];						//0x14
	uint16_t pad0x1A;						//0x1A
	uint32_t dwConstants;					//0x1C
};

struct D2NpcTxt
{
	uint32_t dwNpc;						//0x00
	uint32_t dwSellMult;					//0x04
	uint32_t dwBuyMult;					//0x08
	uint32_t dwRepMult;					//0x0C
	uint32_t dwQuestFlag[3];				//0x10
	uint32_t dwQuestSellMult[3];			//0x1C
	uint32_t dwQuestBuyMult[3];			//0x28
	uint32_t dwQuestRepMult[3];			//0x34
	int nMaxBuy[3];						//0x40
};

struct D2ObjectsTxt
{
	char szName[64];					//0x00
	wchar_t wszName[64];				//0x40
	char szToken[3];					//0xC0
	uint8_t nSpawnMax;						//0xC3
	uint8_t nSelectable[8];				//0xC4
	uint8_t nTrapProb;						//0xCC
	uint8_t pad0xCD[3];					//0xCD
	uint32_t dwSizeX;						//0xD0
	uint32_t dwSizeY;						//0xD4
	uint32_t dwFrameCnt[8];				//0xD8
	uint16_t wFrameDelta[8];				//0xF8
	uint8_t nCycleAnim[8];					//0x108
	uint8_t nLit[8];						//0x110
	uint8_t nBlocksLight[8];				//0x118
	uint8_t nHasCollision[8];				//0x120
	uint8_t nIsAttackable0;				//0x128
	uint8_t nStart[8];						//0x129
	uint8_t nOrderFlag[8];					//0x131
	uint8_t nEnvEffect;					//0x139
	uint8_t nIsDoor;						//0x13A
	uint8_t nBlocksVis;					//0x13B
	uint8_t nOrientation;					//0x13C
	uint8_t nPreOperate;					//0x13D
	uint8_t nTrans;						//0x13E
	uint8_t nMode[8];						//0x13F
	uint8_t pad0x147;						//0x147
	uint32_t dwXOffset;					//0x148
	uint32_t dwYOffset;					//0x14C
	uint8_t nDraw;							//0x150
	uint8_t nHD;							//0x151
	uint8_t nTR;							//0x152
	uint8_t nLG;							//0x153
	uint8_t nRA;							//0x154
	uint8_t nLA;							//0x155
	uint8_t nRH;							//0x156
	uint8_t nLH;							//0x157
	uint8_t nSH;							//0x158
	uint8_t nS1;							//0x159
	uint8_t nS2;							//0x15A
	uint8_t nS3;							//0x15B
	uint8_t nS4;							//0x15C
	uint8_t nS5;							//0x15D
	uint8_t nS6;							//0x15E
	uint8_t nS7;							//0x15F
	uint8_t nS8;							//0x160
	uint8_t nTotalPieces;					//0x161
	uint8_t nXSpace;						//0x162
	uint8_t nYSpace;						//0x163
	uint8_t nRed;							//0x164
	uint8_t nGreen;						//0x165
	uint8_t nBlue;							//0x166
	uint8_t nSubClass;						//0x167
	uint32_t dwNameOffset;					//0x168
	uint8_t pad0x16C;						//0x16C
	uint8_t nMonsterOK;					//0x16D
	uint8_t nOperateRange;					//0x16E
	uint8_t nShrineFunction;				//0x16F
	uint8_t nAct;							//0x170
	uint8_t nLockable;						//0x171
	uint8_t nGore;							//0x172
	uint8_t nRestore;						//0x173
	uint8_t nRestoreVirgins;				//0x174
	uint8_t nSync;							//0x175
	uint16_t pad0x176;						//0x177
	uint32_t dwParm[8];					//0x178
	uint8_t nTgtFX;						//0x198
	uint8_t nTgtFY;						//0x199
	uint8_t nTgtBX;						//0x19A
	uint8_t nTgtBY;						//0x19B
	uint8_t nDamage;						//0x19C
	uint8_t nCollisionSubst;				//0x19D
	uint16_t pad0x19E;						//0x19E
	uint32_t dwLeft;						//0x1A0
	uint32_t dwTop;						//0x1A4
	uint32_t dwWidth;						//0x1A8
	uint32_t dwHeight;						//0x1AC
	uint8_t nBeta;							//0x1B0
	uint8_t nInitFn;						//0x1B1
	uint8_t nPopulateFn;					//0x1B2
	uint8_t nOperateFn;					//0x1B3
	uint8_t nClientFn;						//0x1B4
	uint8_t nOverlay;						//0x1B5
	uint8_t nBlockMissile;					//0x1B6
	uint8_t nDrawUnder;					//0x1B7
	uint8_t nOpenWarp;						//0x1B8
	uint8_t pad0x1B9[3];					//0x1B9
	uint32_t dwAutomap;					//0x1BC
};

struct D2ObjGroupTxt
{
	uint32_t dwId[8];						//0x00
	uint8_t nDensity[8];					//0x20
	uint8_t nProbability[8];				//0x28
	uint8_t nShrines;						//0x30
	uint8_t nWells;						//0x31
	uint16_t pad0x32;						//0x32
};

struct D2ObjModeTypeTxt
{
	char szName[32];					//0x00
	char szToken[20];					//0x20
};

struct D2OverlayTxt
{
	uint16_t wOverlay;						//0x00
	char szFilename[64];				//0x02
	uint16_t wVersion;						//0x42
	uint32_t dwFrames;						//0x44
	uint8_t nPreDraw;						//0x48
	uint8_t pad0x49[3];					//0x49
	uint32_t dw1ofN;						//0x4C
	uint8_t nDir;							//0x50
	uint8_t nOpen;							//0x51
	uint8_t nBeta;							//0x52
	uint8_t pad0x53;						//0x53
	uint32_t dwOffsetX;					//0x54
	uint32_t dwOffsetY;					//0x58
	uint32_t dwHeight[4];					//0x5C
	uint32_t dwAnimRate;					//0x6C
	uint32_t dwInitRadius;					//0x70
	uint32_t dwRadius;						//0x74
	uint32_t dwLoopWaitTime;				//0x78
	uint8_t nTrans;						//0x7C
	uint8_t nRed;							//0x7D
	uint8_t nGreen;						//0x7E
	uint8_t nBlue;							//0x7F
	uint8_t nNumDirections;				//0x80
	uint8_t nLocalBlood;					//0x81
	uint16_t pad0x82;						//0x82
};

struct D2PetTypeTxt
{
	uint32_t dwHcIdx;						//0x00
	uint32_t dwPetFlags;					//0x04
	uint16_t wGroup;						//0x08
	uint16_t wBaseMax;						//0x0A
	uint16_t wStringId;						//0x0C
	uint8_t nIconType;						//0x0E
	char szBaseicon[32];				//0x0F
	char szIcon[4][32];					//0x2F
	uint8_t pad0xAF[3];					//0xAF
	uint16_t wClass[4];						//0xB2
	uint8_t unk0xBA[2];					//0xBA
	int nSkillCount;					//0xBC
	uint16_t wSkillIds[15];					//0xC0
	uint16_t unk0xDE;						//0xDE
};

struct D2PlayerClassTxt
{
	uint32_t dwCode;						//0x00
};

struct D2PlrModeTxtStub
{
	uint32_t dwCode;						//0x00
};

struct D2PlrModeTypeTxt
{
	char szName[32];					//0x00
	char szToken[20];					//0x20
};

struct D2PropertiesTxt
{
	uint16_t wProp;							//0x00
	uint8_t nSet[7];						//0x02
	uint8_t pad0x09;						//0x09
	uint16_t wVal[7];						//0x0A
	uint8_t nFunc[7];						//0x18
	uint8_t pad0x1F;						//0x1F
	uint16_t wStat[7];						//0x20
};

struct D2QualityItemsTxt
{
	uint8_t nArmor;						//0x00
	uint8_t nWeapon;						//0x01
	uint8_t nShield;						//0x02
	uint8_t nScepter;						//0x03
	uint8_t nWand;							//0x04
	uint8_t nStaff;						//0x05
	uint8_t nBow;							//0x06
	uint8_t nBoots;						//0x07
	uint8_t nGloves;						//0x08
	uint8_t nBelt;							//0x09
	uint8_t nNumMods;						//0x0A
	uint8_t pad0x0B;						//0x0B
	D2PropertyStrc pProperties[2];		//0x0C
	char szEffect1[32];					//0x2C
	char szEffect2[32];					//0x4C
	uint16_t wEffect1TblId;					//0x6C
	uint16_t wEffect2TblId;					//0x6E
};

struct D2RareAffixTxt
{
	uint32_t unk0x00[3];					//0x00
	uint16_t wStringId;						//0x0C
	uint16_t wVersion;						//0x0E
	uint16_t wIType[7];						//0x10
	uint16_t wEType[4];						//0x1E
	char szName[32];					//0x26
	uint16_t pad0x46;						//0x46
};

struct D2RunesTxt
{
	char szName[64];						//0x00
	char szRuneName[64];					//0x40
	uint8_t nComplete;							//0x80
	uint8_t nServer;							//0x81
	uint16_t wStringId;							//0x82
	uint16_t pad0x84;							//0x84
	uint16_t wIType[6];							//0x86
	uint16_t wEType[3];							//0x92
	uint32_t dwRune[6];						//0x98
	D2PropertyStrc pProperties[7];			//0xB0
};

struct D2SetItemsTxt
{
	uint16_t wSetItemId;						//0x00
	char szName[32];						//0x02
	uint16_t wVersion;							//0x22
	uint16_t wStringId;							//0x24
	uint16_t pad0x26;							//0x26
	uint32_t szItemCode;						//0x28
	short nSetId;							//0x2C
	short nSetItems;						//0x2E
	uint16_t wLvl;								//0x30
	uint16_t wLvlReq;							//0x32
	uint32_t dwRarity;							//0x34
	uint32_t dwCostMult;						//0x38
	uint32_t dwCostAdd;						//0x3C
	char nChrTransform;						//0x40
	char nInvTransform;						//0x41
	char szFlippyFile[32];					//0x42
	char szInvFile[32];						//0x62
	uint16_t wDropSound;						//0x82
	uint16_t wUseSound;							//0x84
	uint8_t nDropSfxFrame;						//0x86
	uint8_t nAddFunc;							//0x87
	D2PropertyStrc pProperties[9];			//0x88
	D2PropertyStrc pPartialBoni[10];		//0x118
};

struct D2SetsTxt
{
	uint16_t wSetId;						//0x00
	uint16_t wStringId;						//0x02
	uint16_t wVersion;						//0x04
	uint16_t pad0x06;						//0x06
	uint32_t unk0x08;						//0x08
	int nSetItems;						//0x0C
	D2PropertyStrc pBoni2[2];			//0x10
	D2PropertyStrc pBoni3[2];			//0x30
	D2PropertyStrc pBoni4[2];			//0x50
	D2PropertyStrc pBoni5[2];			//0x70
	D2PropertyStrc pFBoni[8];			//0x90
	D2SetItemsTxt* pSetItem[6];			//0x110
};

struct D2ShrinesTxt
{
	uint8_t nCode;							//0x00
	uint8_t pad0x01[3];					//0x01
	uint32_t dwArg0;						//0x04
	uint32_t dwArg1;						//0x08
	uint32_t dwDurationInFrames;			//0x0C
	uint8_t nResetTimeInMins;				//0x10
	uint8_t nRarity;						//0x11
	char szViewname[32];				//0x12
	char szNiftyphrase[128];			//0x32
	uint8_t nEffectClass;					//0xB2
	uint8_t pad0xB3;						//0xB3
	uint32_t dwLevelMin;					//0xB4
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
	uint8_t nRange;						//0x14
	uint8_t nSelectProc;					//0x15
	uint8_t nSeqInput;						//0x16
	uint8_t pad0x17;						//0x17
	int16_t nITypeA[3];						//0x18
	int16_t nITypeB[3];						//0x1E
	int16_t nETypeA[2];						//0x24
	int16_t nETypeB[2];						//0x28
	uint16_t wSrvStartFunc;					//0x2C
	uint16_t wSrvDoFunc;					//0x2E
	uint16_t wSrvPrgFunc[3];				//0x30
	uint16_t pad0x36;						//0x36
	uint32_t dwPrgCalc[3];					//0x38
	uint8_t nPrgDamage;					//0x44
	uint8_t pad0x45;						//0x45
	uint16_t wSrvMissile;					//0x46
	uint16_t wSrvMissileA;					//0x48
	uint16_t wSrvMissileB;					//0x4A
	uint16_t wSrvMissileC;					//0x4C
	uint16_t wSrvOverlay;					//0x4E
	uint32_t dwAuraFilter;					//0x50
	uint16_t wAuraStat[6];					//0x54
	uint32_t dwAuraLenCalc;				//0x60
	uint32_t dwAuraRangeCalc;				//0x64
	int32_t dwAuraStatCalc[6];				//0x68
	int16_t nAuraState;						//0x80
	int16_t wAuraTargetState;				//0x82
	uint16_t wAuraEvent[3];					//0x84
	uint16_t wAuraEventFunc[3];				//0x8A
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
	uint8_t nPetType;						//0xBE
	uint8_t nSumMode;						//0xBF
	uint32_t dwPetMax;						//0xC0
	uint16_t wSumSkill[5];					//0xC4
	uint16_t pad0xCE;						//0xCE
	uint32_t dwSumSkCalc[5];				//0xD0
	uint16_t wSumUMod;						//0xE4
	uint16_t wSumOverlay;					//0xE6
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
	uint8_t nItemTarget;					//0x120
	uint8_t pad0x121;						//0x121
	uint16_t wItemCastSound;				//0x122
	uint16_t wItemCastOverlay;				//0x124
	uint16_t pad0x126;						//0x126
	uint32_t dwPerDelay;					//0x128
	uint16_t wMaxLvl;						//0x12C
	uint16_t wResultFlags;					//0x12E
	uint32_t dwHitFlags;					//0x130
	uint32_t dwHitClass;					//0x134
	uint32_t dwCalc[4];					//0x138
	int32_t dwParam[8];						//0x148
	uint8_t nWeapSel;						//0x168
	uint8_t pad0x169;						//0x169
	uint16_t wItemEffect;					//0x16A
	uint16_t wItemCltEffect;				//0x16C
	uint16_t pad0x16E;					//0x16E
	uint32_t dwSkPoints;					//0x170
	uint16_t wReqLevel;						//0x174
	uint16_t wReqStr;						//0x176
	uint16_t wReqDex;						//0x178
	uint16_t wReqInt;						//0x17A
	uint16_t wReqVit;						//0x17C
	int16_t nReqSkill[3];					//0x17E
	uint16_t wStartMana;					//0x184
	uint16_t wMinMana;						//0x186
	uint8_t nManaShift;					//0x188
	uint8_t pad0x189;						//0x189
	uint16_t wMana;							//0x18A
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
	uint8_t nEType;						//0x1DC
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

struct D2StatesTxt
{
	uint16_t wState;						//0x00
	uint16_t wOverlay[4];					//0x02
	uint16_t wCastOverlay;					//0x0A
	uint16_t wRemoveOverlay;				//0x0C
	uint16_t wPrgOverlay;					//0x0E
	union
	{
		uint8_t nStateFlags[4];			//0x10
		uint32_t dwStateFlags;				//0x10
	};
	uint32_t dwStateFlagsEx;				//0x14
	uint16_t wStat;							//0x18
	uint16_t wSetFunc;						//0x1A
	uint16_t wRemFunc;						//0x1C
	uint16_t wGroup;						//0x1E
	uint8_t nColorPri;						//0x20
	uint8_t nColorShift;					//0x21
	uint8_t nLightRGB[3];					//0x22
	uint8_t pad0x25;						//0x25
	uint16_t wOnSound;						//0x26
	uint16_t wOffSound;						//0x28
	uint16_t wItemType;						//0x2A
	char nItemTrans;					//0x2C
	uint8_t nGfxType;						//0x2D
	uint16_t wGfxClass;						//0x2E
	uint16_t wCltEvent;						//0x30
	uint16_t wCltEventFunc;					//0x32
	uint16_t wCltActiveFunc;				//0x34
	uint16_t wSrvActiveFunc;				//0x36
	uint16_t wSkill;						//0x38
	uint16_t wMissile;						//0x3A
};

struct D2SoundsTxtStub
{
	uint16_t wSound;						//0x00
};

struct D2StorePageTxt
{
	uint32_t dwCode;						//0x00
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
	uint8_t nUtrans[3];					//0x28
	uint8_t pad0x2B;						//0x2B
	uint16_t dwTC[3];						//0x2C
	uint16_t pad0x32;						//0x32
};

struct D2TreasureClassExTxt
{
	char szTreasureClass[32];			//0x00
	int nPicks;							//0x20
	short nGroup;						//0x24
	short nLevel;						//0x26
	short nMagic;						//0x28
	short nRare;						//0x2A
	short nSet;							//0x2C
	short nUnique;						//0x2E
	short nSuperior;					//0x30
	short nNormal;						//0x32
	int nNoDrop;						//0x34
	char szItem[10][64];				//0x38
	int nProb[10];						//0x2B8
};

struct D2UniqueAppellationTxt
{
	uint16_t wStringId;						//0x00
};

struct D2UniqueItemsTxt
{
	uint16_t wId;							//0x00
	char szName[32];					//0x02
	uint16_t wTblIndex;						//0x22
	uint16_t wVersion;						//0x24
	uint16_t pad0x26;						//0x26
	uint32_t dwBaseItemCode;				//0x28
	uint32_t dwUniqueItemFlags;			//0x2C
	uint16_t wRarity;						//0x30
	uint16_t pad0x32;						//0x32
	uint16_t wLvl;							//0x34
	uint16_t wLvlReq;						//0x36
	char nChrTransform;					//0x38
	char nInvTransform;					//0x39
	char szFlippyFile[32];				//0x3A
	char szInvFile[32];					//0x5A
	uint16_t pad0x7A;						//0x7A
	uint32_t dwCostMult;					//0x7C
	uint32_t dwCostAdd;					//0x80
	uint16_t wDropSound;					//0x84
	uint16_t wUseSound;						//0x86
	uint8_t nDropSfxFrame;					//0x88
	uint8_t pad0x89[3];					//0x89
	D2PropertyStrc pProperties[12];		//0x8C
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

struct D2WeaponClassTxt
{
	uint32_t dwCode;						//0x00
};


// sgptDataTable and inlined structs

struct D2ItemDataTbl								//sgptDataTable + 0xCD8
{
	int nItemsTxtRecordCount;						//0x00
	D2ItemsTxt* pItemsTxt;							//0x04
	D2ItemsTxt* pWeapons;							//0x08
	int nWeaponsTxtRecordCount;						//0x0C
	D2ItemsTxt* pArmor;								//0x10
	int nArmorTxtRecordCount;						//0x14
	D2ItemsTxt* pMisc;								//0x18
	int nMiscTxtRecordCount;						//0x1C
};

struct D2GemDataTbl									//sgptDataTable + 0xCFC
{
	int nGemsTxtRecordCount;						//0x00
	D2GemsTxt* pGemsTxt;							//0x04
};

struct D2LowQualityItemDataTbl						//sgptDataTable + 0xD04
{
	int nLowQualityItemsTxtRecordCount;				//0x00
	D2LowQualityItemsTxt* pLowQualityItemsTxt;		//0x04
};

struct D2BookDataTbl								//sgptDataTable + 0xD0C
{
	int nBooksTxtRecordCount;						//0x00
	D2BooksTxt* pBooksTxt;							//0x04
};

struct D2RareAffixDataTbl							//sgptDataTable + 0xD14
{
	int nRareAffixTxtRecordCount;					//0x00
	D2RareAffixTxt* pRareAffixTxt;					//0x04
	D2RareAffixTxt* pRareSuffix;					//0x08
	D2RareAffixTxt* pRarePrefix;					//0x0C
};

struct D2ItemRatioDataTbl							//sgptDataTable + 0xD24
{
	int nItemRatioTxtRecordCount;					//0x00
	D2ItemRatioTxt* pItemRatioTxt;					//0x04
};

struct D2GambleDataTbl								//sgptDataTable + 0xD30
{
	int nGambleTxtRecordCount;						//0x00
	uint32_t* pGambleSelection;						//0x04
	int pGambleChooseLimit[100];					//0x08
};

struct D2MagicAffixDataTbl							//sgptDataTable + 0xEC8
{
	int nMagicAffixTxtRecordCount;					//0x00
	D2MagicAffixTxt* pMagicAffixTxt;				//0x04
	D2MagicAffixTxt* pMagicSuffix;					//0x08
	D2MagicAffixTxt* pMagicPrefix;					//0x0C
	D2MagicAffixTxt* pAutoMagic;					//0x10
};

struct D2RuneDataTbl								//sgptDataTable + 0xEDC
{
	int nRunesTxtRecordCount;						//0x00
	D2RunesTxt* pRunesTxt;							//0x04
};

struct D2QualityItemDataTbl							//sgptDataTable + 0xEE4
{
	int nQualityItemsTxtRecordCount;				//0x00
	D2QualityItemsTxt* pQualityItemsTxt;			//0x04
};

struct D2MonItemPercentDataTbl						//sgptDataTable + 0x105C
{
	int nMonItemPercentTxtRecordCount;				//0x00
	D2MonItemPercentTxt* pMonItemPercentTxt;		//0x04
};

struct D2MonModeDataTbl								//sgptDataTable + 0x10B0
{
	int nMonModeTxtRecordCount;						//0x00
	D2MonModeTxt* pMonModeTxt;						//0x04
	D2MonModeTxt* pMonMode[2];						//0x08
};

struct D2ObjModeDataTbl								//sgptDataTable + 0x10C4
{
	int nObjModeTypeTxtRecordCount;					//0x00
	D2ObjModeTypeTxt* pObjModeTypeTxt;				//0x04
	D2ObjModeTypeTxt* pObjType;						//0x08
	D2ObjModeTypeTxt* pObjMode;						//0x0C
};

struct D2PlrModeDataTbl								//sgptDataTable + 0x10D4
{
	int nPlrModeTypeTxtRecordCount;					//0x00
	D2PlrModeTypeTxt* pPlrModeTypeTxt;				//0x04
	D2PlrModeTypeTxt* pPlayerType;					//0x08
	D2PlrModeTypeTxt* pPlayerMode;					//0x0C
};

//TODO: see comments
struct D2DataTablesStrc
{
	D2PlayerClassTxt* pPlayerClassTxt;				//0x00
	D2TxtLinkStrc* pPlayerClassLinker;				//0x04
	D2BodyLocsTxt* pBodyLocsTxt;					//0x08
	D2TxtLinkStrc* pBodyLocsLinker;					//0x0C
	D2StorePageTxt* pStorePageTxt;					//0x10
	D2TxtLinkStrc* pStorePageLinker;				//0x14
	D2ElemTypesTxt* pElemTypesTxt;					//0x18
	D2TxtLinkStrc* pElemTypesLinker;				//0x1C
	D2HitClassTxt* pHitClassTxt;					//0x20
	D2TxtLinkStrc* pHitClassLinker;					//0x24
	D2MonModeTxtStub* pMonModeTxtStub;				//0x28
	D2TxtLinkStrc* pMonModeLinker;					//0x2C
	D2PlrModeTxtStub* pPlrModeTxtStub;				//0x30
	D2TxtLinkStrc* pPlrModeLinker;					//0x34
	D2SkillCalcTxt* pSkillCalcTxt;					//0x38
	D2TxtLinkStrc* pSkillCalcLinker;				//0x3C
	char* pSkillsCode;								//0x40
	unsigned int nSkillsCodeSize;					//0x44
	unsigned int nSkillsCodeSizeEx;					//0x48
	char* pSkillDescCode;							//0x4C
	unsigned int nSkillDescCodeSize;				//0x50
	unsigned int nSkillDescCodeSizeEx;				//0x54
	D2MissCalcTxt* pMissileCalcTxt;					//0x58
	D2TxtLinkStrc* pMissileCalcLinker;				//0x5C
	char* pMissCode;								//0x60
	unsigned int nMissCodeSize;						//0x64
	int nMissCodeSizeEx;							//0x68

	/**/	const char* pSkillCode;                             //+0000006C   052C445C      skillcode.txt (Id from skills.txt)
	/**/	D2TxtLinkStrc* iSkillCode;                       //+00000070   0141F084      skillcode info

	D2EventsTxt* pEventsTxt;						//0x74
	D2TxtLinkStrc* pEventsLinker;					//0x78
	D2CompCodeTxt* pCompCodeTxt;					//0x7C
	D2TxtLinkStrc* pCompCodeLinker;					//0x80
	int nCompCodeTxtRecordCount;					//0x84
	D2MonAiTxt* pMonAiTxt;							//0x88
	D2TxtLinkStrc* pMonAiLinker;					//0x8C
	int nMonAiTxtRecordCount;						//0x90
	D2TxtLinkStrc* pItemsLinker;					//0x94
	char* pItemsCode;								//0x98
	unsigned int nItemsCodeSize;					//0x9C
	int nItemsCodeSizeEx;							//0xA0
	D2PropertiesTxt* pPropertiesTxt;				//0xA4
	D2TxtLinkStrc* pPropertiesLinker;				//0xA8
	int nPropertiesTxtRecordCount;					//0xAC
	D2TxtLinkStrc* pRunesLinker;					//0xB0
	D2HireDescTxt* pHireDescTxt;					//0xB4
	D2TxtLinkStrc* pHireDescLinker;					//0xB8
	D2StatesTxt* pStatesTxt;						//0xBC
	D2TxtLinkStrc* pStatesLinker;					//0xC0
	int nStatesTxtRecordCount;						//0xC4
	uint32_t* pStateMasks;								//0xC8
	uint32_t* fStateMasks[40];							//0xCC
	short* pProgressiveStates;						//0x16C
	int nProgressiveStates;							//0x170
	short* pCurseStates;							//0x174
	int nCurseStates;								//0x178
	short* pTransformStates;						//0x17C
	int nTransformStates;							//0x180
	short* pActionStates;							//0x184
	int nActionStates;								//0x188
	short* pColourStates;							//0x18C
	int nColourStates;								//0x190
	D2SoundsTxtStub* pSoundsTxtCodes;				//0x194
	D2TxtLinkStrc* pSoundsLinker;					//0x198
	int nSoundsTxtCodes;							//0x19C
	D2HirelingTxt* pHirelingTxt;					//0x1A0
	int nHirelingTxtRecordCount;					//0x1A4
	int nClassicHirelingStartRecordIds[256];		//0x1A8
	int nExpansionHirelingStartRecordIds[256];		//0x5A8
	D2NpcTxt* pNpcTxt;								//0x9A8
	int nNpcTxtRecordCount;							//0x9AC
	D2ColorsTxt* pColorsTxt;						//0x9B0
	D2TxtLinkStrc* pColorsLinker;					//0x9B4
	D2TxtLinkStrc* pTreasureClassExLinker;			//0x9B8
	D2TCExShortStrc* pTreasureClassEx;				//0x9BC
	int nTreasureClassEx;							//0x9C0
	D2TCExShortStrc* pChestTreasureClasses[45];		//0x9C4
	D2MonStatsTxt* pMonStatsTxt;					//0xA78
	D2TxtLinkStrc* pMonStatsLinker;					//0xA7C
	int nMonStatsTxtRecordCount;					//0xA80
	D2MonSoundsTxt* pMonSoundsTxt;					//0xA84
	D2TxtLinkStrc* pMonSoundsLinker;				//0xA88
	int nMonSoundsTxtRecordCount;					//0xA8C
	D2MonStats2Txt* pMonStats2Txt;					//0xA90
	D2TxtLinkStrc* pMonStats2Linker;				//0xA94
	int nMonStats2TxtRecordCount;					//0xA98
	D2MonPlaceTxt* pMonPlaceTxt;					//0xA9C
	D2TxtLinkStrc* pMonPlaceLinker;					//0xAA0
	int nMonPlaceTxtRecordCount;					//0xAA4
	D2MonPresetTxt* pMonPresetTxt;					//0xAA8
	D2MonPresetTxt* pMonPresetTxtActSections[5];	//0xAAC
	int nMonPresetTxtActRecordCounts[5];			//0xAC0
	D2SuperUniquesTxt* pSuperUniquesTxt;			//0xAD4
	D2TxtLinkStrc* pSuperUniquesLinker;				//0xAD8
	int nSuperUniquesTxtRecordCount;				//0xADC
	short nSuperUniqueIds[66];						//0xAE0
	D2MissilesTxt* pMissilesTxt;					//0xB64
	D2TxtLinkStrc* pMissilesLinker;					//0xB68
	int nMissilesTxtRecordCount;					//0xB6C
	D2MonLvlTxt* pMonLvlTxt;						//0xB70
	int nMonLvlTxtRecordCount;						//0xB74
	D2MonSeqTxt* pMonSeqTxt;						//0xB78
	D2TxtLinkStrc* pMonSeqLinker;					//0xB7C
	int nMonSeqTxtRecordCount;						//0xB80
	D2SeqRecordStrc* pMonSeqTable;					//0xB84
	int nMonSeqTableRecordCount;					//0xB88
	D2SkillDescTxt* pSkillDescTxt;					//0xB8C
	D2TxtLinkStrc* pSkillDescLinker;				//0xB90
	int nSkillDescTxtRecordCount;					//0xB94
	D2SkillsTxt* pSkillsTxt;						//0xB98
	D2TxtLinkStrc* pSkillsLinker;					//0xB9C
	int nSkillsTxtRecordCount;						//0xBA0
	int* nClassSkillCount;							//0xBA4
	int nHighestClassSkillCount;					//0xBA8
	short* nClassSkillList;							//0xBAC
	int nPassiveSkills;								//0xBB0
	uint16_t* pPassiveSkills;							//0xBB4
	D2TxtLinkStrc* pOverlayLinker;					//0xBB8
	D2OverlayTxt* pOverlayTxt;						//0xBBC
	int nOverlayTxtRecordCount;						//0xBC0
	D2CharStatsTxt* pCharStatsTxt;					//0xBC4
	int nCharStatsTxtRecordCount;					//0xBC8
	D2ItemStatCostTxt* pItemStatCostTxt;			//0xBCC
	D2TxtLinkStrc* pItemStatCostLinker;				//0xBD0
	int nItemStatCostTxtRecordCount;				//0xBD4
	uint16_t* pStatsWithDescFunc;						//0xBD8
	int nStatsWithDescFunc;							//0xBDC
	D2MonEquipTxt* pMonEquipTxt;					//0xBE0
	int nMonEquipTxtRecordCount;					//0xBE4
	D2PetTypeTxt* pPetTypeTxt;						//0xBE8
	D2TxtLinkStrc* pPetTypeLinker;					//0xBEC
	int nPetTypeTxtRecordCount;						//0xBF0
	D2TxtLinkStrc* pItemTypesLinker;				//0xBF4
	D2ItemTypesTxt* pItemTypesTxt;					//0xBF8
	int nItemTypesTxtRecordCount;					//0xBFC
	int nItemTypesIndex;							//0xC00
	uint32_t* pItemTypesNest;							//0xC04
	D2TxtLinkStrc* pSetsLinker;						//0xC08
	D2SetsTxt* pSetsTxt;							//0xC0C
	int nSetsTxtRecordCount;						//0xC10
	D2TxtLinkStrc* pSetItemsLinker;					//0xC14
	D2SetItemsTxt* pSetItemsTxt;					//0xC18
	int nSetItemsTxtRecordCount;					//0xC1C
	D2TxtLinkStrc* pUniqueItemsLinker;				//0xC20
	D2UniqueItemsTxt* pUniqueItemsTxt;				//0xC24
	int nUniqueItemsTxtRecordCount;					//0xC28
	D2TxtLinkStrc* pMonPropLinker;					//0xC2C
	D2MonPropTxt* pMonPropTxt;						//0xC30
	int nMonPropTxtRecordCount;						//0xC34
	D2TxtLinkStrc* pMonTypeLinker;					//0xC38
	D2MonTypeTxt* pMonTypeTxt;						//0xC3C
	int nMonTypeTxtRecordCount;						//0xC40
	uint32_t* pMonTypeNest;							//0xC44
	int nMonTypeIndex;								//0xC48
	D2TxtLinkStrc* pMonUModLinker;					//0xC4C
	D2MonUModTxt* pMonUModTxt;						//0xC50
	int nMonUModTxtRecordCount;						//0xC54
	D2LevelsTxt* pLevelsTxt;						//0xC58
	int nLevelsTxtRecordCount;						//0xC5C
	D2LevelDefBin* pLevelDefBin;					//0xC60
	D2LvlPrestTxt* pLvlPrestTxt;					//0xC64
	int nLvlPrestTxtRecordCount;					//0xC68
	int nStuff;										//0xC6C
	int nShiftedStuff;								//0xC70
	D2AnimDataStrc* pAnimData;						//0xC74
	D2ExperienceTxt* pExperienceTxt;				//0xC78
	D2DifficultyLevelsTxt* pDifficultyLevelsTxt;	//0xC7C
	int nDifficultyLevelsTxtRecordCount;			//0xC80
	BOOL bCompileTxt;								//0xC84
	int ExpFieldI[9];								//0xC88
	char* pFieldData;								//0xCAC
	D2CollisionFieldStrc pCollisionField;			//0xCB0
	D2CubeMainTxt* pCubeMainTxt;					//0xCC4
	int nCubeMainTxtRecordCount;					//0xCC8
	int nInventoryTxtRecordCount;					//0xCCC
	D2InventoryTxt* pInventoryTxt;					//0xCD0

	uint32_t unk0xCD4;									//0xCD4

	D2ItemDataTbl pItemDataTables;					//0xCD8
	uint16_t* pIndexOldToCurrent;						//0xCF8	
	D2GemDataTbl pGemDataTables;					//0xCFC
	D2LowQualityItemDataTbl pLowQualityItemDataTables;//0xD04
	D2BookDataTbl pBookDataTables;					//0xD0C
	D2RareAffixDataTbl pRareAffixDataTables;		//0xD14
	D2ItemRatioDataTbl pItemRatioDataTables;		//0xD24

	uint32_t unk0xD2C;									//0xD2C

	D2GambleDataTbl pGambleDataTables;				//0xD30
	D2MagicAffixDataTbl pMagicAffixDataTables;		//0xEC8
	D2RuneDataTbl pRuneDataTables;					//0xEDC
	D2QualityItemDataTbl pQualityItemDataTables;	//0xEE4

	uint32_t unk0xEEC;									//0xEEC

	D2SeedStrc pAutomapSeed;						//0xEF0
	D2AutomapRandStrc pAutoMapRand[36];				//0xEF8
	D2LvlTypesTxt* pLvlTypesTxt;					//0x1018
	int* pPortalLevels;								//0x101C
	int nPortalLevels;								//0x1020
	int nLvlTypesTxtRecordCount;					//0x1024
	D2LvlWarpTxt* pLvlWarpTxt;						//0x1028
	int nLvlWarpTxtRecordCount;						//0x102C
	D2LvlMazeTxt* pLvlMazeTxt;						//0x1030
	int nLvlMazeTxtRecordCount;						//0x1034
	D2LvlSubTxt* pLvlSubTxt;						//0x1038
	int nLvlSubTxtRecordCount;						//0x103C
	int* pLvlSubTypeStartIds;						//0x1040
	D2TileLibraryHashStrc** ppTileLibraryHash;		//0x1044
	D2DrlgFileStrc** ppLvlPrestFiles;				//0x1048
	D2DrlgFileStrc** ppLvlSubTypeFiles;				//0x104C
	D2AutomapShortStrc* pAutoMap;					//0x1050
	int nAutoMap;									//0x1054
	char szDefaultString;							//0x1058
	char pad0x1059[3];								//0x1059
	D2MonItemPercentDataTbl pMonItemPercentDataTables;//0x105C
	wchar_t wszDefault;								//0x1064
	uint16_t unk0x1066;									//0x1066
	D2UniqueTitleTxt* pUniqueTitleTxt;				//0x1068
	D2UniquePrefixTxt* pUniquePrefixTxt;			//0x106C
	D2UniqueSuffixTxt* pUniqueSuffixTxt;			//0x1070
	D2UniqueAppellationTxt* pUniqueAppellationTxt;	//0x1074
	int nUniqueTitleTxtRecordCount;					//0x1078
	int nUniquePrefixTxtRecordCount;				//0x107C
	int nUniqueSuffixTxtRecordCount;				//0x1080
	int nUniqueAppellationTxtRecordCount;			//0x1084

	uint32_t unk7[2];                              //+00001088
	short nTreasureClassItemTypes;					//0x1090
	short unk0x1092;								//0x1092

	D2ShrinesTxt* pShrinesTxt;						//0x1094
	int nShrinesTxtRecordCount;						//0x1098
	D2ObjectsTxt* pObjectsTxt;						//0x109C
	int nObjectsTxtRecordCount;						//0x10A0
	D2ObjGroupTxt* pObjGroupTxt;					//0x10A4
	int nObjGroupTxtRecordCount;					//0x10A8
	D2ArmTypeTxt* pArmTypeTxt;						//0x10AC
	D2MonModeDataTbl pMonModeDataTables;			//0x10B0
	D2CompositTxt* pCompositTxt;					//0x10C0
	D2ObjModeDataTbl pObjModeDataTables;			//0x10C4
	D2PlrModeDataTbl pPlrModeDataTables;			//0x10D4
};

#pragma pack()
