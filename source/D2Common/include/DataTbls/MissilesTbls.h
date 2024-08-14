#pragma once 

#include <D2BasicTypes.h>

#pragma pack(1)

enum D2C_MissilesTxtFlagIndices
{
	MISSILESFLAGINDEX_LASTCOLLIDE = 0,
	MISSILESFLAGINDEX_EXPLOSION,
	MISSILESFLAGINDEX_PIERCE,
	MISSILESFLAGINDEX_CANSLOW,
	MISSILESFLAGINDEX_CANDESTROY,
	MISSILESFLAGINDEX_CLIENTSEND,
	MISSILESFLAGINDEX_GETHIT,
	MISSILESFLAGINDEX_SOFTHIT,
	MISSILESFLAGINDEX_APPLYMASTERY,
	MISSILESFLAGINDEX_RETURNFIRE,
	MISSILESFLAGINDEX_TOWN,
	MISSILESFLAGINDEX_SRCTOWN,
	MISSILESFLAGINDEX_NOMULTISHOT,
	MISSILESFLAGINDEX_NOUNIQUEMOD,
	MISSILESFLAGINDEX_HALF2HSRC,
	MISSILESFLAGINDEX_MISSILESKILL,
};

enum D2C_MissilesTxtFlags
{
	MISSILESFLAG_LASTCOLLIDE = (1 << MISSILESFLAGINDEX_LASTCOLLIDE),
	MISSILESFLAG_EXPLOSION = (1 << MISSILESFLAGINDEX_EXPLOSION),
	MISSILESFLAG_PIERCE = (1 << MISSILESFLAGINDEX_PIERCE),
	MISSILESFLAG_CANSLOW = (1 << MISSILESFLAGINDEX_CANSLOW),
	MISSILESFLAG_CANDESTROY = (1 << MISSILESFLAGINDEX_CANDESTROY),
	MISSILESFLAG_CLIENTSEND = (1 << MISSILESFLAGINDEX_CLIENTSEND),
	MISSILESFLAG_GETHIT = (1 << MISSILESFLAGINDEX_GETHIT),
	MISSILESFLAG_SOFTHIT = (1 << MISSILESFLAGINDEX_SOFTHIT),
	MISSILESFLAG_APPLYMASTERY = (1 << MISSILESFLAGINDEX_APPLYMASTERY),
	MISSILESFLAG_RETURNFIRE = (1 << MISSILESFLAGINDEX_RETURNFIRE),
	MISSILESFLAG_TOWN = (1 << MISSILESFLAGINDEX_TOWN),
	MISSILESFLAG_SRCTOWN = (1 << MISSILESFLAGINDEX_SRCTOWN),
	MISSILESFLAG_NOMULTISHOT = (1 << MISSILESFLAGINDEX_NOMULTISHOT),
	MISSILESFLAG_NOUNIQUEMOD = (1 << MISSILESFLAGINDEX_NOUNIQUEMOD),
	MISSILESFLAG_HALF2HSRC = (1 << MISSILESFLAGINDEX_HALF2HSRC),
	MISSILESFLAG_MISSILESKILL = (1 << MISSILESFLAGINDEX_MISSILESKILL),
};


