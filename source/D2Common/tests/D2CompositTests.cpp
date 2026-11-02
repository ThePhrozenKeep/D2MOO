#include <D2CommonTestDefines.h>

#ifdef COMPOSIT_TESTS

#include <doctest.h>

#include <Windows.h>

#include <climits>
#include <cstdarg>
#include <filesystem>
#include <vector>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2BitManip.h>
#include <D2Composit.h>
#include <D2Inventory.h>
#include <D2Items.h>
#include <D2Monsters.h>
#include <D2States.h>
#include <Units/Units.h>

#include <Fixtures/DataTbls/Fixtures.h>


DYNAMIC_ARRAY_TYPE(uint8_t)


namespace
{
	constexpr D2UnitGUID composit_player_guid = 1;
	constexpr D2UnitGUID composit_right_hand_guid = 1001;
	constexpr D2UnitGUID composit_left_hand_guid = 1002;

	// Only checked for presence to pick the sequence mode
	D2AnimSeqTxt composit_anim_seq{};

	struct CompositHandItem
	{
		int nClassId = -1;
		int nUnitType = UNIT_ITEM;
		uint32_t dwItemFlags = 0;
	};

	struct CompositUnitSetup
	{
		int nUnitType = UNIT_PLAYER;
		int nClassId = 0;
		int nAnimMode = 0;
		int nSeqMode = 0;
		BOOL bAnimSeq = FALSE;
		BOOL bInventory = TRUE;
		BOOL bLinkInventory = FALSE;
		CompositHandItem pRightHand{};
		CompositHandItem pLeftHand{};
		D2UnitGUID dwLeftItemGUID = D2UnitInvalidGUID;
		int nTransformState = -1;
	};

	// Holds a unit together with its inventory (only the body location grid is created), the items held in both hands and an extended stat list for states
	struct CompositUnitData
	{
		D2UnitStrc pUnit{};
		D2InventoryStrc pInventory{};
		D2InventoryGridStrc pBodyLocGrid{};
		D2UnitStrc* ppBodyLocItems[NUM_BODYLOC]{};
		D2UnitStrc pRightHandItem{};
		D2ItemDataStrc pRightHandItemData{};
		D2UnitStrc pLeftHandItem{};
		D2ItemDataStrc pLeftHandItemData{};
		D2StatListExStrc pStatListEx{};
		std::vector<uint32_t> StatFlags;

		CompositUnitData() = default;
		CompositUnitData(const CompositUnitData&) = delete;
		CompositUnitData& operator=(const CompositUnitData&) = delete;

		void Setup(const CompositUnitSetup& setup)
		{
			pUnit.dwUnitType = setup.nUnitType;
			pUnit.dwClassId = setup.nClassId;
			pUnit.dwUnitId = composit_player_guid;
			pUnit.dwAnimMode = setup.nAnimMode;
			pUnit.dwSeqMode = setup.nSeqMode;
			pUnit.pAnimSeq = setup.bAnimSeq ? &composit_anim_seq : nullptr;

			if (setup.bInventory)
			{
				pInventory.dwSignature = D2C_InventoryHeader;
				pInventory.pGrids = &pBodyLocGrid;
				pInventory.nGridCount = INVGRID_BODYLOC + 1;
				pInventory.dwLeftItemGUID = setup.dwLeftItemGUID;
				pInventory.dwOwnerGuid = composit_player_guid;

				pBodyLocGrid.nGridWidth = NUM_BODYLOC;
				pBodyLocGrid.nGridHeight = 1;
				pBodyLocGrid.ppItems = ppBodyLocItems;

				const auto setup_item = [this](D2UnitStrc& pItem, D2ItemDataStrc& pItemData, const CompositHandItem& item, D2UnitGUID nGUID, int nBodyLoc)
				{
					if (item.nClassId < 0)
					{
						return;
					}

					pItem.dwUnitType = item.nUnitType;
					pItem.dwClassId = item.nClassId;
					pItem.dwUnitId = nGUID;
					if (item.nUnitType == UNIT_ITEM)
					{
						pItem.pItemData = &pItemData;
						pItemData.dwItemFlags = item.dwItemFlags;
						pItemData.nBodyLoc = nBodyLoc;
						pItemData.dwOwnerGUID = composit_player_guid;
					}

					ppBodyLocItems[nBodyLoc] = &pItem;
				};

				setup_item(pRightHandItem, pRightHandItemData, setup.pRightHand, composit_right_hand_guid, BODYLOC_RARM);
				setup_item(pLeftHandItem, pLeftHandItemData, setup.pLeftHand, composit_left_hand_guid, BODYLOC_LARM);

				if (setup.bLinkInventory)
				{
					pUnit.pInventory = &pInventory;
				}
			}

			if (setup.nTransformState >= 0)
			{
				StatFlags.assign(2 * ((sgptDataTables->nStatesTxtRecordCount + 31) / 32), 0);
				StatFlags[setup.nTransformState >> 5] |= gdwBitMasks[setup.nTransformState & 31];

				pStatListEx.dwFlags |= STATLIST_EXTENDED;
				pStatListEx.StatFlags = StatFlags.data();
				pUnit.pStatListEx = &pStatListEx;
				pUnit.dwFlagEx |= UNITFLAGEX_ISSHAPESHIFTED;
			}
		}
	};

