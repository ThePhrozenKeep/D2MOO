#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct MonSeqTxtFixture : Fixture
{
	std::unique_ptr<D2AnimSeqTxt[]> monseq_txt;
	int monseq_record_count;

	MonSeqTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, monseq] = read_bin_file<D2AnimSeqTxt>(working_directory / "data/global/excel" / "MonSeq.bin");

		sgptDataTables->nMonSeqTxtRecordCount = record_count;
		sgptDataTables->pMonSeqTxt = monseq.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_monseq = reinterpret_cast<D2AnimSeqTxt**>(d2common_base + 0x000A9608 + 0x0000B78);
		*original_monseq = monseq.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000B80);
		*original_record_count = record_count;

		monseq_txt = std::move(monseq);
		monseq_record_count = record_count;
	};
};
