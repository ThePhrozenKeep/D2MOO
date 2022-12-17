#pragma once

#include <D2BasicTypes.h>
#include <Fog.h>
#include <stdio.h> // for FILE

#include "DataTbls/AnimTbls.h"
#include "DataTbls/ArenaTbls.h"
#include "DataTbls/BeltsTbls.h"
#include "DataTbls/DataTbls.h"
#include "DataTbls/FieldTbls.h"
#include "DataTbls/HoradricCube.h"
#include "DataTbls/InvTbls.h"
#include "DataTbls/ItemsTbls.h"
#include "DataTbls/LevelsTbls.h"
#include "DataTbls/MissilesTbls.h"
#include "DataTbls/MonsterTbls.h"
#include "DataTbls/ObjectsTbls.h"
#include "DataTbls/OverlayTbls.h"
#include "DataTbls/SkillsTbls.h"
#include "DataTbls/SequenceTbls.h"
#include "DataTbls/TokenTbls.h"
#include "DataTbls/TransformTbls.h"
#include <D2Seed.h>

#pragma pack(1)

struct D2BodyLocsTxt
{
	uint32_t dwCode;						//0x00
};


struct D2CharStatsTxt
{
	wchar_t wszClassName[16];				//0x00
	char szClassName[16];					//0x20
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
	uint8_t nManaRegen;						//0x3A
	uint8_t pad0x3B;						//0x3B
	uint32_t dwToHitFactor;					//0x3C
	uint8_t nWalkSpeed;						//0x40
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
	uint8_t nStatsPerLevel;					//0x50
	uint8_t pad0x51;						//0x51
	uint16_t wStrAllSkills;					//0x52
	uint16_t wStrSkillTab[3];				//0x54
	uint16_t wStrClassOnly;					//0x5A
	D2CharItemStrc pItems[10];				//0x5C
	uint16_t wStartSkill;					//0xAC
	int16_t nBaseSkill[10];					//0xAE
	uint16_t pad0xC2;						//0xC2
};

struct D2DifficultyLevelsTxt
{
	uint32_t dwResistPenalty;				//0x00
	uint32_t dwDeathExpPenalty;				//0x04
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
	uint32_t dwMonsterCEDmgPercent;			//0x3C
	uint32_t dwStaticFieldMin;				//0x40
	uint32_t dwGambleRare;					//0x44
	uint32_t dwGambleSet;					//0x48
	uint32_t dwGambleUniq;					//0x4C
	uint32_t dwGambleUber;					//0x50
	uint32_t dwGambleUltra;					//0x54
};

struct D2HireDescTxt
{
	uint32_t dwCode;						//0x00
};

struct D2HitClassTxt
{
	uint32_t dwCode;						//0x00
};

struct D2MissCalcTxt
{
	uint32_t dwCode;						//0x00
};

struct D2EventsTxt
{
	uint16_t wEvent;						//0x00
};

struct D2ElemTypesTxt
{
	uint32_t dwCode;						//0x00
};

struct D2ColorsTxt
{
	uint32_t dwCode;						//0x00
};

struct D2ExperienceTxt
{
	uint32_t dwClass[7];					//0x00
	uint32_t dwExpRatio;					//0x1C
};

struct D2PlayerClassTxt
{
	uint32_t dwCode;						//0x00
};

struct D2PlrModeTxtStub
{
	uint32_t dwCode;						//0x00
};

struct D2MonAiTxt
{
	uint16_t wAi;							//0x00
};

struct D2MonModeTxtStub
{
	uint32_t dwCode;						//0x00
};

struct D2MonPlaceTxt
{
	uint16_t nCode;							//0x00
};

struct D2StorePageTxt
{
	uint32_t dwCode;						//0x00
};

enum D2C_PetTypeTxtFlagIndices
{
	PETTYPEFLAGINDEX_WARP = 0,
	PETTYPEFLAGINDEX_RANGE,
	PETTYPEFLAGINDEX_PARTYSEND,
	PETTYPEFLAGINDEX_UNSUMMON,
	PETTYPEFLAGINDEX_AUTOMAP,
	PETTYPEFLAGINDEX_DRAWHP,
};

