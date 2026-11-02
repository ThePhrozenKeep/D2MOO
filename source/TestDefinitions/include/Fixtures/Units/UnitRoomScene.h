#pragma once


#include <deque>
#include <string>
#include <vector>

#include <doctest.h>

#include <TestDefinitions.h>

#include <D2StatList.h>
#include <D2States.h>
#include <Drlg/D2DrlgDrlg.h>
#include <Path/Path.h>
#include <Units/Units.h>


struct UnitRoomSceneRoomDesc
{
	int nSubtileX;
	int nSubtileY;
	int nSubtileWidth;
	int nSubtileHeight;
	int nLevelId = -1;					// -1 = no pDrlgRoom
	uint32_t dwFlags = 0;
	std::vector<int> adjacentRooms = {};	// Indices into the scene's rooms, stored in ppRoomList
};

struct UnitRoomSceneUnitDesc
{
	int nUnitType;
	int nClassId;
	int nRoom;							// Room stored in the unit's path, -1 = none
	int nX;
	int nY;
	int nClientY = 0;
	int nAlignment = -1;				// -1 = no stat list, otherwise value of STAT_ALIGNMENT in a STATE_ALIGNMENT stat list
	uint32_t dwFlags = 0;
	uint32_t dwAnimMode = 0;
	BOOL bHasPath = TRUE;				// FALSE = pDynamicPath/pStaticPath is nullptr
};

// A set of rooms and units which reference each other like they do in game.
// Two scenes built from the same descriptions are identical except for their addresses,
// so one can be passed to the D2MOO implementation and the other one to the original implementation.
struct UnitRoomScene
{
	struct RoomData
	{
		D2ActiveRoomStrc tRoom{};
		D2DrlgRoomStrc tDrlgRoom{};
		D2DrlgLevelStrc tLevel{};
		std::vector<D2ActiveRoomStrc*> adjacentRooms;
	};

	struct UnitData
	{
		D2UnitStrc tUnit{};
		D2DynamicPathStrc tDynamicPath{};
		D2StaticPathStrc tStaticPath{};
		D2StatListExStrc tStatListEx{};
		D2StatListStrc tAlignmentStatList{};
		D2StatStrc tAlignmentStat{};
	};

	D2DrlgActStrc tAct{};
	std::deque<RoomData> rooms;
	std::deque<UnitData> units;

