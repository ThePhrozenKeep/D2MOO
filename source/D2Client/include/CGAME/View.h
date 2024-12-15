#pragma once  

#include <D2BasicTypes.h>
#include <Units/Units.h>
#include <Drlg/D2DrlgDrlg.h>

#pragma pack(push, 1)

enum D2GameViewSpriteType : int32_t
{
	SPRITE_UNIT = 0x0,
	SPRITE_TILE = 0x1,
	SPRITE_UNIT_IN_LoS = 0x2,
};
union D2GameViewSpriteDataPointerStrc
{
	void* pData;
	D2UnitStrc* pUnit;
	D2DrlgTileDataStrc* pDrlgTileData;
};

struct D2GameViewSpriteStrc
{
	D2GameViewSpriteType nType;
	int nX;
	int nY;
	D2GameViewSpriteDataPointerStrc pData;
	D2GameViewSpriteStrc* pNext;
};
struct D2UnkGameViewStrc
{
	int field_0;
	int field_4;
	int field_8;
	int field_C;
	D2GameViewSpriteStrc* pSpriteList;
	int field_14;
	D2GameViewSpriteStrc* field_18;
	D2GameViewSpriteStrc* field_1C;
	int field_20;
};
struct D2GameViewRenderDataStrc
{
	int dwRenderFlags;
	D2GameViewSpriteStrc sprites[3000];
	DWORD nSprites;
	int nOffsetX;
	int nOffsetY;
	D2UnkGameViewStrc* pSquares;
	int nSquares;
	int dwSquareSide;
	int field_EA7C;
	int field_EA80;
};

struct D2GameViewStrc
{
	int dwFlags;
	RECT ViewRadius;
	RECT CheckRadius;
	int nOffsetX;
	int nOffsetY;
	DWORD* pGouraudTblX;
	DWORD* pGouraudTblY;
	DWORD* pGouraudTblValX;
	D2GameViewRenderDataStrc pRenderData;
};
#pragma pack(pop)

//1.10f: D2Client.0x6FAB6FF0
D2GameViewStrc* __fastcall GAMEVIEW_AllocateGameView();
