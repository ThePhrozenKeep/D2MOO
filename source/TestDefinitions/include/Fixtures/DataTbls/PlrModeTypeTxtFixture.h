#pragma once


#include <cstdarg>
#include <cstring>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


// PlrType.txt and PlrMode.txt are not shipped as compiled .bin files, so the records are built from the 1.10f tokens
template<class Fixture>
struct PlrModeTypeTxtFixture : Fixture
{
	std::unique_ptr<D2PlrModeTypeTxt[]> plrmodetype_txt;
	int plrtype_record_count;
	int plrmode_record_count;
	uintptr_t plrmodetype_d2common_base;

	PlrModeTypeTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));
		plrmodetype_d2common_base = d2common_base;

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		constexpr const char* plrtype[][2] = {
			{ "Amazon", "AM" }, { "Sorceress", "SO" }, { "Necromancer", "NE" }, { "Paladin", "PA" },
			{ "Barbarian", "BA" }, { "Druid", "DZ" }, { "Assassin", "AI" },
		};

		constexpr const char* plrmode[][2] = {
			{ "Death", "DT" }, { "Neutral", "NU" }, { "Walk", "WL" }, { "Run", "RN" }, { "Get Hit", "GH" },
			{ "Town Neutral", "TN" }, { "Town Walk", "TW" }, { "Attack1", "A1" }, { "Attack2", "A2" }, { "Block", "BL" },
			{ "Cast", "SC" }, { "Throw", "TH" }, { "Kick", "KK" }, { "Skill1", "S1" }, { "Skill2", "S2" },
			{ "Skill3", "S3" }, { "Skill4", "S4" }, { "Dead", "DD" }, { "Sequence", "GH" }, { "Knock back", "GH" },
		};

		const auto type_count = static_cast<int>(std::size(plrtype));
		const auto mode_count = static_cast<int>(std::size(plrmode));

		auto records = std::make_unique<D2PlrModeTypeTxt[]>(type_count + mode_count);
		std::memset(records.get(), 0, sizeof(D2PlrModeTypeTxt) * (type_count + mode_count));

		const auto fill_record = [](D2PlrModeTypeTxt& record, const char* name, const char* token)
		{
			strcpy_s(record.szName, sizeof(record.szName), name);
			std::memset(record.szToken, ' ', 4);
			std::memcpy(record.szToken, token, std::strlen(token));
		};

		for (auto i = 0; i < type_count; ++i)
		{
			fill_record(records[i], plrtype[i][0], plrtype[i][1]);
		}

		for (auto i = 0; i < mode_count; ++i)
		{
			fill_record(records[type_count + i], plrmode[i][0], plrmode[i][1]);
		}

		sgptDataTables->pPlrModeDataTables.nPlrModeTypeTxtRecordCount = type_count + mode_count;
		sgptDataTables->pPlrModeDataTables.pPlrModeTypeTxt = records.get();
		sgptDataTables->pPlrModeDataTables.pPlayerType = records.get();
		sgptDataTables->pPlrModeDataTables.pPlayerMode = &records[type_count];

		*original_sgptDataTables = sgptDataTables;

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x000010D4);
		*original_record_count = type_count + mode_count;

		const auto original_plrmodetype = reinterpret_cast<D2PlrModeTypeTxt**>(d2common_base + 0x000A9608 + 0x000010D8);
		*original_plrmodetype = records.get();

		const auto original_plrtype = reinterpret_cast<D2PlrModeTypeTxt**>(d2common_base + 0x000A9608 + 0x000010DC);
		*original_plrtype = records.get();

		const auto original_plrmode = reinterpret_cast<D2PlrModeTypeTxt**>(d2common_base + 0x000A9608 + 0x000010E0);
		*original_plrmode = &records[type_count];

		plrmodetype_txt = std::move(records);
		plrtype_record_count = type_count;
		plrmode_record_count = mode_count;
	};

	// The records are freed with the fixture, so don't leave dangling pointers behind for the following tests
	~PlrModeTypeTxtFixture()
	{
		sgptDataTables->pPlrModeDataTables.nPlrModeTypeTxtRecordCount = 0;
		sgptDataTables->pPlrModeDataTables.pPlrModeTypeTxt = nullptr;
		sgptDataTables->pPlrModeDataTables.pPlayerType = nullptr;
		sgptDataTables->pPlrModeDataTables.pPlayerMode = nullptr;

		*reinterpret_cast<int*>(plrmodetype_d2common_base + 0x000A9608 + 0x000010D4) = 0;
		*reinterpret_cast<D2PlrModeTypeTxt**>(plrmodetype_d2common_base + 0x000A9608 + 0x000010D8) = nullptr;
		*reinterpret_cast<D2PlrModeTypeTxt**>(plrmodetype_d2common_base + 0x000A9608 + 0x000010DC) = nullptr;
		*reinterpret_cast<D2PlrModeTypeTxt**>(plrmodetype_d2common_base + 0x000A9608 + 0x000010E0) = nullptr;
	}
};
