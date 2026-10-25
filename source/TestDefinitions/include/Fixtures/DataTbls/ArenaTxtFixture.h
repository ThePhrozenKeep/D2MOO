#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct ArenaTxtFixture : Fixture
{
	std::unique_ptr<D2ArenaTxt[]> arena_txt;
	int arena_record_count;

	ArenaTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		auto [record_count, arena] = read_bin_file<D2ArenaTxt>(working_directory / "data/global/excel" / "Arena.bin");

		gpArenaTxtTable = arena.get();

		const auto original_arena = reinterpret_cast<D2ArenaTxt**>(d2common_base + 0x000A9600);
		*original_arena = arena.get();

		arena_txt = std::move(arena);
		arena_record_count = record_count;
	};
};
