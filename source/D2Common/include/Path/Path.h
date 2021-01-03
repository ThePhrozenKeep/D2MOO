#pragma once

#include "CommonDefinitions.h"
#pragma pack(1)

enum D2PathTypes
{
	PATHTYPE_IDASTAR = 0,
	PATHTYPE_TOWARD = 2,
	PATHTYPE_MISSILE = 4,
	PATHTYPE_KNOCKBACK_SERVER = 8,
	PATHTYPE_KNOCKBACK_CLIENT = 11,
};

struct D2PathPointStrc
{
	uint16_t X;
	uint16_t Y;
};

struct D2DynamicPathStrc
{
	union
	{
		struct
		{
			uint16_t wOffsetX;					//0x00
			uint16_t wPosX;						//0x02
			uint16_t wOffsetY;					//0x04
			uint16_t wPosY;						//0x06
		};

		struct
		{
			uint32_t dwPrecisionX;				//0x00
			uint32_t dwPrecisionY;				//0x04
		};
	};
	uint32_t dwTargetX;						//0x08
	uint32_t dwTargetY;						//0x0C
	uint16_t xSP1;								//0x10
	uint16_t ySP1;								//0x12
	uint16_t xSP2;								//0x14
	uint16_t ySP2;								//0x16
	uint16_t xSP3;								//0x18
	uint16_t ySP3;								//0x1A
	D2RoomStrc* pRoom;						//0x1C
	D2RoomStrc* pRoomNext;					//0x20
	int32_t unk0x24;							//0x24
	int32_t dwPathPoints;						//0x28
	void* unk0x2C;							//0x2C
	D2UnitStrc* pUnit;						//0x30
	uint32_t dwFlags;							//0x34
	uint32_t unk0x38;							//0x38
	uint32_t dwPathType;						//0x3C
	uint32_t dwPrevPathType;					//0x40
	uint32_t dwUnitSize;						//0x44
	uint32_t dwCollisionPattern;				//0x48
	uint32_t dwCollisionType;					//0x4C
	uint32_t unk0x50;							//0x50
	uint16_t unk0x54;							//0x54
	uint16_t unk0x56;							//0x56
	D2UnitStrc* pTargetUnit;				//0x58
	uint32_t dwTargetType;						//0x5C
	uint32_t dwTargetId;						//0x60
	uint8_t nDirection;						//0x64
	uint8_t nNewDirection;						//0x65
	uint8_t nDiffDirection;					//0x66
	uint8_t unk0x67;							//0x67
	uint8_t unk0x68[10];						//0x68
	int32_t unk0x72;							//0x72
	int32_t unk0x76;							//0x76
	char unk0x7A[2];						//0x7A
	uint32_t dwVelocity;						//0x7C
	uint32_t unk0x80;							//0x80
	uint32_t dwVelocity2;						//0x84
	uint32_t unk0x88[2];						//0x88
	uint8_t nDist;								//0x90
	uint8_t nDistMax;							//0x91
	uint8_t unk0x92;							//0x92
	uint8_t nStepNum;							//0x93
	uint8_t nDistance;							//0x94
	char unk0x95[3];						//0x95
	//uint32_t dwPathOffset;						//0x98
	D2PathPointStrc PathPoints[7];			//0x98
	uint32_t unk0xB8[72];						//0xB8
	int32_t unk0x1D4;							//0x1D4
	D2PathPointStrc unk0x1D8[7];			//0x1D8
	char unk0x1DC[12];						//0x1DC
};

struct D2PathInfoStrc
{
	D2PathPointStrc pStartCoord;			//0x00
	D2PathPointStrc field_4;				//0x04
	D2RoomStrc* pRoom;						//0x08
	D2RoomStrc* field_C;					//0x0C
	int32_t field_10;							//0x10
	uint8_t field_14;							//0x14
	uint8_t field_15;							//0x15
	uint16_t field_16;							//0x16
	int32_t nDistMax;							//0x18
	uint8_t field_1C;							//0x1C
	uint8_t field_1D;							//0x1D
	uint16_t field_1E;							//0x1E
	int32_t nPathType;							//0x20
	int32_t nUnitSize;							//0x24
	int32_t nCollisionPattern;					//0x28
	int32_t nCollisionType;						//0x2C
	union
	{
		D2DynamicPathStrc* pDynamicPath;	//0x30 - not sure yet
		D2PathInfoStrc* pNext;				//0x30
	};
};