enum D2C_PetTypeTxtFlags
{
	PETTYPEFLAG_WARP = (1 << PETTYPEFLAGINDEX_WARP),
	PETTYPEFLAG_RANGE = (1 << PETTYPEFLAGINDEX_RANGE),
	PETTYPEFLAG_PARTYSEND = (1 << PETTYPEFLAGINDEX_PARTYSEND),
	PETTYPEFLAG_UNSUMMON = (1 << PETTYPEFLAGINDEX_UNSUMMON),
	PETTYPEFLAG_AUTOMAP = (1 << PETTYPEFLAGINDEX_AUTOMAP),
	PETTYPEFLAG_DRAWHP = (1 << PETTYPEFLAGINDEX_DRAWHP),
};

struct D2PetTypeTxt
{
	uint32_t dwHcIdx;						//0x00
	uint32_t dwPetFlags;					//0x04
	uint16_t wGroup;						//0x08
	uint16_t wBaseMax;						//0x0A
	uint16_t wStringId;						//0x0C
	uint8_t nIconType;						//0x0E
	char szBaseicon[32];					//0x0F
	char szIcon[4][32];						//0x2F
	uint8_t pad0xAF[3];						//0xAF
	uint16_t wClass[4];						//0xB2
	uint8_t unk0xBA[2];						//0xBA
	int32_t nSkillCount;					//0xBC
	uint16_t wSkillIds[15];					//0xC0
	uint16_t unk0xDE;						//0xDE
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
		uint8_t nStateFlags[4];				//0x10
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
	char nItemTrans;						//0x2C
	uint8_t nGfxType;						//0x2D
	uint16_t wGfxClass;						//0x2E
	uint16_t wCltEvent;						//0x30
	uint16_t wCltEventFunc;					//0x32
	uint16_t wCltActiveFunc;				//0x34
	uint16_t wSrvActiveFunc;				//0x36
	uint16_t wSkill;						//0x38
	uint16_t wMissile;						//0x3A
};

//TODO: see comments
struct D2DataTablesStrc
{
	D2PlayerClassTxt* pPlayerClassTxt;					//0x00
	D2TxtLinkStrc* pPlayerClassLinker;					//0x04
	D2BodyLocsTxt* pBodyLocsTxt;						//0x08
	D2TxtLinkStrc* pBodyLocsLinker;						//0x0C
	D2StorePageTxt* pStorePageTxt;						//0x10
	D2TxtLinkStrc* pStorePageLinker;					//0x14
	D2ElemTypesTxt* pElemTypesTxt;						//0x18
	D2TxtLinkStrc* pElemTypesLinker;					//0x1C
	D2HitClassTxt* pHitClassTxt;						//0x20
	D2TxtLinkStrc* pHitClassLinker;						//0x24
	D2MonModeTxtStub* pMonModeTxtStub;					//0x28
	D2TxtLinkStrc* pMonModeLinker;						//0x2C
	D2PlrModeTxtStub* pPlrModeTxtStub;					//0x30
	D2TxtLinkStrc* pPlrModeLinker;						//0x34
	D2SkillCalcTxt* pSkillCalcTxt;						//0x38
	D2TxtLinkStrc* pSkillCalcLinker;					//0x3C
	char* pSkillsCode;									//0x40
	unsigned int nSkillsCodeSize;						//0x44
	unsigned int nSkillsCodeSizeEx;						//0x48
	char* pSkillDescCode;								//0x4C
	unsigned int nSkillDescCodeSize;					//0x50
	unsigned int nSkillDescCodeSizeEx;					//0x54
	D2MissCalcTxt* pMissileCalcTxt;						//0x58
	D2TxtLinkStrc* pMissileCalcLinker;					//0x5C
	char* pMissCode;									//0x60
	unsigned int nMissCodeSize;							//0x64
	int nMissCodeSizeEx;								//0x68

