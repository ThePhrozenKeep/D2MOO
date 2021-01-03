#pragma once 

#include <D2BasicTypes.h>

#pragma pack(1)

struct D2PropertyStrc
{
	int32_t nProperty;					//0x00
	int32_t nLayer;						//0x04
	int32_t nMin;						//0x08
	int32_t nMax;						//0x0C
};

struct D2RunesTxt
{
	char szName[64];						//0x00
	char szRuneName[64];					//0x40
	uint8_t nComplete;						//0x80
	uint8_t nServer;						//0x81
	uint16_t wStringId;						//0x82
	uint16_t pad0x84;						//0x84
	uint16_t wIType[6];						//0x86
	uint16_t wEType[3];						//0x92
	uint32_t dwRune[6];						//0x98
	D2PropertyStrc pProperties[7];			//0xB0
};

struct D2QualityItemsTxt
{
	uint8_t nArmor;							//0x00
	uint8_t nWeapon;						//0x01
	uint8_t nShield;						//0x02
	uint8_t nScepter;						//0x03
	uint8_t nWand;							//0x04
	uint8_t nStaff;							//0x05
	uint8_t nBow;							//0x06
	uint8_t nBoots;							//0x07
	uint8_t nGloves;						//0x08
	uint8_t nBelt;							//0x09
	uint8_t nNumMods;						//0x0A
	uint8_t pad0x0B;						//0x0B
	D2PropertyStrc pProperties[2];			//0x0C
	char szEffect1[32];						//0x2C
	char szEffect2[32];						//0x4C
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
	char szName[32];						//0x26
	uint16_t pad0x46;						//0x46
};

struct D2SetItemsTxt
{
	uint16_t wSetItemId;					//0x00
	char szName[32];						//0x02
	uint16_t wVersion;						//0x22
	uint16_t wStringId;						//0x24
	uint16_t pad0x26;						//0x26
	uint32_t szItemCode;					//0x28
	int16_t nSetId;							//0x2C
	int16_t nSetItems;						//0x2E
	uint16_t wLvl;							//0x30
	uint16_t wLvlReq;						//0x32
	uint32_t dwRarity;						//0x34
	uint32_t dwCostMult;					//0x38
	uint32_t dwCostAdd;						//0x3C
	int8_t nChrTransform;					//0x40
	int8_t nInvTransform;					//0x41
	char szFlippyFile[32];					//0x42
	char szInvFile[32];						//0x62
	uint16_t wDropSound;					//0x82
	uint16_t wUseSound;						//0x84
	uint8_t nDropSfxFrame;					//0x86
	uint8_t nAddFunc;						//0x87
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
	int32_t nSetItems;						//0x0C
	D2PropertyStrc pBoni2[2];				//0x10
	D2PropertyStrc pBoni3[2];				//0x30
	D2PropertyStrc pBoni4[2];				//0x50
	D2PropertyStrc pBoni5[2];				//0x70
	D2PropertyStrc pFBoni[8];				//0x90
	D2SetItemsTxt* pSetItem[6];				//0x110
};

enum D2C_UniqueItemsTxtFlagIndices
{
	UNIQUEITEMSFLAGINDEX_ENABLED = 0,
	UNIQUEITEMSFLAGINDEX_NOLIMIT,
	UNIQUEITEMSFLAGINDEX_CARRY1,
	UNIQUEITEMSFLAGINDEX_LADDER,
};

enum D2C_UniqueItemsTxtFlags
{
	UNIQUEITEMSFLAG_ENABLED = (1 << UNIQUEITEMSFLAGINDEX_ENABLED),
	UNIQUEITEMSFLAG_NOLIMIT = (1 << UNIQUEITEMSFLAGINDEX_NOLIMIT),
	UNIQUEITEMSFLAG_CARRY1 = (1 << UNIQUEITEMSFLAGINDEX_CARRY1),
	UNIQUEITEMSFLAG_LADDER = (1 << UNIQUEITEMSFLAGINDEX_LADDER),
};