struct D2StaticPathStrc
{
	D2RoomStrc* pRoom;						//0x00
	int32_t nTargetX;							//0x04
	int32_t nTargetY;							//0x08
	int32_t nXPos;								//0x0C
	int32_t nYPos;								//0x10
	uint32_t unk0x14[2];						//0x14
	uint8_t nDirection;						//0x1C
	uint8_t unk0x1D[3];						//0x1D
};
#pragma pack()

//D2Common.0x6FDA8220
void __fastcall sub_6FDA8220(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA82A0 (#10141)
void __stdcall D2Common_10141(D2UnitStrc* pUnit, int* pX, int* pY);
//D2Common.0x6FDA8320 (#10222)
void __stdcall D2Common_10222(D2UnitStrc* pUnit);
//D2Common.0x6FDA8450 (#10223)
BOOL __stdcall D2Common_10223(D2UnitStrc* pUnit, int a2);
//D2Common.0x6FDA8600
int __stdcall D2Common_10142(D2DynamicPathStrc* pDynamicPath, D2UnitStrc* pUnit, int a3);
//D2Common.0x6FDA8E30
int __fastcall sub_6FDA8E30(D2DynamicPathStrc* pDynamicPath, D2UnitStrc* pUnit);
//D2Common.0x6FDA8FE0
void __fastcall sub_6FDA8FE0(D2PathInfoStrc* pPathInfo);
//D2Common.0x6FDA90C0
uint8_t __fastcall sub_6FDA90C0(D2PathInfoStrc* pPathInfo);
//D2Common.0x6FDA9190 (#10156)
void __stdcall PATH_FreeDynamicPath(void* pMemPool, D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA91B0 (#11282)
int __stdcall D2Common_11282_Unused(int nMonsterId);
//D2Common.0x6FDA9250 (#11281)
int __stdcall D2Common_11281_Unused(D2UnitStrc* pUnit, int nSize);
//D2Common.0x6FDA92F0 (#10214)
void __stdcall D2Common_10214(D2UnitStrc* pUnit);
//D2Common.0x6FDA9480 (#10152)
void __stdcall PATH_AllocDynamicPath(void* pMemPool, D2RoomStrc* pRoom, int nX, int nY, D2UnitStrc* pUnit, BOOL bSetFlag);
//D2Common.0x6FDA9720
void __fastcall sub_6FDA9720(D2DynamicPathStrc* pDynamicPath, uint8_t nDirection);
//D2Common.0x6FDA9770 (#10193)
void __stdcall D2COMMON_10193_PATH_AdjustDirection(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA97C0 (#10216)
void __stdcall D2Common_10216(D2DynamicPathStrc* pDynamicPath, int nX, int nY, int a4);
//D2Common.0x6FDA9850 (#10228)
void __stdcall D2Common_10228(D2UnitStrc* pUnit);
//D2Common.0x6FDA9870 (#10143)
void __stdcall D2Common_10143(D2UnitStrc* pUnit, int a2);
//D2Common.0x6FDA98F0 (#10144)
void __stdcall D2Common_10144(D2UnitStrc* pUnit, BOOL bDoNothing);
//D2Common.0x6FDA9A70 (#10146)
void __stdcall PATH_SetVelocity(D2DynamicPathStrc* pDynamicPath, int nVelocity, char* szFile, int nLine);
//D2Common.0x6FDA9AB0 (#10147)
int __stdcall PATH_GetVelocity(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9AC0 (#10148)
void __stdcall D2Common_10148(D2DynamicPathStrc* pDynamicPath, int a2);
//D2Common.0x6FDA9AE0 (#10149)
int __stdcall D2Common_10149(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9AF0 (#10150)
void __stdcall D2Common_10150(D2DynamicPathStrc* pDynamicPath, int a2);
//D2Common.0x6FDA9B10 (#10151)
int __stdcall D2Common_10151(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9B20 (#10153)
void __stdcall D2Common_10153(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9B40 (#10208)
void __stdcall D2COMMON_10208_PathSetPathingFlag(D2DynamicPathStrc* pDynamicPath, BOOL bSet);
//D2Common.0x6FDA9B70 (#10209)
BOOL __stdcall D2COMMON_10209_PathCheckPathingFlag(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9B80 (#10154)
int __stdcall D2Common_10154(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9B90 (#11291)
void __stdcall D2Common_11291(D2DynamicPathStrc* pDynamicPath, int a2);
//D2Common.0x6FDA9BC0 (#10155)
int __stdcall D2Common_10155(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9BD0 (#10157)
int __stdcall PATH_GetPathPoints(D2DynamicPathStrc* pDynamicPath, D2PathPointStrc** ppPathPoints);
//D2Common.0x6FDA9BF0 (#10158)
uint8_t __stdcall PATH_GetDirection(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9C10 (#10159)
uint8_t __stdcall PATH_GetNewDirection(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9C20 (#10160)
void __stdcall D2COMMON_10160_PathUpdateDirection(D2DynamicPathStrc* pDynamicPath, uint8_t nDirection);
//D2Common.0x6FDA9C90 (#10161)
void __stdcall PATH_SetDirection(D2DynamicPathStrc* pDynamicPath, uint8_t nDirection);
//D2Common.0x6FDA9CB0 (#10162)
int __stdcall PATH_GetXPosition(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9CF0 (#10163)
int __stdcall PATH_GetYPosition(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9D30 (#10194)
int __stdcall PATH_GetPrecisionX(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9D60 (#10195)
int __stdcall PATH_GetPrecisionY(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9D90 (#10196)
void __stdcall PATH_SetPrecisionX(D2DynamicPathStrc* pDynamicPath, int nPrecisionX);
//D2Common.0x6FDA9DA0 (#10197)
void __stdcall PATH_SetPrecisionY(D2DynamicPathStrc* pDynamicPath, int nPrecisionY);
//D2Common.0x6FDA9DB0 (#10164)
int __stdcall PATH_GetTargetX(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDC3CE0 (#10165)
int __stdcall PATH_GetTargetY(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9DC0
void __fastcall PATH_SetTargetX(D2DynamicPathStrc* pDynamicPath, int nTargetX);
//D2Common.0x6FDA9DD0
void __fastcall PATH_SetTargetY(D2DynamicPathStrc* pDynamicPath, int nTargetY);
//D2Common.0x6FDA9DE0 (#10175)
int __stdcall D2COMMON_10175_PathGetFirstPointX(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9DF0 (#10176)
int __stdcall D2COMMON_10176_PathGetFirstPointY(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9E00 (#10224)
int __stdcall D2Common_10224(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9E10 (#10225)
int __stdcall D2Common_10225(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9E20 (#10177)
int __stdcall D2COMMON_10177_PATH_GetLastPointX(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9E40 (#10178)
int __stdcall D2COMMON_10178_PATH_GetLastPointY(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDB9C10 (#10166)
D2RoomStrc* __stdcall PATH_GetRoom(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9E60 (#10167)
void __stdcall PATH_SetRoom(D2DynamicPathStrc* pDynamicPath, D2RoomStrc* pRoom);
//D2Common.0x6FDA9E70 (#10168)
D2RoomStrc* __stdcall PATH_GetNextRoom(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9E80 (#10169)
void __stdcall PATH_ClearNextRoom(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9E90 (#10170)
void __stdcall D2COMMON_10170_PathSetTargetPos(D2DynamicPathStrc* pDynamicPath, int nX, int nY);
//D2Common.0x6FDA9EC0 (#10172)
BOOL __stdcall D2Common_10172(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9ED0 (#10173)
void __stdcall D2Common_10173(D2DynamicPathStrc* pDynamicPath, BOOL bSet);
//D2Common.0x6FDA9F00 (#10145)
void __stdcall D2Common_10145(D2DynamicPathStrc* pDynamicPath, BOOL bSet);
//D2Common.0x6FDA9F30 (#10174)
BOOL __stdcall D2Common_10174(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9F40 (#10179)
void __stdcall PATH_SetTargetUnit(D2DynamicPathStrc* pDynamicPath, D2UnitStrc* pUnit);
//D2Common.0x6FDA9F60 (#10171)
void __stdcall PATH_GetTargetTypeAndGUID(D2DynamicPathStrc* pDynamicPath, int* pTargetType, int* pTargetGUID);
//D2Common.0x6FDA9FA0 (#10180)
D2UnitStrc* __stdcall PATH_GetTargetUnit(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9FC0 (#10181)
int __stdcall PATH_GetCollisionType(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9FE0 (#10182)
void __stdcall PATH_SetCollisionType(D2DynamicPathStrc* pDynamicPath, int nCollisionType);
//D2Common.0x6FDAA0C0 (#10183)
int __stdcall D2Common_10183(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA0D0 (#10184)
void __stdcall D2Common_10184(D2DynamicPathStrc* pDynamicPath, int a2);
//D2Common.0x6FDAA0E0 (#10185)
void __stdcall PATH_SetType(D2DynamicPathStrc* pDynamicPath, int nPathType);
//D2Common.0x6FDAA1E0 (#10186)
void __stdcall PATH_ResetToPreviousType(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA240 (#10187)
int __stdcall PATH_GetType(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA250 (#10190)
void __stdcall _10190_PATH_SetDistance(D2DynamicPathStrc* pDynamicPath, uint8_t nDistance);
//D2Common.0x6FDAA270 (#10191)
uint8_t __stdcall _10191_PATH_GetDistance(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA280 (#10188)
void __stdcall PATH_SetNewDistance(D2DynamicPathStrc* pDynamicPath, uint8_t nNewDistance);
//D2Common.0x6FDAA2B0 (#10189)
uint8_t __stdcall PATH_GetMaxDistance(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA2C0 (#10201)
uint16_t __stdcall D2Common_10201(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA300 (#10202)
uint16_t __stdcall D2Common_10202(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA310 (#10192)
void __stdcall D2COMMON_10192_PathSetIDAMax(D2DynamicPathStrc* pDynamicPath, int a2);
//D2Common.0x6FDAA350 (#10198)
int __stdcall D2COMMON_10198_PathGetSaveStep(D2DynamicPathStrc* pDynamicPath, D2PathPointStrc** ppPathPoints);
//D2Common.0x6FDAA390 (#10199)
int __stdcall D2COMMON_10199_PathGetSaveX(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA3E0 (#10200)
int __stdcall D2COMMON_10200_PathGetSaveY(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA430 (#10203)
void __stdcall D2COMMON_10203_PATH_SetRotateFlag(D2DynamicPathStrc* pDynamicPath, BOOL bReset);
//D2Common.0x6FDAA460 (#10204)
void __stdcall D2COMMON_10204_PATH_ClearPoint2(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA480 (#10205)
void __stdcall PATH_SetStepNum(D2DynamicPathStrc* pDynamicPath, uint8_t nSteps);
//D2Common.0x6FDAA4B0 (#10206)
int __stdcall PATH_GetStepNum(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA4C0 (#10207)
void __stdcall D2Common_10207(D2DynamicPathStrc* pDynamicPath, char a2, char a3);
//D2Common.0x6FDAA4E0 (#10217)
void __stdcall PATH_SetDistance(D2DynamicPathStrc* pDynamicPath, int nDist);
//D2Common.0x6FDAA520 (#10218)
int __stdcall PATH_GetDistance(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA530 (#10219)
void __stdcall PATH_AddToDistance(D2DynamicPathStrc* pDynamicPath, int nAddition);
//D2Common.0x6FDAA570 (#10210)
int __stdcall PATH_GetUnitCollisionPattern(D2UnitStrc* pUnit);
//D2Common.0x6FDAA580 (#10211)
void __stdcall PATH_SetUnitCollisionPattern(D2UnitStrc* pUnit, int nCollisionPattern);
//D2Common.0x6FDAA5A0 (#10212)
void __stdcall D2COMMON_10212_PATH_SetMoveFlags(D2UnitStrc* pUnit, BOOL bSet);
//D2Common.0x6FDAA600 (#10213)
void __stdcall D2Common_10213(D2UnitStrc* pUnit);
//D2Common.0x6FDAA6A0 (#10220)
int __stdcall D2Common_10220(int a1, int a2, int a3, int a4);
//D2Common.0x6FDAA6D0 (#10221)
void __stdcall D2Common_10221(D2UnitStrc* pUnit);
//D2Common.0x6FDAA6F0 (#10237)
BOOL __stdcall D2Common_10237(D2UnitStrc* pUnit);
