#include <D2CommonTestDefines.h>

#ifdef STATLIST_TESTS

#include <doctest.h>

#include <Windows.h>

#include <algorithm>
#include <cstdarg>
#include <filesystem>
#include <random>
#include <vector>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2Inventory.h>
#include <D2StatList.h>
#include <D2States.h>
#include <GAME/Game.h>
#include <Units/Item.h>
#include <Units/Units.h>

#include <Fixtures/DataTbls/Fixtures.h>
#include <Fixtures/DataTbls/ItemStatCostTxtOpStatsFixture.h>


// Records the calls made to the callbacks which are passed to the stat list functions, so that the calls of both implementations can be compared.
// Units are identified by their id, as the pointers differ between both implementations.
struct StatListCallbackRecord
{
	int nCallback;
	uint32_t nUnitId1;
	uint32_t nUnitId2;
	int nArg1;
	int nArg2;
	int nArg3;
	int nArg4;
};

BEGIN_VISIT(StatListCallbackRecord)
	FIELD(nCallback)
	FIELD(nUnitId1)
	FIELD(nUnitId2)
	FIELD(nArg1)
	FIELD(nArg2)
	FIELD(nArg3)
	FIELD(nArg4)
END_VISIT()


DYNAMIC_ARRAY_TYPE(StatListCallbackRecord)
DYNAMIC_ARRAY_TYPE(D2StatStrc)
DYNAMIC_ARRAY_TYPE(uint32_t)


static std::vector<StatListCallbackRecord>* gpStatListCallbackRecords = nullptr;
static D2GameStrc* const gpFakeGame = reinterpret_cast<D2GameStrc*>(static_cast<uintptr_t>(0x6A3E0000));

static uint32_t GetUnitIdForRecord(const D2UnitStrc* pUnit)
{
	return pUnit ? pUnit->dwUnitId : static_cast<uint32_t>(-1);
}

static void __fastcall RecordStatValueChange(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pUnit, int32_t nLayer_StatId, int32_t nPreviousValue, int32_t nNewValue)
{
	if (gpStatListCallbackRecords)
	{
		gpStatListCallbackRecords->push_back({ 0, GetUnitIdForRecord(pOwner), GetUnitIdForRecord(pUnit), static_cast<int>(reinterpret_cast<uintptr_t>(pGame)), nLayer_StatId, nPreviousValue, nNewValue });
	}
}

static void __fastcall RecordStatRemove(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList)
{
	if (gpStatListCallbackRecords)
	{
		gpStatListCallbackRecords->push_back({ 1, GetUnitIdForRecord(pUnit), static_cast<uint32_t>(-1), nState, static_cast<int>(pStatList->dwStateNo), static_cast<int>(pStatList->dwFlags), pStatList->Stats.nStatCount });
	}
}

static void __fastcall RecordStat(D2UnitStrc* pUnit1, int nStatId, int nValue, D2UnitStrc* pUnit2)
{
	if (gpStatListCallbackRecords)
	{
		gpStatListCallbackRecords->push_back({ 2, GetUnitIdForRecord(pUnit1), GetUnitIdForRecord(pUnit2), nStatId, nValue, 0, 0 });
	}
}


// The functions of the original D2Common.dll which are used to set up identical stat lists for both implementations
struct StatListNativeApi
{
	decltype(&STATLIST_AllocStatList) AllocStatList;
	decltype(&STATLIST_AllocStatListEx) AllocStatListEx;
	decltype(&STATLIST_FreeStatList) FreeStatList;
	decltype(&STATLIST_FreeStatListEx) FreeStatListEx;
	decltype(&STATLIST_SetStat) SetStat;
	decltype(&STATLIST_SetState) SetState;
	decltype(&STATLIST_SetStatRemoveCallback) SetStatRemoveCallback;
	decltype(&D2COMMON_10475_PostStatToStatList) PostStatToStatList;
};

struct StatListWorldConfig
{
	D2C_UnitTypes nUnitType = UNIT_PLAYER;
	BOOL bWithStatListEx = TRUE;
	int nBaseStatCount = 8;
	int nItemCount = 2;
	int nListCount = 6;
	int nStatsPerList = 4;
	int nAlignment = -1;
	BOOL bAttachAll = FALSE;
	BOOL bByTimeStats = FALSE;
};

// A unit with an extended stat list, some items with extended stat lists and some child stat lists.
// Everything is allocated through the original D2Common.dll, so that both implementations can free and reallocate the stats.
struct StatListWorld
{
	static constexpr int max_items = 3;

	const StatListNativeApi* native = nullptr;
	D2UnitStrc unit{};
	D2UnitStrc items[max_items]{};
	D2ItemDataStrc item_data[max_items]{};
	std::vector<D2StatListStrc*> lists;
	std::vector<D2StatListStrc*> detached_lists;
	std::vector<StatListCallbackRecord> records;

	StatListWorld() = default;
	StatListWorld(const StatListWorld&) = delete;
	StatListWorld& operator=(const StatListWorld&) = delete;

	~StatListWorld()
	{
		if (!native)
		{
			return;
		}

		gpStatListCallbackRecords = nullptr;

		for (auto pStatList : detached_lists)
		{
			if (pStatList)
			{
				native->FreeStatList(pStatList);
			}
		}

		if (unit.pStatListEx && STATLIST_IsExtended(unit.pStatListEx))
		{
			while (D2StatListStrc* pStatList = unit.pStatListEx->pMyStats)
			{
				if (STATLIST_IsExtended(pStatList))
				{
					break;
				}
				native->FreeStatList(pStatList);
			}
		}

		for (auto& item : items)
		{
			if (item.pStatListEx)
			{
				native->FreeStatListEx(&item);
			}
		}

		if (unit.pStatListEx)
		{
			native->FreeStatListEx(&unit);
		}
	}

	// All stat lists that are still alive, in a stable order
	std::vector<D2StatListStrc*> get_lists()
	{
		std::vector<D2StatListStrc*> result;
		if (unit.pStatListEx)
		{
			result.push_back(unit.pStatListEx);
		}
		for (auto& item : items)
		{
			if (item.pStatListEx)
			{
				result.push_back(item.pStatListEx);
			}
		}
		for (auto pStatList : lists)
		{
			if (pStatList)
			{
				result.push_back(pStatList);
			}
		}
		for (auto pStatList : detached_lists)
		{
			if (pStatList)
			{
				result.push_back(pStatList);
			}
		}
		return result;
	}

	std::vector<D2StatListExStrc*> get_extended_lists()
	{
		std::vector<D2StatListExStrc*> result;
		for (auto pStatList : get_lists())
		{
			if (D2StatListExStrc* pStatListEx = STATLIST_StatListExCast(pStatList))
			{
				result.push_back(pStatListEx);
			}
		}
		return result;
	}

	std::vector<D2StatsArrayStrc*> get_stats_arrays()
	{
		std::vector<D2StatsArrayStrc*> result;
		for (auto pStatList : get_lists())
		{
			result.push_back(&pStatList->Stats);
			if (D2StatListExStrc* pStatListEx = STATLIST_StatListExCast(pStatList))
			{
				result.push_back(&pStatListEx->FullStats);
			}
		}
		return result;
	}

	std::vector<D2UnitStrc*> get_units()
	{
		std::vector<D2UnitStrc*> result{ &unit };
		for (auto& item : items)
		{
			result.push_back(&item);
		}
		return result;
	}

	int find_list_index(const D2StatListStrc* pStatList)
	{
		if (!pStatList)
		{
			return -1;
		}
		const auto all_lists = get_lists();
		const auto it = std::find(all_lists.begin(), all_lists.end(), pStatList);
		return it != all_lists.end() ? static_cast<int>(it - all_lists.begin()) : -2;
	}

	// Forget about a stat list which has been freed
	void forget_list(const D2StatListStrc* pStatList)
	{
		std::replace(lists.begin(), lists.end(), const_cast<D2StatListStrc*>(pStatList), static_cast<D2StatListStrc*>(nullptr));
		std::replace(detached_lists.begin(), detached_lists.end(), const_cast<D2StatListStrc*>(pStatList), static_cast<D2StatListStrc*>(nullptr));
	}
};

// Calls the function while recording the callbacks into the records of the world
template<typename Function>
static decltype(auto) with_callbacks(StatListWorld& world, Function&& function)
{
	struct ResetRecords
	{
		~ResetRecords() { gpStatListCallbackRecords = nullptr; }
	} reset_records;

	gpStatListCallbackRecords = &world.records;
	return function();
}


struct StatListFixture : CharStatsTxtFixture<StatesTxtFixture<ItemStatCostTxtOpStatsFixture<NoOpFixture>>>
{
	StatListNativeApi native{};
	// Always present in a world, so that the min value and op stat paths do not depend on the random seed
	int fmin_stat_id = -1;
	int three_op_stats_stat_id = -1;
	// Stats changed by the op 6/7 (by time) stats
	std::vector<int> by_time_op_stat_ids;

	StatListFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		native.AllocStatList = reinterpret_cast<decltype(native.AllocStatList)>(d2common_base + 0x00077140);
		native.AllocStatListEx = reinterpret_cast<decltype(native.AllocStatListEx)>(d2common_base + 0x00077190);
		native.FreeStatList = reinterpret_cast<decltype(native.FreeStatList)>(d2common_base + 0x00077030);
		native.FreeStatListEx = reinterpret_cast<decltype(native.FreeStatListEx)>(d2common_base + 0x00077110);
		native.SetStat = reinterpret_cast<decltype(native.SetStat)>(d2common_base + 0x000777B0);
		native.SetState = reinterpret_cast<decltype(native.SetState)>(d2common_base + 0x000772C0);
		native.SetStatRemoveCallback = reinterpret_cast<decltype(native.SetStatRemoveCallback)>(d2common_base + 0x00078CA0);
		native.PostStatToStatList = reinterpret_cast<decltype(native.PostStatToStatList)>(d2common_base + 0x00077340);