struct D2UniqueItemsTxt
{
	uint16_t wId;							//0x00
	char szName[32];						//0x02
	uint16_t wTblIndex;						//0x22
	uint16_t wVersion;						//0x24
	uint16_t pad0x26;						//0x26
	uint32_t dwBaseItemCode;				//0x28
	uint32_t dwUniqueItemFlags;				//0x2C
	uint16_t wRarity;						//0x30
	uint16_t pad0x32;						//0x32
	uint16_t wLvl;							//0x34
	uint16_t wLvlReq;						//0x36
	char nChrTransform;						//0x38
	char nInvTransform;						//0x39
	char szFlippyFile[32];					//0x3A
	char szInvFile[32];						//0x5A
	uint16_t pad0x7A;						//0x7A
	uint32_t dwCostMult;					//0x7C
	uint32_t dwCostAdd;						//0x80
	uint16_t wDropSound;					//0x84
	uint16_t wUseSound;						//0x86
	uint8_t nDropSfxFrame;					//0x88
	uint8_t pad0x89[3];						//0x89
	D2PropertyStrc pProperties[12];			//0x8C
};


struct D2ItemStatCostDescStrc
{
	uint16_t nRecordId;						//0x00
	int16_t nDescPriority;					//0x02
};

struct D2ItemRatioTxt
{
	uint32_t dwUnique;						//0x00
	uint32_t dwUniqueDivisor;				//0x04
	uint32_t dwUniqueMin;					//0x08
	uint32_t dwRare;						//0x0C
	uint32_t dwRareDivisor;					//0x10
	uint32_t dwRareMin;						//0x14
	uint32_t dwSet;							//0x18
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
	uint8_t nClassSpecific;					//0x43 
};

struct D2OpStatDataStrc
{
	uint16_t nOpBase;						//0x00
	uint16_t nStat;							//0x02
	uint8_t nOp;							//0x04
	uint8_t nOpParam;						//0x05
};

enum D2C_ItemStatCostTxtFlagIndices
{
	ITEMSTATCOSTFLAGINDEX_SEND_OTHER = 0,
	ITEMSTATCOSTFLAGINDEX_SIGNED,
	ITEMSTATCOSTFLAGINDEX_DAMAGERELATED,
	ITEMSTATCOSTFLAGINDEX_ITEMSPECIFIC,
	ITEMSTATCOSTFLAGINDEX_DIRECT,

	ITEMSTATCOSTFLAGINDEX_HP_MANA_STAMINA = 5,
	ITEMSTATCOSTFLAGINDEX_HP,
	ITEMSTATCOSTFLAGINDEX_MANA,
	ITEMSTATCOSTFLAGINDEX_STAMINA,

	ITEMSTATCOSTFLAGINDEX_UPDATEANIMRATE = 9,
	ITEMSTATCOSTFLAGINDEX_FMIN,
	ITEMSTATCOSTFLAGINDEX_FCALLBACK,
	ITEMSTATCOSTFLAGINDEX_SAVED,
	ITEMSTATCOSTFLAGINDEX_CSVSIGNED,
};

enum D2C_ItemStatCostTxtFlags
{
	ITEMSTATCOSTFLAG_SEND_OTHER = (1 << ITEMSTATCOSTFLAGINDEX_SEND_OTHER),
	ITEMSTATCOSTFLAG_SIGNED = (1 << ITEMSTATCOSTFLAGINDEX_SIGNED),
	ITEMSTATCOSTFLAG_DAMAGERELATED = (1 << ITEMSTATCOSTFLAGINDEX_DAMAGERELATED),
	ITEMSTATCOSTFLAG_ITEMSPECIFIC = (1 << ITEMSTATCOSTFLAGINDEX_ITEMSPECIFIC),
	ITEMSTATCOSTFLAG_DIRECT = (1 << ITEMSTATCOSTFLAGINDEX_DIRECT),