	/**/	const char* pSkillCode;                             //+0000006C   052C445C      skillcode.txt (Id from skills.txt)
	/**/	D2TxtLinkStrc* iSkillCode;                       //+00000070   0141F084      skillcode info

	D2EventsTxt* pEventsTxt;							//0x74
	D2TxtLinkStrc* pEventsLinker;						//0x78
	D2CompCodeTxt* pCompCodeTxt;						//0x7C
	D2TxtLinkStrc* pCompCodeLinker;						//0x80
	int nCompCodeTxtRecordCount;						//0x84
	D2MonAiTxt* pMonAiTxt;								//0x88
	D2TxtLinkStrc* pMonAiLinker;						//0x8C
	int nMonAiTxtRecordCount;							//0x90
	D2TxtLinkStrc* pItemsLinker;						//0x94
	char* pItemsCode;									//0x98
	unsigned int nItemsCodeSize;						//0x9C
	int nItemsCodeSizeEx;								//0xA0
	D2PropertiesTxt* pPropertiesTxt;					//0xA4
	D2TxtLinkStrc* pPropertiesLinker;					//0xA8
	int nPropertiesTxtRecordCount;						//0xAC
	D2TxtLinkStrc* pRunesLinker;						//0xB0
	D2HireDescTxt* pHireDescTxt;						//0xB4
	D2TxtLinkStrc* pHireDescLinker;						//0xB8
	D2StatesTxt* pStatesTxt;							//0xBC
	D2TxtLinkStrc* pStatesLinker;						//0xC0
	int nStatesTxtRecordCount;							//0xC4
	uint32_t* pStateMasks;								//0xC8
	uint32_t* fStateMasks[40];							//0xCC
	short* pProgressiveStates;							//0x16C
	int nProgressiveStates;								//0x170
	short* pCurseStates;								//0x174
	int nCurseStates;									//0x178
	short* pTransformStates;							//0x17C
	int nTransformStates;								//0x180
	short* pActionStates;								//0x184
	int nActionStates;									//0x188
	short* pColourStates;								//0x18C
	int nColourStates;									//0x190
	D2SoundsTxtStub* pSoundsTxtCodes;					//0x194
	D2TxtLinkStrc* pSoundsLinker;						//0x198
	int nSoundsTxtCodes;								//0x19C
	D2HirelingTxt* pHirelingTxt;						//0x1A0
	int nHirelingTxtRecordCount;						//0x1A4
	int nClassicHirelingStartRecordIds[256];			//0x1A8
	int nExpansionHirelingStartRecordIds[256];			//0x5A8
	D2NpcTxt* pNpcTxt;									//0x9A8
	int nNpcTxtRecordCount;								//0x9AC
	D2ColorsTxt* pColorsTxt;							//0x9B0
	D2TxtLinkStrc* pColorsLinker;						//0x9B4
	D2TxtLinkStrc* pTreasureClassExLinker;				//0x9B8
	D2TCExShortStrc* pTreasureClassEx;					//0x9BC
	int nTreasureClassEx;								//0x9C0
	D2TCExShortStrc* pChestTreasureClasses[45];			//0x9C4
	D2MonStatsTxt* pMonStatsTxt;						//0xA78
	D2TxtLinkStrc* pMonStatsLinker;						//0xA7C
	int nMonStatsTxtRecordCount;						//0xA80
	D2MonSoundsTxt* pMonSoundsTxt;						//0xA84
	D2TxtLinkStrc* pMonSoundsLinker;					//0xA88
	int nMonSoundsTxtRecordCount;						//0xA8C
	D2MonStats2Txt* pMonStats2Txt;						//0xA90
	D2TxtLinkStrc* pMonStats2Linker;					//0xA94
	int nMonStats2TxtRecordCount;						//0xA98
	D2MonPlaceTxt* pMonPlaceTxt;						//0xA9C
	D2TxtLinkStrc* pMonPlaceLinker;						//0xAA0
	int nMonPlaceTxtRecordCount;						//0xAA4
	D2MonPresetTxt* pMonPresetTxt;						//0xAA8
	D2MonPresetTxt* pMonPresetTxtActSections[5];		//0xAAC
	int nMonPresetTxtActRecordCounts[5];				//0xAC0
	D2SuperUniquesTxt* pSuperUniquesTxt;				//0xAD4
	D2TxtLinkStrc* pSuperUniquesLinker;					//0xAD8
	int nSuperUniquesTxtRecordCount;					//0xADC
	short nSuperUniqueIds[66];							//0xAE0
	D2MissilesTxt* pMissilesTxt;						//0xB64
	D2TxtLinkStrc* pMissilesLinker;						//0xB68
	int nMissilesTxtRecordCount;						//0xB6C
	D2MonLvlTxt* pMonLvlTxt;							//0xB70
	int nMonLvlTxtRecordCount;							//0xB74
	D2MonSeqTxt* pMonSeqTxt;							//0xB78
	D2TxtLinkStrc* pMonSeqLinker;						//0xB7C
	int nMonSeqTxtRecordCount;							//0xB80
	D2SeqRecordStrc* pMonSeqTable;						//0xB84
	int nMonSeqTableRecordCount;						//0xB88
	D2SkillDescTxt* pSkillDescTxt;						//0xB8C
	D2TxtLinkStrc* pSkillDescLinker;					//0xB90
	int nSkillDescTxtRecordCount;						//0xB94
	D2SkillsTxt* pSkillsTxt;							//0xB98
	D2TxtLinkStrc* pSkillsLinker;						//0xB9C
	int nSkillsTxtRecordCount;							//0xBA0
	int* nClassSkillCount;								//0xBA4
	int nHighestClassSkillCount;						//0xBA8
	short* nClassSkillList;								//0xBAC
	int nPassiveSkills;									//0xBB0
	uint16_t* pPassiveSkills;							//0xBB4
	D2TxtLinkStrc* pOverlayLinker;						//0xBB8
	D2OverlayTxt* pOverlayTxt;							//0xBBC
	int nOverlayTxtRecordCount;							//0xBC0
	D2CharStatsTxt* pCharStatsTxt;						//0xBC4
	int nCharStatsTxtRecordCount;						//0xBC8
	D2ItemStatCostTxt* pItemStatCostTxt;				//0xBCC
	D2TxtLinkStrc* pItemStatCostLinker;					//0xBD0
	int nItemStatCostTxtRecordCount;					//0xBD4
	uint16_t* pStatsWithDescFunc;						//0xBD8
	int nStatsWithDescFunc;								//0xBDC
	D2MonEquipTxt* pMonEquipTxt;						//0xBE0
	int nMonEquipTxtRecordCount;						//0xBE4
	D2PetTypeTxt* pPetTypeTxt;							//0xBE8
	D2TxtLinkStrc* pPetTypeLinker;						//0xBEC
	int nPetTypeTxtRecordCount;							//0xBF0
	D2TxtLinkStrc* pItemTypesLinker;					//0xBF4
	D2ItemTypesTxt* pItemTypesTxt;						//0xBF8
	int nItemTypesTxtRecordCount;						//0xBFC
	int nItemTypesIndex;								//0xC00
	uint32_t* pItemTypesEquivalenceLUTs;				//0xC04
	D2TxtLinkStrc* pSetsLinker;							//0xC08
	D2SetsTxt* pSetsTxt;								//0xC0C
	int nSetsTxtRecordCount;							//0xC10
	D2TxtLinkStrc* pSetItemsLinker;						//0xC14
	D2SetItemsTxt* pSetItemsTxt;						//0xC18
	int nSetItemsTxtRecordCount;						//0xC1C
	D2TxtLinkStrc* pUniqueItemsLinker;					//0xC20
	D2UniqueItemsTxt* pUniqueItemsTxt;					//0xC24
	int nUniqueItemsTxtRecordCount;						//0xC28
	D2TxtLinkStrc* pMonPropLinker;						//0xC2C
	D2MonPropTxt* pMonPropTxt;							//0xC30
	int nMonPropTxtRecordCount;							//0xC34
	D2TxtLinkStrc* pMonTypeLinker;						//0xC38
	D2MonTypeTxt* pMonTypeTxt;							//0xC3C
	int nMonTypeTxtRecordCount;							//0xC40
	uint32_t* pMonTypeNest;								//0xC44
	int nMonTypeIndex;									//0xC48
	D2TxtLinkStrc* pMonUModLinker;						//0xC4C
	D2MonUModTxt* pMonUModTxt;							//0xC50
	int nMonUModTxtRecordCount;							//0xC54
	D2LevelsTxt* pLevelsTxt;							//0xC58
	int nLevelsTxtRecordCount;							//0xC5C
	D2LevelDefBin* pLevelDefBin;						//0xC60
	D2LvlPrestTxt* pLvlPrestTxt;						//0xC64
	int nLvlPrestTxtRecordCount;						//0xC68
	int nStuff;											//0xC6C
	int nShiftedStuff;									//0xC70
	D2AnimDataStrc* pAnimData;							//0xC74
	D2ExperienceTxt* pExperienceTxt;					//0xC78
	D2DifficultyLevelsTxt* pDifficultyLevelsTxt;		//0xC7C
	int nDifficultyLevelsTxtRecordCount;				//0xC80
	BOOL bCompileTxt;									//0xC84
	int ExpFieldI[9];									//0xC88
	char* pFieldData;									//0xCAC
	D2CollisionFieldStrc pCollisionField;				//0xCB0
	D2CubeMainTxt* pCubeMainTxt;						//0xCC4
	int nCubeMainTxtRecordCount;						//0xCC8
	int nInventoryTxtRecordCount;						//0xCCC
	D2InventoryTxt* pInventoryTxt;						//0xCD0