enum D2C_MissilesTxtCollideTypes
{
	MISSILESCOLLIDE_NONE = 0,
	MISSILESCOLLIDE_PLAYER = 1,
	MISSILESCOLLIDE_MONSTER = 2,
	MISSILESCOLLIDE_PLAYER_AND_MONSTER = 3,
	MISSILESCOLLIDE_NONE4 = 4, // Same as 0
	MISSILESCOLLIDE_MONSTER_5 = 5, // Same as 2
	MISSILESCOLLIDE_MISSILE_BARRIER = 6, // Missile barriers, Doors, ...
	MISSILESCOLLIDE_MISSILE = 7,
	MISSILESCOLLIDE_PLAYER_MONSTER_BARRIER_WALL = 8,
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
	int32_t dwParam[5];					//0x38
	uint32_t dwHitPar[3];					//0x4C
	uint32_t dwCltParam[5];					//0x58
	uint32_t dwCltHitPar[3];				//0x6C
	int32_t dwDmgParam[2];					//0x78
	uint32_t dwSrvCalc;						//0x80
	uint32_t dwCltCalc;						//0x84
	uint32_t dwHitCalc;						//0x88
	uint32_t dwCltHitCalc;					//0x8C
	uint32_t dwDmgCalc;						//0x90
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
	uint8_t nKnockBack;						//0xAE
	uint8_t pad0xAF;						//0xAF
	uint32_t dwMinDamage;					//0xB0
	uint32_t dwMaxDamage;					//0xB4
	uint32_t dwMinLevDam[5];				//0xB8
	uint32_t dwMaxLevDam[5];				//0xCC
	uint32_t dwDmgSymPerCalc;				//0xE0
	uint8_t nElemType;						//0xE4
	uint8_t pad0xE5[3];						//0xE5
	uint32_t dwElemMin;						//0xE8
	uint32_t dwElemMax;						//0xEC
	uint32_t dwMinElemLev[5];				//0xF0
	uint32_t dwMaxElemLev[5];				//0x104
	uint32_t dwElemDmgSymPerCalc;			//0x118
	uint32_t dwElemLen;						//0x11C
	uint32_t dwElemLevLen[3];				//0x120
	uint8_t nCltSrcTown;					//0x12C
	uint8_t nSrcDamage;						//0x12D
	uint8_t nSrcMissDmg;					//0x12E
	uint8_t nHoly;							//0x12F
	uint8_t nLight;							//0x130
	uint8_t nFlicker;						//0x131
	uint8_t nRGB[3];						//0x132
	uint8_t nInitSteps;						//0x135
	uint8_t nActivate;						//0x136
	uint8_t nLoopAnim;						//0x137
	char szCelFile[64];						//0x138
	uint8_t nAnimLen;						//0x178
	uint8_t pad0x179[3];					//0x179
	uint32_t dwRandStart;					//0x17C
	uint8_t nSubLoop;						//0x180
	uint8_t nSubStart;						//0x181
	uint8_t nSubStop;						//0x182
	uint8_t nCollideType;					//0x183 D2C_MissilesTxtCollideTypes
	uint8_t nCollision;						//0x184
	uint8_t nClientCol;						//0x185
	uint8_t nCollideKill;					//0x186
	uint8_t nCollideFriend;					//0x187
	uint8_t nNextHit;						//0x188
	uint8_t nNextDelay;						//0x189
	uint8_t nSize;							//0x18A
	uint8_t nToHit;							//0x18B
	uint8_t nAlwaysExplode;					//0x18C
	uint8_t nTrans;							//0x18D
	uint8_t nQty;							//0x18E
	uint8_t pad0x18F;						//0x18F
	uint32_t dwSpecialSetup;				//0x190
	int16_t nSkill;							//0x194
	uint8_t nHitShift;						//0x196
	uint8_t unk0x197[5];					//0x197
	uint32_t dwDamageRate;					//0x19C
	uint8_t nNumDirections;					//0x1A0
	uint8_t nAnimSpeed;						//0x1A1
	uint8_t nLocalBlood;					//0x1A2
	uint8_t pad0x1A3;						//0x1A3
};

#pragma pack()


//D2Common.0x6FD62EA0
int __fastcall DATATBLS_MapMissilesTxtKeywordToNumber(char* szKey);
//D2Common.0x6FD62F20
int __fastcall sub_6FD62F20(char* szText, int* a2, int a3, int nKeywordNumber);
//D2Common.0x6FD630F0
void __fastcall DATATBLS_MissileCalcLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD63180
void __fastcall DATATBLS_LoadMissilesTxt(HD2ARCHIVE hArchive);
//D2Common.0x6FD64B80
void __fastcall DATATBLS_UnloadMissilesTxt();
//D2Common.0x6FD64BE0 (#10590)
D2COMMON_DLL_DECL int __stdcall DATATBLS_GetMissileVelocityFromMissilesTxt(int nMissileId, int nLevel);
//Inlined at various places
D2MissilesTxt* __fastcall DATATBLS_GetMissilesTxtRecord(int nMissileId);

