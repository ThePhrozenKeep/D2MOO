#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct OverlayTxtFixture : Fixture
{
	std::unique_ptr<D2OverlayTxt[]> overlay_txt;
	int overlay_record_count;

	OverlayTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, overlay] = read_bin_file<D2OverlayTxt>(working_directory / "data/global/excel" / "Overlay.bin");

		sgptDataTables->nOverlayTxtRecordCount = record_count;
		sgptDataTables->pOverlayTxt = overlay.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_overlay = reinterpret_cast<D2OverlayTxt**>(d2common_base + 0x000A9608 + 0x00000BBC);
		*original_overlay = overlay.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000BC0);
		*original_record_count = record_count;

		overlay_txt = std::move(overlay);
		overlay_record_count = record_count;
	};
};
