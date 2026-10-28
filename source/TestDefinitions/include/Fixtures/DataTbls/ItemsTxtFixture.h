#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct ItemsTxtFixture : Fixture
{
	std::unique_ptr<D2ItemsTxt[]> items_txt;
	int items_record_count;

	ItemsTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [weapons_record_count, weapons] = read_bin_file<D2ItemsTxt>(working_directory / "data/global/excel" / "Weapons.bin");
		auto [armor_record_count, armor] = read_bin_file<D2ItemsTxt>(working_directory / "data/global/excel" / "Armor.bin");
		auto [misc_record_count, misc] = read_bin_file<D2ItemsTxt>(working_directory / "data/global/excel" / "Misc.bin");

		const auto record_count = weapons_record_count + armor_record_count + misc_record_count;

		sgptDataTables->pItemDataTables.nWeaponsTxtRecordCount = weapons_record_count;
		sgptDataTables->pItemDataTables.nArmorTxtRecordCount = armor_record_count;
		sgptDataTables->pItemDataTables.nMiscTxtRecordCount = misc_record_count;

		auto items = std::make_unique<D2ItemsTxt[]>(record_count);

		sgptDataTables->pItemDataTables.pWeapons = items.get();
		sgptDataTables->pItemDataTables.pArmor = &sgptDataTables->pItemDataTables.pWeapons[weapons_record_count];
		sgptDataTables->pItemDataTables.pMisc = &sgptDataTables->pItemDataTables.pArmor[armor_record_count];

		std::memcpy(sgptDataTables->pItemDataTables.pWeapons, weapons.get(), sizeof(D2ItemsTxt) * weapons_record_count);
		std::memcpy(sgptDataTables->pItemDataTables.pArmor, armor.get(), sizeof(D2ItemsTxt)* armor_record_count);
		std::memcpy(sgptDataTables->pItemDataTables.pMisc, misc.get(), sizeof(D2ItemsTxt) * misc_record_count);

		sgptDataTables->pItemDataTables.nItemsTxtRecordCount = record_count;
		sgptDataTables->pItemDataTables.pItemsTxt = items.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_items = reinterpret_cast<D2ItemsTxt**>(d2common_base + 0x000A9608 + 0x00000CDC);
		*original_items = items.get();

		const auto original_items_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000CD8);
		*original_items_record_count = record_count;

		const auto original_weapons = reinterpret_cast<D2ItemsTxt**>(d2common_base + 0x000A9608 + 0x00000CE0);
		*original_weapons = sgptDataTables->pItemDataTables.pWeapons;

		const auto original_weapons_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000CE4);
		*original_weapons_record_count = record_count;

		const auto original_armor = reinterpret_cast<D2ItemsTxt**>(d2common_base + 0x000A9608 + 0x00000CE8);
		*original_armor = sgptDataTables->pItemDataTables.pArmor;

		const auto original_armor_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000CEC);
		*original_armor_record_count = record_count;

		const auto original_misc = reinterpret_cast<D2ItemsTxt**>(d2common_base + 0x000A9608 + 0x00000CF0);
		*original_misc = sgptDataTables->pItemDataTables.pMisc;

		const auto original_misc_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000CF4);
		*original_misc_record_count = record_count;

		items_txt = std::move(items);
		items_record_count = record_count;
	};
};