	uint32_t unk0xCD4;									//0xCD4

	D2ItemDataTbl pItemDataTables;						//0xCD8
	uint16_t* pIndexOldToCurrent;						//0xCF8	
	D2GemDataTbl pGemDataTables;						//0xCFC
	D2LowQualityItemDataTbl pLowQualityItemDataTables;	//0xD04
	D2BookDataTbl pBookDataTables;						//0xD0C
	D2RareAffixDataTbl pRareAffixDataTables;			//0xD14
	D2ItemRatioDataTbl pItemRatioDataTables;			//0xD24

	uint32_t unk0xD2C;									//0xD2C

	D2GambleDataTbl pGambleDataTables;					//0xD30
	D2MagicAffixDataTbl pMagicAffixDataTables;			//0xEC8
	D2RuneDataTbl pRuneDataTables;						//0xEDC
	D2QualityItemDataTbl pQualityItemDataTables;		//0xEE4

	uint32_t unk0xEEC;									//0xEEC

	D2SeedStrc pAutomapSeed;							//0xEF0
	D2AutomapRandStrc pAutoMapRand[36];					//0xEF8
	D2LvlTypesTxt* pLvlTypesTxt;						//0x1018
	int* pPortalLevels;									//0x101C
	int nPortalLevels;									//0x1020
	int nLvlTypesTxtRecordCount;						//0x1024
	D2LvlWarpTxt* pLvlWarpTxt;							//0x1028
	int nLvlWarpTxtRecordCount;							//0x102C
	D2LvlMazeTxt* pLvlMazeTxt;							//0x1030
	int nLvlMazeTxtRecordCount;							//0x1034
	D2LvlSubTxt* pLvlSubTxt;							//0x1038
	int nLvlSubTxtRecordCount;							//0x103C
	int* pLvlSubTypeStartIds;							//0x1040
	D2TileLibraryHashStrc** ppTileLibraryHash;			//0x1044
	D2DrlgFileStrc** ppLvlPrestFiles;					//0x1048
	D2DrlgFileStrc** ppLvlSubTypeFiles;					//0x104C
	D2AutomapShortStrc* pAutoMap;						//0x1050
	int nAutoMap;										//0x1054
	char szDefaultString;								//0x1058
	char pad0x1059[3];									//0x1059
	D2MonItemPercentDataTbl pMonItemPercentDataTables;	//0x105C
	wchar_t wszDefault;									//0x1064
	uint16_t unk0x1066;									//0x1066
	D2UniqueTitleTxt* pUniqueTitleTxt;					//0x1068
	D2UniquePrefixTxt* pUniquePrefixTxt;				//0x106C
	D2UniqueSuffixTxt* pUniqueSuffixTxt;				//0x1070
	D2UniqueAppellationTxt* pUniqueAppellationTxt;		//0x1074
	int nUniqueTitleTxtRecordCount;						//0x1078
	int nUniquePrefixTxtRecordCount;					//0x107C
	int nUniqueSuffixTxtRecordCount;					//0x1080
	int nUniqueAppellationTxtRecordCount;				//0x1084

