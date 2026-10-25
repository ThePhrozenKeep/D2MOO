#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct BeltsTxtFixture : Fixture
{
	std::unique_ptr<D2BeltsTxt[]> belts_txt;
	int belts_record_count;

	BeltsTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		auto [record_count, belts] = read_bin_file<D2BeltsTxt>(working_directory / "data/global/excel" / "Belts.bin");

		gpBeltsTxtTable = belts.get();

		const auto original_belts = reinterpret_cast<D2BeltsTxt**>(d2common_base + 0x000A9604);
		*original_belts = belts.get();

		belts_txt = std::move(belts);
		belts_record_count = record_count;
	};
};
