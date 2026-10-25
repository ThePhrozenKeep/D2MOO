#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct ExperienceTxtFixture : Fixture
{
	std::unique_ptr<D2ExperienceTxt[]> experience_txt;
	int experience_record_count;

	ExperienceTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, experience] = read_bin_file<D2ExperienceTxt>(working_directory / "data/global/excel" / "Experience.bin");

		sgptDataTables->pExperienceTxt = (D2ExperienceDataTbl*)experience.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_events = reinterpret_cast<D2ExperienceTxt**>(d2common_base + 0x000A9608 + 0x00000C78);
		*original_events = experience.get();

		experience_txt = std::move(experience);
		experience_record_count = record_count;
	};
};
