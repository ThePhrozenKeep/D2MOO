#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct CompCodeTxtFixture : Fixture
{
	std::unique_ptr<D2CompCodeTxt[]> compcode_txt;
	int compcode_record_count;

	CompCodeTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, compcode] = read_bin_file<D2CompCodeTxt>(working_directory / "data/global/excel" / "CompCode.bin");

		sgptDataTables->nCompCodeTxtRecordCount = record_count;
		sgptDataTables->pCompCodeTxt = compcode.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_compcode = reinterpret_cast<D2CompCodeTxt**>(d2common_base + 0x000A9608 + 0x0000007C);
		*original_compcode = compcode.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000084);
		*original_record_count = record_count;

		compcode_txt = std::move(compcode);
		compcode_record_count = record_count;
	};
};
