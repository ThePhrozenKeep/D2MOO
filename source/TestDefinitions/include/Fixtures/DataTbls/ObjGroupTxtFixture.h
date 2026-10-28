#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct ObjGroupTxtFixture : Fixture
{
	std::unique_ptr<D2ObjGroupTxt[]> objgroup_txt;
	int objgroup_record_count;

	ObjGroupTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, objgroup] = read_bin_file<D2ObjGroupTxt>(working_directory / "data/global/excel" / "ObjGroup.bin");

		sgptDataTables->nObjGroupTxtRecordCount = record_count;
		sgptDataTables->pObjGroupTxt = objgroup.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_objgroup = reinterpret_cast<D2ObjGroupTxt**>(d2common_base + 0x000A9608 + 0x000010A4);
		*original_objgroup = objgroup.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x000010A8);
		*original_record_count = record_count;

		objgroup_txt = std::move(objgroup);
		objgroup_record_count = record_count;
	};
};