		for (int nStatId = 0; nStatId < itemstatcost_record_count; ++nStatId)
		{
			const D2ItemStatCostTxt& record = sgptDataTables->pItemStatCostTxt[nStatId];
			if (fmin_stat_id < 0 && (record.dwItemStatFlags & ITEMSTATCOSTFLAG_FMIN) && !record.bHasOpApplyingToItem)
			{
				fmin_stat_id = nStatId;
			}
			if (three_op_stats_stat_id < 0 && record.wOpStat[2] < itemstatcost_record_count && !record.bHasOpApplyingToItem)
			{
				three_op_stats_stat_id = nStatId;
			}
			if (record.nOp >= 6 && record.nOp <= 7)
			{
				for (const uint16_t wOpStat : record.wOpStat)
				{
					if (wOpStat < itemstatcost_record_count)
					{
						by_time_op_stat_ids.push_back(wOpStat);
					}
				}
			}
		}
		REQUIRE(fmin_stat_id >= 0);
		REQUIRE(three_op_stats_stat_id >= 0);
		REQUIRE(!by_time_op_stat_ids.empty());
	}

	// Gives the stats changed by the by time stats a value that is out of date, so that they have to be recomputed
	void make_by_time_op_stats_stale(StatListWorld& world, int nValue) const
	{
		D2StatListExStrc* pStatListEx = STATLIST_StatListExCast(world.unit.pStatListEx);
		if (!pStatListEx)
		{
			return;
		}

		for (D2StatStrc* pStat = pStatListEx->FullStats.pStat; pStat < &pStatListEx->FullStats.pStat[pStatListEx->FullStats.nStatCount]; ++pStat)
		{
			if (std::find(by_time_op_stat_ids.begin(), by_time_op_stat_ids.end(), pStat->nStat) != by_time_op_stat_ids.end())
			{
				pStat->nValue = nValue;
			}
		}
	}

	int flag_count() const
	{
		return (states_record_count + 31) / 32;
	}

	int random_stat_id(std::mt19937& rng) const
	{
		static constexpr int interesting_stats[] = {
			STAT_STRENGTH, STAT_ENERGY, STAT_DEXTERITY, STAT_VITALITY, STAT_HITPOINTS, STAT_MAXHP, STAT_MANA, STAT_MAXMANA, STAT_STAMINA, STAT_MAXSTAMINA,
			STAT_LEVEL, STAT_ITEM_ARMOR_PERCENT, STAT_ITEM_MAXDAMAGE_PERCENT, STAT_ITEM_MINDAMAGE_PERCENT, STAT_TOHIT, STAT_MINDAMAGE, STAT_MAXDAMAGE,
			STAT_SECONDARY_MINDAMAGE, STAT_SECONDARY_MAXDAMAGE, STAT_ARMORCLASS, STAT_FIRERESIST, STAT_MAXDURABILITY, STAT_ITEM_MAXDURABILITY_PERCENT,
			STAT_ITEM_MAXHP_PERCENT, STAT_ITEM_MAXMANA_PERCENT, STAT_ITEM_THROW_MINDAMAGE, STAT_ITEM_THROW_MAXDAMAGE, STAT_ITEM_FASTERATTACKRATE,
			STAT_ALIGNMENT, STAT_STATE, STAT_DAMAGEPERCENT, STAT_ITEM_ALLSKILLS,
		};

		switch (rng() % 4)
		{
		case 0:
		case 1:
			return interesting_stats[rng() % std::size(interesting_stats)];

		case 2:
			return STAT_ITEM_ARMOR_PERLEVEL + rng() % (STAT_ITEM_DEADLYSTRIKE_PERLEVEL - STAT_ITEM_ARMOR_PERLEVEL + 1);

		default:
			return rng() % itemstatcost_record_count;
		}
	}

	int random_by_time_stat_id(std::mt19937& rng) const
	{
		return STAT_ITEM_ARMOR_BYTIME + rng() % (STAT_ITEM_DEADLYSTRIKE_BYTIME - STAT_ITEM_ARMOR_BYTIME + 1);
	}

	static int random_stat_value(std::mt19937& rng)
	{
		switch (rng() % 10)
		{
		case 0:
		case 1:
			return -static_cast<int>(rng() % 100) - 1;

		case 2:
			return static_cast<int>(rng() % 100000) + 1;

		default:
			return static_cast<int>(rng() % 200) + 1;
		}
	}

	uint16_t random_layer(std::mt19937& rng, int nStatId) const
	{
		// Stats applying to items are collected in an array of 16 elements when posting a stat list, so only use a single layer for those
		if (sgptDataTables->pItemStatCostTxt[nStatId].bHasOpApplyingToItem)
		{
			return 0;
		}

		return rng() % 5 == 0 ? static_cast<uint16_t>(1 + rng() % 3) : 0;
	}

	void set_random_stat(D2StatListStrc* pStatList, std::mt19937& rng) const
	{
		const int nStatId = random_stat_id(rng);
		const int nValue = random_stat_value(rng);
		native.SetStat(pStatList, nStatId, nValue, random_layer(rng, nStatId));
	}

	static uint32_t random_list_flags(std::mt19937& rng)
	{
		static constexpr uint32_t possible_flags[] = {
			STATLIST_BASIC, STATLIST_NEWLENGTH, STATLIST_TEMPONLY, STATLIST_BUFF, STATLIST_CURSE, STATLIST_MAGIC, STATLIST_OVERLAY, STATLIST_UNK_0x100,
			STATLIST_TOGGLE, STATLIST_CONVERT, STATLIST_SET,
		};

		uint32_t nFlags = 0;
		for (const auto nFlag : possible_flags)
		{
			if (rng() % (nFlag == STATLIST_SET ? 5 : 4) == 0)
			{
				nFlags |= nFlag;
			}
		}
		return nFlags;
	}

	void build_world(StatListWorld& world, const StatListWorldConfig& config, uint32_t seed) const
	{
		std::mt19937 rng(seed);
		world.native = &native;

		D2UnitStrc& unit = world.unit;
		unit.dwUnitType = config.nUnitType;
		unit.dwClassId = config.nUnitType == UNIT_PLAYER ? rng() % 7 : rng() % 512;
		unit.dwUnitId = 1;

		if (!config.bWithStatListEx)
		{
			return;
		}

		native.AllocStatListEx(&unit, static_cast<char>(rng() % 2 ? STATLIST_BASIC : 0), RecordStatValueChange, gpFakeGame);
		native.SetStat(unit.pStatListEx, STAT_LEVEL, 1 + rng() % 99, 0);
		native.SetStat(unit.pStatListEx, STAT_HITPOINTS, rng() % 4000, 0);
		native.SetStat(unit.pStatListEx, STAT_MAXHP, rng() % 4000, 0);
		native.SetStat(unit.pStatListEx, STAT_MANA, rng() % 4000, 0);
		native.SetStat(unit.pStatListEx, STAT_MAXMANA, rng() % 4000, 0);
		native.SetStat(unit.pStatListEx, STAT_STAMINA, rng() % 4000, 0);
		native.SetStat(unit.pStatListEx, STAT_MAXSTAMINA, rng() % 4000, 0);
		native.SetStat(unit.pStatListEx, fmin_stat_id, -5, 0);
		native.SetStat(unit.pStatListEx, three_op_stats_stat_id, 1 + rng() % 200, 0);
		for (int i = 0; i < config.nBaseStatCount; ++i)
		{
			set_random_stat(unit.pStatListEx, rng);
		}

		// A small pool of states so that several stat lists share the same state
		int state_pool[6]{};
		for (auto& nState : state_pool)
		{
			nState = 1 + rng() % (states_record_count - 1);
		}

		for (int i = 0; i < config.nItemCount && i < StatListWorld::max_items; ++i)
		{
			D2UnitStrc& item = world.items[i];
			item.dwUnitType = UNIT_ITEM;
			item.dwClassId = rng() % 600;
			item.dwUnitId = 100 + i;
			item.pItemData = &world.item_data[i];
			world.item_data[i].nBodyLoc = rng() % NUM_BODYLOC;

			native.AllocStatListEx(&item, static_cast<char>(rng() % 2 ? STATLIST_BASIC : 0), RecordStatValueChange, gpFakeGame);
			for (int j = 0; j < config.nStatsPerList; ++j)
			{
				set_random_stat(item.pStatListEx, rng);
			}

			if (config.bByTimeStats)
			{
				const int nStatId = random_by_time_stat_id(rng);
				native.SetStat(item.pStatListEx, nStatId, 1 + rng() % 200, 0);
			}

			if (config.bAttachAll || rng() % 4 != 0)
			{
				native.PostStatToStatList(&unit, item.pStatListEx, rng() % 2);
				item.pStatListEx->pUnit = &unit;
			}
		}

		for (int i = 0; i < config.nListCount; ++i)
		{
			const uint32_t nOwnerTypes[] = { UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM, UNIT_MISSILE };
			D2StatListStrc* pStatList = native.AllocStatList(nullptr, random_list_flags(rng), rng() % 1000, nOwnerTypes[rng() % std::size(nOwnerTypes)], 200 + i);
			native.SetState(pStatList, rng() % 4 ? state_pool[rng() % std::size(state_pool)] : 0);
			native.SetStatRemoveCallback(pStatList, RecordStatRemove);
			pStatList->dwSkillNo = rng() % 357;
			pStatList->dwSLvl = rng() % 30;

			for (int j = 0; j < config.nStatsPerList; ++j)
			{
				set_random_stat(pStatList, rng);
			}

			if (config.bAttachAll || rng() % 5 != 0)
			{
				native.PostStatToStatList(&unit, pStatList, rng() % 2);
				world.lists.push_back(pStatList);

				if (rng() % 2)
				{
					// Changing the stats of an attached list also updates the full stats of its parent
					set_random_stat(pStatList, rng);
				}
			}
			else
			{
				world.detached_lists.push_back(pStatList);
			}
		}

		if (config.nAlignment >= 0)
		{
			D2StatListStrc* pStatList = native.AllocStatList(nullptr, 0, 0, config.nUnitType, unit.dwUnitId);
			native.SetState(pStatList, STATE_ALIGNMENT);
			native.SetStat(pStatList, STAT_ALIGNMENT, config.nAlignment, 0);
			native.PostStatToStatList(&unit, pStatList, rng() % 2);
			world.lists.push_back(pStatList);
		}
	}

	void build_worlds(StatListWorld& moo_world, StatListWorld& original_world, const StatListWorldConfig& config) const
	{
		const auto seed = random_unsigned_integer();
		build_world(moo_world, config, seed);
		build_world(original_world, config, seed);
	}

	void check_worlds(StatListWorld& moo_world, StatListWorld& original_world) const
	{
		MOO_CHECK_EQ(moo_world.unit, original_world.unit, "Comparing unit");
		for (int i = 0; i < StatListWorld::max_items; ++i)
		{
			MOO_CHECK_EQ(moo_world.items[i], original_world.items[i], "Comparing item");
		}

		CHECK_EQ(moo_world.detached_lists.size(), original_world.detached_lists.size());
		for (size_t i = 0; i < moo_world.detached_lists.size() && i < original_world.detached_lists.size(); ++i)
		{
			MOO_CHECK_EQ(moo_world.detached_lists[i], original_world.detached_lists[i], "Comparing detached list");
		}

		auto wrapped_moo_records = DynamicArray<StatListCallbackRecord>{ moo_world.records.data(), static_cast<int>(moo_world.records.size()) };
		auto wrapped_original_records = DynamicArray<StatListCallbackRecord>{ original_world.records.data(), static_cast<int>(original_world.records.size()) };
		MOO_CHECK_EQ(wrapped_moo_records, wrapped_original_records, "Comparing callbacks");
	}
};