	ITEMSTATCOSTFLAG_HP_MANA_STAMINA = (1 << ITEMSTATCOSTFLAGINDEX_HP_MANA_STAMINA),
	ITEMSTATCOSTFLAG_HP = (1 << ITEMSTATCOSTFLAGINDEX_HP),
	ITEMSTATCOSTFLAG_MANA = (1 << ITEMSTATCOSTFLAGINDEX_MANA),
	ITEMSTATCOSTFLAG_STAMINA = (1 << ITEMSTATCOSTFLAGINDEX_STAMINA),

	ITEMSTATCOSTFLAG_UPDATEANIMRATE = (1 << ITEMSTATCOSTFLAGINDEX_UPDATEANIMRATE),
	ITEMSTATCOSTFLAG_FMIN = (1 << ITEMSTATCOSTFLAGINDEX_FMIN),
	ITEMSTATCOSTFLAG_FCALLBACK = (1 << ITEMSTATCOSTFLAGINDEX_FCALLBACK),
	ITEMSTATCOSTFLAG_SAVED = (1 << ITEMSTATCOSTFLAGINDEX_SAVED),
	ITEMSTATCOSTFLAG_CSVSIGNED = (1 << ITEMSTATCOSTFLAGINDEX_CSVSIGNED),
};

struct D2ItemStatCostTxt
{
	uint16_t wStatId;						//0x00
	uint16_t pad0x02;						//0x02
	uint32_t dwItemStatFlags;				//0x04
	uint8_t nSendBits;						//0x08
	uint8_t nSendParamBits;					//0x09
	uint8_t nCsvBits;						//0x0A
	uint8_t nCsvParam;						//0x0B
	uint32_t dwDivide;						//0x0C
	uint32_t dwMultiply;					//0x10
	uint32_t dwAdd;							//0x14
	uint8_t nValShift;						//0x18
	uint8_t nSaveBits;						//0x19
	uint8_t n09SaveBits;					//0x1A
	uint8_t pad0x1B;						//0x1B
	uint32_t dwSaveAdd;						//0x1C
	uint32_t dw09SaveAdd;					//0x20
	uint32_t dwSaveParamBits;				//0x24
	uint32_t dw09SaveParamBits;				//0x28
	int32_t dwMinAccr;						//0x2C
	uint8_t nEncode;						//0x30
	uint8_t pad0x31;						//0x31
	uint16_t wMaxStat;						//0x32
	int16_t nDescPriority;					//0x34
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
	uint8_t unk0x51[3];						//0x51 - also related to op stats (see DATATBLS_LoadItemStatCostTxt)
	uint8_t nOp;							//0x54
	uint8_t nOpParam;						//0x55
	uint16_t wOpBase;						//0x56
	uint16_t wOpStat[3];					//0x58
	uint16_t unk0x5E[64];					//0x5E - also related to op stats (see DATATBLS_LoadItemStatCostTxt)
	D2OpStatDataStrc pOpStatData[16];		//0xDE
	uint16_t pad0x13E;						//0x13E
	uint32_t dwStuff;						//0x140
};

