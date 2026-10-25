#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct NpcTxtFixture : Fixture
{
	std::unique_ptr<D2NpcTxt[]> npc_txt;
	int npc_record_count;

	NpcTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, npc] = read_bin_file<D2NpcTxt>(working_directory / "data/global/excel" / "Npc.bin");

		sgptDataTables->nNpcTxtRecordCount = record_count;
		sgptDataTables->pNpcTxt = npc.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_npc = reinterpret_cast<D2NpcTxt**>(d2common_base + 0x000A9608 + 0x000009A8);
		*original_npc = npc.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x000009AC);
		*original_record_count = record_count;

		npc_txt = std::move(npc);
		npc_record_count = record_count;
	};
};
