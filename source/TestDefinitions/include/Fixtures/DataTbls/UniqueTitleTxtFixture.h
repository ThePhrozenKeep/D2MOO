#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct UniqueTitleTxtFixture : Fixture
{
	std::unique_ptr<D2UniqueTitleTxt[]> uniquetitle_txt;
	int uniquetitle_record_count;

	UniqueTitleTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, uniquetitle] = read_bin_file<D2UniqueTitleTxt>(working_directory / "data/global/excel" / "UniqueTitle.bin");

		sgptDataTables->nUniqueTitleTxtRecordCount = record_count;
		sgptDataTables->pUniqueTitleTxt = uniquetitle.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_uniquetitle = reinterpret_cast<D2UniqueTitleTxt**>(d2common_base + 0x000A9608 + 0x00001068);
		*original_uniquetitle = uniquetitle.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00001078);
		*original_record_count = record_count;

		uniquetitle_txt = std::move(uniquetitle);
		uniquetitle_record_count = record_count;
	};
};
