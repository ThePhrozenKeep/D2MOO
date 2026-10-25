#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct UniquePrefixTxtFixture : Fixture
{
	std::unique_ptr<D2UniquePrefixTxt[]> uniqueprefix_txt;
	int uniqueprefix_record_count;

	UniquePrefixTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, uniqueprefix] = read_bin_file<D2UniquePrefixTxt>(working_directory / "data/global/excel" / "UniquePrefix.bin");

		sgptDataTables->nUniquePrefixTxtRecordCount = record_count;
		sgptDataTables->pUniquePrefixTxt = uniqueprefix.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_uniqueprefix = reinterpret_cast<D2UniquePrefixTxt**>(d2common_base + 0x000A9608 + 0x0000106C);
		*original_uniqueprefix = uniqueprefix.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x0000107C);
		*original_record_count = record_count;

		uniqueprefix_txt = std::move(uniqueprefix);
		uniqueprefix_record_count = record_count;
	};
};
