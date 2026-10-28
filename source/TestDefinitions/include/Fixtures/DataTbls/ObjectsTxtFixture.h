#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct ObjectsTxtFixture : Fixture
{
	std::unique_ptr<D2ObjectsTxt[]> objects_txt;
	int objects_record_count;

	ObjectsTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, objects] = read_bin_file<D2ObjectsTxt>(working_directory / "data/global/excel" / "Objects.bin");

		sgptDataTables->nObjectsTxtRecordCount = record_count;
		sgptDataTables->pObjectsTxt = objects.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_objects = reinterpret_cast<D2ObjectsTxt**>(d2common_base + 0x000A9608 + 0x0000109C);
		*original_objects = objects.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x000010A0);
		*original_record_count = record_count;

		objects_txt = std::move(objects);
		objects_record_count = record_count;
	};
};