	uint32_t unk7[2];									//+00001088
	short nTreasureClassItemTypes;						//0x1090
	short unk0x1092;									//0x1092

	D2ShrinesTxt* pShrinesTxt;							//0x1094
	int nShrinesTxtRecordCount;							//0x1098
	D2ObjectsTxt* pObjectsTxt;							//0x109C
	int nObjectsTxtRecordCount;							//0x10A0
	D2ObjGroupTxt* pObjGroupTxt;						//0x10A4
	int nObjGroupTxtRecordCount;						//0x10A8
	D2ArmTypeTxt* pArmTypeTxt;							//0x10AC
	D2MonModeDataTbl pMonModeDataTables;				//0x10B0
	D2CompositTxt* pCompositTxt;						//0x10C0
	D2ObjModeDataTbl pObjModeDataTables;				//0x10C4
	D2PlrModeDataTbl pPlrModeDataTables;				//0x10D4
};

// D2Common.0x6FDE9600
extern D2ArenaTxt* gpArenaTxtTable;
// D2Common.0x6FDE95F8
extern D2CharTemplateTxt* gpCharTemplateTxtTable;
extern int gnCharTemplateTxtTableRecordCount;
extern uint32_t gnCharTemplateStartIds[64];
// D2Common.0x6FDE9604
extern D2BeltsTxt* gpBeltsTxtTable;
extern D2DataTablesStrc gpDataTables;
// D2Common.0x6FDD6A20 (#10042)
D2COMMON_DLL_DECL extern D2DataTablesStrc* sgptDataTables;


