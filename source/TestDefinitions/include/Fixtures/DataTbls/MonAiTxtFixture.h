#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct MonAiTxtFixture : Fixture
{
	std::unique_ptr<D2MonAiTxt[]> monai_txt;
	int monai_record_count;

	MonAiTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, monai] = read_bin_file<D2MonAiTxt>(working_directory / "data/global/excel" / "MonAi.bin");

		sgptDataTables->nMonAiTxtRecordCount = record_count;
		sgptDataTables->pMonAiTxt = monai.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_monai = reinterpret_cast<D2MonAiTxt**>(d2common_base + 0x000A9608 + 0x00000088);
		*original_monai = monai.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000090);
		*original_record_count = record_count;

		monai_txt = std::move(monai);
		monai_record_count = record_count;
	};
};