struct D2ItemsTxt
{
	char szFlippyFile[32];					//0x00
	char szInvFile[32];						//0x20
	char szUniqueInvFile[32];				//0x40
	char szSetInvFile[32];					//0x60
	union
	{
		uint32_t dwCode;					//0x80
		char szCode[4];						//0x80
	};
	uint32_t dwNormCode;					//0x84
	uint32_t dwUberCode;					//0x88
	uint32_t dwUltraCode;					//0x8C
	uint32_t dwAlternateGfx;				//0x90
	uint32_t dwPspell;						//0x94
	uint16_t wState;						//0x98
	uint16_t wCurseState[2];				//0x9A
	uint16_t wStat[3];						//0x9E
	uint32_t dwCalc[3];						//0xA4
	uint32_t dwLen;							//0xB0
	uint8_t nSpellDesc;						//0xB4
	uint8_t pad0xB5;						//0xB5
	uint16_t wSpellDescStr;					//0xB6
	uint32_t dwSpellDescCalc;				//0xB8
	uint32_t dwBetterGem;					//0xBC
	uint32_t dwWeapClass;					//0xC0
	uint32_t dwWeapClass2Hand;				//0xC4
	uint32_t dwTransmogrifyType;			//0xC8
	int32_t dwMinAc;						//0xCC
	int32_t dwMaxAc;						//0xD0
	uint32_t dwGambleCost;					//0xD4
	int32_t dwSpeed;						//0xD8
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
	uint8_t nLevel;							//0xFD
	uint8_t nMinDam;						//0xFE
	uint8_t nMaxDam;						//0xFF
	uint8_t nMinMisDam;						//0x100
	uint8_t nMaxMisDam;						//0x101
	uint8_t n2HandMinDam;					//0x102
	uint8_t n2HandMaxDam;					//0x103
	uint8_t nRangeAdder;					//0x104
	uint8_t unk0x105;						//0x105
	int16_t nStrBonus;						//0x106
	int16_t nDexBonus;						//0x108
	uint16_t wReqStr;						//0x10A
	uint16_t wReqDex;						//0x10C
	uint8_t nAbsorb;						//0x10E
	uint8_t nInvWidth;						//0x10F
	uint8_t nInvHeight;						//0x110
	uint8_t nBlock;							//0x111
	uint8_t nDurability;					//0x112
	uint8_t nNoDurability;					//0x113
	uint8_t nMissile;						//0x114
	uint8_t nComponent;						//0x115
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
	uint8_t nQuest;							//0x12A
	uint8_t nQuestDiffCheck;				//0x12B
	uint8_t nTransparent;					//0x12C
	uint8_t nTransTbl;						//0x12D
	uint8_t pad0x12E;						//0x12E
	uint8_t nLightRadius;					//0x12F
	uint8_t nBelt;							//0x130
	uint8_t nAutoBelt;						//0x131
	uint8_t nStackable;						//0x132
	uint8_t nSpawnable;						//0x133
	uint8_t nSpellIcon;						//0x134
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
	int8_t nTransform;						//0x141
	int8_t nInvTrans;						//0x142
	uint8_t nCompactSave;					//0x143
	uint8_t nSkipName;						//0x144
	uint8_t nNameable;						//0x145
	uint8_t nVendorMin[17];					//0x146
	uint8_t nVendorMax[17];					//0x157
	uint8_t nVendorMagicMin[17];			//0x168
	uint8_t nVendorMagicMax[17];			//0x179
	uint8_t nVendorMagicLvl[17];			//0x18A
	uint8_t pad0x19B;						//0x19B
	uint32_t dwNightmareUpgrade;			//0x19C
	uint32_t dwHellUpgrade;					//0x1A0
	uint8_t nPermStoreItem;					//0x1A4
	uint8_t nMultibuy;						//0x1A5
	uint16_t pad0x1A6;						//0x1A6
};

