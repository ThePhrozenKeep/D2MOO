#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct SkillDescTxtFixture : Fixture
{
	std::unique_ptr<D2SkillDescTxt[]> skilldesc_txt;
	int skilldesc_record_count;

	SkillDescTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, skilldesc] = read_bin_file<D2SkillDescTxt>(working_directory / "data/global/excel" / "SkillDesc.bin");

		sgptDataTables->nSkillDescTxtRecordCount = record_count;
		sgptDataTables->pSkillDescTxt = skilldesc.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_skilldesc = reinterpret_cast<D2SkillDescTxt**>(d2common_base + 0x000A9608 + 0x00000B8C);
		*original_skilldesc = skilldesc.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000B94);
		*original_record_count = record_count;

		skilldesc_txt = std::move(skilldesc);
		skilldesc_record_count = record_count;
	};
};
