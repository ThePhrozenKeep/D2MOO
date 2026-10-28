#pragma once


#include <cstdarg>
#include <memory>

#include <Windows.h>

#include <D2DataTbls.h>
#include <DataTbls/AnimTbls.h>


// Provides an anim data table without any records (like an empty AnimData.d2),
// so that every lookup resolves to the default record of the table
template<class Fixture>
struct AnimDataFixture : Fixture
{
	std::unique_ptr<D2AnimDataTableStrc> anim_data;
	std::unique_ptr<D2AnimDataBucketStrc> anim_data_empty_bucket;

	AnimDataFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		anim_data_empty_bucket = std::make_unique<D2AnimDataBucketStrc>();
		anim_data_empty_bucket->nbEntries = 0;

		anim_data = std::make_unique<D2AnimDataTableStrc>();
		for (auto& pBucket : anim_data->pHashTableBucket)
		{
			pBucket = anim_data_empty_bucket.get();
		}

		// Same values as in DATATBLS_LoadAnimDataD2
		anim_data->tDefaultRecord.dwFrames = 2048;
		anim_data->tDefaultRecord.dwAnimSpeed = 256;

		sgptDataTables->pAnimData = anim_data.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_anim_data = reinterpret_cast<D2AnimDataTableStrc**>(d2common_base + 0x000A9608 + 0x00000C74);
		*original_anim_data = anim_data.get();
	};
};
