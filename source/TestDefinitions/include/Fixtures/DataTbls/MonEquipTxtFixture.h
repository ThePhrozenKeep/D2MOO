#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct MonEquipTxtFixture : Fixture
{
	std::unique_ptr<D2MonEquipTxt[]> monequip_txt;
	int monequip_record_count;

	MonEquipTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, monequip] = read_bin_file<D2MonEquipTxt>(working_directory / "data/global/excel" / "MonEquip.bin");

		sgptDataTables->nMonEquipTxtRecordCount = record_count;
		sgptDataTables->pMonEquipTxt = monequip.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_monequip = reinterpret_cast<D2MonEquipTxt**>(d2common_base + 0x000A9608 + 0x00000BE0);
		*original_monequip = monequip.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000BE4);
		*original_record_count = record_count;

		monequip_txt = std::move(monequip);
		monequip_record_count = record_count;
	};
};
