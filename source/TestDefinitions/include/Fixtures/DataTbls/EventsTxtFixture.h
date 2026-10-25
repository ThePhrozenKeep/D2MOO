#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct EventsTxtFixture : Fixture
{
	std::unique_ptr<D2EventsTxt[]> events_txt;
	int events_record_count;

	EventsTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, events] = read_bin_file<D2EventsTxt>(working_directory / "data/global/excel" / "Events.bin");

		sgptDataTables->pEventsTxt = events.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_events = reinterpret_cast<D2EventsTxt**>(d2common_base + 0x000A9608 + 0x00000074);
		*original_events = events.get();

		events_txt = std::move(events);
		events_record_count = record_count;
	};
};
