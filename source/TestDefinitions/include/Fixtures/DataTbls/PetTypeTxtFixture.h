#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct PetTypeTxtFixture : Fixture
{
	std::unique_ptr<D2PetTypeTxt[]> pettype_txt;
	int pettype_record_count;

	PetTypeTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, pettype] = read_bin_file<D2PetTypeTxt>(working_directory / "data/global/excel" / "PetType.bin");

		sgptDataTables->nPetTypeTxtRecordCount = record_count;
		sgptDataTables->pPetTypeTxt = pettype.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_pettype = reinterpret_cast<D2PetTypeTxt**>(d2common_base + 0x000A9608 + 0x00000BE8);
		*original_pettype = pettype.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000BF0);
		*original_record_count = record_count;

		pettype_txt = std::move(pettype);
		pettype_record_count = record_count;
	};
};