extern D2SeedStrc* gpAutomapSeed;
//D2Common.0x6FDEA704
extern LPCRITICAL_SECTION gpLvlSubTypeFilesCriticalSection;

//TODO: Reimport defs from .cpps

//D2Common.0x6FDD6A24
extern BOOL DATATBLS_LoadFromBin;



#pragma pack()

//D2Common.0x6FDC45EE
size_t __cdecl DATATBLS_LockAndWriteToFile(const void* Str, size_t Size, size_t Count, FILE* File);
//D2Common.0x6FD494D0
uint16_t __fastcall DATATBLS_GetStringIdFromReferenceString(char* szReference);
//D2Common.0x6FD49500 - Changed this function a lot (had 6 hardcoded (i.e. pre-defined) Args)
void __fastcall DATATBLS_InitUnicodeClassNamesInCharStatsTxt();
//D2Common.0x6FD49660 (#11255)
D2COMMON_DLL_DECL uint32_t __stdcall DATATBLS_GetCodeFromCompCodeTxt(int nCompCode);
//D2Common.0x6FD49680 (#11249)
D2COMMON_DLL_DECL uint32_t __stdcall DATATBLS_GetExpRatio(int nLevel);
//D2Common.0x6FD496B0 (#10628)
D2COMMON_DLL_DECL uint32_t __stdcall DATATBLS_GetLevelThreshold(int nClass, uint32_t dwLevel);
//D2Common.0x6FD496E0 (#10629)
D2COMMON_DLL_DECL int __stdcall DATATBLS_GetMaxLevel(int nClass);
//D2Common.0x6FD49710 (#10630)
D2COMMON_DLL_DECL uint32_t __stdcall DATATBLS_GetCurrentLevelFromExp(int nClass, uint32_t dwExperience);
//D2Common.0x6FD49760
void __fastcall DATATBLS_GetBinFileHandle(void* pMemPool, const char* szFile, void** ppFileHandle, int* pSize, int* pSizeEx);
//D2Common.0x6FD49850
int __fastcall DATATBLS_AppendMemoryBuffer(char** ppCodes, int* pSize, int* pSizeEx, char* pBuffer, int nBufferSize);




