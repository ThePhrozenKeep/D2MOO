#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct SkillsTxtFixture : Fixture
{
	std::unique_ptr<D2SkillsTxt[]> skills_txt;
	int skills_record_count;
	std::unique_ptr<int[]> class_skill_counts;
	std::unique_ptr<short[]> class_skill_list;

	SkillsTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, skills] = read_bin_file<D2SkillsTxt>(working_directory / "data/global/excel" / "Skills.bin");

		class_skill_counts = std::make_unique<int[]>(7);
		std::memset(class_skill_counts.get(), 0, 7 * sizeof(int));

		sgptDataTables->nSkillsTxtRecordCount = record_count;
		sgptDataTables->pSkillsTxt = skills.get();
		sgptDataTables->nClassSkillCount = class_skill_counts.get();

		for (auto i = 0; i < record_count; ++i)
		{
			const auto nClass = skills[i].nCharClass;
			if (nClass >= 0 && nClass < 7)
			{
				++sgptDataTables->nClassSkillCount[nClass];
			}
		}

		auto nHighestClassSkillCount = 0;
		sgptDataTables->nHighestClassSkillCount = 0;

		for (auto i = 0; i < 7; ++i)
		{
			if (sgptDataTables->nClassSkillCount[i] > nHighestClassSkillCount)
			{
				nHighestClassSkillCount = sgptDataTables->nClassSkillCount[i];
			}
		}

		sgptDataTables->nHighestClassSkillCount = nHighestClassSkillCount;

		std::memset(class_skill_counts.get(), 0, 7 * sizeof(int));

		class_skill_list = std::make_unique<short[]>(7 * nHighestClassSkillCount);
		std::memset(class_skill_list.get(), 0, 7 * nHighestClassSkillCount * sizeof(short));
		sgptDataTables->nClassSkillList = class_skill_list.get();

		for (auto i = 0; i < record_count; ++i)
		{
			const auto nClass = skills[i].nCharClass;
			if (nClass >= 0 && nClass < 7)
			{
				sgptDataTables->nClassSkillList[sgptDataTables->nClassSkillCount[nClass] + nHighestClassSkillCount * nClass] = i;
				++sgptDataTables->nClassSkillCount[nClass];
			}
		}

		*original_sgptDataTables = sgptDataTables;

		const auto original_skills = reinterpret_cast<D2SkillsTxt**>(d2common_base + 0x000A9608 + 0x00000B98);
		*original_skills = skills.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000BA0);
		*original_record_count = record_count;

		skills_txt = std::move(skills);
		skills_record_count = record_count;
	};
};