struct D2ItemTypesTxt
{
	char szCode[4];							//0x00
	int16_t nEquiv1;						//0x04
	int16_t nEquiv2;						//0x06
	uint8_t nRepair;						//0x08
	uint8_t nBody;							//0x09
	uint8_t nBodyLoc1;						//0x0A
	uint8_t nBodyLoc2;						//0x0B
	uint16_t wShoots;						//0x0C
	uint16_t wQuiver;						//0x0E
	uint8_t nThrowable;						//0x10
	uint8_t nReload;						//0x11
	uint8_t nReEquip;						//0x12
	uint8_t nAutoStack;						//0x13
	uint8_t nMagic;							//0x14
	uint8_t nRare;							//0x15
	uint8_t nNormal;						//0x16
	uint8_t nCharm;							//0x17
	uint8_t nGem;							//0x18
	uint8_t nBeltable;						//0x19
	uint8_t nMaxSock1;						//0x1A
	uint8_t nMaxSock25;						//0x1B
	uint8_t nMaxSock40;						//0x1C
	uint8_t nTreasureClass;					//0x1D
	uint8_t nRarity;						//0x1E
	uint8_t nStaffMods;						//0x1F
	uint8_t nCostFormula;					//0x20
	uint8_t nClass;							//0x21
	uint8_t nStorePage;						//0x22
	uint8_t nVarInvGfx;						//0x23
	char szInvGfx[6][32];					//0x24
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

struct D2GambleTxt
{
	uint32_t dwItemCode;					//0x00
	int32_t nLevel;							//0x04
	int32_t nItemId;						//0x08
	//char szItemCode[12];					//0x00
};

struct D2GemsTxt
{
	char szName[32];						//0x00
	char szLetter[8];						//0x20
	union
	{
		uint32_t dwItemCode;				//0x28
		char szItemCode[4];					//0x28
	};
	uint16_t wStringId;						//0x2C
	uint8_t nNumMods;						//0x2E
	uint8_t nTransForm;						//0x2F
	D2PropertyStrc pProperties[3][3];		//0x30
};

struct D2GemDataTbl									//sgptDataTable + 0xCFC
{
	int nGemsTxtRecordCount;						//0x00
	D2GemsTxt* pGemsTxt;							//0x04
};

struct D2LowQualityItemsTxt
{
	char szName[32];						//0x00
	uint16_t wTblId;						//0x20
};

struct D2LowQualityItemDataTbl						//sgptDataTable + 0xD04
{
	int nLowQualityItemsTxtRecordCount;				//0x00
	D2LowQualityItemsTxt* pLowQualityItemsTxt;		//0x04
};


struct D2BooksTxt
{
	uint16_t wStr;									//0x00
	uint8_t nSpellIcon;								//0x02
	uint8_t pad0x03;								//0x03
	uint32_t dwPSpell;								//0x04
	uint32_t dwScrollSkillId;						//0x08
	uint32_t dwBookSkillId;							//0x0C
	uint32_t dwBaseCost;							//0x10
	uint32_t dwCostPerCharge;						//0x14
	uint32_t dwScrollSpellCode;						//0x18
	uint32_t dwBookSpellCode;						//0x1C
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

struct D2MagicAffixTxt
{
	char szName[32];								//0x00
	uint16_t wTblIndex;								//0x20
	uint16_t wVersion;								//0x22
	D2PropertyStrc pProperties[3];					//0x24
	uint16_t wSpawnable;							//0x54
	char nTransformColor;							//0x56
	uint8_t unk0x57;								//0x57
	uint32_t dwLevel;								//0x58
	uint32_t dwGroup;								//0x5C
	uint32_t dwMaxLevel;							//0x60
	uint8_t nRare;									//0x64
	uint8_t nLevelReq;								//0x65
	uint8_t nClassSpecific;							//0x66
	uint8_t nClass;									//0x67
	uint8_t nClassLevelReq;							//0x68
	uint8_t pad0x69;								//0x69
	uint16_t wIType[7];								//0x6A
	uint16_t wEType[5];								//0x78
	uint16_t wFrequency;							//0x82
	uint32_t dwDivide;								//0x84
	uint32_t dwMultiply;							//0x88
	uint32_t dwAdd;									//0x8C
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

#pragma pack()

//D2Common.0x6FD550E0
int __fastcall DATATBLS_MapItemsTxtKeywordToNumber(char* szKey);
//D2Common.0x6FD55140
int __fastcall DATATBLS_Return2();
//D2Common.0x6FD55150
int __fastcall sub_6FD55150(char* szText, int* a2, int a3, int nKeywordNumber);
//D2Common.0x6FD551E0
void __fastcall DATATBLS_ItemCalcLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD55280
void __fastcall DATATBLS_LoadItemsTxt(void* pMemPool);
//D2Common.0x6FD575D0
void __fastcall DATATBLS_UnloadItemsTxt();
//D2Common.0x6FD57620 (#10599)
D2ItemDataTbl* __stdcall DATATBLS_GetItemDataTables();
//D2Common.0x6FD57630 (#10597)
int __stdcall DATATBLS_MapOldItemIndexToCurrent(int nItemId);
//D2Common.0x6FD57680 (#10600)
D2ItemsTxt* __stdcall DATATBLS_GetItemsTxtRecord(int nItemId);
//D2Common.0x6FD576D0 (#10601)
D2ItemsTxt* __stdcall DATATBLS_GetItemRecordFromItemCode(uint32_t dwCode, int* pItemId);
//D2Common.0x6FD57720 (#10602)
int __stdcall DATATBLS_GetItemIdFromItemCode(uint32_t dwCode);
//D2Common.0x6FD57740
void __fastcall DATATBLS_ItemParamLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD57820
void __fastcall DATATBLS_LoadMagicSuffix_Prefix_AutomagicTxt(void* pMemPool);
//D2Common.0x6FD58080
void __fastcall DATATBLS_UnloadMagicSuffix_Prefix_AutomagicTxt();
//D2Common.0x6FD580B0 (#10603)
D2MagicAffixDataTbl* __stdcall DATATBLS_GetMagicAffixDataTables();
//D2Common.0x6FD580C0 (#10604)
D2MagicAffixTxt* __stdcall DATATBLS_GetMagicAffixTxtRecord(int nIndex);
//D2Common.0x6FD58110
void __fastcall DATATBLS_LoadRareSuffix_PrefixTxt(void* pMemPool);
//D2Common.0x6FD58450
void __fastcall DATATBLS_UnloadRareSuffix_PrefixTxt();
//D2Common.0x6FD58480 (#10605)
D2RareAffixDataTbl* __fastcall DATATBLS_GetRareAffixDataTables();
//D2Common.0x6FD58490 (#10606)
D2RareAffixTxt* __stdcall DATATBLS_GetRareAffixTxtRecord(int nId);
//D2Common.0x6FD584E0
void __fastcall DATATBLS_LoadUniqueItemsTxt(void* pMemPool);
//D2Common.0x6FD59110
void __fastcall DATATBLS_UnloadUniqueItemsTxt();
//D2Common.0x6FD59140
void __fastcall DATATBLS_LoadSets_SetItemsTxt(void* pMemPool);
//D2Common.0x6FD5AE00
void __fastcall DATATBLS_UnloadSets_SetItemsTxt();
//D2Common.0x6FD5AE40
void __fastcall DATATBLS_LoadQualityItemsTxt(void* pMemPool);
//D2Common.0x6FD5B250
void __fastcall DATATBLS_UnloadQualityItemsTxt();
//D2Common.0x6FD5B260 (#10611)
D2QualityItemDataTbl* __fastcall DATATBLS_GetQualityItemDataTables();
//D2Common.0x6FD5B270 (#10612)
D2QualityItemsTxt* __stdcall DATATBLS_GetQualityItemsTxtRecord(int nIndex);
//D2Common.0x6FD5B2F0
void __fastcall DATATBLS_LoadGemsTxt(void* pMemPool);
//D2Common.0x6FD5BAE0
void __fastcall DATATBLS_UnloadGemsTxt();
//D2Common.0x6FD5BAF0 (#10615)
D2GemDataTbl* __fastcall DATATBLS_GetGemDataTables();
//D2Common.0x6FD5BB00 (#10616)
D2GemsTxt* __stdcall DATATBLS_GetGemsTxtRecord(int nGemId);
//D2Common.0x6FD5BB70
void __fastcall DATATBLS_LoadBooksTxt(void* pMemPool);
//D2Common.0x6FD5BD10
void __fastcall DATATBLS_UnloadBooksTxt();
//D2Common.0x6FD5BD20 (#10617)
D2BookDataTbl* __fastcall DATATBLS_GetBookDataTables();
//D2Common.0x6FD5BD30 (#10618)
D2BooksTxt* __stdcall DATATBLS_GetBooksTxtRecord(int nBookId);
//D2Common.0x6FD5BDA0
void __fastcall DATATBLS_LoadLowQualityItemsTxt(void* pMemPool);
//D2Common.0x6FD5BE40
void __fastcall DATATBLS_UnloadLowQualityItemsTxt();
//D2Common.0x6FD5BE50 (#10613)
D2LowQualityItemDataTbl* __fastcall DATATBLS_GetLowQualityItemDataTables();
//D2Common.0x6FD5BE60 (#10614)
D2LowQualityItemsTxt* __stdcall DATATBLS_GetLowQualityItemsTxtRecord(int nId);
//D2Common.0x6FD5BED0
void __fastcall DATATBLS_LoadItemRatioTxt(void* pMemPool);
//D2Common.0x6FD5C200
void __fastcall DATATBLS_UnloadItemRatioTxt();
//D2Common.0x6FD5C210 (#10622)
D2ItemRatioDataTbl* __fastcall DATATBLS_GetItemRatioDataTables();
//D2Common.0x6FD5C220 (#10623)
D2ItemRatioTxt* __stdcall DATATBLS_GetItemRatioTxtRecord(int nItemId, uint8_t nDifficulty, uint16_t wVersion);
//D2Common.0x6FD5C2F0
int __cdecl DATATBLS_CompareItemStatCostDescs(const void* pRecord1, const void* pRecord2);
//D2Common.0x6FD5C320
void __fastcall DATATBLS_LoadItemStatCostTxt(void* pMemPool);
//D2Common.0x6FD5D070
void __fastcall DATATBLS_UnloadItemStatCostTxt();
//D2Common.0x6FD5D0D0
void __fastcall DATATBLS_LoadPropertiesTxt(void* pMemPool);
//D2Common.0x6FD5D5E0
void __fastcall DATATBLS_UnloadPropertiesTxt();
//D2Common.0x6FD5D600
void __fastcall DATATBLS_LoadGambleTxt(void* pMemPool);
//D2Common.0x6FD5D790
int __cdecl DATATBLS_CompareGambleTxtRecords(const void* pRecord1, const void* pRecord2);
//D2Common.0x6FD5D7B0
void __fastcall DATATBLS_UnloadGambleTxt();
//D2Common.0x6FD5D7F0 (#10671)
D2GambleDataTbl* __fastcall DATATBLS_GetGambleDataTables();
//D2Common.0x6FD5D800
BOOL __fastcall DATATBLS_CheckNestedItemTypes(int nItemType1, int nItemType2);
//D2Common.0x6FD5D8C0
void __fastcall DATATBLS_LoadItemTypesTxt(void* pMemPool);
//D2Common.0x6FD5DFE0
void __fastcall DATATBLS_UnloadItemTypesTxt();
//D2Common.0x6FD5E020
void __fastcall DATATBLS_LoadRunesTxt(void* pMemPool);
//D2Common.0x6FD5E9C0
void __fastcall DATATBLS_UnloadRunesTxt();
//D2Common.0x6FD5E9E0 (#10619)
D2RuneDataTbl* __fastcall DATATBLS_GetRuneDataTables();
//D2Common.0x6FD5E9F0 (#10621)
void __stdcall DATATBLS_AddOrChangeRunesTxtRecord(int nRecordId, D2RunesTxt* pRecord);
//D2Common.0x6FD5EAA0 (#10620)
D2RunesTxt* __stdcall DATATBLS_GetRunesTxtRecord(int nRunewordId);