	UnitRoomScene(const std::vector<UnitRoomSceneRoomDesc>& roomDescs, const std::vector<UnitRoomSceneUnitDesc>& unitDescs)
	{
		for (const auto& desc : roomDescs)
		{
			RoomData& room = rooms.emplace_back();
			room.tRoom.tCoords.nSubtileX = desc.nSubtileX;
			room.tRoom.tCoords.nSubtileY = desc.nSubtileY;
			room.tRoom.tCoords.nSubtileWidth = desc.nSubtileWidth;
			room.tRoom.tCoords.nSubtileHeight = desc.nSubtileHeight;
			room.tRoom.dwFlags = desc.dwFlags;
			room.tRoom.pAct = &tAct;

			if (desc.nLevelId >= 0)
			{
				room.tLevel.nLevelId = desc.nLevelId;
				room.tDrlgRoom.pLevel = &room.tLevel;
				room.tDrlgRoom.pRoom = &room.tRoom;
				room.tRoom.pDrlgRoom = &room.tDrlgRoom;
			}
		}

		for (size_t i = 0; i < roomDescs.size(); ++i)
		{
			RoomData& room = rooms[i];
			for (const int nAdjacentRoom : roomDescs[i].adjacentRooms)
			{
				room.adjacentRooms.push_back(&rooms[nAdjacentRoom].tRoom);
			}

			room.tRoom.ppRoomList = room.adjacentRooms.empty() ? nullptr : room.adjacentRooms.data();
			room.tRoom.nNumRooms = static_cast<int>(room.adjacentRooms.size());
		}

		for (size_t i = 0; i < unitDescs.size(); ++i)
		{
			const auto& desc = unitDescs[i];
			UnitData& unit = units.emplace_back();
			unit.tUnit.dwUnitType = desc.nUnitType;
			unit.tUnit.dwClassId = desc.nClassId;
			unit.tUnit.dwUnitId = static_cast<D2UnitGUID>(i + 1);
			unit.tUnit.dwAnimMode = desc.dwAnimMode;
			unit.tUnit.dwFlags = desc.dwFlags;

			D2ActiveRoomStrc* pRoom = desc.nRoom >= 0 ? &rooms[desc.nRoom].tRoom : nullptr;

			switch (desc.nUnitType)
			{
			case UNIT_OBJECT:
			case UNIT_ITEM:
			case UNIT_TILE:
				unit.tStaticPath.pRoom = pRoom;
				unit.tStaticPath.tGameCoords.nX = desc.nX;
				unit.tStaticPath.tGameCoords.nY = desc.nY;
				unit.tStaticPath.dwClientCoordX = desc.nX;
				unit.tStaticPath.dwClientCoordY = desc.nClientY;
				unit.tUnit.pStaticPath = desc.bHasPath ? &unit.tStaticPath : nullptr;
				break;

			default:
				unit.tDynamicPath.pRoom = pRoom;
				unit.tDynamicPath.tGameCoords.wPosX = static_cast<uint16_t>(desc.nX);
				unit.tDynamicPath.tGameCoords.wOffsetX = static_cast<uint16_t>(0x1234 + i);
				unit.tDynamicPath.tGameCoords.wPosY = static_cast<uint16_t>(desc.nY);
				unit.tDynamicPath.tGameCoords.wOffsetY = static_cast<uint16_t>(0x4321 + i);
				unit.tDynamicPath.dwClientCoordX = desc.nX;
				unit.tDynamicPath.dwClientCoordY = desc.nClientY;
				unit.tDynamicPath.dwPathPoints = 3;
				unit.tDynamicPath.pUnit = &unit.tUnit;
				unit.tUnit.pDynamicPath = desc.bHasPath ? &unit.tDynamicPath : nullptr;
				break;
			}

			if (desc.nAlignment >= 0)
			{
				unit.tAlignmentStat.nLayer = 0;
				unit.tAlignmentStat.nStat = STAT_ALIGNMENT;
				unit.tAlignmentStat.nValue = desc.nAlignment;

				unit.tAlignmentStatList.dwStateNo = STATE_ALIGNMENT;
				unit.tAlignmentStatList.Stats.pStat = &unit.tAlignmentStat;
				unit.tAlignmentStatList.Stats.nStatCount = 1;
				unit.tAlignmentStatList.Stats.nCapacity = 1;

				unit.tStatListEx.dwFlags = STATLIST_EXTENDED;
				unit.tStatListEx.pMyLastList = &unit.tAlignmentStatList;
				unit.tStatListEx.pOwner = &unit.tUnit;
				unit.tUnit.pStatListEx = &unit.tStatListEx;
			}
		}
	}

	UnitRoomScene(const UnitRoomScene&) = delete;
	UnitRoomScene& operator=(const UnitRoomScene&) = delete;

	D2ActiveRoomStrc* Room(int nRoom)
	{
		return nRoom >= 0 ? &rooms[nRoom].tRoom : nullptr;
	}

	D2UnitStrc* Unit(int nUnit)
	{
		return nUnit >= 0 ? &units[nUnit].tUnit : nullptr;
	}

	// Returns -1 for nullptr and -2 for units which are not part of the scene
	int IndexOf(const D2UnitStrc* pUnit) const
	{
		if (!pUnit)
		{
			return -1;
		}

		for (size_t i = 0; i < units.size(); ++i)
		{
			if (&units[i].tUnit == pUnit)
			{
				return static_cast<int>(i);
			}
		}

		return -2;
	}

	// Returns -1 for nullptr and -2 for rooms which are not part of the scene
	int IndexOf(const D2ActiveRoomStrc* pRoom) const
	{
		if (!pRoom)
		{
			return -1;
		}

		for (size_t i = 0; i < rooms.size(); ++i)
		{
			if (&rooms[i].tRoom == pRoom)
			{
				return static_cast<int>(i);
			}
		}

		return -2;
	}

