#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct PropertiesTxtFixture : Fixture
{
	std::unique_ptr<D2PropertiesTxt[]> properties_txt;
	int properties_record_count;

	PropertiesTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, properties] = read_bin_file<D2PropertiesTxt>(working_directory / "data/global/excel" / "Properties.bin");

		sgptDataTables->nPropertiesTxtRecordCount = record_count;
		sgptDataTables->pPropertiesTxt = properties.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_properties = reinterpret_cast<D2PropertiesTxt**>(d2common_base + 0x000A9608 + 0x000000A4);
		*original_properties = properties.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x000000AC);
		*original_record_count = record_count;

		properties_txt = std::move(properties);
		properties_record_count = record_count;
	};
};
