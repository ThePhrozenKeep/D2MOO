#pragma once

#include "CommonDefinitions.h"

struct D2ActiveRoomStrc;
struct D2UnitStrc;

#pragma pack(1)

enum D2PathConstants {
	PATH_NB_DIRECTIONS = 64,
	PATH_DIR_NULL = 255,
	PATH_MAX_STEPNUM = 20,
	PATH_MAXDESTSHIFT = 8, // Used in DRLG_OutFlr.cpp
};

enum D2PathTypes
{
	PATHTYPE_IDASTAR = 0,
	PATHTYPE_ASTAR = 1,
	PATHTYPE_TOWARD = 2,
	PATHTYPE_UNKNOWN_3 = 3,			// Same as PATHTYPE_TOWARD, unused ?
	PATHTYPE_MISSILE = 4,			// Requires PATH_MISSILE_MASK
	PATHTYPE_MON_CIRCLE_CW = 5,		// Clock-wise
	PATHTYPE_MON_CIRCLE_CCW = 6,	// Counter-clock-wise
	PATHTYPE_STRAIGHT = 7,			// Goes in a straight line except once close enough, will try to use AStar
	PATHTYPE_KNOCKBACK_SERVER = 8,
	PATHTYPE_LEAP = 9,
	PATHTYPE_CHARGEDBOLT = 10,		// Requires PATH_MISSILE_MASK
	PATHTYPE_KNOCKBACK_CLIENT = 11,
	PATHTYPE_BACKUP_TURN = 12,		// Turns when colliding before reaching destination.
	PATHTYPE_TOWARD_FINISH = 13,
	PATHTYPE_BLESSEDHAMMER = 14,	// Requires PATH_MISSILE_MASK
	PATHTYPE_WALL_FOLLOW = 15,		// PATH_WF
	PATHTYPE_MISSILE_STREAM = 16,	// Missile streams seems to be unused in the game even though the code exists?
	PATHTYPE_UNKNOWN_17 = 17,		// Unused, stubbed?

	PATHTYPE_COUNT = 18
};

enum D2PathFlags : uint32_t {
	PATH_UNKNOWN_FLAG_0x00001 = 0x00001, // Set when rider and mount were in different rooms
	PATH_CURRENT_ROOM_INVALID = 0x00002,
	PATH_UNUSED_FLAG_0x00004  = 0x00004, // This is unused
	PATH_UNKNOWN_FLAG_0x00008 = 0x00008, // Set when rider and mount have the same position
	PATH_UNKNOWN_FLAG_0x00010 = 0x00010,
	PATH_UNKNOWN_FLAG_0x00020 = 0x00020, // Only set on the client side
	PATH_UNKNOWN_FLAG_0x00040 = 0x00040,
	PATH_UNKNOWN_FLAG_0x00200 = 0x00200,
	PATH_UNKNOWN_FLAG_0x00800 = 0x00800,
	PATH_UNKNOWN_FLAG_0x01000 = 0x01000,
	PATH_UNKNOWN_FLAG_0x02000 = 0x02000,
	PATH_UNKNOWN_FLAG_0x04000 = 0x04000,
	PATH_WITH_PREVIOUS_VELOCITY = 0x08000,
	PATH_UNKNOWN_FLAG_0x10000 = 0x10000,
	PATH_SAVE_STEPS_MASK      = 0x20000,
	PATH_MISSILE_MASK         = 0x40000,
};

struct D2PathPointStrc
{
	uint16_t X;
	uint16_t Y;

	bool operator==(const D2PathPointStrc& other) const { return X == other.X && Y == other.Y; }
	bool operator!=(const D2PathPointStrc& other) const { return !(*this == other); }
	int SquaredDistance(const D2PathPointStrc& other) const
	{
		const int nDiffX = other.X - X;
		const int nDiffY = other.Y - Y;
		return nDiffX * nDiffX + nDiffY * nDiffY;
	}
};

// Represents a position with 16bit fixed point precision
union D2FP32_16
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
	D2PathPointStrc ToPathPoint() const { return { wPosX,wPosY }; }
};

struct D2DynamicPathStrc
{
	static const size_t MAXPATHLEN = 78;
	static const size_t PATH_MAX_STEP_LEN = 10;

