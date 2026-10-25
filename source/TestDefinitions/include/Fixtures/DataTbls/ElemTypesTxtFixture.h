#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct ElemTypesTxtFixture : Fixture
{
	std::unique_ptr<D2ElemTypesTxt[]> elemtypes_txt;
	int elemtypes_record_count;

	ElemTypesTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, elemtypes] = read_bin_file<D2ElemTypesTxt>(working_directory / "data/global/excel" / "ElemTypes.bin");

		sgptDataTables->pElemTypesTxt = elemtypes.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_elemtypes = reinterpret_cast<D2ElemTypesTxt**>(d2common_base + 0x000A9608 + 0x00000018);
		*original_elemtypes = elemtypes.get();

		elemtypes_txt = std::move(elemtypes);
		elemtypes_record_count = record_count;
	};
};
