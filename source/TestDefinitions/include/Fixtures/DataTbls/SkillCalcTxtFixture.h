#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct SkillCalcTxtFixture : Fixture
{
	std::unique_ptr<D2SkillCalcTxt[]> skillcalc_txt;
	int skillcalc_record_count;

	SkillCalcTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, skillcalc] = read_bin_file<D2SkillCalcTxt>(working_directory / "data/global/excel" / "SkillCalc.bin");

		sgptDataTables->pSkillCalcTxt = skillcalc.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_skillcalc = reinterpret_cast<D2SkillCalcTxt**>(d2common_base + 0x000A9608 + 0x00000038);
		*original_skillcalc = skillcalc.get();

		skillcalc_txt = std::move(skillcalc);
		skillcalc_record_count = record_count;
	};
};