	// Links the units into the unit list of the room, the first unit becomes pUnitFirst
	void LinkUnitList(int nRoom, const std::vector<int>& unitIndices)
	{
		D2UnitStrc** ppUnit = &rooms[nRoom].tRoom.pUnitFirst;
		for (const int nUnit : unitIndices)
		{
			*ppUnit = Unit(nUnit);
			ppUnit = &(*ppUnit)->pRoomNext;
		}
		*ppUnit = nullptr;
	}

	// Links the units into the update list of the room and sets UNITFLAG_ISLINKREFRESHMSG for them
	void LinkUpdateList(int nRoom, const std::vector<int>& unitIndices)
	{
		D2UnitStrc** ppUnit = &rooms[nRoom].tRoom.pUnitUpdate;
		for (const int nUnit : unitIndices)
		{
			*ppUnit = Unit(nUnit);
			(*ppUnit)->dwFlags |= UNITFLAG_ISLINKREFRESHMSG;
			ppUnit = &(*ppUnit)->pChangeNextUnit;
		}
		*ppUnit = nullptr;
	}

	std::vector<int> GetUnitList(int nRoom) const
	{
		std::vector<int> result;
		for (const D2UnitStrc* pUnit = rooms[nRoom].tRoom.pUnitFirst; pUnit && result.size() <= units.size(); pUnit = pUnit->pRoomNext)
		{
			result.push_back(IndexOf(pUnit));
		}
		return result;
	}

	std::vector<int> GetUpdateList(int nRoom) const
	{
		std::vector<int> result;
		for (const D2UnitStrc* pUnit = rooms[nRoom].tRoom.pUnitUpdate; pUnit && result.size() <= units.size(); pUnit = pUnit->pChangeNextUnit)
		{
			result.push_back(IndexOf(pUnit));
		}
		return result;
	}
};

// Compares every room and unit of both scenes, including the links which are not covered by the visitors
inline void check_scenes_eq(UnitRoomScene& moo_scene, UnitRoomScene& original_scene)
{
	REQUIRE_EQ(moo_scene.rooms.size(), original_scene.rooms.size());
	REQUIRE_EQ(moo_scene.units.size(), original_scene.units.size());

	MOO_CHECK_EQ(moo_scene.tAct, original_scene.tAct, "Comparing tAct");

	for (size_t i = 0; i < moo_scene.rooms.size(); ++i)
	{
		const std::string title = "Comparing room " + std::to_string(i);
		MOO_CHECK_EQ(moo_scene.rooms[i].tRoom, original_scene.rooms[i].tRoom, title.c_str());
		CHECK_MESSAGE(moo_scene.GetUnitList(static_cast<int>(i)) == original_scene.GetUnitList(static_cast<int>(i)), (title + " unit list"));
		CHECK_MESSAGE(moo_scene.GetUpdateList(static_cast<int>(i)) == original_scene.GetUpdateList(static_cast<int>(i)), (title + " update list"));
	}

	for (size_t i = 0; i < moo_scene.units.size(); ++i)
	{
		const std::string title = "Comparing unit " + std::to_string(i);
		auto& moo_unit = moo_scene.units[i];
		auto& original_unit = original_scene.units[i];
		MOO_CHECK_EQ(moo_unit.tUnit, original_unit.tUnit, title.c_str());
		MOO_CHECK_EQ(moo_unit.tDynamicPath, original_unit.tDynamicPath, title.c_str());
		MOO_CHECK_EQ(moo_unit.tStaticPath, original_unit.tStaticPath, title.c_str());

		// pRoomNext, pChangeNextUnit, tDynamicPath.pRoom, tDynamicPath.pPreviousRoom, tStaticPath.pRoom
		const auto get_links = [](const UnitRoomScene& scene, const UnitRoomScene::UnitData& unit) {
			return std::vector<int>{
				scene.IndexOf(unit.tUnit.pRoomNext),
				scene.IndexOf(unit.tUnit.pChangeNextUnit),
				scene.IndexOf(unit.tDynamicPath.pRoom),
				scene.IndexOf(unit.tDynamicPath.pPreviousRoom),
				scene.IndexOf(unit.tStaticPath.pRoom),
			};
		};
		CHECK_MESSAGE(get_links(moo_scene, moo_unit) == get_links(original_scene, original_unit), (title + " links"));
	}
}
