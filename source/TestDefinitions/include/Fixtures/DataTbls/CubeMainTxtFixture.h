#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct CubeMainTxtFixture : Fixture
{
	std::unique_ptr<D2CubeMainTxt[]> cubemain_txt;
	int cubemain_record_count;

	CubeMainTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, cubemain] = read_bin_file<D2CubeMainTxt>(working_directory / "data/global/excel" / "CubeMain.bin");

		sgptDataTables->nCubeMainTxtRecordCount = record_count;
		sgptDataTables->pCubeMainTxt = cubemain.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_cubemain = reinterpret_cast<D2CubeMainTxt**>(d2common_base + 0x000A9608 + 0x00000CC4);
		*original_cubemain = cubemain.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000CC8);
		*original_record_count = record_count;

		cubemain_txt = std::move(cubemain);
		cubemain_record_count = record_count;
	};
};
