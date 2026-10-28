#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct ItemTypesTxtFixture : Fixture
{
	std::unique_ptr<D2ItemTypesTxt[]> itemtypes_txt;
	int itemtypes_record_count;
	std::unique_ptr<uint32_t[]> itemtypes_equivalent_luts;

	ItemTypesTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, itemtypes] = read_bin_file<D2ItemTypesTxt>(working_directory / "data/global/excel" / "ItemTypes.bin");

		sgptDataTables->nItemTypesTxtRecordCount = record_count;
		sgptDataTables->pItemTypesTxt = itemtypes.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_itemtypes = reinterpret_cast<D2ItemTypesTxt**>(d2common_base + 0x000A9608 + 0x00000BF8);
		*original_itemtypes = itemtypes.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000BFC);
		*original_record_count = record_count;

		itemtypes_txt = std::move(itemtypes);
		itemtypes_record_count = record_count;

		sgptDataTables->nItemTypesIndex = (record_count + 31) / 32;

		itemtypes_equivalent_luts = std::make_unique<uint32_t[]>(record_count * ((record_count + 31) / 32));
		std::memset(itemtypes_equivalent_luts.get(), 0, sizeof(uint32_t) * record_count * ((record_count + 31) / 32));
		sgptDataTables->pItemTypesEquivalenceLUTs = itemtypes_equivalent_luts.get();

		for (int i = 0; i < record_count; ++i)
		{
			auto* pItemTypesNest = &itemtypes_equivalent_luts[sgptDataTables->nItemTypesIndex * i];

			for (int j = 0; j < record_count; ++j)
			{
				if (DATATBLS_CheckItemTypesEquivalenceNested(i, j))
				{
					pItemTypesNest[j >> 5] |= gdwBitMasks[j & 31];
				}
			}
		}
	};
};
