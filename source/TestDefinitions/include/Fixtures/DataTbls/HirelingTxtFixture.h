#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>
#include <DataTbls/MonsterTbls.h>

static uint16_t fake_name_id(char* name)
{
	static std::unordered_map<std::string, uint16_t> name_to_id_map{
		{ "merc01", 1 },
		{ "merc41", 41 },
		{ "merca201", 201 },
		{ "merca221", 221 },
		{ "merca222", 222 },
		{ "merca241", 241 },
		{ "MercX101", 101 },
		{ "MercX167", 167 },
	};

	return name_to_id_map[name];
}

template<class Fixture>
struct HirelingTxtFixture : Fixture
{
	std::unique_ptr<D2HirelingTxt[]> hireling_txt;
	int hireling_record_count;

	HirelingTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, hireling] = read_bin_file<D2HirelingTxt>(working_directory / "data/global/excel" / "Hireling.bin");

		for (auto i = 0; i < record_count; ++i)
		{
			hireling[i].wNameFirst = fake_name_id(hireling[i].szNameFirst);
			hireling[i].wNameLast = fake_name_id(hireling[i].szNameLast);
		}

		sgptDataTables->nHirelingTxtRecordCount = record_count;
		sgptDataTables->pHirelingTxt = hireling.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_hireling = reinterpret_cast<D2HirelingTxt**>(d2common_base + 0x000A9608 + 0x000001A0);
		*original_hireling = hireling.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x000001A4);
		*original_record_count = record_count;

		hireling_txt = std::move(hireling);
		hireling_record_count = record_count;
	};
};