TEST_SUITE("D2StatListTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));


	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB57C0 (#10563)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_AreUnitsAligned, dll_base + 0x000757C0);

		REPEAT_5();

		const auto unit_type1 = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_OBJECT, UNIT_ITEM);
		const auto unit_type2 = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_MISSILE);

		SUBCASE("")
		{
			for (int alignment1 = -1; alignment1 <= UNIT_ALIGNMENT_GOOD; ++alignment1)
			{
				for (int alignment2 = -1; alignment2 <= UNIT_ALIGNMENT_GOOD; ++alignment2)
				{
					// Input data
					StatListWorld moo_world1;
					StatListWorld moo_world2;
					StatListWorld original_world1;
					StatListWorld original_world2;

					StatListWorldConfig config1;
					config1.nUnitType = unit_type1;
					config1.nAlignment = alignment1;
					config1.bWithStatListEx = random_unsigned_integer(0, 7) != 0;
					StatListWorldConfig config2;
					config2.nUnitType = unit_type2;
					config2.nAlignment = alignment2;

					build_worlds(moo_world1, original_world1, config1);
					build_worlds(moo_world2, original_world2, config2);

					// Call both implementations
					const auto moo_result = sut(&moo_world1.unit, &moo_world2.unit);
					const auto original_result = original(&original_world1.unit, &original_world2.unit);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

					// Same unit
					const auto moo_result_same = sut(&moo_world1.unit, &moo_world1.unit);
					const auto original_result_same = original(&original_world1.unit, &original_world1.unit);
					MOO_CHECK_EQ(moo_result_same, original_result_same, "Comparing results (same unit)");

					// Null units
					const auto moo_result_null1 = sut(nullptr, &moo_world2.unit);
					const auto original_result_null1 = original(nullptr, &original_world2.unit);
					MOO_CHECK_EQ(moo_result_null1, original_result_null1, "Comparing results (null unit 1)");
					const auto moo_result_null2 = sut(&moo_world1.unit, nullptr);
					const auto original_result_null2 = original(&original_world1.unit, nullptr);
					MOO_CHECK_EQ(moo_result_null2, original_result_null2, "Comparing results (null unit 2)");

					// Compare potentially modified input data
					check_worlds(moo_world1, original_world1);
					check_worlds(moo_world2, original_world2);
				}
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB5830")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDB5830, dll_base + 0x00075830);

		REPEAT_5();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			build_worlds(moo_world, original_world, config);

			const auto moo_lists = moo_world.get_extended_lists();
			const auto original_lists = original_world.get_extended_lists();
			REQUIRE_EQ(moo_lists.size(), original_lists.size());

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				for (int nStatId = 0; nStatId < itemstatcost_record_count; ++nStatId)
				{
					for (uint16_t nLayer = 0; nLayer < 2; ++nLayer)
					{
						const auto nLayer_StatId = D2SLayerStatIdStrc::Make(nLayer, nStatId).nPackedValue;

						// Call both implementations
						const auto moo_result = sut(moo_lists[i], nLayer_StatId);
						const auto original_result = original(original_lists[i], nLayer_StatId);

						// Compare return values
						MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
					}
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB6300")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_FindStatIndex_6FDB6300, dll_base + 0x00076300);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			auto moo_arrays = moo_world.get_stats_arrays();
			auto original_arrays = original_world.get_stats_arrays();
			REQUIRE_EQ(moo_arrays.size(), original_arrays.size());

			// An empty array
			D2StatsArrayStrc moo_empty_array{};
			D2StatsArrayStrc original_empty_array{};
			moo_arrays.push_back(&moo_empty_array);
			original_arrays.push_back(&original_empty_array);

			for (size_t i = 0; i < moo_arrays.size(); ++i)
			{
				for (int nStatId = 0; nStatId < itemstatcost_record_count; ++nStatId)
				{
					for (uint16_t nLayer = 0; nLayer < 4; ++nLayer)
					{
						const auto nLayer_StatId = D2SLayerStatIdStrc::Make(nLayer, nStatId).nPackedValue;

						// Call both implementations
						const auto moo_result = sut(moo_arrays[i], nLayer_StatId);
						const auto original_result = original(original_arrays[i], nLayer_StatId);

						// Compare return values
						MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
					}
				}

				// Compare potentially modified input data
				MOO_CHECK_EQ(*moo_arrays[i], *original_arrays[i], "Comparing pStatArray");
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB6340")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetBaseStat_6FDB6340, dll_base + 0x00076340);

		REPEAT_5();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_record = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			build_worlds(moo_world, original_world, config);

			const auto moo_lists = moo_world.get_lists();
			const auto original_lists = original_world.get_lists();
			REQUIRE_EQ(moo_lists.size(), original_lists.size());

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				for (int nStatId = 0; nStatId < itemstatcost_record_count; ++nStatId)
				{
					for (uint16_t nLayer = 0; nLayer < 2; ++nLayer)
					{
						const auto nLayer_StatId = D2SLayerStatIdStrc::Make(nLayer, nStatId).nPackedValue;
						// Both implementations share the same table
						D2ItemStatCostTxt* pItemStatCostTxtRecord = with_record ? &itemstatcost_txt[nStatId] : nullptr;

						// Call both implementations
						const auto moo_result = sut(moo_lists[i], nLayer_StatId, pItemStatCostTxtRecord);
						const auto original_result = original(original_lists[i], nLayer_StatId, pItemStatCostTxtRecord);

						// Compare return values
						MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
					}
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB63E0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetTotalStat_6FDB63E0, dll_base + 0x000763E0);

		REPEAT_5();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_record = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			build_worlds(moo_world, original_world, config);

			const auto moo_lists = moo_world.get_lists();
			const auto original_lists = original_world.get_lists();
			REQUIRE_EQ(moo_lists.size(), original_lists.size());

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				for (int nStatId = 0; nStatId < itemstatcost_record_count; ++nStatId)
				{
					for (uint16_t nLayer = 0; nLayer < 2; ++nLayer)
					{
						const auto nLayer_StatId = D2SLayerStatIdStrc::Make(nLayer, nStatId).nPackedValue;
						// Both implementations share the same table
						D2ItemStatCostTxt* pItemStatCostTxtRecord = with_record ? &itemstatcost_txt[nStatId] : nullptr;

						// Call both implementations
						const auto moo_result = sut(moo_lists[i], nLayer_StatId, pItemStatCostTxtRecord);
						const auto original_result = original(original_lists[i], nLayer_StatId, pItemStatCostTxtRecord);

						// Compare return values
						MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
					}
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB64A0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDB64A0, dll_base + 0x000764A0);

		REPEAT_5();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			build_worlds(moo_world, original_world, config);

			const auto moo_lists = moo_world.get_extended_lists();
			const auto original_lists = original_world.get_extended_lists();
			REQUIRE_EQ(moo_lists.size(), original_lists.size());

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				const auto with_unit = random_unsigned_integer(0, 1);
				for (int nStatId = 0; nStatId < itemstatcost_record_count; ++nStatId)
				{
					const uint16_t nLayer = random_unsigned_integer(0, 4) == 0 ? 1 : 0;
					const auto nLayer_StatId = D2SLayerStatIdStrc::Make(nLayer, nStatId).nPackedValue;
					// Both implementations share the same table
					D2ItemStatCostTxt* pItemStatCostTxtRecord = &itemstatcost_txt[nStatId];

					// Call both implementations
					const auto moo_result = with_callbacks(moo_world, [&] { return sut(moo_lists[i], nLayer_StatId, pItemStatCostTxtRecord, with_unit ? &moo_world.unit : nullptr); });
					const auto original_result = with_callbacks(original_world, [&] { return original(original_lists[i], nLayer_StatId, pItemStatCostTxtRecord, with_unit ? &original_world.unit : nullptr); });

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}

				// Compare potentially modified input data
				check_worlds(moo_world, original_world);
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB6920")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_FindStat_6FDB6920, dll_base + 0x00076920);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			auto moo_arrays = moo_world.get_stats_arrays();
			auto original_arrays = original_world.get_stats_arrays();
			REQUIRE_EQ(moo_arrays.size(), original_arrays.size());

			// An empty array
			D2StatsArrayStrc moo_empty_array{};
			D2StatsArrayStrc original_empty_array{};
			moo_arrays.push_back(&moo_empty_array);
			original_arrays.push_back(&original_empty_array);

			for (size_t i = 0; i < moo_arrays.size(); ++i)
			{
				for (int nStatId = 0; nStatId < itemstatcost_record_count; ++nStatId)
				{
					for (uint16_t nLayer = 0; nLayer < 4; ++nLayer)
					{
						const auto nLayer_StatId = D2SLayerStatIdStrc::Make(nLayer, nStatId).nPackedValue;

						// Call both implementations
						const auto moo_result = sut(moo_arrays[i], nLayer_StatId);
						const auto original_result = original(original_arrays[i], nLayer_StatId);

						// Compare return values
						MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
						const auto moo_index = moo_result ? moo_result - moo_arrays[i]->pStat : -1;
						const auto original_index = original_result ? original_result - original_arrays[i]->pStat : -1;
						CHECK_EQ(moo_index, original_index);
					}
				}

				// Compare potentially modified input data
				MOO_CHECK_EQ(*moo_arrays[i], *original_arrays[i], "Comparing pStatArray");
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB6970")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_InsertStatOrFail_6FDB6970, dll_base + 0x00076970);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			const auto moo_arrays = moo_world.get_stats_arrays();
			const auto original_arrays = original_world.get_stats_arrays();
			REQUIRE_EQ(moo_arrays.size(), original_arrays.size());

			for (size_t i = 0; i < moo_arrays.size(); ++i)
			{
				// Enough insertions to trigger reallocations, as well as insertions of stats which are already in the array
				for (int j = 0; j < 12; ++j)
				{
					D2SLayerStatIdStrc::PackedType nLayer_StatId = 0;
					if (moo_arrays[i]->nStatCount > 0 && random_unsigned_integer(0, 3) == 0)
					{
						nLayer_StatId = moo_arrays[i]->pStat[random_unsigned_integer(0, moo_arrays[i]->nStatCount - 1)].nPackedValue;
					}
					else
					{
						nLayer_StatId = D2SLayerStatIdStrc::Make(random_unsigned_integer(0, 3), random_unsigned_integer(0, itemstatcost_record_count - 1)).nPackedValue;
					}

					// Call both implementations
					const auto moo_result = sut(nullptr, moo_arrays[i], nLayer_StatId);
					const auto original_result = original(nullptr, original_arrays[i], nLayer_StatId);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
					const auto moo_index = moo_result ? moo_result - moo_arrays[i]->pStat : -1;
					const auto original_index = original_result ? original_result - original_arrays[i]->pStat : -1;
					CHECK_EQ(moo_index, original_index);

					// Compare potentially modified input data
					MOO_CHECK_EQ(*moo_arrays[i], *original_arrays[i], "Comparing pStatsArray");
				}
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB6A30")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_RemoveStat_6FDB6A30, dll_base + 0x00076A30);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nBaseStatCount = 24;
			config.nStatsPerList = 12;
			build_worlds(moo_world, original_world, config);

			const auto moo_arrays = moo_world.get_stats_arrays();
			const auto original_arrays = original_world.get_stats_arrays();
			REQUIRE_EQ(moo_arrays.size(), original_arrays.size());

			for (size_t i = 0; i < moo_arrays.size(); ++i)
			{
				// A stat past the end of the array is ignored
				sut(nullptr, moo_arrays[i], &moo_arrays[i]->pStat[moo_arrays[i]->nStatCount]);
				original(nullptr, original_arrays[i], &original_arrays[i]->pStat[original_arrays[i]->nStatCount]);
				MOO_CHECK_EQ(*moo_arrays[i], *original_arrays[i], "Comparing pStatEx");

				// Remove all stats in a random order, which also shrinks the arrays
				while (moo_arrays[i]->nStatCount > 0)
				{
					const auto index = random_unsigned_integer(0, moo_arrays[i]->nStatCount - 1);

					// Call both implementations
					sut(nullptr, moo_arrays[i], &moo_arrays[i]->pStat[index]);
					original(nullptr, original_arrays[i], &original_arrays[i]->pStat[index]);

					// Compare potentially modified input data
					MOO_CHECK_EQ(*moo_arrays[i], *original_arrays[i], "Comparing pStatEx");
				}
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB6AB0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_UpdateUnitStat_6FDB6AB0, dll_base + 0x00076AB0);

		REPEAT_10();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			build_worlds(moo_world, original_world, config);

			const auto moo_lists = moo_world.get_extended_lists();
			const auto original_lists = original_world.get_extended_lists();
			REQUIRE_EQ(moo_lists.size(), original_lists.size());

			std::mt19937 rng(random_unsigned_integer());
			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				for (int j = 0; j < 40; ++j)
				{
					int nStatId = 0;
					uint16_t nLayer = 0;
					if (moo_lists[i]->FullStats.nStatCount > 0 && rng() % 2)
					{
						const D2StatStrc& stat = moo_lists[i]->FullStats.pStat[rng() % moo_lists[i]->FullStats.nStatCount];
						nStatId = stat.nStat;
						nLayer = stat.nLayer;
					}
					else
					{
						nStatId = random_stat_id(rng);
						nLayer = random_layer(rng, nStatId);
					}
					const auto nLayer_StatId = D2SLayerStatIdStrc::Make(nLayer, nStatId).nPackedValue;
					const int nNewValue = rng() % 4 == 0 ? 0 : random_stat_value(rng);
					D2ItemStatCostTxt* pItemStatCostTxtRecord = &itemstatcost_txt[nStatId];
					const auto with_unit = rng() % 2;

					// Call both implementations
					with_callbacks(moo_world, [&] { sut(moo_lists[i], nLayer_StatId, nNewValue, pItemStatCostTxtRecord, with_unit ? &moo_world.unit : nullptr); });
					with_callbacks(original_world, [&] { original(original_lists[i], nLayer_StatId, nNewValue, pItemStatCostTxtRecord, with_unit ? &original_world.unit : nullptr); });

					// Compare potentially modified input data
					check_worlds(moo_world, original_world);
				}
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB6C10")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDB6C10, dll_base + 0x00076C10);

		REPEAT_10();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			build_worlds(moo_world, original_world, config);

			const auto moo_lists = moo_world.get_lists();
			const auto original_lists = original_world.get_lists();
			REQUIRE_EQ(moo_lists.size(), original_lists.size());

			std::mt19937 rng(random_unsigned_integer());
			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				for (int j = 0; j < 10; ++j)
				{
					const int nStatId = random_stat_id(rng);
					const auto nLayer_StatId = D2SLayerStatIdStrc::Make(random_layer(rng, nStatId), nStatId).nPackedValue;
					const int nValue = rng() % 8 == 0 ? 0 : random_stat_value(rng);
					const auto with_unit = rng() % 2;

					// Call both implementations
					with_callbacks(moo_world, [&] { sut(static_cast<D2StatListExStrc*>(moo_lists[i]), nLayer_StatId, nValue, with_unit ? &moo_world.unit : nullptr); });
					with_callbacks(original_world, [&] { original(static_cast<D2StatListExStrc*>(original_lists[i]), nLayer_StatId, nValue, with_unit ? &original_world.unit : nullptr); });

					// Compare potentially modified input data
					check_worlds(moo_world, original_world);
				}
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB6E30")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_ExpireStatList_6FDB6E30, dll_base + 0x00076E30);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			build_worlds(moo_world, original_world, config);

			sut(nullptr);
			original(nullptr);

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			REQUIRE_EQ(moo_lists.size(), original_lists.size());

			// Expire the lists in a random order
			std::vector<size_t> order(moo_lists.size());
			for (size_t i = 0; i < order.size(); ++i)
			{
				order[i] = i;
			}
			std::shuffle(order.begin(), order.end(), std::mt19937(random_unsigned_integer()));

			for (const auto i : order)
			{
				// Call both implementations
				with_callbacks(moo_world, [&] { sut(moo_lists[i]); });
				with_callbacks(original_world, [&] { original(original_lists[i]); });

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_lists[i], original_lists[i], "Comparing pStatList");
				check_worlds(moo_world, original_world);

				// The expired child lists are no longer part of the unit, make sure they are freed
				if (!STATLIST_IsExtended(moo_lists[i]))
				{
					moo_world.forget_list(moo_lists[i]);
					original_world.forget_list(original_lists[i]);
					moo_world.detached_lists.push_back(moo_lists[i]);
					original_world.detached_lists.push_back(original_lists[i]);
				}
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7030 (#10485)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_FreeStatList, dll_base + 0x00077030);

		REPEAT_20();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			sut(nullptr);
			original(nullptr);

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			REQUIRE_EQ(moo_lists.size(), original_lists.size());

			std::vector<size_t> order(moo_lists.size());
			for (size_t i = 0; i < order.size(); ++i)
			{
				order[i] = i;
			}
			std::shuffle(order.begin(), order.end(), std::mt19937(random_unsigned_integer()));

			for (const auto i : order)
			{
				const bool is_extended = STATLIST_IsExtended(moo_lists[i]);

				// Call both implementations
				with_callbacks(moo_world, [&] { sut(moo_lists[i]); });
				with_callbacks(original_world, [&] { original(original_lists[i]); });

				// Extended lists are not freed by this function
				if (!is_extended)
				{
					moo_world.forget_list(moo_lists[i]);
					original_world.forget_list(original_lists[i]);
				}

				// Compare potentially modified input data
				check_worlds(moo_world, original_world);
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7050")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_STATLIST_FreeStatListImpl_6FDB7050, dll_base + 0x00077050);

		REPEAT_20();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			// Free some child lists
			for (size_t i = 0; i < moo_world.lists.size(); ++i)
			{
				if (random_unsigned_integer(0, 2) != 0)
				{
					continue;
				}

				D2StatListStrc* pMooStatList = moo_world.lists[i];
				D2StatListStrc* pOriginalStatList = original_world.lists[i];

				// Call both implementations
				with_callbacks(moo_world, [&] { sut(pMooStatList); });
				with_callbacks(original_world, [&] { original(pOriginalStatList); });

				moo_world.forget_list(pMooStatList);
				original_world.forget_list(pOriginalStatList);

				// Compare potentially modified input data
				check_worlds(moo_world, original_world);
			}

			// Free the items
			for (int i = 0; i < StatListWorld::max_items; ++i)
			{
				if (!moo_world.items[i].pStatListEx || random_unsigned_integer(0, 1) == 0)
				{
					continue;
				}

				// Call both implementations
				with_callbacks(moo_world, [&] { sut(moo_world.items[i].pStatListEx); });
				with_callbacks(original_world, [&] { original(original_world.items[i].pStatListEx); });

				moo_world.items[i].pStatListEx = nullptr;
				original_world.items[i].pStatListEx = nullptr;

				// Compare potentially modified input data
				check_worlds(moo_world, original_world);
			}

			// Free the unit, which also frees the remaining child lists
			with_callbacks(moo_world, [&] { sut(moo_world.unit.pStatListEx); });
			with_callbacks(original_world, [&] { original(original_world.unit.pStatListEx); });

			moo_world.unit.pStatListEx = nullptr;
			original_world.unit.pStatListEx = nullptr;
			std::fill(moo_world.lists.begin(), moo_world.lists.end(), nullptr);
			std::fill(original_world.lists.begin(), original_world.lists.end(), nullptr);

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7110 (#10527)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_FreeStatListEx, dll_base + 0x00077110);

		REPEAT_20();

		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			// Free an item first, then the unit
			if (moo_world.items[0].pStatListEx)
			{
				with_callbacks(moo_world, [&] { sut(&moo_world.items[0]); });
				with_callbacks(original_world, [&] { original(&original_world.items[0]); });
				check_worlds(moo_world, original_world);
			}

			// Call both implementations
			with_callbacks(moo_world, [&] { sut(&moo_world.unit); });
			with_callbacks(original_world, [&] { original(&original_world.unit); });

			// The child lists were freed as well
			std::fill(moo_world.lists.begin(), moo_world.lists.end(), nullptr);
			std::fill(original_world.lists.begin(), original_world.lists.end(), nullptr);

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
			CHECK_EQ(moo_world.unit.pStatListEx, nullptr);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7140 (#10470)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_AllocStatList, dll_base + 0x00077140);

		SUBCASE("")
		{
			for (int i = 0; i < 100; ++i)
			{
				// Input data
				const uint32_t fFilter = random_unsigned_integer() & ~STATLIST_EXTENDED;
				const uint32_t dwTimeout = random_unsigned_integer();
				const int nUnitType = random_unsigned_integer(0, 6);
				const D2UnitGUID nUnitGUID = random_unsigned_integer();

				// Call both implementations
				const auto moo_result = sut(nullptr, fFilter, dwTimeout, nUnitType, nUnitGUID);
				const auto original_result = original(nullptr, fFilter, dwTimeout, nUnitType, nUnitGUID);

				// Compare return values
				REQUIRE_NE(moo_result, nullptr);
				REQUIRE_NE(original_result, nullptr);
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				CHECK_EQ(moo_result->dwFlags, fFilter);

				native.FreeStatList(moo_result);
				native.FreeStatList(original_result);
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7190 (#10526)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_AllocStatListEx, dll_base + 0x00077190);

		REPEAT_10();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			const char nFlags = static_cast<char>(random_unsigned_integer(0, 255));
			const auto with_callback = random_unsigned_integer(0, 1);

			// Call both implementations, the previous stat list is freed
			with_callbacks(moo_world, [&] { sut(&moo_world.unit, nFlags, with_callback ? RecordStatValueChange : nullptr, gpFakeGame); });
			with_callbacks(original_world, [&] { original(&original_world.unit, nFlags, with_callback ? RecordStatValueChange : nullptr, gpFakeGame); });

			std::fill(moo_world.lists.begin(), moo_world.lists.end(), nullptr);
			std::fill(original_world.lists.begin(), original_world.lists.end(), nullptr);

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
			REQUIRE_NE(moo_world.unit.pStatListEx, nullptr);
			CHECK_EQ(moo_world.unit.pStatListEx->pGame, gpFakeGame);
			CHECK_EQ(original_world.unit.pStatListEx->pGame, gpFakeGame);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7260 (#10471)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetOwnerType, dll_base + 0x00077260);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				// Call both implementations
				const auto moo_result = sut(moo_lists[i]);
				const auto original_result = original(original_lists[i]);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FD912D0 (#10472)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetOwnerGUID, dll_base + 0x000512D0);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				// Call both implementations
				const auto moo_result = sut(moo_lists[i]);
				const auto original_result = original(original_lists[i]);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7280 (#11304)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetBaseStatsCount, dll_base + 0x00077280);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				// Call both implementations
				const auto moo_result = sut(moo_lists[i]);
				const auto original_result = original(original_lists[i]);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB72A0 (#11305)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetFullStatsCountFromUnit, dll_base + 0x000772A0);

		REPEAT_10();

		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				// Call both implementations
				const auto moo_result = sut(moo_units[i]);
				const auto original_result = original(original_units[i]);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB72C0 (#10478)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_SetState, dll_base + 0x000772C0);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				const int nState = random_unsigned_integer(0, states_record_count - 1);

				// Call both implementations
				sut(moo_lists[i], nState);
				original(original_lists[i], nState);
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB72E0 (#10479)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetState, dll_base + 0x000772E0);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				// Call both implementations
				const auto moo_result = sut(moo_lists[i]);
				const auto original_result = original(original_lists[i]);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7300 (#10528)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_SetExpireFrame, dll_base + 0x00077300);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				const int nExpireFrames[] = { 0, -1, static_cast<int>(random_unsigned_integer()), static_cast<int>(random_unsigned_integer(1, 10000)) };
				const int nExpireFrame = nExpireFrames[random_unsigned_integer(0, std::size(nExpireFrames) - 1)];

				// Call both implementations
				sut(moo_lists[i], nExpireFrame);
				original(original_lists[i], nExpireFrame);
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7320 (#10529)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetExpireFrame, dll_base + 0x00077320);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				// Call both implementations
				const auto moo_result = sut(moo_lists[i]);
				const auto original_result = original(original_lists[i]);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7340 (#10475)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10475_PostStatToStatList, dll_base + 0x00077340);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			config.nListCount = 10;
			build_worlds(moo_world, original_world, config);

			// Create more lists to post, with the original implementation
			std::mt19937 moo_rng(random_unsigned_integer());
			std::mt19937 original_rng = moo_rng;
			for (int i = 0; i < 4; ++i)
			{
				const auto create_list = [this](StatListWorld& world, std::mt19937& rng)
				{
					D2StatListStrc* pStatList = native.AllocStatList(nullptr, random_list_flags(rng), rng() % 1000, UNIT_PLAYER, 300);
					native.SetState(pStatList, 1 + rng() % (states_record_count - 1));
					native.SetStatRemoveCallback(pStatList, RecordStatRemove);
					for (int j = 0; j < 6; ++j)
					{
						set_random_stat(pStatList, rng);
					}
					world.detached_lists.push_back(pStatList);
				};
				create_list(moo_world, moo_rng);
				create_list(original_world, original_rng);
			}

			// Post the detached lists, the items and some lists which are already posted
			std::vector<D2StatListStrc*> moo_lists = moo_world.detached_lists;
			std::vector<D2StatListStrc*> original_lists = original_world.detached_lists;
			for (int i = 0; i < StatListWorld::max_items; ++i)
			{
				if (moo_world.items[i].pStatListEx)
				{
					moo_lists.push_back(moo_world.items[i].pStatListEx);
					original_lists.push_back(original_world.items[i].pStatListEx);
				}
			}
			for (size_t i = 0; i < moo_world.lists.size(); i += 3)
			{
				moo_lists.push_back(moo_world.lists[i]);
				original_lists.push_back(original_world.lists[i]);
			}

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				const BOOL bResetFlag = random_unsigned_integer(0, 1);

				// Call both implementations
				with_callbacks(moo_world, [&] { sut(&moo_world.unit, moo_lists[i], bResetFlag); });
				with_callbacks(original_world, [&] { original(&original_world.unit, original_lists[i], bResetFlag); });

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_lists[i], original_lists[i], "Comparing pStatList");
				check_worlds(moo_world, original_world);
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7560 (#10464)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_AddStat, dll_base + 0x00077560);

		REPEAT_10();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			build_worlds(moo_world, original_world, config);

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			std::mt19937 rng(random_unsigned_integer());
			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				for (int j = 0; j < 10; ++j)
				{
					int nStatId = random_stat_id(rng);
					uint16_t nLayer = random_layer(rng, nStatId);
					int nValue = rng() % 8 == 0 ? 0 : random_stat_value(rng);
					if (moo_lists[i] && moo_lists[i]->Stats.nStatCount > 0 && rng() % 3 == 0)
					{
						// Cancel an existing stat, which removes it
						const D2StatStrc& stat = moo_lists[i]->Stats.pStat[rng() % moo_lists[i]->Stats.nStatCount];
						nStatId = stat.nStat;
						nLayer = stat.nLayer;
						nValue = -stat.nValue;
					}

					// Call both implementations
					with_callbacks(moo_world, [&] { sut(moo_lists[i], nStatId, nValue, nLayer); });
					with_callbacks(original_world, [&] { original(original_lists[i], nStatId, nValue, nLayer); });

					// Compare potentially modified input data
					check_worlds(moo_world, original_world);
				}
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7690")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_InsertStatModOrFail_6FDB7690, dll_base + 0x00077690);

		REPEAT_5();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			const auto moo_lists = moo_world.get_lists();
			const auto original_lists = original_world.get_lists();

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				for (int nStatId = 0; nStatId < itemstatcost_record_count; ++nStatId)
				{
					const auto nLayer_StatId = D2SLayerStatIdStrc::Make(random_unsigned_integer(0, 2), nStatId).nPackedValue;

					// Call both implementations
					sut(moo_lists[i], nLayer_StatId);
					original(original_lists[i], nLayer_StatId);
				}

				// Compare potentially modified input data
				check_worlds(moo_world, original_world);
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB77B0 (#10463)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_SetStat, dll_base + 0x000777B0);

		REPEAT_10();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			build_worlds(moo_world, original_world, config);

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			std::mt19937 rng(random_unsigned_integer());
			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				for (int j = 0; j < 10; ++j)
				{
					int nStatId = random_stat_id(rng);
					uint16_t nLayer = random_layer(rng, nStatId);
					int nValue = rng() % 4 == 0 ? 0 : random_stat_value(rng);
					if (moo_lists[i] && moo_lists[i]->Stats.nStatCount > 0 && rng() % 3 == 0)
					{
						// Change an existing stat, possibly to the same value
						const D2StatStrc& stat = moo_lists[i]->Stats.pStat[rng() % moo_lists[i]->Stats.nStatCount];
						nStatId = stat.nStat;
						nLayer = stat.nLayer;
						nValue = rng() % 3 == 0 ? stat.nValue : nValue;
					}

					// Call both implementations
					const auto moo_result = with_callbacks(moo_world, [&] { return sut(moo_lists[i], nStatId, nValue, nLayer); });
					const auto original_result = with_callbacks(original_world, [&] { return original(original_lists[i], nStatId, nValue, nLayer); });

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

					// Compare potentially modified input data
					check_worlds(moo_world, original_world);
				}
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7910 (#10465)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_SetStatIfListIsValid, dll_base + 0x00077910);

		REPEAT_10();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			build_worlds(moo_world, original_world, config);

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			std::mt19937 rng(random_unsigned_integer());
			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				for (int j = 0; j < 5; ++j)
				{
					const int nStatId = random_stat_id(rng);
					const uint16_t nLayer = random_layer(rng, nStatId);
					const int nValue = rng() % 4 == 0 ? 0 : random_stat_value(rng);

					// Call both implementations
					with_callbacks(moo_world, [&] { sut(moo_lists[i], nStatId, nValue, nLayer); });
					with_callbacks(original_world, [&] { original(original_lists[i], nStatId, nValue, nLayer); });

					// Compare potentially modified input data
					check_worlds(moo_world, original_world);
				}
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7930 (#11294)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_SetBaseStat, dll_base + 0x00077930);

		REPEAT_10();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			build_worlds(moo_world, original_world, config);

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			std::mt19937 rng(random_unsigned_integer());
			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				for (int j = 0; j < 10; ++j)
				{
					int nStatId = random_stat_id(rng);
					uint16_t nLayer = random_layer(rng, nStatId);
					int nValue = rng() % 4 == 0 ? 0 : random_stat_value(rng);
					if (moo_lists[i] && moo_lists[i]->Stats.nStatCount > 0 && rng() % 3 == 0)
					{
						const D2StatStrc& stat = moo_lists[i]->Stats.pStat[rng() % moo_lists[i]->Stats.nStatCount];
						nStatId = stat.nStat;
						nLayer = stat.nLayer;
						nValue = rng() % 3 == 0 ? stat.nValue : nValue;
					}
					const auto unit_index = rng() % 3;
					D2UnitStrc* pMooUnit = unit_index == 0 ? nullptr : unit_index == 1 ? &moo_world.unit : &moo_world.items[0];
					D2UnitStrc* pOriginalUnit = unit_index == 0 ? nullptr : unit_index == 1 ? &original_world.unit : &original_world.items[0];

					// Call both implementations
					const auto moo_result = with_callbacks(moo_world, [&] { return sut(moo_lists[i], nStatId, nValue, nLayer, pMooUnit); });
					const auto original_result = with_callbacks(original_world, [&] { return original(original_lists[i], nStatId, nValue, nLayer, pOriginalUnit); });

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

					// Compare potentially modified input data
					check_worlds(moo_world, original_world);
				}
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7AB0 (#10517)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_SetUnitStat, dll_base + 0x00077AB0);

		REPEAT_10();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			const auto moo_units = moo_world.get_units();
			const auto original_units = original_world.get_units();

			std::mt19937 rng(random_unsigned_integer());
			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				for (int j = 0; j < 10; ++j)
				{
					const int nStatId = random_stat_id(rng);
					const uint16_t nLayer = random_layer(rng, nStatId);
					const int nValue = rng() % 4 == 0 ? 0 : random_stat_value(rng);

					// Call both implementations
					with_callbacks(moo_world, [&] { sut(moo_units[i], nStatId, nValue, nLayer); });
					with_callbacks(original_world, [&] { original(original_units[i], nStatId, nValue, nLayer); });

					// Compare potentially modified input data
					check_worlds(moo_world, original_world);
				}
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7B00 (#10518)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_AddUnitStat, dll_base + 0x00077B00);

		REPEAT_10();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			const auto moo_units = moo_world.get_units();
			const auto original_units = original_world.get_units();

			std::mt19937 rng(random_unsigned_integer());
			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				for (int j = 0; j < 10; ++j)
				{
					const int nStatId = random_stat_id(rng);
					const uint16_t nLayer = random_layer(rng, nStatId);
					const int nValue = rng() % 4 == 0 ? 0 : random_stat_value(rng);

					// Call both implementations
					with_callbacks(moo_world, [&] { sut(moo_units[i], nStatId, nValue, nLayer); });
					with_callbacks(original_world, [&] { original(original_units[i], nStatId, nValue, nLayer); });

					// Compare potentially modified input data
					check_worlds(moo_world, original_world);
				}
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7C30 (#10519)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_UnitGetStatValue, dll_base + 0x00077C30);

		REPEAT_5();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				for (int nStatId = -1; nStatId <= itemstatcost_record_count; ++nStatId)
				{
					for (uint16_t nLayer = 0; nLayer < 2; ++nLayer)
					{
						// Call both implementations
						const auto moo_result = sut(moo_units[i], nStatId, nLayer);
						const auto original_result = original(original_units[i], nStatId, nLayer);

						// Compare return values
						MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
					}
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7E30 (#10520)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_UnitGetItemStatOrSkillStatValue, dll_base + 0x00077E30);

		REPEAT_5();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			// The original does not check for null units
			const auto moo_units = moo_world.get_units();
			const auto original_units = original_world.get_units();

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				for (int nStatId = -1; nStatId <= itemstatcost_record_count; ++nStatId)
				{
					for (uint16_t nLayer = 0; nLayer < 2; ++nLayer)
					{
						// Call both implementations
						const auto moo_result = sut(moo_units[i], nStatId, nLayer);
						const auto original_result = original(original_units[i], nStatId, nLayer);

						// Compare return values
						MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
					}
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7B30 (#10521)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetUnitBaseStat, dll_base + 0x00077B30);

		REPEAT_5();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				for (int nStatId = -1; nStatId <= itemstatcost_record_count; ++nStatId)
				{
					for (uint16_t nLayer = 0; nLayer < 2; ++nLayer)
					{
						// Call both implementations
						const auto moo_result = sut(moo_units[i], nStatId, nLayer);
						const auto original_result = original(original_units[i], nStatId, nLayer);

						// Compare return values
						MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
					}
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB7F40 (#10522)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetUnitStatBonus, dll_base + 0x00077F40);

		REPEAT_5();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			const auto moo_units = moo_world.get_units();
			const auto original_units = original_world.get_units();

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				for (int nStatId = -1; nStatId <= itemstatcost_record_count; ++nStatId)
				{
					for (uint16_t nLayer = 0; nLayer < 2; ++nLayer)
					{
						// Call both implementations
						const auto moo_result = sut(moo_units[i], nStatId, nLayer);
						const auto original_result = original(original_units[i], nStatId, nLayer);

						// Compare return values
						MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
					}
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB80C0 (#10515)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_DeactivateTemporaryStates, dll_base + 0x000780C0);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			config.nListCount = 10;
			build_worlds(moo_world, original_world, config);

			// Call both implementations
			with_callbacks(moo_world, [&] { sut(&moo_world.unit); });
			with_callbacks(original_world, [&] { original(&original_world.unit); });

			// The temporary lists were freed
			std::fill(moo_world.lists.begin(), moo_world.lists.end(), nullptr);
			std::fill(original_world.lists.begin(), original_world.lists.end(), nullptr);

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8120 (#10467)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10467, dll_base + 0x00078120);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				const int nStatCount = moo_lists[i] ? moo_lists[i]->Stats.nStatCount : 0;
				for (int nStat = 0; nStat <= nStatCount + 1; ++nStat)
				{
					// Call both implementations
					const auto moo_result = sut(moo_lists[i], nStat);
					const auto original_result = original(original_lists[i], nStat);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8150 (#10468)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_RemoveAllStats, dll_base + 0x00078150);

		REPEAT_10();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			build_worlds(moo_world, original_world, config);

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				// Call both implementations
				with_callbacks(moo_world, [&] { sut(moo_lists[i]); });
				with_callbacks(original_world, [&] { original(original_lists[i]); });

				// Compare potentially modified input data
				check_worlds(moo_world, original_world);
				if (moo_lists[i])
				{
					CHECK_EQ(moo_lists[i]->Stats.nStatCount, 0);
				}
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8190")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_GetStateFromStatListEx_6FDB8190, dll_base + 0x00078190);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nListCount = 10;
			build_worlds(moo_world, original_world, config);

			auto moo_lists = moo_world.get_extended_lists();
			auto original_lists = original_world.get_extended_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				for (int nStateId = 0; nStateId < states_record_count; ++nStateId)
				{
					// Call both implementations
					const auto moo_result = sut(moo_lists[i], nStateId);
					const auto original_result = original(original_lists[i], nStateId);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
					CHECK_EQ(moo_world.find_list_index(moo_result), original_world.find_list_index(original_result));
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB81E0 (#10480)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetStatListFromUnitAndState, dll_base + 0x000781E0);

		REPEAT_10();

		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.bWithStatListEx = with_stat_list;
			config.nListCount = 10;
			build_worlds(moo_world, original_world, config);

			const auto moo_units = moo_world.get_units();
			const auto original_units = original_world.get_units();

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				for (int nState = 0; nState < states_record_count; ++nState)
				{
					// Call both implementations
					const auto moo_result = sut(moo_units[i], nState);
					const auto original_result = original(original_units[i], nState);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
					CHECK_EQ(moo_world.find_list_index(moo_result), original_world.find_list_index(original_result));
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8200 (#10482)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetStatListFromFlag, dll_base + 0x00078200);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nListCount = 10;
			build_worlds(moo_world, original_world, config);

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				for (int nBit = -1; nBit < 32; ++nBit)
				{
					const int nFlag = nBit < 0 ? static_cast<int>(random_unsigned_integer()) : 1 << nBit;

					// Call both implementations
					const auto moo_result = sut(moo_lists[i], nFlag);
					const auto original_result = original(original_lists[i], nFlag);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
					CHECK_EQ(moo_world.find_list_index(moo_result), original_world.find_list_index(original_result));
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8230 (#10481)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetStatListFromUnitAndFlag, dll_base + 0x00078230);

		REPEAT_10();

		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.bWithStatListEx = with_stat_list;
			config.nListCount = 10;
			build_worlds(moo_world, original_world, config);

			const auto moo_units = moo_world.get_units();
			const auto original_units = original_world.get_units();

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				for (int nBit = -1; nBit < 32; ++nBit)
				{
					for (const int nSetFlag : { 0, static_cast<int>(STATLIST_SET) })
					{
						const int nFlag = (nBit < 0 ? static_cast<int>(random_unsigned_integer()) : 1 << nBit) | nSetFlag;

						// Call both implementations
						const auto moo_result = sut(moo_units[i], nFlag);
						const auto original_result = original(original_units[i], nFlag);

						// Compare return values
						MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
						CHECK_EQ(moo_world.find_list_index(moo_result), original_world.find_list_index(original_result));
					}
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8270 (#10483)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetStatListFromUnitStateOrFlag, dll_base + 0x00078270);

		REPEAT_10();

		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.bWithStatListEx = with_stat_list;
			config.nListCount = 10;
			build_worlds(moo_world, original_world, config);

			const auto moo_units = moo_world.get_units();
			const auto original_units = original_world.get_units();

			// Use the states of the lists, as well as random states
			std::vector<int> states{ 0 };
			for (auto pStatList : moo_world.get_lists())
			{
				states.push_back(pStatList->dwStateNo);
			}
			for (int i = 0; i < 10; ++i)
			{
				states.push_back(random_unsigned_integer(0, states_record_count - 1));
			}

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				for (const auto nState : states)
				{
					for (int nBit = -1; nBit < 32; ++nBit)
					{
						const int nFlag = (nBit < 0 ? static_cast<int>(random_unsigned_integer()) : 1 << nBit) | (random_unsigned_integer(0, 1) ? STATLIST_SET : 0);

						// Call both implementations
						const auto moo_result = sut(moo_units[i], nState, nFlag);
						const auto original_result = original(original_units[i], nState, nFlag);

						// Compare return values
						MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
						CHECK_EQ(moo_world.find_list_index(moo_result), original_world.find_list_index(original_result));
					}
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB82C0 (#10484)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetStatListFromUnitStateAndFlag, dll_base + 0x000782C0);

		REPEAT_10();

		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.bWithStatListEx = with_stat_list;
			config.nListCount = 10;
			build_worlds(moo_world, original_world, config);

			const auto moo_units = moo_world.get_units();
			const auto original_units = original_world.get_units();

			// Use the states of the lists, as well as random states
			std::vector<int> states{ 0 };
			for (auto pStatList : moo_world.get_lists())
			{
				states.push_back(pStatList->dwStateNo);
			}
			for (int i = 0; i < 10; ++i)
			{
				states.push_back(random_unsigned_integer(0, states_record_count - 1));
			}

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				for (const auto nState : states)
				{
					for (int nBit = -2; nBit < 32; ++nBit)
					{
						const int nFlag = (nBit == -2 ? 0 : nBit < 0 ? static_cast<int>(random_unsigned_integer()) : 1 << nBit) | (random_unsigned_integer(0, 1) ? STATLIST_SET : 0);

						// Call both implementations
						const auto moo_result = sut(moo_units[i], nState, nFlag);
						const auto original_result = original(original_units[i], nState, nFlag);

						// Compare return values
						MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
						CHECK_EQ(moo_world.find_list_index(moo_result), original_world.find_list_index(original_result));
					}
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8310 (#10523)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_MergeStatLists, dll_base + 0x00078310);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.nItemCount = StatListWorld::max_items;
			build_worlds(moo_world, original_world, config);

			with_callbacks(moo_world, [&] { sut(nullptr, &moo_world.items[0], TRUE); });
			with_callbacks(original_world, [&] { original(nullptr, &original_world.items[0], TRUE); });
			with_callbacks(moo_world, [&] { sut(&moo_world.unit, nullptr, TRUE); });
			with_callbacks(original_world, [&] { original(&original_world.unit, nullptr, TRUE); });

			// Merge the items several times, which toggles between the different states
			for (int i = 0; i < 4 * StatListWorld::max_items; ++i)
			{
				const auto item_index = random_unsigned_integer(0, StatListWorld::max_items - 1);
				const BOOL bType = random_unsigned_integer(0, 1);
				if (random_unsigned_integer(0, 3) == 0)
				{
					moo_world.item_data[item_index].nBodyLoc = random_unsigned_integer(0, NUM_BODYLOC - 1);
					original_world.item_data[item_index].nBodyLoc = moo_world.item_data[item_index].nBodyLoc;
				}

				// Call both implementations
				with_callbacks(moo_world, [&] { sut(&moo_world.unit, &moo_world.items[item_index], bType); });
				with_callbacks(original_world, [&] { original(&original_world.unit, &original_world.items[item_index], bType); });

				// Compare potentially modified input data
				check_worlds(moo_world, original_world);
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB83A0 (#10535)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetOwner, dll_base + 0x000783A0);

		REPEAT_10();

		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.bWithStatListEx = with_stat_list;
			config.nItemCount = StatListWorld::max_items;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				for (const auto with_output : { 0, 1 })
				{
					BOOL moo_bStatNotDynamic = -1;
					BOOL original_bStatNotDynamic = -1;

					// Call both implementations
					const auto moo_result = sut(moo_units[i], with_output ? &moo_bStatNotDynamic : nullptr);
					const auto original_result = original(original_units[i], with_output ? &original_bStatNotDynamic : nullptr);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
					CHECK_EQ(moo_result == &moo_world.unit, original_result == &original_world.unit);
					MOO_CHECK_EQ(moo_bStatNotDynamic, original_bStatNotDynamic, "Comparing pStatNotDynamic");
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8420 (#10512)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10512, dll_base + 0x00078420);

		REPEAT_10();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			config.nBaseStatCount = 20;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				for (int nStatId = 0; nStatId < itemstatcost_record_count; ++nStatId)
				{
					const auto with_unit2 = random_unsigned_integer(0, 1);

					// Call both implementations
					with_callbacks(moo_world, [&] { sut(moo_units[i], with_unit2 ? &moo_world.items[0] : nullptr, nStatId, RecordStat); });
					with_callbacks(original_world, [&] { original(original_units[i], with_unit2 ? &original_world.items[0] : nullptr, nStatId, RecordStat); });
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB84E0 (#10513)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10513, dll_base + 0x000784E0);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			config.nBaseStatCount = 20;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				for (const auto with_unit2 : { 0, 1 })
				{
					// Call both implementations
					with_callbacks(moo_world, [&] { sut(moo_units[i], with_unit2 ? &moo_world.items[0] : nullptr, RecordStat); });
					with_callbacks(original_world, [&] { original(original_units[i], with_unit2 ? &original_world.items[0] : nullptr, RecordStat); });
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB85D0 (#10511)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_FreeModStats, dll_base + 0x000785D0);

		REPEAT_10();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				// Call both implementations
				sut(moo_units[i]);
				original(original_units[i]);
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8620 (#10562)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetUnitAlignment, dll_base + 0x00078620);

		REPEAT_5();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_OBJECT, UNIT_MISSILE, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			for (int alignment = -1; alignment <= UNIT_ALIGNMENT_GOOD + 1; ++alignment)
			{
				// Input data
				StatListWorld moo_world;
				StatListWorld original_world;

				StatListWorldConfig config;
				config.nUnitType = unit_type;
				config.bWithStatListEx = with_stat_list;
				config.nAlignment = alignment;
				build_worlds(moo_world, original_world, config);

				// Call both implementations
				const auto moo_result = sut(&moo_world.unit);
				const auto original_result = original(&original_world.unit);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				const auto moo_result_null = sut(nullptr);
				const auto original_result_null = original(nullptr);
				MOO_CHECK_EQ(moo_result_null, original_result_null, "Comparing results (null unit)");

				// Compare potentially modified input data
				check_worlds(moo_world, original_world);
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8750 (#10534)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10534, dll_base + 0x00078750);

		REPEAT_10();

		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				// Call both implementations
				sut(moo_units[i]);
				original(original_units[i]);
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8770 (#10530)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10530_D2CheckStatlistFlagDMGRed, dll_base + 0x00078770);

		REPEAT_10();

		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				// Set the flag on some units
				if (moo_units[i] && moo_units[i]->pStatListEx && random_unsigned_integer(0, 1))
				{
					moo_units[i]->pStatListEx->dwFlags |= STATLIST_UNK_0x100;
					original_units[i]->pStatListEx->dwFlags |= STATLIST_UNK_0x100;
				}

				// Call both implementations
				const auto moo_result = sut(moo_units[i]);
				const auto original_result = original(original_units[i]);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB87A0 (#10532)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetTotalStatValue_Layer0, dll_base + 0x000787A0);

		REPEAT_5();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			build_worlds(moo_world, original_world, config);

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				for (int nStatId = -1; nStatId <= itemstatcost_record_count; ++nStatId)
				{
					// Call both implementations
					const auto moo_result = sut(moo_lists[i], nStatId);
					const auto original_result = original(original_lists[i], nStatId);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8890 (#10533)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_RemoveAllStatsFromOverlay, dll_base + 0x00078890);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			config.nListCount = 10;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				// Call both implementations
				with_callbacks(moo_world, [&] { sut(moo_units[i]); });
				with_callbacks(original_world, [&] { original(original_units[i]); });

				// Compare potentially modified input data
				check_worlds(moo_world, original_world);
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8900")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_STATES_ToggleState_6FDB8900, dll_base + 0x00078900);

		REPEAT_5();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_MISSILE);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			const uint32_t unit_flags_ex = random_unsigned_integer();
			moo_world.unit.dwFlagEx = unit_flags_ex;
			original_world.unit.dwFlagEx = unit_flags_ex;

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				for (int j = 0; j < 2 * states_record_count; ++j)
				{
					const int nState = random_unsigned_integer(0, states_record_count - 1);
					const BOOL bSet = random_unsigned_integer(0, 1);

					// Call both implementations
					sut(moo_units[i], nState, bSet);
					original(original_units[i], nState, bSet);
				}

				// Compare potentially modified input data
				check_worlds(moo_world, original_world);
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8A90")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_STATES_GetStatFlags_6FDB8A90, dll_base + 0x00078A90);

		REPEAT_10();

		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				// Fill the flags with random values
				if (moo_units[i] && moo_units[i]->pStatListEx)
				{
					for (int j = 0; j < 2 * flag_count(); ++j)
					{
						const auto new_flag = random_unsigned_integer();
						moo_units[i]->pStatListEx->StatFlags[j] = new_flag;
						original_units[i]->pStatListEx->StatFlags[j] = new_flag;
					}
				}

				// Call both implementations
				const auto moo_result = sut(moo_units[i]);
				const auto original_result = original(original_units[i]);

				// Compare return values
				REQUIRE_EQ(moo_result == nullptr, original_result == nullptr);
				if (moo_result)
				{
					auto wrapped_moo_result = DynamicArray<uint32_t>{ moo_result, 2 * flag_count() };
					auto wrapped_original_result = DynamicArray<uint32_t>{ original_result, 2 * flag_count() };
					MOO_CHECK_EQ(wrapped_moo_result, wrapped_original_result, "Comparing results");
					CHECK_EQ(moo_result, moo_units[i]->pStatListEx->StatFlags);
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8AC0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_STATES_GetListGfxFlags_6FDB8AC0, dll_base + 0x00078AC0);

		REPEAT_10();

		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				// Fill the flags with random values
				if (moo_units[i] && moo_units[i]->pStatListEx)
				{
					for (int j = 0; j < 2 * flag_count(); ++j)
					{
						const auto new_flag = random_unsigned_integer();
						moo_units[i]->pStatListEx->StatFlags[j] = new_flag;
						original_units[i]->pStatListEx->StatFlags[j] = new_flag;
					}
				}

				// Call both implementations
				const auto moo_result = sut(moo_units[i]);
				const auto original_result = original(original_units[i]);

				// Compare return values
				REQUIRE_EQ(moo_result == nullptr, original_result == nullptr);
				if (moo_result)
				{
					auto wrapped_moo_result = DynamicArray<uint32_t>{ moo_result, flag_count() };
					auto wrapped_original_result = DynamicArray<uint32_t>{ original_result, flag_count() };
					MOO_CHECK_EQ(wrapped_moo_result, wrapped_original_result, "Comparing results");
					CHECK_EQ(moo_result, &moo_units[i]->pStatListEx->StatFlags[flag_count()]);
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8BA0 (#11268)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetFullStatsDataFromUnit, dll_base + 0x00078BA0);

		REPEAT_10();

		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				const int nStatCount = moo_units[i] && moo_units[i]->pStatListEx ? moo_units[i]->pStatListEx->FullStats.nStatCount : 0;
				for (int nBufferSize = -1; nBufferSize <= nStatCount + 2; ++nBufferSize)
				{
					// The buffer is filled with garbage, to make sure only the right elements are written
					std::vector<D2StatStrc> moo_buffer(nStatCount + 3);
					for (auto& stat : moo_buffer)
					{
						stat.nPackedValue = random_unsigned_integer();
						stat.nValue = random_unsigned_integer();
					}
					std::vector<D2StatStrc> original_buffer = moo_buffer;

					// Call both implementations
					const auto moo_result = sut(moo_units[i], moo_buffer.data(), nBufferSize);
					const auto original_result = original(original_units[i], original_buffer.data(), nBufferSize);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

					// Compare potentially modified input data
					auto wrapped_moo_buffer = DynamicArray<D2StatStrc>{ moo_buffer.data(), static_cast<int>(moo_buffer.size()) };
					auto wrapped_original_buffer = DynamicArray<D2StatStrc>{ original_buffer.data(), static_cast<int>(original_buffer.size()) };
					MOO_CHECK_EQ(wrapped_moo_buffer, wrapped_original_buffer, "Comparing pOutStatBuffer");
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8C00 (#11243)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetBaseStatsData, dll_base + 0x00078C00);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				const int nStatCount = moo_lists[i] ? moo_lists[i]->Stats.nStatCount : 0;
				for (int nBufferSize = -1; nBufferSize <= nStatCount + 2; ++nBufferSize)
				{
					// The buffer is filled with garbage, to make sure only the right elements are written
					std::vector<D2StatStrc> moo_buffer(nStatCount + 3);
					for (auto& stat : moo_buffer)
					{
						stat.nPackedValue = random_unsigned_integer();
						stat.nValue = random_unsigned_integer();
					}
					std::vector<D2StatStrc> original_buffer = moo_buffer;

					// Call both implementations
					const auto moo_result = sut(moo_lists[i], moo_buffer.data(), nBufferSize);
					const auto original_result = original(original_lists[i], original_buffer.data(), nBufferSize);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

					// Compare potentially modified input data
					auto wrapped_moo_buffer = DynamicArray<D2StatStrc>{ moo_buffer.data(), static_cast<int>(moo_buffer.size()) };
					auto wrapped_original_buffer = DynamicArray<D2StatStrc>{ original_buffer.data(), static_cast<int>(original_buffer.size()) };
					MOO_CHECK_EQ(wrapped_moo_buffer, wrapped_original_buffer, "Comparing pOutStatBuffer");
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8C50 (#10573)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_MergeBaseStats, dll_base + 0x00078C50);

		REPEAT_10();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			build_worlds(moo_world, original_world, config);

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			for (int j = 0; j < 10; ++j)
			{
				const auto target = random_unsigned_integer(0, moo_lists.size() - 1);
				const auto source = random_unsigned_integer(0, moo_lists.size() - 1);

				// Call both implementations
				with_callbacks(moo_world, [&] { sut(moo_lists[target], moo_lists[source]); });
				with_callbacks(original_world, [&] { original(original_lists[target], original_lists[source]); });

				// Compare potentially modified input data
				check_worlds(moo_world, original_world);
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8CA0 (#10477)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_SetStatRemoveCallback, dll_base + 0x00078CA0);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				const StatListRemoveCallback pfStatRemove = random_unsigned_integer(0, 1) ? RecordStatRemove : nullptr;

				// Call both implementations
				sut(moo_lists[i], pfStatRemove);
				original(original_lists[i], pfStatRemove);
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8CC0 (#10469)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10469, dll_base + 0x00078CC0);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			config.nListCount = 10;
			build_worlds(moo_world, original_world, config);

			// Call both implementations
			with_callbacks(moo_world, [&] { sut(&moo_world.unit); });
			with_callbacks(original_world, [&] { original(&original_world.unit); });
			sut(nullptr);
			original(nullptr);

			// Some lists were freed
			std::fill(moo_world.lists.begin(), moo_world.lists.end(), nullptr);
			std::fill(original_world.lists.begin(), original_world.lists.end(), nullptr);

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8D30 (#10514)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_ClampStaminaManaHP, dll_base + 0x00078D30);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				// Call both implementations
				with_callbacks(moo_world, [&] { sut(moo_units[i]); });
				with_callbacks(original_world, [&] { original(original_units[i]); });

				// Compare potentially modified input data
				check_worlds(moo_world, original_world);
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8EB0 (#10574)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10574, dll_base + 0x00078EB0);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			config.nListCount = 10;
			build_worlds(moo_world, original_world, config);

			// Use the states of the lists, as well as random states
			std::vector<int> states{ 0 };
			for (auto pStatList : moo_world.get_lists())
			{
				states.push_back(pStatList->dwStateNo);
			}
			for (int i = 0; i < 5; ++i)
			{
				states.push_back(random_unsigned_integer(0, states_record_count - 1));
			}

			for (int j = 0; j < 20; ++j)
			{
				const int nStateId = states[random_unsigned_integer(0, states.size() - 1)];
				const BOOL bSet = random_unsigned_integer(0, 1);

				// Call both implementations
				const auto moo_result = with_callbacks(moo_world, [&] { return sut(&moo_world.unit, nStateId, bSet); });
				const auto original_result = with_callbacks(original_world, [&] { return original(&original_world.unit, nStateId, bSet); });

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				check_worlds(moo_world, original_world);
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB8F30 (#10525)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10525, dll_base + 0x00078F30);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_OBJECT);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bByTimeStats = TRUE;
			config.nItemCount = StatListWorld::max_items;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				// pDrlgAct is null so the period of day never changes: make the stored values stale so that the recomputation is observable
				const int nStaleValue = random_unsigned_integer(1, 100000);
				make_by_time_op_stats_stale(moo_world, nStaleValue);
				make_by_time_op_stats_stale(original_world, nStaleValue);

				// Call both implementations
				with_callbacks(moo_world, [&] { sut(&moo_world.unit, moo_units[i]); });
				with_callbacks(original_world, [&] { original(&original_world.unit, original_units[i]); });

				// Compare potentially modified input data
				check_worlds(moo_world, original_world);
			}

			sut(nullptr, &moo_world.items[0]);
			original(nullptr, &original_world.items[0]);
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB91C0 (#10474)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10474, dll_base + 0x000791C0);

		REPEAT_20();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			sut(&moo_world.unit, nullptr);
			original(&original_world.unit, nullptr);

			const auto moo_lists = moo_world.lists;
			const auto original_lists = original_world.lists;

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				// Call both implementations
				with_callbacks(moo_world, [&] { sut(&moo_world.unit, moo_lists[i]); });
				with_callbacks(original_world, [&] { original(&original_world.unit, original_lists[i]); });

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_lists[i], original_lists[i], "Comparing pStatList");
				check_worlds(moo_world, original_world);

				// The expired lists are no longer part of the unit, make sure they are freed
				moo_world.forget_list(moo_lists[i]);
				original_world.forget_list(original_lists[i]);
				moo_world.detached_lists.push_back(moo_lists[i]);
				original_world.detached_lists.push_back(original_lists[i]);
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB91D0 (#10564)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetMaxLifeFromUnit, dll_base + 0x000791D0);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				// Call both implementations
				const auto moo_result = sut(moo_units[i]);
				const auto original_result = original(original_units[i]);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB92C0 (#10565)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetMaxManaFromUnit, dll_base + 0x000792C0);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				// Call both implementations
				const auto moo_result = sut(moo_units[i]);
				const auto original_result = original(original_units[i]);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB93B0 (#10566)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetMaxStaminaFromUnit, dll_base + 0x000793B0);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				// Call both implementations
				const auto moo_result = sut(moo_units[i]);
				const auto original_result = original(original_units[i]);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB94A0 (#10567)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetMaxDurabilityFromUnit, dll_base + 0x000794A0);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			// Make sure the stat is present on some units
			for (auto& world : { &moo_world, &original_world })
			{
				if (world->items[0].pStatListEx)
				{
					native.SetStat(world->items[0].pStatListEx, STAT_MAXDURABILITY, 250, 0);
				}
			}

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				// Call both implementations
				const auto moo_result = sut(moo_units[i]);
				const auto original_result = original(original_units[i]);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB95D0 (#10568)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetMaxDamageFromUnit, dll_base + 0x000795D0);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			// Make sure the stats are present on some units
			for (auto& world : { &moo_world, &original_world })
			{
				if (world->items[0].pStatListEx)
				{
					native.SetStat(world->items[0].pStatListEx, STAT_MAXDAMAGE, 30, 0);
					native.SetStat(world->items[0].pStatListEx, STAT_SECONDARY_MAXDAMAGE, 45, 0);
				}
			}

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				for (const BOOL b2Handed : { FALSE, TRUE, 2 })
				{
					// Call both implementations
					const auto moo_result = sut(moo_units[i], b2Handed);
					const auto original_result = original(original_units[i], b2Handed);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB96F0 (#10569)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetMinDamageFromUnit, dll_base + 0x000796F0);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			// Make sure the stats are present on some units
			for (auto& world : { &moo_world, &original_world })
			{
				if (world->items[0].pStatListEx)
				{
					native.SetStat(world->items[0].pStatListEx, STAT_MINDAMAGE, 3, 0);
					native.SetStat(world->items[0].pStatListEx, STAT_SECONDARY_MINDAMAGE, 7, 0);
				}
			}

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				for (const BOOL b2Handed : { FALSE, TRUE, 2 })
				{
					// Call both implementations
					const auto moo_result = sut(moo_units[i], b2Handed);
					const auto original_result = original(original_units[i], b2Handed);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB9810 (#10570)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetMaxThrowDamageFromUnit, dll_base + 0x00079810);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			// Make sure the stat is present on some units
			for (auto& world : { &moo_world, &original_world })
			{
				if (world->items[0].pStatListEx)
				{
					native.SetStat(world->items[0].pStatListEx, STAT_ITEM_THROW_MAXDAMAGE, 25, 0);
				}
			}

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				// Call both implementations
				const auto moo_result = sut(moo_units[i]);
				const auto original_result = original(original_units[i]);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB9900 (#10571)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetMinThrowDamageFromUnit, dll_base + 0x00079900);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			// Make sure the stat is present on some units
			for (auto& world : { &moo_world, &original_world })
			{
				if (world->items[0].pStatListEx)
				{
					native.SetStat(world->items[0].pStatListEx, STAT_ITEM_THROW_MINDAMAGE, 12, 0);
				}
			}

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				// Call both implementations
				const auto moo_result = sut(moo_units[i]);
				const auto original_result = original(original_units[i]);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB99F0 (#10572)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetDefenseFromUnit, dll_base + 0x000799F0);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			// Make sure the stat is present on some units
			for (auto& world : { &moo_world, &original_world })
			{
				if (world->items[0].pStatListEx)
				{
					native.SetStat(world->items[0].pStatListEx, STAT_ARMORCLASS, 120, 0);
				}
			}

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				// Call both implementations
				const auto moo_result = sut(moo_units[i]);
				const auto original_result = original(original_units[i]);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB9AE0 (#10524)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_ExpireUnitStatlist, dll_base + 0x00079AE0);

		REPEAT_20();

		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.bWithStatListEx = with_stat_list;
			config.nItemCount = StatListWorld::max_items;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			// Expire the items before the unit
			std::reverse(moo_units.begin(), moo_units.end());
			std::reverse(original_units.begin(), original_units.end());

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				// Call both implementations
				with_callbacks(moo_world, [&] { sut(&moo_world.unit, moo_units[i]); });
				with_callbacks(original_world, [&] { original(&original_world.unit, original_units[i]); });

				// Compare potentially modified input data
				check_worlds(moo_world, original_world);
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB9B00 (#10531)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10531_SetStatInStatListLayer0, dll_base + 0x00079B00);

		REPEAT_10();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			build_worlds(moo_world, original_world, config);

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			std::mt19937 rng(random_unsigned_integer());
			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				for (int j = 0; j < 10; ++j)
				{
					const int nStatId = random_stat_id(rng);
					const int nValue = rng() % 4 == 0 ? 0 : random_stat_value(rng);
					const int nUnused = rng();

					// Call both implementations
					const auto moo_result = with_callbacks(moo_world, [&] { return sut(moo_lists[i], nStatId, nValue, nUnused); });
					const auto original_result = with_callbacks(original_world, [&] { return original(original_lists[i], nStatId, nValue, nUnused); });

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

					// Compare potentially modified input data
					check_worlds(moo_world, original_world);
				}
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB9B10 (#11248)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11248, dll_base + 0x00079B10);

		REPEAT_5();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			auto moo_units = moo_world.get_units();
			auto original_units = original_world.get_units();
			moo_units.push_back(nullptr);
			original_units.push_back(nullptr);

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				for (int nStatId = -1; nStatId <= itemstatcost_record_count; ++nStatId)
				{
					// Call both implementations
					const auto moo_result = sut(&moo_world.unit, moo_units[i], nStatId);
					const auto original_result = original(&original_world.unit, original_units[i], nStatId);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDA9E60 (#11264)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_SetSkillId, dll_base + 0x00069E60);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			// The original does not check for null stat lists
			const auto moo_lists = moo_world.get_lists();
			const auto original_lists = original_world.get_lists();

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				const int nSkillId = random_unsigned_integer();

				// Call both implementations
				sut(moo_lists[i], nSkillId);
				original(original_lists[i], nSkillId);
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB9C10 (#11265)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetSkillId, dll_base + 0x00079C10);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			// The original does not check for null stat lists
			const auto moo_lists = moo_world.get_lists();
			const auto original_lists = original_world.get_lists();

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				// Call both implementations
				const auto moo_result = sut(moo_lists[i]);
				const auto original_result = original(original_lists[i]);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB9C20 (#11266)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_SetSkillLevel, dll_base + 0x00079C20);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			// The original does not check for null stat lists
			const auto moo_lists = moo_world.get_lists();
			const auto original_lists = original_world.get_lists();

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				const int nSkillLevel = random_unsigned_integer(0, 1) ? random_unsigned_integer(0, 60) : random_unsigned_integer();

				// Call both implementations
				sut(moo_lists[i], nSkillLevel);
				original(original_lists[i], nSkillLevel);
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDA9E70 (#11267)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_GetSkillLevel, dll_base + 0x00069E70);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;
			build_worlds(moo_world, original_world, {});

			// The original does not check for null stat lists
			const auto moo_lists = moo_world.get_lists();
			const auto original_lists = original_world.get_lists();

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				// Call both implementations
				const auto moo_result = sut(moo_lists[i]);
				const auto original_result = original(original_lists[i]);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB9C30 (#11269)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_11269_CopyStats, dll_base + 0x00079C30);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nBaseStatCount = 30;
			build_worlds(moo_world, original_world, config);

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				for (int nStatId = 0; nStatId < itemstatcost_record_count; ++nStatId)
				{
					const int nBufferSize = random_unsigned_integer(0, 4);

					// The buffer is filled with garbage, to make sure only the right elements are written
					D2StatStrc moo_buffer[5]{};
					for (auto& stat : moo_buffer)
					{
						stat.nPackedValue = random_unsigned_integer();
						stat.nValue = random_unsigned_integer();
					}
					D2StatStrc original_buffer[5]{};
					std::copy(std::begin(moo_buffer), std::end(moo_buffer), std::begin(original_buffer));

					// Call both implementations
					const auto moo_result = sut(static_cast<D2StatListExStrc*>(moo_lists[i]), nStatId, moo_buffer, nBufferSize);
					const auto original_result = original(static_cast<D2StatListExStrc*>(original_lists[i]), nStatId, original_buffer, nBufferSize);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

					// Compare potentially modified input data
					auto wrapped_moo_buffer = DynamicArray<D2StatStrc>{ moo_buffer, static_cast<int>(std::size(moo_buffer)) };
					auto wrapped_original_buffer = DynamicArray<D2StatStrc>{ original_buffer, static_cast<int>(std::size(original_buffer)) };
					MOO_CHECK_EQ(wrapped_moo_buffer, wrapped_original_buffer, "Comparing pBuffer");
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB9C50")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_CopyStats_6FDB9C50, dll_base + 0x00079C50);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nBaseStatCount = 30;
			build_worlds(moo_world, original_world, config);

			auto moo_lists = moo_world.get_lists();
			auto original_lists = original_world.get_lists();
			moo_lists.push_back(nullptr);
			original_lists.push_back(nullptr);

			for (size_t i = 0; i < moo_lists.size(); ++i)
			{
				for (int nStatId = 0; nStatId < itemstatcost_record_count; ++nStatId)
				{
					const int nBufferSize = random_unsigned_integer(0, 4);

					// The buffer is filled with garbage, to make sure only the right elements are written
					D2StatStrc moo_buffer[5]{};
					for (auto& stat : moo_buffer)
					{
						stat.nPackedValue = random_unsigned_integer();
						stat.nValue = random_unsigned_integer();
					}
					D2StatStrc original_buffer[5]{};
					std::copy(std::begin(moo_buffer), std::end(moo_buffer), std::begin(original_buffer));

					// Call both implementations
					const auto moo_result = sut(moo_lists[i], nStatId, moo_buffer, nBufferSize);
					const auto original_result = original(original_lists[i], nStatId, original_buffer, nBufferSize);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

					// Compare potentially modified input data
					auto wrapped_moo_buffer = DynamicArray<D2StatStrc>{ moo_buffer, static_cast<int>(std::size(moo_buffer)) };
					auto wrapped_original_buffer = DynamicArray<D2StatStrc>{ original_buffer, static_cast<int>(std::size(original_buffer)) };
					MOO_CHECK_EQ(wrapped_moo_buffer, wrapped_original_buffer, "Comparing pBuffer");
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB9D20 (#11270)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(STATLIST_CopyStats, dll_base + 0x00079D20);

		REPEAT_10();

		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.bWithStatListEx = with_stat_list;
			config.nBaseStatCount = 30;
			build_worlds(moo_world, original_world, config);

			const auto moo_units = moo_world.get_units();
			const auto original_units = original_world.get_units();

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				for (int nStatId = 0; nStatId < itemstatcost_record_count; ++nStatId)
				{
					const int nBufferSize = random_unsigned_integer(0, 4);

					// The buffer is filled with garbage, to make sure only the right elements are written
					D2StatStrc moo_buffer[5]{};
					for (auto& stat : moo_buffer)
					{
						stat.nPackedValue = random_unsigned_integer();
						stat.nValue = random_unsigned_integer();
					}
					D2StatStrc original_buffer[5]{};
					std::copy(std::begin(moo_buffer), std::end(moo_buffer), std::begin(original_buffer));

					// Call both implementations
					const auto moo_result = sut(moo_units[i], nStatId, moo_buffer, nBufferSize);
					const auto original_result = original(original_units[i], nStatId, original_buffer, nBufferSize);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

					// Compare potentially modified input data
					auto wrapped_moo_buffer = DynamicArray<D2StatStrc>{ moo_buffer, static_cast<int>(std::size(moo_buffer)) };
					auto wrapped_original_buffer = DynamicArray<D2StatStrc>{ original_buffer, static_cast<int>(std::size(original_buffer)) };
					MOO_CHECK_EQ(wrapped_moo_buffer, wrapped_original_buffer, "Comparing pBuffer");
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB9D60 (#11273)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11273, dll_base + 0x00079D60);

		REPEAT_5();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER, UNIT_ITEM);
		const auto with_stat_list = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.bWithStatListEx = with_stat_list;
			build_worlds(moo_world, original_world, config);

			const auto moo_units = moo_world.get_units();
			const auto original_units = original_world.get_units();

			for (size_t i = 0; i < moo_units.size(); ++i)
			{
				for (int nStatId = 0; nStatId < itemstatcost_record_count; ++nStatId)
				{
					// Call both implementations
					const auto moo_result = sut(moo_units[i], nStatId);
					const auto original_result = original(original_units[i], nStatId);

					// Compare return values
					MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				}
			}

			// Compare potentially modified input data
			check_worlds(moo_world, original_world);
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB9D90 (#11274)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11274, dll_base + 0x00079D90);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.nItemCount = StatListWorld::max_items;
			build_worlds(moo_world, original_world, config);

			const auto moo_result_null1 = sut(nullptr, &moo_world.items[0]);
			const auto original_result_null1 = original(nullptr, &original_world.items[0]);
			MOO_CHECK_EQ(moo_result_null1, original_result_null1, "Comparing results (null target)");
			const auto moo_result_null2 = sut(&moo_world.unit, nullptr);
			const auto original_result_null2 = original(&original_world.unit, nullptr);
			MOO_CHECK_EQ(moo_result_null2, original_result_null2, "Comparing results (null unit)");

			for (int i = 0; i < 4 * StatListWorld::max_items; ++i)
			{
				const auto item_index = random_unsigned_integer(0, StatListWorld::max_items - 1);
				if (random_unsigned_integer(0, 3) == 0)
				{
					moo_world.item_data[item_index].nBodyLoc = random_unsigned_integer(0, NUM_BODYLOC - 1);
					original_world.item_data[item_index].nBodyLoc = moo_world.item_data[item_index].nBodyLoc;
				}

				// Call both implementations
				const auto moo_result = with_callbacks(moo_world, [&] { return sut(&moo_world.unit, &moo_world.items[item_index]); });
				const auto original_result = with_callbacks(original_world, [&] { return original(&original_world.unit, &original_world.items[item_index]); });

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				check_worlds(moo_world, original_world);
			}
		}
	}

	TEST_CASE_FIXTURE(StatListFixture, "D2Common.0x6FDB9E60 (#11275)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11275, dll_base + 0x00079E60);

		REPEAT_20();

		const auto unit_type = GENERATE(UNIT_PLAYER, UNIT_MONSTER);

		SUBCASE("")
		{
			// Input data
			StatListWorld moo_world;
			StatListWorld original_world;

			StatListWorldConfig config;
			config.nUnitType = unit_type;
			config.nItemCount = StatListWorld::max_items;
			build_worlds(moo_world, original_world, config);

			const auto moo_result_null1 = sut(nullptr, &moo_world.items[0]);
			const auto original_result_null1 = original(nullptr, &original_world.items[0]);
			MOO_CHECK_EQ(moo_result_null1, original_result_null1, "Comparing results (null target)");
			const auto moo_result_null2 = sut(&moo_world.unit, nullptr);
			const auto original_result_null2 = original(&original_world.unit, nullptr);
			MOO_CHECK_EQ(moo_result_null2, original_result_null2, "Comparing results (null unit)");

			for (int i = 0; i < 4 * StatListWorld::max_items; ++i)
			{
				const auto item_index = random_unsigned_integer(0, StatListWorld::max_items - 1);
				if (random_unsigned_integer(0, 3) == 0)
				{
					moo_world.item_data[item_index].nBodyLoc = random_unsigned_integer(0, NUM_BODYLOC - 1);
					original_world.item_data[item_index].nBodyLoc = moo_world.item_data[item_index].nBodyLoc;
				}

				// Call both implementations
				const auto moo_result = with_callbacks(moo_world, [&] { return sut(&moo_world.unit, &moo_world.items[item_index]); });
				const auto original_result = with_callbacks(original_world, [&] { return original(&original_world.unit, &original_world.items[item_index]); });

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				check_worlds(moo_world, original_world);
			}
		}
	}
}

#endif //STATLIST_TESTS