	D2FP32_16 tGameCoords;						//0x00
	int32_t dwClientCoordX;						//0x08
	int32_t dwClientCoordY;						//0x0C
	D2PathPointStrc tTargetCoord;				//0x10
	D2PathPointStrc tPrevTargetCoord;			//0x14
	D2PathPointStrc tFinalTargetCoord;			//0x18
	D2ActiveRoomStrc* pRoom;					//0x1C
	D2ActiveRoomStrc* pPreviousRoom;			//0x20
	int32_t dwCurrentPointIdx;					//0x24
	int32_t dwPathPoints;						//0x28
	void* unk0x2C;								//0x2C
	D2UnitStrc* pUnit;							//0x30
	uint32_t dwFlags;							//0x34 D2PathFlags
	uint32_t unk0x38;							//0x38
	uint32_t dwPathType;						//0x3C
	uint32_t dwPrevPathType;					//0x40
	uint32_t dwUnitSize;						//0x44
	uint32_t dwCollisionPattern;				//0x48
	uint32_t nFootprintCollisionMask;			//0x4C
	uint32_t nMoveTestCollisionMask;			//0x50
	uint16_t nCollidedWithMask;					//0x54
	uint16_t padding0x56;						//0x56
	D2UnitStrc* pTargetUnit;					//0x58
	uint32_t dwTargetType;						//0x5C
	uint32_t dwTargetId;						//0x60
	uint8_t nDirection;							//0x64
	uint8_t nNewDirection;						//0x65
	uint8_t nDiffDirection;						//0x66
	uint8_t unk0x67;							//0x67
	uint8_t dwSpeed;							//0x68
	uint8_t unk0x69;							//0x69
	D2CoordStrc tDirectionVector;				//0x6A
	D2CoordStrc tVelocityVector;				//0x72
	char padding0x7A[2];						//0x7A
	int32_t dwVelocity;							//0x7C
	uint32_t nPreviousVelocity;					//0x80
	int32_t dwMaxVelocity;						//0x84
	int32_t dwAcceleration;						//0x88
	int32_t dwAccelerationsCounter;				//0x8C Incremented each time we want to accelerate. Used to accelerate only 1 out 5 times
	uint8_t nDist;								//0x90
	uint8_t nDistMax;							//0x91
	uint8_t nIDAStarInitFScore;					//0x92 // Used only with PATHTYPE_IDASTAR
	uint8_t nStepNum;							//0x93
	uint8_t nDistance;							//0x94
	char padding0x95[3];						//0x95
	int32_t nDirOffset;							//0x98 See gaOffsetForPathType
	D2PathPointStrc PathPoints[MAXPATHLEN];		//0x9C
	int32_t nSavedStepsCount;					//0x1D4
	D2PathPointStrc SavedSteps[PATH_MAX_STEP_LEN];//0x1D8
};

struct D2PathInfoStrc
{
	D2PathPointStrc tStartCoord;				//0x00
	D2PathPointStrc tTargetCoord;				//0x04
	D2ActiveRoomStrc* pStartRoom;				//0x08
	D2ActiveRoomStrc* pTargetRoom;				//0x0C
	int32_t field_10;							//0x10
	uint8_t field_14;							//0x14
	uint8_t field_15;							//0x15
	uint16_t field_16;							//0x16
	int32_t nDistMax;							//0x18
	uint8_t nMinimumFScoreToEvaluate;			//0x1C for IDAStar: max heuristic value or max FScore?
	uint8_t field_1D;							//0x1D
	uint16_t field_1E;							//0x1E
	int32_t nPathType;							//0x20
	int32_t nUnitSize;							//0x24
	int32_t nCollisionPattern;					//0x28
	int32_t nCollisionMask;						//0x2C
	union
	{
		D2DynamicPathStrc* pDynamicPath;	//0x30 - not sure yet
		D2PathInfoStrc* pNext;				//0x30
	};
};

struct D2StaticPathStrc
{
	D2ActiveRoomStrc* pRoom;				//0x00
	int32_t dwClientCoordX;					//0x04
	int32_t dwClientCoordY;					//0x08
	D2CoordStrc tGameCoords;				//0x0C
	uint32_t unk0x14[2];					//0x14
	uint8_t nDirection;						//0x1C
	uint8_t bRoomNeedsUpdate;				//0x1D
	uint8_t unk0x1E[2];						//0x1E
};