//D2Common.0x6FD4E4B0 (#10593)
D2COMMON_DLL_DECL D2CharStatsTxt* __fastcall DATATBLS_GetCharstatsTxtTable();
//D2Common.0x6FD4E4C0
D2AnimDataStrc* __fastcall DATATBLS_GetAnimData();
//D2Common.0x6FD4E4D0 (#10655)
D2COMMON_DLL_DECL D2DifficultyLevelsTxt* __stdcall DATATBLS_GetDifficultyLevelsTxtRecord(int nDifficulty);
//D2Common.0x6FD4E500
void __fastcall DATATBLS_LoadStatesTxt(void* pMemPool);
//D2Common.0x6FD4F4A0
void __fastcall DATATBLS_UnloadStatesTxt();
// Helper function
D2StatesTxt* DATATBLS_GetStatesTxtRecord(int nStateId);
//D2Common.0x6FD4F5A0
void __fastcall DATATBLS_LoadPetTypeTxt(void* pMemPool);
//D2Common.0x6FD4F990 (#11298)
D2COMMON_DLL_DECL char* __stdcall DATATBLS_GetUnitNameFromUnit(D2UnitStrc* pUnit, char* szName);
//D2Common.0x6FD4FB50 (#11299)
D2COMMON_DLL_DECL char* __stdcall DATATBLS_GetUnitNameFromUnitTypeAndClassId(int nUnitType, int nClassId, char* szName);
//D2Common.0x6FD4FCF0 (#10580)
D2COMMON_DLL_DECL void __stdcall DATATBLS_WriteBinFile(char* szFileName, void* pWriteBuffer, size_t nBufferSize, int nRecordCount);
//D2Common.0x6FD4FD70 (#10578)
D2COMMON_DLL_DECL void* __stdcall DATATBLS_CompileTxt(void* pMemPool, const char* szName, D2BinFieldStrc* pTbl, int* pRecordCount, int nSize);
//D2Common.0x6FD500F0 (#11242)
D2COMMON_DLL_DECL void __stdcall DATATBLS_ToggleCompileTxtFlag(BOOL bSilent);
//D2Common.0x6FD50110 (#10579)
D2COMMON_DLL_DECL void __stdcall DATATBLS_UnloadBin(void* pBinFile);
//D2Common.0x6FD50150 (#10575)
D2COMMON_DLL_DECL void __stdcall DATATBLS_UnloadAllBins();
//D2Common.0x6FD504B0 (#10576)
D2COMMON_DLL_DECL void __stdcall DATATBLS_LoadAllTxts(void* pMemPool, int a2, int a3);
//D2Common.0x6FD507B0
void __fastcall DATATBLS_LoadSomeTxts(void* pMemPool);
//D2Common.0x6FD50FB0
void __fastcall DATATBLS_LoadCharStatsTxt(void* pMemPool);
//D2Common.0x6FD51BF0
void __fastcall DATATBLS_LoadDifficultyLevelsTxt(void* pMemPool);

