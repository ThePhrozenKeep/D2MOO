#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct UniqueSuffixTxtFixture : Fixture
{
	std::unique_ptr<D2UniqueSuffixTxt[]> uniquesuffix_txt;
	int uniquesuffix_record_count;

	UniqueSuffixTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, uniquesuffix] = read_bin_file<D2UniqueSuffixTxt>(working_directory / "data/global/excel" / "UniqueSuffix.bin");

		sgptDataTables->nUniqueSuffixTxtRecordCount = record_count;
		sgptDataTables->pUniqueSuffixTxt = uniquesuffix.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_uniquesuffix = reinterpret_cast<D2UniqueSuffixTxt**>(d2common_base + 0x000A9608 + 0x00001070);
		*original_uniquesuffix = uniquesuffix.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00001080);
		*original_record_count = record_count;

		uniquesuffix_txt = std::move(uniquesuffix);
		uniquesuffix_record_count = record_count;
	};
};
