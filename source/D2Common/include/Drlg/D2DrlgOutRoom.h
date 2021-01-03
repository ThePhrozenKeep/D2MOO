#pragma once

#include "CommonDefinitions.h"
#include "D2DrlgDrlgGrid.h"

#pragma pack(1)


struct D2DrlgOutdoorRoomStrc
{
	D2DrlgGridStrc pOrientationGrid;		//0x00
	D2DrlgGridStrc pWallGrid;				//0x14
	D2DrlgGridStrc pFloorGrid;				//0x28
	D2DrlgGridStrc pCellGrid;				//0x3C
	D2DrlgVertexStrc* pVertex;				//0x50
	int32_t dwFlags;							//0x54
	int32_t dwFlagsEx;							//0x58
	int32_t unk0x5C;							//0x5C
	int32_t unk0x60;							//0x60
	int32_t nSubType;							//0x64
	int32_t nSubTheme;							//0x68
	int32_t nSubThemePicked;					//0x6C
};

#pragma pack()


//D2Common.0x6FD83D20
void __fastcall DRLGOUTROOM_FreeDrlgOutdoorRoom(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD83D90
void __fastcall DRLGOUTROOM_FreeDrlgOutdoorRoomData(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD83DE0
void __fastcall DRLGOUTROOM_AllocDrlgOutdoorRoom(D2RoomExStrc* pRoomEx);
//D2Common.6FD83E20
void __fastcall DRLGOUTROOM_InitializeDrlgOutdoorRoom(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD83EC0
BOOL __fastcall DRLGOUTROOM_LinkLevelsByLevelCoords(D2DrlgLevelLinkDataStrc* pLevelLinkData);
//D2Common.0x6FD83F70
BOOL __fastcall DRLGOUTROOM_LinkLevelsByLevelDef(D2DrlgLevelLinkDataStrc* pLevelLinkData);
//D2Common.0x6FD84010
BOOL __fastcall DRLGOUTROOM_LinkLevelsByOffsetCoords(D2DrlgLevelLinkDataStrc* pLevelLinkData);
