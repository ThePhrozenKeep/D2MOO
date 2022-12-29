#pragma once 

#include <D2BasicTypes.h>

#pragma pack(1)

enum D2ObjectSubClasses
{
	OBJSUBCLASS_SHRINE = 0x01,
	OBJSUBCLASS_OBELISK = 0x02,
	OBJSUBCLASS_TOWNPORTAL = 0x04,
	OBJSUBCLASS_CHEST = 0x08,
	OBJSUBCLASS_PORTAL = 0x10,
	OBJSUBCLASS_WELL = 0x20,
	OBJSUBCLASS_WAYPOINT = 0x40,
	OBJSUBCLASS_DOOR = 0x80,
};

struct D2ObjectsTxt
{
	char szName[64];						//0x00
	wchar_t wszName[64];					//0x40
	char szToken[3];						//0xC0
	uint8_t nSpawnMax;						//0xC3
	uint8_t nSelectable[8];					//0xC4
	uint8_t nTrapProb;						//0xCC
	uint8_t pad0xCD[3];						//0xCD
	int32_t dwSizeX;						//0xD0
	int32_t dwSizeY;						//0xD4
	uint32_t dwFrameCnt[8];					//0xD8
	uint16_t wFrameDelta[8];				//0xF8
	uint8_t nCycleAnim[8];					//0x108
	uint8_t nLit[8];						//0x110
	uint8_t nBlocksLight[8];				//0x118
	uint8_t nHasCollision[8];				//0x120
	uint8_t nIsAttackable0;					//0x128
	uint8_t nStart[8];						//0x129
	uint8_t nOrderFlag[8];					//0x131
	uint8_t nEnvEffect;						//0x139
	uint8_t nIsDoor;						//0x13A
	uint8_t nBlocksVis;						//0x13B
	uint8_t nOrientation;					//0x13C
	uint8_t nPreOperate;					//0x13D
	uint8_t nTrans;							//0x13E
	uint8_t nMode[8];						//0x13F
	uint8_t pad0x147;						//0x147
	int32_t dwXOffset;						//0x148
	int32_t dwYOffset;						//0x14C
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
	uint8_t nGreen;							//0x165
	uint8_t nBlue;							//0x166
	uint8_t nSubClass;						//0x167
	uint32_t dwNameOffset;					//0x168
	uint8_t pad0x16C;						//0x16C
	uint8_t nMonsterOK;						//0x16D
	uint8_t nOperateRange;					//0x16E
	uint8_t nShrineFunction;				//0x16F
	uint8_t nAct;							//0x170
	uint8_t nLockable;						//0x171
	uint8_t nGore;							//0x172
	uint8_t nRestore;						//0x173
	uint8_t nRestoreVirgins;				//0x174
	uint8_t nSync;							//0x175
	uint16_t pad0x176;						//0x177
	uint32_t dwParm[8];						//0x178
	uint8_t nTgtFX;							//0x198
	uint8_t nTgtFY;							//0x199
	uint8_t nTgtBX;							//0x19A
	uint8_t nTgtBY;							//0x19B
	uint8_t nDamage;						//0x19C
	uint8_t nCollisionSubst;				//0x19D
	uint16_t pad0x19E;						//0x19E
	uint32_t dwLeft;						//0x1A0
	uint32_t dwTop;							//0x1A4
	uint32_t dwWidth;						//0x1A8
	uint32_t dwHeight;						//0x1AC
	uint8_t nBeta;							//0x1B0
	uint8_t nInitFn;						//0x1B1
	uint8_t nPopulateFn;					//0x1B2
	uint8_t nOperateFn;						//0x1B3
	uint8_t nClientFn;						//0x1B4
	uint8_t nOverlay;						//0x1B5
	uint8_t nBlockMissile;					//0x1B6
	uint8_t nDrawUnder;						//0x1B7
	uint8_t nOpenWarp;						//0x1B8
	uint8_t pad0x1B9[3];					//0x1B9
	uint32_t dwAutomap;						//0x1BC
};

struct D2ObjGroupTxt
{
	uint32_t dwId[8];						//0x00
	uint8_t nDensity[8];					//0x20
	uint8_t nProbability[8];				//0x28
	uint8_t nShrines;						//0x30
	uint8_t nWells;							//0x31
	uint16_t pad0x32;						//0x32
};



struct D2ShrinesTxt
{
	uint8_t nCode;							//0x00
	uint8_t pad0x01[3];						//0x01
	uint32_t dwArg0;						//0x04
	uint32_t dwArg1;						//0x08
	uint32_t dwDurationInFrames;			//0x0C
	uint8_t nResetTimeInMins;				//0x10
	uint8_t nRarity;						//0x11
	char szViewname[32];					//0x12
	char szNiftyphrase[128];				//0x32
	uint8_t nEffectClass;					//0xB2
	uint8_t pad0xB3;						//0xB3
	uint32_t dwLevelMin;					//0xB4
};

#pragma pack()

//D2Common.0x6FD6FDF0
void __fastcall DATATBLS_LoadObjectsTxt(void* pMemPool);
//D2Common.0x6FD718F0 (#10626)
D2COMMON_DLL_DECL D2ObjectsTxt* __stdcall DATATBLS_GetObjectsTxtRecord(int nObjectId);
//D2Common.0x6FD71960
void __fastcall DATATBLS_UnloadObjectsTxt();
//D2Common.0x6FD71980
void __fastcall DATATBLS_UnloadObjGroupTxt();
//D2Common.0x6FD719A0
void __fastcall DATATBLS_LoadObjGroupTxt(void* pMemPool);
//D2Common.0x6FD71E00 (#10627)
D2COMMON_DLL_DECL D2ObjGroupTxt* __stdcall DATATBLS_GetObjGroupTxtRecord(int nId);
//D2Common.0x6FD71E30
void __fastcall DATATBLS_LoadShrinesTxt(void* pMemPool);
//D2Common.0x6FD72000 (#10624)
D2COMMON_DLL_DECL D2ShrinesTxt* __stdcall DATATBLS_GetShrinesTxtRecord(int nShrineId);
//D2Common.0x6FD72070 (#10625)
D2COMMON_DLL_DECL int __stdcall DATATBLS_GetShrinesTxtRecordCount();
//D2Common.0x6FD72080
void __fastcall DATATBLS_UnloadShrinesTxt();