	// D2COMMON_11013_ConvertMode only looks at the transform states, which are collected when the data tables are loaded
	struct TransformStatesScope
	{
		std::vector<short> transform_states;
		short* previous_transform_states;
		int previous_transform_state_count;

		TransformStatesScope()
		{
			for (auto i = 0; i < sgptDataTables->nStatesTxtRecordCount; ++i)
			{
				if (sgptDataTables->pStatesTxt[i].dwStateFlags & gdwBitMasks[STATEMASK_DISGUISE])
				{
					transform_states.push_back(static_cast<short>(i));
				}
			}

			previous_transform_states = sgptDataTables->pTransformStates;
			previous_transform_state_count = sgptDataTables->nTransformStates;

			sgptDataTables->pTransformStates = transform_states.data();
			sgptDataTables->nTransformStates = static_cast<int>(transform_states.size());
		}

		~TransformStatesScope()
		{
			sgptDataTables->pTransformStates = previous_transform_states;
			sgptDataTables->nTransformStates = previous_transform_state_count;
		}
	};

	int get_weapons_record_count()
	{
		return sgptDataTables->pItemDataTables.nWeaponsTxtRecordCount;
	}

	int get_armor_record_count()
	{
		return sgptDataTables->pItemDataTables.nArmorTxtRecordCount;
	}

	// Random weapon or armor (shields included) record
	int random_equipment_id()
	{
		return random_unsigned_integer(0, get_weapons_record_count() + get_armor_record_count() - 1);
	}

	uint32_t random_item_flags()
	{
		switch (random_unsigned_integer(0, 9))
		{
		case 0:
			return IFLAG_BROKEN;
		case 1:
			return IFLAG_NOEQUIP;
		default:
			return IFLAG_IDENTIFIED;
		}
	}

	CompositHandItem random_hand_item()
	{
		switch (random_unsigned_integer(0, 3))
		{
		case 0:
			return {};
		case 1:
			return { static_cast<int>(random_unsigned_integer(0, get_weapons_record_count() - 1)), UNIT_ITEM, random_item_flags() };
		case 2:
			return { static_cast<int>(get_weapons_record_count() + random_unsigned_integer(0, get_armor_record_count() - 1)), UNIT_ITEM, random_item_flags() };
		default:
			return { random_equipment_id(), UNIT_ITEM, random_item_flags() };
		}
	}

	D2UnitGUID random_left_item_guid()
	{
		constexpr D2UnitGUID guids[] = { D2UnitInvalidGUID, composit_right_hand_guid, composit_left_hand_guid };
		return guids[random_unsigned_integer(0, 2)];
	}

	CompositUnitSetup random_player_setup(int nClass, int nMode)
	{
		CompositUnitSetup setup{};
		setup.nUnitType = UNIT_PLAYER;
		setup.nClassId = nClass;
		setup.nAnimMode = nMode;
		setup.pRightHand = random_hand_item();
		setup.pLeftHand = random_hand_item();
		setup.dwLeftItemGUID = random_left_item_guid();
		return setup;
	}
}