struct D2MapAIPathPositionStrc
{
	int32_t nMapAIAction;					//0x00
	int32_t nX;								//0x04
	int32_t nY;								//0x08
};

struct D2MapAIStrc
{
	int32_t nPathNodes;						//0x00
	D2MapAIPathPositionStrc* pPosition;		//0x04
};

#pragma pack()

// Path "precise" positions are encoded using 16bits fixed point
inline uint32_t PATH_ToFP16Corner(uint16_t value) {
	return (value << 16);
}
inline uint32_t PATH_ToFP16Center(uint16_t value) {
	return (value << 16) + (1 << 15);
}
// Called FRACTIONAL_TO_GAMESQUARE in the original game
inline uint16_t PATH_FromFP16(uint32_t value) {
	return (value >> 16) & 0xFFFF;
}
inline uint32_t PATH_FP16FitToCenter(uint32_t value)
{
	return (value & 0xFFFF0000) + (1 << 15);
}

// Helper functions
inline uint8_t PATH_NormalizeDirection(uint8_t nDirection) { return nDirection % PATH_NB_DIRECTIONS; }
void PATH_UpdateClientCoords(D2DynamicPathStrc* pDynamicPath);

//D2Common.0x6FDA8220
void __fastcall sub_6FDA8220(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA82A0 (#10141)
D2COMMON_DLL_DECL void __stdcall PATH_GetClientCoordsVelocity(D2UnitStrc* pUnit, int* pX, int* pY);
//D2Common.0x6FDA8320 (#10222)
D2COMMON_DLL_DECL void __stdcall PATH_AddCollisionFootprintForUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDA8450 (#10223)
D2COMMON_DLL_DECL BOOL __stdcall PATH_RemoveCollisionFootprintForUnit(D2UnitStrc* pUnit, BOOL bForce);
//1.00:  D2Common.0x10059110 (#10141)
//1.10f: D2Common.0x6FDA8600 (#10142)
//1.13c: D2Common.0x6FD86230 (#10334)
D2COMMON_DLL_DECL int __stdcall D2Common_10142(D2DynamicPathStrc* pPath, D2UnitStrc* pUnit, int bAllowInTown);
//D2Common.0x6FDA8E30
int __fastcall PATH_ComputePathClassicMissile(D2DynamicPathStrc* pDynamicPath, D2UnitStrc* pUnit);
//D2Common.0x6FDA8FE0
void __fastcall PATH_FindValidTargetCoordsByMovingOrthogonally(D2PathInfoStrc* pPathInfo);
//D2Common.0x6FDA90C0
uint8_t __fastcall PATH_AdvanceToDoor(D2PathInfoStrc* pPathInfo);
//D2Common.0x6FDA9190 (#10156)
D2COMMON_DLL_DECL void __stdcall PATH_FreeDynamicPath(void* pMemPool, D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA91B0 (#11282)
D2COMMON_DLL_DECL int __stdcall PATH_GetCollisionPatternFromMonStats2Txt(int nMonsterId);
//D2Common.0x6FDA9250 (#11281)
D2COMMON_DLL_DECL int __stdcall D2Common_11281_CollisionPatternFromSize(D2UnitStrc* pUnit, int nSize);
//D2Common.0x6FDA92F0 (#10214)
D2COMMON_DLL_DECL void __stdcall D2Common_10214(D2UnitStrc* pUnit);
//D2Common.0x6FDA9480 (#10152)
D2COMMON_DLL_DECL void __stdcall PATH_AllocDynamicPath(void* pMemPool, D2ActiveRoomStrc* pRoom, int nX, int nY, D2UnitStrc* pUnit, BOOL bSetFlag);
//D2Common.0x6FDA9720
void __fastcall sub_6FDA9720(D2DynamicPathStrc* pDynamicPath, uint8_t nDirection);
//D2Common.0x6FDA9770 (#10193)
D2COMMON_DLL_DECL void __stdcall D2COMMON_10193_PATH_AdjustDirection(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA97C0 (#10216)
D2COMMON_DLL_DECL void __stdcall D2Common_10216(D2DynamicPathStrc* pDynamicPath, int nX, int nY, int a4);
//D2Common.0x6FDA9850 (#10228)
D2COMMON_DLL_DECL void __stdcall D2Common_10228(D2UnitStrc* pUnit);
//D2Common.0x6FDA9870 (#10143)
D2COMMON_DLL_DECL void __stdcall PATH_SetUnitDeadCollision(D2UnitStrc* pUnit, BOOL bForGameLogic);
//D2Common.0x6FDA98F0 (#10144)
D2COMMON_DLL_DECL void __stdcall PATH_SetUnitAliveCollision(D2UnitStrc* pUnit, BOOL bForGameLogic);
//D2Common.0x6FDA9A70 (#10146)
D2COMMON_DLL_DECL void __stdcall PATH_SetVelocity(D2DynamicPathStrc* pDynamicPath, int nVelocity, const char* szFile, int nLine);
//D2Common.0x6FDA9AB0 (#10147)
D2COMMON_DLL_DECL int __stdcall PATH_GetVelocity(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9AC0 (#10148)
D2COMMON_DLL_DECL void __stdcall PATH_SetMaxVelocity(D2DynamicPathStrc* pDynamicPath, int nMaxVelocity);
//D2Common.0x6FDA9AE0 (#10149)
D2COMMON_DLL_DECL int __stdcall PATH_GetMaxVelocity(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9AF0 (#10150)
D2COMMON_DLL_DECL void __stdcall PATH_SetAcceleration(D2DynamicPathStrc* pDynamicPath, int nAcceleration);
//D2Common.0x6FDA9B10 (#10151)
D2COMMON_DLL_DECL int __stdcall PATH_GetAcceleration(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9B20 (#10153)
D2COMMON_DLL_DECL void __stdcall D2Common_10153(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9B40 (#10208)
D2COMMON_DLL_DECL void __stdcall D2COMMON_10208_PathSetPathingFlag(D2DynamicPathStrc* pDynamicPath, BOOL bSet);
//D2Common.0x6FDA9B70 (#10209)
D2COMMON_DLL_DECL BOOL __stdcall D2COMMON_10209_PathCheckPathingFlag(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9B80 (#10154)
D2COMMON_DLL_DECL int __stdcall PATH_GetNumberOfPathPoints(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9B90 (#11291)
D2COMMON_DLL_DECL void __stdcall PATH_SetNumberOfPathPoints(D2DynamicPathStrc* pDynamicPath, int a2);
//D2Common.0x6FDA9BC0 (#10155)
D2COMMON_DLL_DECL int __stdcall D2Common_10155(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9BD0 (#10157)
D2COMMON_DLL_DECL int __stdcall PATH_GetPathPoints(D2DynamicPathStrc* pDynamicPath, D2PathPointStrc** ppPathPoints);
//D2Common.0x6FDA9BF0 (#10158)
D2COMMON_DLL_DECL uint8_t __stdcall PATH_GetDirection(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9C10 (#10159)
D2COMMON_DLL_DECL uint8_t __stdcall PATH_GetNewDirection(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9C20 (#10160)
D2COMMON_DLL_DECL void __stdcall D2COMMON_10160_PathUpdateDirection(D2DynamicPathStrc* pDynamicPath, uint8_t nDirection);
//D2Common.0x6FDA9C90 (#10161)
D2COMMON_DLL_DECL void __stdcall PATH_SetDirection(D2DynamicPathStrc* pDynamicPath, uint8_t nDirection);
//D2Common.0x6FDA9CB0 (#10162)
D2COMMON_DLL_DECL int __stdcall PATH_GetXPosition(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9CF0 (#10163)
D2COMMON_DLL_DECL int __stdcall PATH_GetYPosition(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9D30 (#10194)
D2COMMON_DLL_DECL int __stdcall PATH_GetPrecisionX(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9D60 (#10195)
D2COMMON_DLL_DECL int __stdcall PATH_GetPrecisionY(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9D90 (#10196)
D2COMMON_DLL_DECL void __stdcall PATH_SetPrecisionX(D2DynamicPathStrc* pDynamicPath, int nPrecisionX);
//D2Common.0x6FDA9DA0 (#10197)
D2COMMON_DLL_DECL void __stdcall PATH_SetPrecisionY(D2DynamicPathStrc* pDynamicPath, int nPrecisionY);
//D2Common.0x6FDA9DB0 (#10164)
D2COMMON_DLL_DECL int __stdcall PATH_GetClientCoordX(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDC3CE0 (#10165)
D2COMMON_DLL_DECL int __stdcall PATH_GetClientCoordY(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9DC0
void __fastcall PATH_SetClientCoordX(D2DynamicPathStrc* pDynamicPath, int nTargetX);
//D2Common.0x6FDA9DD0
void __fastcall PATH_SetClientCoordY(D2DynamicPathStrc* pDynamicPath, int nTargetY);
//D2Common.0x6FDA9DE0 (#10175)
D2COMMON_DLL_DECL int __stdcall D2COMMON_10175_PathGetFirstPointX(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9DF0 (#10176)
D2COMMON_DLL_DECL int __stdcall D2COMMON_10176_PathGetFirstPointY(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9E00 (#10224)
D2COMMON_DLL_DECL int __stdcall D2Common_10224(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9E10 (#10225)
D2COMMON_DLL_DECL int __stdcall D2Common_10225(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9E20 (#10177)
D2COMMON_DLL_DECL int __stdcall D2COMMON_10177_PATH_GetLastPointX(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9E40 (#10178)
D2COMMON_DLL_DECL int __stdcall D2COMMON_10178_PATH_GetLastPointY(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDB9C10 (#10166)
D2COMMON_DLL_DECL D2ActiveRoomStrc* __stdcall PATH_GetRoom(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9E60 (#10167)
D2COMMON_DLL_DECL void __stdcall PATH_SetRoom(D2DynamicPathStrc* pDynamicPath, D2ActiveRoomStrc* pRoom);
//D2Common.0x6FDA9E70 (#10168)
D2COMMON_DLL_DECL D2ActiveRoomStrc* __stdcall PATH_GetNextRoom(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9E80 (#10169)
D2COMMON_DLL_DECL void __stdcall PATH_ClearNextRoom(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9E90 (#10170)
D2COMMON_DLL_DECL void __stdcall D2COMMON_10170_PathSetTargetPos(D2DynamicPathStrc* pDynamicPath, int nX, int nY);
//D2Common.0x6FDA9EC0 (#10172)
D2COMMON_DLL_DECL BOOL __stdcall PATH_IsCurrentRoomInvalid(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9ED0 (#10173)
D2COMMON_DLL_DECL void __stdcall PATH_SetCurrentRoomInvalid(D2DynamicPathStrc* pDynamicPath, BOOL bSet);
//D2Common.0x6FDA9F00 (#10145)
D2COMMON_DLL_DECL void __stdcall PATH_SetUnusedFlag_0x00004(D2DynamicPathStrc* pDynamicPath, BOOL bSet);
//D2Common.0x6FDA9F30 (#10174)
D2COMMON_DLL_DECL BOOL __stdcall PATH_GetUnusedFlag_0x00004(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9F40 (#10179)
D2COMMON_DLL_DECL void __stdcall PATH_SetTargetUnit(D2DynamicPathStrc* pDynamicPath, D2UnitStrc* pUnit);
//D2Common.0x6FDA9F60 (#10171)
D2COMMON_DLL_DECL void __stdcall PATH_GetTargetTypeAndGUID(D2DynamicPathStrc* pDynamicPath, int* pTargetType, D2UnitGUID* pTargetGUID);
//D2Common.0x6FDA9FA0 (#10180)
D2COMMON_DLL_DECL D2UnitStrc* __stdcall PATH_GetTargetUnit(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9FC0 (#10181)
D2COMMON_DLL_DECL int __stdcall PATH_GetFootprintCollisionMask(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDA9FE0 (#10182)
D2COMMON_DLL_DECL void __stdcall PATH_SetFootprintCollisionMask(D2DynamicPathStrc* pDynamicPath, int nCollisionMask);
//D2Common.0x6FDAA0C0 (#10183)
D2COMMON_DLL_DECL int __stdcall PATH_GetMoveTestCollisionMask(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA0D0 (#10184)
D2COMMON_DLL_DECL void __stdcall PATH_SetMoveTestCollisionMask(D2DynamicPathStrc* pDynamicPath, int nCollisionMask);
//D2Common.0x6FDAA0E0 (#10185)
D2COMMON_DLL_DECL void __stdcall PATH_SetType(D2DynamicPathStrc* pDynamicPath, int nPathType);
//D2Common.0x6FDAA1E0 (#10186)
D2COMMON_DLL_DECL void __stdcall PATH_ResetToPreviousType(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA240 (#10187)
D2COMMON_DLL_DECL int __stdcall PATH_GetType(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA250 (#10190)
D2COMMON_DLL_DECL void __stdcall D2COMMON_10190_PATH_SetDistance(D2DynamicPathStrc* pDynamicPath, uint8_t nDistance);
//D2Common.0x6FDAA270 (#10191)
D2COMMON_DLL_DECL uint8_t __stdcall D2COMMON_10191_PATH_GetDistance(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA280 (#10188)
D2COMMON_DLL_DECL void __stdcall PATH_SetNewDistance(D2DynamicPathStrc* pDynamicPath, uint8_t nNewDistance);
//D2Common.0x6FDAA2B0 (#10189)
D2COMMON_DLL_DECL uint8_t __stdcall PATH_GetMaxDistance(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA2C0 (#10201)
D2COMMON_DLL_DECL uint16_t __stdcall D2Common_10201(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA300 (#10202)
D2COMMON_DLL_DECL uint16_t __stdcall D2Common_10202(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA310 (#10192)
D2COMMON_DLL_DECL void __stdcall PATH_SetIDAStarInitFScore(D2DynamicPathStrc* pDynamicPath, int nIDAStarInitFScore);
//D2Common.0x6FDAA350 (#10198)
D2COMMON_DLL_DECL int __stdcall D2COMMON_10198_PathGetSaveStep(D2DynamicPathStrc* pDynamicPath, D2PathPointStrc** ppPathPoints);
//D2Common.0x6FDAA390 (#10199)
D2COMMON_DLL_DECL int __stdcall D2COMMON_10199_PathGetSaveX(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA3E0 (#10200)
D2COMMON_DLL_DECL int __stdcall D2COMMON_10200_PathGetSaveY(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA430 (#10203)
D2COMMON_DLL_DECL void __stdcall D2COMMON_10203_PATH_SetRotateFlag(D2DynamicPathStrc* pDynamicPath, BOOL bReset);
//D2Common.0x6FDAA460 (#10204)
D2COMMON_DLL_DECL void __stdcall D2COMMON_10204_PATH_ClearPoint2(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA480 (#10205)
D2COMMON_DLL_DECL void __stdcall PATH_SetStepNum(D2DynamicPathStrc* pDynamicPath, uint8_t nSteps);
//D2Common.0x6FDAA4B0 (#10206)
D2COMMON_DLL_DECL int __stdcall PATH_GetStepNum(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA4C0 (#10207)
D2COMMON_DLL_DECL void __stdcall D2Common_10207(D2DynamicPathStrc* pDynamicPath, char a2, char a3);
//D2Common.0x6FDAA4E0 (#10217)
D2COMMON_DLL_DECL void __stdcall PATH_SetDistance(D2DynamicPathStrc* pDynamicPath, int nDist);
//D2Common.0x6FDAA520 (#10218)
D2COMMON_DLL_DECL int __stdcall PATH_GetDistance(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAA530 (#10219)
D2COMMON_DLL_DECL void __stdcall PATH_AddToDistance(D2DynamicPathStrc* pDynamicPath, int nAddition);
//D2Common.0x6FDAA570 (#10210)
D2COMMON_DLL_DECL int __stdcall PATH_GetUnitCollisionPattern(D2UnitStrc* pUnit);
//D2Common.0x6FDAA580 (#10211)
D2COMMON_DLL_DECL void __stdcall PATH_SetUnitCollisionPattern(D2UnitStrc* pUnit, int nCollisionPattern);
//D2Common.0x6FDAA5A0 (#10212)
D2COMMON_DLL_DECL void __stdcall D2COMMON_10212_PATH_SetMoveFlags(D2UnitStrc* pUnit, BOOL bSet);
//D2Common.0x6FDAA600 (#10213)
D2COMMON_DLL_DECL void __stdcall D2Common_10213(D2UnitStrc* pUnit);
//D2Common.0x6FDAA6A0 (#10220)
D2COMMON_DLL_DECL int __stdcall PATH_ComputeSquaredDistance(int nX1, int nY1, int nX2, int nY2);
//D2Common.0x6FDAA6D0 (#10221)
D2COMMON_DLL_DECL void __stdcall PATH_AddCollisionFootprintForOptionalUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDAA6F0 (#10237)
D2COMMON_DLL_DECL BOOL __stdcall D2Common_10237(D2UnitStrc* pUnit);
