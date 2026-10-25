#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct ArmTypeTxtFixture : Fixture
{
	std::unique_ptr<D2ArmTypeTxt[]> armtype_txt;
	int armtype_record_count;

	ArmTypeTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, armtype] = read_bin_file<D2ArmTypeTxt>(working_directory / "data/global/excel" / "ArmType.bin");

		sgptDataTables->pArmTypeTxt = armtype.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_armtype = reinterpret_cast<D2ArmTypeTxt**>(d2common_base + 0x000A9608 + 0x000010AC);
		*original_armtype = armtype.get();

		armtype_txt = std::move(armtype);
		armtype_record_count = record_count;
	};
};
