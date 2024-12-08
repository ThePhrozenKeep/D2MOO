#pragma once  

#include <D2BasicTypes.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Config.h>

#pragma pack(push, 1)
enum eD2GameTypes : uint32_t
{
	GAMETYPE_SINGLEPLAYER = 0x0,
	GAMETYPE_SINGLEPLAYER_UNCAPPED = 0x1,
	GAMETYPE_BNET_BETA = 0x2,
	GAMETYPE_BNET = 0x3,
	GAMETYPE_BNET_INTERNAL = 0x4,
	GAMETYPE_BNET_UNUSED = 0x5,
	GAMETYPE_OBNET_HOST = 0x6,
	GAMETYPE_OBNET_JOIN = 0x7,
	GAMETYPE_LAN_HOST = 0x8,
	GAMETYPE_LAN_JOIN = 0x9,
	GAMETYPE_COUNT = 0xA,
};

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

//1.00 :D2Client.0x10003300
//1.10f:D2Client.0x6FAA25D0
//1.13c:D2Client.0x
int __fastcall ExecuteMessageLoop_6FAA25D0(int(__stdcall* pLoopBody)(int));

//1.00 :D2Client.0x10001B7C
//1.10f:D2Client.0x6FAA9AF0
//1.13c:D2Client.0x6FAF4B50
signed int __stdcall MainLoop_6FAA9AF0(int nIteration);

//1.10f:D2Client.0x6FAAB370
//1.13c:D2Client.0x6FAF4F40
int __fastcall D2Client_Main_sub_6FAAB370();

//1.00 :D2Client.0x10001CA3
//1.10f:D2Client.0x6FB283D0
D2UnitStrc* __fastcall D2CLIENT_GetPlayerUnit_6FB283D0();


// Helper function
eD2GameTypes GAME_GetGameType();
// Helper function
D2ConfigStrc* CONFIG_GetConfig();
//1.10f:D2Client.0x6FAAB250
BOOL __fastcall sub_6FAAB250();
//1.10f:D2Client.0x6FAAB290
BOOL __fastcall sub_6FAAB290();
//1.10f:D2Client.0x6FAAB2E0
BOOL __fastcall sub_6FAAB2E0();
//1.10f:D2Client.0x6FAAB320
BOOL __fastcall sub_6FAAB320();