TEST_SUITE("D2CompositTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));


	TEST_CASE_FIXTURE(PlrModeTypeTxtFixture<ObjModeTypeTxtFixture<MonModeTxtFixture<MonStatsTxtFixture<MonStats2TxtFixture<CharStatsTxtFixture<ItemTypesTxtFixture<ItemsTxtFixture<NoOpFixture>>>>>>>>, "D2Common.0x6FD466C0 (#10884)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10884_COMPOSIT_unk, dll_base + 0x000066C0);

		const auto check_path = [&](const CompositUnitSetup& setup, int nClass, int nMode, int nUnitType, int nWeaponClassCode, BOOL bAddPathPrefix, int a9)
		{
			// Input data
			CompositUnitData moo_data{};
			char moo_szPath[MAX_PATH]{};
			int moo_pWeaponClassCode = nWeaponClassCode;
			CompositUnitData original_data{};
			char original_szPath[MAX_PATH]{};
			int original_pWeaponClassCode = nWeaponClassCode;

			moo_data.Setup(setup);
			original_data.Setup(setup);

			// Call both implementations
			sut(&moo_data.pUnit, nClass, nMode, nUnitType, setup.bInventory ? &moo_data.pInventory : nullptr, moo_szPath, &moo_pWeaponClassCode, bAddPathPrefix, a9);
			original(&original_data.pUnit, nClass, nMode, nUnitType, setup.bInventory ? &original_data.pInventory : nullptr, original_szPath, &original_pWeaponClassCode, bAddPathPrefix, a9);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_data.pUnit, original_data.pUnit, "Comparing pUnit");
			MOO_CHECK_EQ(moo_data.pInventory, original_data.pInventory, "Comparing pInventory");
			MOO_CHECK_EQ(moo_szPath, original_szPath, "Comparing szPath");
			MOO_CHECK_EQ(moo_pWeaponClassCode, original_pWeaponClassCode, "Comparing pWeaponClassCode");
		};

		SUBCASE("Players")
		{
			REPEAT_5();

			for (auto nClass = 0; nClass < NUMBER_OF_PLAYERCLASSES; ++nClass)
			{
				for (auto nMode = 0; nMode < NUMBER_OF_PLRMODES; ++nMode)
				{
					for (auto bAddPathPrefix = 0; bAddPathPrefix < 2; ++bAddPathPrefix)
					{
						for (auto a9 = 0; a9 < 2; ++a9)
						{
							const auto setup = random_player_setup(nClass, nMode);

							// Without a9 the weapon class code is an input (e.g. 1hs while throwing)
							const auto nWeaponClassCode = setup.pRightHand.nClassId >= 0 ? sgptDataTables->pItemDataTables.pItemsTxt[setup.pRightHand.nClassId].dwWeapClass : ' hth';

							check_path(setup, nClass, nMode, UNIT_PLAYER, nWeaponClassCode, bAddPathPrefix, a9);
						}
					}
				}
			}
		}

		SUBCASE("Monsters")
		{
			for (auto nClass = 0; nClass < monstats_record_count; ++nClass)
			{
				for (auto nMode = 0; nMode < NUMBER_OF_MONMODES; ++nMode)
				{
					CompositUnitSetup setup{};
					setup.nUnitType = UNIT_MONSTER;
					setup.nClassId = nClass;
					setup.nAnimMode = nMode;
					setup.bInventory = FALSE;

					check_path(setup, nClass, nMode, UNIT_MONSTER, 0, nMode & 1, nClass & 1);
				}
			}
		}

		SUBCASE("Monsters with invalid class")
		{
			for (const auto nClass : { -1, monstats_record_count, monstats_record_count + 1, INT_MAX, INT_MIN })
			{
				for (auto bAddPathPrefix = 0; bAddPathPrefix < 2; ++bAddPathPrefix)
				{
					CompositUnitSetup setup{};
					setup.nUnitType = UNIT_MONSTER;
					setup.bInventory = FALSE;

					check_path(setup, nClass, MONMODE_NEUTRAL, UNIT_MONSTER, ' 1th', bAddPathPrefix, 1);
				}
			}
		}

		SUBCASE("Objects")
		{
			for (auto nClass = 0; nClass < objtype_record_count; ++nClass)
			{
				for (auto nMode = 0; nMode < objmode_record_count; ++nMode)
				{
					for (auto bAddPathPrefix = 0; bAddPathPrefix < 2; ++bAddPathPrefix)
					{
						CompositUnitSetup setup{};
						setup.nUnitType = UNIT_OBJECT;
						setup.nClassId = nClass;
						setup.nAnimMode = nMode;
						setup.bInventory = FALSE;

						check_path(setup, nClass, nMode, UNIT_OBJECT, 0, bAddPathPrefix, 0);
					}
				}
			}
		}

		SUBCASE("Other unit types")
		{
			for (const int nUnitType : std::initializer_list<int>{ UNIT_MISSILE, UNIT_ITEM, UNIT_TILE, 6, -1 })
			{
				CompositUnitSetup setup{};
				setup.nUnitType = nUnitType;
				setup.bInventory = FALSE;

				check_path(setup, 0, 0, nUnitType, ' sh1', TRUE, 1);
			}
		}
	}

	TEST_CASE_FIXTURE(StatesTxtFixture<PlrModeTypeTxtFixture<ObjModeTypeTxtFixture<MonModeTxtFixture<MonStatsTxtFixture<MonStats2TxtFixture<CharStatsTxtFixture<ItemTypesTxtFixture<ItemsTxtFixture<NoOpFixture>>>>>>>>>, "D2Common.0x6FD46BC0 (#10885)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10885_COMPOSIT_unk, dll_base + 0x00006BC0);

		TransformStatesScope transform_states{};

		const auto check_path = [&](const CompositUnitSetup& setup, int nWeaponClassCode, BOOL bAddPathPrefix, int a5, int nAnimMode)
		{
			// Input data
			CompositUnitData moo_data{};
			char moo_szPath[MAX_PATH]{};
			int moo_pWeaponClassCode = nWeaponClassCode;
			CompositUnitData original_data{};
			char original_szPath[MAX_PATH]{};
			int original_pWeaponClassCode = nWeaponClassCode;

			moo_data.Setup(setup);
			original_data.Setup(setup);

			// Call both implementations
			sut(&moo_data.pUnit, moo_szPath, &moo_pWeaponClassCode, bAddPathPrefix, a5, setup.bInventory ? &moo_data.pInventory : nullptr, nAnimMode);
			original(&original_data.pUnit, original_szPath, &original_pWeaponClassCode, bAddPathPrefix, a5, setup.bInventory ? &original_data.pInventory : nullptr, nAnimMode);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_data.pUnit, original_data.pUnit, "Comparing pUnit");
			MOO_CHECK_EQ(moo_szPath, original_szPath, "Comparing szPath");
			MOO_CHECK_EQ(moo_pWeaponClassCode, original_pWeaponClassCode, "Comparing pWeaponClassCode");
			MOO_CHECK_EQ(moo_data.pInventory, original_data.pInventory, "Comparing pInventory");
		};

		SUBCASE("Players")
		{
			REPEAT_5();

			for (auto nClass = 0; nClass < NUMBER_OF_PLAYERCLASSES; ++nClass)
			{
				for (auto nMode = 0; nMode < NUMBER_OF_PLRMODES; ++nMode)
				{
					auto setup = random_player_setup(nClass, nMode);
					setup.nSeqMode = random_unsigned_integer(0, NUMBER_OF_PLRMODES - 1);
					setup.bAnimSeq = random_unsigned_integer(0, 1);

					const auto nAnimMode = random_unsigned_integer(0, 1) ? -1 : static_cast<int>(random_unsigned_integer(0, NUMBER_OF_PLRMODES - 1));

					check_path(setup, ' hth', random_unsigned_integer(0, 1), 1, nAnimMode);
				}
			}
		}

		SUBCASE("Shapeshifted players")
		{
			for (const auto nTransformState : transform_states.transform_states)
			{
				for (auto nMode = 0; nMode < NUMBER_OF_PLRMODES; ++nMode)
				{
					auto setup = random_player_setup(PCLASS_DRUID, nMode);
					setup.nTransformState = nTransformState;

					check_path(setup, ' hth', TRUE, 1, -1);
					check_path(setup, ' hth', FALSE, 1, MONMODE_NEUTRAL);
				}
			}
		}

		SUBCASE("Monsters")
		{
			for (auto nClass = 0; nClass < monstats_record_count; ++nClass)
			{
				CompositUnitSetup setup{};
				setup.nUnitType = UNIT_MONSTER;
				setup.nClassId = nClass;
				setup.nAnimMode = random_unsigned_integer(0, NUMBER_OF_MONMODES - 1);
				setup.bInventory = FALSE;

				check_path(setup, 0, nClass & 1, 1, -1);
			}
		}

		SUBCASE("Objects")
		{
			for (auto nClass = 0; nClass < objtype_record_count; ++nClass)
			{
				CompositUnitSetup setup{};
				setup.nUnitType = UNIT_OBJECT;
				setup.nClassId = nClass;
				setup.nAnimMode = random_unsigned_integer(0, objmode_record_count - 1);
				setup.bInventory = FALSE;

				check_path(setup, 0, TRUE, 0, -1);
				check_path(setup, 0, FALSE, 0, 0);
			}
		}
	}

	TEST_CASE_FIXTURE(ItemsTxtFixture<NoOpFixture>, "D2Common.0x6FD46C60 (#10886)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COMPOSIT_GetWeaponClassIdFromCode, dll_base + 0x00006C60);

		const auto check_code = [&](int nWeaponClassCode)
		{
			// Call both implementations
			const auto moo_result = sut(nWeaponClassCode);
			const auto original_result = original(nWeaponClassCode);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		};

		SUBCASE("Weapon class codes")
		{
			for (const auto nWeaponClassCode : { ' hth', ' wob', ' sh1', ' th1', ' fts', ' sh2', ' th2', ' wbx', ' sj1', ' tj1', ' ss1', ' ts1', ' 1th', ' 2th' })
			{
				check_code(nWeaponClassCode);
			}
		}

		SUBCASE("Item weapon class codes")
		{
			for (auto i = 0; i < items_record_count; ++i)
			{
				check_code(items_txt[i].dwWeapClass);
				check_code(items_txt[i].dwWeapClass2Hand);
			}
		}

		SUBCASE("Invalid codes")
		{
			for (const auto nWeaponClassCode : { 0, -1, 1, 'hth', '1hs', ' SH1', INT_MAX, INT_MIN })
			{
				check_code(nWeaponClassCode);
			}

			for (auto i = 0; i < 100; ++i)
			{
				check_code(random_unsigned_integer());
			}
		}
	}

	TEST_CASE_FIXTURE(MonStatsTxtFixture<MonStats2TxtFixture<CharStatsTxtFixture<ItemTypesTxtFixture<ItemsTxtFixture<NoOpFixture>>>>>, "D2Common.0x6FD46C90 (#10887)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COMPOSIT_GetWeaponClassCode, dll_base + 0x00006C90);

		const auto check_class_code = [&](const CompositUnitSetup& setup, int nUnitType, int nClass, int nMode)
		{
			// Input data
			CompositUnitData moo_data{};
			int moo_pWeaponClassId = -1;
			CompositUnitData original_data{};
			int original_pWeaponClassId = -1;

			moo_data.Setup(setup);
			original_data.Setup(setup);

			// Call both implementations
			const auto moo_result = sut(&moo_data.pUnit, nUnitType, nClass, nMode, setup.bInventory ? &moo_data.pInventory : nullptr, &moo_pWeaponClassId);
			const auto original_result = original(&original_data.pUnit, nUnitType, nClass, nMode, setup.bInventory ? &original_data.pInventory : nullptr, &original_pWeaponClassId);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_data.pUnit, original_data.pUnit, "Comparing pUnit");
			MOO_CHECK_EQ(moo_data.pInventory, original_data.pInventory, "Comparing pInventory");
			MOO_CHECK_EQ(moo_data.pRightHandItem, original_data.pRightHandItem, "Comparing pRightHandItem");
			MOO_CHECK_EQ(moo_data.pLeftHandItem, original_data.pLeftHandItem, "Comparing pLeftHandItem");
			MOO_CHECK_EQ(moo_pWeaponClassId, original_pWeaponClassId, "Comparing pWeaponClassId");
		};

		SUBCASE("Players with a weapon or armor in the right hand")
		{
			for (auto nClass = 0; nClass < NUMBER_OF_PLAYERCLASSES; ++nClass)
			{
				for (auto nMode = 0; nMode < NUMBER_OF_PLRMODES; ++nMode)
				{
					for (auto nItemId = 0; nItemId < get_weapons_record_count() + get_armor_record_count(); ++nItemId)
					{
						auto setup = random_player_setup(nClass, nMode);
						setup.pRightHand = { nItemId, UNIT_ITEM, random_item_flags() };

						check_class_code(setup, UNIT_PLAYER, nClass, nMode);
					}
				}
			}
		}

		SUBCASE("Players dual wielding")
		{
			REPEAT_10();

			for (auto nClass = 0; nClass < NUMBER_OF_PLAYERCLASSES; ++nClass)
			{
				for (auto i = 0; i < 100; ++i)
				{
					auto setup = random_player_setup(nClass, random_unsigned_integer(PLRMODE_NEUTRAL, PLRMODE_DEAD - 1));
					setup.pRightHand = { static_cast<int>(random_unsigned_integer(0, get_weapons_record_count() - 1)) };
					setup.pLeftHand = { static_cast<int>(random_unsigned_integer(0, get_weapons_record_count() - 1)) };

					check_class_code(setup, UNIT_PLAYER, nClass, setup.nAnimMode);
				}
			}
		}

		SUBCASE("Players without items or inventory")
		{
			for (auto nClass = 0; nClass < NUMBER_OF_PLAYERCLASSES; ++nClass)
			{
				for (auto nMode = 0; nMode < NUMBER_OF_PLRMODES; ++nMode)
				{
					CompositUnitSetup setup{};
					setup.nClassId = nClass;
					setup.nAnimMode = nMode;
					check_class_code(setup, UNIT_PLAYER, nClass, nMode);

					setup.bInventory = FALSE;
					check_class_code(setup, UNIT_PLAYER, nClass, nMode);
				}
			}
		}

		SUBCASE("Players holding a non-item unit")
		{
			for (auto nClass = 0; nClass < NUMBER_OF_PLAYERCLASSES; ++nClass)
			{
				CompositUnitSetup setup{};
				setup.nClassId = nClass;
				setup.nAnimMode = PLRMODE_ATTACK1;
				setup.pRightHand = { 0, UNIT_MONSTER };
				setup.pLeftHand = { 0 };

				check_class_code(setup, UNIT_PLAYER, nClass, PLRMODE_ATTACK1);
			}
		}

		SUBCASE("Monsters")
		{
			for (auto nClass = 0; nClass < monstats_record_count; ++nClass)
			{
				for (auto nMode = 0; nMode < NUMBER_OF_MONMODES; ++nMode)
				{
					CompositUnitSetup setup{};
					setup.nUnitType = UNIT_MONSTER;
					setup.nClassId = nClass;
					setup.bInventory = FALSE;

					check_class_code(setup, UNIT_MONSTER, nClass, nMode);
				}
			}

			for (const auto nClass : { -1, monstats_record_count, INT_MAX, INT_MIN })
			{
				CompositUnitSetup setup{};
				setup.nUnitType = UNIT_MONSTER;
				setup.bInventory = FALSE;

				check_class_code(setup, UNIT_MONSTER, nClass, MONMODE_NEUTRAL);
			}
		}

		SUBCASE("Objects and other unit types")
		{
			for (const int nUnitType : std::initializer_list<int>{ UNIT_OBJECT, UNIT_MISSILE, UNIT_ITEM, UNIT_TILE, 6, -1 })
			{
				CompositUnitSetup setup{};
				setup.nUnitType = nUnitType;
				setup.pRightHand = { 0 };

				check_class_code(setup, nUnitType, 0, 1);
			}
		}
	}

	TEST_CASE_FIXTURE(StatesTxtFixture<MonStatsTxtFixture<MonStats2TxtFixture<CharStatsTxtFixture<ItemTypesTxtFixture<ItemsTxtFixture<NoOpFixture>>>>>>, "D2Common.0x6FD47150 (#10888)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COMPOSIT_GetWeaponClassId, dll_base + 0x00007150);

		TransformStatesScope transform_states{};

		const auto check_class_id = [&](const CompositUnitSetup& setup, int nAnimMode, BOOL a5)
		{
			// Input data
			CompositUnitData moo_data{};
			int moo_pWeaponClassId = -1;
			CompositUnitData original_data{};
			int original_pWeaponClassId = -1;

			moo_data.Setup(setup);
			original_data.Setup(setup);

			// Call both implementations
			const auto moo_result = sut(&moo_data.pUnit, setup.bInventory ? &moo_data.pInventory : nullptr, &moo_pWeaponClassId, nAnimMode, a5);
			const auto original_result = original(&original_data.pUnit, setup.bInventory ? &original_data.pInventory : nullptr, &original_pWeaponClassId, nAnimMode, a5);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_data.pUnit, original_data.pUnit, "Comparing pUnit");
			MOO_CHECK_EQ(moo_data.pInventory, original_data.pInventory, "Comparing pInventory");
			MOO_CHECK_EQ(moo_pWeaponClassId, original_pWeaponClassId, "Comparing pWeaponClassId");
		};

		SUBCASE("pUnit = nullptr")
		{
			int moo_pWeaponClassId = -1;
			int original_pWeaponClassId = -1;

			const auto moo_result = sut(nullptr, nullptr, &moo_pWeaponClassId, -1, TRUE);
			const auto original_result = original(nullptr, nullptr, &original_pWeaponClassId, -1, TRUE);

			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			MOO_CHECK_EQ(moo_pWeaponClassId, original_pWeaponClassId, "Comparing pWeaponClassId");
		}

		SUBCASE("Players")
		{
			REPEAT_5();

			for (auto nClass = 0; nClass < NUMBER_OF_PLAYERCLASSES; ++nClass)
			{
				for (auto nMode = 0; nMode < NUMBER_OF_PLRMODES; ++nMode)
				{
					auto setup = random_player_setup(nClass, nMode);
					setup.nSeqMode = random_unsigned_integer(0, NUMBER_OF_PLRMODES - 1);
					setup.bAnimSeq = random_unsigned_integer(0, 1);

					check_class_id(setup, -1, random_unsigned_integer(0, 1));
					check_class_id(setup, random_unsigned_integer(0, NUMBER_OF_PLRMODES - 1), random_unsigned_integer(0, 1));
				}
			}
		}

		SUBCASE("Shapeshifted players")
		{
			for (const auto nTransformState : transform_states.transform_states)
			{
				for (auto nMode = 0; nMode < NUMBER_OF_PLRMODES; ++nMode)
				{
					auto setup = random_player_setup(PCLASS_DRUID, nMode);
					setup.nTransformState = nTransformState;

					for (auto a5 = 0; a5 < 2; ++a5)
					{
						check_class_id(setup, -1, a5);
						check_class_id(setup, (nMode + 1) % NUMBER_OF_PLRMODES, a5);
					}
				}
			}
		}

		SUBCASE("Monsters")
		{
			for (auto nClass = 0; nClass < monstats_record_count; ++nClass)
			{
				CompositUnitSetup setup{};
				setup.nUnitType = UNIT_MONSTER;
				setup.nClassId = nClass;
				setup.nAnimMode = random_unsigned_integer(0, NUMBER_OF_MONMODES - 1);
				setup.bInventory = FALSE;

				check_class_id(setup, -1, FALSE);
				check_class_id(setup, random_unsigned_integer(0, NUMBER_OF_MONMODES - 1), TRUE);
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD47200 (#10889)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COMPOSIT_IsArmorComponent, dll_base + 0x00007200);

		SUBCASE("")
		{
			for (auto nComponent = -2; nComponent < NUM_COMPONENTS + 2; ++nComponent)
			{
				// Call both implementations
				const auto moo_result = sut(nComponent);
				const auto original_result = original(nComponent);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}

			for (const auto nComponent : { INT_MAX, INT_MIN })
			{
				// Call both implementations
				const auto moo_result = sut(nComponent);
				const auto original_result = original(nComponent);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}
		}
	}

	TEST_CASE_FIXTURE(StatesTxtFixture<MonStatsTxtFixture<MonStats2TxtFixture<CharStatsTxtFixture<ItemTypesTxtFixture<ItemsTxtFixture<NoOpFixture>>>>>>, "D2Common.0x6FD47230 (#10890)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COMPOSIT_IsWeaponBowOrXBow, dll_base + 0x00007230);

		TransformStatesScope transform_states{};

		const auto check_bow = [&](CompositUnitSetup setup)
		{
			setup.bLinkInventory = TRUE;

			// Input data
			CompositUnitData moo_data{};
			CompositUnitData original_data{};

			moo_data.Setup(setup);
			original_data.Setup(setup);

			// Call both implementations
			const auto moo_result = sut(&moo_data.pUnit);
			const auto original_result = original(&original_data.pUnit);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_data.pUnit, original_data.pUnit, "Comparing pUnit");
		};

		SUBCASE("Players with a weapon in the right hand")
		{
			for (auto nClass = 0; nClass < NUMBER_OF_PLAYERCLASSES; ++nClass)
			{
				for (auto nItemId = 0; nItemId < get_weapons_record_count(); ++nItemId)
				{
					auto setup = random_player_setup(nClass, random_unsigned_integer(0, NUMBER_OF_PLRMODES - 1));
					setup.pRightHand = { nItemId };
					setup.bAnimSeq = random_unsigned_integer(0, 1);
					setup.nSeqMode = random_unsigned_integer(0, NUMBER_OF_PLRMODES - 1);

					check_bow(setup);
				}
			}
		}

		SUBCASE("Players with a weapon in the left hand")
		{
			for (auto nItemId = 0; nItemId < get_weapons_record_count(); ++nItemId)
			{
				auto setup = random_player_setup(PCLASS_AMAZON, PLRMODE_ATTACK1);
				setup.pRightHand = {};
				setup.pLeftHand = { nItemId };

				check_bow(setup);
			}
		}

		SUBCASE("Shapeshifted players")
		{
			for (const auto nTransformState : transform_states.transform_states)
			{
				for (auto nMode = 0; nMode < NUMBER_OF_PLRMODES; ++nMode)
				{
					auto setup = random_player_setup(PCLASS_DRUID, nMode);
					setup.nTransformState = nTransformState;

					check_bow(setup);
				}
			}
		}

		SUBCASE("Monsters")
		{
			for (auto nClass = 0; nClass < monstats_record_count; ++nClass)
			{
				CompositUnitSetup setup{};
				setup.nUnitType = UNIT_MONSTER;
				setup.nClassId = nClass;
				setup.nAnimMode = random_unsigned_integer(0, NUMBER_OF_MONMODES - 1);
				setup.bInventory = FALSE;

				check_bow(setup);
			}
		}
	}

	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD472E0 (#10891)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(COMPOSIT_GetArmorTypeFromComponent, dll_base + 0x000072E0);

		REPEAT_10();

		SUBCASE("")
		{
			// Components other than the armor ones halt the game
			for (const int nComponent : { COMPOSIT_TORSO, COMPOSIT_LEGS, COMPOSIT_RIGHTARM, COMPOSIT_LEFTARM, COMPOSIT_SPECIAL1, COMPOSIT_SPECIAL2 })
			{
				// Input data
				uint8_t moo_pArmorComponents[NUM_COMPONENTS]{};
				uint8_t original_pArmorComponents[NUM_COMPONENTS]{};

				for (auto i = 0; i < NUM_COMPONENTS; ++i)
				{
					const auto nArmorType = static_cast<uint8_t>(random_unsigned_integer(0, UINT8_MAX));
					moo_pArmorComponents[i] = nArmorType;
					original_pArmorComponents[i] = nArmorType;
				}

				// Call both implementations
				const auto moo_result = sut(nComponent, moo_pArmorComponents);
				const auto original_result = original(nComponent, original_pArmorComponents);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare potentially modified input data
				auto wrapped_moo_pArmorComponents = DynamicArray<uint8_t>{ moo_pArmorComponents, NUM_COMPONENTS };
				auto wrapped_original_pArmorComponents = DynamicArray<uint8_t>{ original_pArmorComponents, NUM_COMPONENTS };
				MOO_CHECK_EQ(wrapped_moo_pArmorComponents, wrapped_original_pArmorComponents, "Comparing pArmorComponents");
			}
		}
	}
}
#endif
