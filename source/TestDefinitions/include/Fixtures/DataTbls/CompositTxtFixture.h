#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct CompositTxtFixture : Fixture
{
	std::unique_ptr<D2CompositTxt[]> composit_txt;
	int composit_record_count;

	CompositTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, composit] = read_bin_file<D2CompositTxt>(working_directory / "data/global/excel" / "Composit.bin");

		sgptDataTables->pCompositTxt = composit.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_composit = reinterpret_cast<D2CompositTxt**>(d2common_base + 0x000A9608 + 0x000010C0);
		*original_composit = composit.get();

		composit_txt = std::move(composit);
		composit_record_count = record_count;
	};
};
