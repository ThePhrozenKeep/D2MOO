#pragma once


#include <cstdarg>
#include <cstdint>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct StatesTxtFixture : Fixture
{
	std::unique_ptr<D2StatesTxt[]> states_txt;
	int states_record_count;
	std::unique_ptr<uint32_t[]> state_masks;

	StatesTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, states] = read_bin_file<D2StatesTxt>(working_directory / "data/global/excel" / "States.bin");

		sgptDataTables->nStatesTxtRecordCount = record_count;
		sgptDataTables->pStatesTxt = states.get();

		state_masks = std::make_unique<uint32_t[]>(std::size(sgptDataTables->fStateMasks) * ((sgptDataTables->nStatesTxtRecordCount + 31) / 32));
		sgptDataTables->pStateMasks = state_masks.get();

		for (int i = 0; i < std::size(sgptDataTables->fStateMasks); ++i)
		{
			const auto data = &sgptDataTables->pStateMasks[((sgptDataTables->nStatesTxtRecordCount + 31) / 32) * i];
			sgptDataTables->fStateMasks[i] = data;

			for (int j = 0; j < sgptDataTables->nStatesTxtRecordCount; ++j)
			{
				if (sgptDataTables->pStatesTxt[j].nStateFlags[i >> 3] & gdwBitMasks[i & 7])
				{
					data[j >> 5] |= gdwBitMasks[j & 31];
				}
			}
		}

		*original_sgptDataTables = sgptDataTables;

		const auto original_states = reinterpret_cast<D2StatesTxt**>(d2common_base + 0x000A9608 + 0x000000BC);
		*original_states = states.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x000000C4);
		*original_record_count = record_count;

		states_txt = std::move(states);
		states_record_count = record_count;
	};
};
